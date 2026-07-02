#include "../include/Config.h"
#include "../include/DrawEpicInfo.h"

#include "../epic_plot_macro/ePIC_style.C"

const std::vector<EColor> kp6 = {kP6Violet, kP6Gray, kP6Grape, kP6Red, kP6Yellow, kP6Blue};


void DrawOneSplitTwo(TH1D* all, TH1D* h1, TH1D* h2){
  
  all->SetLineColor(kBlack);
  //h1->SetLineColor(0);
  h1->SetFillColorAlpha(kp6[0],0.8);
  h1->SetFillStyle(3004);
  //h2->SetLineColor(0);
  h2->SetFillColorAlpha(kp6[1],0.8);
  h2->SetFillStyle(3005);
  //all->SetMaximum(1.25*all->GetMaximum());
  all->DrawCopy("hist p");
  h1->DrawCopy("same");
  h2->DrawCopy("same");
  
}


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

void DrawCutLine(const std::string& cut_expr,
                 double ymin, double ymax)
{
  // Extract number from expression (simple parsing)
  double cut_val = 0;

  if (cut_expr.find("<") != std::string::npos) {
    cut_val = std::stod(cut_expr.substr(cut_expr.find("<") + 1));
  }

  // +cut
  TLine *line1 = new TLine(cut_val, ymin, cut_val, ymax);
  line1->SetLineColor(kGreen+2);
  line1->SetLineWidth(2);
  line1->SetLineStyle(2);
  line1->Draw();

  // -cut (for abs variables)
  if (cut_expr.find("abs") != std::string::npos) {
    TLine *line2 = new TLine(-cut_val, ymin, -cut_val, ymax);
    line2->SetLineColor(kGreen+2);
    line2->SetLineWidth(2);
    line2->SetLineStyle(2);
    line2->Draw();
  }
}

// later wrapper this stuff in RunEverything.C
// std::string plus_dir = "/w/work6/home/gp140f/combirad_trees/Campaign26.03.1/ddvcs_ee_18x275_hplus/";
// std::string minus_dir = "/w/work6/home/gp140f/combirad_trees/Campaign26.03.1/ddvcs_ee_18x275_hminus/";
// std::string result_dir = "./results/Campaign26.03.1/ddvcs_ee_18x275_hplus/"

// std::string plus_dir = "/w/work6/home/gp140f/combirad_trees/Campaign26.04.1/ddvcs_ee_9x130_hplus/";
// std::string minus_dir = "/w/work6/home/gp140f/combirad_trees/Campaign26.04.1/ddvcs_ee_9x130_hminus/";
// std::string result_dir = "./results/Campaign26.04.1/ddvcs_ee_9x130_hplus/"

std::string bg_dir = "/w/work6/home/gp140f/tcs_bg_merge/bg_rad/";
std::string nobg_dir = "/w/work6/home/gp140f/tcs_bg_merge/nobg_rad/";
std::string result_dir = "./results/BGMerge/9x130";

