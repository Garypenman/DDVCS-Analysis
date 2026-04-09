#include "AnalysisManager.h"
#include "HepMCElectro.h"
#include "BasicKinematicsRDF.h"
#include "KinematicsProcElectro.h"
#include "ElectronScatterKinematics.h"
#include "gammaN_2_Spin0Spin0SpinHalf.h"
#include "DefineNames.h"

#include "TCSKinematics.h"

using namespace rad::consts;

using ROOT::RVec;
using ROOT::Math::PxPyPzMVector;


template <typename Processor>
auto TCS_Exclusive_Topology = [](Processor& p) {
  p.Creator().Sum("gprime", {{"ele", "pos"}});       
  p.Creator().Diff("miss",    {{BeamEle(),BeamIon()},{ScatEle(), "gprime", "pprime"}});
  p.Creator().Diff("miss",    {{BeamEle(),BeamIon()},{ScatEle(), "gprime", "pprime"}});
  p.Creator().Diff("miss_hadro",    {{BeamEle(),BeamIon()},{ScatEle()}});
  p.Creator().Diff("miss_pprime",    {{BeamEle(),BeamIon()},{ScatEle(), "gprime"}});
  p.Creator().Diff("delta",{{BeamIon()},{"pprime"}});
  
  p.SetMesonParticles({"ele","pos"});
  p.SetBaryonParticles({"pprime"});
  //p.SetBaryonParticles({"Miss"});
  
  // 2. Calculate kinematic variables
  p.Q2();         
  p.xbj();
  p.y();         
  p.nu();
  p.tau();
  p.tauprime();

  p.RegisterCalc("GammaPol",rad::physics::ElS_PolVirtPhot);
  p.RegisterCalc("GammaPolCirc",rad::physics::ElS_CircPolVirtPhot);

  p.RegisterCalc("CosThetaHel",rad::gn2s0s0s12::CosThetaHel);
  p.RegisterCalc("ThetaHel",rad::gn2s0s0s12::ThetaHel);
  p.RegisterCalc("PhiHel",rad::gn2s0s0s12::PhiHel);
  
  //need to add proton rest PR production angles
  p.CosThetaCM(); 
  //p.ThetaCM();
  p.PhiCM();       
  //p.CosThetaPR();
  //p.ThetaPR();
  //p.PhiPR();
    
  // 3. Calculate Invariant Masses
  p.Mass("Qp",{"gprime"});             
  p.Mass("GMass",{"gprime"});             
  p.Mass2("Qp2",{"gprime"});
  p.Mass2("s_photo",{VirtGamma(),BeamIon()});
  p.Mass("W",{"miss_hadro"});
  p.Mass2("W2",{"miss_hadro"});
  p.Mass("Whad",{"gprime","pprime"});
  p.Mass2("Whad2",{"gprime","pprime"});
    
  // 3b. Exclusivity
  p.Mass("MissMass",{"miss"});
  p.Mass2("MissMass2", {"miss"});             
  //p.P("MissP",{"miss"});
  p.Pt("MissPt",{"miss"});
  //p.Pz("MissPz",{"miss"});
  //p.Theta("MissTheta",{"miss"});
    
  // 3c. Missing proton (later split to own toplogy recipe)
  p.Mass("MissMass_pprime",{"miss_pprime"});
  p.Mass2("MissMass2_pprime", {"miss_pprime"}); // better to use miss or particles?      
  //p.P("MissP_pprime",{"miss_pprime"});
  p.Pt("MissPt_pprime",{"miss_pprime"});
  //p.Pz("MissPz_pprime",{"miss_pprime"});
  //p.Theta("MissTheta_pprime",{"miss_pprime"});
  
    
  //4. Calculate Mandelstam t (requires beam definition)
  p.RegisterCalc("t_top", rad::physics::TTop);
  p.RegisterCalc("t_bot", rad::physics::TBot);
  p.RegisterCalc("DeltaT", rad::physics::DeltaTBot);
  
  //5. Misc Kinematics Variables for cuts etc
  p.RegisterCalc("DeltaPhiProton", rad::DeltaPhi,{{"pprime","miss_pprime"}});
  p.Energy("GammaE", {VirtGamma()});
    
  //6. Particle Basic Observables
  p.ParticleTheta({"scat_ele","ele","pos","gprime","pprime"});
  p.ParticlePhi({"scat_ele","ele","pos","gprime","pprime"});
  p.ParticleP({"scat_ele","ele","pos","gprime","pprime"});
  p.ParticleEta({"scat_ele","ele","pos","gprime","pprime"});

};
  
