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

#include "../include/ApplyKinematics.h"
#include "../include/MakeRADHistos.h"
#include "../include/DrawRADHistos.h"
#include "../include/FileProcessing.h"
#include "../include/DetectorAssociations.h"

void ProcessMCReconDetector(std::vector<std::string> infiles={"/w/work5/home/garyp/eic/Farm/EpIC_DDVCS_ee_18x275/recon/18x275_ddvcs_edecay_hminus_0_recon.root"}, 
			    std::string outfile="/w/work5/home/garyp/rad_trees/MCReconDetector_ddvcs_ee_18x275_hminus.root",
			    const int scat_ele_idx=0, 
			    const int scat_ion_idx=1, 
			    const int lep_minus_idx=2, 
			    const int lep_plus_idx=3,
			    const int lep_PDG=11)
{  
  using namespace rad::names::data_type; //for Rec(), Truth()
  
  gBenchmark->Start("df");
  
  //ROOT::EnableImplicitMT(8);
  rad::config::ePICDetectorReaction epic{"events", infiles };
  epic.SetBeamsFromMC();
  epic.AliasColumnsAndMC();
  // epic.FixBeamElectronMomentum(0,0,-18);
  // epic.FixBeamIonMomentum(0,0,275,rad::constant::M_pro());
  
  
  //epic.setScatElectronIndex(rad::indice::useNthOccurance(1,11),{"rec_pid"});
  //epic.setParticleIndex("pprime",rad::indice::useNthOccurance(1,2212),{"rec_pid"});
  //epic.setParticleIndex("ele",rad::indice::useNthOccurance(2,11),{"rec_pid"});
  //epic.setParticleIndex("pos",rad::indice::useNthOccurance(1,-11),{"rec_pid"});
  
  epic.setScatElectronIndex(scat_ele_idx);
  epic.setParticleIndex("pprime",scat_ion_idx,2212);
  epic.setParticleIndex("ele",lep_minus_idx,lep_PDG);
  epic.setParticleIndex("pos",lep_plus_idx,-lep_PDG);
  
  rad::epic::ePICParticleModifier p_modifier(epic);
  rad::epic::ePICParticleCreator  p_creator(epic);
  
  epic.Particles().Sum("gprime",{"ele","pos"});
  
  //create recoil proton from missing 4-vector, e-' and gamma
  epic.Particles().Miss("calc_pprime",{rad::names::ScatEle().data(),"gprime"});
  // p_modifier.FixMassTo("calc_pprime",rad::constant::M_pro());
  // p_modifier.Apply("calc_pprimeMass");
  
  epic.setMesonParticles({"ele","pos"});
  epic.setBaryonParticles({"pprime"});
  
  epic.makeParticleMap();
  
  //get proton from rp detectors
  //this needs to come after make particle map to work
  p_creator.RomanPotProton("RPproton");
  p_modifier.OverwriteRecParticle("pprime","rec_RPproton");
  p_modifier.Apply("GetRPproton");
  //this needs to come after make particle map to work
  p_creator.B0Proton("B0proton");
  p_modifier.OverwriteRecParticle("pprime","rec_B0proton");
  p_modifier.Apply("GetB0proton");
  
  //rad::rdf::PrintParticles(epic,Truth());
  //rad::rdf::PrintParticles(epic,Rec());
  
  MakeDetectorAssociations(epic);

  ApplyKinematics(epic);
  
  //filters to fix seg faulting
  //epic.Define("ff_Nproton",Form("rad::helpers::Count(rec_pid,2212)"));
  //epic.Filter("(rec_Nele>1)*(rec_Npos>0)*(rec_Npro>0)","recon_filter");
  //epic.Filter("(rec_Nele>1)*(rec_Npos>0)*(rec_pmag[pprime]>0)","recon_filter");
  //epic.Filter("(rec_Nele>1)*(rec_Npos>0)*(ff_Nproton>0)","recon_filter");
  
  //Define subsets of particles and corresponing variables to plot
  //epic.Define("electrons","rad::helpers::PositionsWhere(tru_pid==11)");
  //epic.Define(Truth()+"elsP",Form("Take(%spmag,electrons)",Truth().data()));
  ClusterSums(epic);
  
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

  // save all histograms to file
  // histo.File("histograms.root");

  gBenchmark->Stop("df");
  gBenchmark->Print("df");
    
}