void ExclusiveAnalysis(std::string infiledir = bg_dir, std::string outfiledir = result_dir, int ebeam = 9, int pbeam = 130, std::string fcampaign="26.04.1"){

  set_ePIC_style();

  // ----------------------------------------------------------
  // Metadata
  // ----------------------------------------------------------
  int Ecom = sqrt(4*ebeam*pbeam); //sqrt(s)
  string Eset = to_string(ebeam) + "x" + to_string(pbeam);
  double Ngen = 1e5;
  double xsec = 5.0; //nb
  double dxsec = 0.1; //nb
  double lumi_fb = 1;
  double lumi_nb = lumi_fb * 1e6; //fb-1 to nb-1
  // double scale = xsec*Lumi / Ngen;
  // double dscale = dxsec*Lumi;

  // ----------------------------------------------------------
  // Files
  // ----------------------------------------------------------
  std::string tru_filename = infiledir + "TCS_tru_Tree.root";
  std::string rec_filename = infiledir + "TCS_all_rec_Tree.root";
  std::string outfilename = "TCS_"+Eset+"_exclusive_analysis.root";
  TFile fout(outfilename.c_str(), "RECREATE");
  
  // ----------------------------------------------------------
  // TCuts (seperate header later)
  // ----------------------------------------------------------
  //scattered electron
  //
  //e+e- pair -> gprime
  //
  //recoil pprime
  //from hdelta_*_pprime fits
  double dpmag_pprime = 1.45; //GeV/c
  double dtheta_pprime = 0.25; //mrad
  double dphi_pprime = 0.19; //rad
  TCut dpmag_pprime_cut = Form("fabs(delta_pmag_pprime) < 3*%f",dpmag_pprime);
  TCut dtheta_pprime_cut = Form("fabs(delta_theta_pprime) < 3*%f",dtheta_pprime);
  TCut dphi_pprime_cut = Form("fabs(delta_phi_pprime) < 3*%f",dphi_pprime);
  //exclusivity
  TCut excl_phys = "rec_Q2<10"; //dphi_pprime_cut; //placeholder
  TCut final_selec = excl_phys;
  

  // ----------------------------------------------------------
  // Dataframes
  // ----------------------------------------------------------
  ROOT::RDataFrame df_tru("tree", tru_filename);
  ROOT::RDataFrame df_rec("tree", rec_filename);
  auto df_sig = df_rec.Filter("isTruth==1 && rec_ele_eta > -4 && rec_pos_eta > -4");
  auto df_bg  = df_rec.Filter("isTruth!=1 && rec_ele_eta > -4 && rec_pos_eta > -4");

  
  // ----------------------------------------------------------
  // Exclusivity variable definition
  // ----------------------------------------------------------
  struct ExclVar {
    std::string name;
    std::string branch;
    int nbins;
    double xmin;
    double xmax;
    bool use_abs;       
    std::string cut_expr;  // filled later
  };
  
  std::vector<ExclVar> vars = {
    {"M^{2}_{miss}", "rec_MissMass2", 250, -15, 15, true, ""},
    {"P_{T,miss}",    "rec_MissPt",    250, 0,   2.0, false, ""},
    {"#delta#phi_{p'}",  "rec_DeltaPhiProton", 250, -3.2, 3.2, true, ""}
    //{"MissPhi",   "rec_MissPhi",   250, -3.2, 3.2, true, ""},
    //{"MissTheta", "rec_MissTheta", 250, 0,   0.1, false, ""}
  };

  
  // ----------------------------------------------------------
  // STORAGE (IMPORTANT: keeps things alive)
  // ----------------------------------------------------------
  std::vector<ROOT::RDF::RResultPtr<TH1D>> hists;

  // ----------------------------------------------------------
  // 1. Raw distributions (signal vs background)
  // ----------------------------------------------------------
  for (auto &v : vars) {

    auto h_sig = df_sig.Histo1D(
				{("h_"+v.name+"_sig").c_str(), (v.name+";"+v.name+";Events").c_str(),
				 v.nbins, v.xmin, v.xmax},
				v.branch
				);
    hists.push_back(h_sig);

    auto h_bg = df_bg.Histo1D(
			      {("h_"+v.name+"_bg").c_str(), (v.name+";"+v.name+";Events").c_str(),
			       v.nbins, v.xmin, v.xmax},
			      v.branch
			      );
    hists.push_back(h_bg);
  }

  
  double S0 = *df_sig.Count();
  double B0 = *df_bg.Count();
  
  std::cout << "\n=== Initial Sample ===\n";
  std::cout << "S = " << S0 << "\n";
  std::cout << "B = " << B0 << "\n";
  std::cout << "S/B = " << (B0 > 0 ? S0/B0 : 0) << "\n";
  

  std::vector<TCanvas*> canvases;

  for (size_t i = 0; i < vars.size(); i++) {
    
    auto *c = new TCanvas(("c_raw_"+vars[i].name).c_str(),
			  vars[i].name.c_str(), 800, 600);
    
    auto h_sig = hists[2*i];       // stored order
    auto h_bg  = hists[2*i + 1];
    
    h_sig->SetLineColor(kRed);
    h_sig->SetLineWidth(2);
    
    h_bg->SetLineColor(kBlue);
    h_bg->SetLineWidth(2);
    
    auto h_bg_copy = h_bg->DrawCopy("hist");
    auto h_sig_copy = h_sig->DrawCopy("hist same");

    //auto ymax = std::max(h_sig->GetMaximum(), h_bg->GetMaximum());
    //DrawCutLine(vars[i].cut_expr, 0, ymax);
    
    h_bg_copy->SetMinimum(0);
    h_sig_copy->SetMinimum(0);
    
    auto leg = new TLegend(0.65,0.75,0.88,0.88);
    leg->AddEntry(h_sig_copy, "Signal", "l");
    leg->AddEntry(h_bg_copy, "Background", "l");
    leg->Draw();
    
    c->Write();
    canvases.push_back(c);
  }

  // ----------------------------------------------------------
  // CUT OPTIMISATION FUNCTION
  // ----------------------------------------------------------
  auto OptimizeCut = [&](ROOT::RDF::RNode df_s,
                         ROOT::RDF::RNode df_b,
                         const ExclVar &v)
  {
    double best_cut = 0;
    double best_sig = 0;

    for (double cut = 0.001; cut < v.xmax; cut += (v.xmax / 100.0)) {

      std::string expr;

      if (v.use_abs)
        expr = "abs(" + v.branch + ") < " + std::to_string(cut);
      else
        expr = v.branch + " < " + std::to_string(cut);

      double S = *df_s.Filter(expr).Count();
      double B = *df_b.Filter(expr).Count();

      if (S + B == 0) continue;

      double signif = S / sqrt(S + B);

      if (signif > best_sig) {
        best_sig = signif;
        best_cut = cut;
      }
    }

    if (v.use_abs)
      return "abs(" + v.branch + ") < " + std::to_string(best_cut);
    else
      return v.branch + " < " + std::to_string(best_cut);
  };

  // ----------------------------------------------------------
  // 2. AUTOMATIC OPTIMISATION
  // ----------------------------------------------------------

  std::cout << "\n=== Automatic Cut Optimisation ===\n";

  for (size_t i = 0; i < vars.size(); i++) {

    
    std::string other_cuts = "";

    for (size_t j = 0; j < vars.size(); j++) {
      if (i == j) continue;
      if (vars[j].cut_expr.empty()) continue;

      if (!other_cuts.empty()) other_cuts += " && ";
      other_cuts += "(" + vars[j].cut_expr + ")";
    }

    auto df_s = df_sig;
    auto df_b = df_bg;

    if (!other_cuts.empty()) {
      df_s = df_sig.Filter(other_cuts);
      df_b = df_bg.Filter(other_cuts);
    }

    vars[i].cut_expr = OptimizeCut(df_s, df_b, vars[i]);

    std::cout << vars[i].name << " -> " << vars[i].cut_expr << "\n";

  }
  

  
  // ----------------------------------------------------------
  // 3. Individual Cut Performance)
  // ----------------------------------------------------------
  std::cout << "\n=== Individual Cut Performance ===\n";

  double SB0 = (B0 > 0 ? S0/B0 : 0);

  for (auto &v : vars) {

    double S = *df_sig.Filter(v.cut_expr).Count();
    double B = *df_bg.Filter(v.cut_expr).Count();

    double effS = (S0 > 0 ? S/S0 : 0);
    double effB = (B0 > 0 ? B/B0 : 0);

    double SB = (B > 0 ? S/B : 0);

    std::cout << v.name << "\n";
    std::cout << "  Cut: " << v.cut_expr << "\n";
    std::cout << "  Signal efficiency: " << effS << "\n";
    std::cout << "  Background efficiency: " << effB << "\n";
    std::cout << "  Background rejection: " << (1 - effB) << "\n";
    std::cout << "  S/B: " << SB << "\n";
    std::cout << "  S/B improvement x"
	      << (SB0 > 0 ? SB / SB0 : 0) << "\n\n";
  }

  // ----------------------------------------------------------
  // 3. Histograms with all other cuts applied
  // ----------------------------------------------------------
  for (size_t i = 0; i < vars.size(); i++) {
    std::string other_cuts = "";
    for (size_t j = 0; j < vars.size(); j++) {
      if (i == j) continue;

      if (!other_cuts.empty()) other_cuts += " && ";
      other_cuts += "(" + vars[j].cut_expr + ")";
    }
    
    hists.push_back(
		    df_sig.Filter(other_cuts).Histo1D(
						      {("h_"+vars[i].name+"_sig_post").c_str(),
						       (vars[i].name+";"+vars[i].name+";Events").c_str(),
						       vars[i].nbins, vars[i].xmin, vars[i].xmax},
						      vars[i].branch)
		    );
    
    hists.push_back(
		    df_bg.Filter(other_cuts).Histo1D(
						     {("h_"+vars[i].name+"_bg_post").c_str(),
						      (vars[i].name+";"+vars[i].name+";Events").c_str(),
						      vars[i].nbins, vars[i].xmin, vars[i].xmax},
						     vars[i].branch)
		    );
    
  }

  // ----------------------------------------------------------
  // 4. Draw histos with all other cuts applied
  // ----------------------------------------------------------
  int offset = vars.size()*2;

  for (size_t i = 0; i < vars.size(); i++) {

    auto *c = new TCanvas(("c_post_"+vars[i].name).c_str(),
			  vars[i].name.c_str(), 800, 600);

    auto h_sig = hists[offset + 2*i];
    auto h_bg  = hists[offset + 2*i + 1];

    h_sig->SetLineColor(kRed);
    h_sig->SetLineWidth(2);

    h_bg->SetLineColor(kBlue);
    h_bg->SetLineWidth(2);

    auto h_bg_copy = h_bg->DrawCopy("hist");
    auto h_sig_copy = h_sig->DrawCopy("hist same");
    auto ymax = std::max(h_sig->GetMaximum(), h_bg->GetMaximum());
    h_bg_copy->SetMinimum(0);
    h_sig_copy->SetMinimum(0);
    h_bg_copy->SetMaximum(ymax);
    h_sig_copy->SetMaximum(ymax);
    
    DrawCutLine(vars[i].cut_expr, 0, ymax);
    
    auto leg = new TLegend(0.65,0.75,0.88,0.88);
    leg->AddEntry(h_sig_copy, "Signal", "l");
    leg->AddEntry(h_bg_copy, "Background", "l");
    leg->Draw();

    c->Write();
    canvases.push_back(c);
  }
 
  // ----------------------------------------------------------
  // 5. Sequential Cut Flow Performance
  // ----------------------------------------------------------
  std::cout << "\n=== Cut Flow ===\n";
 
  auto df_s = df_sig;
  auto df_b = df_bg;

  double S_prev = S0;
  double B_prev = B0;

  for (auto &v : vars) {

    df_s = df_s.Filter(v.cut_expr);
    df_b = df_b.Filter(v.cut_expr);

    double S = *df_s.Count();
    double B = *df_b.Count();

    double SB = (B > 0 ? S/B : 0);
    double SB_prev = (B_prev > 0 ? S_prev/B_prev : 0);

    std::cout << v.name << "\n";
    std::cout << "  S=" << S << "  B=" << B << "\n";
    std::cout << "  S/B=" << SB << "\n";
    std::cout << "  Improvement x"
	      << (SB_prev > 0 ? SB/SB_prev : 0) << "\n\n";

    S_prev = S;
    B_prev = B;

  }
  fout.cd();
  
  for (auto &h : hists) {
    h->Write();
  }

  //==
  
  // ----------------------------------------------------------
  // 6. Final Physics Impacts With Cuts
  // ----------------------------------------------------------
  std::string final_cut = "";

  for (auto &v : vars) {
    if (!final_cut.empty()) final_cut += " && ";
    final_cut += "(" + v.cut_expr + ")";
  }
  
  auto df_rec_cut = df_rec.Filter(final_cut);

  
  //|t|
  auto htru_tbot = df_tru.Histo1D({"htru_tbot",";|t|_{pp'} [GeV^{2}];N_{evts} / 0.02 GeV^{2} ",50,0,1},"tru_t_bot");
  auto htru_tbot_acc = df_rec_cut.Histo1D({"htru_tbot_acc",";|t|_{pp'} [GeV^{2}];N_{evts} / 0.02 GeV^{2} ",50,0,1},"tru_t_bot");
  auto hrec_tbot = df_rec_cut.Histo1D({"hrec_tbot",";|t|_{pp'} [GeV^{2}];N_{evts} / 0.02 GeV^{2} ",50,0,1},"rec_t_bot");
  //Qp2
  auto htru_Qp2 = df_tru.Histo1D({"htru_Qp2",";Q^{'2} [GeV^{2}];N_{evts} / 0.4 GeV^{2}",50,0,20},"tru_Qp2");
  auto htru_Qp2_acc = df_rec_cut.Histo1D({"htru_Qp2_acc",";Q^{#prime2} [GeV^{2}];N_{evts} / 0.4 GeV^{2}",50,0,20},"tru_Qp2");
  auto hrec_Qp2 = df_rec_cut.Histo1D({"hrec_Qp2",";Q^{#prime2} [GeV^{2}];N_{evts} / 0.4 GeV^{2}",50,0,20},"rec_Qp2");
  //theta_L
  auto htru_CosThetaHel = df_tru.Histo1D({"htru_CosThetaHel",";cos(#theta_{l});N_{evts} / 0.04 rad",50,-1,1},"tru_CosThetaHel");
  auto htru_CosThetaHel_acc = df_rec_cut.Histo1D({"htru_CosThetaHel_acc",";cos(#theta_{l});N_{evts} / 0.04 rad",50,-1,1},"tru_CosThetaHel");
  auto hrec_CosThetaHel = df_rec_cut.Histo1D({"hrec_CosThetaHel",";cos(#theta_{l});N_{evts} / 0.04 rad",50,-1,1},"rec_CosThetaHel");
  //phi_L
  auto htru_PhiHel = df_tru.Histo1D({"htru_PhiHel",";#phi_{l} [rad];N_{evts} / 0.13 rad",50,-M_PI,M_PI},"tru_PhiHel");
  auto htru_PhiHel_acc = df_rec_cut.Histo1D({"htru_PhiHel_acc",";#phi_{l} [rad];N_{evts} / 0.13 rad",50,-M_PI,M_PI},"tru_PhiHel");
  auto hrec_PhiHel = df_rec_cut.Histo1D({"hrec_PhiHel",";#phi_{l} [rad];N_{evts} / 0.13 rad",50,-M_PI,M_PI},"rec_PhiHel");

  
  // ----------------------------------------------------------
  // Canvases
  // ----------------------------------------------------------
  new TCanvas("hdNdt");
  gPad->SetLogy();
  auto hacc_tbot = AcceptancePlot(htru_tbot.GetPtr(),hrec_tbot.GetPtr(),"hacc_tbot");
  auto htru_dNdt = XSecPlot(htru_tbot.GetPtr(),xsec*lumi_nb/Ngen,"htru_dNdt");
  auto htru_dNdt_acc = XSecPlot(htru_tbot_acc.GetPtr(),xsec*lumi_nb/Ngen,"htru_dNdt_acc");
  auto hrec_dNdt = XSecPlot(hrec_tbot.GetPtr(),xsec*lumi_nb/Ngen,"hrec_dNdt");
  auto hrec_dNdt_corr = XSecPlot(hrec_tbot.GetPtr(),xsec*lumi_nb/Ngen,"htru_dNdt_corr", hacc_tbot);
  
  htru_dNdt->SetMarkerStyle(0);
  htru_dNdt->GetYaxis()->SetTitle("dN/dt [GeV^{-2}]");
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
  DrawLumiText(lumi_fb);
  DrawSimText(fcampaign);
  TLegend *leg_tbot = new TLegend(0.6,0.55,0.8,0.7);
  leg_tbot->AddEntry(htru_dNdt,"MC","l");
  leg_tbot->AddEntry(htru_dNdt_acc,"MC Accepted","P");
  leg_tbot->AddEntry(hrec_dNdt,"RECO","P");
  leg_tbot->Draw();
  gPad->Update();
  gPad->Print("hdNdt.pdf");
  gPad->Print("hdNdt.C");
  gPad->Write();
  //gPad->Close();
  
  // new TCanvas("hdNdt_corr");
  // gPad->SetLogy();
  // hrec_dNdt_corr->SetMarkerStyle(4);
  // hrec_dNdt_corr->SetMarkerColor(kp6[2]);
  // hrec_dNdt_corr->SetLineColor(kp6[2]);
  // htru_dNdt->DrawCopy("hist");
  // hrec_dNdt_corr->DrawCopy("same p");
  // DrawEpicText();
  // DrawEpicEnergyText(Eset);
  // DrawLumiText(lumi_fb);
  // DrawSimText(fcampaign);
  // TLegend *leg_tcorr = new TLegend(0.6,0.6,0.8,0.7);
  // leg_tcorr->AddEntry(htru_dNdt,"MC","l");
  // leg_tcorr->AddEntry(hrec_dNdt_corr,"RECO Corrected","P");
  // leg_tcorr->Draw();
  // gPad->Update();
  // gPad->Print("hdNdt_corr.pdf");
  // gPad->Print("hdNdt_corr.C");
  // gPad->Write();
  // gPad->Close();
  

  //Qp2
  new TCanvas("hdNdQp2");
  gPad->SetLogy();
  auto hacc_Qp2 = AcceptancePlot(htru_Qp2.GetPtr(),hrec_Qp2.GetPtr(),"hacc_Qp2");
  auto htru_dNdQp2 = XSecPlot(htru_Qp2.GetPtr(),xsec*lumi_nb/Ngen,"htru_dNdQp2");
  auto htru_dNdQp2_acc = XSecPlot(htru_Qp2_acc.GetPtr(),xsec*lumi_nb/Ngen,"htru_dNdQp2_acc");
  auto hrec_dNdQp2 = XSecPlot(hrec_Qp2.GetPtr(),xsec*lumi_nb/Ngen,"hrec_dNdQp2");
  auto hrec_dNdQp2_corr = XSecPlot(hrec_Qp2.GetPtr(),xsec*lumi_nb/Ngen,"htru_dNdQp2_corr", hacc_Qp2);
  
  htru_dNdQp2->SetMarkerStyle(0);
  htru_dNdQp2->GetYaxis()->SetTitle("dN/dQ'^{2} [GeV^{-2}]");
  htru_dNdQp2->SetMinimum(1e1);
  htru_dNdQp2->SetMaximum(1e11);
  htru_dNdQp2->DrawCopy("hist");
  
  htru_dNdQp2_acc->SetMarkerStyle(25);
  htru_dNdQp2_acc->SetMarkerColor(kp6[1]);
  htru_dNdQp2_acc->SetLineColor(kp6[1]);
  htru_dNdQp2_acc->DrawCopy("same pe");

  hrec_dNdQp2->SetMarkerStyle(4);
  hrec_dNdQp2->SetMarkerColor(kp6[0]);
  hrec_dNdQp2->SetLineColor(kp6[0]);
  hrec_dNdQp2->DrawCopy("same pe");
  
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawLumiText(lumi_fb);
  DrawSimText(fcampaign);
  TLegend *leg_Qp2 = new TLegend(0.6,0.55,0.8,0.7);
  leg_Qp2->AddEntry(htru_dNdQp2,"MC","l");
  leg_Qp2->AddEntry(htru_dNdQp2_acc,"MC Accepted","P");
  leg_Qp2->AddEntry(hrec_dNdQp2,"RECO","P");
  leg_Qp2->Draw();
  gPad->Update();
  gPad->Print("hdNQp2.pdf");
  gPad->Print("hdNQp2.C");
  gPad->Write();
  //gPad->Close();
  
  //CosThetaHel
  new TCanvas("hdNdCosThetaHel");
  gPad->SetLogy();
  auto hacc_CosThetaHel = AcceptancePlot(htru_CosThetaHel.GetPtr(),hrec_CosThetaHel.GetPtr(),"hacc_CosThetaHel");
  auto htru_dNdCosThetaHel = XSecPlot(htru_CosThetaHel.GetPtr(),xsec*lumi_nb/Ngen,"htru_dNdCosThetaHel");
  auto htru_dNdCosThetaHel_acc = XSecPlot(htru_CosThetaHel_acc.GetPtr(),xsec*lumi_nb/Ngen,"htru_dNdCosThetaHel_acc");
  auto hrec_dNdCosThetaHel = XSecPlot(hrec_CosThetaHel.GetPtr(),xsec*lumi_nb/Ngen,"hrec_dNdCosThetaHel");
  auto hrec_dNdCosThetaHel_corr = XSecPlot(hrec_CosThetaHel.GetPtr(),xsec*lumi_nb/Ngen,"htru_dNdCosThetaHel_corr", hacc_CosThetaHel);
  
  htru_dNdCosThetaHel->SetMarkerStyle(0);
  htru_dNdCosThetaHel->GetYaxis()->SetTitle("dN/dcos(#theta_{l})");
  htru_dNdCosThetaHel->SetMinimum(1e2);
  htru_dNdCosThetaHel->SetMaximum(1e13);
  htru_dNdCosThetaHel->DrawCopy("hist");
  
  htru_dNdCosThetaHel_acc->SetMarkerStyle(25);
  htru_dNdCosThetaHel_acc->SetMarkerColor(kp6[1]);
  htru_dNdCosThetaHel_acc->SetLineColor(kp6[1]);
  htru_dNdCosThetaHel_acc->DrawCopy("same pe");

  hrec_dNdCosThetaHel->SetMarkerStyle(4);
  hrec_dNdCosThetaHel->SetMarkerColor(kp6[0]);
  hrec_dNdCosThetaHel->SetLineColor(kp6[0]);
  hrec_dNdCosThetaHel->DrawCopy("same pe");
  
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawLumiText(lumi_fb);
  DrawSimText(fcampaign);
  TLegend *leg_CosThetaHel = new TLegend(0.6,0.55,0.8,0.7);
  leg_CosThetaHel->AddEntry(htru_dNdCosThetaHel,"MC","l");
  leg_CosThetaHel->AddEntry(htru_dNdCosThetaHel_acc,"MC Accepted","P");
  leg_CosThetaHel->AddEntry(hrec_dNdCosThetaHel,"RECO","P");
  leg_CosThetaHel->Draw();
  gPad->Update();
  gPad->Print("hdNCosThetaHel.pdf");
  gPad->Print("hdNCosThetaHel.C");
  gPad->Write();
  //gPad->Close();
  
  //PhiHel
  new TCanvas("hdNdPhiHel");
  gPad->SetLogy();
  auto hacc_PhiHel = AcceptancePlot(htru_PhiHel.GetPtr(),hrec_PhiHel.GetPtr(),"hacc_PhiHel");
  auto htru_dNdPhiHel = XSecPlot(htru_PhiHel.GetPtr(),xsec*lumi_nb/Ngen,"htru_dNdPhiHel");
  auto htru_dNdPhiHel_acc = XSecPlot(htru_PhiHel_acc.GetPtr(),xsec*lumi_nb/Ngen,"htru_dNdPhiHel_acc");
  auto hrec_dNdPhiHel = XSecPlot(hrec_PhiHel.GetPtr(),xsec*lumi_nb/Ngen,"hrec_dNdPhiHel");
  auto hrec_dNdPhiHel_corr = XSecPlot(hrec_PhiHel.GetPtr(),xsec*lumi_nb/Ngen,"htru_dNdPhiHel_corr", hacc_PhiHel);
  
  htru_dNdPhiHel->SetMarkerStyle(0);
  htru_dNdPhiHel->GetYaxis()->SetTitle("dN/d#phi_{l} [rad^{-1}]");
  htru_dNdPhiHel->SetMinimum(1e2);
  htru_dNdPhiHel->SetMaximum(1e13);
  htru_dNdPhiHel->DrawCopy("hist");
  
  htru_dNdPhiHel_acc->SetMarkerStyle(25);
  htru_dNdPhiHel_acc->SetMarkerColor(kp6[1]);
  htru_dNdPhiHel_acc->SetLineColor(kp6[1]);
  htru_dNdPhiHel_acc->DrawCopy("same pe");

  hrec_dNdPhiHel->SetMarkerStyle(4);
  hrec_dNdPhiHel->SetMarkerColor(kp6[0]);
  hrec_dNdPhiHel->SetLineColor(kp6[0]);
  hrec_dNdPhiHel->DrawCopy("same pe");
  
  DrawEpicText();
  DrawEpicEnergyText(Eset);
  DrawLumiText(lumi_fb);
  DrawSimText(fcampaign);
  TLegend *leg_PhiHel = new TLegend(0.6,0.55,0.8,0.7);
  leg_PhiHel->AddEntry(htru_dNdPhiHel,"MC","l");
  leg_PhiHel->AddEntry(htru_dNdPhiHel_acc,"MC Accepted","P");
  leg_PhiHel->AddEntry(hrec_dNdPhiHel,"RECO","P");
  leg_PhiHel->Draw();
  gPad->Update();
  gPad->Print("hdNPhiHel.pdf");
  gPad->Print("hdNPhiHel.C");
  gPad->Write();
  //gPad->Close();

  fout.Write();
  fout.Close();
  gSystem->Exec( Form("mv h*.pdf h*.C TCS*.root %s",outfiledir.c_str()) );
}


