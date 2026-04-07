#include "/home/garyp/eic/epic_plot_macro/ePIC_style.C"
#include "/home/garyp/eic/EPICStyle/EPICStyle.C"

const std::vector<EColor> kp6 = {kP6Violet, kP6Gray, kP6Grape, kP6Red, kP6Yellow, kP6Blue};
const std::vector<int> vEcom = {20, 45, 140};
const std::vector<string> vEset = {"5x41", "10x100", "18x275"};

using ROOT::VecOps::RVec;
using ROOT::Math::PxPyPzMVector;
using ROOT::Math::VectorUtil::boost;


//later put these into headers in main analysis repo
void DrawEpicText(){

  TLatex text_ePIC;
   text_ePIC.SetTextSize(0.05);
   text_ePIC.SetTextFont(62);
   text_ePIC.DrawLatexNDC(.15,.88,"ePIC Performance");  // performance plot
   //text_ePIC.DrawLatexNDC(.15,.88,"ePIC Internal");  // for internal use only
   //text_ePIC.DrawLatexNDC(.15,.88,"ePIC Preliminary"); // preliminary released version 
   //text_ePIC.DrawLatexNDC(.15,.88,"ePIC Work in Progress"); // work in progress to be shown outside
   //text_ePIC.DrawLatexNDC(.15,.88,"ePIC"); // final published version
   //return text_ePIC;

}

void DrawEpicCOMText(int Ecom){

  TLatex text_com;
  text_com.SetTextAlign(13);  //align at top
  text_com.SetTextSize(0.04);
  text_com.SetTextFont(42);
  text_com.DrawLatexNDC(.15,.85,Form("e+p, #sqrt{s} = %d GeV",Ecom));
}

void DrawEpicEnergyText(string Ecomb){

  TLatex tex;
  tex.SetTextAlign(13);  //align at top
  tex.SetTextSize(0.04);
  tex.SetTextFont(42);
  tex.DrawLatexNDC(.15,.85,Form("e+p, %s GeV",Ecomb.c_str()));

}

void DrawLumiText(int lumi = 10){
  
  TLatex tex;
  tex.SetTextAlign(13);  //align at top
  tex.SetTextSize(0.04);
  tex.SetTextFont(42);
  tex.DrawLatexNDC(.15,.8,Form("L_{proj} = %d fb^{-1}",lumi));
  
}

void DrawSimText(double xpos=0.65, double ypos=1.0){
  
  TLatex tex;
  tex.SetTextAlign(13);
  tex.SetTextSize(0.04);
  tex.SetTextFont(42);
  tex.DrawLatexNDC(xpos,ypos-0.08,"25.10.3 Campaign");
  tex.DrawLatexNDC(xpos,ypos-0.12,"EpIC 1.1.6");
  tex.DrawLatexNDC(xpos,ypos-0.16,"EpIC ep #rightarrow e'#gammap");
  
}
void DrawEPIC(){

  // ===== Add ePIC logo to the figure if desired ======
  TImage *logo = TImage::Open("/home/garyp/eic/epic_plot_macro/EPIC-logo_black_small.png");
  TPad *pad2 = new TPad("pad2", "Pad 2", 0.8, 0.8, 0.93, 0.93); // Create a new pad and then draw the image in it
  pad2->Draw();
  pad2->cd(); // Enter the new pad
  logo->Draw();
}

void DrawOneSplitTwo(TH1D* all, TH1D* h1, TH1D* h2 = nullptr){

  all->SetLineColor(kBlack);

  if (h1) {
    h1->SetFillColorAlpha(kp6[0],0.8);
    h1->SetFillStyle(3004);
  }

  if (h2) {
    h2->SetFillColorAlpha(kp6[1],0.8);
    h2->SetFillStyle(3005);
  }

  all->DrawCopy("hist p");
  if (h1) h1->DrawCopy("same");
  if (h2) h2->DrawCopy("same");
}

// void DrawOneSplitTwo(TH1D* all, TH1D* h1, TH1D* h2){
  
//   all->SetLineColor(kBlack);
//   //h1->SetLineColor(0);
//   h1->SetFillColorAlpha(kp6[0],0.8);
//   h1->SetFillStyle(3004);
//   //h2->SetLineColor(0);
//   h2->SetFillColorAlpha(kp6[1],0.8);
//   h2->SetFillStyle(3005);
//   //all->SetMaximum(1.25*all->GetMaximum());
//   all->DrawCopy("hist p");
//   h1->DrawCopy("same");
//   h2->DrawCopy("same");
  
// }


TH1D* AcceptancePlot(TH1D* tru, TH1D* rec, std::string name){

  TH1D* acc = (TH1D*)rec->Clone(name.c_str());
  acc->Divide(tru);
  acc->SetMinimum(0);
  acc->SetMaximum(1);
  acc->GetYaxis()->SetTitle("Acceptance");
  acc->SetMarkerStyle(7);
  return acc;
  
}


TH1D* XSecPlot(TH1D* h, double scale, std::string name, TH1D* acc=nullptr){

  TH1D* sig = (TH1D*)h->Clone(name.c_str());
  if(!(acc==nullptr)) sig->Divide(acc);
  sig->Scale(scale,"width");
  
  return sig;

}


