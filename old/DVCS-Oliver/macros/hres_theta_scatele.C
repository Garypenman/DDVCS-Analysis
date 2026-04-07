#ifdef __CLING__
#pragma cling optimize(0)
#endif
void hres_theta_scatele()
{
//=========Macro generated from canvas: hres_theta_scatele/
//=========  (Fri Feb  6 17:08:49 2026) by ROOT version 6.38.00
   TCanvas *hres_theta_scatele = new TCanvas("hres_theta_scatele", "", 0, 0, 700, 500);
   gStyle->SetOptFit(0);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   TColor::SetPalette(57, nullptr);
   hres_theta_scatele->Range(-7.846154,-27876.21,7.538462,204425.5);
   hres_theta_scatele->SetFillColor(0);
   hres_theta_scatele->SetBorderMode(0);
   hres_theta_scatele->SetBorderSize(2);
   hres_theta_scatele->SetLeftMargin(0.12);
   hres_theta_scatele->SetTopMargin(0.05);
   hres_theta_scatele->SetBottomMargin(0.12);
   hres_theta_scatele->SetFrameBorderMode(0);
   hres_theta_scatele->SetFrameBorderMode(0);
   
   TH1D *hres_theta_scatele_copy__21 = new TH1D("hres_theta_scatele_copy", "", 100, -6, 6);
   std::vector<Double_t> hres_theta_scatele_copy__21_vect2{
      1357, 17, 16, 15, 12, 20, 19, 18, 14, 27,
      29, 25, 37, 42, 33, 47, 38, 46, 66, 73,
      81, 79, 97, 132, 144, 160, 165, 209, 253, 287,
      329, 388, 418, 553, 591, 798, 965, 1192, 1415, 1751,
      2232, 2988, 4067, 5674, 8713, 14306, 26224, 49895, 90984, 143806,
      183629, 183472, 142854, 90915, 49806, 25619, 14191, 8528, 5614, 3867,
      2850, 2245, 1809, 1415, 1144, 912, 814, 635, 599, 429,
      362, 309, 276, 281, 223, 197, 143, 127, 118, 111,
      85, 63, 64, 61, 68, 61, 45, 44, 44, 37,
      26, 35, 35, 24, 19, 26, 18, 24, 27, 19,
      19, 3457
   };
   for (Int_t bin = 0; bin < 102; bin++)
      if (hres_theta_scatele_copy__21_vect2[bin])
         hres_theta_scatele_copy__21->SetBinContent(bin, hres_theta_scatele_copy__21_vect2[bin]);
   hres_theta_scatele_copy__21->SetEntries(1088642);
   hres_theta_scatele_copy__21->SetDirectory(nullptr);
   hres_theta_scatele_copy__21->SetStats(0);
   hres_theta_scatele_copy__21->SetLineWidth(2);
   hres_theta_scatele_copy__21->SetMarkerStyle(20);
   hres_theta_scatele_copy__21->SetMarkerSize(1.200000047683716);
   hres_theta_scatele_copy__21->GetXaxis()->SetTitle("#Delta#theta_{e} [mrad]");
   hres_theta_scatele_copy__21->GetXaxis()->SetLabelFont(42);
   hres_theta_scatele_copy__21->GetXaxis()->SetLabelSize(0.04199999943375587);
   hres_theta_scatele_copy__21->GetXaxis()->SetTitleSize(0.04500000178813934);
   hres_theta_scatele_copy__21->GetXaxis()->SetTitleOffset(1.100000023841858);
   hres_theta_scatele_copy__21->GetXaxis()->SetTitleFont(42);
   hres_theta_scatele_copy__21->GetYaxis()->SetTitle("N_{evts}");
   hres_theta_scatele_copy__21->GetYaxis()->SetLabelFont(42);
   hres_theta_scatele_copy__21->GetYaxis()->SetLabelSize(0.04199999943375587);
   hres_theta_scatele_copy__21->GetYaxis()->SetTitleSize(0.04500000178813934);
   hres_theta_scatele_copy__21->GetYaxis()->SetTitleOffset(1.100000023841858);
   hres_theta_scatele_copy__21->GetYaxis()->SetTitleFont(42);
   hres_theta_scatele_copy__21->GetZaxis()->SetLabelFont(42);
   hres_theta_scatele_copy__21->GetZaxis()->SetLabelSize(0.04199999943375587);
   hres_theta_scatele_copy__21->GetZaxis()->SetTitleSize(0.04500000178813934);
   hres_theta_scatele_copy__21->GetZaxis()->SetTitleOffset(1.100000023841858);
   hres_theta_scatele_copy__21->GetZaxis()->SetTitleFont(42);
   hres_theta_scatele_copy__21->Draw();
   TLatex *tex = new TLatex(0.6, 0.6, "#sigma_{#Delta#theta} = 0.28 mrad");
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
   hres_theta_scatele->Modified();
   hres_theta_scatele->SetSelected(hres_theta_scatele);
}
