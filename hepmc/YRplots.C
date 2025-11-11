#include "EPICStyle.C"

void YRplots(){
  
  //TString filename="/home/garyp/eic/rad/output/mcmatch_tcs_1M_plus.root";
  TString filename="/home/garyp/eic/rad/output/HepMC_tcs_plus.root";
  TFile *f = TFile::Open(filename);
  TTree *T = (TTree*)f->Get("rad_tree");
  
  TString ddvcs_filename="/home/garyp/eic/rad/output/HepMC_ddvcs_plus.root";
  TFile *fddvcs = TFile::Open(ddvcs_filename);
  TTree *Tddvcs = (TTree*)fddvcs->Get("rad_tree");
  
  TString kgates_filename="/home/garyp/eic/rad/output/kgates_nov21_tcs_plus_out.root";
  TFile *fkg = TFile::Open(kgates_filename);
  TTree *Tkg = (TTree*)fkg->Get("rad_tree");
  
  SetEPICStyle();
  
  //from file
  // /w/work5/home/garyp/eic/Farm/data/EpIC_TCS_ee_18x275/TCS_events_plus.hepmc 
  double tcs_xsec = 3.72; //nb
  double tcs_dxsec = 0.14; //nb
  double ddvcs_xsec = 0.45; //nb
  double kg_xsec = 0.37; //nb
  
  //cross sections
  //from derek chat
  //slight was 541.366 pb
  //elspectro was 0.13569 nb
  double jpsi_xsec = 0.541366;
  double jpsi_mass = 3.0969;
  double jpsi_width = 93 *1e-6; //93keV width;
  
  double tcs_ngen = T->GetEntries();
  double jpsi_ngen = 1e6;
  double ddvcs_ngen = Tddvcs->GetEntries();
  double kg_ngen = Tkg->GetEntries();
  
  double L=5*1e6; //5fb-1 / y
  double tcs_exp = L*tcs_xsec;
  double jpsi_exp = L*jpsi_xsec;
  double ddvcs_exp = L*ddvcs_xsec;
  double kg_exp = L*kg_xsec;
  
  double tcs_scale = tcs_exp/tcs_ngen;
  double jpsi_scale = jpsi_exp/jpsi_ngen;
  
  
  //cuts
  TCut thetaL_locut = "mc_Heli_Theta<(3.14/6)"; 
  TCut thetaL_hicut = "mc_Heli_Theta>(5*3.14/6)";
  TCut thetaL_mid = !thetaL_locut && !thetaL_hicut;
  
  TCut Q2_cut = "mc_Q2<0.1 && mc_Q2 > 1e-3";
  TCut Qp2_cut = "mc_GMass*mc_GMass>2.0 && mc_GMass<3.1";
  
  TCut tau_cut = "mc_tau<0.02";
  
  TCut y_cut = "mc_y>0.05 && mc_y<0.95";
  
  TCut YRcuts = thetaL_mid && Q2_cut && Qp2_cut && tau_cut && y_cut;
  
  
  //draws
  T->Draw("mc_pmag[ele]:mc_eta[ele]>>h2d_pmag_eta_ele(250,-8,8,250,0.1,100)","","goff");
  
  T->Draw("mc_pmag[ele]:mc_eta[ele]>>h2d_pmag_eta_ele_cuts(250,-8,8,250,0.1,100)",YRcuts,"goff");
  
  T->Draw("mc_pmag[pos]:mc_eta[pos]>>h2d_pmag_eta_pos(250,-8,8,250,0.1,100)","","goff");
  
  T->Draw("mc_pmag[pos]:mc_eta[pos]>>h2d_pmag_eta_pos_cuts(250,-8,8,250,0.1,100)",YRcuts,"goff");
  
  //eta split by theta cut
  T->Draw("mc_eta[ele]>>hmc_eta_lo(100,-10,8)",thetaL_locut,"goff");
  T->Draw("mc_eta[ele]>>hmc_eta_hi(100,-10,8)",thetaL_hicut,"goff");
  T->Draw("mc_eta[ele]>>hmc_eta_mid(100,-10,8)",thetaL_mid,"goff");
  
  //t compare with kayleigh
  T->Draw("mc_t_bot>>hmc_t_bot(100,0,1)",YRcuts,"goff");
  Tddvcs->Draw("mc_t_bot>>hmc_t_bot_ddvcs(100,0,1)",YRcuts,"goff");
  Tkg->Draw("mc_t_bot>>hmc_t_bot_kg(100,0,1)",YRcuts,"goff");
  
  
  auto h2d_pmag_eta_ele = (TH1D*)gDirectory->Get("h2d_pmag_eta_ele");
  auto h2d_pmag_eta_ele_cuts = (TH1D*)gDirectory->Get("h2d_pmag_eta_ele_cuts");
  
  auto h2d_pmag_eta_pos = (TH1D*)gDirectory->Get("h2d_pmag_eta_pos");
  auto h2d_pmag_eta_pos_cuts = (TH1D*)gDirectory->Get("h2d_pmag_eta_pos_cuts");

  new TCanvas();
  gPad->SetLogy();
  gPad->SetLogz();
  h2d_pmag_eta_ele->SetTitle(";#eta;p [GeV/c]");
  h2d_pmag_eta_ele->Draw("colz");
  
  new TCanvas();
  gPad->SetLogy();
  gPad->SetLogz();
  h2d_pmag_eta_ele_cuts->SetTitle(";#eta;p [GeV/c]");
  h2d_pmag_eta_ele_cuts->Draw("colz");

  new TCanvas();
  gPad->SetLogy();
  gPad->SetLogz();
  h2d_pmag_eta_pos->SetTitle(";#eta;p [GeV/c]");
  h2d_pmag_eta_pos->Draw("colz");
  
  new TCanvas();
  gPad->SetLogy();
  gPad->SetLogz();
  h2d_pmag_eta_pos_cuts->SetTitle(";#eta;p [GeV/c]");
  h2d_pmag_eta_pos_cuts->Draw("colz");

  auto hmc_eta_lo = (TH1D*)gDirectory->Get("hmc_eta_lo");
  auto hmc_eta_hi = (TH1D*)gDirectory->Get("hmc_eta_hi");
  auto hmc_eta_mid = (TH1D*)gDirectory->Get("hmc_eta_mid");
  new TCanvas();
  gPad->SetLeftMargin(0.15);
  gPad->SetRightMargin(0.05);
  hmc_eta_lo->SetLineColor(kRed);
  hmc_eta_hi->SetLineColor(kBlue);
  hmc_eta_mid->SetLineColor(kBlack);
  hmc_eta_lo->SetTitle("#theta < #pi/6;#eta;Events / 0.16");
  hmc_eta_hi->SetTitle("#theta > 5#pi/6;#eta;Events / 0.16");
  hmc_eta_mid->SetTitle("#pi/6 < #theta < 5#pi/6;#eta;Events / 0.16");
  hmc_eta_hi->SetMaximum(1.2*hmc_eta_hi->GetMaximum());
  hmc_eta_hi->Draw("");
  hmc_eta_lo->Draw("same");
  hmc_eta_mid->Draw("same");
  TLegend *leg00 = new TLegend(0.2,0.75,0.4,0.85);
  leg00->AddEntry(hmc_eta_lo);
  leg00->AddEntry(hmc_eta_hi);
  leg00->AddEntry(hmc_eta_mid);
  leg00->Draw();
  
  new TCanvas();
  gPad->SetLogy();
  gPad->SetRightMargin(0.05);
  gPad->SetBottomMargin(0.12);
  auto hmc_t_bot = (TH1D*) gDirectory->Get("hmc_t_bot");
  auto hmc_t_bot_ddvcs = (TH1D*) gDirectory->Get("hmc_t_bot_ddvcs");
  auto hmc_t_bot_kg = (TH1D*) gDirectory->Get("hmc_t_bot_kg");
  hmc_t_bot->SetTitle("TCS Module 2025;|t| [GeV^{2}];d#sigma/d|t| [fb GeV^{-2}]");
  hmc_t_bot->Scale(tcs_xsec/tcs_ngen,"width"); //event weight = xsec/ngen;
  hmc_t_bot->Scale(1e6); //nb to fb
  hmc_t_bot->SetMinimum(6e2);
  hmc_t_bot->SetMaximum(8.5e6);
  hmc_t_bot->SetLineColor(kBlack);
  hmc_t_bot->SetLineWidth(2);
  hmc_t_bot->Draw("h");
  
  hmc_t_bot_ddvcs->SetTitle("DDVCS Module 2025;|t| [GeV^{2}];d#sigma/d|t| [fb GeV^{-2}]");
  hmc_t_bot_ddvcs->Scale(ddvcs_xsec/ddvcs_ngen,"width"); //event weight = xsec/ngen;
  hmc_t_bot_ddvcs->Scale(1e6); //nb to fb
  // hmc_t_bot_ddvcs->SetMinimum(6e2);
  // hmc_t_bot_ddvcs->SetMaximum(8.5e6);
  hmc_t_bot_ddvcs->SetLineColor(kRed);
  hmc_t_bot_ddvcs->SetLineWidth(2);
  hmc_t_bot_ddvcs->Draw("h same");
  
  hmc_t_bot_kg->SetTitle("KGates TCS 2021;|t| [GeV^{2}];d#sigma/d|t| [fb GeV^{-2}]");
  hmc_t_bot_kg->Scale(kg_xsec/kg_ngen,"width"); //event weight = xsec/ngen;
  hmc_t_bot_kg->Scale(1e6); //nb to fb
  // hmc_t_bot_kg->SetMinimum(6e2);
  // hmc_t_bot_kg->SetMaximum(8.5e6);
  hmc_t_bot_kg->SetLineColor(kBlue);
  hmc_t_bot_kg->SetLineWidth(2);
  hmc_t_bot_kg->Draw("h same");

  TLegend *leg_tbot = new TLegend(0.6,0.6,0.8,0.9);
  leg_tbot->AddEntry(hmc_t_bot);
  leg_tbot->AddEntry(hmc_t_bot_ddvcs);
  leg_tbot->AddEntry(hmc_t_bot_kg);
  leg_tbot->Draw();
  
  cout << "hmc_t_bot_tcs->Integral(1,100) / hmc_t_bot_ddvcs->Integral(1,100) " << hmc_t_bot->Integral(1,100) / hmc_t_bot_ddvcs->Integral(1,100) << endl;
  
}
