#pragma once

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
auto TCS_Topology = [](Processor& p) {

  // Calculate kinematic variables
  p.Q2();         
  p.xbj();
  p.y();         
  p.nu();
  p.tau();
  p.tauprime();

  // Polarisation
  p.RegisterCalc("GammaPol",rad::physics::ElS_PolVirtPhot);
  p.RegisterCalc("GammaPolCirc",rad::physics::ElS_CircPolVirtPhot);

  // Helicity frame decays
  p.RegisterCalc("CosThetaHel",rad::gn2s0s0s12::CosThetaHel);
  p.RegisterCalc("ThetaHel",rad::gn2s0s0s12::ThetaHel);
  p.RegisterCalc("PhiHel",rad::gn2s0s0s12::PhiHel);

  // C.M. decay
  p.CosThetaCM(); 
  //p.ThetaCM();
  p.PhiCM();       

  //need to add proton rest PR production angles
  //p.CosThetaPR();
  //p.ThetaPR();
  //p.PhiPR();
    
  // Calculate Invariant Masses
  p.Mass("Qp",{"gprime"});             
  p.Mass("GMass",{"gprime"});             
  p.Mass2("Qp2",{"gprime"});
  p.Mass2("s_photo",{VirtGamma(),BeamIon()});
  p.Mass("W",{"miss_hadro"});
  p.Mass2("W2",{"miss_hadro"});
  p.Mass("Whad",{"gprime","pprime"});
  p.Mass2("Whad2",{"gprime","pprime"});
  
    
  // Calculate Mandelstam t (requires beam definition)
  p.RegisterCalc("t_top", rad::physics::TTop);
  p.RegisterCalc("t_bot", rad::physics::TBot);
  p.RegisterCalc("DeltaT", rad::physics::DeltaTBot);

  //Delta phi between each particle
  //not so useful
  p.RegisterCalc("DeltaPhiExP", rad::DeltaPhi,{{"scat_ele","pprime"}});
  p.RegisterCalc("DeltaPhiExG", rad::DeltaPhi,{{"scat_ele","gprime"}});
  //maybe useful
  p.RegisterCalc("DeltaPhiGxP", rad::DeltaPhi,{{"gprime","pprime"}});

  //p.Energy("GammaE", {VirtGamma()});
    
 };

template <typename Processor>
auto TCS_Exclusive_Topology = [](Processor& p) {
  p.Creator().Sum("gprime", {{"ele", "pos"}});       
  p.Creator().Diff("miss",    {{BeamEle(),BeamIon()},{ScatEle(), "gprime", "pprime"}});
  p.Creator().Diff("miss_hadro",    {{BeamEle(),BeamIon()},{ScatEle()}});
  p.Creator().Diff("miss_scatele",    {{BeamEle(),BeamIon()}, {"gprime", "pprime"}});
  p.Creator().Diff("miss_pprime",    {{BeamEle(),BeamIon()}, {ScatEle(), "gprime"}});
  p.Creator().Diff("delta",{{BeamIon()},{"pprime"}});
  
  p.SetMesonParticles({"ele","pos"});
  p.SetBaryonParticles({"pprime"});

  // Exclusivity Miss Variables
  p.Mass("MissMass",{"miss"});
  p.Mass2("MissMass2", {"miss"});             
  p.Pt("MissPt",{"miss"});
    
  // Miss(Calculated) Proton info
  p.Mass("MissMass_pprime",{"miss_pprime"});
  p.Mass2("MissMass2_pprime", {"miss_pprime"});       
  p.Pt("MissPt_pprime",{"miss_pprime"});
  
  TCS_Topology<Processor>(p);

  //Exclusivity delta variables
  p.RegisterCalc("DeltaPhiProton", rad::DeltaPhi,{{"pprime","miss_pprime"}});
  p.RegisterCalc("DeltaThetaProton", rad::DeltaTheta,{{"pprime","miss_pprime"}});
  p.RegisterCalc("DeltaPProton", rad::DeltaP,{{"pprime","miss_pprime"}});

  p.RegisterCalc("DeltaPhiScatEle", rad::DeltaPhi,{{"scat_ele","miss_scatele"}});
  p.RegisterCalc("DeltaThetaScatEle", rad::DeltaTheta,{{"scat_ele","miss_scatele"}});
  p.RegisterCalc("DeltaPScatEle", rad::DeltaP,{{"scat_ele","miss_scatele"}});

  // Particle Basic Observables
  p.ParticleTheta({"scat_ele","ele","pos","gprime","pprime","miss"});
  p.ParticlePhi({"scat_ele","ele","pos","gprime","pprime","miss"});
  p.ParticleP({"scat_ele","ele","pos","gprime","pprime","miss"});
  p.ParticleEta({"scat_ele","ele","pos","gprime","pprime","miss"});


};

template <typename Processor>
auto TCS_MissProton_Topology = [](Processor& p) {
  p.Creator().Sum("gprime", {{"ele", "pos"}});       
  p.Creator().Diff("pprime",    {{BeamEle(),BeamIon()},{ScatEle(), "gprime"}});
  p.Creator().Diff("miss_hadro",    {{BeamEle(),BeamIon()},{ScatEle()}});
  p.Creator().Diff("delta",{{BeamIon()},{"pprime"}});

  p.SetMesonParticles({"ele","pos"});
  p.SetBaryonParticles({"pprime"});
    
  // Particle Basic Observables
  p.ParticleTheta({"scat_ele","ele","pos","gprime","pprime"});
  p.ParticlePhi({"scat_ele","ele","pos","gprime","pprime"});
  p.ParticleP({"scat_ele","ele","pos","gprime","pprime"});
  p.ParticleEta({"scat_ele","ele","pos","gprime","pprime"});

 };


template <typename Processor>
void TCS_Particle_Associations(Processor& p, std::string name){
  
  p.PassThrough(name,
                "rec_pid",
                "_pid");

  p.PassThrough(name,
                "rec_charge",
                "_charge");

  p.PassThrough(name,
                "rec_cal_energy",
                "_cal_energy");

  p.PassThrough(name,
                "rec_track_time",
                "_track_time");

}

template <typename Processor>
void TCS_ScatEle_Associations(Processor& p){
  TCS_Particle_Associations(p, "scat_ele");
}

template <typename Processor>
void TCS_Ele_Associations(Processor& p){
  TCS_Particle_Associations(p, "ele");
}

template <typename Processor>
void TCS_Pos_Associations(Processor& p){
  TCS_Particle_Associations(p, "pos");
}

template <typename Processor>
void TCS_PPrime_Associations(Processor& p){
  TCS_Particle_Associations(p, "pprime");
}


template <typename Processor>
void TCS_All_Associations(Processor& p){
  TCS_ScatEle_Associations(p);
  TCS_Ele_Associations(p);
  TCS_Pos_Associations(p);
  TCS_PPrime_Associations(p);  
}

template <typename Processor>
void TCS_MissProton_Associations(Processor& p){
  TCS_ScatEle_Associations(p);
  TCS_Ele_Associations(p);
  TCS_Pos_Associations(p);
}

