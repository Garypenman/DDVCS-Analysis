#include "TPaveText.h"
#include "../EPICStyle.C"

TLegend* PadLegend(const TVirtualPad *pad,double x1,double y1, double x2, double y2){
  
  TLegend *leg = new TLegend(x1,y1,x2,y2);
  leg->SetTextSize(0.04);
  leg->SetTextFont(62);
  leg->SetHeader("ePIC Sim + EICRecon v25.08.0");
  leg->SetTextFont(42);
  leg->SetBorderSize(0);
  //leg->SetFillColorAlpha(kWhite,0.0);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetEntrySeparation(0.3);
  
  return leg;
}

void DrawHistos(TString filename="MCMatched_ddvcs_ee_18x275_histos.root"){
  
  TFile *f = TFile::Open(filename);
  SetEPICStyle();
  TLatex tex;
  tex.SetTextSize(0.05);
  tex.SetTextAlign(13);
  tex.SetNDC();
  
  auto h2d_tru_Q2_t_bot = (TH1D*)f->Get("h2d_tru_Q2_t_bot");
  auto h2d_rec_Q2_t_bot = (TH1D*)f->Get("h2d_rec_Q2_t_bot");
  auto h2d_rec_Q2_t_top = (TH1D*)f->Get("h2d_rec_Q2_t_top");
  auto h2d_tru_Qp2_t_bot = (TH1D*)f->Get("h2d_tru_Qp2_t_bot");
  auto h2d_rec_Qp2_t_bot = (TH1D*)f->Get("h2d_rec_Qp2_t_bot");
  auto h2d_rec_Qp2_t_top = (TH1D*)f->Get("h2d_rec_Qp2_t_top");
  TCanvas *c00 = new TCanvas();
  c00->Divide(3,2);
  c00->cd(1)->SetLogz();
  h2d_tru_Q2_t_bot->SetTitle(";Truth |t| [GeV^{2}];Truth Q^{2} [GeV^{2}]");
  h2d_tru_Q2_t_bot->Draw("colz");
  c00->cd(2)->SetLogz();
  h2d_rec_Q2_t_bot->SetTitle(";Recon (p') |t| [GeV^{2}];Recon Q^{2} [GeV^{2}]");
  h2d_rec_Q2_t_bot->Draw("colz");
  c00->cd(3)->SetLogz();
  h2d_rec_Q2_t_top->SetTitle(";Recon (e'gamma*) |t| [GeV^{2}];Recon Q^{2} [GeV^{2}]");
  h2d_rec_Q2_t_top->Draw("colz");
  c00->cd(4)->SetLogz();
  h2d_tru_Qp2_t_bot->SetTitle(";Truth |t| [GeV^{2}];Truth M_{ee}^{2} [GeV^{2}]");
  h2d_tru_Qp2_t_bot->Draw("colz");
  c00->cd(5)->SetLogz();
  h2d_rec_Qp2_t_bot->SetTitle(";Recon (p') |t| [GeV^{2}];Recon M_{ee}^{2} [GeV^{2}]");
  h2d_rec_Qp2_t_bot->Draw("colz");
  c00->cd(6)->SetLogz();
  h2d_rec_Qp2_t_bot->SetTitle(";Recon (e'gamma*) |t| [GeV^{2}];Recon M_{ee}^{2} [GeV^{2}]");
  h2d_rec_Qp2_t_bot->Draw("colz");
  c00->Print("2D_Q2_t.png");
  
  auto htru_Q2 = (TH1D*)f->Get("htru_Q2");
  auto hrec_Q2 = (TH1D*)f->Get("hrec_Q2");
  auto hrec_Q2_tagger = (TH1D*)f->Get("hrec_Q2_tagger");
  auto hrec_Q2_barrel = (TH1D*)f->Get("hrec_Q2_barrel");
  auto hrec_Q2_excl = (TH1D*)f->Get("hrec_Q2_excl");
  new TCanvas();
  gPad->SetLogy();
  htru_Q2->SetTitle(";Q^{2} [GeV^{2}]; Events / 0.05 GeV^{2}]");
  htru_Q2->SetMinimum(1);
  htru_Q2->SetLineColor(kBlack);
  hrec_Q2->SetLineColor(kRed);
  hrec_Q2_tagger->SetLineColor(9);
  hrec_Q2_tagger->SetFillColorAlpha(9,0.3);
  hrec_Q2_barrel->SetLineColor(8);
  hrec_Q2_barrel->SetFillColorAlpha(8,0.3);
  htru_Q2->Draw();
  hrec_Q2->Draw("same");
  hrec_Q2_tagger->Draw("same");
  hrec_Q2_barrel->Draw("same");
  gPad->Close();
  
  TCanvas *c001 = new TCanvas("","",2*1.414*600,600);
  c001->Divide(2,1);
  c001->cd(1);
  auto htru_eta_ele = (TH1D*)f->Get("htru_eta_ele");
  auto hrec_eta_ele = (TH1D*)f->Get("hrec_eta_ele");
  auto hrec_eta_ele_good = (TH1D*)f->Get("hrec_eta_ele_good");
  double eta_lep_max = 1.5*htru_eta_ele->GetMaximum();
  htru_eta_ele->SetMaximum(eta_lep_max);
  htru_eta_ele->SetLineColor(kBlack);
  hrec_eta_ele->SetLineColor(kRed);
  hrec_eta_ele_good->SetLineColor(kGreen);
  htru_eta_ele->SetTitle("MC Truth;#eta_{e^{-}};Events / 0.18");
  hrec_eta_ele->SetTitle("All Reconstructed;#eta_{e^{-}};Events / 0.18");
  hrec_eta_ele_good->SetTitle("Good Reconstructed;#eta_{e^{-}};Events / 0.18");
  htru_eta_ele->Draw();
  hrec_eta_ele->Draw("same");
  hrec_eta_ele_good->Draw("same");
  auto leg_eta_ele = PadLegend(gPad, 0.4,0.8,0.8-gPad->GetRightMargin(),0.95-gPad->GetTopMargin());
  leg_eta_ele->AddEntry(htru_eta_ele,"","l");
  leg_eta_ele->AddEntry(hrec_eta_ele,"","l");
  leg_eta_ele->AddEntry(hrec_eta_ele_good,"","l");
  leg_eta_ele->Draw();
  
  c001->cd(2);
  auto htru_eta_pos = (TH1D*)f->Get("htru_eta_pos");
  auto hrec_eta_pos = (TH1D*)f->Get("hrec_eta_pos");
  auto hrec_eta_pos_good = (TH1D*)f->Get("hrec_eta_pos_good");
  htru_eta_pos->SetMaximum(eta_lep_max);
  htru_eta_pos->SetLineColor(kBlack);
  hrec_eta_pos->SetLineColor(kRed);
  hrec_eta_pos_good->SetLineColor(kGreen);
  htru_eta_pos->SetTitle("MC Truth;#eta_{e^{+}};Events / 0.18");
  hrec_eta_pos->SetTitle("All Reconstructed;#eta_{e^{+}};Events / 0.18");
  hrec_eta_pos_good->SetTitle("Good Reconstructed;#eta_{e^{+}};Events / 0.18");
  htru_eta_pos->Draw();
  hrec_eta_pos->Draw("same");
  hrec_eta_pos_good->Draw("same");
  c001->Print("lep_pair_eta.png");
  
  auto h2d_tru_pmag_eta_ele = (TH1D*)f->Get("h2d_tru_pmag_eta_ele");
  auto h2d_rec_pmag_eta_ele = (TH1D*)f->Get("h2d_rec_pmag_eta_ele");
  auto h2d_tru_pmag_eta_pos = (TH1D*)f->Get("h2d_tru_pmag_eta_pos");
  auto h2d_rec_pmag_eta_pos = (TH1D*)f->Get("h2d_rec_pmag_eta_pos");
  TCanvas *c002 = new TCanvas();
  c002->Divide(2,2);
  c002->cd(1);
  //gPad->SetLogy();
  gPad->SetLogz();
  h2d_tru_pmag_eta_ele->SetTitle(";Truth #eta_{e^{-}}; Truth p_{e^{-}} [GeV/c^{2}]");
  h2d_tru_pmag_eta_ele->GetYaxis()->SetLabelOffset(999);
  h2d_tru_pmag_eta_ele->GetYaxis()->SetTickLength(0);
  h2d_tru_pmag_eta_ele->Draw("colz");
  gPad->Update();
  double x1 = gPad->GetUxmin();
  double y1 = gPad->GetUymin();
  double y2 = gPad->GetUymax();
  int    font   = gStyle->GetLabelFont("Y");
  double lsize  = gStyle->GetLabelSize("Y");
  double toffset= gStyle->GetTitleOffset("Y");
  double tsize  = gStyle->GetTitleSize("Y");
  double lwoff  = gStyle->GetLineWidth();
  cout << "LOGY AXIS x1,y1,y2" << x1 << " " << y1 << " " << y2 << endl;
  TGaxis *newaxis = new TGaxis(x1, y1, x1, y2, pow(10, y1), pow(10, y2), 510, "-RG");
  newaxis->SetLabelFont(font);
  newaxis->SetLabelSize(lsize);
  newaxis->SetTitleFont(font);
  newaxis->SetTitleSize(tsize);
  newaxis->SetTitleOffset(toffset);
  newaxis->SetLineWidth(lwoff);
  newaxis->SetTickSize(gStyle->GetTickLength("Y"));
  newaxis->SetLabelOffset(0.005);
  newaxis->SetTitle("");
  newaxis->Draw("R");
  tex.DrawLatex(.2,.8,"Truth e^{-}");
  c002->cd(2);
  //gPad->SetLogy();
  gPad->SetLogz();
  h2d_rec_pmag_eta_ele->SetTitle(";Recon #eta_{e^{-}}; Recon p_{e^{-}} [GeV/c^{2}]");
  h2d_rec_pmag_eta_ele->GetYaxis()->SetLabelOffset(999);
  h2d_rec_pmag_eta_ele->GetYaxis()->SetTickLength(0);
  h2d_rec_pmag_eta_ele->Draw("colz");
  newaxis->Draw();
  tex.DrawLatex(.2,.8,"Reconstructed e^{-}");
  c002->cd(3);
  //gPad->SetLogy();
  gPad->SetLogz();
  h2d_tru_pmag_eta_pos->SetTitle(";Truth #eta_{e^{+}}; Truth p_{e^{+}} [GeV/c^{2}]"); 
  h2d_tru_pmag_eta_pos->GetYaxis()->SetLabelOffset(999);
  h2d_tru_pmag_eta_pos->GetYaxis()->SetTickLength(0);
  h2d_tru_pmag_eta_pos->Draw("colz");
  newaxis->Draw();
  tex.DrawLatex(.2,.8,"Truth e^{+}");
  c002->cd(4);
  //gPad->SetLogy();
  gPad->SetLogz();
  h2d_rec_pmag_eta_pos->SetTitle(";Recon #eta_{e^{+}}; Recon p_{e^{+}} [GeV/c^{2}]");
  h2d_rec_pmag_eta_pos->GetYaxis()->SetLabelOffset(999);
  h2d_rec_pmag_eta_pos->GetYaxis()->SetTickLength(0);
  h2d_rec_pmag_eta_pos->Draw("colz");
  newaxis->Draw();
  tex.DrawLatex(.2,.8,"Reconstructed e^{+}");
  c002->Print("2D_leppair_pmag_eta.png");
  
  
  new TCanvas();
  auto hres_GMass = (TH1D*)f->Get("hres_GMass");
  auto gmass_sig = ((TParameter<double>*)f->Get("gmass_sig"))->GetVal();
  hres_GMass->SetTitle("Resolution;#DeltaM_{e^{+}e^{-}} [GeV/c^{2}];Events / 0.01 GeV/c^{2}");
  hres_GMass->SetLineColor(kRed);
  hres_GMass->Draw();
  tex.DrawLatex(.2,.8,Form("#sigma = %1.2f GeV/c^{2}",gmass_sig));
  gPad->Print("gmass_res.png");
  auto htru_GMass = (TH1D*)f->Get("htru_GMass");
  auto hrec_GMass = (TH1D*)f->Get("hrec_GMass");
  auto hrec_GMass_good = (TH1D*)f->Get("hrec_GMass_good");
  TCanvas *c01 = new TCanvas();
  //c01->cd()->SetLogy();
  htru_GMass->SetTitle("MC Truth;M_{e^{+}e^{-}} [GeV/c^{2}]; Events / 0.05 GeV/c^{2}");
  hrec_GMass->SetTitle("All Reconstructed;M_{e^{+}e^{-}} [GeV/c^{2}]; Events / 0.05 GeV/c^{2}");
  hrec_GMass_good->SetTitle("Resolution Cut;M_{e^{+}e^{-}} [GeV/c^{2}]; Events / 0.05 GeV/c^{2}");
  htru_GMass->SetMinimum(1);
  htru_GMass->SetLineColor(kBlack);
  hrec_GMass->SetLineColor(kRed);
  hrec_GMass_good->SetLineColor(kGreen);
  htru_GMass->Draw();
  hrec_GMass->Draw("same");
  hrec_GMass_good->Draw("same");
  auto leg_gmass = PadLegend(gPad,0.45,0.6,0.9-gPad->GetRightMargin(),0.9-gPad->GetTopMargin());
  leg_gmass->AddEntry(htru_GMass,"","l");
  leg_gmass->AddEntry(hrec_GMass,"","l");
  leg_gmass->AddEntry(hrec_GMass_good,"","l");
  leg_gmass->Draw();
  gPad->Print("gmass.png");
  
  auto htru_t_bot = (TH1D*)f->Get("htru_t_bot");
  auto hrec_t_bot = (TH1D*)f->Get("hrec_t_bot");
  auto hrec_t_bot_excl = (TH1D*)f->Get("hrec_t_bot_excl");
  auto hrec_t_top_semincl = (TH1D*)f->Get("hrec_t_top_semincl");
  
  new TCanvas();
  gPad->SetLogy();
  htru_t_bot->SetTitle("MC Truth;|t| [GeV^{2}]; Events / 0.01 GeV^{2}");
  hrec_t_bot->SetTitle("Proton Reconstructed;|t| [GeV^{2}]; Events / 0.01 GeV^{2}");
  hrec_t_bot_excl->SetTitle("Exclusive Reconstruction;|t| [GeV^{2}]; Events / 0.01 GeV^{2}");
  hrec_t_top_semincl->SetTitle("e'#gamma^{'} Reconstruction;|t| [GeV^{2}]; Events / 0.01 GeV^{2}");
  htru_t_bot->SetLineColor(kBlack);
  hrec_t_bot->SetLineColor(kRed);
  hrec_t_top_semincl->SetLineColor(kMagenta);
  hrec_t_bot_excl->SetLineColor(kBlue);
  htru_t_bot->Draw();
  hrec_t_bot->Draw("same");
  hrec_t_top_semincl->Draw("same");
  hrec_t_bot_excl->Draw("same");
  TLegend *leg_t_bot = PadLegend(gPad,0.2,0.65,0.5,0.9);
  leg_t_bot->AddEntry(htru_t_bot,"","l");
  leg_t_bot->AddEntry(hrec_t_bot,"","l");
  leg_t_bot->AddEntry(hrec_t_top_semincl,"","l");
  leg_t_bot->AddEntry(hrec_t_bot_excl,"","l");
  leg_t_bot->Draw();
  gPad->Print("t.png");
  
  auto htru_logt_bot = (TH1D*)f->Get("htru_logt_bot");
  auto hrec_logt_bot = (TH1D*)f->Get("hrec_logt_bot");
  auto hrec_logt_bot_excl = (TH1D*)f->Get("hrec_logt_bot_excl");
  new TCanvas();
  htru_logt_bot->SetTitle("MC Truth;log10(|t|) [GeV^{2}]; Events / 0.01 GeV^{2}");
  hrec_logt_bot->SetTitle("Proton Reconstructed;log10(|t|) [GeV^{2}]; Events / 0.01 GeV^{2}");
  hrec_logt_bot_excl->SetTitle("Exclusive Reconstruction;log10(|t|) [GeV^{2}]; Events / 0.01 GeV^{2}");
  htru_logt_bot->SetLineColor(kBlack);
  hrec_logt_bot->SetLineColor(kRed);
  hrec_logt_bot_excl->SetLineColor(kBlue);
  htru_logt_bot->Draw();
  hrec_logt_bot->Draw("same");
  hrec_logt_bot_excl->Draw("same");
  TLegend *leg_logt_bot = PadLegend(gPad,0.2,0.65,0.5,0.9);
  leg_logt_bot->AddEntry(htru_logt_bot,"","l");
  leg_logt_bot->AddEntry(hrec_logt_bot,"","l");
  leg_logt_bot->AddEntry(hrec_logt_bot_excl,"","l");
  leg_logt_bot->Draw();
  gPad->Print("logt.png");
  
  new TCanvas();
  auto h2d_res_t_bot = (TH2D*)f->Get("h2d_res_t_bot");
  h2d_res_t_bot->FitSlicesY();
  auto h2d_res_t_bot_0 = (TH1D*)f->Get("h2d_res_t_bot_0");
  auto h2d_res_t_bot_1 = (TH1D*)f->Get("h2d_res_t_bot_1");
  auto h2d_res_t_bot_2 = (TH1D*)f->Get("h2d_res_t_bot_2");
  h2d_res_t_bot->SetTitle("2D Resolution;|t| [GeV^{2};Truth - Reco |t| [GeV^{2}]");
  h2d_res_t_bot->Draw("colz");
  h2d_res_t_bot_1->SetMarkerStyle(2);
  h2d_res_t_bot_1->SetMarkerColor(kRed);
  h2d_res_t_bot_1->Draw("same");;
  gPad->Print("2D_t_acc.png");
  
  new TCanvas();
  auto hres_Q2 = (TH1D*)f->Get("hres_Q2");
  hres_Q2->SetLineColor(kBlack);
  hres_Q2->SetTitle("No Cuts; MC - Reco Q^{2} [GeV^{2}]; Events / 0.004 GeV^{2}");
  hres_Q2->Fit("gaus","Q","",-0.5,0.5);
  hres_Q2->Draw();
  
  new TCanvas();
  gPad->SetLogy();
  auto htru_theta_scat_ele = (TH1D*)f->Get("htru_theta_scat_ele");
  auto hrec_theta_scat_ele = (TH1D*)f->Get("hrec_theta_scat_ele");
  auto hrec_theta_scat_ele_good = (TH1D*)f->Get("hrec_theta_scat_ele_good");
  htru_theta_scat_ele->SetTitle("MC Truth;#theta_{e'} [rad];Events / 0.9 mrad");
  hrec_theta_scat_ele->SetTitle("Recon No Cuts;#theta_{e'} [rad];Events / 0.9 mrad");
  hrec_theta_scat_ele_good->SetTitle("Good e^{-} Resolution Cuts;#theta_{e'} [rad];Events / 0.9 mrad");
  htru_theta_scat_ele->SetMaximum(1e6);
  htru_theta_scat_ele->SetMinimum(1);
  htru_theta_scat_ele->SetLineColor(kBlack);
  hrec_theta_scat_ele->SetLineColor(kRed);
  hrec_theta_scat_ele_good->SetLineColor(kGreen);
  htru_theta_scat_ele->Draw();
  hrec_theta_scat_ele->Draw("same");
  hrec_theta_scat_ele_good->Draw("same");
  TLine *ltheta = new TLine(3.12,1,3.12,htru_theta_scat_ele->GetMaximum());
  ltheta->SetLineColor(kRed);
  ltheta->Draw();
  TLegend *leg_th_scatele = PadLegend(gPad, 0.05+gPad->GetLeftMargin(),0.6+gPad->GetLeftMargin(),0.4,0.9);
  leg_th_scatele->AddEntry(htru_theta_scat_ele,"","l");
  leg_th_scatele->AddEntry(hrec_theta_scat_ele,"","l");
  leg_th_scatele->AddEntry(hrec_theta_scat_ele_good,"","l");
  leg_th_scatele->Draw();
  
  auto htru_pmag_scat_ele = (TH1D*)f->Get("htru_pmag_scat_ele");
  auto hrec_pmag_scat_ele_inclus = (TH1D*)f->Get("hrec_pmag_scat_ele_inclus");
  auto hrec_pmag_scat_ele_semincl = (TH1D*)f->Get("hrec_pmag_scat_ele_semincl");
  auto hrec_pmag_scat_ele_semexcl = (TH1D*)f->Get("hrec_pmag_scat_ele_semexcl");
  auto hrec_pmag_scat_ele_excl = (TH1D*)f->Get("hrec_pmag_scat_ele_excl");
  
  TCanvas *c03 = new TCanvas();
  c03->Divide(2,2);
  c03->cd(1)->SetLogy();
  htru_pmag_scat_ele->SetLineColor(kBlack);
  htru_pmag_scat_ele->SetTitle("MC Truth;p_{e} [GeV/c]; Events / 0.18 GeV");
  hrec_pmag_scat_ele_inclus->SetTitle("Tagger e^{-};p_{e} [GeV/c]; Events / 0.18 GeV");
  hrec_pmag_scat_ele_semincl->SetTitle("Tagger e^{-} + #gamma;p_{e} [GeV/c]; Events / 0.18 GeV");
  hrec_pmag_scat_ele_semexcl->SetTitle("Tagger e^{-} + p;p_{e} [GeV/c]; Events / 0.18 GeV");
  hrec_pmag_scat_ele_excl->SetTitle("Exclusive;p_{e} [GeV/c]; Events / 0.18 GeV");
  htru_pmag_scat_ele->SetMinimum(1);
  htru_pmag_scat_ele->SetMaximum(1e5);
  htru_pmag_scat_ele->Draw();
  hrec_pmag_scat_ele_inclus->SetLineColor(kRed);
  hrec_pmag_scat_ele_inclus->Draw("same");
  hrec_pmag_scat_ele_semincl->SetLineColor(kGreen);
  hrec_pmag_scat_ele_semincl->Draw("same");
  hrec_pmag_scat_ele_semexcl->SetLineColor(kOrange);
  hrec_pmag_scat_ele_semexcl->Draw("same");
  hrec_pmag_scat_ele_excl->SetLineColor(kBlue);
  hrec_pmag_scat_ele_excl->Draw("same");
  TLegend *leg_pmag_scatele = PadLegend(gPad,0.05+gPad->GetLeftMargin(),0.55+gPad->GetBottomMargin(),0.4+gPad->GetLeftMargin(),0.95-gPad->GetTopMargin());
  leg_pmag_scatele->AddEntry(htru_pmag_scat_ele,"","l");
  leg_pmag_scatele->AddEntry(hrec_pmag_scat_ele_inclus,"","l");
  leg_pmag_scatele->AddEntry(hrec_pmag_scat_ele_semincl,"","l");
  leg_pmag_scatele->AddEntry(hrec_pmag_scat_ele_semexcl,"","l");
  leg_pmag_scatele->AddEntry(hrec_pmag_scat_ele_excl,"","l");
  leg_pmag_scatele->Draw();
  
  auto hres_pmag_scat_ele = (TH1D*)f->Get("hres_pmag_scat_ele");
  auto hres_theta_scat_ele = (TH1D*)f->Get("hres_theta_scat_ele");
  auto hres_phi_scat_ele = (TH1D*)f->Get("hres_phi_scat_ele");
  
  auto hres_pmag_scat_ele_tagger = (TH1D*)f->Get("hres_pmag_scat_ele_tagger");
  auto hres_theta_scat_ele_tagger = (TH1D*)f->Get("hres_theta_scat_ele_tagger");
  auto hres_phi_scat_ele_tagger = (TH1D*)f->Get("hres_phi_scat_ele_tagger");
  
  auto hres_pmag_scat_ele_barrel = (TH1D*)f->Get("hres_pmag_scat_ele_barrel");
  auto hres_theta_scat_ele_barrel = (TH1D*)f->Get("hres_theta_scat_ele_barrel");
  auto hres_phi_scat_ele_barrel = (TH1D*)f->Get("hres_phi_scat_ele_barrel");
  
  auto hres_pmag_scat_ele_tagger1 = (TH1D*)f->Get("hres_pmag_scat_ele_tagger1");
  auto hres_theta_scat_ele_tagger1 = (TH1D*)f->Get("hres_theta_scat_ele_tagger1");
  auto hres_phi_scat_ele_tagger1 = (TH1D*)f->Get("hres_phi_scat_ele_tagger1");
  
  auto hres_pmag_scat_ele_tagger2 = (TH1D*)f->Get("hres_pmag_scat_ele_tagger2");
  auto hres_theta_scat_ele_tagger2 = (TH1D*)f->Get("hres_theta_scat_ele_tagger2");
  auto hres_phi_scat_ele_tagger2 = (TH1D*)f->Get("hres_phi_scat_ele_tagger2");
  
  auto tagger_pmag_sig = ((TParameter<double>*)f->Get("tagger_pmag_sig"))->GetVal();
  auto tagger_theta_sig = ((TParameter<double>*)f->Get("tagger_pmag_sig"))->GetVal();
  auto tagger_phi_sig = ((TParameter<double>*)f->Get("tagger_pmag_sig"))->GetVal();
  
  c03->cd(2);
  // hres_pmag_scat_ele->SetMinimum(0);
  // hres_pmag_scat_ele->SetLineColor(kBlack);
  // hres_pmag_scat_ele_tagger1->SetLineColor(kRed);
  // hres_pmag_scat_ele_tagger2->SetLineColor(kBlue);
  // hres_pmag_scat_ele_barrel->SetLineColor(kGreen);
  // hres_pmag_scat_ele->SetTitle("All;#Deltap_{e} / p_{e} [GeV/c]; Events / 0.2 GeV/c");
  // hres_pmag_scat_ele_tagger1->SetTitle("Tagger1;#Deltap_{e} / p_{e} [GeV/c]; Events / 0.2 GeV/c");
  // hres_pmag_scat_ele_tagger2->SetTitle("Tagger2;#Deltap_{e} / p_{e} [GeV/c]; Events / 0.2 GeV/c");
  // hres_pmag_scat_ele_barrel->SetTitle("Barrel;#Deltap_{e} / p_{e} [GeV/c]; Events / 0.2 GeV/c");
  // hres_pmag_scat_ele->Draw();
  // hres_pmag_scat_ele_tagger1->Draw("same");
  // hres_pmag_scat_ele_tagger2->Draw("same");
  // hres_pmag_scat_ele_barrel->Draw("same");
  // TLegend *leg_res_pmag_scatele = PadLegend(gPad,0.05+gPad->GetLeftMargin(),0.6+gPad->GetLeftMargin(),0.4,0.9);
  // leg_res_pmag_scatele->AddEntry(hres_pmag_scat_ele);
  // leg_res_pmag_scatele->AddEntry(hres_pmag_scat_ele_tagger1);
  // leg_res_pmag_scatele->AddEntry(hres_pmag_scat_ele_tagger2);
  // leg_res_pmag_scatele->AddEntry(hres_pmag_scat_ele_barrel);
  // leg_res_pmag_scatele->Draw();
  hres_pmag_scat_ele_tagger->SetMinimum(0);
  hres_pmag_scat_ele_tagger->SetLineColor(kRed);
  hres_pmag_scat_ele_tagger->SetTitle("Tagger;#Deltap_{e} / p_{e} [GeV/c]; Events / 0.2 GeV/c");
  hres_pmag_scat_ele_tagger->Draw();
  tex.DrawLatex(.2,.8,Form("#sigma_{#Deltap/p} = %1.2f %%",100*tagger_pmag_sig));
  
  c03->cd(3);
  // hres_theta_scat_ele->SetMinimum(0);
  // hres_theta_scat_ele->SetLineColor(kBlack);
  // hres_theta_scat_ele_tagger1->SetLineColor(kRed);
  // hres_theta_scat_ele_tagger2->SetLineColor(kBlue);
  // hres_theta_scat_ele_barrel->SetLineColor(kGreen);
  // hres_theta_scat_ele->SetTitle("All;#Delta#theta_{e} [rad]; Events / 0.12 mrad");
  // hres_theta_scat_ele_tagger1->SetTitle("Tagger1;#Delta#theta_{e} [rad]; Events / 0.12 mrad");
  // hres_theta_scat_ele_tagger2->SetTitle("Tagger2;#Delta#theta_{e} [rad]; Events / 0.12 mrad");
  // hres_theta_scat_ele_barrel->SetTitle("Barrel;#Delta#theta_{e} [rad]; Events / 0.12 mrad");
  // hres_theta_scat_ele->Draw();
  // hres_theta_scat_ele_tagger1->Draw("same");
  // hres_theta_scat_ele_tagger2->Draw("same");
  // hres_theta_scat_ele_barrel->Draw("same");
  // TLegend* leg_res_th_scatele = PadLegend(gPad,0.05+gPad->GetLeftMargin(),0.6+gPad->GetLeftMargin(),0.4,0.9);
  // leg_res_th_scatele->AddEntry(hres_theta_scat_ele);
  // leg_res_th_scatele->AddEntry(hres_theta_scat_ele_tagger1);
  // leg_res_th_scatele->AddEntry(hres_theta_scat_ele_tagger2);
  // leg_res_th_scatele->AddEntry(hres_theta_scat_ele_barrel);
  // leg_res_th_scatele->Draw();
  hres_theta_scat_ele_tagger->SetMinimum(0);
  hres_theta_scat_ele_tagger->SetLineColor(kRed);
  hres_theta_scat_ele_tagger->SetTitle("Tagger;#Delta#theta_{e} [rad]; Events / 0.12 mrad");
  hres_theta_scat_ele_tagger->Draw();
  tex.DrawLatex(.2,.8,Form("#sigma_{#Delta#theta} = %1.2f mrad",1000*tagger_theta_sig));
  
  c03->cd(4);
  // hres_phi_scat_ele->SetMinimum(0);
  // hres_phi_scat_ele->SetLineColor(kBlack);
  // hres_phi_scat_ele_tagger1->SetLineColor(kRed);
  // hres_phi_scat_ele_tagger2->SetLineColor(kBlue);
  // hres_phi_scat_ele_barrel->SetLineColor(kGreen);
  // hres_phi_scat_ele->SetTitle("All;#Delta#phi_{e} [rad]; Events / 0.0628 rad");
  // hres_phi_scat_ele_tagger1->SetTitle("Tagger1;#Delta#phi_{e} [rad]; Events / 0.0628 rad");
  // hres_phi_scat_ele_tagger2->SetTitle("Tagger2;#Delta#phi_{e} [rad]; Events / 0.0628 rad");
  // hres_phi_scat_ele_barrel->SetTitle("Barrel;#Delta#phi_{e} [rad]; Events / 0.0628 rad");
  // hres_phi_scat_ele->Draw();
  // hres_phi_scat_ele_tagger1->Draw("same");
  // hres_phi_scat_ele_tagger2->Draw("same");
  // hres_phi_scat_ele_barrel->Draw("same");
  // TLegend* leg_res_phi_scatele = PadLegend(gPad,0.05+gPad->GetLeftMargin(),0.6+gPad->GetLeftMargin(),0.4,0.9);
  // leg_res_phi_scatele->AddEntry(hres_phi_scat_ele);
  // leg_res_phi_scatele->AddEntry(hres_phi_scat_ele_tagger1);
  // leg_res_phi_scatele->AddEntry(hres_phi_scat_ele_tagger2);
  // leg_res_phi_scatele->AddEntry(hres_phi_scat_ele_barrel);
  // leg_res_phi_scatele->Draw();
  hres_phi_scat_ele_tagger->SetMinimum(0);
  hres_phi_scat_ele_tagger->SetLineColor(kRed);
  hres_phi_scat_ele_tagger->SetTitle("Tagger;#Delta#phi_{e} [rad]; Events / 0.0628 mrad");
  hres_phi_scat_ele_tagger->Draw();
  tex.DrawLatex(.2,.8,Form("#sigma_{#Delta#phi} = %1.2f rad",tagger_phi_sig));
  
  c03->Print("escat_recon_res.png");
  
  auto h2d_tru_pmag_Q2 = (TH1D*)f->Get("h2d_tru_pmag_Q2");
  auto h2d_tru_pmag_Q2_recon = (TH1D*)f->Get("h2d_tru_pmag_Q2_recon");
  auto h2d_tru_pmag_Q2_good = (TH1D*)f->Get("h2d_tru_pmag_Q2_good");
  
  auto h2d_rec_pmag_Q2 = (TH1D*)f->Get("h2d_rec_pmag_Q2");
  auto h2d_rec_pmag_Q2_good = (TH1D*)f->Get("h2d_rec_pmag_Q2_good");
  TCanvas *c04 = new TCanvas();
  c04->Divide(3,2);
  
  c04->cd(1);
  gPad->SetTopMargin(0.1);
  h2d_tru_pmag_Q2->SetTitle("MC Truth; p_{e} [GeV]; log_{10}(Q^{2}) [GeV^{2}]");
  h2d_tru_pmag_Q2->Draw("colz");
  TPaveText *pt041 = new TPaveText(0.15,0.93,0.85,0.99,"NDC");
  pt041->AddText("MC Truth");
  pt041->SetFillColor(0);      // transparent background
  pt041->SetTextFont(42);      // nice readable font
  pt041->SetTextSize(0.04);    // adjust size
  pt041->Draw();
  
  c04->cd(2);
  gPad->SetTopMargin(0.1);
  h2d_tru_pmag_Q2_recon->SetTitle("MC Truth (All Reco) Scat Elec; p_{e} [GeV]; log_{10}(Q^{2}) [GeV^{2}]");
  h2d_tru_pmag_Q2_recon->Draw("colz");
  TPaveText *pt042 = new TPaveText(0.15,0.93,0.85,0.99,"NDC");
  pt042->AddText("MC Truth (All Reco) Scat Elec");
  pt042->SetFillColor(0);      // transparent background
  pt042->SetTextFont(42);      // nice readable font
  pt042->SetTextSize(0.04);    // adjust size
  pt042->Draw();
  
  c04->cd(3);
  gPad->SetTopMargin(0.1);
  h2d_rec_pmag_Q2_good->SetTitle("Reco (Good Resolution) Scat Elec; p_{e} [GeV]; log_{10}(Q^{2}) [GeV^{2}]");
  h2d_rec_pmag_Q2_good->Draw("colz");
  h2d_tru_pmag_Q2_good->SetTitle("MC truth (Reco Good Resolution) Scat Elec; p_{e} [GeV]; log_{10}(Q^{2}) [GeV^{2}]");
  h2d_tru_pmag_Q2_good->Draw("colz");
  TPaveText *pt043 = new TPaveText(0.15,0.93,0.85,0.99,"NDC");
  pt043->AddText("MC Truth (Reco Good Res) Scat Elec");
  pt043->SetFillColor(0);      // transparent background
  pt043->SetTextFont(42);      // nice readable font
  pt043->SetTextSize(0.04);    // adjust size
  pt043->Draw();
  
  
  c04->cd(4);
  // auto h2d_acc_pmag_Q2 = (TH1D*)h2d_rec_pmag_Q2_good->Clone("h2d_acc_pmag_Q2");
  // h2d_acc_pmag_Q2->Divide(h2d_tru_pmag_Q2);
  // h2d_acc_pmag_Q2->Draw("colz");
  
  c04->cd(5);
  gPad->SetTopMargin(0.1);
  auto h2d_acc_pmag_Q2_recon = (TH1D*)h2d_tru_pmag_Q2_recon->Clone("h2d_acc_pmag_Q2_recon");
  h2d_acc_pmag_Q2_recon->SetTitle("Acceptance (All) Scat Elec; p_{e} [GeV]; log_{10}(Q^{2}) [GeV^{2}]");
  h2d_acc_pmag_Q2_recon->Divide(h2d_tru_pmag_Q2);
  h2d_acc_pmag_Q2_recon->Draw("colz");
  TPaveText *pt045 = new TPaveText(0.15,0.93,0.85,0.99,"NDC");
  pt045->AddText("Acceptance (All) Scat Elec");
  pt045->SetFillColor(0);      // transparent background
  pt045->SetTextFont(42);      // nice readable font
  pt045->SetTextSize(0.04);    // adjust size
  pt045->Draw();
  
  c04->cd(6);
  gPad->SetTopMargin(0.1);
  auto h2d_acc_pmag_Q2_good = (TH1D*)h2d_tru_pmag_Q2_good->Clone("h2d_acc_pmag_Q2_good");
  h2d_acc_pmag_Q2_good->SetTitle("Acceptance (Good Resolution) Scat Elec; p_{e} [GeV]; log_{10}(Q^{2}) [GeV^{2}]");
  h2d_acc_pmag_Q2_good->Divide(h2d_tru_pmag_Q2);
  h2d_acc_pmag_Q2_good->Draw("colz");
  TPaveText *pt046 = new TPaveText(0.15,0.93,0.85,0.99,"NDC");
  pt046->AddText("Acceptance (Good Resolution) Scat Elec");
  pt046->SetFillColor(0);      // transparent background
  pt046->SetTextFont(42);      // nice readable font
  pt046->SetTextSize(0.04);    // adjust size
  pt046->Draw();
  
  new TCanvas();
  h2d_acc_pmag_Q2_good->Draw("colz");
  gPad->Print("Tagger_2D_Acceptance_Good.png");
  
  auto h2d_tru_xbj_Q2 = (TH1D*)f->Get("h2d_tru_xbj_Q2");
  auto h2d_tru_xbj_Q2_recon = (TH1D*)f->Get("h2d_tru_xbj_Q2_recon");
  auto h2d_tru_xbj_Q2_good = (TH1D*)f->Get("h2d_tru_xbj_Q2_good");
  
  auto h2d_rec_xbj_Q2 = (TH1D*)f->Get("h2d_rec_xbj_Q2");
  auto h2d_rec_xbj_Q2_good = (TH1D*)f->Get("h2d_rec_xbj_Q2_good");
  TCanvas *c05 = new TCanvas();
  c05->Divide(3,2);
  
  c05->cd(1);
  gPad->SetTopMargin(0.1);
  h2d_tru_xbj_Q2->SetTitle("MC Truth; p_{e} [GeV]; log_{10}(Q^{2}) [GeV^{2}]");
  h2d_tru_xbj_Q2->Draw("colz");
  TPaveText *pt051 = new TPaveText(0.15,0.93,0.85,0.99,"NDC");
  pt051->AddText("MC Truth");
  pt051->SetFillColor(0);      // transparent background
  pt051->SetTextFont(42);      // nice readable font
  pt051->SetTextSize(0.04);    // adjust size
  pt051->Draw();
  
  c05->cd(2);
  gPad->SetTopMargin(0.1);
  h2d_tru_xbj_Q2_recon->SetTitle("MC Truth (All Reco) Scat Elec; p_{e} [GeV]; log_{10}(Q^{2}) [GeV^{2}]");
  h2d_tru_xbj_Q2_recon->Draw("colz");
  TPaveText *pt052 = new TPaveText(0.15,0.93,0.85,0.99,"NDC");
  pt052->AddText("MC Truth (All Reco) Scat Elec");
  pt052->SetFillColor(0);      // transparent background
  pt052->SetTextFont(42);      // nice readable font
  pt052->SetTextSize(0.04);    // adjust size
  pt052->Draw();
  
  c05->cd(3);
  gPad->SetTopMargin(0.1);
  h2d_rec_xbj_Q2_good->SetTitle("Reco (Good Resolution) Scat Elec; p_{e} [GeV]; log_{10}(Q^{2}) [GeV^{2}]");
  h2d_rec_xbj_Q2_good->Draw("colz");
  h2d_tru_xbj_Q2_good->SetTitle("MC truth (Reco Good Resolution) Scat Elec; p_{e} [GeV]; log_{10}(Q^{2}) [GeV^{2}]");
  h2d_tru_xbj_Q2_good->Draw("colz");
  TPaveText *pt053 = new TPaveText(0.15,0.93,0.85,0.99,"NDC");
  pt053->AddText("MC Truth (Reco Good Res) Scat Elec");
  pt053->SetFillColor(0);      // transparent background
  pt053->SetTextFont(42);      // nice readable font
  pt053->SetTextSize(0.04);    // adjust size
  pt053->Draw();
  
  
  c05->cd(4);
  // auto h2d_acc_xbj_Q2 = (TH1D*)h2d_rec_xbj_Q2_good->Clone("h2d_acc_xbj_Q2");
  // h2d_acc_xbj_Q2->Divide(h2d_tru_xbj_Q2);
  // h2d_acc_xbj_Q2->Draw("colz");
  
  c05->cd(5);
  gPad->SetTopMargin(0.1);
  auto h2d_acc_xbj_Q2_recon = (TH1D*)h2d_tru_xbj_Q2_recon->Clone("h2d_acc_xbj_Q2_recon");
  h2d_acc_xbj_Q2_recon->SetTitle("Acceptance (All) Scat Elec; p_{e} [GeV]; log_{10}(Q^{2}) [GeV^{2}]");
  h2d_acc_xbj_Q2_recon->Divide(h2d_tru_xbj_Q2);
  h2d_acc_xbj_Q2_recon->Draw("colz");
  TPaveText *pt055 = new TPaveText(0.15,0.93,0.85,0.99,"NDC");
  pt055->AddText("Acceptance (All) Scat Elec");
  pt055->SetFillColor(0);      // transparent background
  pt055->SetTextFont(42);      // nice readable font
  pt055->SetTextSize(0.04);    // adjust size
  pt055->Draw();
  
  c05->cd(6);
  gPad->SetTopMargin(0.1);
  auto h2d_acc_xbj_Q2_good = (TH1D*)h2d_tru_xbj_Q2_good->Clone("h2d_acc_xbj_Q2_good");
  h2d_acc_xbj_Q2_good->SetTitle("Acceptance (Good Resolution) Scat Elec; p_{e} [GeV]; log_{10}(Q^{2}) [GeV^{2}]");
  h2d_acc_xbj_Q2_good->Divide(h2d_tru_xbj_Q2);
  h2d_acc_xbj_Q2_good->Draw("colz");
  TPaveText *pt056 = new TPaveText(0.15,0.93,0.85,0.99,"NDC");
  pt056->AddText("Acceptance (Good Resolution) Scat Elec");
  pt056->SetFillColor(0);      // transparent background
  pt056->SetTextFont(42);      // nice readable font
  pt056->SetTextSize(0.04);    // adjust size
  pt056->Draw();
  
  cout << "Electron (Low-Q2 Tagger) reconstruction: " << 100. * hrec_Q2_tagger->Integral(1,100) / htru_Q2->Integral(1,100) << "%" << endl;
  cout << "Electron (Barrel) reconstruction: " << 100. * hrec_Q2_barrel->Integral(1,100) / htru_Q2->Integral(1,100) << "%" << endl;
  cout << "Meson (Barrel) reconstruction: " << 100. * hrec_GMass_good->Integral(1,100) / htru_GMass->Integral(1,100) << "%" << endl;
  cout << "Proton (RP) reconstruction: " << 100. * hrec_t_bot->Integral(1,100) / htru_t_bot->Integral(1,100) << "%" << endl;
  cout << "Exclusive Reconstruction : " << (double) 100. * hrec_t_bot_excl->Integral(1,100) / htru_t_bot->Integral(1,100) << "%" << endl;
//cout << "Exclusive Reconstruction : " << (double) 100. * T->GetEntries(exclusive) / T->GetEntries() << "%" << endl;

}
