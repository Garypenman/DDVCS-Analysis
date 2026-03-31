#ifndef CONFIG_H
#define CONFIG_H

///////////////////////////////////////
// HEPMC
///////////////////////////////////////

//double checked with EpIC hepmc file
const int epic_beam_ele_idx=0; 
const int epic_beam_ion_idx=3; 
const int epic_scat_ele_idx=1; 
const int epic_scat_ion_idx=5; 
const int epic_lep_minus_idx=6; 
const int epic_lep_plus_idx=7;

//double checked with eslight hepmc file
//the lepton pair swap around event by event at hepmc level
//so actually need to figure this out not only for epic-rad analysis
//but also even at hepmc level analysis.
const int eslight_beam_ele_idx=0; 
const int eslight_beam_ion_idx=3; 
const int eslight_scat_ele_idx=2; 
const int eslight_scat_ion_idx=6; 
const int eslight_lep_minus_idx=5; 
const int eslight_lep_plus_idx=4;

//double checked with elspectro hepmc file
const int elspectro_beam_ele_idx=0; 
const int elspectro_beam_ion_idx=1; 
const int elspectro_scat_ele_idx=2; 
const int elspectro_scat_ion_idx=3; 
const int elspectro_lep_minus_idx=4; 
const int elspectro_lep_plus_idx=5;


///////////////////////////////////////
// EIC-RECON
// check eicrecon output order with
// events->Scan("MCParticles.PDG","MCParticles.generatorStatus>1")
// beams are absolute positions
// then beams become {0,1} in rad particles
// and everything else is pushed in afterwards
// as seen in the lines below
///////////////////////////////////////

const int edm4hep_epic_beam_ele_idx=0;
const int edm4hep_epic_beam_ion_idx=3;
const int edm4hep_epic_scat_ele_idx=2;
const int edm4hep_epic_scat_ion_idx=5;
const int edm4hep_epic_lep_minus_idx=6;
const int edm4hep_epic_lep_plus_idx=7;

//the lep plus and minus seem to swap around event by event 
//for the elspectro jpsi eicrecon files. Need to figure out eventually
//THIS NEEDS TO BE CHECKED
const int edm4hep_eslight_beam_ele_idx=0;
const int edm4hep_eslight_beam_ion_idx=1;
const int edm4hep_eslight_scat_ele_idx=2;
const int edm4hep_eslight_scat_ion_idx=3;
const int edm4hep_eslight_lep_minus_idx=4;
const int edm4hep_eslight_lep_plus_idx=5;

//currenlty dont have elspectr sim+eicrecon
//these are placeholder/guesses
//update in future.
const int edm4hep_elspectro_beam_ele_idx=0;
const int edm4hep_elspectro_beam_ion_idx=1;
const int edm4hep_elspectro_scat_ele_idx=2;
const int edm4hep_elspectro_scat_ion_idx=3;
const int edm4hep_elspectro_lep_minus_idx=4;
const int edm4hep_elspectro_lep_plus_idx=5;


const std::string farm_dir = "/w/work5/home/garyp/eic/Farm/";
const std::string out_dir = "/w/work5/home/garyp/combirad_trees/";

//vector<std::string> brufit_cols = {"UID","mc_t_bot","mc_t_top","mc_GammaPolCirc","mc_GammaE","mc_PhiHel","mc_ThetaHel","mc_CosThetaHel","mc_CMPhi","mc_CMTheta","mc_Q2","mc_Qp","mc_Qp2","mc_s_photo","mc_DeltaT"};
vector<std::string> brufit_cols = {"rdfentry_","mc_t_bot","mc_t_top","mc_GammaPolCirc","mc_GammaE","mc_PhiHel","mc_ThetaHel","mc_CosThetaHel","mc_PhiCM","mc_CosThetaCM","mc_Q2","mc_Qp","mc_Qp2","mc_s_photo","mc_DeltaT"};
  
#endif
