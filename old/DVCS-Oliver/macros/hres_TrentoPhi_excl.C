#ifdef __CLING__
#pragma cling optimize(0)
#endif
void hres_TrentoPhi_excl()
{
//=========Macro generated from canvas: hres_TrentoPhi_excl/
//=========  (Fri Feb  6 17:08:49 2026) by ROOT version 6.38.00
   TCanvas *hres_TrentoPhi_excl = new TCanvas("hres_TrentoPhi_excl", "", 0, 0, 700, 500);
   gStyle->SetOptFit(0);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   TColor::SetPalette(57, nullptr);
   hres_TrentoPhi_excl->Range(-1.307692,-6547.749,1.25641,48016.83);
   hres_TrentoPhi_excl->SetFillColor(0);
   hres_TrentoPhi_excl->SetBorderMode(0);
   hres_TrentoPhi_excl->SetBorderSize(2);
   hres_TrentoPhi_excl->SetLeftMargin(0.12);
   hres_TrentoPhi_excl->SetTopMargin(0.05);
   hres_TrentoPhi_excl->SetBottomMargin(0.12);
   hres_TrentoPhi_excl->SetFrameBorderMode(0);
   hres_TrentoPhi_excl->SetFrameBorderMode(0);
   
   TH1D *hres_TrentoPhi_copy__28 = new TH1D("hres_TrentoPhi_copy", "", 100, -1, 1);
   std::vector<Double_t> hres_TrentoPhi_copy__28_vect6{
      30477, 282, 314, 290, 292, 283, 278, 280, 315, 319,
      280, 281, 303, 303, 306, 324, 301, 263, 342, 339,
      328, 322, 336, 352, 340, 357, 387, 410, 429, 417,
      433, 476, 594, 605, 647, 737, 840, 938, 1176, 1581,
      2048, 2918, 4535, 6743, 10459, 15032, 20630, 26740, 33239, 39440,
      43132, 41632, 36672, 30679, 25767, 21087, 16927, 12700, 9610, 6989,
      5263, 3841, 2888, 2153, 1683, 1426, 1133, 899, 803, 676,
      621, 571, 528, 476, 445, 396, 402, 392, 398, 389,
      377, 335, 334, 314, 315, 308, 281, 344, 322, 335,
      305, 284, 306, 307, 331, 316, 307, 285, 254, 264,
      280, 29838
   };
   for (Int_t bin = 0; bin < 102; bin++)
      if (hres_TrentoPhi_copy__28_vect6[bin])
         hres_TrentoPhi_copy__28->SetBinContent(bin, hres_TrentoPhi_copy__28_vect6[bin]);
   hres_TrentoPhi_copy__28->SetEntries(516581);
   hres_TrentoPhi_copy__28->SetDirectory(nullptr);
   hres_TrentoPhi_copy__28->SetStats(0);
   hres_TrentoPhi_copy__28->SetLineWidth(2);
   hres_TrentoPhi_copy__28->SetMarkerStyle(20);
   hres_TrentoPhi_copy__28->SetMarkerSize(1.200000047683716);
   hres_TrentoPhi_copy__28->GetXaxis()->SetTitle("#phi_{l} [rad]");
   hres_TrentoPhi_copy__28->GetXaxis()->SetLabelFont(42);
   hres_TrentoPhi_copy__28->GetXaxis()->SetLabelSize(0.04199999943375587);
   hres_TrentoPhi_copy__28->GetXaxis()->SetTitleSize(0.04500000178813934);
   hres_TrentoPhi_copy__28->GetXaxis()->SetTitleOffset(1.100000023841858);
   hres_TrentoPhi_copy__28->GetXaxis()->SetTitleFont(42);
   hres_TrentoPhi_copy__28->GetYaxis()->SetTitle("N_{evts}");
   hres_TrentoPhi_copy__28->GetYaxis()->SetLabelFont(42);
   hres_TrentoPhi_copy__28->GetYaxis()->SetLabelSize(0.04199999943375587);
   hres_TrentoPhi_copy__28->GetYaxis()->SetTitleSize(0.04500000178813934);
   hres_TrentoPhi_copy__28->GetYaxis()->SetTitleOffset(1.100000023841858);
   hres_TrentoPhi_copy__28->GetYaxis()->SetTitleFont(42);
   hres_TrentoPhi_copy__28->GetZaxis()->SetLabelFont(42);
   hres_TrentoPhi_copy__28->GetZaxis()->SetLabelSize(0.04199999943375587);
   hres_TrentoPhi_copy__28->GetZaxis()->SetTitleSize(0.04500000178813934);
   hres_TrentoPhi_copy__28->GetZaxis()->SetTitleOffset(1.100000023841858);
   hres_TrentoPhi_copy__28->GetZaxis()->SetTitleFont(42);
   hres_TrentoPhi_copy__28->Draw();
   TLatex *tex = new TLatex(0.6, 0.6, "#sigma_{#Delta#phi_{l}} = 0.07 rad");
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
   hres_TrentoPhi_excl->Modified();
   hres_TrentoPhi_excl->SetSelected(hres_TrentoPhi_excl);
}
