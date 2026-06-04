#pragma once

#include "../epic_plot_macro/ePIC_style.C"

#include "Config.h"
#include "ParticleConfig.h"
#include "FiducialCuts.h"
#include "DerivedObservable.h"

#include "DrawHelperFuncs.h"



void MakeParticlePlots(const ParticleConfig& pc,
		       const std::string& tru_file,
		       const std::string& rec_file,
		       TFile& outfile)
{

  // -- DataFrames --
  ROOT::RDataFrame df_tru("tree", tru_file);
  //auto df_tru_fidu = df_tru.Filter(pc.TruthFiducialCut());

  ROOT::RDataFrame df_rec_base("tree", rec_file);
  auto df_rec_filt = df_rec_base.Filter("isTruth==1");
  
  auto df_rec = df_rec_filt
    .Define("res_"+pc.name+"_pmag",[](double tru, double rec){
      return (tru - rec)/tru;
    },{pc.tru_p, pc.rec_p})
    .Define("res_"+pc.name+"_theta",[](double tru, double rec){
      return 1000*(tru - rec);
    },{pc.tru_theta, pc.rec_theta})
    .Define("res_"+pc.name+"_phi",[](double tru, double rec){
      return tru - rec;
    },{pc.tru_phi, pc.rec_phi})
    .Define("res_"+pc.name+"_eta",[](double tru, double rec){
      return tru - rec;
    },{pc.tru_eta, pc.rec_eta})
    .Define("tru_"+pc.name+"_theta_mrad",[](double theta){
      return 1000*theta;
    },{pc.tru_theta});
  
  //auto df_fidu = df_rec.Filter(

  //y-axis per bin units
  std::string pbin_acc = FormatValue((pc.p_max - pc.p_min) / pc.nbins_acc);
  std::string thbin_acc = FormatValue((pc.t_max - pc.t_min) / pc.nbins_acc);
  std::string phibin_acc = FormatValue((pc.phi_max - pc.phi_min) / pc.nbins_acc);
  std::string etabin_acc = FormatValue((pc.eta_max - pc.eta_min) / pc.nbins_acc);

  std::string pbin_res = FormatValue(100*(pc.p_max_res - pc.p_min_res) / pc.nbins_res);
  std::string thbin_res = FormatValue((pc.t_max_res - pc.t_min_res) / pc.nbins_res);
  std::string phibin_res = FormatValue((pc.phi_max_res - pc.phi_min_res) / pc.nbins_res);
  std::string etabin_res = FormatValue((pc.eta_max_res - pc.eta_min_res) / pc.nbins_res);
  
  // ----------------------------------------------------------
  // Histograms: Truth
  // ----------------------------------------------------------
  auto h_tru_p = df_tru.Histo1D({
      ("h_tru_p_" + pc.name).c_str(),
      (";p_{"+pc.label+"} [GeV/c];Events / "+pbin_acc+" GeV/c").c_str(),
      pc.nbins_acc, pc.p_min, pc.p_max
    },pc.tru_p);
  
  auto h_tru_theta = df_tru.Histo1D({
      ("h_tru_theta_" + pc.name).c_str(),
      (";#theta_{"+pc.label+"} [rad];Events / "+thbin_acc+" mrad").c_str(),
      pc.nbins_acc, pc.t_min, pc.t_max
    },pc.tru_theta);

  auto h_tru_phi = df_tru.Histo1D({
      ("h_tru_phi_" + pc.name).c_str(),
      (";#phi_{"+pc.label+"} [rad];Events / "+phibin_acc+" mrad").c_str(),
      pc.nbins_acc, pc.phi_min, pc.phi_max
    },pc.tru_phi);

  auto h_tru_eta = df_tru.Histo1D({
      ("h_tru_eta_" + pc.name).c_str(),
      (";#eta_{"+pc.label+"};Events / "+etabin_acc).c_str(),
      pc.nbins_acc, pc.eta_min, pc.eta_max
    },pc.tru_eta);

  
  // ----------------------------------------------------------
  // Histograms: Reconstructed
  // ----------------------------------------------------------
  auto h_rec_p = df_rec.Histo1D({
      ("h_rec_p_" + pc.name).c_str(),
      (";p_{"+pc.label+"} [GeV/c];Events / "+pbin_acc+" GeV/c").c_str(),
      pc.nbins_acc, pc.p_min, pc.p_max
    },pc.rec_p);
  
  auto h_rec_theta = df_rec.Histo1D({
      ("h_rec_theta_" + pc.name).c_str(),
      (";#theta_{"+pc.label+"} [rad];Events / "+thbin_acc+" mrad").c_str(),
      pc.nbins_acc, pc.t_min, pc.t_max
    },pc.rec_theta);

  auto h_rec_phi = df_rec.Histo1D({
      ("h_rec_phi_" + pc.name).c_str(),
      (";#phi_{"+pc.label+"} [rad];Events / "+phibin_acc+" rad").c_str(),
      pc.nbins_acc, pc.phi_min, pc.phi_max
    },pc.rec_phi);

  auto h_rec_eta = df_rec.Histo1D({
      ("h_rec_eta_" + pc.name).c_str(),
      (";#eta_{"+pc.label+"};Events / "+etabin_acc).c_str(),
      pc.nbins_acc, pc.eta_min, pc.eta_max
    },pc.rec_eta);

  // ----------------------------------------------------------
  // Histograms: Resolutions
  // ----------------------------------------------------------
  auto h_res_p = df_rec.Histo1D({
      ("h_res_p_" + pc.name).c_str(),
      (";#Deltap_{"+pc.label+"}/p_{"+pc.label+"} ;Events / "+pbin_res+" %").c_str(),
      pc.nbins_res, pc.p_min_res, pc.p_max_res
    },pc.res_p);
  
  auto h_res_theta = df_rec.Histo1D({
      ("h_res_theta_" + pc.name).c_str(),
      (";#Delta#theta_{"+pc.label+"} [mrad];Events / "+thbin_res+" mrad").c_str(),
      pc.nbins_res, pc.t_min_res, pc.t_max_res
    },pc.res_theta);
  
  auto h_res_phi = df_rec.Histo1D({
      ("h_res_phi_" + pc.name).c_str(),
      (";#Delta#phi_{"+pc.label+"} [rad];Events / "+phibin_res+" rad").c_str(),
      pc.nbins_res, pc.phi_min_res, pc.phi_max_res
    },pc.res_phi);
  
  auto h_res_eta = df_rec.Histo1D({
      ("h_res_eta_" + pc.name).c_str(),
      (";#Delta#eta_{"+pc.label+"};Events / "+etabin_res).c_str(),
      pc.nbins_res, pc.eta_min_res, pc.eta_max_res
    },pc.res_eta);
  
  // -- End of Booking --

  
  // ----------------------------------------------------------
  // Acceptance histograms
  // ----------------------------------------------------------
  TH1D* h_acc_p = (TH1D*)h_rec_p->Clone(("h_acc_p_" + pc.name).c_str());
  h_acc_p->Divide(h_tru_p.GetPtr());
  h_acc_p->SetTitle(("Acceptance;p_{"+pc.label+"} [GeV/c]; Acceptance").c_str());
  
  TH1D* h_acc_theta = (TH1D*)h_rec_theta->Clone(("h_acc_theta_" + pc.name).c_str());
  h_acc_theta->Divide(h_tru_theta.GetPtr());
  h_acc_theta->SetTitle(("Acceptance;#theta_{"+pc.label+"} [rad]; Acceptance").c_str());
  
  TH1D* h_acc_phi = (TH1D*)h_rec_phi->Clone(("h_acc_phi_" + pc.name).c_str());
  h_acc_phi->Divide(h_tru_phi.GetPtr());
  h_acc_phi->SetTitle(("Acceptance;#phi_{"+pc.label+"} [rad]; Acceptance").c_str());
  
  TH1D* h_acc_eta = (TH1D*)h_rec_eta->Clone(("h_acc_eta_" + pc.name).c_str());
  h_acc_eta->Divide(h_tru_eta.GetPtr());
  h_acc_eta->SetTitle(("Acceptance;#eta_{"+pc.label+"}; Acceptance").c_str());
 

  h_tru_p->Write();
  h_rec_p->Write();
  h_res_p->Write();
  h_acc_p->Write();
  
  h_tru_theta->Write();
  h_rec_theta->Write();
  h_res_theta->Write();
  h_acc_theta->Write();

  h_tru_phi->Write();
  h_rec_phi->Write();
  h_res_phi->Write();
  h_acc_phi->Write();

  h_tru_eta->Write();
  h_rec_eta->Write();
  h_res_eta->Write();
  h_acc_eta->Write();

}

