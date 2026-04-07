#ifdef __CLING__
#pragma cling optimize(0)
#endif
void hres_pmag_scatele()
{
//=========Macro generated from canvas: hres_pmag_scatele/
//=========  (Fri Feb  6 17:08:49 2026) by ROOT version 6.38.00
   TCanvas *hres_pmag_scatele = new TCanvas("hres_pmag_scatele", "", 14, 35, 700, 500);
   gStyle->SetOptFit(0);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   TColor::SetPalette(57, nullptr);
   hres_pmag_scatele->Range(-0.1307692,-2912.69,0.125641,25510.56);
   hres_pmag_scatele->SetFillColor(0);
   hres_pmag_scatele->SetBorderMode(0);
   hres_pmag_scatele->SetBorderSize(2);
   hres_pmag_scatele->SetLeftMargin(0.12);
   hres_pmag_scatele->SetTopMargin(0.05);
   hres_pmag_scatele->SetBottomMargin(0.12);
   hres_pmag_scatele->SetFrameBorderMode(0);
   hres_pmag_scatele->SetFrameBorderMode(0);
   
   TH1D *hres_pmag_scatele_copy__19 = new TH1D("hres_pmag_scatele_copy", "", 100, -0.1, 0.1);
   std::vector<Double_t> hres_pmag_scatele_copy__19_vect0{
      29580, 1568, 1683, 1757, 1832, 1933, 2113, 2215, 2328, 2520,
      2689, 2743, 2990, 3161, 3297, 3468, 3694, 3895, 4209, 4340,
      4636, 5010, 5234, 5469, 5633, 6213, 6585, 6942, 7297, 7757,
      8085, 8698, 9289, 9744, 10244, 10909, 11765, 12345, 13167, 13954,
      14668, 15612, 16292, 17561, 18117, 19032, 20149, 20864, 21412, 22243,
      22700, 22748, 22966, 22486, 22221, 21585, 20622, 20091, 19535, 18810,
      18064, 17370, 16359, 15764, 14962, 14337, 13791, 12820, 12258, 11716,
      10754, 10584, 10205, 9413, 9190, 8379, 8148, 7850, 7326, 6935,
      6415, 6226, 5966, 5553, 5250, 5035, 4788, 4592, 4248, 4138,
      3846, 3695, 3501, 3372, 3119, 2933, 2818, 2729, 2402, 2257,
      2297, 116532
   };
   for (Int_t bin = 0; bin < 102; bin++)
      if (hres_pmag_scatele_copy__19_vect0[bin])
         hres_pmag_scatele_copy__19->SetBinContent(bin, hres_pmag_scatele_copy__19_vect0[bin]);
   hres_pmag_scatele_copy__19->SetEntries(1088642);
   hres_pmag_scatele_copy__19->SetDirectory(nullptr);
   hres_pmag_scatele_copy__19->SetStats(0);
   hres_pmag_scatele_copy__19->SetLineWidth(2);
   hres_pmag_scatele_copy__19->SetMarkerStyle(20);
   hres_pmag_scatele_copy__19->SetMarkerSize(1.200000047683716);
   hres_pmag_scatele_copy__19->GetXaxis()->SetTitle("#Deltap_{e} / p_{e}");
   hres_pmag_scatele_copy__19->GetXaxis()->SetLabelFont(42);
   hres_pmag_scatele_copy__19->GetXaxis()->SetLabelSize(0.04199999943375587);
   hres_pmag_scatele_copy__19->GetXaxis()->SetTitleSize(0.04500000178813934);
   hres_pmag_scatele_copy__19->GetXaxis()->SetTitleOffset(1.100000023841858);
   hres_pmag_scatele_copy__19->GetXaxis()->SetTitleFont(42);
   hres_pmag_scatele_copy__19->GetYaxis()->SetTitle("N_{evts}");
   hres_pmag_scatele_copy__19->GetYaxis()->SetLabelFont(42);
   hres_pmag_scatele_copy__19->GetYaxis()->SetLabelSize(0.04199999943375587);
   hres_pmag_scatele_copy__19->GetYaxis()->SetTitleSize(0.04500000178813934);
   hres_pmag_scatele_copy__19->GetYaxis()->SetTitleOffset(1.100000023841858);
   hres_pmag_scatele_copy__19->GetYaxis()->SetTitleFont(42);
   hres_pmag_scatele_copy__19->GetZaxis()->SetLabelFont(42);
   hres_pmag_scatele_copy__19->GetZaxis()->SetLabelSize(0.04199999943375587);
   hres_pmag_scatele_copy__19->GetZaxis()->SetTitleSize(0.04500000178813934);
   hres_pmag_scatele_copy__19->GetZaxis()->SetTitleOffset(1.100000023841858);
   hres_pmag_scatele_copy__19->GetZaxis()->SetTitleFont(42);
   hres_pmag_scatele_copy__19->Draw();
   TLatex *tex = new TLatex(0.6, 0.6, "#sigma_{#Deltap/p} = 2.15 %");
   tex->SetTextAlign(13);
   tex->SetTextFont(42);
   tex->SetLineWidth(2);
   tex->SetNDC();
   tex->Draw();
   tex = new TLatex(0.15, 0.88, "ePIC Performance");
   tex->SetLineWidth(2);
   tex->SetNDC();
   tex->Draw();
   tex = new TLatex(0.15, 0.85, "e+p, 10x100 GeV");
   tex->SetTextAlign(13);
   tex->SetTextFont(42);
   tex->SetTextSize(0.04);
   tex->SetLineWidth(2);
   tex->SetNDC();
   tex->Draw();
   tex = new TLatex(0.65, 0.92, "25.10.3 Campaign");
   tex->SetTextAlign(13);
   tex->SetTextFont(42);
   tex->SetTextSize(0.04);
   tex->SetLineWidth(2);
   tex->SetNDC();
   tex->Draw();
   tex = new TLatex(0.65, 0.88, "EpIC 1.1.6");
   tex->SetTextAlign(13);
   tex->SetTextFont(42);
   tex->SetTextSize(0.04);
   tex->SetLineWidth(2);
   tex->SetNDC();
   tex->Draw();
   tex = new TLatex(0.65, 0.84, "EpIC ep #rightarrow e'#gammap");
   tex->SetTextAlign(13);
   tex->SetTextFont(42);
   tex->SetTextSize(0.04);
   tex->SetLineWidth(2);
   tex->SetNDC();
   tex->Draw();
   hres_pmag_scatele->Modified();
   hres_pmag_scatele->SetSelected(hres_pmag_scatele);
}
