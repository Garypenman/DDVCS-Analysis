void comparelep(){

  TString efile = "/w/work5/home/garyp/rad_trees/JPsi_eslight_330k_out.root";
  TString mfile = "/w/work5/home/garyp/rad_trees/JPsi_eslight_mumu_18x275_out.root";
  
  TFile *ef = TFile::Open(efile);
  TTree *eT = (TTree*)ef->Get("rad_tree");
			  
  TFile *mf = TFile::Open(mfile);
  TTree *mT = (TTree*)mf->Get("rad_tree");
  
  //probably need ePIC style macro
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  
  gStyle->SetPaperSize(20,26);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadRightMargin(0.05);
  
  gStyle->SetTitleSize(0.05, "XYZ");
  gStyle->SetLabelSize(0.04, "XYZ");
  
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(1.2);
  gStyle->SetHistLineWidth(2.);
  gStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes

  gStyle->SetEndErrorSize(0.);
  
  gStyle->SetTextFont(42);
  gStyle->SetLabelFont(42,"XYZ");
  gStyle->SetTitleFont(42,"XYZ");
  
  gStyle->SetPalette(kBird);
  
  TLatex tex;
  tex.SetTextSize(0.05);
  tex.SetTextAlign(13);
  tex.SetNDC();
  
  TCut ele_recon = "rec_pmag[ele]>0";
  TCut pos_recon = "rec_pmag[pos]>0";
  TCut gprime_recon = ele_recon && pos_recon;
  
  eT->Draw("rec_GMass>>hrec_Mee(100,2,4.5)",gprime_recon,"goff");
  mT->Draw("rec_GMass>>hrec_Mmumu(100,2,4.5)",gprime_recon,"goff");
  
  auto hrec_Mee = (TH1D*) gDirectory->Get("hrec_Mee");
  auto hrec_Mmumu = (TH1D*) gDirectory->Get("hrec_Mmumu");
  
  TF1 *fgaus = new TF1("fgaus","gaus");
  
  eT->Draw("tru_GMass-rec_GMass>>hres_Mee(100,-0.5,0.5)",gprime_recon,"goff");
  mT->Draw("tru_GMass-rec_GMass>>hres_Mmumu(100,-0.5,0.5)",gprime_recon,"goff");
  auto hres_Mee = (TH1D*)gDirectory->Get("hres_Mee");
  auto hres_Mmumu = (TH1D*)gDirectory->Get("hres_Mmumu");
  
  hres_Mee->Fit(fgaus,"QN","",-0.05,0.05);
  double ee_mean = fgaus->GetParameter(1);
  double ee_sig = fgaus->GetParameter(2);
  
  hres_Mmumu->Fit(fgaus,"QN","",-0.05,0.05);
  double mumu_mean = fgaus->GetParameter(1);
  double mumu_sig = fgaus->GetParameter(2);
  
  new TCanvas();
  hres_Mee->SetTitle("Resolution;#DeltaM_{e^{+}e^{-}} [GeV/c^{2}];Events / 0.01 GeV/c^{2}");
  hres_Mee->SetLineColor(kRed);
  hres_Mee->Draw();
  hres_Mmumu->SetTitle("Resolution;#DeltaM_{#mu^{+}#mu^{-}} [GeV/c^{2}];Events / 0.01 GeV/c^{2}");
  hres_Mmumu->SetLineColor(kRed);
  hres_Mmumu->Draw();
  tex.DrawLatex(.2,.8,Form("#sigma_{ee} = %1.2f GeV/c^{2}",ee_sig));
  tex.DrawLatex(.2,.7,Form("#sigma_{#mu#mu} = %1.2f GeV/c^{2}",mumu_sig));
  gPad->Print("e_res.png");

  new TCanvas();
  hrec_Mee->SetLineColor(kBlack);
  hrec_Mmumu->SetLineColor(kRed);
  hrec_Mmumu->SetTitle(";M_{l^{+}l^{-}}; Events / 0.25 GeV/c^2");
  hrec_Mmumu->Draw("");
  hrec_Mee->Draw("same");
  tex.DrawLatex(.3,.8,"ee");
  tex.DrawLatex(.3,.7,"#mu#mu");
  gPad->Print("Mee_mumu.png");
}
