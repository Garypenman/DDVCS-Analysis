#include "ePICDetectorReaction.h"
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

#include "TSystem.h"
#include "TBenchmark.h"
#include "ROOT/RLogger.hxx"
#include "ROOT/RDF/RInterface.hxx"

#include "ePICFileStreamer.h"

#include "../include/ApplyKinematics.h"
#include "../include/MakeRADHistos.h"
#include "../include/DrawRADHistos.h"
#include "../include/FileProcessing.h"
#include "../include/DetectorAssociations.h"

void ProcessMCMatchedDetector(std::vector<std::string> infiles={"root://dtn-eic.jlab.org//volatile/eic/EPIC/RECO/25.10.2/epic_craterlake/EXCLUSIVE/DVCS_ABCONV/10x100/*.root"},
			      std::string outfile="/w/work5/home/garyp/rad_trees/MCMatchedDetector_DVCS_10x100.root",
			      const int scat_ele_idx=0, 
			      const int scat_ion_idx=2, 
			      const int gprime_idx=1
			      )
{
  
  using namespace rad::names::data_type; //for Rec(), Truth()
  ROOT::EnableImplicitMT(8);
  
  std::string infiledir = "/volatile/eic/EPIC/RECO/25.10.2/epic_craterlake/EXCLUSIVE/DVCS_ABCONV/10x100/";
  infiles = rad::files::GetXRootDFiles("dtn-eic.jlab.org/",infiledir,"edm4eic.root",-1);
  
  rad::config::ePICDetectorReaction epic{"events", infiles };
   
  epic.SetBeamsFromMC(); //for this file 0=ebeam 3=pbeam
  epic.AliasColumnsAndMatchWithMC();
  
  epic.setScatElectronIndex(scat_ele_idx);
  epic.setParticleIndex("pprime",scat_ion_idx,2212);
  epic.setParticleIndex("gprime",gprime_idx,22);
  
  rad::epic::ePICParticleModifier p_modifier(epic);
  rad::epic::ePICParticleCreator  p_creator(epic);
  
  p_creator.MCMatchedFarForwardProton("pprime");
  
  epic.setMesonParticles({"gprime"});
  epic.setBaryonParticles({"pprime"});

  epic.makeParticleMap();

  //get proton from rp detectors
  //this needs to come after make particle map to work
  // p_creator.RomanPotProton("RPproton");
  // p_modifier.OverwriteRecParticle("pprime","rec_RPproton");
  // p_modifier.Apply("GetRPproton");
  // //this needs to come after make particle map to work
  // p_creator.B0Proton("B0proton");
  // p_modifier.OverwriteRecParticle("pprime","rec_B0proton");
  // p_modifier.Apply("GetB0proton");
  
  p_creator.Miss("calc_pprime",{rad::names::ScatEle().data(),"gprime"});
  p_modifier.OverwriteRecParticle("calc_pprime","calc_pprime");
  p_modifier.Apply("GetCalcPPrime");
  
  p_creator.Miss("calc_gprime",{rad::names::ScatEle().data(),"pprime"});
  p_modifier.OverwriteRecParticle("calc_gprime","calc_gprime");
  p_modifier.Apply("GetCalcGPrime");
  
  p_creator.Miss("calc_scatele",{"pprime","gprime"});
  p_modifier.OverwriteRecParticle("calc_scatele","calc_scatele");
  p_modifier.Apply("GetCalcScatEle");
  
  // rad::rdf::PrintParticles(epic,Truth());
  // rad::rdf::PrintParticles(epic,Rec());

  //MakeDetectorAssociations(epic);

  ApplyKinematics(epic);

  using ROOT::VecOps::RVec;
  using ROOT::Math::PxPyPzMVector;
  using ROOT::Math::VectorUtil::boost;
  epic.Define("rec_calc_Q2",[](RVec<float>px,RVec<float>py,RVec<float>pz,RVec<float>m,int ibeam, int iscat){
      PxPyPzMVector beam(px[ibeam],py[ibeam],pz[ibeam],m[ibeam]);
      PxPyPzMVector scat(px[iscat],py[iscat],pz[iscat],m[iscat]);
      // cout << "Beam: " << beam << endl;
      // cout << "Scat: " << scat << endl;
      auto Q2 = - (beam-scat).M2();
      //cout << "Q2 FUNC: " << Q2 << endl;
      return Q2;
    },{"rec_px","rec_py","rec_pz","rec_m",rad::names::BeamEle().data(),"rec_calc_scatele"});
  
  epic.Define("rec_calc_GammaPol",[](RVec<float>px,RVec<float>py,RVec<float>pz,RVec<float>m,int ibeam, int iscat, int ipbeam){
      PxPyPzMVector ebeam(px[ibeam],py[ibeam],pz[ibeam],m[ibeam]);
      PxPyPzMVector escat(px[iscat],py[iscat],pz[iscat],m[iscat]);
      PxPyPzMVector pbeam(px[ipbeam],py[ipbeam],pz[ipbeam],m[ipbeam]);
      
      auto Q2 = - (ebeam-escat).M2();
      
      auto pboost = pbeam.BoostToCM();
      auto prbeam = boost(ebeam,pboost);
      auto prscat = boost(escat,pboost);
      auto num = prbeam.Vect().Dot(prscat.Vect());
      auto denom = prbeam.Vect().R() * prscat.Vect().R();
      auto ElScatTh = acos(num/denom);
      auto GammaE = prbeam.E() - prscat.E();
      //cout << ElScatTh << " " << GammaE << " " << Q2 << endl; 
      auto pol = 1./(1.+2.*(1.+GammaE*GammaE/Q2)*TMath::Tan(ElScatTh/2.)*TMath::Tan(ElScatTh/2.));
      return pol;
    },{"rec_px","rec_py","rec_pz","rec_m",rad::names::BeamEle().data(),"rec_calc_scatele",rad::names::BeamIon().data()});

  epic.Define("rec_calc_GammaPolCirc", [](double pol){
    return sqrt(1-pol*pol);
  },{"rec_calc_GammaPol"});
  
  //Define subsets of particles and corresponing variables to plot
  //epic.Define("electrons","rad::helpers::PositionsWhere(tru_pid==11)");
  //epic.Define(Truth()+"elsP",Form("Take(%spmag,electrons)",Truth().data()));
  //ClusterSums(epic);

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
  //histo.File("histograms.root");

  gBenchmark->Stop("df");
  gBenchmark->Print("df");
  
}
