#include "../include/Config.h"

#include "../hepmc/ProcessHepMC.C"
//#include "../epic/ProcessMCMatched.C"
//#include "../epic/ProcessMCMatchedDetector.C"
//#include "../epic/ProcessDetector.C"

void ProcessHepMCElspectroJPsi(const std::string config="18x275"){
  std::string ee_infile = farm_dir+"/data/JPsi_elspectro_"+config+"/rootfiles/elspectro_jpsi_"+config+".root";
  //std::string mumu_infile = farm_dir+"/data/JPsi_elspectro_mumu_"+config+"/rootfiles/elspectro_jpsi_mumu_"+config+".root";
  
  std::string ee_outfile = out_dir+"HepMC_elspectro_jpsi_ee_"+config+".root";
  //std::string mumu_outfile = out_dir+"HepMC_elspectro_jpsi_mumu_"+config+".root";
    
  ProcessHepMC(ee_infile, ee_outfile, elspectro_beam_ele_idx, elspectro_beam_ion_idx, elspectro_scat_ele_idx, elspectro_scat_ion_idx, elspectro_lep_plus_idx, elspectro_lep_minus_idx);

  //ProcessHepMC(mumu_infile, mumu_outfile, elspectro_beam_ele_idx, elspectro_beam_ion_idx, elspectro_scat_ele_idx, elspectro_scat_ion_idx, elspectro_lep_plus_idx, elspectro_lep_minus_idx);

}

void ProcessHepMCEstarlightJPsi(const std::string config="18x275"){
  std::string ee_infile = farm_dir+"/data/JPsi_eslight_"+config+"/rootfiles/eslight_jpsi_"+config+".root";
  std::string mumu_infile = farm_dir+"/data/JPsi_eslight_mumu_"+config+"/rootfiles/eslight_jpsi_mumu_"+config+".root";
  
  std::string ee_outfile = out_dir+"HepMC_eslight_jpsi_ee_"+config+".root";
  std::string mumu_outfile = out_dir+"HepMC_eslight_jpsi_mumu_"+config+".root";
    
  ProcessHepMC(ee_infile, ee_outfile, eslight_beam_ele_idx, eslight_beam_ion_idx, eslight_scat_ele_idx, eslight_scat_ion_idx, eslight_lep_plus_idx, eslight_lep_minus_idx);

  ProcessHepMC(mumu_infile, mumu_outfile, eslight_beam_ele_idx, eslight_beam_ion_idx, eslight_scat_ele_idx, eslight_scat_ion_idx, eslight_lep_plus_idx, eslight_lep_minus_idx);

}
