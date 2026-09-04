#include "Math/Boost.h"
#include "Math/RotationX.h"
#include "Math/RotationY.h"
#include "Math/Vector4D.h"

using FourVec = ROOT::Math::PxPyPzEVector;

constexpr const double mp = 0.9382720813;
constexpr const double me = 0.00051099895;

struct UndoAfterburnerTransform
{
    ROOT::Math::Boost boostToCM;
    ROOT::Math::RotationY rotY;
    ROOT::Math::RotationX rotX;
    ROOT::Math::Boost boostToHeadOn;

    FourVec eBeamNominal;
    FourVec pBeamNominal;
};


UndoAfterburnerTransform BuildUndoAfterburnerTransform(
    double crossingAngle = -0.025,
    double yAngle = 100e-6,
    double Ee = 9.0,
    double Ep = 130.0)
{
    UndoAfterburnerTransform t;

    double pe =
        std::sqrt(Ee*Ee - me*me);

    double pp =
        std::sqrt(Ep*Ep - mp*mp);

    t.eBeamNominal =
        FourVec(
            0.0,
            0.0,
           -pe,
            Ee);

    FourVec hBeamCrossing(
        crossingAngle * Ep,
        yAngle * Ep,
        pp,
        Ep);

    t.pBeamNominal =
        FourVec(
            0.0,
            0.0,
            pp,
            Ep);

    FourVec cmFrame =
        t.eBeamNominal +
        hBeamCrossing;

    ROOT::Math::XYZVector beta(
        -cmFrame.Px()/cmFrame.E(),
        -cmFrame.Py()/cmFrame.E(),
        -cmFrame.Pz()/cmFrame.E());

    t.boostToCM =
        ROOT::Math::Boost(beta);

    auto hBeamCM =
        t.boostToCM(hBeamCrossing);

    double rotYAngle =
        -std::atan2(
            hBeamCM.Px(),
            hBeamCM.Pz());

    double rotXAngle =
         std::atan2(
            hBeamCM.Py(),
            hBeamCM.Pz());

    t.rotY =
        ROOT::Math::RotationY(rotYAngle);

    t.rotX =
        ROOT::Math::RotationX(rotXAngle);

    FourVec headOnFrame(
        0.0,
        0.0,
        cmFrame.Pz(),
        cmFrame.E());

    t.boostToHeadOn =
        ROOT::Math::Boost(
            headOnFrame.Px()/headOnFrame.E(),
            headOnFrame.Py()/headOnFrame.E(),
            headOnFrame.Pz()/headOnFrame.E());

    return t;
}


FourVec UndoAfterburner(
    const FourVec& p,
    const UndoAfterburnerTransform& t)
{
    FourVec out = p;

    out = t.boostToCM(out);
    out = t.rotY(out);
    out = t.rotX(out);
    out = t.boostToHeadOn(out);

    return out;
}


enum class FrameMode { Raw, HeadOnNominal };


struct TCSEvent
{
    enum ParticleIndex
    {
        kEBeam = 0,
        kScatEle,
        kVirtPhot,
        kPBeam,
        kGPrime,
        kPPrime,
        kEle,
        kPos,
        kNParticles
    };

    static constexpr const char* ParticleNames[kNParticles] =
    {
        "eBeam",
        "scatele",
        "phot",
        "pBeam",
        "gprime",
        "pprime",
        "ele",
        "pos"
    };  
  
  std::vector<FourVec> particles;
  
  FourVec& eBeam()     { return particles[kEBeam]; }
  FourVec& scatele()     { return particles[kScatEle]; }
  FourVec& phot()   { return particles[kVirtPhot]; }
  FourVec& pBeam()     { return particles[kPBeam]; }
  FourVec& gprime() { return particles[kGPrime]; }
  FourVec& pprime()     { return particles[kPPrime]; }
  FourVec& ele()    { return particles[kEle]; }
  FourVec& pos()     { return particles[kPos]; }
  
  const FourVec& eBeam() const     { return particles[kEBeam]; }
  const FourVec& scatele() const     { return particles[kScatEle]; }
  const FourVec& phot() const   { return particles[kVirtPhot]; }
  const FourVec& pBeam() const     { return particles[kPBeam]; }
  const FourVec& gprime() const { return particles[kGPrime]; }
  const FourVec& pprime() const     { return particles[kPPrime]; }
  const FourVec& ele() const    { return particles[kEle]; }
  const FourVec& pos() const     { return particles[kPos]; }
  
  FourVec eBeamPhysics;
  FourVec pBeamPhysics;
  
  double Q2;
  double Qp2;
  double t;
  double xB;
  double W;
  double MM2;
  
  double Mee;
  double cosTheta;
  double phi;

  double epsilon;
  double circPol;
};


