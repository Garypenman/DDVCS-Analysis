struct datafile{
  std::string file;
  std::string label;
  TH1D* ht;
  TH1D* hQ2;
  TH1D* hs;
  TH1D* hQp2;

  TH2D* h2d_L_t;
  TH2D* h2d_L_Qp2;
  TH2D* h2d_L_Q2;
  TH2D* h2d_L_s;
  TH2D* h2d_L_CosTheta;
  TH2D* h2d_L_Phi;
  
  
  Color_t color;
  
  datafile(std::string ffile,
	   std::string flabel)
    : file(ffile), label(flabel)
  {
    
  }
};


void DrawWithTitle (TH2 *h, const char *title) {
  h->SetTitle(title);
  h->Draw("colz");
  TLatex l;
  l.SetNDC();
  l.SetTextSize(0.04);
  l.DrawLatex(0.12, 0.92, title);
};


void Loop(datafile& f){

  gStyle->SetHistLineColor(f.color);
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  
  ROOT::EnableImplicitMT(8);
  ROOT::RDataFrame df_base("tree",f.file.c_str());
  auto df_filt = df_base.Filter("mc_Qp2>0"); //dummy filter to get correct return type
  auto df = df_filt
    .Define("log10_t",   "log10(mc_t_bot)")
    .Define("log10_Q2",   "log10(mc_Q2)")
    .Define("log10_s",   "log10(mc_s_photo)")
    .Define("invL",      "1.0 / L")
    .Define("log10_invL","log10(invL)");
	    
    
  std::string weight;
  if(f.label!="epic")
    weight="invL";
  else
    weight="1";

  df = df.Define("weight",weight);
  
    //df = df.Filter("mc_t_bot<=1");
  
  auto ht = df.Histo1D({
      ("ht_"+f.label).c_str(),
      (f.label+";|t| [GeV^{2}];").c_str(),
      100,0,2},"mc_t_bot");

  auto hQ2 = df.Histo1D({
      ("hQ2_"+f.label).c_str(),
      (f.label+";Q^{2} [GeV^{2}];").c_str(),
      100,0,1},"mc_Q2");

  auto hQp2 = df.Histo1D({
      ("hQp2_"+f.label).c_str(),
      (f.label+";Q'^{2} [GeV^{2}];").c_str(),
      100,0,20},"mc_Qp2");

  auto hs = df.Histo1D({
      ("hs_"+f.label).c_str(),
      (f.label+";s [GeV^{2}];").c_str(),
      100,0,4000},"mc_s_photo");

  auto h2d_L_t = df.Histo2D({
      ("h2d_L_t"+f.label).c_str(),
      (f.label+";log_{10}(|t|) [GeV^{2}];log_{10}(1/L)").c_str(),
      100,-4.4,0.6,
      100,-2.4,3.8},
    "log10_t",
    "log10_invL",
    "weight");

  auto h2d_L_Qp2 = df.Histo2D({
      ("h2d_L_Qp2"+f.label).c_str(),
      (f.label+";Q'^{2} = M_{ee}^{2} [GeV^{2}];log_{10}(1/L)").c_str(),
      100,0,20,
      100,-2.4,3.8},
    "mc_Qp2",
    "log10_invL",
    "weight");

  auto h2d_L_Q2 = df.Histo2D({
      ("h2d_L_Q2"+f.label).c_str(),
      (f.label+";Q'^{2} = log_{10}(Q^{2}) [GeV^{2}];log_{10}(1/L)").c_str(),
      100,-10,1.5,
      100,-2.4,3.8},
    "log10_Q2",
    "log10_invL",
    "weight");

  auto h2d_L_s = df.Histo2D({
      ("h2d_L_s"+f.label).c_str(),
      (f.label+";log_{10}(s) [GeV^{2}];log_{10}(1/L)").c_str(),
      100,-4.4,4.4,
      100,-2.4,3.8},
    "log10_s",
    "log10_invL",
    "weight");

  ht->Scale(1.0 / ht->Integral());
  hQ2->Scale(1.0 / hQ2->Integral());
  hQp2->Scale(1.0 / hQp2->Integral());
  hs->Scale(1.0 / hs->Integral());

  ht->SetMinimum(1e-6);
  hQ2->SetMinimum(1e-6);
  hQp2->SetMinimum(1e-6);
  hs->SetMinimum(1e-6);
  
  f.ht = (TH1D*)ht->Clone(ht->GetName());
  f.hQ2 = (TH1D*)hQ2->Clone(hQ2->GetName());
  f.hQp2 = (TH1D*)hQp2->Clone(hQp2->GetName());
  f.hs = (TH1D*)hs->Clone(hs->GetName());

  f.h2d_L_t = (TH2D*)h2d_L_t->Clone(h2d_L_t->GetName());
  f.h2d_L_Qp2 = (TH2D*)h2d_L_Qp2->Clone(h2d_L_Qp2->GetName());
  f.h2d_L_s = (TH2D*)h2d_L_s->Clone(h2d_L_s->GetName());
  
}
void CompareElspectroEpic(){

  std::string file1 = "/w/work6/home/gp140f/combirad_trees/HepMC_TCS_10x100_hplus/TCS_mc_Tree_brufit.root";
  std::string file2 = "/w/work6/home/gp140f/combirad_trees/HepMC_TCS_10x100_Phasespace/TCS_mc_Tree_brufit.root";
  std::string file3 = "/w/work6/home/gp140f/combirad_trees/HepMC_TCS_10x100_Phasespace_t1/TCS_mc_Tree_brufit.root";
  
  datafile epic(file1, "epic");
  datafile els(file2, "elspectro");
  datafile els1(file3, "elspectro_t1");

  epic.color = kBlack;
  els.color = kP6Red;
  els1.color = kP6Blue;
  
  std::vector<datafile> fvec;
  fvec.push_back(epic);
  fvec.push_back(els);
  fvec.push_back(els1);
  
  for (auto& f: fvec){
    Loop(f);
  }

  TCanvas *ct = new TCanvas();
  TCanvas *cs = new TCanvas();
  TCanvas *cQ2 = new TCanvas();
  TCanvas *cQp2 = new TCanvas();
  TCanvas *c00 = new TCanvas("c00","2D L v |t|",2000,1400);
  c00->Divide(2,2);

  TCanvas *c01 = new TCanvas("c01","2D L v M2ee",2000,1400);
  c01->Divide(2,2);

  TCanvas *c02 = new TCanvas("c02","2D L v s",2000,1400);
  c02->Divide(2,2);

  int iter=1;
  Option_t* drawopts = "hist";
  for (auto& f: fvec){
    if(iter>1) drawopts = "hist same";
    ct->cd()->SetLogy();
    f.ht->Draw(drawopts);
    cQ2->cd()->SetLogy();
    f.hQ2->Draw(drawopts);
    cQp2->cd()->SetLogy();
    f.hQp2->Draw(drawopts);
    cs->cd()->SetLogy();
    f.hs->Draw(drawopts);

    c00->cd(iter)->SetLogz();
    DrawWithTitle(f.h2d_L_t,f.label.c_str());
    c01->cd(iter)->SetLogz();
    DrawWithTitle(f.h2d_L_Qp2,f.label.c_str());
    c02->cd(iter)->SetLogz();
    DrawWithTitle(f.h2d_L_s,f.label.c_str());
    
    iter++;
  }

  
  TCanvas *csummary = new TCanvas("csummary","Summary",2000,1400);
  csummary->Divide(2,2);
  csummary->cd(1);
  ct->DrawClonePad();
  gPad->BuildLegend(0.7,0.7,0.9,0.9);
  csummary->cd(2);
  cQ2->DrawClonePad();
  csummary->cd(3);
  cs->DrawClonePad();
  csummary->cd(4);
  cQp2->DrawClonePad();
  
  ct->Close();
  cQ2->Close();
  cQp2->Close();
  cs->Close();

  
}