//main plot macro
void Analysis(TString filename="/w/work5/home/garyp/rad_trees/MCMatchedDetector_DVCS_10x100.root"){
  
  using ROOT::VecOps::RVec;
  
  set_ePIC_style(); //official epic
  //SetEPICStyle(); //my version
  //style corrections needed for official epic
  gStyle->SetOptTitle(0);
  gStyle->SetOptFit(0);
  gStyle->SetPadRightMargin(0.1);

  TLatex tex;
  tex.SetTextSize(0.05);
  tex.SetTextAlign(13);
  tex.SetNDC();
  TGaxis::SetMaxDigits(3);
  ROOT::RDataFrame dfbase("rad_tree",filename);
  auto dfsafe = dfbase.Filter("tru_Q2>0 && tru_theta[scat_ele]!=TMath::Pi()");
  
  int Ecom = vEcom[1];
  string Eset = vEset[1];
  //double Ngen = 1e6;
  double xsec = 4.68; //nb
  double dxsec = 0.12; //nb
  double lumi_fb = 5;
  double lumi_nb = lumi_fb * 1e6; //fb-1 to nb-1
  // double scale = xsec*Lumi / Ngen;
  // double dscale = dxsec*Lumi;
  
  //scattered electron kinematic defines
  auto df = dfsafe
    .Define("tru_logQ2",[](double Q2){
	return std::log10(Q2);
      },{"tru_Q2"})
    .Define("rec_logQ2",[](double Q2){
	return std::log10(Q2);
      },{"rec_Q2"})
    .Define("tru_pmag_scatele",[](RVec<double> pmag,int idx){
	return pmag[idx];
      },{"tru_pmag","scat_ele"})
    .Define("rec_pmag_scatele",[](RVec<double> pmag,int idx){
	return pmag[idx];
      },{"rec_pmag","scat_ele"})
    .Define("tru_theta_scatele",[](RVec<double> theta,int idx){
	return theta[idx];
      },{"tru_theta","scat_ele"})
    .Define("rec_theta_scatele",[](RVec<double> theta,int idx){
	return theta[idx];
      },{"rec_theta","scat_ele"})
    .Define("tru_phi_scatele",[](RVec<double> phi,int idx){
	return phi[idx];
      },{"tru_phi","scat_ele"})
    .Define("rec_phi_scatele",[](RVec<double> phi,int idx){
	return phi[idx];
      },{"rec_phi","scat_ele"});
  
  //calculated scatele
  // df = df
  //   .Define("rec_pmag_calc_scatele",[](RVec<double> pmag,int idx){
  // 	return pmag[idx];
  //     },{"rec_pmag","rec_calc_scatele"})
  //   .Define("rec_theta_calc_scatele",[](RVec<double> theta,int idx){
  // 	return theta[idx];
  //     },{"rec_theta","rec_calc_scatele"})
  //   .Define("rec_phi_calc_scatele",[](RVec<double> phi,int idx){
  // 	return phi[idx];
  //     },{"rec_phi","rec_calc_scatele"})
  //   .Define("rec_calc_logQ2",[](double Q2){
  // 	return log10(Q2);
  //     },{"rec_calc_Q2"});
    
  //scattered electron resolution defines
  df = df
    .Define("res_pmag_scatele",[](RVec<double> tru, RVec<double> rec, int idx){
	return (tru[idx] - rec[idx])/(tru[idx]);
      },{"tru_pmag","rec_pmag","scat_ele"})
    .Define("res_theta_scatele",[](RVec<double> tru, RVec<double> rec, int idx){
	return 1000 * (tru[idx] - rec[idx]);
      },{"tru_theta","rec_theta","scat_ele"})
   .Define("res_phi_scatele",[](RVec<double> tru, RVec<double> rec, int idx){
	return tru[idx] - rec[idx];
      },{"tru_phi","rec_phi","scat_ele"})
    .Define("res_Q2",[](double tru_Q2, double rec_Q2){
	return tru_Q2 - rec_Q2;
      },{"tru_Q2","rec_Q2"});
  
  //scattered electron resolution defines
  // df = df
  //   .Define("res_pmag_calc_scatele",[](RVec<double> tru, RVec<double> rec, int tru_idx, int rec_idx){
  // 	return (tru[tru_idx] - rec[rec_idx])/(tru[tru_idx]);
  //     },{"tru_pmag","rec_pmag","scat_ele","rec_calc_scatele"})
  //   .Define("res_theta_calc_scatele",[](RVec<double> tru, RVec<double> rec, int tru_idx, int rec_idx){
  // 	return 1000*(tru[tru_idx] - rec[rec_idx]);
  //     },{"tru_theta","rec_theta","scat_ele","rec_calc_scatele"})
  //   .Define("res_phi_calc_scatele",[](RVec<double> tru, RVec<double> rec, int tru_idx, int rec_idx){
  // 	return tru[tru_idx] - rec[rec_idx];
  //     },{"tru_phi","rec_phi","scat_ele","rec_calc_scatele"})
  //   .Define("res_calc_Q2",[](double tru, double rec){
  // 	return tru-rec;
  //     },{"tru_Q2","rec_calc_Q2"});
  
  //recoil proton
  df = df
    .Define("tru_pmag_pprime",[](RVec<double> pmag, int idx){
	return pmag[idx];
      },{"tru_pmag","pprime"})
    .Define("tru_theta_pprime",[](RVec<double> theta, int idx){
	return theta[idx];
      },{"tru_theta","pprime"})
    .Define("tru_phi_pprime",[](RVec<double> phi, int idx){
      return phi[idx];
    },{"tru_phi","pprime"})
    .Define("rec_pmag_pprime",[](RVec<double> pmag, int idx){
	return pmag[idx];
      },{"rec_pmag","pprime"})
    .Define("rec_theta_pprime",[](RVec<double> theta, int idx){
	return theta[idx];
      },{"rec_theta","pprime"})
    .Define("rec_phi_pprime",[](RVec<double> phi, int idx){
      return phi[idx];
    },{"rec_phi","pprime"});
  
  
  //calculated recoil proton
  df = df
    .Define("rec_pmag_calc_pprime",[](RVec<double> pmag, int idx){
	return pmag[idx];
      },{"rec_pmag","rec_calc_pprime"})
    .Define("rec_theta_calc_pprime",[](RVec<double> theta, int idx){
	return theta[idx];
      },{"rec_theta","rec_calc_pprime"})
    .Define("rec_phi_calc_pprime",[](RVec<double> phi, int idx){
	return phi[idx];
      },{"rec_phi","rec_calc_pprime"});
  
  df = df
    .Define("delta_pmag_pprime",[](double rec, double calc){
	return rec - calc;
      },{"rec_pmag_pprime","rec_pmag_calc_pprime"})
    .Define("delta_theta_pprime",[](double rec, double calc){
	return 1000*(rec - calc);
      },{"rec_theta_pprime","rec_theta_calc_pprime"})
    .Define("delta_phi_pprime",[](double rec, double calc){
	return rec - calc;
      },{"rec_phi_pprime","rec_phi_calc_pprime"});
  
  
  //physics variable defines
  df = df 
    .Define("res_tbot",[](double tru_t, double rec_t){
	return tru_t - rec_t;
      },{"tru_t_bot","rec_t_bot"})
      .Define("res_ttop",[](double tru_t, double rec_t){
	return tru_t - rec_t;
      },{"tru_t_top","rec_t_top"})
    .Define("res_W",[](double tru_W, double rec_W){
	return tru_W - rec_W;
      },{"tru_W","rec_W"})
    .Define("res_Whad",[](double tru_Whad, double rec_Whad){
	return tru_Whad - rec_Whad;
      },{"tru_Whad","rec_Whad"})
    .Define("res_TrentoPhi",[](double tru_ph, double rec_ph){
	return tru_ph - rec_ph;
      },{"tru_TrentoPhi","rec_TrentoPhi"});
  
  df = df
    .Define("tru_logt_top",[](double t){
	return log10(t);
      },{"tru_t_top"})
    .Define("rec_logt_top",[](double t){
	return log10(t);
      },{"rec_t_top"})
    .Define("tru_logt_bot",[](double t){
	return log10(t);
      },{"tru_t_bot"})
    .Define("rec_logt_bot",[](double t){
	return log10(t);
      },{"rec_t_bot"});
  
  //polarisation
  df = df
    .Define("res_GammaPol",[](double tru, double rec){
	return tru-rec;
      },{"tru_GammaPol","rec_GammaPol"})
    .Define("res_calc_GammaPol",[](double tru, double rec){
	return tru-rec;
      },{"tru_GammaPol","rec_calc_GammaPol"})
    .Define("res_GammaPolCirc",[](double tru, double rec){
	return tru-rec;
      },{"tru_GammaPolCirc","rec_GammaPolCirc"})
    .Define("res_calc_GammaPolCirc",[](double tru, double rec){
	return tru-rec;
      },{"tru_GammaPolCirc","rec_calc_GammaPolCirc"});
  
  df = df
    .Define("tru_pmag_gprime", [](RVec<double> pmag, int idx){
	return pmag[idx];
      },{"tru_pmag","gprime"})
    .Define("tru_theta_gprime", [](RVec<double> theta, int idx){
	return theta[idx];
      },{"tru_theta","gprime"})
    .Define("tru_eta_gprime", [](RVec<double> eta, int idx){
	return eta[idx];
      },{"tru_eta","gprime"})
    .Define("tru_phi_gprime", [](RVec<double> phi, int idx){
	return phi[idx];
      },{"tru_phi","gprime"})
    .Define("rec_pmag_gprime", [](RVec<double> pmag, int idx){
	return pmag[idx];
      },{"rec_pmag","gprime"})
    .Define("rec_theta_gprime", [](RVec<double> theta, int idx){
	return theta[idx];
      },{"rec_theta","gprime"})
    .Define("rec_eta_gprime", [](RVec<double> eta, int idx){
	return eta[idx];
      },{"rec_eta","gprime"})
    .Define("rec_phi_gprime", [](RVec<double> phi, int idx){
	return phi[idx];
      },{"rec_phi","gprime"});
    
  //reconstructed scattered electron cuts
  TCut recon_scatele = "rec_pmag_scatele>0 && ReconstructedParticles_PDG[scat_ele]==11";
  
  //reconstructed meson cuts
  TCut recon_gprime = "rec_pmag[gprime]>0";
  
  //reconstructed proton cuts
  //this ensures the pprime pmag was filled from rp and not b0
  //good for 275, but for lower energy configs in future a better method needed
  //probably short algorithm in a lambda capture - maybe combis?
  TCut recon_pprime = "rec_pmag[pprime]>0";
  
  //calculated electron cuts
  
  //recoil pprime
  //from hdelta_*_pprime fits
  double dpmag_pprime = 1.45; //GeV/c
  double dtheta_pprime = 0.25; //mrad
  double dphi_pprime = 0.19; //rad
  TCut dpmag_pprime_cut = Form("fabs(delta_pmag_pprime) < 3*%f",dpmag_pprime);
  TCut dtheta_pprime_cut = Form("fabs(delta_theta_pprime) < 3*%f",dtheta_pprime);
  TCut dphi_pprime_cut = Form("fabs(delta_phi_pprime) < 3*%f",dphi_pprime);
  dphi_pprime_cut = "";
  
  //missing pprime
  TCut semi_excl_recon = recon_scatele && recon_gprime && (!recon_pprime);
  TCut pprime_missmass_cut = "fabs(rec_MissMass_pprime - 0.938) < 1"; 
  //pprime_missmass_cut = "";
  TCut semi_excl_phys = semi_excl_recon && pprime_missmass_cut; //missing proton cuts
  
  //exclusivity
  TCut excl_recon = recon_scatele && recon_gprime && recon_pprime;
  TCut excl_phys = excl_recon && dphi_pprime_cut;
  
  auto df_scatele = df.Filter(recon_scatele.GetTitle());
  auto df_gprime = df.Filter(recon_gprime.GetTitle());
  auto df_pprime = df.Filter(recon_pprime.GetTitle());
  
  auto df_semi_excl = df.Filter(semi_excl_recon.GetTitle());
  auto df_semi_excl_phys = df_semi_excl.Filter(semi_excl_phys.GetTitle());
  
  auto df_excl = df.Filter(excl_recon.GetTitle());
  auto df_excl_phys = df.Filter(excl_phys.GetTitle());
  
  ////////////////////////////
  //Histograms
  ////////////////////////////
  
  auto nbins_acc = 50;
  auto nbins_xsec = nbins_acc;
  auto nbins_res = 100;
  auto nbins2D = 100;
  
  //electron acceptance
  //truth
  auto htru_Q2 = df.Histo1D({"htru_Q2", ";Q^{2} [GeV^{2}]", nbins_acc, 0, 10},"tru_Q2");
  auto htru_logQ2 = df.Histo1D({"htru_logQ2", ";log_{10}(Q^{2}) [GeV^{2}]", nbins_acc, 0, 2},"tru_logQ2");
  auto htru_W = df.Histo1D({"htru_W", ";W [GeV]", nbins_acc, 0, 150},"tru_W");
  auto htru_pmag_scatele = df.Histo1D({"htru_pmag_scatele", ";p_{e} [GeV/c]", nbins_acc, 0, 18},"tru_pmag_scatele");
  auto htru_theta_scatele = df.Histo1D({"htru_theta_scatele", ";#theta_{e} [rad]", nbins_acc, 3.13, M_PI},"tru_theta_scatele");
  auto htru_phi_scatele = df.Histo1D({"htru_phi_scatele", ";#phi_{e} [rad]", nbins_acc, -M_PI, M_PI},"tru_phi_scatele");
  //reconstructed
  auto hrec_Q2_scatele = df_scatele.Histo1D({"hrec_Q2_scatele", ";Q^{2} [GeV^{2}]", nbins_acc, 0, 10},"rec_Q2");
  auto hrec_logQ2_scatele = df_scatele.Histo1D({"hrec_logQ2_scatele", ";log_{10}(Q^{2}) [GeV^{2}]", nbins_acc, 0, 2},"rec_logQ2");
  auto hrec_W_scatele = df_scatele.Histo1D({"hrec_W_scatele", ";W [GeV]", nbins_acc, 0, 150},"rec_W");
  auto hrec_pmag_scatele = df_scatele.Histo1D({"hrec_pmag_scatele", ";p_{e} [GeV/c]", nbins_acc, 0, 18},"rec_pmag_scatele");
  auto hrec_theta_scatele = df_scatele.Histo1D({"hrec_theta_scatele", ";#theta_{e} [rad]", nbins_acc, 0, M_PI},"rec_theta_scatele");
  auto hrec_phi_scatele = df_scatele.Histo1D({"hrec_phi_scatele", ";#phi_{e} [rad]", nbins_acc, -M_PI, M_PI},"rec_phi_scatele");
  
  
  //meson(photon) acceptance in main detector
  //truth
  auto htru_pmag_gprime = df.Histo1D({"htru_pmag_gprime",";p_{#gamma} [GeV/c];N_{evts}}",nbins_acc,0,30},"tru_pmag_gprime");
  auto htru_theta_gprime = df.Histo1D({"htru_theta_gprime",";#theta_{#gamma} [rad];N_{evts}}",nbins_acc,0,M_PI},"tru_theta_gprime");
  auto htru_eta_gprime = df.Histo1D({"htru_eta_gprime",";#eta_{#gamma};N_{evts}}",nbins_acc,-10,10},"tru_eta_gprime");
  auto htru_phi_gprime = df.Histo1D({"htru_phi_gprime",";#phi_{#gamma} [rad];N_{evts}}",nbins_acc,0,M_PI},"tru_phi_gprime");
  //reconstructed
  auto hrec_pmag_gprime = df_gprime.Histo1D({"hrec_pmag_gprime",";p_{#gamma} [GeV/c];N_{evts}}",nbins_acc,-10,30},"rec_pmag_gprime");
  auto hrec_theta_gprime = df_gprime.Histo1D({"hrec_theta_gprime",";#theta_{#gamma} [rad];N_{evts}}",nbins_acc,0,M_PI},"rec_theta_gprime");
  auto hrec_eta_gprime = df_gprime.Histo1D({"hrec_eta_gprime",";#eta_{#gamma};N_{evts}}",nbins_acc,-10,10},"rec_eta_gprime");
  auto hrec_phi_gprime = df_gprime.Histo1D({"hrec_phi_gprime",";#phi_{#gamma} [rad];N_{evts}}",nbins_acc,0,M_PI},"rec_phi_gprime");
  
  
  //proton acceptance in far forward
  //truth
  auto htru_ttop = df.Histo1D({"htru_ttop", ";|t|_{e#gamma} [GeV^{2}]", nbins_acc, 0, 2},"tru_t_top");
  auto htru_tbot = df.Histo1D({"htru_tbot", ";|t|_{pp'} [GeV^{2}]", nbins_acc, 0, 2},"tru_t_bot");
  auto htru_pmag_pprime = df.Histo1D({"htru_pmag_pprime", ";p_{p'} [GeV/c]", nbins_acc, 0, 300},"tru_pmag_pprime");
  auto htru_theta_pprime = df.Histo1D({"htru_theta_pprime", ";#theta_{p'} [rad]", nbins_acc, 0, 0.007},"tru_theta_pprime");
  auto htru_phi_pprime = df.Histo1D({"htru_phi_pprime", ";#phi_{p'} [rad]", nbins_acc, -M_PI, M_PI},"tru_phi_pprime");
  //reconstructed
  auto hrec_ttop = df_pprime.Histo1D({"hrec_tbot", ";|t|_{e#gamma} [GeV^{2}]", nbins_acc, 0, 2},"rec_t_top");
  auto hrec_tbot = df_pprime.Histo1D({"hrec_tbot", ";|t|_{pp'} [GeV^{2}]", nbins_acc, 0, 2},"rec_t_bot");
  auto hrec_pmag_pprime = df_pprime.Histo1D({"hrec_pmag_pprime", ";p_{p'} [GeV/c]", nbins_acc, 0, 300},"rec_pmag_pprime");
  auto hrec_theta_pprime = df_pprime.Histo1D({"hrec_theta_pprime", ";#theta_{p'} [rad]", nbins_acc, 0, 0.007},"rec_theta_pprime");
  auto hrec_phi_pprime = df_pprime.Histo1D({"hrec_phi_pprime", ";#phi_{p'} [rad]", nbins_acc, -M_PI, M_PI},"rec_phi_pprime");

  


  //electron resolutions
  auto hres_Q2 = df_scatele.Histo1D({"hres_Q2", ";#DeltaQ^{2} [GeV^{2}];N_{evts}", nbins_res, -0.001, 0.001},"res_Q2");
  auto hres_pmag_scatele = df_scatele.Histo1D({"hres_pmag_scatele", ";#Deltap_{e} / p_{e};N_{evts}", nbins_res, -0.1, 0.1},"res_pmag_scatele");
  auto hres_theta_scatele = df_scatele.Histo1D({"hres_theta_scatele", ";#Delta#theta_{e} [mrad];N_{evts}", nbins_res, -6, 6},"res_theta_scatele");
  auto hres_phi_scatele = df_scatele.Histo1D({"hres_phi_scatele", ";#Delta#phi_{e} [rad];N_{evts}", nbins_res, -M_PI, M_PI},"res_phi_scatele");
  
  //photon resolutions
  //pmag eta theta phi
  
  //proton resolutions
  //pmag eta theta phi
  
  //physics resolutions
  //these need at minimum other detector cuts
  //W e'
  auto hres_W = df_scatele.Histo1D({"hres_W",";#DeltaW [GeV];N_{evts}",nbins_res,-5,5},"res_W");
  //Whad p'
  auto hres_Whad = df_pprime.Histo1D({"hres_Whad",";#DeltaWhad [GeV];N_{evts}",nbins_res,-5,5},"res_Whad");
  //ttop needs e' and gamma'
  auto hres_ttop = df_semi_excl.Histo1D({"hres_ttop",";#Delta|t_{e#gamma'}| [GeV^{2}];N_{evts}",nbins_res,-1,1},"res_ttop");
  //tbot needs p'
  auto hres_tbot = df_pprime.Histo1D({"hres_tbot",";#Delta|t_{pp'}| [GeV^{2}];N_{evts}",nbins_res,-1,1},"res_tbot");
  //Phi
  auto hres_TrentoPhi_semi_excl = df_semi_excl.Histo1D({"hres_TrentoPhi_semi_excl",";#phi_{l} [rad];N_{evts}",nbins_res,-1,1},"res_TrentoPhi");
  auto hres_TrentoPhi_excl = df_excl.Histo1D({"hres_TrentoPhi",";#phi_{l} [rad];N_{evts}",nbins_res,-1,1},"res_TrentoPhi");
  
  
  //Polarisation of electron beam (virtual photon)
  //measured pol
  auto hres_GammaPol = df_scatele.Histo1D({"hres_GammaPol", ";#DeltaPol_{lin};N_{evts}", nbins_res, -0.1, 0.1},"res_GammaPol");
  auto hres_GammaPolCirc = df_scatele.Histo1D({"hres_GammaPolCirc", ";#DeltaPol_{circ};N_{evts}", nbins_res, -0.1,0.1},"res_GammaPolCirc");
  
  //calculated pol
  auto hres_calc_GammaPol = df_scatele.Histo1D({"hres_calc_GammaPol", ";#DeltaPol_{lin} (#gamma'p');N_{evts}", nbins_res, -1,1},"res_calc_GammaPol");
  auto hres_calc_GammaPolCirc = df_scatele.Histo1D({"hres_calc_GammaPolCirc", ";#DeltaPol_{circ} (#gamma'p');N_{evts}", nbins_res, -1,1},"res_calc_GammaPolCirc");
  
  //2D meas vs calc
  auto h2d_GammaPol = df_scatele.Histo2D({"h2d_GammaPol",";Pol_{lin,recon};Pol_{lin,truth}",nbins2D,0.5,1.05,nbins2D,0.5,1.05},"rec_GammaPol","tru_GammaPol");
  auto h2d_calcGammaPol = df_scatele.Histo2D({"h2d_GammaPol",";Pol_{lin,miss e'};Pol_{lin,truth}",nbins2D,0.5,1.05,nbins2D,0.5,1.05},"rec_calc_GammaPol","tru_GammaPol");


  //exclusivity cut resolutions 
  auto hdelta_pmag_pprime = df_excl.Histo1D({"hdelta_pmag_pprime",";#Deltap_{p'} = p_{p',rec} - p_{p',calc} [GeV/c];N_{evts}",nbins_res,-20,20},"delta_pmag_pprime");
  auto hdelta_theta_pprime = df_excl.Histo1D({"hdelta_theta_pprime",";#Delta#theta_{p'} = #theta_{p',rec} - #theta_{p',calc} [mrad];N_{evts}",nbins_res,-2*M_PI,2*M_PI},"delta_theta_pprime");
  auto hdelta_phi_pprime = df_excl.Histo1D({"hdelta_phi_pprime",";#Delta#phi_{p'} = #phi_{p',rec} - #phi_{p',calc} [rad];N_{evts}",nbins_res,-2*M_PI,2*M_PI},"delta_phi_pprime");

  
  
  //d4sigma physic cross sections 
  //t exclusive
  auto htru_tbot_acc = df_excl_phys.Histo1D({"htru_tbot_acc",";|t|_{pp'} [GeV^{2}];N_{evts} / 0.02 GeV^{2} ",nbins_xsec,0,2},"tru_t_bot");
  auto hrec_tbot_phys = df_excl_phys.Histo1D({"hrec_tbot_phys",";|t|_{pp'} [GeV^{2}];N_{evts} / 0.02 GeV^{2} ",nbins_xsec,0,2},"rec_t_bot");
  
  //t semi-exclusive
  //auto htru_ttop = df.Histo1D({"htru_ttop",";|t|_{pp'} [GeV^{2}];N_{evts} / 0.02 GeV^{2} ",nbins_xsec,0,1},"tru_t_top");
  auto htru_ttop_acc = df_semi_excl_phys.Histo1D({"htru_ttop_acc",";|t|_{pp'} [GeV^{2}];N_{evts} / 0.02 GeV^{2} ",nbins_xsec,0,2},"tru_t_top");
  auto hrec_ttop_phys = df_semi_excl_phys.Histo1D({"hrec_ttop_phys",";|t|_{pp'} [GeV^{2}];N_{evts} / 0.02 GeV^{2} ",nbins_xsec,0,2},"rec_t_top");
  
  //Q2
  auto htru_Q2_acc = df_excl_phys.Histo1D({"htru_Q2",";Q^{#prime2} [GeV^{2}];N_{evts} / 0.4 GeV^{2}",nbins_xsec,0,20},"tru_Q2");
  auto hrec_Q2 = df_excl_phys.Histo1D({"hrec_Q2",";Q^{#prime2} [GeV^{2}];N_{evts} / 0.4 GeV^{2}",nbins_xsec,0,20},"rec_Q2");
  
  //xbjorken
  auto htru_xbj = df.Histo1D({"htru_xbj",";x_{bj};N_{evts} / 0.04 rad",nbins_xsec,-1,1},"tru_xbj");
  auto htru_xbj_acc = df_excl_phys.Histo1D({"htru_xbj_acc",";x_{bj};N_{evts} / 0.04 rad",nbins_xsec,-1,1},"tru_xbj");
  auto hrec_xbj = df_excl_phys.Histo1D({"hrec_xbj",";x_{bj};N_{evts} / 0.04 rad",nbins_xsec,-1,1},"rec_xbj");
  
  //phi_h (trento)
  auto htru_TrentoPhi = df.Histo1D({"htru_TrentoPhi",";#phi_{h} [rad];N_{evts} / 0.13 rad",nbins_xsec,-M_PI,M_PI},"tru_TrentoPhi");
  auto htru_TrentoPhi_acc = df_excl_phys.Histo1D({"htru_TrentoPhi_acc",";#phi_{h} [rad];N_{evts} / 0.13 rad",nbins_xsec,-M_PI,M_PI},"tru_TrentoPhi");
  auto hrec_TrentoPhi = df_excl_phys.Histo1D({"hrec_TrentoPhi",";#phi_{h} [rad];N_{evts} / 0.13 rad",nbins_xsec,-M_PI,M_PI},"rec_TrentoPhi");
  
  
  auto nev = df.Count();
  auto nev_scatele = df_scatele.Count();
  auto nev_gprime = df_gprime.Count();
  auto nev_pprime = df_pprime.Count();
  auto nev_semi_excl = df_semi_excl.Count();
  auto nev_excl = df_excl.Count();
  auto nev_semi_excl_phys = df_semi_excl_phys.Count();
  auto nev_excl_phys = df_excl_phys.Count();
  
  
  //trigger
  auto Ngen = *nev;
  
  //output file, put histos and canvases
  TFile outfile("TCS_TDR_histograms.root","RECREATE");
  outfile.cd();
  
  ////////////////////////////
  //Draw
  ////////////////////////////
  
  //truth and rec 1D Q2
  new TCanvas("hQ2");
  gPad->SetLogy();
  htru_Q2->SetMinimum(1);
  htru_Q2->SetMaximum(1.5*htru_Q2->GetMaximum());
  htru_Q2->GetYaxis()->SetTitle("N_{evts} / 5 GeV^{2}");
  DrawOneSplitTwo(htru_Q2.GetPtr(),hrec_Q2_scatele.GetPtr());
  htru_Q2->SetMarkerStyle(8);
  auto leg_Q2_scatele = new TLegend(0.65,0.66,0.85,0.76);
  leg_Q2_scatele->AddEntry(htru_Q2.GetPtr(),"MC Data","lp");
  leg_Q2_scatele->AddEntry(hrec_Q2_scatele.GetPtr(),"Reconstructed","f");
  leg_Q2_scatele->Draw();
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawSimText();
  gPad->Print("hQ2.pdf");
  gPad->Print("hQ2.C");
  gPad->Write();
  gPad->Close();
  
  new TCanvas("hlogQ2");
  gPad->SetLogy();
  htru_logQ2->SetMinimum(1);
  htru_logQ2->SetMaximum(10*htru_logQ2->GetMaximum());
  htru_logQ2->GetYaxis()->SetTitle("N_{evts} / 5 GeV^{2}");
  DrawOneSplitTwo(htru_logQ2.GetPtr(),hrec_logQ2_scatele.GetPtr());
  htru_logQ2->SetMarkerStyle(8);
  auto leg_logQ2 = new TLegend(0.65,0.66,0.85,0.76);
  leg_logQ2->AddEntry(htru_logQ2.GetPtr(),"MC Data","lp");
  leg_logQ2->AddEntry(hrec_logQ2_scatele.GetPtr(),"Reconstructed","f");
  leg_logQ2->Draw();
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawSimText();
  gPad->Print("hlogQ2.pdf");
  gPad->Print("hlogQ2.C");
  gPad->Write();
  gPad->Close();
  
  //single particle acceptances
  ///acceptances 1D
  new TCanvas("hacc_Q2_scatele");
  auto hacc_Q2_scatele = AcceptancePlot(htru_Q2.GetPtr(), hrec_Q2_scatele.GetPtr(), "hacc_Q2");
  hacc_Q2_scatele->SetMarkerStyle(0);
  //DrawOneSplitTwo(hacc_Q2, hacc_Q2, hacc_Q2_barrel);
  hacc_Q2_scatele->DrawCopy();
  //auto leg_q = new TLegend(0.25,0.7,0.45,0.8);
  // auto leg_q = new TLegend(0.6,0.6,0.8,0.7);
  // leg_q->AddEntry(hacc_Q2,"Tagger","f");
  // leg_q->AddEntry(hacc_Q2_barrel,"Barrel","f");
  // leg_q->Draw();
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawSimText();
  hacc_Q2_scatele->DrawCopy();
  gPad->Print("hacc_Q2_scatele.pdf");
  gPad->Print("hacc_Q2_scatele.C");
  gPad->Write();
  gPad->Close();
  
  new TCanvas("hacc_logQ2");
  auto hacc_logQ2 = AcceptancePlot(htru_logQ2.GetPtr(), hrec_logQ2_scatele.GetPtr(), "hacc_logQ2");
  hacc_logQ2->SetMarkerStyle(0);
  //DrawOneSplitTwo(hacc_logQ2, hacc_logQ2, hacc_logQ2_barrel);
  hacc_logQ2->DrawCopy();
  auto leg_logq = new TLegend(0.6,0.6,0.8,0.7);
  //auto leg_logq = new TLegend(0.25,0.7,0.45,0.8);
  // // leg_logq->AddEntry(hacc_logQ2,"Tagger","f");
  // // leg_logq->AddEntry(hacc_logQ2_barrel,"Barrel","f");
  // //hacc_logQ2->DrawCopy();
  //leg_logq->Draw();
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawSimText();
  gPad->Print("hacc_logQ2.pdf");
  gPad->Print("hacc_logQ2.C");
  gPad->Write();
  gPad->Close();
  
  new TCanvas("hacc_W");
  auto hacc_W = AcceptancePlot(htru_W.GetPtr(), hrec_W_scatele.GetPtr(), "hacc_W");
  hacc_W->GetYaxis()->SetTitle("Acceptance");
  hacc_W->DrawCopy();
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawSimText();
  gPad->Print("hacc_W.pdf");
  gPad->Print("hacc_W.C");
  gPad->Write();
  gPad->Close();
  
  new TCanvas("hacc_pmag_scatele");
  auto hacc_pmag_scatele = AcceptancePlot(htru_pmag_scatele.GetPtr(), hrec_pmag_scatele.GetPtr(), "hacc_pmag_scatele");
  hacc_pmag_scatele->GetYaxis()->SetTitle("Acceptance");
  hacc_pmag_scatele->DrawCopy();
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawSimText();
  gPad->Print("hacc_pmag_scatele.pdf");
  gPad->Print("hacc_pmag_scatele.C");
  gPad->Write();
  gPad->Close();
  
  new TCanvas("hacc_theta_scatele");
  auto hacc_theta_scatele = AcceptancePlot(htru_theta_scatele.GetPtr(), hrec_theta_scatele.GetPtr(), "hacc_theta_scatele");
  hacc_theta_scatele->GetYaxis()->SetTitle("Acceptance");
  hacc_theta_scatele->DrawCopy();
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawSimText();
  gPad->Print("hacc_theta_scatele.pdf");
  gPad->Print("hacc_theta_scatele.C");
  gPad->Write();
  gPad->Close();
  
  new TCanvas("hacc_phi_scatele");
  auto hacc_phi_scatele = AcceptancePlot(htru_phi_scatele.GetPtr(), hrec_phi_scatele.GetPtr(), "hacc_phi_scatele");
  hacc_phi_scatele->GetYaxis()->SetTitle("Acceptance");
  hacc_phi_scatele->DrawCopy();
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawSimText();
  gPad->Print("hacc_phi_scatele.pdf");
  gPad->Print("hacc_phi_scatele.C");
  gPad->Write();
  gPad->Close();
  
  //proton
  new TCanvas("hacc_tbot_pprime");
  auto hacc_tbot_pprime = AcceptancePlot(htru_tbot.GetPtr(), hrec_tbot.GetPtr(), "hacc_tbot_pprime");
  hacc_tbot_pprime->SetMaximum(1.3);
  hacc_tbot_pprime->GetYaxis()->SetTitle("Acceptance");
  hacc_tbot_pprime->DrawCopy();
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawSimText();
  gPad->Print("hacc_tbot_pprime.pdf");
  gPad->Print("hacc_tbot_pprime.C");
  gPad->Write();
  gPad->Close();
  
  new TCanvas("hacc_pmag_pprime");
  auto hacc_pmag_pprime = AcceptancePlot(htru_pmag_pprime.GetPtr(), hrec_pmag_pprime.GetPtr(), "hacc_pmag_pprime");
  hacc_pmag_pprime->GetYaxis()->SetTitle("Acceptance");
  hacc_pmag_pprime->DrawCopy();
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawSimText();
  gPad->Print("hacc_pmag_pprime.pdf");
  gPad->Print("hacc_pmag_pprime.C");
  gPad->Write();
  gPad->Close();
  
  new TCanvas("hacc_theta_pprime");
  auto hacc_theta_pprime = AcceptancePlot(htru_theta_pprime.GetPtr(), hrec_theta_pprime.GetPtr(), "hacc_theta_pprime");
  hacc_theta_pprime->SetMaximum(1.2);
  hacc_theta_pprime->GetYaxis()->SetTitle("Acceptance");
  hacc_theta_pprime->DrawCopy();
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawSimText();
  gPad->Print("hacc_theta_pprime.pdf");
  gPad->Print("hacc_theta_pprime.C");
  gPad->Write();
  gPad->Close();
  
  new TCanvas("hacc_phi_pprime");
  auto hacc_phi_pprime = AcceptancePlot(htru_phi_pprime.GetPtr(), hrec_phi_pprime.GetPtr(), "hacc_phi_pprime");
  hacc_phi_pprime->GetYaxis()->SetTitle("Acceptance");
  hacc_phi_pprime->DrawCopy();
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawSimText();
  gPad->Print("hacc_phi_pprime.pdf");
  gPad->Print("hacc_phi_pprime.C");
  gPad->Write();
  gPad->Close();
  
  new TCanvas("hacc_pmag_gprime");
  auto hacc_pmag_gprime = AcceptancePlot(htru_pmag_gprime.GetPtr(), hrec_pmag_gprime.GetPtr(), "hacc_pmag_gprime");
  hacc_pmag_gprime->SetMaximum(1.5);
  hacc_pmag_gprime->GetYaxis()->SetTitle("Acceptance");
  hacc_pmag_gprime->DrawCopy();
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawSimText();
  gPad->Print("hacc_pmag_gprime.pdf");
  gPad->Print("hacc_pmag_gprime.C");
  gPad->Write();
  gPad->Close();
  
  new TCanvas("hacc_theta_gprime");
  auto hacc_theta_gprime = AcceptancePlot(htru_theta_gprime.GetPtr(), hrec_theta_gprime.GetPtr(), "hacc_theta_gprime");
  hacc_theta_gprime->GetYaxis()->SetTitle("Acceptance");
  hacc_theta_gprime->DrawCopy();
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawSimText();
  gPad->Print("hacc_theta_gprime.pdf");
  gPad->Print("hacc_theta_gprime.C");
  gPad->Write();
  gPad->Close();
  
  new TCanvas("hacc_eta_gprime");
  auto hacc_eta_gprime = AcceptancePlot(htru_eta_gprime.GetPtr(), hrec_eta_gprime.GetPtr(), "hacc_eta_gprime");
  hacc_eta_gprime->GetYaxis()->SetTitle("Acceptance");
  hacc_eta_gprime->DrawCopy();
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawSimText();
  gPad->Print("hacc_eta_gprime.pdf");
  gPad->Print("hacc_eta_gprime.C");
  gPad->Write();
  gPad->Close();
  
  new TCanvas("hacc_phi_gprime");
  auto hacc_phi_gprime = AcceptancePlot(htru_phi_gprime.GetPtr(), hrec_phi_gprime.GetPtr(), "hacc_phi_gprime");
  hacc_phi_gprime->GetYaxis()->SetTitle("Acceptance");
  hacc_phi_gprime->DrawCopy();
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawSimText();
  gPad->Print("hacc_phi_gprime.pdf");
  gPad->Print("hacc_phi_gprime.C");
  gPad->Write();
  gPad->Close();
  


  ///resolutions
  auto fres = new TF1("fres","gaus");
  auto fdelta = new TF1("fdelta","gaus");
  auto fitopts = "QN";
  
  new TCanvas("hres_pmag_scatele");
  hres_pmag_scatele->Fit(fres,fitopts,"",-0.01,0.01);
  auto res_pmag_scatele_sig = fres->GetParameter(2);
  hres_pmag_scatele->DrawCopy();
  tex.DrawLatex(.6,.6,Form("#sigma_{#Deltap/p} = %1.2f %%",100*res_pmag_scatele_sig));
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawSimText();
  gPad->Print("hres_pmag_scatele.pdf");
  gPad->Print("hres_pmag_scatele.C");
  gPad->Write();
  gPad->Close();
    
  new TCanvas("hres_Q2");
  hres_Q2->Fit(fres,fitopts,"",-0.0002,0.0002);
  auto res_Q2_sig = 100000*fres->GetParameter(2);
  hres_Q2->DrawCopy();
  tex.DrawLatex(.6,.6,Form("#sigma_{#DeltaQ^{2}} = %1.0fe-5 GeV^{2}",res_Q2_sig));
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawSimText();
  gPad->Print("hres_Q2.pdf");
  gPad->Print("hres_Q2.C");
  gPad->Write();
  gPad->Close();
  
  new TCanvas("hres_theta_scatele");
  hres_theta_scatele->Fit(fres,fitopts,"",-1,1);
  auto res_theta_scatele_sig = fres->GetParameter(2);
  hres_theta_scatele->DrawCopy();
  tex.DrawLatex(.6,.6,Form("#sigma_{#Delta#theta} = %1.2f mrad",res_theta_scatele_sig));
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawSimText();
  gPad->Print("hres_theta_scatele.pdf");
  gPad->Print("hres_theta_scatele.C");
  gPad->Write();
  gPad->Close();
  
  new TCanvas("hres_phi_scatele");
  hres_phi_scatele->Fit(fres,fitopts,"",-0.3,0.3);
  auto res_phi_scatele_sig = fres->GetParameter(2);
  hres_phi_scatele->DrawCopy();
  tex.DrawLatex(.6,.6,Form("#sigma_{#Delta#phi} = %1.2f rad",res_phi_scatele_sig));
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawSimText();
  gPad->Print("hres_phi_scatele.pdf");
  gPad->Print("hres_phi_scatele.C");
  gPad->Write();
  gPad->Close();
  
  
  // //calculated escat resolutions
  // new TCanvas("hres_pmag_calc_scatele");
  // hres_pmag_calc_scatele->Fit(fres,fitopts,"",-0.4,0.4);
  // auto res_pmag_calc_scatele_sig = fres->GetParameter(2);
  // hres_pmag_calc_scatele->DrawCopy();
  // tex.DrawLatex(.6,.6,Form("#sigma_{#Deltap/p} = %1.2f %%",100*res_pmag_calc_scatele_sig));
  // DrawEpicText();
  // DrawEpicEnergyText(Eset);
  // DrawSimText();
  // gPad->Print("hres_pmag_calc_scatele.pdf");
  // gPad->Print("hres_pmag_calc_scatele.C");
  // gPad->Write();
  // gPad->Close();
    
  // new TCanvas("hres_calc_Q2");
  // hres_calc_Q2->Fit(fres,fitopts,"",-10,10);
  // auto res_calc_Q2_sig = fres->GetParameter(2);
  // hres_calc_Q2->DrawCopy();
  // tex.DrawLatex(.6,.6,Form("#sigma_{#DeltaQ^{2}} = %1.0f GeV^{2}",res_calc_Q2_sig));
  // DrawEpicText();
  // DrawEpicEnergyText(Eset);
  // DrawSimText();
  // gPad->Print("hres_calc_Q2.pdf");
  // gPad->Print("hres_calc_Q2.C");
  // gPad->Write();
  // gPad->Close();
  
  // new TCanvas("hres_theta_calc_scatele");
  // hres_theta_calc_scatele->Fit(fres,fitopts,"",-5,5);
  // auto res_theta_calc_scatele_sig = fres->GetParameter(2);
  // hres_theta_calc_scatele->DrawCopy();
  // tex.DrawLatex(.6,.6,Form("#sigma_{#Delta#theta} = %1.2f mrad",res_theta_calc_scatele_sig));
  // DrawEpicText();
  // DrawEpicEnergyText(Eset);
  // DrawSimText();
  // gPad->Print("hres_theta_calc_scatele.pdf");
  // gPad->Print("hres_theta_calc_scatele.C");
  // gPad->Write();
  // gPad->Close();
  
  // new TCanvas("hres_phi_calc_scatele");
  // hres_phi_calc_scatele->Fit(fres,fitopts,"",-2,2);
  // auto res_phi_calc_scatele_sig = fres->GetParameter(2);
  // hres_phi_calc_scatele->DrawCopy();
  // tex.DrawLatex(.6,.6,Form("#sigma_{#Delta#phi} = %1.2f rad",res_phi_calc_scatele_sig));
  // DrawEpicText();
  // DrawEpicEnergyText(Eset);
  // DrawSimText();
  // gPad->Print("hres_phi_calc_scatele.pdf");
  // gPad->Print("hres_phi_calc_scatele.C");
  // gPad->Write();
  // gPad->Close();
  
  
  ///physics resolutions
  //w,t,xbj,phi_h
  new TCanvas("hres_W");
  hres_W->Fit(fres,fitopts,"",-0.5,0.5);
  auto res_W_sig = fres->GetParameter(2);
  hres_W->DrawCopy();
  tex.DrawLatex(.6,.6,Form("#sigma_{#DeltaW} = %1.2f GeV",res_W_sig));
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawSimText();
  gPad->Print("hres_W.pdf");
  gPad->Print("hres_W.C");
  gPad->Write();
  gPad->Close();
  	      
  new TCanvas("hres_Whad");
  hres_Whad->Fit(fres,fitopts,"",-0.5,0.5);
  auto res_Whad_sig = fres->GetParameter(2);
  hres_Whad->DrawCopy();
  tex.DrawLatex(.6,.6,Form("#sigma_{#DeltaWhad} = %1.2f GeV",res_Whad_sig));
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawSimText();
  gPad->Print("hres_Whad.pdf");
  gPad->Print("hres_Whad.C");
  gPad->Write();
  gPad->Close();
  
  new TCanvas("hres_ttop");
  hres_ttop->Fit(fres,fitopts,"",-0.1,0.1);
  auto res_ttop_sig = fres->GetParameter(2);
  hres_ttop->DrawCopy();
  tex.DrawLatex(.6,.6,Form("#sigma_{#Delta|t_{e#gamma}|} = %1.2f GeV^{2}",res_ttop_sig));
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawSimText();
  gPad->Print("hres_ttop.pdf");
  gPad->Print("hres_ttop.C");
  gPad->Write();
  gPad->Close();
  
  new TCanvas("hres_tbot");
  hres_tbot->Fit(fres,fitopts,"",-0.1,0.1);
  auto res_tbot_sig = fres->GetParameter(2);
  hres_tbot->DrawCopy();
  tex.DrawLatex(.6,.6,Form("#sigma_{#Delta|t_{pp'}|} = %1.2f GeV^{2}",res_tbot_sig));
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawSimText();
  gPad->Print("hres_tbot.pdf");
  gPad->Print("hres_tbot.C");
  gPad->Write();
  gPad->Close();
  
  
  new TCanvas("hres_TrentoPhi_semi_excl");
  hres_TrentoPhi_semi_excl->Fit(fres,fitopts,"",-0.1,0.1);
  auto res_TrentoPhi_semi_excl_sig = fres->GetParameter(2);
  hres_TrentoPhi_semi_excl->DrawCopy();
  tex.DrawLatex(.6,.6,Form("#sigma_{#Delta#phi_{l}} = %1.2f rad",res_TrentoPhi_semi_excl_sig));
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawSimText();
  gPad->Print("hres_TrentoPhi_semi_excl.pdf");
  gPad->Print("hres_TrentoPhi_semi_excl.C");
  gPad->Write();
  gPad->Close();
  
  new TCanvas("hres_TrentoPhi_excl");
  hres_TrentoPhi_excl->Fit(fres,fitopts,"",-0.1,0.1);
  auto res_TrentoPhi_excl_sig = fres->GetParameter(2);
  hres_TrentoPhi_excl->DrawCopy();
  tex.DrawLatex(.6,.6,Form("#sigma_{#Delta#phi_{l}} = %1.2f rad",res_TrentoPhi_excl_sig));
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawSimText();
  gPad->Print("hres_TrentoPhi_excl.pdf");
  gPad->Print("hres_TrentoPhi_excl.C");
  gPad->Write();
  gPad->Close();
  
  new TCanvas("h2d_GammaPol");
  gPad->SetLogz();
  h2d_GammaPol->DrawCopy("colz");
  gPad->Print("h2d_GammaPol.pdf");
  gPad->Print("h2d_GammaPol.C");
  gPad->Write();
  gPad->Close();
  
  new TCanvas("h2d_calcGammaPol");
  gPad->SetLogz();
  h2d_calcGammaPol->DrawCopy("colz");
  gPad->Print("h2d_calcGammaPol.pdf");
  gPad->Print("h2d_calcGammaPol.C");
  gPad->Write();
  gPad->Close();
  
  new TCanvas("hdelta_pmag_pprime");
  hdelta_pmag_pprime->Fit(fdelta,fitopts,"",-5,5);
  auto delta_pmag_pprime_sig = fdelta->GetParameter(2);
  hdelta_pmag_pprime->DrawCopy();
  tex.DrawLatex(.6,.6,Form("#sigma_{#Deltap} = %1.2f GeV/c",delta_pmag_pprime_sig));
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawSimText();
  gPad->Print("hdelta_pmag_pprime.pdf");
  gPad->Print("hdelta_pmag_pprime.C");
  gPad->Write();
  gPad->Close();
  
  new TCanvas("hdelta_theta_pprime");
  hdelta_theta_pprime->Fit(fdelta,fitopts,"",-1,1);
  auto delta_theta_pprime_sig = fdelta->GetParameter(2);
  hdelta_theta_pprime->DrawCopy();
  tex.DrawLatex(.6,.6,Form("#sigma_{#Delta#theta} = %1.2f mrad",delta_theta_pprime_sig));
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawSimText();
  gPad->Print("hdelta_theta_pprime.pdf");
  gPad->Print("hdelta_theta_pprime.C");
  gPad->Write();
  gPad->Close();
  
  new TCanvas("hdelta_phi_pprime");
  hdelta_phi_pprime->Fit(fdelta,fitopts,"",-1,1);
  auto delta_phi_pprime_sig = fdelta->GetParameter(2);
  hdelta_phi_pprime->DrawCopy();
  tex.DrawLatex(.6,.6,Form("#sigma_{#Delta#phi} = %1.2f rad",delta_phi_pprime_sig));
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawSimText();
  gPad->Print("hdelta_phi_pprime.pdf");
  gPad->Print("hdelta_phi_pprime.C");
  gPad->Write();
  gPad->Close();
  
  
  new TCanvas("hdNdt");
  gPad->SetLogy();
  auto hacc_tbot = AcceptancePlot(htru_tbot.GetPtr(),hrec_tbot_phys.GetPtr(),"hacc_tbot");
  auto htru_dNdt = XSecPlot(htru_tbot.GetPtr(),xsec*lumi_nb/Ngen,"htru_dNdt");
  auto htru_dNdt_acc = XSecPlot(htru_tbot_acc.GetPtr(),xsec*lumi_nb/Ngen,"htru_dNdt_acc");
  auto hrec_dNdt = XSecPlot(hrec_tbot_phys.GetPtr(),xsec*lumi_nb/Ngen,"hrec_dNdt");
  auto hrec_dNdt_corr = XSecPlot(hrec_tbot_phys.GetPtr(),xsec*lumi_nb/Ngen,"htru_dNdt_corr", hacc_tbot);
  
  htru_dNdt->SetMarkerStyle(0);
  htru_dNdt->GetYaxis()->SetTitle("dN/dt [nb GeV^{-2}]");
  htru_dNdt->SetMinimum(1e1);
  htru_dNdt->SetMaximum(1e11);
  htru_dNdt->DrawCopy("hist");
  
  htru_dNdt_acc->SetMarkerStyle(25);
  htru_dNdt_acc->SetMarkerColor(kp6[1]);
  htru_dNdt_acc->SetLineColor(kp6[1]);
  htru_dNdt_acc->DrawCopy("same pe");

  hrec_dNdt->SetMarkerStyle(4);
  hrec_dNdt->SetMarkerColor(kp6[0]);
  hrec_dNdt->SetLineColor(kp6[0]);
  hrec_dNdt->DrawCopy("same pe");
  
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawLumiText(5);
  DrawSimText();
  TLegend *leg_tbot = new TLegend(0.6,0.55,0.8,0.7);
  leg_tbot->AddEntry(htru_dNdt,"MC","l");
  leg_tbot->AddEntry(htru_dNdt_acc,"MC Accepted","P");
  leg_tbot->AddEntry(hrec_dNdt,"RECO","P");
  leg_tbot->Draw();
  gPad->Update();
  gPad->Print("hdNdt.pdf");
  gPad->Print("hdNdt.C");
  gPad->Write();
  gPad->Close();
  
  new TCanvas("hdNdt_corr");
  gPad->SetLogy();
  hrec_dNdt_corr->SetMarkerStyle(4);
  hrec_dNdt_corr->SetMarkerColor(kp6[2]);
  hrec_dNdt_corr->SetLineColor(kp6[2]);
  htru_dNdt->DrawCopy("hist");
  hrec_dNdt_corr->DrawCopy("same p");
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawLumiText(5);
  DrawSimText();
  TLegend *leg_tcorr = new TLegend(0.6,0.6,0.8,0.7);
  leg_tcorr->AddEntry(htru_dNdt,"MC","l");
  leg_tcorr->AddEntry(hrec_dNdt_corr,"RECO Corrected","P");
  leg_tcorr->Draw();
  gPad->Update();
  gPad->Print("hdNdt_corr.pdf");
  gPad->Print("hdNdt_corr.C");
  gPad->Write();
  gPad->Close();
  
  //semi-exclusive t
  new TCanvas("hdNdttoptop");
  gPad->SetLogy();
  auto hacc_ttop = AcceptancePlot(htru_ttop.GetPtr(),hrec_ttop_phys.GetPtr(),"hacc_ttop");
  auto htru_dNdttop = XSecPlot(htru_ttop.GetPtr(),xsec*lumi_nb/Ngen,"htru_dNdttop");
  auto htru_dNdttop_acc = XSecPlot(htru_ttop_acc.GetPtr(),xsec*lumi_nb/Ngen,"htru_dNdttop_acc");
  auto hrec_dNdttop = XSecPlot(hrec_ttop_phys.GetPtr(),xsec*lumi_nb/Ngen,"hrec_dNdttop");
  auto hrec_dNdttop_corr = XSecPlot(hrec_ttop_phys.GetPtr(),xsec*lumi_nb/Ngen,"htru_dNdttop_corr", hacc_ttop);
  
  htru_dNdttop->SetMarkerStyle(0);
  htru_dNdttop->GetYaxis()->SetTitle("dN/dttop [nb GeV^{-2}]");
  htru_dNdttop->SetMinimum(1e1);
  htru_dNdttop->SetMaximum(1e11);
  htru_dNdttop->DrawCopy("hist");
  
  htru_dNdttop_acc->SetMarkerStyle(25);
  htru_dNdttop_acc->SetMarkerColor(kp6[1]);
  htru_dNdttop_acc->SetLineColor(kp6[1]);
  htru_dNdttop_acc->DrawCopy("same pe");

  hrec_dNdttop->SetMarkerStyle(4);
  hrec_dNdttop->SetMarkerColor(kp6[0]);
  hrec_dNdttop->SetLineColor(kp6[0]);
  hrec_dNdttop->DrawCopy("same pe");
  
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawLumiText(5);
  DrawSimText();
  TLegend *leg_ttop = new TLegend(0.6,0.55,0.8,0.7);
  leg_ttop->AddEntry(htru_dNdttop,"MC","l");
  leg_ttop->AddEntry(htru_dNdttop_acc,"MC Accepted","P");
  leg_ttop->AddEntry(hrec_dNdttop,"RECO","P");
  leg_ttop->Draw();
  gPad->Update();
  gPad->Print("hdNdttop.pdf");
  gPad->Print("hdNdttop.C");
  gPad->Write();
  gPad->Close();
  
  new TCanvas("hdNdttop_corr");
  gPad->SetLogy();
  hrec_dNdttop_corr->SetMarkerStyle(4);
  hrec_dNdttop_corr->SetMarkerColor(kp6[2]);
  hrec_dNdttop_corr->SetLineColor(kp6[2]);
  htru_dNdttop->DrawCopy("hist");
  hrec_dNdttop_corr->DrawCopy("same p");
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawLumiText(5);
  DrawSimText();
  TLegend *leg_ttopcorr = new TLegend(0.6,0.6,0.8,0.7);
  leg_ttopcorr->AddEntry(htru_dNdttop,"MC","l");
  leg_ttopcorr->AddEntry(hrec_dNdttop_corr,"RECO Corrected","P");
  leg_ttopcorr->Draw();
  gPad->Update();
  gPad->Print("hdNdttop_corr.pdf");
  gPad->Print("hdNdttop_corr.C");
  gPad->Write();
  gPad->Close();
  

  //Q2
  new TCanvas("hdNdQ2");
  gPad->SetLogy();
  auto hacc_Q2 = AcceptancePlot(htru_Q2.GetPtr(),hrec_Q2.GetPtr(),"hacc_Q2");
  auto htru_dNdQ2 = XSecPlot(htru_Q2.GetPtr(),xsec*lumi_nb/Ngen,"htru_dNdQ2");
  auto htru_dNdQ2_acc = XSecPlot(htru_Q2_acc.GetPtr(),xsec*lumi_nb/Ngen,"htru_dNdQ2_acc");
  auto hrec_dNdQ2 = XSecPlot(hrec_Q2.GetPtr(),xsec*lumi_nb/Ngen,"hrec_dNdQ2");
  auto hrec_dNdQ2_corr = XSecPlot(hrec_Q2.GetPtr(),xsec*lumi_nb/Ngen,"htru_dNdQ2_corr", hacc_Q2);
  
  htru_dNdQ2->SetMarkerStyle(0);
  htru_dNdQ2->GetYaxis()->SetTitle("dN/dQ'^{2} [nb GeV^{-2}]");
  htru_dNdQ2->SetMinimum(1e1);
  htru_dNdQ2->SetMaximum(1e11);
  htru_dNdQ2->DrawCopy("hist");
  
  htru_dNdQ2_acc->SetMarkerStyle(25);
  htru_dNdQ2_acc->SetMarkerColor(kp6[1]);
  htru_dNdQ2_acc->SetLineColor(kp6[1]);
  htru_dNdQ2_acc->DrawCopy("same pe");

  hrec_dNdQ2->SetMarkerStyle(4);
  hrec_dNdQ2->SetMarkerColor(kp6[0]);
  hrec_dNdQ2->SetLineColor(kp6[0]);
  hrec_dNdQ2->DrawCopy("same pe");
  
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawLumiText(5);
  DrawSimText();
  TLegend *leg_Q2 = new TLegend(0.6,0.55,0.8,0.7);
  leg_Q2->AddEntry(htru_dNdQ2,"MC","l");
  leg_Q2->AddEntry(htru_dNdQ2_acc,"MC Accepted","P");
  leg_Q2->AddEntry(hrec_dNdQ2,"RECO","P");
  leg_Q2->Draw();
  gPad->Update();
  gPad->Print("hdNQ2.pdf");
  gPad->Print("hdNQ2.C");
  gPad->Write();
  gPad->Close();
  
  //xbj
  new TCanvas("hdNdxbj");
  gPad->SetLogy();
  auto hacc_xbj = AcceptancePlot(htru_xbj.GetPtr(),hrec_xbj.GetPtr(),"hacc_xbj");
  auto htru_dNdxbj = XSecPlot(htru_xbj.GetPtr(),xsec*lumi_nb/Ngen,"htru_dNdxbj");
  auto htru_dNdxbj_acc = XSecPlot(htru_xbj_acc.GetPtr(),xsec*lumi_nb/Ngen,"htru_dNdxbj_acc");
  auto hrec_dNdxbj = XSecPlot(hrec_xbj.GetPtr(),xsec*lumi_nb/Ngen,"hrec_dNdxbj");
  auto hrec_dNdxbj_corr = XSecPlot(hrec_xbj.GetPtr(),xsec*lumi_nb/Ngen,"htru_dNdxbj_corr", hacc_xbj);
  
  htru_dNdxbj->SetMarkerStyle(0);
  htru_dNdxbj->GetYaxis()->SetTitle("dN/dcos(#theta_{l}) [nb]");
  htru_dNdxbj->SetMinimum(1e2);
  htru_dNdxbj->SetMaximum(1e13);
  htru_dNdxbj->DrawCopy("hist");
  
  htru_dNdxbj_acc->SetMarkerStyle(25);
  htru_dNdxbj_acc->SetMarkerColor(kp6[1]);
  htru_dNdxbj_acc->SetLineColor(kp6[1]);
  htru_dNdxbj_acc->DrawCopy("same pe");

  hrec_dNdxbj->SetMarkerStyle(4);
  hrec_dNdxbj->SetMarkerColor(kp6[0]);
  hrec_dNdxbj->SetLineColor(kp6[0]);
  hrec_dNdxbj->DrawCopy("same pe");
  
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawLumiText(5);
  DrawSimText();
  TLegend *leg_xbj = new TLegend(0.6,0.55,0.8,0.7);
  leg_xbj->AddEntry(htru_dNdxbj,"MC","l");
  leg_xbj->AddEntry(htru_dNdxbj_acc,"MC Accepted","P");
  leg_xbj->AddEntry(hrec_dNdxbj,"RECO","P");
  leg_xbj->Draw();
  gPad->Update();
  gPad->Print("hdNxbj.pdf");
  gPad->Print("hdNxbj.C");
  gPad->Write();
  gPad->Close();
  
  //TrentoPhi
  new TCanvas("hdNdTrentoPhi");
  gPad->SetLogy();
  auto hacc_TrentoPhi = AcceptancePlot(htru_TrentoPhi.GetPtr(),hrec_TrentoPhi.GetPtr(),"hacc_TrentoPhi");
  auto htru_dNdTrentoPhi = XSecPlot(htru_TrentoPhi.GetPtr(),xsec*lumi_nb/Ngen,"htru_dNdTrentoPhi");
  auto htru_dNdTrentoPhi_acc = XSecPlot(htru_TrentoPhi_acc.GetPtr(),xsec*lumi_nb/Ngen,"htru_dNdTrentoPhi_acc");
  auto hrec_dNdTrentoPhi = XSecPlot(hrec_TrentoPhi.GetPtr(),xsec*lumi_nb/Ngen,"hrec_dNdTrentoPhi");
  auto hrec_dNdTrentoPhi_corr = XSecPlot(hrec_TrentoPhi.GetPtr(),xsec*lumi_nb/Ngen,"htru_dNdTrentoPhi_corr", hacc_TrentoPhi);
  
  htru_dNdTrentoPhi->SetMarkerStyle(0);
  htru_dNdTrentoPhi->GetYaxis()->SetTitle("dN/d#phi_{l} [nb rad^{-1}]");
  htru_dNdTrentoPhi->SetMinimum(1e2);
  htru_dNdTrentoPhi->SetMaximum(1e13);
  htru_dNdTrentoPhi->DrawCopy("hist");
  
  htru_dNdTrentoPhi_acc->SetMarkerStyle(25);
  htru_dNdTrentoPhi_acc->SetMarkerColor(kp6[1]);
  htru_dNdTrentoPhi_acc->SetLineColor(kp6[1]);
  htru_dNdTrentoPhi_acc->DrawCopy("same pe");

  hrec_dNdTrentoPhi->SetMarkerStyle(4);
  hrec_dNdTrentoPhi->SetMarkerColor(kp6[0]);
  hrec_dNdTrentoPhi->SetLineColor(kp6[0]);
  hrec_dNdTrentoPhi->DrawCopy("same pe");
  
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawLumiText(5);
  DrawSimText();
  TLegend *leg_TrentoPhi = new TLegend(0.6,0.55,0.8,0.7);
  leg_TrentoPhi->AddEntry(htru_dNdTrentoPhi,"MC","l");
  leg_TrentoPhi->AddEntry(htru_dNdTrentoPhi_acc,"MC Accepted","P");
  leg_TrentoPhi->AddEntry(hrec_dNdTrentoPhi,"RECO","P");
  leg_TrentoPhi->Draw();
  gPad->Update();
  gPad->Print("hdNTrentoPhi.pdf");
  gPad->Print("hdNTrentoPhi.C");
  gPad->Write();
  gPad->Close();
  
  //Asymmtery in phi_h with uncertainties - later
  
  
  auto dNdt_integral = hrec_dNdt->Integral(1,100);
  //auto nev_proj = (*nev_final) * xsec * lumi_nb / Ngen;
  auto scatele_acc = (double) *nev_scatele / *nev;
  auto gprime_acc = (double) *nev_gprime / *nev;
  auto pprime_acc = (double) *nev_pprime / *nev;
  auto seminlc_acc = (double) *nev_semi_excl / *nev;
  auto excl_acc = (double) *nev_excl / *nev;
  auto semi_excl_phys_acc = (double) *nev_semi_excl_phys / *nev;
  auto excl_phys_acc = (double) *nev_excl_phys / *nev;
  
  cout << "Electron Acceptance: " << scatele_acc << endl;
  cout << "Photon Acceptance: " <<  gprime_acc << endl;
  cout << "Proton Acceptance: " <<  pprime_acc << endl;
  cout << "Folded Acceptance: " << scatele_acc*gprime_acc*pprime_acc << endl;
  cout << "Semi-Exclusive (e'#gamma (MX=MP)) Physics Cut Acceptance: " << semi_excl_phys_acc << endl;
  cout << "Exclusive (e' #gamma p') Physics Cut Acceptance: " << excl_phys_acc << endl;
  //cout << "Scaled up by xsec*Lum/Ngen: " << nev_proj << endl;
  
  outfile.Write();
  outfile.Close();
  
  gSystem->Exec("mkdir -p plots");
  gSystem->Exec("mkdir -p macros");
  gSystem->Exec("mv *.pdf ./plots/");
  gSystem->Exec("mv h*.C ./macros/");
}
