#include "ProcessHepMCDDVCS.C"

void BatchRAD(){
  string path = "/w/work5/home/garyp/eic/TCS_BH_Test/50k/";
  string indir = path + "/rootfiles/";
  string outdir = path + "/rad_trees/";
  
  string file1 = indir + "18x275_ddvcs_edecay_hplus.root";
  string file1_out = outdir + "HepMC_ddvcs_ee_18x275_plus.root";
  
  string file2 = indir + "18x275_ddvcs_edecay_hminus.root";
  string file2_out = outdir + "HepMC_ddvcs_ee_18x275_minus.root";
  
  string file3 = indir + "18x275_ddvcsonly_edecay_hplus.root";
  string file3_out = outdir + "HepMC_ddvcsonly_ee_18x275_plus.root";
  
  string file4 = indir + "18x275_ddvcs_edecay_bh_hplus.root";
  string file4_out = outdir + "HepMC_ddvcs_ee_18x275_bh_plus.root";
  
  ProcessHepMCDDVCS(file1,file1_out);
  ProcessHepMCDDVCS(file2,file2_out);
  ProcessHepMCDDVCS(file3,file3_out);
  ProcessHepMCDDVCS(file4,file4_out);
  
}