void DrawParticlePlots(const std::string& infile,
		       const ParticleConfig& pc)
{
  std::string particle = pc.name;
  TFile* f = TFile::Open(infile.c_str());
  if (!f || f->IsZombie()) {
    Error("DrawParticlePlots", "Cannot open file %s", infile.c_str());
    return;
  }

  // Truth histograms
  TH1D* htru_p     = (TH1D*)f->Get(("h_tru_p_"     + particle).c_str());
  TH1D* htru_theta = (TH1D*)f->Get(("h_tru_theta_" + particle).c_str());
  TH1D* htru_eta   = (TH1D*)f->Get(("h_tru_eta_"   + particle).c_str());
  TH1D* htru_phi   = (TH1D*)f->Get(("h_tru_phi_"   + particle).c_str());

  // Reco histograms
  TH1D* hrec_p     = (TH1D*)f->Get(("h_rec_p_"     + particle).c_str());
  TH1D* hrec_theta = (TH1D*)f->Get(("h_rec_theta_" + particle).c_str());
  TH1D* hrec_eta   = (TH1D*)f->Get(("h_rec_eta_"   + particle).c_str());
  TH1D* hrec_phi   = (TH1D*)f->Get(("h_rec_phi_"   + particle).c_str());

  // Acceptance histograms
  TH1D* h_p     = (TH1D*)f->Get(("h_acc_p_"     + particle).c_str());
  TH1D* h_theta = (TH1D*)f->Get(("h_acc_theta_" + particle).c_str());
  TH1D* h_eta   = (TH1D*)f->Get(("h_acc_eta_"   + particle).c_str());
  TH1D* h_phi   = (TH1D*)f->Get(("h_acc_phi_"   + particle).c_str());

  if (!h_p || !h_theta || !h_eta || !h_phi) {
    Error("DrawParticlePlots", "Missing acceptance histograms for %s",
          particle.c_str());
    return;
  }

  // Acceptance histograms
  TH1D* h_p_res     = (TH1D*)f->Get(("h_res_p_"     + particle).c_str());
  TH1D* h_theta_res = (TH1D*)f->Get(("h_res_theta_" + particle).c_str());
  TH1D* h_eta_res   = (TH1D*)f->Get(("h_res_eta_"   + particle).c_str());
  TH1D* h_phi_res   = (TH1D*)f->Get(("h_res_phi_"   + particle).c_str());

  if (!h_p_res || !h_theta_res || !h_eta_res || !h_phi_res) {
    Error("DrawParticlePlots", "Missing resolution histograms for %s",
          particle.c_str());
    return;
  }

  // -- all style -- 
  for (auto h : {h_p, h_theta, h_eta, h_phi, h_p_res, h_theta_res, h_eta_res, h_phi_res}) {
    h->SetMinimum(0.0);
    h->SetLineColor(kBlack);
    h->SetLineWidth(2);
    h->SetMarkerStyle(20);
    h->SetStats(0);
  }
  // -- truth plots --
  for (auto h: {htru_p, htru_theta, htru_eta, htru_phi}){
    h->SetLineColor(kBlack);
  }
  // -- reco plots --
  for (auto h: {hrec_p, hrec_theta, hrec_eta, hrec_phi}){
    h->SetLineColor(kMagenta);
  }
  
  // -- phi plots --
  for (auto h : {htru_phi, hrec_phi}){
    h->SetMinimum(0.0);
  }
  // -- acceptance specific style --
  for (auto h : {h_p, h_theta, h_eta, h_phi}) {
    double acc_max = 1.05;
    if(h->GetMaximum()<acc_max)
      h->SetMaximum(acc_max);
  }
  // -- resolution specific style --
  for (auto h : {h_p_res, h_theta_res, h_eta_res, h_phi_res}) {
    h->SetLineColor(kRed);
  }
  
  set_ePIC_style(); //official epic
  //gStyle->SetHistogramLineColor(kBlack);
  gStyle->SetOptTitle(0);
  gStyle->SetOptTitle(0);
  gStyle->SetOptFit(0);
  gStyle->SetPadRightMargin(0.1);
  
  TLatex tex;
  tex.SetTextSize(0.05);
  tex.SetTextAlign(13);
  tex.SetNDC();
  TGaxis::SetMaxDigits(3);


  // -- Draw Tru/Rec --
  TCanvas* c0 = new TCanvas(("c_rectru_" + particle).c_str(),
			    (pc.title + " Reco vs Truth").c_str(),
			    1400,1000);
  c0->Divide(2,2);
  c0->cd(1)->SetLogy();
  htru_p->SetMinimum(0.1);
  htru_p->Draw("hist");
  hrec_p->Draw("hist same");
  gPad->Print((pc.name+"_pmag.png").c_str());

  c0->cd(2);
  htru_theta->Draw("hist");
  hrec_theta->Draw("hist same");
  gPad->Print((pc.name+"_theta.png").c_str());

  c0->cd(3);
  htru_eta->Draw("hist");
  hrec_eta->Draw("hist same");
  gPad->Print((pc.name+"_eta.png").c_str());

  c0->cd(4);
  htru_phi->Draw("hist");
  hrec_phi->Draw("hist same");
  gPad->Print((pc.name+"_phi.png").c_str());

  //c0->Update();
  c0->Print((pc.name+"_trurec.png").c_str());
  c0->Print((pc.name+"_trurec.pdf").c_str());
  
  //c0->Close();

  // -- Draw Acceptances --
  TCanvas* c1 = new TCanvas(("c_acc_" + particle).c_str(),
			   (pc.title + " Acceptances").c_str(),
			   1400,1000);
  c1->Divide(2,2);
  c1->cd(1);
  h_p->Draw("hist");
  DrawUnityLine(h_p);
  gPad->Print((pc.name+"_pmag_acc.png").c_str());

  c1->cd(2);
  h_theta->Draw("hist");
  DrawUnityLine(h_theta);
  gPad->Print((pc.name+"_theta_acc.png").c_str());

  c1->cd(3);
  h_eta->Draw("hist");
  DrawUnityLine(h_eta);
  gPad->Print((pc.name+"_phi_acc.png").c_str());

  c1->cd(4);
  h_phi->Draw("hist");
  DrawUnityLine(h_phi);
  gPad->Print((pc.name+"_eta_acc.png").c_str());

  //c1->Update();
  c1->Print((pc.name+"_acceptances.png").c_str());
  c1->Print((pc.name+"_acceptances.pdf").c_str());
  
  //c1->Close();

  // -- Draw Resolutions --
  TCanvas* c2 = new TCanvas(("c_res_" + particle).c_str(),
                            (pc.title + " Resolutions").c_str(),
			    1400,1000);
  c2->cd();
  
  TF1 *fp = new TF1("fp","gaus");
  h_p_res->Fit(fp,"Q","");
  TF1 *fth = new TF1("fth","gaus");
  h_theta_res->Fit(fth,"Q","");
  TF1 *feta = new TF1("feta","gaus");
  h_eta_res->Fit(feta,"Q","");
  TF1 *fphi = new TF1("fphi","gaus");
  h_phi_res->Fit(fphi,"Q","");

  c2->Clear();
  c2->Divide(2,2);

  c2->cd(1);
  h_p_res->Draw("hist");
  auto p_sig = fp->GetParameter(2);
  tex.DrawLatex(.2,.8,Form("#sigma_{#Deltap/p} = %s %%", FormatValue(100*p_sig).c_str()));
  gPad->Print((pc.name+"_pmag_res.png").c_str());

		
  c2->cd(2);
  h_theta_res->Draw("hist");
  auto th_sig = fth->GetParameter(2);
  tex.DrawLatex(.2,.8,Form("#sigma_{#Delta#theta} = %s mrad", FormatValue(th_sig).c_str()));
  gPad->Print((pc.name+"_theta_res.png").c_str());
  
  c2->cd(3);
  h_eta_res->Draw("hist");
  auto eta_sig = feta->GetParameter(2);
  tex.DrawLatex(.2,.8,Form("#sigma_{#Delta#eta} = %s", FormatValue(eta_sig).c_str()));
  gPad->Print((pc.name+"_eta_res.png").c_str());
  
  c2->cd(4);
  h_phi_res->Draw("hist");
  auto phi_sig = fphi->GetParameter(2);
  tex.DrawLatex(.2,.8,Form("#sigma_{#Delta#phi} = %s rad", FormatValue(phi_sig).c_str()));
  gPad->Print((pc.name+"_phi_res.png").c_str());

  //c2->Update();
  c2->Print((pc.name+"_resolutions.png").c_str());
  c2->Print((pc.name+"_resolutions.pdf").c_str());
  c2->Close();

}
