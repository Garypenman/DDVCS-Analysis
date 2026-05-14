#include <TBenchmark.h>
#include <TCanvas.h>

#include "../include/FileProcessing.h"
#include "../include/Config.h"
#include "ProcessHEPMCTCSCombi.C"

std::vector<Long64_t> unique_random_indices(Long64_t total, Long64_t pick, UInt_t seed) {
    std::vector<Long64_t> indices(total);
    std::iota(indices.begin(), indices.end(), 0);

    std::mt19937 rng(seed);
    std::shuffle(indices.begin(), indices.end(), rng);

    indices.resize(pick);
    return indices;
}


void ShuffleTree(const std::string &infile, const std::string &outfile, const std::string &treename) {
    // Open input file & tree
    TFile inFile(infile.c_str(), "READ");
    if (inFile.IsZombie()) {
        std::cerr << "Cannot open " << infile << std::endl;
        return;
    }
    TTree* tIn = nullptr;
    inFile.GetObject(treename.c_str(), tIn);
    if (!tIn) {
        std::cerr << "Tree " << treename << " not found in " << infile << std::endl;
        return;
    }

    // Create output file & clone tree structure
    TFile outFile(outfile.c_str(), "RECREATE");
    TTree* tOut = tIn->CloneTree(0); // clone structure only

    // Prepare index vector
    Long64_t nEntries = tIn->GetEntries();
    std::vector<Long64_t> indices(nEntries);
    std::iota(indices.begin(), indices.end(), 0);
    
    // ROOT RNG ? std::mt19937
    TRandom3 rootRand(0); // seed
    std::mt19937 gen(static_cast<unsigned>(rootRand.Integer(1e9)));
    
    // Shuffle indices
    std::shuffle(indices.begin(), indices.end(), gen);

    // Loop over shuffled indices
    for (auto idx : indices) {
        tIn->GetEntry(idx);
        tOut->Fill();
    }

    // Write output
    tOut->Write();
    outFile.Close();
    inFile.Close();

    std::cout << "Shuffled tree saved to " << outfile << std::endl;
}

void DFMerge(const std::string plus_file, const std::string minus_file, const std::string outfile, const std::string treename, const double pol, const int nev){
  
  gBenchmark->Start("Setup");
  
  ROOT::RDataFrame df_plus(treename, plus_file);
  ROOT::RDataFrame df_minus(treename, minus_file);
  
  // Step 1: Add helicity and thread-safe RNG
  auto df_plus_heli = df_plus.Define("egen_helicity", "1")
    .Define("rand", [] { return rad::random::Generator().Rndm(); });
  
  auto df_minus_heli = df_minus.Define("egen_helicity", "-1")
    .Define("rand", [] { return rad::random::Generator().Rndm(); });
  
  
  // Step 2: calculate asym n+ and n-
  auto nplus = (double)nev * (1.0 + pol) / 2.0; //9000 for 0.8 e.g.
  auto nminus = (double)nev * (1.0 - pol) / 2.0 ; //1000 for 0.8 e.g.
  
  auto total_plus = *df_plus_heli.Count();
  auto total_minus = *df_minus_heli.Count();
  
  // Random non-overlapping indices
  auto all_plus_indices = unique_random_indices(total_plus, nplus + nminus, 42);
  auto all_minus_indices = unique_random_indices(total_minus, nplus + nminus, 43);
  
  std::vector<Long64_t> indices_plus_for_plus(all_plus_indices.begin(), all_plus_indices.begin() + nplus);
  std::vector<Long64_t> indices_plus_for_minus(all_plus_indices.begin() + nplus, all_plus_indices.end());
  
  std::vector<Long64_t> indices_minus_for_minus(all_minus_indices.begin(), all_minus_indices.begin() + nplus);
  std::vector<Long64_t> indices_minus_for_plus(all_minus_indices.begin() + nplus, all_minus_indices.end());
  
  // Convert to unordered_set for O(1) filtering
  auto set_plus_for_plus = std::unordered_set<Long64_t>(indices_plus_for_plus.begin(), indices_plus_for_plus.end());
  auto set_plus_for_minus = std::unordered_set<Long64_t>(indices_plus_for_minus.begin(), indices_plus_for_minus.end());
  auto set_minus_for_minus = std::unordered_set<Long64_t>(indices_minus_for_minus.begin(), indices_minus_for_minus.end());
  auto set_minus_for_plus = std::unordered_set<Long64_t>(indices_minus_for_plus.begin(), indices_minus_for_plus.end());
   
  gBenchmark->Stop("Setup");
  gBenchmark->Print("Setup");
  
  gBenchmark->Start("Filter");
  
  // Filter each group
    auto df_plus_plus = df_plus_heli.Filter(
        [set_plus_for_plus](ULong64_t entry) {
            return set_plus_for_plus.count(entry);
        }, {"rdfentry_"});

    auto df_plus_minus = df_minus_heli.Filter(
        [set_minus_for_plus](ULong64_t entry) {
            return set_minus_for_plus.count(entry);
        }, {"rdfentry_"});

    auto df_minus_plus = df_plus_heli.Filter(
        [set_plus_for_minus](ULong64_t entry) {
            return set_plus_for_minus.count(entry);
        }, {"rdfentry_"});

    auto df_minus_minus = df_minus_heli.Filter(
        [set_minus_for_minus](ULong64_t entry) {
            return set_minus_for_minus.count(entry);
        }, {"rdfentry_"});

  
  gBenchmark->Stop("Filter");
  gBenchmark->Print("Filter");
  
  gBenchmark->Start("Snapshots");
  auto df_plus_plus_snap = df_plus_plus.Define("pol",[pol] { return pol; }).Define("pol_helicity","1").Redefine("mc_GammaPolCirc","mc_GammaPolCirc*pol").Snapshot(treename,"plus_plus_snap_test.root",brufit_cols);
  auto df_plus_minus_snap = df_plus_minus.Define("pol",[pol] { return pol; }).Define("pol_helicity","1").Redefine("mc_GammaPolCirc","mc_GammaPolCirc*pol").Snapshot(treename,"plus_minus_snap_test.root",brufit_cols);
  auto df_minus_plus_snap = df_minus_plus.Define("pol",[pol] { return -pol; }).Define("pol_helicity","-1").Redefine("mc_GammaPolCirc","mc_GammaPolCirc*pol").Snapshot(treename,"minus_plus_snap_test.root",brufit_cols);
  auto df_minus_minus_snap = df_minus_minus.Define("pol",[pol] { return -pol; }).Define("pol_helicity","-1").Redefine("mc_GammaPolCirc","mc_GammaPolCirc*pol").Snapshot(treename,"minus_minus_snap_test.root",brufit_cols);
  gBenchmark->Stop("Snapshots");
  gBenchmark->Print("Snapshots");
  
  gBenchmark->Start("hadd");
  gSystem->Exec("hadd -f pol_mixed.root plus_plus_snap_test.root plus_minus_snap_test.root minus_plus_snap_test.root minus_minus_snap_test.root");
  
  gSystem->Exec("rm *snap_test.root");
  gBenchmark->Stop("hadd");
  gBenchmark->Print("hadd");

  gBenchmark->Start("Shuffle");
  ShuffleTree("pol_mixed.root", outfile, treename);
  gSystem->Exec("rm pol_mixed.root");
  gBenchmark->Stop("Shuffle");
  gBenchmark->Print("Shuffle");
}


