#ifdef __CLING__
#pragma cling optimize(0)
#endif
void hres_W()
{
//=========Macro generated from canvas: hres_W/
//=========  (Fri Feb  6 17:08:49 2026) by ROOT version 6.38.00
   TCanvas *hres_W = new TCanvas("hres_W", "", 14, 35, 700, 500);
   gStyle->SetOptFit(0);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   TColor::SetPalette(57, nullptr);
   hres_W->Range(-6.538462,-6165.651,6.282051,45214.77);
   hres_W->SetFillColor(0);
   hres_W->SetBorderMode(0);
   hres_W->SetBorderSize(2);
   hres_W->SetLeftMargin(0.12);
   hres_W->SetTopMargin(0.05);
   hres_W->SetBottomMargin(0.12);
   hres_W->SetFrameBorderMode(0);
   hres_W->SetFrameBorderMode(0);
   
   TH1D *hres_W_copy__23 = new TH1D("hres_W_copy", "", 100, -5, 5);
   std::vector<Double_t> hres_W_copy__23_vect3{
      219969, 3368, 3610, 3581, 3837, 3803, 3752, 3847, 3972, 4090,
      4212, 4310, 4313, 4431, 4718, 4649, 4815, 4792, 4888, 5165,
      5178, 5355, 5334, 5715, 5792, 5665, 6050, 6314, 6354, 6448,
      6732, 6863, 7122, 7477, 7630, 8158, 8242, 8629, 9194, 9503,
      9924, 10315, 11265, 11891, 12637, 13848, 15578, 17577, 20885, 26571,
      40615, 38640, 24121, 18505, 15199, 13156, 11661, 10484, 9660, 8888,
      8283, 7600, 7199, 6779, 6395, 6047, 5784, 5366, 5277, 5025,
      4718, 4633, 4376, 4297, 4004, 3830, 3655, 3584, 3467, 3271,
      3122, 3057, 2931, 2835, 2790, 2703, 2563, 2496, 2447, 2326,
      2285, 2143, 2083, 2004, 1953, 1963, 1932, 1789, 1765, 1633,
      1656, 157279
   };
   for (Int_t bin = 0; bin < 102; bin++)
      if (hres_W_copy__23_vect3[bin])
         hres_W_copy__23->SetBinContent(bin, hres_W_copy__23_vect3[bin]);
   hres_W_copy__23->SetEntries(1088642);
   hres_W_copy__23->SetDirectory(nullptr);
   hres_W_copy__23->SetStats(0);
   hres_W_copy__23->SetLineWidth(2);
   hres_W_copy__23->SetMarkerStyle(20);
   hres_W_copy__23->SetMarkerSize(1.200000047683716);
   hres_W_copy__23->GetXaxis()->SetTitle("#DeltaW [GeV]");
   hres_W_copy__23->GetXaxis()->SetLabelFont(42);
   hres_W_copy__23->GetXaxis()->SetLabelSize(0.04199999943375587);
   hres_W_copy__23->GetXaxis()->SetTitleSize(0.04500000178813934);
   hres_W_copy__23->GetXaxis()->SetTitleOffset(1.100000023841858);
   hres_W_copy__23->GetXaxis()->SetTitleFont(42);
   hres_W_copy__23->GetYaxis()->SetTitle("N_{evts}");
   hres_W_copy__23->GetYaxis()->SetLabelFont(42);
   hres_W_copy__23->GetYaxis()->SetLabelSize(0.04199999943375587);
   hres_W_copy__23->GetYaxis()->SetTitleSize(0.04500000178813934);
   hres_W_copy__23->GetYaxis()->SetTitleOffset(1.100000023841858);
   hres_W_copy__23->GetYaxis()->SetTitleFont(42);
   hres_W_copy__23->GetZaxis()->SetLabelFont(42);
   hres_W_copy__23->GetZaxis()->SetLabelSize(0.04199999943375587);
   hres_W_copy__23->GetZaxis()->SetTitleSize(0.04500000178813934);
   hres_W_copy__23->GetZaxis()->SetTitleOffset(1.100000023841858);
   hres_W_copy__23->GetZaxis()->SetTitleFont(42);
   hres_W_copy__23->Draw();
   TLatex *tex = new TLatex(0.6, 0.6, "#sigma_{#DeltaW} = 0.32 GeV");
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
   hres_W->Modified();
   hres_W->SetSelected(hres_W);
}
