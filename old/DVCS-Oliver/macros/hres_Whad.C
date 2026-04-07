#ifdef __CLING__
#pragma cling optimize(0)
#endif
void hres_Whad()
{
//=========Macro generated from canvas: hres_Whad/
//=========  (Fri Feb  6 17:08:49 2026) by ROOT version 6.38.00
   TCanvas *hres_Whad = new TCanvas("hres_Whad", "", 0, 0, 700, 500);
   gStyle->SetOptFit(0);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   TColor::SetPalette(57, nullptr);
   hres_Whad->Range(-6.538462,-10027.63,6.282051,73535.93);
   hres_Whad->SetFillColor(0);
   hres_Whad->SetBorderMode(0);
   hres_Whad->SetBorderSize(2);
   hres_Whad->SetLeftMargin(0.12);
   hres_Whad->SetTopMargin(0.05);
   hres_Whad->SetBottomMargin(0.12);
   hres_Whad->SetFrameBorderMode(0);
   hres_Whad->SetFrameBorderMode(0);
   
   TH1D *hres_Whad_copy__24 = new TH1D("hres_Whad_copy", "", 100, -5, 5);
   std::vector<Double_t> hres_Whad_copy__24_vect4{
      34306, 659, 584, 635, 626, 611, 593, 606, 605, 571,
      590, 535, 518, 513, 456, 424, 494, 413, 404, 372,
      311, 312, 302, 287, 283, 279, 242, 265, 331, 368,
      413, 502, 526, 626, 759, 876, 1074, 1175, 1418, 1713,
      2098, 2648, 3450, 4664, 6166, 8511, 12598, 18888, 28722, 41463,
      54489, 63458, 66055, 63231, 56951, 50772, 44372, 38108, 33359, 28598,
      24072, 20530, 17574, 14670, 12553, 10561, 9028, 7751, 6844, 6014,
      5115, 4649, 4210, 3950, 3600, 3308, 3218, 2964, 2740, 2628,
      2471, 2389, 2324, 2221, 2117, 2019, 1932, 1933, 1813, 1717,
      1608, 1563, 1510, 1439, 1404, 1432, 1348, 1283, 1193, 1195,
      1122, 113170
   };
   for (Int_t bin = 0; bin < 102; bin++)
      if (hres_Whad_copy__24_vect4[bin])
         hres_Whad_copy__24->SetBinContent(bin, hres_Whad_copy__24_vect4[bin]);
   hres_Whad_copy__24->SetEntries(1000360);
   hres_Whad_copy__24->SetDirectory(nullptr);
   hres_Whad_copy__24->SetStats(0);
   hres_Whad_copy__24->SetLineWidth(2);
   hres_Whad_copy__24->SetMarkerStyle(20);
   hres_Whad_copy__24->SetMarkerSize(1.200000047683716);
   hres_Whad_copy__24->GetXaxis()->SetTitle("#DeltaWhad [GeV]");
   hres_Whad_copy__24->GetXaxis()->SetLabelFont(42);
   hres_Whad_copy__24->GetXaxis()->SetLabelSize(0.04199999943375587);
   hres_Whad_copy__24->GetXaxis()->SetTitleSize(0.04500000178813934);
   hres_Whad_copy__24->GetXaxis()->SetTitleOffset(1.100000023841858);
   hres_Whad_copy__24->GetXaxis()->SetTitleFont(42);
   hres_Whad_copy__24->GetYaxis()->SetTitle("N_{evts}");
   hres_Whad_copy__24->GetYaxis()->SetLabelFont(42);
   hres_Whad_copy__24->GetYaxis()->SetLabelSize(0.04199999943375587);
   hres_Whad_copy__24->GetYaxis()->SetTitleSize(0.04500000178813934);
   hres_Whad_copy__24->GetYaxis()->SetTitleOffset(1.100000023841858);
   hres_Whad_copy__24->GetYaxis()->SetTitleFont(42);
   hres_Whad_copy__24->GetZaxis()->SetLabelFont(42);
   hres_Whad_copy__24->GetZaxis()->SetLabelSize(0.04199999943375587);
   hres_Whad_copy__24->GetZaxis()->SetTitleSize(0.04500000178813934);
   hres_Whad_copy__24->GetZaxis()->SetTitleOffset(1.100000023841858);
   hres_Whad_copy__24->GetZaxis()->SetTitleFont(42);
   hres_Whad_copy__24->Draw();
   TLatex *tex = new TLatex(0.6, 0.6, "#sigma_{#DeltaWhad} = 0.34 GeV");
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
   hres_Whad->Modified();
   hres_Whad->SetSelected(hres_Whad);
}
