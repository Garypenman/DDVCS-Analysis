#include "ePICFileStreamer.h"
#include "../include/FileProcessing.h"

#include "../include/Config.h"
#include "../include/DatasetConfig.h"

#include "ProcessRecTruthTCSCombi.C"
#include "ProcessRecTruthTCSSingleTrack.C"

#include "SingleParticleAnalysis.C"

void RunCampaignFilesCombi(std::string campaign="26.03.1",
			   std::string config="18x275",
			   int nfiles = -1)
{
  
  //doesnt change
  std::string jlab_redirector = "dtn-eic.jlab.org/";
  std::string bnl_redirector = "epicxrd1.sdcc.bnl.gov:1095/";
  std::string extension = "edm4eic.root";
  
  //make local output directory for this campaign, if it doesnt already exist
  std::string outdir = "/w/work6/home/gp140f/combirad_trees/Campaign"+campaign+"/";
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
    std::string outfile_acceptance = dataset_outdir + "TCS_scat_ele_tru_Tree.root";
    
    auto files = rad::files::GetXRootDFiles(jlab_redirector, "/volatile/"+d.xrdfsPath, extension, nfiles);
    auto files2 = rad::files::GetXRootDFiles(bnl_redirector, d.xrdfsPath, extension, nfiles);
    
    if (files.empty()) {
      std::cout << "No files at " << "volatile/"+d.xrdfsPath << ". Checking BNL.\n";

      if (files2.empty()) {
	std::cout << "No files at " << d.xrdfsPath << ". Skipping.\n";
	continue;
      }else{files=files2;}
    }
    
    if (!checkFileExists(outfile))
      ProcessRecTruthTCSCombi(files, dataset_outdir, d.beam_ele_idx, d.beam_ion_idx, d.scat_ele_idx, d.scat_ion_idx, d.lep_minus_idx, d.lep_plus_idx, d.pdg);
    if (!checkFileExists(outfile_acceptance))
      ProcessRecTruthTCSSingleTrack(files, dataset_outdir, d.beam_ele_idx, d.beam_ion_idx, d.scat_ele_idx, d.scat_ion_idx, d.lep_minus_idx, d.lep_plus_idx, d.pdg);
    
  }
}
