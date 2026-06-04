#include "../include/Config.h"
#include "/home/gp140f//epic_plot_macro/ePIC_style.C"

std::string plus_dir = "/w/work6/home/gp140f/combirad_trees/Campaign26.03.1/ddvcs_ee_18x275_hplus/";
std::string minus_dir = "/w/work6/home/gp140f/combirad_trees/Campaign26.03.1/ddvcs_ee_18x275_hminus/";

void ExclusiveAnalysis(std::string infiledir = plus_dir, std::string beam_setting = "18x275"){

  // ----------------------------------------------------------
  // Output
  // ----------------------------------------------------------
  std::string outfilename = "TCS_"+beam_setting+"_exclusive_analysis.root";
  TFile fout(outfilename.c_str(), "RECREATE");
  
  std::string tru_filename = infiledir + "TCS_tru_Tree.root";
  std::string rec_filename = infiledir + "TCS_all_rec_Tree.root";

  // DataFrames
  ROOT::RDataFrame df_tru("tree", tru_filename);
  ROOT::RDataFrame df_rec_base("tree", rec_filename);
  auto df_rec_tru = df_rec_base.Filter("isTruth==1");
  
  auto hrec_MissMass2 = df_rec_base.Histo1D({"hrec_MissMass2","",250,-15,15},"rec_MissMass2");
  auto hrec_MissMass2_tru = df_rec_tru.Histo1D({"hrec_MissMass2_tru","",250,-15,15},"rec_MissMass2");
  
  //hrec_MissMass2->Fit("gaus","");
  hrec_MissMass2->DrawCopy();
  hrec_MissMass2_tru->SetLineColor(kRed);
  hrec_MissMass2_tru->DrawCopy("same");
  //gPad->SetLogy();
  
}
