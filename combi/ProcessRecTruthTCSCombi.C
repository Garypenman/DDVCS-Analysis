#include "AnalysisManager.h"
#include "ePICReaction.h"
#include "BasicKinematicsRDF.h"
#include "KinematicsProcElectro.h"
#include "ElectronScatterKinematics.h"
#include "gammaN_2_Spin0Spin0SpinHalf.h"
#include <TBenchmark.h>

#include "ePICFileStreamer.h"
#include "../include/FileProcessing.h"

#include "TopologyRecipes.h"
#include "CorrectionRecipes.h"
#include "HistogramRecipes.h"

const std::string my_out_dir = "/w/work5/home/garyp/combirad_trees/TCS_18x275_hplus_test/";

void ProcessRecTruthTCSCombi(std::vector<std::string> infiles={""}, 
			     std::string outdir = my_out_dir,
			     const int BeamEle_idx = 0,
			     const int BeamIon_idx = 3,
			     const int Role_ScatEle  = 2,
			     const int Role_Recoil   = 5,
			     const int Role_DecayEle = 6, 
			     const int Role_DecayPos = 7, 
			     const int lep_PDG = 11) 
{
  using namespace rad;
  using namespace rad::consts::data_type; 

  gBenchmark->Start("df");

  using Reaction = epic::ePICReaction;
  using Processor = KinematicsProcElectro;
  // =================================================================================
  // 1. SETUP & MATCHING
  // =================================================================================
  if(infiles[0] == ""){
    std::cout << "Infiles empty, grabbing test xrootd file!" << std::endl;
    infiles = rad::files::GetXRootDFiles("dtn-eic.jlab.org/","/volatile/eic/EPIC/RECO/26.02.0/epic_craterlake/EXCLUSIVE/DDVCS_ABCONV/EpIC1.1.6-1.0/18x275/q2_0_10/edecay/hplus/","edm4eic.root",1);
  }
  
  AnalysisManager<Reaction,Processor>  mgr{
    "TCS",
    "events",
    infiles};
  
  mgr.SetOutputDir(outdir);
  auto& df = mgr.Reaction();
  df.SetBeamsFromMC(0, 3); 
  
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
  
  // --- Auxiliary Data Handling ---
  //collect and associate cluster energies with particles
  //rec_cal_energy will be synched with momentum ordering
  rec_all_df.DefineAssociation("clusters", {"EcalBarrelClusters", "EcalEndcapPClusters"}, "energy");
  rec_all_df.DefineProjection("rec_cal_energy", "rec_clusters_energy", "rad::util::First");
  
  rec_all_df.DefineAssociation("tracks",
   			       {"TaggerTrackerReconstructedParticles"},
   			       {"PDG"});
  rec_all_df.DefineProjection("rec_tracks_pid", 
  			      "rec_tracks_PDG", 
  			      "rad::util::First");
  // rec_all_df.SetParticleCandidates(consts::ScatEle(), 
  // 				   Role_ScatEle, 
  // 				   rad::index::FilterIndices((double)11), 
  // 				   {"rec_tracks_pid"});
  
  rec_all_df.DefineDetectorFlag("rec_from_tagger","TaggerTrackerReconstructedParticleAssociations");
  //auto lowQ2_ele = [](){};
  
  // --- Any Further Cloned Managers ---
  //clone these after any mgr or df defines
  auto rec_charge_mgr = rec_all_mgr.Clone("TCS_charge",copystreams);
  auto& rec_charge_df = rec_charge_mgr.Reaction();
  auto rec_pid_mgr = rec_all_mgr.Clone("TCS_pid",copystreams);
  auto& rec_pid_df = rec_pid_mgr.Reaction();
 
  
  //now do the rec_all_df candidates and combis
  // rec_all_df.SetParticleCandidates(consts::ScatEle(), Role_ScatEle, 
  // 				   rad::index::FilterIndicesWithFlag(11), 
  // 				   {"rec_true_pid", "rec_from_tagger"});
  
  rec_all_df.SetParticleCandidates(consts::ScatEle(), Role_ScatEle, rad::index::FilterIndices(11), {"rec_true_pid"});
  rec_all_df.SetParticleCandidates("ele", Role_DecayEle, rad::index::FilterIndices(11),  {"rec_true_pid"}); 
  rec_all_df.SetParticleCandidates("pos", Role_DecayPos, rad::index::FilterIndices(-11), {"rec_true_pid"}); 
  rec_all_df.SetParticleCandidates("pprime", Role_Recoil, rad::index::FilterIndices(2212), {"rec_true_pid"}); 
  
  rec_all_df.MakeCombinations();
  rec_all_mgr.AddStream(Truth(),"");
  rec_all_mgr.AddStream(Rec(),"");
  rec_all_mgr.AddStream(Rec(),"match");
  rec_all_mgr.AddStream(Rec(),"cuts");
  
  //rec charge combis
  //  rec_charge_df.SetupMatching();
  //rec_charge_df.SetParticleCandidates(consts::ScatEle(), Role_ScatEle, rad::index::FilterIndices((short)-1), {"rec_charge"});
  rec_charge_df.SetParticleCandidates(consts::ScatEle(), Role_ScatEle, rad::index::FilterIndicesWithFlag((short)-1), {"rec_charge", "rec_from_tagger"});
  rec_charge_df.SetParticleCandidates("ele", Role_DecayEle, rad::index::FilterIndices((short)-1), {"rec_charge"});
  rec_charge_df.SetParticleCandidates("pos", Role_DecayPos, rad::index::FilterIndices((short)1), {"rec_charge"});
  rec_charge_df.SetParticleCandidates("pprime", Role_Recoil, rad::index::FilterIndices(2212), {"rec_true_pid"}); 
  
  rec_charge_df.MakeCombinations();
  rec_charge_mgr.AddStream(Truth(),"");
  rec_charge_mgr.AddStream(Rec(),"");
  rec_charge_mgr.AddStream(Rec(),"match");
  rec_charge_mgr.AddStream(Rec(),"cuts");
  
  //rec PID
  //rec_pid_df.SetupMatching(); 
  //rec_pid_df.SetParticleCandidates(consts::ScatEle(), Role_ScatEle, rad::index::FilterIndices(11), {"rec_pid"});
  rec_pid_df.SetParticleCandidates(consts::ScatEle(), Role_ScatEle, rad::index::FilterIndicesWithFlag(11), {"rec_pid", "rec_from_tagger"});
  rec_pid_df.SetParticleCandidates("ele", Role_DecayEle, rad::index::FilterIndices(11),  {"rec_pid"}); 
  rec_pid_df.SetParticleCandidates("pos", Role_DecayPos, rad::index::FilterIndices(-11), {"rec_pid"}); 
  rec_pid_df.SetParticleCandidates("pprime", Role_Recoil, rad::index::FilterIndices(2212), {"rec_pid"}); 
  
  rec_pid_df.MakeCombinations();
  rec_pid_mgr.AddStream(Truth(),"");
  rec_pid_mgr.AddStream(Rec(),"");
  rec_pid_mgr.AddStream(Rec(),"match");
  rec_pid_mgr.AddStream(Rec(),"cuts");
  
  
  // [A] Topology
  auto topology_recipe = TCS_Exclusive_Topology<Processor>;
  
  
  // [B] REC-SPECIFIC CORRECTIONS
  auto correction_recipe = TCS_Mass_Corrections;
  
  
  // [C] SELECTION CUTS
  auto selection_recipe = TCS_Selection_Recipe;
  
  rec_all_mgr.Reaction().Define("rec_isTruth_match","rec_isTruth");
  rec_charge_mgr.Reaction().Define("rec_isTruth_match","rec_isTruth");
  rec_pid_mgr.Reaction().Define("rec_isTruth_match","rec_isTruth");
  
  auto match_recipe = [](PhysicsSelection& s) {
    //this doesnt work as variables not called just isTruth?
    s.AddCutBool("match_cut", rad::consts::TruthMatchedCombi()); 
    //s.AddTruthMatchedCut("truthmatch_cut");
  };
  
  
  // [D] HISTOGRAMS
  auto histogram_recipe =  TCS_Histogram_Recipe;
  
  
  // Apply Topology to ALL streams
  // Apply Corrections to All Rec streams
  // Apply Nothing to the base Rec stream
  // Apply IsTruth==1 Selection to match Rec stream
  // Apply Physics Selections (cuts) to cuts Rec stream
  // Apply Histograms to ALL streams
  
  mgr.ConfigureKinematics(TCS_Exclusive_Topology);
  //mgr.ConfigureKinematics(Rec(), correction_recipe);
  //mgr.ConfigureSelection(Rec(), selection_recipe);
  mgr.ConfigureHistograms(histogram_recipe);
  
  rec_all_mgr.ConfigureKinematics(TCS_Exclusive_Topology);
  rec_all_mgr.ConfigureKinematics(Rec(), correction_recipe);
  rec_all_mgr.ConfigureSelection(Rec()+"match", match_recipe);
  rec_all_mgr.ConfigureSelection(Rec()+"cuts", selection_recipe);
  rec_all_mgr.ConfigureHistograms(histogram_recipe);
  
  rec_charge_mgr.ConfigureKinematics(TCS_Exclusive_Topology);
  rec_charge_mgr.ConfigureKinematics(Rec(), correction_recipe);
  rec_charge_mgr.ConfigureSelection(Rec()+"match", match_recipe);
  rec_charge_mgr.ConfigureSelection(Rec()+"cuts", selection_recipe);
  rec_charge_mgr.ConfigureHistograms(histogram_recipe);
  
  rec_pid_mgr.ConfigureKinematics(TCS_Exclusive_Topology);
  rec_pid_mgr.ConfigureKinematics(Rec(), correction_recipe);
  rec_pid_mgr.ConfigureSelection(Rec()+"match", match_recipe);
  rec_pid_mgr.ConfigureSelection(Rec()+"cuts", selection_recipe);
  rec_pid_mgr.ConfigureHistograms(histogram_recipe);
  
  // rad::rdf::PrintParticles(rad_df, Truth());
  // rad::rdf::PrintParticles(rad_df, Rec());
  
  // [D] TREES
  mgr.Snapshot();
  rec_all_mgr.Snapshot();
  rec_charge_mgr.Snapshot();
  rec_pid_mgr.Snapshot();
  
  //Print diagnostics BEFORE running expensive event loop
  //std::cout << "\n=== CHECKING ANALYSIS SETUP ===\n" << std::endl;
  //mgr.PrintDiagnostics();
  //PrintDefinedColumnNames(mgr.Reaction().CurrFrame());
  //PrintColumnValues(mgr.Reaction(),{"rec_scat_ele"},10);
//mgr.Reaction().Define("print",[](const Indices_t& idx, const RVecResultType& pz, const RVecResultType& tpz){cout<<" matching "<<idx<<pz<<tpz<<endl; return 1;},{"rec_match_id","rec_pz","tru_pz"});
  //mgr.Reaction().Filter("print","test");
  
  // =================================================================================
  // 3. RUN IT ALL 
  // =================================================================================
  gBenchmark->Start("analysis");
  mgr.Run();
  rec_all_mgr.Run();
  rec_charge_mgr.Run();
  rec_pid_mgr.Run();
  gBenchmark->Stop("analysis");
  gBenchmark->Print("analysis");
 }


//---Glob Overload---
void ProcessRecTruthTCSCombi(const std::string& infile_glob,
                             std::string outdir = my_out_dir,
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