// // Exclusivity (truth = zero, tru comes from tru rec event flag)
  // auto hrec_MissMass2_bg = df_bg.Histo1D({"hrec_MissMass2","All Brem BG Combis; M^{2}_{miss} (e' e^{+}e^{-} p') [GeV^{2}/c^{2}]; Events / 0.12 GeV/c",250,-15,15},"rec_MissMass2");
  // auto hrec_MissMass2_tru = df_sig.Histo1D({"hrec_MissMass2_tru","Truth Events",250,-15,15},"rec_MissMass2");
  // auto hrec_MissMass2_cut = df_rec.Filter((miss_pt_cut).GetTitle()).Histo1D({"hrec_MissMass2","BG+Cuts",250,-15,15},"rec_MissMass2");
  // auto hrec_MissMass2_tru_cut = df_rec.Histo1D({"hrec_MissMass2","BG+Cuts",250,-15,15},"rec_MissMass2");
  
  // auto hrec_MissPt = df_rec.Histo1D({"hrec_MissPt","All Brem BG Combis; p_{T,miss} (e' e^{+}e^{-} p') [GeV/c]; Events / 0.12 GeV/c",250,0,2.0},"rec_MissPt");
  // auto hrec_MissPt_tru = df_rec_tru.Histo1D({"hrec_MissPt_tru","Truth Events",250,0,2.0},"rec_MissPt");
  // auto hrec_MissPt_cut = df_rec_cut.Histo1D({"hrec_MissPt","All Brem BG Combis",250,0,2.0},"rec_MissPt");

  // new TCanvas();
  // //hrec_MissMass2->Fit("gaus","");
  // hrec_MissMass2->SetMinimum(0);
  // hrec_MissMass2->DrawCopy();
  // hrec_MissMass2_tru->SetLineColor(kRed);
  // hrec_MissMass2_tru->DrawCopy("same");
  // //gPad->SetLogy();
  // gPad->BuildLegend(0.15,0.7,0.35,0.8);

  // new TCanvas();
  // //hrec_MissPt->Fit("gaus","");
  // hrec_MissPt->SetMinimum(0);
  // hrec_MissPt->DrawCopy();
  // hrec_MissPt_tru->SetLineColor(kRed);
  // hrec_MissPt_tru->DrawCopy("same");
  
