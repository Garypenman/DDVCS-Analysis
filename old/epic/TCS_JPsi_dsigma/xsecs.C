#include "TPaveText.h"
#include "EPICStyle.C"

TH1D* sighisto(TH1D *htru, TH1D *hrec, double weight){

  auto hacc = (TH1D*)hrec->Clone("hacc");
  hacc->Divide(htru);
  int nbins = htru->GetNbinsX();
  int xlow = (htru->GetBinCenter(0)+htru->GetBinCenter(1))/2;
  int xhi =  (htru->GetBinCenter(nbins)+htru->GetBinCenter(nbins+1))/2;
  TH1D* hsig = new TH1D("","",nbins,xlow,xhi);
  for(int i=1;i<nbins;i++){
    // if (hacc->GetBinContent(i) == 0){
    //   hsig->SetBinContent(i,0);
    // }else{
    //double value = hrec->GetBinContent(i) / hacc->GetBinContent(i);
    double value = hrec->GetBinContent(i);
    hsig->SetBinContent(i,value);
  }
  //}
  hsig->Scale(weight,"width");
  hsig->SetMarkerStyle(2);
  hsig->SetMarkerColor(kBlack);
  return hsig;
}

void xsecs(TString tcsfile="/w/work5/home/garyp/rad_trees/HepMC_tcs_plus.root", TString jpsifile="/w/work5/home/garyp/rad_trees/HepMC_eslight_jpsi_ee_out.root"){
  
  tcsfile="/w/work5/home/garyp/rad_trees/MCMatched_tcs_ee_18x275_plus.root";
  jpsifile="/w/work5/home/garyp/rad_trees/MCMatched_eslight_jpsi_ee_.root";
 
  TFile *f1 = TFile::Open(tcsfile);
  TTree *T1 = (TTree*)f1->Get("rad_tree");
  TFile *f2 = TFile::Open(jpsifile);
  TTree *T2 = (TTree*)f2->Get("rad_tree");
  
  SetEPICStyle();
  
  //from file
  // /w/work5/home/garyp/eic/Farm/data/EpIC_TCS_ee_18x275/TCS_events_plus.hepmc 
  double tcs_xsec = 3.72; //nb
  double tcs_dxsec = 0.14; //nb
  
  //from derek chat
  //slight was 541.366 pb
  //elspectro was 0.13569 nb
  double jpsi_xsec = 0.541366;
  double jpsi_mass = 3.0969;
  double jpsi_width = 93 *1e-6; //93keV width;
  
  double tcs_ngen = 1e6;
  double jpsi_ngen = 1e6;
  
  double L=5*1e6; //5fb-1 / y
  double tcs_exp = L*tcs_xsec;
  double jpsi_exp = L*jpsi_xsec;
  
  double tcs_scale = tcs_exp/tcs_ngen;
  double jpsi_scale = jpsi_exp/jpsi_ngen;
  
  TCut gmass_cut = "fabs(tru_GMass-rec_GMass) < 5 * 0.02";
  
  T1->Draw("tru_GMass>>htcs_tru_Mee(100,0,5)",gmass_cut,"goff");
  T1->Draw("rec_GMass>>htcs_rec_Mee(100,0,5)",gmass_cut,"goff");
  
  T2->Draw("tru_GMass>>hjpsi_tru_Mee(100,0,5)","","goff");
  T2->Draw("rec_GMass>>hjpsi_rec_Mee(100,0,5)","","goff");
  
  
  auto htcs_tru_Mee = (TH1D*)gDirectory->Get("htcs_tru_Mee");
  auto htcs_rec_Mee = (TH1D*)gDirectory->Get("htcs_rec_Mee");
  auto hjpsi_tru_Mee = (TH1D*)gDirectory->Get("hjpsi_tru_Mee");
  auto hjpsi_rec_Mee = (TH1D*)gDirectory->Get("hjpsi_rec_Mee");
  
  auto htcs_dsig_Mee = (TH1D*)sighisto(htcs_tru_Mee, htcs_rec_Mee, tcs_xsec/tcs_ngen);
  auto htcs_dN_Mee = (TH1D*)sighisto(htcs_tru_Mee, htcs_rec_Mee, tcs_exp/tcs_ngen);
  
  auto hjpsi_dsig_Mee = (TH1D*)sighisto(hjpsi_tru_Mee, hjpsi_rec_Mee, jpsi_xsec/jpsi_ngen);
  auto hjpsi_dN_Mee = (TH1D*)sighisto(hjpsi_tru_Mee, hjpsi_rec_Mee, jpsi_exp/jpsi_ngen);
  
  auto hdsig_tot = (TH1D*)htcs_dsig_Mee->Clone("hdsig_tot");
  hdsig_tot->Add(hjpsi_dsig_Mee);
  hdsig_tot->SetTitle("Combined Spectrum;M_{ee} [GeV];d#sigma/dM_{ee} [nb GeV^{-1}]");
  
  auto hdN_tot = (TH1D*)htcs_dN_Mee->Clone("hdN_tot");
  hdN_tot->Add(hjpsi_dN_Mee);
  hdN_tot->SetTitle("Combined Spectrum;M_{ee} [GeV];dN/dM_{ee} [GeV^{-1}]");
  
  new TCanvas();
  htcs_tru_Mee->SetLineColor(kBlack);
  htcs_rec_Mee->SetLineColor(kRed);
  htcs_tru_Mee->Draw("");
  htcs_rec_Mee->Draw("same");
  
  new TCanvas();
  hjpsi_tru_Mee->SetLineColor(kBlack);
  hjpsi_rec_Mee->SetLineColor(kRed);
  hjpsi_tru_Mee->Draw("");
  hjpsi_rec_Mee->Draw("same");
  
  new TCanvas();
  hdsig_tot->Draw("");
  // htcs_dsig_Mee->SetLineColor(kRed);
  // htcs_dsig_Mee->Draw("hist same");
  // hjpsi_dsig_Mee->SetLineColor(kBlue);
  // hjpsi_dsig_Mee->Draw("hist same");
  gPad->Print("dsig_tot.png");
  
  new TCanvas();
  hdN_tot->Draw("");
  // htcs_dN_Mee->SetLineColor(kRed);
  // htcs_dN_Mee->Draw("hist same");
  // hjpsi_dN_Mee->SetLineColor(kBlue);
  // hjpsi_dN_Mee->Draw("hist same");
  gPad->Print("dN_tot.png");
  
}
