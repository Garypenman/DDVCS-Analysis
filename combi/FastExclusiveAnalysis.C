#include "../include/Config.h"
#include "../include/DrawHelperFuncs.h"
#include "../include/DrawEpicInfo.h"

#include "../epic_plot_macro/ePIC_style.C"

std::string file1 = "/w/work6/home/gp140f/tcs_bg_merge/bg_rad/";
std::string outdir1 = "./results/FastTiming/";


void FastExclusiveAnalysis(std::string infiledir=file1,
                           std::string outfiledir=outdir1,
                           int ebeam=9,
                           int pbeam=130,
			   bool optimizeCuts = false)
{

  set_ePIC_style();

  string Eset = to_string(ebeam)+"x"+to_string(pbeam);

  gSystem->Exec(("mkdir -p "+outfiledir).c_str());

  // --------------------------------------------------
  // Files
  // --------------------------------------------------

  ROOT::RDataFrame df_rec_base(
    "tree",
    infiledir + "/TCS_all_rec_Tree.root");

  auto df_rec = df_rec_base.Define(
    "rec_scatele_pprime_tcoin",
    [](double tele, double tprot)
    {
      return tele - tprot;
    },
    {"rec_scat_ele_track_time",
     "rec_pprime_rphits_timeavg"}
  );

  // --------------------------------------------------
  // Signal / BG
  // --------------------------------------------------

  auto df_sig =
    df_rec.Filter(
      "isTruth==1 && fabs(rec_ele_eta)<4 && fabs(rec_pos_eta)<4");

  auto df_bg =
    df_rec.Filter(
      "isTruth!=1 && fabs(rec_ele_eta)<4 && fabs(rec_pos_eta)<4");

  // --------------------------------------------------
  // HARDCODED EXCLUSIVITY CUTS
  // --------------------------------------------------

  std::string MissMass2Cut =
    "abs(rec_MissMass2)<0.5";

  std::string MissPtCut =
    "rec_MissPt<0.8";

  std::string DeltaPhiProtonCut =
    "abs(rec_DeltaPhiProton)<0.2";

  std::string DeltaThetaProtonCut =
    "rec_DeltaThetaProton < 0.0025";
  
  std::string DeltaPProtonCut = 
    "rec_DeltaPProton < 4.0";
  
  //rough fit by hand to peaks for now
  double tag1peak = -49.0; //ns
  double tag2peak = 12.2; //ns
  double tcoin_sigma = 3.0; //ns
  double nsigma_cut = 2.0;
  
  std::string TimingCut = Form("abs(rec_scatele_pprime_tcoin - %f) < (%f * %f) || abs(rec_scatele_pprime_tcoin - %f) < (%f * %f)",
			       tag1peak, nsigma_cut, tcoin_sigma, tag2peak, nsigma_cut, tcoin_sigma);

 
  // ----------------------------------------------------------
  // Exclusivity variable definition
  // ----------------------------------------------------------
  struct ExclVar {
    std::string name;
    std::string branch;
    int nbins;
    double xmin;
    double xmax;
    bool use_cut;       
    std::string cut_expr;  // filled later
  };
  
  std::vector<ExclVar> vars = {
    {"M^{2}_{miss}", "rec_MissMass2", 250, -15, 15, true, MissMass2Cut},
    {"P_{T,miss}",    "rec_MissPt",    250, 0,   2.0, true, MissPtCut},

    {"#delta#phi_{p'}",  "rec_DeltaPhiProton", 250, -3.14, 3.14, true, DeltaPhiProtonCut},
    {"#delta#theta_{p'}",  "rec_DeltaThetaProton", 250, 0.0, 0.005, true, DeltaThetaProtonCut},
    {"#deltap_{p'}",  "rec_DeltaPProton", 250, 0.0, 10.0, true, DeltaPProtonCut},

    {"#delta#phi_{e'}",  "rec_DeltaPhiScatEle", 250, -3.14, 3.14, true, ""},
    {"#delta#theta_{e'}",  "rec_DeltaThetaScatEle", 250, 0.0, 0.01, true, ""},
    {"#deltap_{e'}",  "rec_DeltaPScatEle", 250, 0.0, 10.0, true, ""},

    // {"#delta#phi_{ExP}",  "rec_DeltaPhiExP", 250, -3.14, 3.14, true, ""},
    // {"#delta#phi_{ExG}",  "rec_DeltaPhiExG", 250, -3.14, 3.14, true, ""},
    // {"#delta#phi_{GxP}",  "rec_DeltaPhiGxP", 250, -3.14, 3.14, true, ""},

    {"P_{x, miss}", "rec_miss_px", 250, -5, 5, true, ""},
    {"P_{y, miss}", "rec_miss_py", 250, -5, 5, true, ""},
    {"P_{z, miss}", "rec_miss_pz", 250, -5, 5, true, ""}
    //{"#phi_{miss}",   "rec_miss_phi",   250, -3.2, 3.2, true, ""},
    //{"#theta_{miss}", "rec_miss_theta", 250, 0,   0.1, true, ""}
  };

  std::string excl_cut = "";

  for (auto &v : vars) {

    if (v.cut_expr.empty())
      continue;

    if (!v.use_cut)
      continue;
    
    if (!excl_cut.empty())
      excl_cut += " && ";

    excl_cut += "(" + v.cut_expr + ")";
  }
  
  auto df_sig_cut =
    df_sig.Filter(excl_cut);

  auto df_bg_cut =
    df_bg.Filter(excl_cut);

  auto df_sig_final =
    df_sig_cut.Filter(TimingCut);

  auto df_bg_final =
    df_bg_cut.Filter(TimingCut);

  
  int nsigma_test = 3;
  
  std::vector<ROOT::RDF::RResultPtr<ULong64_t>> Nsig_test(nsigma_test);
  std::vector<ROOT::RDF::RResultPtr<ULong64_t>> Nbg_test(nsigma_test);
  
  for (int i = 1; i <= nsigma_test; i++) {
    
    std::string TimingCutTest =
      Form("abs(rec_scatele_pprime_tcoin-(%f))<(%d*%f) || "
	   "abs(rec_scatele_pprime_tcoin-(%f))<(%d*%f)",
	   tag1peak, i, tcoin_sigma,
	   tag2peak, i, tcoin_sigma);
    
    Nsig_test[i-1] = df_sig_cut.Filter(TimingCutTest).Count();
    Nbg_test[i-1] = df_bg_cut.Filter(TimingCutTest).Count();
  }

  
  auto Nsig = df_sig.Count();
  auto Nbg = df_bg.Count();
  auto Nsig_cut = df_sig_cut.Count();
  auto Nbg_cut = df_bg_cut.Count();
  auto Nsig_final = df_sig_final.Count();
  auto Nbg_final = df_bg_final.Count();
  
  // ----------------------------------------------------------
  // STORAGE (IMPORTANT: keeps things alive)
  // ----------------------------------------------------------
  std::vector<ROOT::RDF::RResultPtr<TH1D>> rawhists;
  std::vector<ROOT::RDF::RResultPtr<TH1D>> cuthists;
  
  // ----------------------------------------------------------
  // 1. Raw distributions (signal vs background)
  // ----------------------------------------------------------
  for (auto &v : vars) {

    auto h_sig = df_sig.Histo1D(
				{("h_"+v.name+"_sig").c_str(), (v.name+";"+v.name+";Events").c_str(),
				 v.nbins, v.xmin, v.xmax},
				v.branch
				);
    rawhists.push_back(h_sig);

    auto h_bg = df_bg.Histo1D(
			      {("h_"+v.name+"_bg").c_str(), (v.name+";"+v.name+";Events").c_str(),
			       v.nbins, v.xmin, v.xmax},
			      v.branch
			      );
    rawhists.push_back(h_bg);
  }

  // ----------------------------------------------------------
  // 2. Histograms with all other cuts applied
  // ----------------------------------------------------------
  for (size_t i = 0; i < vars.size(); i++) {
    std::string other_cuts = "";
    for (size_t j = 0; j < vars.size(); j++) {
      if (i == j) continue;
      if (vars[j].cut_expr=="") continue;
      if (!other_cuts.empty()) other_cuts += " && ";
      other_cuts += "(" + vars[j].cut_expr + ")";
    }
    
    cuthists.push_back(df_sig.Filter(other_cuts).Histo1D({("h_"+vars[i].name+"_sig_post").c_str(),
	  (vars[i].name+";"+vars[i].name+";Events").c_str(),
	  vars[i].nbins, vars[i].xmin, vars[i].xmax},
	vars[i].branch)
      );
    
    cuthists.push_back(df_bg.Filter(other_cuts).Histo1D({("h_"+vars[i].name+"_bg_post").c_str(),
	  (vars[i].name+";"+vars[i].name+";Events").c_str(),
	  vars[i].nbins, vars[i].xmin, vars[i].xmax},
	vars[i].branch)
      );
    
  }
  
  // --------------------------------------------------
  // 3. Timing Histograms
  // --------------------------------------------------
  
  double timelo = -250;
  double timehi = 250;
  
  auto hSigTimeBefore =
    df_sig.Histo1D(
      {"hSigTimeBefore",
       ";e-p' coincidence time [ns];Events",
       200,timelo,timehi},
      "rec_scatele_pprime_tcoin");

  auto hBgTimeBefore =
    df_bg.Histo1D(
      {"hBgTimeBefore",
       ";e-p' coincidence time [ns];Events",
       200,timelo,timehi},
      "rec_scatele_pprime_tcoin");

  auto hSigTimeAfter =
    df_sig_cut.Histo1D(
      {"hSigTimeAfter",
       ";e-p' coincidence time [ns];Events",
       200,timelo,timehi},
      "rec_scatele_pprime_tcoin");

  auto hBgTimeAfter =
    df_bg_cut.Histo1D(
      {"hBgTimeAfter",
       ";e-p' coincidence time [ns];Events",
       200,timelo,timehi},
      "rec_scatele_pprime_tcoin");

  std::vector<TCanvas*> canvases;

  for (size_t i = 0; i < vars.size(); i++) {
    
    auto *c = new TCanvas(("c_raw_"+vars[i].name).c_str(),
			  vars[i].name.c_str(), 800, 600);
    c->cd()->SetLogy();
    auto h_sig = rawhists[2*i];
    auto h_bg  = rawhists[2*i + 1];
    auto h_sig_cut = cuthists[2*i];
    auto h_bg_cut  = cuthists[2*i + 1];
    
    h_sig->SetLineColor(kRed);
    h_sig->SetLineWidth(2);
    h_sig_cut->SetLineColor(kRed+1);
    h_sig_cut->SetLineWidth(2);
    
    h_bg->SetLineColor(kBlue);
    h_bg->SetLineWidth(2);
    h_bg_cut->SetLineColor(kBlue+1);
    h_bg_cut->SetLineWidth(2);

    auto h_bg_copy = h_bg->DrawCopy("hist");
    auto h_sig_copy = h_sig->DrawCopy("hist same");
    auto h_bg_cut_copy = h_bg_cut->DrawCopy("hist same");
    auto h_sig_cut_copy = h_sig_cut->DrawCopy("hist same");

    auto ymax = std::max(h_sig->GetMaximum(), h_bg->GetMaximum());
    if(vars[i].cut_expr!="")
      DrawCutLine(vars[i].cut_expr, 0, ymax);
    
    h_bg_copy->SetMinimum(0.1);
    h_sig_copy->SetMinimum(0.1);
    h_bg_cut_copy->SetMinimum(0.1);
    h_sig_cut_copy->SetMinimum(0.1);
    
    auto leg = new TLegend(0.6,0.5,0.8,0.7);
    leg->AddEntry(h_sig_copy, "Raw Signal", "l");
    leg->AddEntry(h_bg_copy, "Raw Background", "l");
    leg->AddEntry(h_sig_cut_copy, "All other cuts Sig", "l");
    leg->AddEntry(h_bg_cut_copy, "All other cuts BG", "l");
    leg->Draw();
    
    //c->Write();
    std::string printname = vars[i].branch + ".png";
    c->Print(printname.c_str());
    c->Print(Form("temp_%03d.pdf",static_cast<int>(i) ) );
    canvases.push_back(c);
  }
  gSystem->Exec("pdfunite temp_*.pdf ExclVars.pdf");
  gSystem->Exec("rm temp_*.pdf");
  
  // --------------------------------------------------
  // BEFORE CUTS
  // --------------------------------------------------

  auto cBefore =
    new TCanvas("cTimeBefore",
                "Timing Before Cuts",
                800,600);
  cBefore->cd()->SetLogy();
  hSigTimeBefore->SetLineColor(kRed);
  hBgTimeBefore->SetLineColor(kBlue);

  hBgTimeBefore->SetMinimum(0.1);
  hSigTimeBefore->SetMinimum(0.1);

  hBgTimeBefore->DrawCopy("hist");
  hSigTimeBefore->DrawCopy("hist same");

  auto leg1 =
    new TLegend(0.65,0.65,0.88,0.80);

  leg1->AddEntry(
    hSigTimeBefore.GetPtr(),
    "Signal",
    "l");

  leg1->AddEntry(
    hBgTimeBefore.GetPtr(),
    "Background",
    "l");

  leg1->Draw();

  cBefore->Print("TimingBeforeCuts.pdf");

  // --------------------------------------------------
  // AFTER CUTS
  // --------------------------------------------------

  auto cAfter =
    new TCanvas("cTimeAfter",
                "Timing After Cuts",
                800,600);
  cAfter->cd()->SetLogy();
  
  hSigTimeAfter->SetLineColor(kRed);
  hBgTimeAfter->SetLineColor(kBlue);

  hBgTimeAfter->SetMinimum(0.1);
  hSigTimeAfter->SetMinimum(0.1);

  hBgTimeAfter->DrawCopy("hist");
  hSigTimeAfter->DrawCopy("hist same");

  DrawLine(hSigTimeAfter.GetPtr(),
	   tag1peak - nsigma_cut*tcoin_sigma);

  DrawLine(hSigTimeAfter.GetPtr(),
	   tag1peak + nsigma_cut*tcoin_sigma);

  DrawLine(hSigTimeAfter.GetPtr(),
	   tag2peak - nsigma_cut*tcoin_sigma);

  DrawLine(hSigTimeAfter.GetPtr(),
         tag2peak + nsigma_cut*tcoin_sigma);

  auto leg2 =
    new TLegend(0.65,0.65,0.88,0.8);

  leg2->AddEntry(
    hSigTimeAfter.GetPtr(),
    "Signal",
    "l");

  leg2->AddEntry(
    hBgTimeAfter.GetPtr(),
    "Background",
    "l");

  leg2->Draw();

  cAfter->SaveAs("TimingAfterCuts.pdf");

  // --------------------------------------------------
  // COUNTS
  // --------------------------------------------------
  std::cout << "\nBefore cuts\n";
  std::cout << "Signal = "
            << *Nsig
            << "\n";
  std::cout << "Background = "
            << *Nbg
            << "\n";
  std::cout << "S/B = "
	    << (double) *Nsig / *Nbg
	    << "\n";
  
  std::cout << "\nAfter cuts\n";
  std::cout << "Signal = "
            << *Nsig_cut
            << "\n";
  std::cout << "Background = "
            << *Nbg_cut
            << "\n";
  std::cout << "S/B = "
	    << (double) *Nsig_cut / *Nbg_cut
	    << "\n";

  std::cout << "\n=== Timing Cut Scan ===\n";
  
  for (int i = 1; i <= nsigma_test; i++) {
    
    auto S = *Nsig_test[i-1];
    auto B = *Nbg_test[i-1];
    
    double SB = (B > 0) ? (double)S/B : 0.0;
    
    std::cout
      << i << " sigma:"
      << "  S=" << S
      << "  B=" << B
      << "  S/B=" << SB
      << "\n";
  }
  
  std::cout << "\nAfter timing cuts\n";
  std::cout << "Signal = "
            << *Nsig_final
            << "\n";
  std::cout << "Background = "
            << *Nbg_final
            << "\n";
  std::cout << "S/B = "
	    << (double) *Nsig_final / *Nbg_final
	    << "\n";
  
 

  // --------------------------------------------------
  // ROOT output
  // --------------------------------------------------
  
  // TFile fout((outfiledir +
  // 	      "/FastExclusive.root").c_str(),
  // 	     "RECREATE");
  // fout.cd();
  
  // hSigTimeBefore->Write();
  // hBgTimeBefore->Write();
  
  // hSigTimeAfter->Write();
  // hBgTimeAfter->Write();
  
  // cBefore->Write();
  // cAfter->Write();

  // fout.Close();

  gSystem->Exec("pdfunite ExclVars.pdf TimingBeforeCuts.pdf TimingAfterCuts.pdf ExclTimingAnalysis.pdf");
  gSystem->Exec(Form("mv *.pdf *.png %s",outfiledir.c_str()));
}
