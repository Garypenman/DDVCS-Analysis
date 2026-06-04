void RunAnalysis(const std::string infile = "/w/work6/home/gp140f/combirad_trees/HepMC_TCS_18x275_hplus/TCS_mc_Tree_brufit.root",
		 const std::string outdir = "phys_phi")
{
  gSystem->Exec(Form("mkdir -p %s",outdir.c_str()));

  //ROOT::EnableImplicitMT(2);
  ROOT::RDataFrame df_base("tree", infile.c_str());
  auto df = df_base.Filter("mc_t_bot<2.0");
  
  //df = df.Filter("mc_t_bot<0.2").Filter("mc_Qp2>4.0");
  //df = df.Filter("mc_s_photo<50");
  
  auto h2d_phi_L = df.Histo2D({"h2d_phi_L",";L (Lepton Propagator) [GeV^{2}];#phi_{l} [rad]",100,0,70,100,-TMath::Pi(),TMath::Pi()},"L","mc_PhiHel");
  auto h2d_Qp2_L = df.Histo2D({"h2d_Qp2_L",";L (Lepton Propagator) [GeV^{2}];Q'^{2} [GeV^{2}]",100,0,70,100,0,20},"L","mc_Qp2");
  auto h2d_t_L = df.Histo2D({"h2d_t_L",";L (Lepton Propagator) [GeV^{2}];|t| [GeV^{2}]",100,0,70,100,0,2},"L","mc_t_bot");

  h2d_phi_L->DrawCopy();
  gPad->SetLogz();
  gPad->Print("h2d_phi_L.png");
  
  h2d_Qp2_L->DrawCopy();
  gPad->SetLogz();
  gPad->Print("h2d_Qp2_L.png");

  h2d_t_L->DrawCopy();
  gPad->SetLogz();
  gPad->Print("h2d_Qp2_L.png");

  int n=4;
  int m=n*n;
  int nxbins = h2d_phi_L->GetNbinsX();
  int binsPerSlice = nxbins / m;
  std::vector<TH1D*> projY;
  projY.reserve(m);
  // ---- make m = n x n Y-projections ----
  for (int i = 0; i < m; i++) {
    
    int binLow  = i * binsPerSlice + 1;
    int binHigh = (i == m-1)
      ? nxbins
      : (i+1) * binsPerSlice;
    
    TString name  = Form("h_phi_slice_%02d", i);
    TString title = Form("#phi_{l} projection %d;#phi_{l} [rad];Counts", i);
      
    //TH1D *hproj = h2d_phi_L->ProjectionY(name, binLow, binHigh);
    TH1D *hproj = h2d_phi_L->ProjectionY(name, i+1, i+2);
    hproj->SetTitle(title);
      
    projY.push_back(hproj);
  }
  
  // ---- draw on 4x4 canvas ----
  TCanvas *c = new TCanvas("c_proj", "Phi projections vs L", 1200, 1200);
  c->Divide(n, n);
  
  for (int i = 0; i < m; i++) {
    c->cd(i+1);
    projY[i]->Draw();
  }
  
  // ---- save ----
  c->Print("phi_L_projections.png");

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
  
  RunAnalysis(Fullfile, out1);
  RunAnalysis(BHfile, out2);
  RunAnalysis(PSfile, out3);

}

void PhiAnalysis(){
  RunFiles();
}
