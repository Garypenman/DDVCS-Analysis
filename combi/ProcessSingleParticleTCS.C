#include "AnalysisManager.h"
#include "ePICReaction.h"
#include "BasicKinematicsRDF.h"
#include "KinematicsProcElectro.h"
#include "ElectronScatterKinematics.h"
#include "gammaN_2_Spin0Spin0SpinHalf.h"
#include <TBenchmark.h>

#include "ePICFileStreamer.h"
#include "../include/FileProcessing.h"
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
  
  AnalysisManager<Reaction,Processor>  mgr_scatele{
    "TCS_scatele",
    "events",
    infiles};
  
  mgr_scatele.SetOutputDir(outdir);
  auto& df_scatele = mgr.Reaction();
  df_scatele.SetBeamsFromMC(0, 3); 
  
 
  df_scatele.SetupMatching();
  
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
  bool copystreams = false;
  auto mgr_ele = mgr_scatele.Clone("TCS_ele",copystreams);
  auto mgr_pos = mgr_scatele.Clone("TCS_pos",copystreams);
  auto mgr_pprime = mgr_scatele.Clone("TCS_prime",copystreams);
  
  auto& df_ele = mgr_ele.Reaction();
  auto& df_pos = mgr_pos.Reaction();
  auto& df_pprime = mgr_pprime.Reaction();
  
  
  //now do the rec_all_df candidates and combis
  rec_all_df.SetParticleCandidates(consts::ScatEle(), Role_ScatEle, rad::index::FilterIndicesWithFlag(11), {"rec_true_pid", "rec_from_tagger"});
  //rec_all_df.SetParticleCandidates(consts::ScatEle(), Role_ScatEle, rad::index::FilterIndices(11), {"rec_true_pid"});
  rec_all_df.SetParticleCandidates("ele", Role_DecayEle, rad::index::FilterIndices(11),  {"rec_true_pid"}); 
  rec_all_df.SetParticleCandidates("pos", Role_DecayPos, rad::index::FilterIndices(-11), {"rec_true_pid"}); 
  rec_all_df.SetParticleCandidates("pprime", Role_Recoil, rad::index::FilterIndices(2212), {"rec_true_pid"}); 
  
  rec_all_df.MakeCombinations();
  rec_all_mgr.AddStream(Truth(),"");
  rec_all_mgr.AddStream(Rec(),"");
  
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
  
  
  
  // =================================================================================
  // 2. ANALYSIS CONFIGURATION 
  // =================================================================================
  
  // [A] SHARED KINEMATICS (Topology)
  // Applied to both Rec and Truth streams. Defines the decay chain.
  auto topology_recipe = [](Processor& p) {
    using namespace consts;
    p.Creator().Sum("gprime", {{"ele", "pos"}});       
    p.Creator().Diff("miss",    {{BeamEle(),BeamIon()},{consts::ScatEle(), "gprime", "pprime"}});
    p.Creator().Diff("miss_hadro",    {{BeamEle(),BeamIon()},{consts::ScatEle()}});
    p.Creator().Diff("miss_pprime",    {{BeamEle(),BeamIon()},{consts::ScatEle(), "gprime"}});
  
    p.SetMesonParticles({"ele","pos"});
    p.SetBaryonParticles({"pprime"});
    //p.SetBaryonParticles({"miss"});
    
    // 2. Calculate kinematic variables
    // basic DIS kinematics
    p.Q2();
    p.xbj();
    p.y();         
    p.nu();
    p.tau();
    p.tauprime();
    

    // virtgam polarisations
    p.RegisterCalc("GammaPol",rad::physics::ElS_PolVirtPhot);
    p.RegisterCalc("GammaPolCirc",rad::physics::ElS_CircPolVirtPhot);
    
    // decay angles
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
    p.Mass2("Qp2",{"gprime"});
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
    //care about tprime top and bot?
    
    //5. Misc Kinematics Variables for cuts etc
    p.RegisterCalc("DeltaPhiProton", rad::DeltaPhi,{{"pprime","miss_pprime"}});
    p.Energy("GammaE", {VirtGamma()});
    
    //6. Particle Basic Observables
    p.ParticleTheta({"scat_ele","gprime","pprime","ele","pos","miss","miss_pprime"});
    p.ParticlePhi({"scat_ele","gprime","pprime","ele","pos","miss","miss_pprime"});
    p.ParticleP({"scat_ele","gprime","pprime","ele","pos","miss","miss_pprime"});    
    p.ParticleEta({"scat_ele","gprime","pprime","ele","pos","miss","miss_pprime"});
    
  };

  // [B] REC-SPECIFIC CORRECTIONS
  // Applied only to Rec stream. Corrects electron momentum using ECal energy.
  auto correction_recipe = [](KinematicsProcessor& p) {
    using namespace consts;
    p.PreModifier().FixMass(ScatEle(), M_ele());
    p.PreModifier().FixMass("ele", M_ele());
    p.PreModifier().FixMass("pos", M_ele());
    p.PreModifier().FixMass("pprime", M_pro());
  };
  
  // [C] SELECTION CUTS
  // Applied to Rec stream (Primary). Determines which combinations are saved.
  // Can use variables defined in kinematics recipe
  auto selection_recipe = [](PhysicsSelection& s) {
    
    // At least reconstruct each particle
    s.AddCutRange("scatele_pmag_cut", "scat_ele_pmag", 0, 18); 
    s.AddCutMin("gprime_pmag_cut", "gprime_pmag", 0);
    s.AddCutRange("pprime_pmag_cut", "pprime_pmag", 0, 275);
    
    s.AddCutRange("Qp2_cut",    "Qp2", 0.0, 20.0); 
    
  };
  rec_all_mgr.Reaction().Define("rec_isTruth_match","rec_isTruth");
  rec_charge_mgr.Reaction().Define("rec_isTruth_match","rec_isTruth");
  rec_pid_mgr.Reaction().Define("rec_isTruth_match","rec_isTruth");
  
  auto match_recipe = [](PhysicsSelection& s) {
    //this doesnt work as variables not called just isTruth?
    s.AddCutBool("match_cut", rad::consts::TruthMatchedCombi()); 
    //s.AddTruthMatchedCut("truthmatch_cut");
  };
  
  // [C] HISTOGRAMS
  // Shared definitions for Rec and Truth.
  auto histogram_recipe = [](histo::Histogrammer& h) {
    //kinematics
    h.Create("Q2",";Q^2[GeV^{2}]", 100, 0, 1.0, "Q2");
    h.Create("nu",";#nu [GeV]",100,0,10000.,"nu");
    h.Create("xbj",";xbj",100,0,1.,"xbj");
    h.Create("y",";y",100,0,1.,"y");
    
    h.Create("W",";W (electro) [GeV]",100,0,200.,"W");
    h.Create("Whad","W (hadronic) [GeV]",100,0,200.,"Whad");
    
    h.Create("Qp", ";Q' = M_{e+e-} [GeV]", 100, 0.0, 5.0, "Qp");
    h.Create("Qp2", ";Q'^{2} = M_{e+e-} [GeV^2]", 100, 0.0, 25.0, "Qp2");
    
    h.Create("ttop","t(p,p^') [GeV^2]",100,0,2.0,"t_top");
    h.Create("tbot","t(p,p^') [GeV^2]",100,0,2.0,"t_bot");
    // h.Create("tptop","t' top [GeV^2]",100,0,2.0,"tp_top");
    // h.Create("tpbot","t' bot [GeV^2]",100,0,2.0,"tp_bot");
    
    h.Create2D("ttop_Qp2",";t_{eXBE} [GeV^{2}]; Q'^{2} [GeV^{2}]",100, 0.0, 2.0, 100, 0.0, 25.0,"t_top","Qp2");
    h.Create2D("tbot_Qp2",";t_{BABE} [GeV^{2}]; Q'^{2} [GeV^{2}]",100, 0.0, 2.0, 100, 0.0, 25.0,"t_bot","Qp2");
    
    h.Create2D("httop_W2",";t_{eXBE} [GeV^{2}]; W^{2} [GeV^{2}]",100, 0.0, 2.0, 100, 0.0, 200.0, "t_top","W2");
    h.Create2D("htbot_W2had",";t_{BABE} [GeV^{2}]; W^{2} [GeV^{2}]",100, 0.0, 2.0, 100, 0.0, 200.0, "t_bot", "W2");
    
    h.Create2D("W2_Qp2","",100, 0.0, 25., 100, 0.0, 200.0, "Qp2","W");
 
    //CM and PR Decay Angles
    h.Create("CosThetaCM","cos(#theta_CM)",100,-1,1,"CosThetaCM");
    h.Create("PhiCM","#phi_CM",100,-TMath::Pi(),TMath::Pi(),"PhiCM");
    //h.Create("CosThetaPR","cos(#theta_{PR})",100,-1,1,"CosThetaPR");
    //h.Create("PhiPR","#phi_{PR}",100,-TMath::Pi(),TMath::Pi(),"PhiPR");
  
    //exclusivity
    h.Create("MissMass","Mmiss [GeV]",100,-10,10,"MissMass");
    h.Create("MissMass2",     "Missing Mass squared; [GeV]", 100, -50, 50, "MissMass2");
    //h.Create("missP","p_miss(e',#gamma',p')",100,0,100,"miss_pmag");
    h.Create("missPt","p_t,miss(e',#gamma',p')",100,0,10,"MissPt");
    //h.Create("missPz","p_z,miss(e',#gamma',p')",100,0,100,"MissPz");
    //h.Create("missTheta","#theta_miss(e',#gamma',p')",100,0,1,"miss_theta");
  
    //semi-exclusivity
    h.Create("MissMass_pprime","Missing Mass of Proton; M_{miss,p'} [GeV/c^{2}]",100,-5., 5., "MissMass_pprime");
    h.Create("MissMass2_pprime", "Missing Mass of Proton squared; M^{2}_{miss,p'} [GeV^{2}/c^{4}]", 100, -25., 25., "MissMass2_pprime");
    
    //scattered electron
    h.Create("scat_ele_pmag","Momentum of Scattered Electron; p_{e'} [GeV/c]",100,0,18,"scat_ele_pmag");
    h.Create("scat_ele_eta","Pseudorapidity of Scattered Electron; #eta_{e'}",100,-10,10,"scat_ele_eta");
    h.Create("scat_ele_theta","Polar Angle of Scattered Electron; #theta_{e'} [rad]",100, 0.0,TMath::Pi(),"scat_ele_theta");
    h.Create("scat_ele_phi","Azimuthal Angle of Scattered Electron; #phi_{e'} [rad]",100,-TMath::Pi(),TMath::Pi(),"scat_ele_phi");
  
    //decay ele
    h.Create("ele_pmag","Momentum of Decay Ele; p_{e-} [GeV/c]",100,0,275,"ele_pmag");
    h.Create("ele_eta","Pseudorapidity of Decay Ele; #eta_{e-}",100,-10,10,"ele_eta");
    h.Create("ele_theta","Polar Angle of Decay Ele; #theta_{e-} [rad]",100, 0.0,TMath::Pi(),"ele_theta");
    h.Create("ele_phi","Azimuthal Angle of Decay Ele; #theta_{e-} [rad]",100,-TMath::Pi(),TMath::Pi(),"ele_phi");
    
    //decay pos
    h.Create("pos_pmag","Momentum of Decay Pos; p_{e+} [GeV/c]",100,0,275,"pos_pmag");
    h.Create("pos_eta","Pseudorapidity of Decay Pos; #eta_{e+}",100,-10,10,"pos_eta");
    h.Create("pos_theta","Polar Angle of Decay Pos; #theta_{e+} [rad]",100, 0.0,TMath::Pi(),"pos_theta");
    h.Create("pos_phi","Azimuthal Angle of Decay Pos; #phi_{e+} [rad]",100,-TMath::Pi(),TMath::Pi(),"pos_phi");
    
    //recoil proton
    h.Create("pprime_pmag","Momentum of Recoil Proton; p_{p'} [GeV/c]",100,0,275,"pprime_pmag");
    h.Create("pprime_eta","Pseudorapidity of Recoil Proton; #eta_{p'}",100,-10,10,"pprime_eta");
    h.Create("pprime_theta","Polar Angle of Recoil Proton; #theta_{p'} [rad]",100, 0.0,TMath::Pi(),"pprime_theta");
    h.Create("pprime_phi","Azimuthal Angle of Recoil Proton; #phi_{p'} [rad]",100,-TMath::Pi(),TMath::Pi(),"pprime_phi");
  
    //for brufit need
    //CM_Phi Heli_theta Heli_phi GammaPolCirc=sqrt(1-epsilon)*Pol t GammaE
    h.Create("GammaPol","Polarisation of Virtual Photon;#epsilon",100,0,1,"GammaPol");
    h.Create("GammaE","Energy of Virtual Photon;E_{#gamma} [GeV]",100,0,18,"GammaE");
    h.Create("CosThetaHel","CosTheta Decay Angle;cos(#theta_{l})",100,-1,1,"CosThetaHel");
    h.Create("ThetaHel","Theta Decay Angle;#theta_{l} [rad]",100, 0.0,TMath::Pi(),"ThetaHel");
    h.Create("PhiHel","Phi Decay Angle;#phi_{l} [rad]",100,-TMath::Pi()-1,TMath::Pi()+1,"PhiHel");
 
    //Polarisation and kinematic limits for cicular
    h.Create2D("y_W","Wele missmass vs y",100,0,1,100,0,200,"y","W");
    h.Create2D("y_Escatele","E_e' vs y",100,0,1,100,0,18,"y","scat_ele_pmag");
    h.Create2D("y_CircPol","Circular Polarisation vs y",100,0,1,100,0,1,"y","GammaPolCirc");
    h.Create2D("W_CircPol","Circular Polarisation vs Wele missmiass",100,0,200,100,0,1,"W","GammaPolCirc");

  };
  
  // Apply Topology to ALL streams
  // Apply Corrections to All Rec streams
  // Apply Nothing to the base Rec stream
  // Apply IsTruth==1 Selection to match Rec stream
  // Apply Physics Selections (cuts) to cuts Rec stream
  // Apply Histograms to ALL streams
  
  mgr.ConfigureKinematics(topology_recipe);
  //mgr.ConfigureKinematics(Rec(), correction_recipe);
  //mgr.ConfigureSelection(Rec(), selection_recipe);
  mgr.ConfigureHistograms(histogram_recipe);
  
  rec_all_mgr.ConfigureKinematics(topology_recipe);
  rec_all_mgr.ConfigureKinematics(Rec(), correction_recipe);
  rec_all_mgr.ConfigureSelection(Rec()+"match", match_recipe);
  rec_all_mgr.ConfigureSelection(Rec()+"cuts", selection_recipe);
  rec_all_mgr.ConfigureHistograms(histogram_recipe);
  
  rec_charge_mgr.ConfigureKinematics(topology_recipe);
  rec_charge_mgr.ConfigureKinematics(Rec(), correction_recipe);
  rec_charge_mgr.ConfigureSelection(Rec()+"match", match_recipe);
  rec_charge_mgr.ConfigureSelection(Rec()+"cuts", selection_recipe);
  rec_charge_mgr.ConfigureHistograms(histogram_recipe);
  
  rec_pid_mgr.ConfigureKinematics(topology_recipe);
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