void MixHelicityTrees(std::string filebase="/w/work6/home/gp140f/epic_generator_output/rootfiles/18x275_ddvcs_ee", std::string outdirbase="/w/work6/home/gp140f/combirad_trees/HepMC_TCS_18x275"){
  
  // Enable implicit multi-threading
  ROOT::EnableImplicitMT(8);
  gROOT->SetBatch(kTRUE);
  gBenchmark->Start("Total");
  
  std::string plus_file = filebase+"_hplus.hepmc3.tree.root";
  std::string minus_file = filebase+"_hminus.hepmc3.tree.root";
    
  std::string plus_outdir = outdirbase + "_hplus/";
  std::string minus_outdir = outdirbase + "_hminus/";
  std::string mixed_outdir = outdirbase + "_hmixed/";
  
  std::string plus_outfile = plus_outdir+"TCS_mc_Tree.root";
  std::string minus_outfile = minus_outdir+"TCS_mc_Tree.root";
  std::string mixed_outfile = mixed_outdir+"TCS_mc_Tree.root";
  
  gSystem->Exec(Form("mkdir -p %s",plus_outdir.c_str()));
  gSystem->Exec(Form("mkdir -p %s",minus_outdir.c_str()));
  gSystem->Exec(Form("mkdir -p %s",mixed_outdir.c_str()));
		
  if(!checkFileExists(plus_outfile))
    ProcessHEPMCTCSCombi(plus_file, plus_outdir,
			 epic_beam_ele_idx,
			 epic_beam_ion_idx,
			 epic_scat_ele_idx,
			 epic_scat_ion_idx,
			 epic_lep_minus_idx,
			 epic_lep_plus_idx,
			 11);
  
  if(!checkFileExists(minus_outfile))
    ProcessHEPMCTCSCombi(minus_file, minus_outdir,
			 epic_beam_ele_idx,
			 epic_beam_ion_idx,
			 epic_scat_ele_idx,
			 epic_scat_ion_idx,
			 epic_lep_minus_idx,
			 epic_lep_plus_idx,
			 11);

  
  std::string treename="tree";
  
  double pol=0.8;
  int nev=100000;

  if(!checkFileExists(mixed_outfile))
    DFMerge(plus_outfile, minus_outfile, mixed_outfile, treename, pol, nev);
  
  gBenchmark->Stop("Total");
  gBenchmark->Print("Total");
    
} 
