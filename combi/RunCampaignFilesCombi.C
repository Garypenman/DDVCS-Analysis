#include "ePICFileStreamer.h"
#include "../include/FileProcessing.h"

#include "../include/Config.h"
#include "../include/DatasetConfig.h"
#include "../include/Datasets.h"

#include "ProcessRecTruthTCSCombi.C"
#include "ProcessRecTruthTCSSingleTrack.C"

#include "SingleParticleAnalysis.C"

void RunCampaignFilesCombi(const DatasetConfig& d,
			   int nfiles = -1,
			   bool dry_run = false,
			   std::string outbase_override = "")
{
  std::string jlab_redirector = "dtn-eic.jlab.org/";
  std::string bnl_redirector = "epicxrd1.sdcc.bnl.gov:1095/";
  std::string extension = "edm4eic.root";
    
  
  std::string outdir;
  
  if(outbase_override.empty()){
    outdir = "/w/work6/home/gp140f/combirad_trees/Campaign" + d.campaign + "/";
  } else {
    outdir = outbase_override;
  }
  gSystem->Exec(Form("mkdir -p %s",outdir.c_str()));
  
  std::string sdecay = (d.pdg == 11 ? "ee" : "mumu");
  std::string sbkg = (d.bkg == BkgType::None ? "" : "_bkgd");
  std::string shel = (d.helicity == "" ? "" : "_"+d.helicity);

  std::string dataset =  d.config + "_" + d.reaction + "_" + sdecay + shel + sbkg;
    
  std::string dataset_outdir = outdir + dataset + "/";
  gSystem->Exec(Form("mkdir -p %s",dataset_outdir.c_str()));

  std::string outfile            = dataset_outdir + "TCS_tru_Tree.root";
  std::string outfile_acceptance = dataset_outdir + "TCS_scat_ele_tru_Tree.root";

  
  std::cout << "\n====================================\n";
  std::cout << "Dataset: " << dataset << std::endl;
  std::cout << "Campaign: " << d.campaign << std::endl;
  std::cout << "Config: " << d.config << std::endl;
  std::cout << "Decay: " << sdecay << std::endl;
  std::cout << "Helicity: " << d.helicity << std::endl;
  std::cout << "Background: "
	    << (d.bkg == BkgType::None ? "None" : "Yes") << std::endl;
  std::cout << "XRDFS path: " << d.xrdfsPath << std::endl;
  std::cout << "Output dir: " << dataset_outdir << std::endl;
  std::cout << "Output file: " << outfile << std::endl;
  std::cout << "Acceptance file: " << outfile_acceptance << std::endl;  

  if(dry_run){
    std::cout << "[DRY RUN] Skipping XRootD and processing\n";
    return;
  }
  
  auto files = rad::files::GetXRootDFiles(jlab_redirector,
					  "/volatile/"+d.xrdfsPath,
					  extension, nfiles);
  
  if (files.empty()) {

    auto files_bnl = rad::files::GetXRootDFiles(bnl_redirector,
						d.xrdfsPath,
						extension, nfiles);
    
    if (files_bnl.empty()) {
      std::cout << "No files at (JLab + BNL). Skipping.\n";
      return;
    }
    files = files_bnl;
  }
  
  if (!checkFileExists(outfile))
    ProcessRecTruthTCSCombi(files, dataset_outdir, d.beam_ele_idx, d.beam_ion_idx, d.scat_ele_idx, d.scat_ion_idx, d.lep_minus_idx, d.lep_plus_idx, d.pdg);
  
  //only run acceptance for NON-background datasets
  if (d.bkg == BkgType::None) {
    
    if (!checkFileExists(outfile_acceptance))
      ProcessRecTruthTCSSingleTrack(files, dataset_outdir, d.beam_ele_idx, d.beam_ion_idx, d.scat_ele_idx, d.scat_ion_idx, d.lep_minus_idx, d.lep_plus_idx, d.pdg);
    
  }
}


