#include "/home/gp140f/epic_plot_macro/ePIC_style.C"

#include "../include/Config.h"
#include "../include/ParticleConfig.h"
#include "../include/FiducialCuts.h"



void DrawUnityLine(TH1* h)
{
  double xmin = h->GetXaxis()->GetXmin();
  double xmax = h->GetXaxis()->GetXmax();

  TLine* l = new TLine(xmin, 1.0, xmax, 1.0);
  l->SetLineColor(kRed);
  l->SetLineStyle(2);
  l->SetLineWidth(2);
  l->Draw("same");
}

std::string FormatValue(double x) {
  return (std::abs(x) >= 0.01)
    ? Form("%.2f", x)
    : Form("%.*f", int(std::ceil(-std::log10(std::abs(x)))) + 1, x);
}

double RoundToSigFig(double x, int sigfig = 1)
{
  if (x == 0.0) return 0.0;

  double scale = std::pow(10.0,
      std::floor(std::log10(std::abs(x))) - sigfig + 1);

  return std::round(x / scale) * scale;
}

TString FormatSigma(double sigma, const char* name, const char* unit)
{
  if (sigma >= 0.1)
    return Form("#sigma_{#Delta%s} = %.2f %s", name, sigma, unit);
  else if (sigma >= 0.01)
    return Form("#sigma_{#Delta%s} = %.3f %s", name, sigma, unit);
  else
    return Form("#sigma_{#Delta%s} = %.2e %s", name, sigma, unit);
}

void MakeParticlePlots(const ParticleConfig& pc,
			 const std::string& tru_file,
			 const std::string& rec_file,
			 TFile& outfile)
{

  // -- DataFrames --
  ROOT::RDataFrame df_tru("tree", tru_file);
  //auto df_tru_fidu = df_tru.Filter(
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
    },{pc.tru_eta, pc.rec_eta});


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
  
  // -- Draw Acceptances --
  TCanvas* c = new TCanvas(("c_acc_" + particle).c_str(),
			   (pc.title + " Acceptances").c_str(),
			   1400,1000);
  c->Divide(2,2);
  c->cd(1);
  h_p->Draw("hist");
  DrawUnityLine(h_p);
  gPad->Print((pc.name+"_pmag_acc.png").c_str());

  c->cd(2);
  h_theta->Draw("hist");
  DrawUnityLine(h_theta);
  gPad->Print((pc.name+"_theta_acc.png").c_str());

  c->cd(3);
  h_eta->Draw("hist");
  DrawUnityLine(h_eta);
  gPad->Print((pc.name+"_phi_acc.png").c_str());

  c->cd(4);
  h_phi->Draw("hist");
  DrawUnityLine(h_phi);
  gPad->Print((pc.name+"_eta_acc.png").c_str());

  //c->Update();
  c->Print((pc.name+"_acceptances.png").c_str());
  c->Print((pc.name+"_acceptances.pdf").c_str());
  
  c->Close();

  // -- Draw Resolutions --
  TCanvas* c1 = new TCanvas(("c_res_" + particle).c_str(),
                            (pc.title + " Resolutions").c_str(),
			    1400,1000);
  c1->cd();
  
  TF1 *fp = new TF1("fp","gaus");
  h_p_res->Fit(fp,"Q","");
  TF1 *fth = new TF1("fth","gaus");
  h_theta_res->Fit(fth,"Q","");
  TF1 *feta = new TF1("feta","gaus");
  h_eta_res->Fit(feta,"Q","");
  TF1 *fphi = new TF1("fphi","gaus");
  h_phi_res->Fit(fphi,"Q","");

  c1->Clear();
  c1->Divide(2,2);

  c1->cd(1);
  h_p_res->Draw("hist");
  auto p_sig = fp->GetParameter(2);
  tex.DrawLatex(.2,.8,Form("#sigma_{#Deltap/p} = %s %%", FormatValue(100*p_sig).c_str()));
  gPad->Print((pc.name+"_pmag_res.png").c_str());

		
  c1->cd(2);
  h_theta_res->Draw("hist");
  auto th_sig = fth->GetParameter(2);
  tex.DrawLatex(.2,.8,Form("#sigma_{#Delta#theta} = %s mrad", FormatValue(th_sig).c_str()));
  gPad->Print((pc.name+"_theta_res.png").c_str());
  
  c1->cd(3);
  h_eta_res->Draw("hist");
  auto eta_sig = feta->GetParameter(2);
  tex.DrawLatex(.2,.8,Form("#sigma_{#Delta#eta} = %s", FormatValue(eta_sig).c_str()));
  gPad->Print((pc.name+"_eta_res.png").c_str());
  
  c1->cd(4);
  h_phi_res->Draw("hist");
  auto phi_sig = fphi->GetParameter(2);
  tex.DrawLatex(.2,.8,Form("#sigma_{#Delta#phi} = %s rad", FormatValue(phi_sig).c_str()));
  gPad->Print((pc.name+"_phi_res.png").c_str());

  //c1->Update();
  c1->Print((pc.name+"_resolutions.png").c_str());
  c1->Print((pc.name+"_resolutions.pdf").c_str());
  c1->Close();

}

