//hepmc includes
#include "MixHelicityTrees.C"
#include "SnapshotBrufit.C"

//epic sim includes
#include "RunCampaignFilesCombi.C"

void RunHepMC(){

  
  std::vector<std::string> configs = {
    "10x100", "10x130", "18x275",
    "9x100", "9x130", "9x275"
  };
  
  std::vector<std::string> decays = {
    "ee", "mumu"
  };
  
  std::vector<std::string> modes = {
    "all", "bhonly"
  };
  
  for (auto& conf : configs){
    for (auto& decay : decays){
      for (auto& mode : modes){

	std::string data_path = "/w/work6/home/gp140f/epic_generator_output/DDVCS/rootfiles/";
	std::string filebase;
	
	if(mode == "all"){
	  filebase = data_path + conf + "_ddvcs_" + decay;
	} else {
	  filebase = data_path + conf + "_ddvcs_" + decay + "_" + mode;
	}
        
	std::string plusfile = filebase+"_hplus.hepmc3.tree.root";
	std::string minusfile = filebase+"_hminus.hepmc3.tree.root";
            
	if(!(checkFileExists(plusfile) && checkFileExists(minusfile)))
	  continue;

	std::string output_path = "/w/work6/home/gp140f/combirad_trees/";
	std::string outdirbase;
	
	if(mode == "all"){
	  outdirbase = output_path + conf + "_ddvcs_" + decay;
	} else {
	  outdirbase = output_path + conf + "_ddvcs_" + decay + "_" + mode;
	}

	std::cout << "Mixing filebase: " + filebase + " into outdirbase " << outdirbase << std::endl;
	MixHelicityTrees(filebase,outdirbase);

      }
    }
  }

}


void RunPhasespace(){

  std::string basedir = "/w/work6/home/gp140f/phasespace/";
  std::string filedir = basedir + "rootfiles/";
  std::string outbase = basedir + "trees/";

  std::vector<std::pair<std::string,std::string>> files = {
    {"0", "dilep_phsp_18_275"},
    {"1", "dilep_phsp_18_275_v2"},
    {"2", "dilep_phsp_18_275_M1.5GeV"},
    {"3", "dilep_phsp_10_100"},
    {"4", "dilep_phsp_10_100_t1"},
    {"5", "dilep_phsp_10_100_b5_10M"}
  };

  for(const auto& [tag, file] : files){
    auto inputfile = filedir + file + ".root";
    auto outdir = outbase + file + "/";
    auto outputfile = outdir + "TCS_mc_Tree.root";
    auto brufitfile = outdir + "TCS_mc_Tree_brufit.root";
    
    if( checkFileExists(inputfile) && !checkFileExists(outputfile)){
      cout << "Processing " + tag + ": " + inputfile << endl;
      cout << "Output: " + outdir << endl;
      ProcessElspectro(inputfile, outdir);
    }

    if( !(checkFileExists(brufitfile)) ){
      cout << "Processing: " + outputfile << endl;
      cout << "Output: " + brufitfile << endl;
      SnapshotBrufit(outputfile, brufitfile);
    }
  }
  
}


void RunCampaigns(bool dry_run=true){
  auto datasets = GetDatasets();
  for (const auto& d : datasets){    
    RunCampaignFilesCombi(d,-1,dry_run);
  }
}

void RunCampaignTest(){

  std::string test_out = "/w/work6/home/gp140f/combirad_trees/test_run/";

  //std::vector<DatasetConfig> test_datasets = {

  auto nobg = DatasetConfig("ddvcs","edecay","hplus","18x275","26.02.0",
			    BkgType::None);

  auto bg = DatasetConfig("ddvcs","edecay","hplus","18x275","26.02.0",
			  BkgType::Exact1S_2us_GoldCt_5um);

  auto april_p = DatasetConfig("ddvcs","edecay","hplus","10x130","26.04.1",
			       BkgType::None);
  auto april_m = DatasetConfig("ddvcs","edecay","hminus","10x130","26.04.1",
			       BkgType::None);

  //RunCampaignFilesCombi(nobg, 10, false, test_out);
  //RunCampaignFilesCombi(bg, 1000, false, test_out);

  RunCampaignFilesCombi(april_p, -1, false);
  RunCampaignFilesCombi(april_m, -1, false);
  
  //};

  // for(const auto& d : test_datasets){

  //   std::cout << "\n=== TEST RUN ===\n";

    // RunCampaignFilesCombi(
    //   d,
    //   10,             // <-- only 10 files
    //   false,          // dry run OFF (real run)
    //   test_out        // <-- force test directory
  //   );
  // }
}


void RunEverything(){
  
  //RunPhasespace();
  //RunHepMC();
  RunCampaignTest();
  //RunCampaigns();
  
}
