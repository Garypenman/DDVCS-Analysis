#include "ProcessRecTruthTCSSingleTrack.C"
#include "ProcessRecTruthTCSCombi.C"

#include "SingleParticleAnalysis.C"
//#include "ExclusiveAnalysis.C"

void RunBGMergedRAD(){

  //gErrorIgnoreLevel = kWarning;
  gErrorIgnoreLevel = kError;
  
  //No Background (control dataset):
  //only first 10 recon files used for merge (100k + 200M) sample
  std::vector<std::string> infiles;
  std::string indir = "/w/work6/home/gp140f/9x130_ddvcs_ee_hplus/recon/";
  std::string nobg_outdir = "/w/work6/home/gp140f/tcs_bg_merge/nobg_rad/";
  std::string nobg_resultdir = "./results/NoBGMerge/9x130/";
  for (int i=0; i<10; i++){
    infiles.push_back(indir+"9x130_ddvcs_ee_hplus_"+to_string(i)+"_recon.root");
  }
  
  //Background merged
  //std::vector<std::string> merged_infiles;
  std::string bg_indir = "/w/work6/home/gp140f/tcs_bg_merge/recon_out/";
  std::string bg_outdir = "/w/work6/home/gp140f/tcs_bg_merge/bg_rad/";
  std::string bg_resultdir = "./results/BGMerge/9x130/";
  //merged_infiles.push_back(bg_indir+"reco_seg_*.root");
  std::string merged_infiles = bg_indir+"reco_seg_*.root";
  

  //RAD Macros
  if(!checkFileExists(nobg_outdir+"TCS_scat_ele_rec_Tree.root"))
    ProcessRecTruthTCSSingleTrack(infiles, nobg_outdir);
  
  
  if(!checkFileExists(nobg_outdir+"TCS_all_rec_Tree.root"))
    ProcessRecTruthTCSCombi(infiles, nobg_outdir);
  
  
  if(!checkFileExists(bg_outdir+"TCS_scat_ele_rec_Tree.root"))
    ProcessRecTruthTCSSingleTrack(merged_infiles, bg_outdir);
  
  
  if(!checkFileExists(bg_outdir+"TCS_all_rec_Tree.root"))
    ProcessRecTruthTCSCombi(merged_infiles, bg_outdir);
  

  
  //Analysis Macros
  SingleParticleAnalysis(nobg_outdir, nobg_resultdir, 9, 130, "hplus", "26.07.1");
  SingleParticleAnalysis(bg_outdir, bg_resultdir, 9, 130, "hplus", "26.07.1");

  //ExclusiveAnalysis(nbg_outdir, nobg_resultdir, 9, 130, "hplus", "26.07.1");
  //ExclusiveAnalysis(bg_outdir, bg_resultdir, 9, 130, "hplus", "26.07.1");
  
}
