//hepmc includes
#include "MixHelicityTrees.C"

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
	//MixHelicityTrees(filebase,outdirbase);

      }
    }
  }

}
  

void RunCampaigns(){
  std::vector<string> campaigns;
  campaigns.push_back("26.02.0");
  campaigns.push_back("26.03.1");
  campaigns.push_back("26.04.1");

  for (auto& campaign : campaigns){
    ;
    //RunCampaignFilesCombi(campaign);
  }
  
}
void RunEverything(){
  
  RunHepMC();
  //RunCampaigns();
  
}
