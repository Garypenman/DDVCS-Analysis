#include "DrawHistos.C"
void MakeHistos(TString filename="/home/garyp/eic/rad/output/MCMatched_ddvcs_ee_18x275.root"){
  
  TFile *f = TFile::Open(filename);
  TTree *T = (TTree*)f->Get("rad_tree");
  TString outfilename = filename;
  outfilename = gSystem->BaseName(outfilename);
  outfilename.ReplaceAll(".root","_histos.root");
  TFile *fout = new TFile(outfilename,"UPDATE");
  fout->cd();
  TCut escat_recon = "rec_pmag[scat_ele]>0";
  TCut ele_recon = "rec_pmag[ele]>0";
  TCut pos_recon = "rec_pmag[pos]>0";
  TCut gprime_recon = ele_recon && pos_recon;
  TCut pprime_recon = "rec_pmag[pprime]>0";
  
  TCut tagger_theta = "rec_theta[scat_ele]>3.12";
  TCut barrel_theta = !tagger_theta;
  TCut tagger_recon = escat_recon && tagger_theta;
  TCut barrel_recon = escat_recon && barrel_theta;
  
  double tagger_pmag_cut=12.0;
  TParameter<double>("tagger_pmag_cut", tagger_pmag_cut).Write();
  TCut tagger1_recon = tagger_recon && Form("rec_pmag[scat_ele]>%f",tagger_pmag_cut);
  TCut tagger2_recon = tagger_recon && (!tagger1_recon);
  
  //resolutions
  T->Draw("(tru_theta[scat_ele]-rec_theta[scat_ele])>>hres_theta_scat_ele(100,-0.006,0.006)",escat_recon,"goff");
  T->Draw("(tru_phi[scat_ele]-rec_phi[scat_ele])>>hres_phi_scat_ele(100,-3.14,3.14)",escat_recon,"goff");
  T->Draw("(tru_pmag[scat_ele]-rec_pmag[scat_ele])/tru_pmag[scat_ele]>>hres_pmag_scat_ele(100,-0.1,0.1)",escat_recon,"goff");
  T->Draw("(tru_theta[scat_ele]-rec_theta[scat_ele])>>hres_theta_scat_ele_tagger(100,-0.006,0.006)",tagger_recon,"goff");
  T->Draw("(tru_phi[scat_ele]-rec_phi[scat_ele])>>hres_phi_scat_ele_tagger(100,-3.14,3.14)",tagger_recon,"goff");
  T->Draw("(tru_pmag[scat_ele]-rec_pmag[scat_ele])/tru_pmag[scat_ele]>>hres_pmag_scat_ele_tagger(100,-0.1,0.1)",tagger_recon,"goff");
  T->Draw("(tru_theta[scat_ele]-rec_theta[scat_ele])>>hres_theta_scat_ele_barrel(100,-0.006,0.006)",barrel_recon,"goff");
  T->Draw("(tru_phi[scat_ele]-rec_phi[scat_ele])>>hres_phi_scat_ele_barrel(100,-3.14,3.14)",barrel_recon,"goff");
  T->Draw("(tru_pmag[scat_ele]-rec_pmag[scat_ele])/tru_pmag[scat_ele]>>hres_pmag_scat_ele_barrel(100,-0.1,0.1)",barrel_recon,"goff");
  
  cout << "Fitting Tagger Resolutions..." << endl;
  TF1 *fgaus = new TF1("fgaus","gaus");
  auto hres_pmag_scat_ele = (TH1D*)gDirectory->Get("hres_pmag_scat_ele");
  auto hres_theta_scat_ele = (TH1D*)gDirectory->Get("hres_theta_scat_ele");
  auto hres_phi_scat_ele = (TH1D*)gDirectory->Get("hres_phi_scat_ele");
  
  auto hres_pmag_scat_ele_tagger = (TH1D*)gDirectory->Get("hres_pmag_scat_ele_tagger");
  auto hres_theta_scat_ele_tagger = (TH1D*)gDirectory->Get("hres_theta_scat_ele_tagger");
  auto hres_phi_scat_ele_tagger = (TH1D*)gDirectory->Get("hres_phi_scat_ele_tagger");
  
  auto hres_pmag_scat_ele_barrel = (TH1D*)gDirectory->Get("hres_pmag_scat_ele_barrel");
  auto hres_theta_scat_ele_barrel = (TH1D*)gDirectory->Get("hres_theta_scat_ele_barrel");
  auto hres_phi_scat_ele_barrel = (TH1D*)gDirectory->Get("hres_phi_scat_ele_barrel");
  
  hres_pmag_scat_ele->Fit(fgaus,"QN","",-0.01,0.01);
  double escat_pmag_mean = fgaus->GetParameter(1);
  double escat_pmag_sig = fgaus->GetParameter(2);
  hres_theta_scat_ele->Fit(fgaus,"QN","",-0.001,0.001);
  double escat_theta_mean = fgaus->GetParameter(1);
  double escat_theta_sig = fgaus->GetParameter(2);
  hres_phi_scat_ele->Fit(fgaus,"QN","",-0.3,0.3);
  double escat_phi_mean = fgaus->GetParameter(1);
  double escat_phi_sig = fgaus->GetParameter(2);

  TParameter<double>("escat_pmag_mean", escat_pmag_mean).Write();
  TParameter<double>("escat_pmag_sig", escat_pmag_sig).Write();
  TParameter<double>("escat_theta_mean", escat_theta_mean).Write();
  TParameter<double>("escat_theta_sig", escat_theta_sig).Write();
  TParameter<double>("escat_phi_mean", escat_phi_mean).Write();
  TParameter<double>("escat_phi_sig", escat_phi_sig).Write();
  
  hres_pmag_scat_ele_tagger->Fit(fgaus,"QN","",-0.01,0.01);
  double tagger_pmag_mean = fgaus->GetParameter(1);
  double tagger_pmag_sig = fgaus->GetParameter(2);
  hres_theta_scat_ele_tagger->Fit(fgaus,"QN","",-0.001,0.001);
  double tagger_theta_mean = fgaus->GetParameter(1);
  double tagger_theta_sig = fgaus->GetParameter(2);
  hres_phi_scat_ele_tagger->Fit(fgaus,"QN","",-0.3,0.3);
  double tagger_phi_mean = fgaus->GetParameter(1);
  double tagger_phi_sig = fgaus->GetParameter(2);
  
  TParameter<double>("tagger_pmag_mean", tagger_pmag_mean).Write();
  TParameter<double>("tagger_pmag_sig", tagger_pmag_sig).Write();
  TParameter<double>("tagger_theta_mean", tagger_theta_mean).Write();
  TParameter<double>("tagger_theta_sig", tagger_theta_sig).Write();
  TParameter<double>("tagger_phi_mean", tagger_phi_mean).Write();
  TParameter<double>("tagger_phi_sig", tagger_phi_sig).Write();

  hres_pmag_scat_ele_barrel->Fit(fgaus,"QN","",-0.01,0.01);
  double barrel_pmag_mean = fgaus->GetParameter(1);
  double barrel_pmag_sig = fgaus->GetParameter(2);
  hres_theta_scat_ele_barrel->Fit(fgaus,"QN","",-0.001,0.001);
  double barrel_theta_mean = fgaus->GetParameter(1);
  double barrel_theta_sig = fgaus->GetParameter(2);
  hres_phi_scat_ele_barrel->Fit(fgaus,"QN","",-0.3,0.3);
  double barrel_phi_mean = fgaus->GetParameter(1);
  double barrel_phi_sig = fgaus->GetParameter(2);
  
  TParameter<double>("barrel_pmag_mean", barrel_pmag_mean).Write();
  TParameter<double>("barrel_pmag_sig", barrel_pmag_sig).Write();
  TParameter<double>("barrel_theta_mean", barrel_theta_mean).Write();
  TParameter<double>("barrel_theta_sig", barrel_theta_sig).Write();
  TParameter<double>("barrel_phi_mean", barrel_phi_mean).Write();
  TParameter<double>("barrel_phi_sig", barrel_phi_sig).Write();

  double res_nsig = 5.0;
  TParameter<double>("res_nsig", res_nsig).Write();
  
  TCut escat_pmag_res = Form("fabs((tru_pmag[scat_ele] - rec_pmag[scat_ele])/tru_pmag[scat_ele] - %f) < %f * %f",escat_pmag_mean,res_nsig,escat_pmag_sig);
  TCut escat_theta_res = Form("fabs((tru_theta[scat_ele] - rec_theta[scat_ele]) - %f) < %f * %f",escat_theta_mean,res_nsig,escat_theta_sig);
  TCut escat_phi_res = Form("fabs((tru_phi[scat_ele] - rec_phi[scat_ele]) - %f) < %f * %f",escat_phi_mean,res_nsig,escat_phi_sig);
  
  TCut tagger_pmag_res = Form("fabs((tru_pmag[scat_ele] - rec_pmag[scat_ele])/tru_pmag[scat_ele] - %f) < %f * %f",tagger_pmag_mean,res_nsig,tagger_pmag_sig);
  TCut tagger_theta_res = Form("fabs((tru_theta[scat_ele] - rec_theta[scat_ele]) - %f) < %f * %f",tagger_theta_mean,res_nsig,tagger_theta_sig);
  TCut tagger_phi_res = Form("fabs((tru_phi[scat_ele] - rec_phi[scat_ele]) - %f) < %f * %f",tagger_phi_mean,res_nsig,tagger_phi_sig);
  
  TCut barrel_pmag_res = Form("fabs((tru_pmag[scat_ele] - rec_pmag[scat_ele])/tru_pmag[scat_ele] - %f) < %f * %f",barrel_pmag_mean,res_nsig,barrel_pmag_sig);
  TCut barrel_theta_res = Form("fabs((tru_theta[scat_ele] - rec_theta[scat_ele]) - %f) < %f * %f",barrel_theta_mean,res_nsig,barrel_theta_sig);
  TCut barrel_phi_res = Form("fabs((tru_phi[scat_ele] - rec_phi[scat_ele]) - %f) < %f * %f",barrel_phi_mean,res_nsig,barrel_phi_sig);
  
  TCut good_escat = escat_recon && escat_pmag_res && escat_theta_res && escat_phi_res;
  TCut good_tagger = tagger_recon && tagger_pmag_res && tagger_theta_res && tagger_phi_res;
  TCut good_barrel = barrel_recon && barrel_pmag_res && barrel_theta_res && barrel_phi_res;

  TCut good_tagger1 = good_tagger && tagger1_recon;
  TCut good_tagger2 = good_tagger && tagger2_recon;
  
  T->Draw("(tru_theta[scat_ele]-rec_theta[scat_ele])>>hres_theta_scat_ele_tagger1(100,-0.006,0.006)",tagger1_recon,"goff");
  T->Draw("(tru_phi[scat_ele]-rec_phi[scat_ele])>>hres_phi_scat_ele_tagger1(100,-3.14,3.14)",tagger1_recon,"goff");
  T->Draw("(tru_pmag[scat_ele]-rec_pmag[scat_ele])/tru_pmag[scat_ele]>>hres_pmag_scat_ele_tagger1(100,-0.1,0.1)",tagger1_recon,"goff");
  
  T->Draw("(tru_theta[scat_ele]-rec_theta[scat_ele])>>hres_theta_scat_ele_tagger2(100,-0.006,0.006)",tagger2_recon,"goff");
  T->Draw("(tru_phi[scat_ele]-rec_phi[scat_ele])>>hres_phi_scat_ele_tagger2(100,-3.14,3.14)",tagger2_recon,"goff");
  T->Draw("(tru_pmag[scat_ele]-rec_pmag[scat_ele])/tru_pmag[scat_ele]>>hres_pmag_scat_ele_tagger2(100,-0.1,0.1)",tagger2_recon,"goff");
  
  //gprime
  T->Draw("tru_GMass-rec_GMass>>hres_GMass(100,-0.5,0.5)",gprime_recon,"goff");
  auto hres_GMass = (TH1D*)gDirectory->Get("hres_GMass");
  //hres_GMass->Draw();
  hres_GMass->Fit(fgaus,"QN","",-0.05,0.05);
  double gmass_mean = fgaus->GetParameter(1);
  double gmass_sig = fgaus->GetParameter(2);
  TParameter<double>("gmass_mean", gmass_mean).Write();
  TParameter<double>("gmass_sig", gmass_sig).Write();
  TCut good_gprime_recon = Form("fabs(tru_GMass - rec_GMass - %f) < %f * %f",gmass_mean,res_nsig,gmass_sig);
  
  TCut good_gprime = gprime_recon && good_gprime_recon; //more later
  
  //pprime
  TCut good_pprime = pprime_recon; //more later
  
  
  //later base these better around actual cuts
  TCut semi_inclusive = good_escat && good_gprime;
  TCut semi_exclusive = good_escat && good_pprime;
  TCut exclusive = good_escat && good_gprime && good_pprime;
  
  //Generator kinematic 2D
  T->Draw("tru_Q2:tru_t_bot>>h2d_tru_Q2_t_bot(100,0,2,100,0,10)","","goff");
  T->Draw("rec_Q2:tru_t_bot>>h2d_rec_Q2_t_bot(100,0,2,100,0,10)",escat_recon && pprime_recon,"goff");
  T->Draw("rec_Q2:tru_t_top>>h2d_rec_Q2_t_top(100,0,2,100,0,10)",escat_recon && gprime_recon,"goff");
  
  T->Draw("(tru_GMass*tru_GMass):tru_t_bot>>h2d_tru_Qp2_t_bot(100,0,2,100,0,10)","","goff");
  T->Draw("(rec_GMass*rec_GMass):tru_t_bot>>h2d_rec_Qp2_t_bot(100,0,2,100,0,10)",gprime_recon && pprime_recon,"goff");
  T->Draw("(rec_GMass*rec_GMass):tru_t_top>>h2d_rec_Qp2_t_top(100,0,2,100,0,10)",gprime_recon && escat_recon,"goff");
  
  
  //Q2 
  T->Draw("tru_Q2>>htru_Q2(100,0,10)","","goff");
  T->Draw("rec_Q2>>hrec_Q2(100,0,10)",good_escat,"goff");
  T->Draw("rec_Q2>>hrec_Q2_tagger(100,0,10)",good_tagger,"goff");
  T->Draw("rec_Q2>>hrec_Q2_barrel(100,0,10)",good_barrel,"goff");
  T->Draw("rec_Q2>>hrec_Q2_excl(100,0,10)",exclusive,"goff");
  
  T->Draw("log10(tru_Q2):log10(tru_xbj)>>h2d_tru_xbj_Q2(100,-10,0,100,-10,0)","","goff");
  T->Draw("log10(tru_Q2):log10(tru_xbj)>>h2d_tru_xbj_Q2_recon(100,-10,0,100,-10,0)",escat_recon,"goff");
  T->Draw("log10(tru_Q2):log10(tru_xbj)>>h2d_tru_xbj_Q2_good(100,-10,0,100,-10,0)",good_escat,"goff");
  T->Draw("log10(rec_Q2):log10(rec_xbj)>>h2d_rec_xbj_Q2(100,-10,0,100,-10,0)",escat_recon,"goff");
  T->Draw("log10(rec_Q2):log10(rec_xbj)>>h2d_rec_xbj_Q2_good(100,-10,0,100,-10,0)",good_escat,"goff");
  
  T->Draw("tru_Q2-rec_Q2>>hres_Q2(100,-0.001,0.001)",good_escat,"goff");
  
  //escat theta
  T->Draw("tru_theta[scat_ele]>>htru_theta_scat_ele(100,3.06,3.15)","","goff");
  T->Draw("rec_theta[scat_ele]>>hrec_theta_scat_ele(100,3.06,3.15)",escat_recon,"goff");
  T->Draw("rec_theta[scat_ele]>>hrec_theta_scat_ele_good(100,3.06,3.15)",good_escat,"goff");
  
  //escat pmag
  T->Draw("tru_pmag[scat_ele]>>htru_pmag_scat_ele(100,0,18)","","goff");
  T->Draw("rec_pmag[scat_ele]>>hrec_pmag_scat_ele_inclus(100,0,18)",good_tagger,"goff");
  T->Draw("rec_pmag[scat_ele]>>hrec_pmag_scat_ele_semincl(100,0,18)",semi_inclusive && good_tagger,"goff");
  T->Draw("rec_pmag[scat_ele]>>hrec_pmag_scat_ele_semexcl(100,0,18)",semi_exclusive && good_tagger,"goff");
  T->Draw("rec_pmag[scat_ele]>>hrec_pmag_scat_ele_excl(100,0,18)",exclusive && good_tagger,"goff");
  
  //escat 2D acceptance
  T->Draw("log10(tru_Q2):tru_pmag[scat_ele]>>h2d_tru_pmag_Q2(100,0,18,100,-8,0)","","goff");
  T->Draw("log10(tru_Q2):tru_pmag[scat_ele]>>h2d_tru_pmag_Q2_recon(100,0,18,100,-8,0)",escat_recon,"goff");
  T->Draw("log10(tru_Q2):tru_pmag[scat_ele]>>h2d_tru_pmag_Q2_good(100,0,18,100,-8,0)",good_escat,"goff");
  T->Draw("log10(rec_Q2):rec_pmag[scat_ele]>>h2d_rec_pmag_Q2(100,0,18,100,-8,0)",escat_recon,"goff");
  T->Draw("log10(rec_Q2):rec_pmag[scat_ele]>>h2d_rec_pmag_Q2_good(100,0,18,100,-8,0)",good_escat,"goff");
  
  //meson
  T->Draw("tru_GMass>>htru_GMass(100,0,5)","","goff");
  T->Draw("rec_GMass>>hrec_GMass(100,0,5)",gprime_recon,"goff"); //all recon
  T->Draw("rec_GMass>>hrec_GMass_good(100,0,5)",good_gprime,"goff"); //good resolution
  
  T->Draw("tru_eta[ele]>>htru_eta_ele(100,-10,8)","","goff");
  T->Draw("rec_eta[ele]>>hrec_eta_ele(100,-10,8)",ele_recon,"goff");
  T->Draw("rec_eta[ele]>>hrec_eta_ele_good(100,-10,8)",good_gprime,"goff");
  
  T->Draw("tru_eta[pos]>>htru_eta_pos(100,-10,8)","","goff");
  T->Draw("rec_eta[pos]>>hrec_eta_pos(100,-10,8)",pos_recon,"goff");
  T->Draw("rec_eta[pos]>>hrec_eta_pos_good(100,-10,8)",good_gprime,"goff");
  
  auto logy_min=log10(5e-2);
  auto logy_max=log10(4e2);
  
  T->Draw(Form("log10(tru_pmag[ele]):tru_eta[ele]>>h2d_tru_pmag_eta_ele(250,-8,8,250,%f,%f)",logy_min,logy_max),"","goff");
  T->Draw(Form("log10(rec_pmag[ele]):rec_eta[ele]>>h2d_rec_pmag_eta_ele(250,-8,8,250,%f,%f)",logy_min,logy_max),"","goff");
  T->Draw(Form("log10(tru_pmag[pos]):tru_eta[pos]>>h2d_tru_pmag_eta_pos(250,-8,8,250,%f,%f)",logy_min,logy_max),"","goff");
  T->Draw(Form("log10(rec_pmag[pos]):rec_eta[pos]>>h2d_rec_pmag_eta_pos(250,-8,8,250,%f,%f)",logy_min,logy_max),"","goff");
  
  //t / proton recon
  T->Draw("tru_t_bot>>htru_t_bot(100,0,2)","","goff");
  T->Draw("rec_t_bot>>hrec_t_bot(100,0,2)",pprime_recon,"goff");
  T->Draw("rec_t_bot>>hrec_t_bot_excl(100,0,2)",exclusive,"goff");
  T->Draw("rec_t_top>>hrec_t_top_semincl(100,0,2)",semi_inclusive,"goff");
  
  T->Draw("log10(tru_t_bot)>>htru_logt_bot(100,-10,1)","","goff");
  T->Draw("log10(rec_t_bot)>>hrec_logt_bot(100,-10,1)",pprime_recon,"goff");
  T->Draw("log10(rec_t_bot)>>hrec_logt_bot_excl(100,-10,1)",exclusive,"goff");
  
  T->Draw("tru_t_bot-rec_t_bot:tru_t_bot>>h2d_res_t_bot(100,0,1.2,50,-0.4,0.4)",exclusive,"goff");
  
  T->Draw("tru_t_bot-rec_t_bot>>hres_t_bot(100,-10,10)",pprime_recon,"goff");
  
  gDirectory->Write();
  fout->Close();
  DrawHistos(outfilename);
}
