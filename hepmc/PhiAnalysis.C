void PhiAnalysis(const std::string infile = "/w/work6/home/gp140f/combirad_trees/HepMC_TCS_18x275_hplus/TCS_mc_Tree_brufit.root",
		 const std::string outdir = "phys_phi")
{
  gSystem->Exec(Form("mkdir -p %s",outdir.c_str()));

  //ROOT::EnableImplicitMT(2);
  ROOT::RDataFrame df("tree", infile.c_str());

  auto h2d_phi_L = df.Histo2D({"h2d_phi_L",";L (Lepton Propagator) [GeV^{2}];#phi_{l} [rad]",100,0,70,100,-TMath::Pi(),TMath::Pi()},"L","mc_PhiHel");

  h2d_phi_L->DrawCopy();
  gPad->SetLogz();
  gPad->Print("h2d_phi_L.png");

  int n=4;
  int m=n*n;
  for(int i=1; i<m+1; i++){

    //make 16 1bin Y-projections
  }
  //draw 4x4 canvas of projections
  //print
  gSystem->Exec(Form("mv *.png %s",outdir.c_str()));
}

void RunFiles(const std::string config = "18x275")
{
  const std::string Fullfile = "/w/work6/home/gp140f/combirad_trees/HepMC_TCS_18x275_hplus/TCS_mc_Tree_brufit.root";
  const std::string BHfile = "/w/work6/home/gp140f/eic/TCS_BH_Test/50k/combirad_trees/TCS_mc_Tree_brufit.root";
  const std::string PSfile = "/w/work6/home/gp140f/combirad_trees/HepMC_TCS_18x275_Phasespace_t0.5/TCS_mc_Tree_brufit.root";

  const std::string out1 = "phys_phi";
  const std::string out2 = "bh_phi";
  const std::string out3 = "ps_phi";
  
  PhiAnalysis(Fullfile, out1);
  PhiAnalysis(BHfile, out2);
  PhiAnalysis(PSfile, out3);

}
