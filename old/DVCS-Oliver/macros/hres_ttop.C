#ifdef __CLING__
#pragma cling optimize(0)
#endif
void hres_ttop()
{
//=========Macro generated from canvas: hres_ttop/
//=========  (Fri Feb  6 17:08:49 2026) by ROOT version 6.38.00
   TCanvas *hres_ttop = new TCanvas("hres_ttop", "", 0, 0, 700, 500);
   gStyle->SetOptFit(0);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   TColor::SetPalette(57, nullptr);
   hres_ttop->Range(-1.307692,-3728.993,1.25641,27345.95);
   hres_ttop->SetFillColor(0);
   hres_ttop->SetBorderMode(0);
   hres_ttop->SetBorderSize(2);
   hres_ttop->SetLeftMargin(0.12);
   hres_ttop->SetTopMargin(0.05);
   hres_ttop->SetBottomMargin(0.12);
   hres_ttop->SetFrameBorderMode(0);
   hres_ttop->SetFrameBorderMode(0);
   
   TH1D *hres_ttop_copy__25 = new TH1D("hres_ttop_copy", "", 100, -1, 1);
   std::vector<Double_t> hres_ttop_copy__25_vect5{
      147462, 1645, 1661, 1728, 1829, 1734, 1764, 1808, 1941, 1881,
      2009, 1940, 1937, 2040, 2176, 2073, 2223, 2332, 2381, 2368,
      2490, 2491, 2665, 2767, 2811, 2992, 2985, 3090, 3261, 3454,
      3561, 3745, 3916, 4029, 4258, 4305, 4631, 4817, 5171, 5353,
      5643, 5906, 6418, 6902, 7832, 8805, 10265, 12672, 16742, 23266,
      24564, 16533, 10553, 7616, 5989, 5048, 4487, 3906, 3344, 3121,
      2912, 2626, 2426, 2320, 2163, 2177, 1975, 1817, 1835, 1697,
      1704, 1545, 1511, 1470, 1401, 1378, 1217, 1205, 1155, 1163,
      1020, 1056, 980, 890, 944, 887, 878, 886, 828, 781,
      745, 754, 642, 672, 686, 639, 626, 577, 570, 590,
      576, 15583
   };
   for (Int_t bin = 0; bin < 102; bin++)
      if (hres_ttop_copy__25_vect5[bin])
         hres_ttop_copy__25->SetBinContent(bin, hres_ttop_copy__25_vect5[bin]);
   hres_ttop_copy__25->SetEntries(514843);
   hres_ttop_copy__25->SetDirectory(nullptr);
   hres_ttop_copy__25->SetStats(0);
   hres_ttop_copy__25->SetLineWidth(2);
   hres_ttop_copy__25->SetMarkerStyle(20);
   hres_ttop_copy__25->SetMarkerSize(1.200000047683716);
   hres_ttop_copy__25->GetXaxis()->SetTitle("#Delta|t_{e#gamma'}| [GeV^{2}]");
   hres_ttop_copy__25->GetXaxis()->SetLabelFont(42);
   hres_ttop_copy__25->GetXaxis()->SetLabelSize(0.04199999943375587);
   hres_ttop_copy__25->GetXaxis()->SetTitleSize(0.04500000178813934);
   hres_ttop_copy__25->GetXaxis()->SetTitleOffset(1.100000023841858);
   hres_ttop_copy__25->GetXaxis()->SetTitleFont(42);
   hres_ttop_copy__25->GetYaxis()->SetTitle("N_{evts}");
   hres_ttop_copy__25->GetYaxis()->SetLabelFont(42);
   hres_ttop_copy__25->GetYaxis()->SetLabelSize(0.04199999943375587);
   hres_ttop_copy__25->GetYaxis()->SetTitleSize(0.04500000178813934);
   hres_ttop_copy__25->GetYaxis()->SetTitleOffset(1.100000023841858);
   hres_ttop_copy__25->GetYaxis()->SetTitleFont(42);
   hres_ttop_copy__25->GetZaxis()->SetLabelFont(42);
   hres_ttop_copy__25->GetZaxis()->SetLabelSize(0.04199999943375587);
   hres_ttop_copy__25->GetZaxis()->SetTitleSize(0.04500000178813934);
   hres_ttop_copy__25->GetZaxis()->SetTitleOffset(1.100000023841858);
   hres_ttop_copy__25->GetZaxis()->SetTitleFont(42);
   hres_ttop_copy__25->Draw();
   TLatex *tex = new TLatex(0.6, 0.6, "#sigma_{#Delta|t_{e#gamma}|} = 0.06 GeV^{2}");
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
   hres_ttop->Modified();
   hres_ttop->SetSelected(hres_ttop);
}
