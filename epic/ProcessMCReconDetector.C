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
  
  ROOT::EnableImplicitMT(8);
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
  
  
  // epic.Particles().Miss("calc_scatele",{"pprime","gprime"});
  // epic.Particles().Miss("calc_pprime",{rad::names::ScatEle().data(),"gprime"});
  // epic.Particles().Miss("calc_gprime",{rad::names::ScatEle().data(),"pprime"});
  
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
  
  //calculate missing versions of each particle
  //g.penman 25.11.25, 01:55am, I am losing my mind.
  //the modifier partts are a complete hack to get the 
  //newly created particles "into" the map
  //but after the above rewrite of the rec_pprime
  //and THAT needs to come after particle map 
  //because otherwise the order of inserting a new particle
  //from epic particle creator before make particle map
  //screws up where ConfigReaction inserts the beam indices into the truth
  //vectors somehow. ?!?!?!?!
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
  
  MakeDetectorAssociations(epic);

  ApplyKinematics(epic);
  
  //filters to fix seg faulting
  //epic.Define("ff_Nproton",Form("rad::helpers::Count(rec_pid,2212)"));
  //epic.Filter("(rec_Nele>1)*(rec_Npos>0)*(rec_Npro>0)","recon_filter");
  //epic.Filter("(rec_Nele>1)*(rec_Npos>0)*(rec_pmag[pprime]>0)","recon_filter");
  //epic.Filter("(rec_Nele>1)*(rec_Npos>0)*(ff_Nproton>0)","recon_filter");
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
