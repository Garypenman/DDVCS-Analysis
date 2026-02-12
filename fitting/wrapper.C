#include "ToyMaker.C"
//#include "FitThetaPhi.C"

void wrapper(){
  
  TLatex tex;
  tex.SetNDC();
  tex.SetTextSize(0.04);
  
  int nev=5e5;
  double th_cut = 0.8;
  //TCut mc_theta_cut = "mc_Heli_Theta>pi/4 && mc_Heli_Theta<3*pi/4";
  TCut mc_theta_cut = Form("mc_Heli_CosTheta>-%f && mc_Heli_CosTheta<%f",th_cut,th_cut);
  int N=1;
  int nrows=(sqrt(N+2));
  int ncols = ceil( (N+2) / nrows);
  
  TH1D *hth[N];
  TH1D *hph[N];
  TH2D *h2d_cth_ph[N];
  double BH[N],ImM[N],ReM[N],TCS[N];
  for (int i=0; i<N; i++){
    BH[i] = 1.0;
    ImM[i] = 0.5;
    //ReM[i] = 0.5;
    TCS[i] = 0.0;
    
    ToyMaker(nev,BH[i],ImM[i],TCS[i]);
    TFile *f = new TFile("toys_test/Toy0.root","OPEN");
    TTree *T = (TTree*)f->Get("ToyData");
    hth[i] = new TH1D(Form("hth_%i",i),"",100,-1,1);
    hph[i] = new TH1D(Form("hph_%i",i),"",100,-M_PI,M_PI);
    h2d_cth_ph[i] = new TH2D(Form("h2d_cth_ph_%i",i),"",100,-M_PI,M_PI,100,-1,1);
    h2d_cth_ph[i]->SetTitle("Brufit Toy Amplitudes;#phi_{ee} [rad];cos(#theta_{ee})");
    //T->Draw(Form("cos(mc_Heli_Theta)>>hth_%i",i),mc_theta_cut,"goff");
    T->Draw(Form("mc_Heli_CosTheta>>hth_%i",i),mc_theta_cut,"goff");
    T->Draw(Form("mc_Heli_Phi>>hph_%i",i),mc_theta_cut,"goff");
    //T->Draw(Form("cos(mc_Heli_Theta):mc_Heli_Phi>>h2d_cth_ph_%i",i),mc_theta_cut,"goff");
    T->Draw(Form("mc_Heli_CosTheta:mc_Heli_Phi>>h2d_cth_ph_%i",i),mc_theta_cut,"goff");
  
  }
  
  auto max=hth[0]->GetMaximum();
  //auto max=550;
  auto min=0;
  TFile *fphys = new TFile("/w/work5/home/garyp/rad_trees/HepMC_ddvcs_ee_18x275_mixed.root");
  TTree *Tphys = (TTree*)fphys->Get("rad_tree");
  TFile *fBH = new TFile("/w/work5/home/garyp/eic/TCS_BH_Test/50k/rad_trees/HepMC_ddvcs_ee_18x275_bh_plus.root");
  TTree *TBH = (TTree*)fBH->Get("rad_tree");
  
  TCanvas *cth = new TCanvas();
  cth->Divide(ncols,nrows);
  for(int i=0; i<N; i++){
    cth->cd(i+1);
    hth[i]->SetMinimum(min);
    hth[i]->SetMaximum(max);
    hth[i]->Draw();
    tex.DrawLatex(0.3,0.8,Form("BH = %1.2f",BH[i]));
    tex.DrawLatex(0.3,0.75,Form("ImM = %1.2f",ImM[i]));
    //tex.DrawLatex(0.55,0.75,Form("ImM0- = %1.2f",ImM2[i]));
    //tex.DrawLatex(0.3,0.7,Form("ReM-- = %1.2f",ReM[i]));
    //tex.DrawLatex(0.55,0.7,Form("ReM0- = %1.2f",ImM2[i]));
    tex.DrawLatex(0.3,0.7,Form("TCS = %1.2f",TCS[i]));
    
  }
  cth->cd(N+1);
  TH1D *hphys_theta = new TH1D("hphys_theta","",100,-1,1);
  Tphys->Draw("mc_Heli_CosTheta>>hphys_theta",mc_theta_cut,"goff");
  auto nevphys=hphys_theta->GetEntries();
  hphys_theta->Scale(nev/nevphys);
  hphys_theta->SetMinimum(min);
  hphys_theta->SetMaximum(max);
  hphys_theta->SetLineColor(kRed);
  hphys_theta->Draw("hist");
  tex.DrawLatex(0.3,0.65,"EpIC BH+TCS+INT");
  //new TCanvas();
  cth->cd(N+2);
  TH1D *hBH_theta = new TH1D("hBH_theta","",100,-1,1);
  TBH->Draw("mc_Heli_CosTheta>>hBH_theta",mc_theta_cut,"goff");
  auto nevBH = hBH_theta->GetEntries();
  hBH_theta->Scale(nev/nevBH);
  hBH_theta->SetMinimum(min);
  hBH_theta->SetMaximum(max);
  hBH_theta->Draw("hist");
  tex.DrawLatex(0.3,0.65,"EpIC BH");
  cth->Print("cth.pdf");

  TCanvas *cph = new TCanvas();
  cph->Divide(ncols,nrows);
  for(int i=0; i<N; i++){
    cph->cd(i+1);
    hph[i]->SetMinimum(min);
    hph[i]->SetMaximum(max);
    hph[i]->Draw();
    tex.DrawLatex(0.3,0.8,Form("BH = %1.2f",BH[i]));
    tex.DrawLatex(0.3,0.75,Form("ImM = %1.2f",ImM[i]));
    //tex.DrawLatex(0.55,0.75,Form("ImM0- = %1.2f",ImM2[i]));
    //tex.DrawLatex(0.3,0.7,Form("ReM-- = %1.2f",ReM[i]));
    //tex.DrawLatex(0.55,0.7,Form("ReM0- = %1.2f",ReM2[i]));
    tex.DrawLatex(0.3,0.7,Form("TCS = %1.2f",TCS[i]));
    
  }
  cph->cd(N+1);
  TH1D *hphys_phi = new TH1D("hphys_phi","",100,-M_PI,M_PI);
  Tphys->Draw("mc_Heli_Phi>>hphys_phi",mc_theta_cut,"goff");
  nevphys=hphys_phi->GetEntries();
  hphys_phi->Scale(nev/nevphys);
  hphys_phi->SetMinimum(min);
  hphys_phi->SetMaximum(max);
  hphys_phi->SetLineColor(kRed);
  hphys_phi->Draw("hist");
  tex.DrawLatex(0.3,0.65,"EpIC BH+TCS+INT");
  
  //new TCanvas();
  cph->cd(N+2);
  TH1D *hBH_phi = new TH1D("hBH_phi","",100,-M_PI,M_PI);
  TBH->Draw("mc_Heli_Phi>>hBH_phi",mc_theta_cut,"goff");
  nevBH = hBH_phi->GetEntries();
  hBH_phi->Scale(nev/nevBH);
  hBH_phi->SetMinimum(min);
  hBH_phi->SetMaximum(max);
  hBH_phi->Draw("hist");
  tex.DrawLatex(0.3,0.65,"EpIC BH");
  cph->Print("phi.pdf");
  
  TCanvas *c2D = new TCanvas();
  c2D->Divide(ncols,nrows);
  for(int i=0; i<N; i++){
    c2D->cd(i+1);
    h2d_cth_ph[i]->Draw("colz");
  }
  
  c2D->cd(N+1);
  TH2D *h2d_phys_cth_ph = new TH2D("h2d_phys_cth_ph","EpIC BH+TCS+INT;#phi_{ee} [rad]; cos(#theta_{ee})",100,-M_PI,M_PI,100,-1,1);
  Tphys->Draw("cos(mc_Heli_Theta):mc_Heli_Phi>>h2d_phys_cth_ph",mc_theta_cut,"goff");
  h2d_phys_cth_ph->Draw("colz");
  
  c2D->cd(N+2);
  TH2D *h2d_BH_cth_ph = new TH2D("h2d_BH_cth_ph","EpIC BH Only;#phi_{ee} [rad]; cos(#theta_{ee})",100,-M_PI,M_PI,100,-1,1);
  TBH->Draw("cos(mc_Heli_Theta):mc_Heli_Phi>>h2d_BH_cth_ph",mc_theta_cut,"goff");
  //new TCanvas();
  h2d_BH_cth_ph->Draw("colz");

  c2D->Print("2D.pdf");
  
}
