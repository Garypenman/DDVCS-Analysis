#include "ePICReaction.h"
#include "ParticleCreator.h"
#include "ParticleGenerator.h"
#include "ParticleModifier.h"
#include "ePICParticleModifier.h"
#include "ePICParticleCreator.h"
#include "Indicing.h"
#include "Histogrammer.h"
#include "BasicKinematicsRDF.h"
#include "ReactionKinematicsRDF.h"
#include "ElectronScatterKinematicsRDF.h"
#include "gammaN_2_Spin0Spin0SpinHalfRDF.h"

#include "../include/ApplyKinematics.h"
#include "../include/MakeRADHistos.h"
#include "../include/DrawRADHistos.h"
#include "../include/FileProcessing.h"

void ProcessMCMatched(std::string infile="/w/work5/home/garyp/eic/Farm/EpIC_DDVCS_ee_18x275/recon/18x275_ddvcs_edecay_hminus_*.root",
		      std::string outfile="/w/work5/home/garyp/rad_trees/MCMatched_ddvcs_ee_18x275_minus.root",
		      const int scat_ele_idx=0, 
		      const int scat_ion_idx=1, 
		      const int lep_minus_idx=2, 
		      const int lep_plus_idx=3,
		      const int lep_PDG=11)
{
  using namespace rad::names::data_type; //for Rec(), Truth()
  
  gBenchmark->Start("df");
  
  ROOT::EnableImplicitMT(8);
  rad::config::ePICReaction epic{"events", infile };
   
  epic.SetBeamsFromMC(); //for this file 0=ebeam 3=pbeam
  epic.AliasColumnsAndMatchWithMC();
  
  epic.setScatElectronIndex(scat_ele_idx);
  epic.setParticleIndex("pprime",scat_ion_idx,2212);
  
  epic.setParticleIndex("ele",lep_minus_idx,lep_PDG);
  epic.setParticleIndex("pos",lep_plus_idx,-lep_PDG);
  
  rad::epic::ePICParticleModifier p_modifier(epic);
  rad::epic::ePICParticleCreator  p_creator(epic);
  
  // p_creator.Sum("gprime",{"ele","pos"});
  epic.Particles().Sum("gprime",{"ele","pos"});
  p_creator.MCMatchedFarForwardProton("pprime");
  
  //create recoil proton from missing 4-vector, e-' and gamma
  epic.Particles().Miss("calc_pprime",{rad::names::ScatEle().data(),"gprime"});
  p_modifier.FixMassTo("calc_pprime",rad::constant::M_pro());
  p_modifier.Apply("calc_pprimeMass");
  
  epic.setMesonParticles({"ele","pos"});
  epic.setBaryonParticles({"pprime"});
  
  epic.makeParticleMap();
  
  // rad::rdf::PrintParticles(epic,Truth());
  // rad::rdf::PrintParticles(epic,Rec());
  
  ApplyKinematics(epic);
  
  //Define subsets of particles and corresponing variables to plot
  epic.Define("electrons","rad::helpers::PositionsWhere(tru_pid==11)");
  //epic.Define(Truth()+"elsP",Form("Take(%spmag,electrons)",Truth().data()));
  
  //Define histograms
  rad::histo::Histogrammer histo{"set1",epic};
  histo.Init({Rec(),Truth()});
  MakeRADHistos(histo);
  
  epic.BookLazySnapshot(outfile);
  gBenchmark->Start("processing");
  
  DrawRADHistos(histo);
  MakeOutputPDF(outfile);
  
  gBenchmark->Stop("processing");
  gBenchmark->Print("processing");

  //save all histograms to file
  //histo.File("MCMatchedDDVCS_hists.root");

  gBenchmark->Stop("df");
  gBenchmark->Print("df");
  
}

// void ProcessMCMatchedDDVCS(){
  
//   std::string plus_infilename =  "/w/work5/home/garyp/eic/Farm/EpIC_ep_DDVCS_18x275/recon/18x275_ddvcs_1M_events_plus_flat_0_recon.root";
//   std::string minus_infilename =  "/w/work5/home/garyp/eic/Farm/EpIC_ep_DDVCS_18x275/recon/18x275_ddvcs_1M_events_minus_flat_0_recon.root";
  
//   std::string plus_outfilename = "MCMatchedDDVCS_plus.root";
//   std::string minus_outfilename = "MCMatchedDDVCS_minus.root";
  
//   EventLoop(plus_infilename, plus_outfilename);
//   EventLoop(minus_infilename, minus_outfilename);
  
// }
