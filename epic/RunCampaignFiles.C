#include "ePICFileStreamer.h"
#include "../include/Config.h"

#include "ProcessMCMatched.C"
#include "ProcessMCMatchedDetector.C"
#include "ProcessMCRecon.C"
#include "ProcessMCReconDetector.C"

//#include <podio/ROOTReader.h>
//#include <podio/CollectionBase.h>


struct Task {
  //std::function<void(const std::string&, const std::string&)> func;
  std::function<void( std::vector<std::string>&, const std::string&, const int, const int, const int, const int, const int)> func;
  std::string name; // Derived name for output files
  
  Task(std::function<void( std::vector<std::string>&, const std::string&, const int, const int, const int, const int, const int)> func, std::string name) : func(func), name(name) {}
};

  
void RunCampaignFiles(std::string campaign="25.10.3",
		      std::string config="18x275")
{
  
  //doesnt change
  std::string redirector = "dtn-eic.jlab.org/";
  std::string extension = "edm4eic.root";
  auto nfiles = -1;
  
  //make local output directory for this campaign, if it doesnt already exist
  std::string outdir = "/w/work5/home/garyp/rad_trees/Campaign25.10.3/";
  gSystem->Exec(Form("mkdir -p %s",outdir.data()));
  
  // Build the list with derived names
  std::vector<Task> tasks = {
    //Task(ProcessMCMatched, "MCMatched"),
    Task(ProcessMCMatchedDetector, "MCMatchedDetector"),
    //Task(ProcessMCRecon, "MCRecon"),
    Task(ProcessMCReconDetector, "MCReconDetector")
  };
  
  
  std::vector<std::string> reacs = {"ddvcs","jpsi"};
  std::vector<std::string> decays = {"edecay","mudecay"};
  std::vector<std::string> helicities;
  
  for (auto& reac : reacs){
    int scat_ele_idx, scat_ion_idx, lep_minus_idx, lep_plus_idx;
    if(reac=="ddvcs"){
      helicities = {"hplus","hminus"};
      scat_ele_idx = edm4hep_epic_scat_ele_idx;
      scat_ion_idx = edm4hep_epic_scat_ion_idx;
      lep_minus_idx = edm4hep_epic_lep_minus_idx;
      lep_plus_idx = edm4hep_epic_lep_plus_idx;
      }
    
    if(reac=="jpsi"){
      helicities = {""};
      scat_ele_idx = edm4hep_eslight_scat_ele_idx;
      scat_ion_idx = edm4hep_eslight_scat_ion_idx;
      lep_minus_idx = edm4hep_eslight_lep_minus_idx;
      lep_plus_idx = edm4hep_eslight_lep_plus_idx;
    }

    for (auto& decay : decays){
      int pdg=-1;
      std::string sdecay;
      if(decay == "edecay"){
	pdg = 11;
	sdecay = "ee";
      }
      if(decay == "mudecay"){
	pdg = 13;
	sdecay = "mumu";
      }
	
      for (auto& hel : helicities){
  	//build paths from input info
  	std::string xrdfsPath;
  	std::string basepath = "/volatile/eic/EPIC/RECO/"+campaign+"/epic_craterlake/EXCLUSIVE/";
      
  	if(reac=="ddvcs")
  	  xrdfsPath = basepath + "DDVCS_ABCONV/EpIC1.1.6-1.0/"+config+"/q2_0_10/"+decay+"/"+hel+"/";
  	if(reac=="jpsi")
  	  xrdfsPath = basepath + "PHOTOPRODUCTION_JPSI_ABCONV/coherent_ep/eSTARlight1.3.0-1.0/"+config+"/q2_nocut/"+decay+"/";
      
  	for (auto& task : tasks) {
	
  	  std::string outputfile = outdir + task.name + "_" + reac + "_" + sdecay + "_" + config + "_" + hel + ".root";
  	  
	  //fix any "_.root" from jpsi with no helicity
	  size_t pos;
	  while ((pos = outputfile.find("_.root")) != std::string::npos) {
	    outputfile.replace(pos, 6, ".root"); // 6 = length of "_.root"
	  }
	  
	  if(!checkFileExists(outputfile)){
	    //std::cout << "Input path: " << files[0] << std::endl;
	    std::cout << "Processing files for " + outputfile << "\n" << std::endl;
	    auto files = rad::files::GetXRootDFiles(redirector,xrdfsPath,extension,nfiles);
	    
	    if (files.empty() == true){
	      std::cout << "No files for " + xrdfsPath + "! Skipping." << std::endl;
	      continue;
	    }
	    
	    task.func(files, outputfile,scat_ele_idx, scat_ion_idx, lep_minus_idx, lep_plus_idx, pdg);
	  }
  	}
      }
    }
  }  
}