void SingleParticleAnalysis(std::string infiledir = test_out_dir, std::string beam_setting = "18x275", std::string campaign = "test", std::string helicity = "hplus"){

  // ----------------------------------------------------------
  // Output
  // ----------------------------------------------------------
  std::string resultdir = "./results/"+campaign+"/"+beam_setting+"/"+helicity+"/"; 
  gSystem->Exec(("mkdir -p "+resultdir).c_str());
  std::string outfilename = "TCS_"+beam_setting+"_particles.root";
  TFile fout(outfilename.c_str(), "RECREATE");
  
  std::string tru_filename = infiledir + "TCS_tru_Tree.root";
  std::string rec_filename = infiledir + "TCS_all_rec_Tree.root";

  std::vector<ParticleConfig> particles;
  //scattered electron
  ParticleConfig scat_ele{
      "scat_ele", "Scattered Electron", "e'",
      11,//pdg
      50, 100,//nbins acc ,res
      0.0, 20.0, -0.1, 0.1,//pmag
      3.12, 3.14, -6, 6,//theta
      -3.14, 3.14, -3.14, 3.14,//phi
      -12.0, -6.0, -1, 1//eta
  };
  
  //decay electron
  ParticleConfig ele{
      "ele", "Decay Electron", "e^{-}",
      11,
      50, 100,
      0.0, 200.0, -0.1, 0.1,
      0, 3.14, -10, 10,
      -3.14, 3.14, -0.2, 0.2,
      -10.0, 10.0, -0.05, 0.05
  };
  
  //decay positron
  ParticleConfig pos = ele;
  pos.name = "pos"; pos.title = "Decay Positron";
  pos.label = "e^{+}"; pos.pdg = -11;
  pos.UpdateDerived();
  
  //recoil proton
  ParticleConfig pprime{
      "pprime", "Recoil Proton", "p'",
      2212,
      50, 100,
      150, 275.0, -0.1, 0.1,
      0.0, 0.01, -5, 5,
      -3.14, 3.14, -3.14, 3.14,
      4.00, 12.0, -1, 1
    };

  particles.push_back(scat_ele);
  particles.push_back(ele);
  particles.push_back(pos);
  particles.push_back(pprime);
  
  for (const auto& pc : particles) {
    MakeParticlePlots(pc, tru_filename, infiledir+pc.rec_filename, fout);
  }

  fout.Close();
  
  for (const auto& pc : particles) {
    DrawParticlePlots(outfilename, pc);
  }

  gSystem->Exec("pdfunite *_acceptances.pdf *_resolutions.pdf SingleParticleResults.pdf");
  gSystem->Exec(("mv *.png *.pdf *.root "+resultdir).c_str());
}
