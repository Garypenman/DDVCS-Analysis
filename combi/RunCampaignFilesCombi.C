#include "ePICFileStreamer.h"
#include "../include/Config.h"
#include "../include/FileProcessing.h"

#include "DatasetConfig.h"
#include "ProcessRecTruthTCSCombi.C"


void RunCampaignFilesCombi(std::string campaign="26.02.0",
			   std::string config="18x275",
			   int nfiles = -1)
{
  
  //doesnt change
  std::string redirector = "dtn-eic.jlab.org/";
  std::string extension = "edm4eic.root";
  
  //make local output directory for this campaign, if it doesnt already exist
  std::string outdir = "/w/work5/home/garyp/combirad_trees/Campaign"+campaign+"/";
  gSystem->Exec(Form("mkdir -p %s",outdir.data()));
  
  //add datasets here by hand, easy for adding new analyses
  std::vector<DatasetConfig> datasets;
  datasets.emplace_back("ddvcs", "edecay", "hplus", "18x275", campaign);
  datasets.emplace_back("ddvcs", "edecay", "hminus", "18x275", campaign);

  
  // ---- Processing loop ----
  for (auto& d : datasets) {
    
    std::string sdecay = (d.pdg == 11 ? "ee" : "mumu");
    
    std::string dataset = d.reaction + "_" + sdecay + "_" +
      d.config + "_" + d.helicity;
    
    std::string dataset_outdir = outdir + "/" + dataset + "/";
    gSystem->Exec(Form("mkdir -p %s",dataset_outdir.c_str()));
    std::string outfile = dataset_outdir + "TCS_tru_Tree.root";
    
    if (checkFileExists(outfile)) continue;
    
    auto files = rad::files::GetXRootDFiles(redirector, d.xrdfsPath, extension, nfiles);
    
    if (files.empty()) {
      std::cout << "No files at " << d.xrdfsPath << ". Skipping.\n";
      continue;
    }
    
    ProcessRecTruthTCSCombi(files, dataset_outdir, d.beam_ele_idx, d.beam_ion_idx, d.scat_ele_idx, d.scat_ion_idx, d.lep_minus_idx, d.lep_plus_idx, d.pdg);
  }
}
