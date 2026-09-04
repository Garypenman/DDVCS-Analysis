#include "AnalysisManager.h"
#include "ePICReaction.h"
#include "KinematicsProcElectro.h"
#include "ElectronScatterKinematics.h"
#include "gammaN_2_Spin0Spin0SpinHalf.h"
//#include "DefineNames.h"

#include <TBenchmark.h>
#include <vector>
#include <memory>
#include <string>

#include "ePICFileStreamer.h"
#include "../include/Config.h"
#include "../include/FileProcessing.h"

#include "../include/DetectorAssociations.h"

#include "../include/TopologyRecipes.h"
#include "../include/CorrectionRecipes.h"
#include "../include/SelectionRecipes.h"
//#include "../include/HistogramRecipes.h"


void ProcessRecTruthTCSCombi(std::vector<std::string> infiles={""}, 
			     std::string outdir = "./testout/",
			     const int BeamEle_idx = 0,
			     const int BeamIon_idx = 3,
			     const int Role_ScatEle  = 2,
			     const int Role_Recoil   = 5,
			     const int Role_DecayEle = 6, 
			     const int Role_DecayPos = 7, 
			     const int lep_PDG = 11) 
{ 
  ROOT::EnableImplicitMT(16);

  using namespace rad;
  using namespace rad::consts::data_type; 

  gBenchmark->Start("df");

  using Reaction = epic::ePICReaction;
  using Processor = KinematicsProcElectro;
  // =================================================================================
  // 1. SETUP & MATCHING
  // =================================================================================
  if(infiles[0] == ""){
    int nfiles = 10;
    std::cout << "Infiles empty, grabbing test xrootd file!" << std::endl;
    infiles = rad::files::GetXRootDFiles("dtn-eic.jlab.org/","/volatile/eic/EPIC/RECO/26.04.1/epic_craterlake/EXCLUSIVE/DDVCS_ABCONV/EpIC1.1.6-1.1/10x130/q2_0_10/edecay/hplus/","edm4eic.root",nfiles);
    if(infiles.empty()){
      infiles = rad::files::GetXRootDFiles("epicxrd1.sdcc.bnl.gov:1095/","/eic/EPIC/RECO/26.04.1/epic_craterlake/EXCLUSIVE/DDVCS_ABCONV/EpIC1.1.6-1.1/10x130/q2_0_10/edecay/hplus/","edm4eic.root",nfiles);

    }
    
  }
  
  AnalysisManager<Reaction,Processor>  mgr{
    "TCS",
    "events",
    infiles};
  
  mgr.SetOutputDir(outdir);
  auto& df = mgr.Reaction();
  df.SetBeamsFromMC(BeamEle_idx, BeamIon_idx); 
  // df.SetBeamElectron(0.0, 0.0, -9.0);
  // df.SetBeamIon(0.0, 0.0, 130.0, 0.938);
  // df.CorrectCrossingAngle(kFALSE);
  
  // --- Rec Clone ---
  //"pure" rec clone setup before SetupTruth in order to do SetupMatching
  bool copystreams = false;
  auto rec_all_mgr = mgr.Clone("TCS_all",copystreams);
  auto& rec_all_df = rec_all_mgr.Reaction();
  
  
  // --- Pure Truth Tree ---
  df.SetupTruth();

  df.SetParticleCandidates(consts::ScatEle(), {Role_ScatEle});
  df.SetParticleCandidates("pprime", {Role_Recoil});
  df.SetParticleCandidates("ele", {Role_DecayEle});
  df.SetParticleCandidates("pos", {Role_DecayPos});
  
  df.MakeCombinations();
  mgr.AddStream(Truth(),"");

  rec_all_df.SetupMatching();
  
  BuildAssociations(rec_all_df);
  
  
  // --- Any Further Cloned Managers ---
  //clone these after any mgr or df defines
  auto rec_pmiss_mgr = rec_all_mgr.Clone("TCS_pmiss",copystreams);
  auto& rec_pmiss_df = rec_pmiss_mgr.Reaction();
 
  
  //now do the rec_all_df candidates and combis
  rec_all_df.SetParticleCandidates(consts::ScatEle(), Role_ScatEle, 
				   rad::index::FilterIndicesWithFlag(11), 
				   {"rec_true_pid", "rec_from_tagger"});
  
  //rec_all_df.SetParticleCandidates("ele", Role_DecayEle, rad::index::FilterIndicesWithFlag(11),  {"rec_true_pid", "rec_not_from_tagger"}); 
  rec_all_df.SetParticleCandidates("ele",Role_DecayEle,
				   [](const ROOT::RVecI& pid, const ROOT::RVec<int>& not_tagger)
				   {
				     return ROOT::VecOps::Nonzero((pid != 2212) && (not_tagger == 1));
				   }, {"rec_pid","rec_not_from_tagger"});
  
  //rec_all_df.SetParticleCandidates("pos", Role_DecayPos, rad::index::FilterIndicesWithFlag(-11), {"rec_true_pid",  "rec_not_from_tagger"}); 
  rec_all_df.SetParticleCandidates("pos",Role_DecayPos,
				   [](const ROOT::RVecI& pid, const ROOT::RVec<int>& not_tagger)
				   {
				     return ROOT::VecOps::Nonzero((pid != 2212) && (not_tagger == 1));
				   }, {"rec_pid","rec_not_from_tagger"});

  rec_all_df.SetParticleCandidates("pprime", Role_Recoil, rad::index::FilterIndices(2212), {"rec_true_pid"}); 

  //rec_all_df.SetParticleCandidates("pprime", Role_Recoil, 
  //rad::index::FilterIndicesWithFlag(2212), 
  //{"rec_true_pid", "rec_from_romanpot"});
  
  rec_all_df.MakeCombinations();
  rec_all_mgr.AddStream(Truth(),"");
  rec_all_mgr.AddStream(Rec(),"");


  rec_pmiss_df.SetParticleCandidates(consts::ScatEle(), Role_ScatEle, 
				   rad::index::FilterIndicesWithFlag(11), 
				   {"rec_true_pid", "rec_from_tagger"});
  
  rec_pmiss_df.SetParticleCandidates("ele", Role_DecayEle, rad::index::FilterIndicesWithFlag(11),  {"rec_true_pid", "rec_not_from_tagger"}); 
  rec_pmiss_df.SetParticleCandidates("pos", Role_DecayPos, rad::index::FilterIndicesWithFlag(-11), {"rec_true_pid",  "rec_not_from_tagger"}); 
  
  rec_pmiss_df.MakeCombinations();
  rec_pmiss_mgr.AddStream(Truth(),"");
  rec_pmiss_mgr.AddStream(Rec(),"");

  
  
  // [A] Topology
  auto topology_recipe = TCS_Exclusive_Topology<Processor>;
  
  
  // [B] REC-SPECIFIC CORRECTIONS
  auto correction_recipe = TCS_MassP_Corrections;
  
  
  // [C] SELECTION CUTS
  auto selection_recipe = TCS_Selection_Recipe;
  
  rec_all_mgr.Reaction().Define("rec_isTruth_match","rec_isTruth");
  //rec_pmiss_mgr.Reaction().Define("rec_isTruth_match","rec_isTruth");
  
  auto match_recipe = [](PhysicsSelection& s) {
    //this doesnt work as variables not called just isTruth?
    s.AddCutBool("match_cut", rad::consts::TruthMatchedCombi()); 
    //s.AddTruthMatchedCut("truthmatch_cut");
  };
  
  
  // [D] HISTOGRAMS
  //auto histogram_recipe =  TCS_Histogram_Recipe;
  
  
  // Apply Topology to ALL streams
  // Apply Corrections to All Rec streams
  // Apply Histograms to ALL streams
  
  mgr.ConfigureKinematics(topology_recipe);
  //mgr.ConfigureHistograms(histogram_recipe);
  
  rec_all_mgr.ConfigureKinematics(topology_recipe);
  rec_all_mgr.ConfigureKinematics(Rec(), correction_recipe);
  rec_all_mgr.ConfigureKinematics(Rec(), TCS_All_Associations<Processor>);
  //rec_all_mgr.ConfigureHistograms(histogram_recipe);

  rec_pmiss_mgr.ConfigureKinematics(TCS_MissProton_Topology<Processor>);
  rec_pmiss_mgr.ConfigureKinematics(Rec(), correction_recipe);
  rec_pmiss_mgr.ConfigureKinematics(Rec(), TCS_MissProton_Associations<Processor>);
  //rec_pmiss_mgr.ConfigureHistograms(histogram_recipe);
  
  // rad::rdf::PrintParticles(df, Truth());
  // rad::rdf::PrintParticles(rec_all_df, Truth());
  // rad::rdf::PrintParticles(rec_all_df, Rec());

  
  // [D] TREES
  std::vector<std::string> extra_cols = {"rec_pprime_rphits_time0","rec_pprime_rphits_timeavg"};
  mgr.Snapshot();
  rec_all_mgr.Snapshot(extra_cols);
  rec_pmiss_mgr.Snapshot();
  
  //Print diagnostics BEFORE running expensive event loop
  //mgr.PrintDiagnostics();

  //PrintDefinedColumnNames(rec_all_mgr.Reaction().CurrFrame());

  //PrintColumnValues(rec_all_mgr.Reaction(),{"rec_ele"},10);
  
  //mgr.Reaction().Define("print",[](const Indices_t& idx, const RVecResultType& pz, const RVecResultType& tpz){cout<<" matching "<<idx<<pz<<tpz<<endl; return 1;},{"rec_match_id","rec_pz","tru_pz"});

  //mgr.Reaction().Filter("print","test");
  
  // =================================================================================
  // 3. RUN IT ALL 
  // =================================================================================
  gBenchmark->Start("analysis");
  mgr.Run();
  rec_all_mgr.Run();
  rec_pmiss_mgr.Run();

  gBenchmark->Stop("analysis");
  gBenchmark->Print("analysis");
 }


//---Glob Overload---
void ProcessRecTruthTCSCombi(const std::string& infile_glob,
                             std::string outdir,
                             int BeamEle_idx = 0,
                             int BeamIon_idx = 3,
                             int Role_ScatEle = 2,
                             int Role_Recoil = 5,
                             int Role_DecayEle = 6,
                             int Role_DecayPos = 7,
                             int lep_PDG = 11)
{
  auto infiles = GlobToVector(infile_glob);
  
  // Forward all arguments to the real implementation
  ProcessRecTruthTCSCombi(infiles,
			  outdir,
			  BeamEle_idx,
			  BeamIon_idx,
			  Role_ScatEle,
			  Role_Recoil,
			  Role_DecayEle,
			  Role_DecayPos,
			  lep_PDG);
}