inline void CalculateVirtualPhotonPolarisation(
    TCSEvent& evt)
{
    // Boost into target (proton) rest frame

    ROOT::Math::Boost boostToTargetRF(
        evt.pBeamPhysics.BoostToCM());

    FourVec eBeamRF =
        boostToTargetRF(
            evt.eBeamPhysics);

    FourVec eScatRF =
        boostToTargetRF(evt.scatele());

    FourVec gammaRF =
        eBeamRF - eScatRF;

    // Electron scattering angle in target rest frame

    double num =
        eBeamRF.Px()*eScatRF.Px()
      + eBeamRF.Py()*eScatRF.Py()
      + eBeamRF.Pz()*eScatRF.Pz();

    double denom =
        eBeamRF.P() *
        eScatRF.P();

    double theta_e = 0.0;

    if(denom > 0.0)
    {
        double ratio =
            std::clamp(
                num/denom,
                -1.0,
                1.0);

        theta_e =
            std::acos(ratio);
    }

    // Virtual photon energy in target rest frame

    double gammaE =
        gammaRF.E();

    double Q2 =
        -gammaRF.M2();

    // Virtual photon linear polarization epsilon

    evt.epsilon = 0.0;

    if(Q2 > 0.0)
    {
        double tanHalfTheta =
            std::tan(theta_e/2.0);

        evt.epsilon =
            1.0 /
            (
                1.0
                +
                2.0
                *
                (
                    1.0 +
                    gammaE*gammaE/Q2
                )
                *
                tanHalfTheta*tanHalfTheta
            );
    }

    // Circular polarization

    evt.circPol =
        std::sqrt(
            std::max(
                0.0,
                1.0
                -
                evt.epsilon*evt.epsilon));
}


inline void CalculatePhotoHelicityDecayAngles(
    TCSEvent& evt)
{
    using XYZ = ROOT::Math::XYZVector;

    // e+e- pair
    FourVec pair =
        evt.ele() + evt.pos();

    // Boost into pair rest frame
    ROOT::Math::Boost boostToPairCM(
        pair.BoostToCM());

    FourVec pprimeCM =
        boostToPairCM(evt.pprime());

    FourVec photCM =
        boostToPairCM(evt.phot());

    FourVec eleCM =
        boostToPairCM(evt.ele());

    // Helicity frame axes

    XYZ zV =
        -pprimeCM.Vect().Unit();

    XYZ yV =
        pprimeCM.Vect()
        .Cross(
            photCM.Vect())
        .Unit();

    XYZ xV =
        yV.Cross(zV).Unit();

    // Electron momentum projected onto helicity axes

    XYZ proj(
        eleCM.Vect().Dot(xV),
        eleCM.Vect().Dot(yV),
        eleCM.Vect().Dot(zV));

    evt.cosTheta =
        proj.Z()/proj.R();

    evt.phi =
        proj.Phi();
}


TCSEvent BuildTCSEvent(HepMC3::GenEventData* evt,
			 FrameMode framemode)
{
  TCSEvent out;
  out.particles.resize(8);
    
  auto& p = evt->particles;
    
  out.eBeam() =
    {p[0].momentum.px(),
     p[0].momentum.py(),
     p[0].momentum.pz(),
     p[0].momentum.e()};

  out.scatele() =
    {p[1].momentum.px(),
     p[1].momentum.py(),
     p[1].momentum.pz(),
     p[1].momentum.e()};

  out.pBeam() =
    {p[3].momentum.px(),
     p[3].momentum.py(),
     p[3].momentum.pz(),
     p[3].momentum.e()};
    
  out.phot() =
    {p[4].momentum.px(),
     p[4].momentum.py(),
     p[4].momentum.pz(),
     p[4].momentum.e()};

  out.pprime() =
    {p[5].momentum.px(),
     p[5].momentum.py(),
     p[5].momentum.pz(),
     p[5].momentum.e()};
    
  out.ele() =
    {p[6].momentum.px(),
     p[6].momentum.py(),
     p[6].momentum.pz(),
     p[6].momentum.e()};
    
  out.pos() =
    {p[7].momentum.px(),
     p[7].momentum.py(),
     p[7].momentum.pz(),
     p[7].momentum.e()};


  //Handle Beams for different frames
  
  if(framemode == FrameMode::HeadOnNominal)
    {
      static auto transform =
	BuildUndoAfterburnerTransform();
      for(auto& parts : out.particles)
	{
	  parts = UndoAfterburner(
			      parts,
			      transform);
	}
      out.eBeamPhysics =
	transform.eBeamNominal;
      out.pBeamPhysics =
	transform.pBeamNominal;
    }
  else
    {
      out.eBeamPhysics = out.eBeam();
      out.pBeamPhysics = out.pBeam();
    }
  
  //Perform kinematic calculations
  auto q = out.eBeamPhysics - out.scatele();
  
  out.Q2 = -q.M2();


  out.t = (out.pBeamPhysics - out.pprime()).M2();

  out.xB = out.Q2 /(2.0*out.pBeamPhysics.Dot(q));

  double W2 = (out.pBeamPhysics + q).M2();

  out.W = W2 > 0.0 ? sqrt(W2) : 0.0;

  auto pair = out.ele() + out.pos();
  
  out.Qp2 = pair.M2();

  out.Mee = pair.M();

  auto miss =
    out.eBeamPhysics +
    out.pBeamPhysics -
    out.scatele() -
    out.ele() -
    out.pos() -
    out.pprime();

  out.MM2 =
    miss.M2();
  
  CalculateVirtualPhotonPolarisation(out);
  CalculatePhotoHelicityDecayAngles(out);
  return out;
}

bool PassCuts(const TCSEvent& evt){

  if(fabs(evt.cosTheta)>1.0) return false;

  return true;
}
