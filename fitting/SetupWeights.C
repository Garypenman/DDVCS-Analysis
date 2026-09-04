void SetupWeights(std::string phs_dir = "/w/work6/home/gp140f/phasespace/trees/dilep_phsp_10_100/"){

  std::string phs_file = phs_dir + "/TCS_mc_Tree_brufit.root";
  std::string out_file = phs_dir + "/ImpSampleWeights.root";
  TFile *rad_file = new TFile(phs_file.c_str(), "OPEN");
  TTree *rad_tree = (TTree*) rad_file->Get("tree");

  Weights* wgtsData=new Weights("bruWeights");
  wgtsData->SetFile(out_file.c_str());
  wgtsData->SetSpecies("PhaseSpace");
  wgtsData->SetIDName("UID");
 
  wgtsData->WeightBySelection(rad_tree,"(mc_s_photo>0.0)","BruWeight");
  //wgtsData->SortWeights();  //this might not be needed and can eat up a lot of memory....
  wgtsData->Save();//Save to disc
  delete wgtsData; 
}
