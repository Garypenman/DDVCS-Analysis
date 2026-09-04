#include "AnalysisManager.h"
#include "ePICReaction.h"
#include "KinematicsProcessor.h"
#include "ElectronScatterKinematics.h"

#include <TBenchmark.h>
#include <vector>
#include <memory>
#include <string>

#include "ePICFileStreamer.h"
#include "../include/Config.h"
#include "../include/FileProcessing.h"

#include "../include/DetectorAssociations.h"

#include "../include/TopologyRecipes.h"
//#include "../include/CorrectionRecipes.h"
//#include "../include/SelectionRecipes.h"
//#include "../include/HistogramRecipes.h"

void ProcessRecTruthTCSSingleTrack(
    std::vector<std::string> infiles={""}, 
    std::string outdir = "./testout",
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
  using Reaction = epic::ePICReaction;
  using Processor = KinematicsProcessor; // Use base processor to avoid Q2/W overhead

  gSystem->Exec(Form("mkdir -p %s)",outdir.c_str()));
  gBenchmark->Start("df");

  if(infiles[0] == ""){
    int nfiles = 10;
    std::cout << "Infiles empty, grabbing test xrootd file!" << std::endl;
    infiles = rad::files::GetXRootDFiles("dtn-eic.jlab.org/","/volatile/eic/EPIC/RECO/26.04.1/epic_craterlake/EXCLUSIVE/DDVCS_ABCONV/EpIC1.1.6-1.1/10x130/q2_0_10/edecay/hplus/","edm4eic.root",nfiles);
    if(infiles.empty()){
      infiles = rad::files::GetXRootDFiles("epicxrd1.sdcc.bnl.gov:1095/","/eic/EPIC/RECO/26.04.1/epic_craterlake/EXCLUSIVE/DDVCS_ABCONV/EpIC1.1.6-1.1/10x130/q2_0_10/edecay/hplus/","edm4eic.root",nfiles);

    }
    
  }
  
  // =================================================================================
  // 1. BASE REACTION SETUP (Shared by all managers)
  // =================================================================================
  Reaction base_df{"events", infiles};
  base_df.SetBeamsFromMC(BeamEle_idx, BeamIon_idx); 
  base_df.SetupMatching();
  
  BuildAssociations(base_df);
  
  // =================================================================================
  // 2. RECIPE GENERATORS
  // =================================================================================

  
  // Creates a Kinematics recipe for a specific particle
  auto make_kine_recipe = [](const std::string& pName) {
    return [pName](Processor& p) {
      //Particle specific kinematic
      if(pName==consts::ScatEle()){
	p.RegisterCalc("Q2",rad::physics::SpacelikeM2, { {consts::BeamEle()} , {consts::ScatEle()} } );
      }
      if(pName=="pprime"){
	p.RegisterCalc("t_bot",rad::physics::SpacelikeM2, {{consts::BeamIon()},{"pprime"}});
      }
      if(pName=="gprime"){
	p.Creator().Sum("gprime", {{"ele", "pos"}});       
	p.Mass("Qp",{"gprime"});
	p.Mass2("Qp2",{"gprime"});
	
      }     
	
      
      //Basic Particle vector quantities
      p.ParticleTheta({pName});
      p.ParticlePhi({pName});
      p.ParticleP({pName});    
      p.ParticleEta({pName});
    };
  };
  
  // Creates a Correction recipe for a specific particle
  auto make_corr_recipe = [](const std::string& pName, double mass) {
    return [pName, mass](Processor& p) {
      p.PreModifier().FixMassWithP(pName, mass);
    };
  };
  
  // Creates a Histogram recipe for a specific particle
  auto make_hist_recipe = [](const std::string& pName) {
      return [pName](histo::Histogrammer& h) {
          h.Create(pName + "_pmag", pName + " Momentum; p [GeV/c]", 100, 0, 275, pName + "_pmag");
          h.Create(pName + "_eta", pName + " Pseudorapidity; #eta", 100, -10, 10, pName + "_eta");
          h.Create(pName + "_theta", pName + " Polar Angle; #theta [rad]", 100, 0.0, TMath::Pi(), pName + "_theta");
          h.Create(pName + "_phi", pName + " Azimuthal Angle; #phi [rad]", 100, -TMath::Pi(), TMath::Pi(), pName + "_phi");
      };
  };

  // Truth match requirement
  auto match_recipe = [](PhysicsSelection& s) {
      s.AddCutBool("match_cut", rad::consts::TruthMatchedCombi()); 
  };

  // =================================================================================
  // 3. SPAWN PARTICLE MANAGERS (Using a Builder Lambda)
  // =================================================================================

  // Manager config to sync our managers and additional params like snapshot columns
  struct ManagerConfig {
  std::unique_ptr<AnalysisManager<Reaction,Processor>> mgr;
  std::vector<std::string> extra_cols;
  };
  
  // Vector to securely hold our managers in memory
  std::vector<ManagerConfig> managers;
  //std::vector<std::unique_ptr<AnalysisManager<Reaction, Processor>>> managers;
  //std::vector<std::vector<std::string>> extra_mgr_cols;

  // The Builder Lambda: 'auto filter_func' allows both FilterIndices and FilterIndicesWithFlag
  auto build_stream = [&](const std::string& pName, const std::string& stream, int role, auto filter_func, 
                          const std::vector<std::string>& filterCols, double mass) 
  {
    // Clones the base_df so it automatically inherits the built auxiliary columns!
      auto mgr = std::make_unique<AnalysisManager<Reaction, Processor>>("TCS_" + pName, base_df);
      mgr->SetOutputDir(outdir);
      
      // 1. Setup Candidates
      mgr->Reaction().SetParticleCandidates(pName, role, filter_func, filterCols); 

      // 2. The Hack: Register EVERYTHING as a "Meson" just to get it into the ReactionMap
      mgr->Reaction().SetMesonParticles({pName});

      // 3. Make Combinations
      mgr->Reaction().MakeCombinations();
      
      // 4. Add Streams
      mgr->AddStream(Truth(), stream);
      mgr->AddStream(Rec(), stream);
      
      // 5. Apply Recipes
      mgr->ConfigureKinematics(make_kine_recipe(pName));
      mgr->ConfigureKinematics(Rec(), make_corr_recipe(pName, mass));
      if(pName==consts::ScatEle())
	mgr->ConfigureKinematics(Rec(), TCS_ScatEle_Associations<Processor>);
      if(pName=="ele" || pName=="gprime")
	mgr->ConfigureKinematics(Rec(), TCS_Ele_Associations<Processor>);
      if(pName=="pos" || pName=="gprime")
	mgr->ConfigureKinematics(Rec(), TCS_Pos_Associations<Processor>);
      if(pName=="pprime")
	mgr->ConfigureKinematics(Rec(), TCS_PPrime_Associations<Processor>);

      ///////mgr->ConfigureSelection(Rec(), match_recipe);
      mgr->ConfigureHistograms(make_hist_recipe(pName));

      
      // 6. Handle case by case extra snapshot colums
      std::vector<std::string> extra_cols = {};
      if(pName=="pprime"){
	extra_cols.push_back("rec_pprime_rphits_time0");
	extra_cols.push_back("rec_pprime_rphits_timeavg");
      }
      //extra_mgr_cols.push_back(extra_cols);

      // 7. Store safely
      managers.push_back({
	  std::move(mgr),
	  std::move(extra_cols)
	});
      
  };


  // --- Build all particle streams ---
  // Now they all use the exact same signature!
  build_stream(consts::ScatEle(), "", Role_ScatEle, rad::index::FilterIndicesWithFlag(11), {"rec_true_pid", "rec_from_tagger"}, consts::M_ele());
  //build_stream("ele", "trupid", Role_DecayEle, rad::index::FilterIndicesWithFlag(11), {"rec_true_pid", "rec_not_from_tagger"}, consts::M_ele());
  build_stream("ele", "", Role_DecayEle,
	       [](const ROOT::RVecI& pid, const ROOT::RVec<int>& not_tagger)
	       {
		 return ROOT::VecOps::Nonzero((pid != 2212) && (not_tagger == 1));
	       }, {"rec_pid","rec_not_from_tagger"}, consts::M_ele());
  
  //build_stream("pos", "", Role_DecayPos, rad::index::FilterIndicesWithFlag(-11), {"rec_true_pid", "rec_not_from_tagger"}, consts::M_ele());
  build_stream("pos", "", Role_DecayPos,
	       [](const ROOT::RVecI& pid, const ROOT::RVec<int>& not_tagger)
	       {
		 return ROOT::VecOps::Nonzero((pid != 2212) && (not_tagger == 1));
	       }, {"rec_pid","rec_not_from_tagger"}, consts::M_ele());
  
  build_stream("pprime", "", Role_Recoil, rad::index::FilterIndices(2212), {"rec_true_pid"}, consts::M_pro());

  // --- TO DO ---
  // analyse ele and pos candidates from all rec particles not tagger or pprime
  // get charge and pid info as branche we can do analysis cuts on instead
  //build_stream("ele", "pid", Role_DecayEle, rad::index::FilterIndicesWithFlag(11), {"rec_pid", "rec_not_from_tagger"}, consts::M_ele());
  //build_stream("pos", "pid", Role_DecayPos, rad::index::FilterIndicesWithFlag(-11), {"rec_pid", "rec_not_from_tagger"}, consts::M_ele());

  // =================================================================================
  // 3b. Dedicated mgr for e+e- pair folded together
  // =================================================================================
  auto build_pair_stream = [&](const std::string stream, const std::string flag){
      auto pair_mgr = std::make_unique<AnalysisManager<Reaction,Processor>>("TCS_gprime", base_df);
      pair_mgr->Reaction().SetParticleCandidates("ele", Role_DecayEle, rad::index::FilterIndicesWithFlag(11), {flag, "rec_not_from_tagger"});
      pair_mgr->Reaction().SetParticleCandidates("pos", Role_DecayPos, rad::index::FilterIndicesWithFlag(-11), {flag, "rec_not_from_tagger"});
      pair_mgr->SetOutputDir(outdir);
      pair_mgr->Reaction().SetMesonParticles({"ele","pos"});
      pair_mgr->Reaction().MakeCombinations();
      pair_mgr->AddStream(Truth(), stream);
      pair_mgr->AddStream(Rec(), stream);
      pair_mgr->ConfigureKinematics(make_kine_recipe("gprime"));
      pair_mgr->ConfigureKinematics(make_kine_recipe("ele"));
      pair_mgr->ConfigureKinematics(make_kine_recipe("pos"));
      //pair_mgr->ConfigureKinematics(Rec(), make_corr_recipe("ele", consts::M_ele()));
      //pair_mgr->ConfigureKinematics(Rec(), make_corr_recipe("pos", consts::M_ele()));
      //pair_mgr->ConfigureSelection(Rec(), match_recipe);
      pair_mgr->ConfigureHistograms(make_hist_recipe("gprime"));
      std::vector<std::string> extra_cols = {};
      managers.push_back({
	  std::move(pair_mgr),
	  std::move(extra_cols)
	});
  };
  build_pair_stream("","rec_true_pid");
  build_pair_stream("pid","rec_pid");
  
  // =================================================================================
  // 4. BOOK TREES & RUN
  // =================================================================================
  
  // Book all snapshots (Lazy Execution)
  int iter=0;
  for (auto& m : managers) {
    //set correct extra cols here
    m.mgr->Snapshot(m.extra_cols);
  }

  gBenchmark->Start("analysis");

  // The first Run() triggers the single event loop across the entire underlying RDataFrame.
  // The subsequent loops just finalize the TFile writes.
  for (auto& m : managers) {
      m.mgr->Run();
  }

  gBenchmark->Stop("analysis");
  gBenchmark->Print("analysis");
}

void ProcessRecTruthTCSSingleTrack(std::string infile_glob, 
				   std::string outdir,
				   const int BeamEle_idx = 0,
				   const int BeamIon_idx = 3,
				   const int Role_ScatEle  = 2,
				   const int Role_Recoil   = 5,
				   const int Role_DecayEle = 6, 
				   const int Role_DecayPos = 7,
				   const int lep_PDG = 11) 
{
  auto infiles = GlobToVector(infile_glob);
  
  // Forward all arguments to the real implementation
  ProcessRecTruthTCSSingleTrack(infiles,
			  outdir,
			  BeamEle_idx,
			  BeamIon_idx,
			  Role_ScatEle,
			  Role_Recoil,
			  Role_DecayEle,
			  Role_DecayPos,
			  lep_PDG);
}

