#ifdef __CLING__
#pragma cling optimize(0)
#endif
void hres_Q2()
{
//=========Macro generated from canvas: hres_Q2/
//=========  (Fri Feb  6 17:08:49 2026) by ROOT version 6.38.00
   TCanvas *hres_Q2 = new TCanvas("hres_Q2", "", 0, 0, 700, 500);
   gStyle->SetOptFit(0);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   TColor::SetPalette(57, nullptr);
   hres_Q2->Range(-0.001307692,79.55157,0.00125641,171.2052);
   hres_Q2->SetFillColor(0);
   hres_Q2->SetBorderMode(0);
   hres_Q2->SetBorderSize(2);
   hres_Q2->SetLeftMargin(0.12);
   hres_Q2->SetTopMargin(0.05);
   hres_Q2->SetBottomMargin(0.12);
   hres_Q2->SetFrameBorderMode(0);
   hres_Q2->SetFrameBorderMode(0);
   
   TH1D *hres_Q2_copy__20 = new TH1D("hres_Q2_copy", "", 100, -0.001, 0.001);
   std::vector<Double_t> hres_Q2_copy__20_vect1{
      455656, 142, 143, 131, 116, 163, 135, 142, 150, 112,
      135, 120, 108, 118, 115, 131, 116, 112, 128, 137,
      115, 113, 138, 114, 128, 138, 126, 135, 104, 136,
      125, 146, 132, 132, 127, 127, 139, 121, 127, 129,
      94, 127, 124, 127, 135, 140, 123, 129, 126, 124,
      131, 132, 138, 119, 119, 118, 118, 144, 112, 118,
      131, 121, 143, 153, 125, 132, 109, 127, 132, 129,
      129, 135, 132, 130, 129, 132, 135, 125, 136, 146,
      134, 134, 139, 133, 114, 135, 141, 116, 129, 134,
      130, 127, 109, 109, 122, 127, 154, 119, 134, 119,
      119, 620173
   };
   for (Int_t bin = 0; bin < 102; bin++)
      if (hres_Q2_copy__20_vect1[bin])
         hres_Q2_copy__20->SetBinContent(bin, hres_Q2_copy__20_vect1[bin]);
   hres_Q2_copy__20->SetEntries(1088642);
   hres_Q2_copy__20->SetDirectory(nullptr);
   hres_Q2_copy__20->SetStats(0);
   hres_Q2_copy__20->SetLineWidth(2);
   hres_Q2_copy__20->SetMarkerStyle(20);
   hres_Q2_copy__20->SetMarkerSize(1.200000047683716);
   hres_Q2_copy__20->GetXaxis()->SetTitle("#DeltaQ^{2} [GeV^{2}]");
   hres_Q2_copy__20->GetXaxis()->SetLabelFont(42);
   hres_Q2_copy__20->GetXaxis()->SetLabelSize(0.04199999943375587);
   hres_Q2_copy__20->GetXaxis()->SetTitleSize(0.04500000178813934);
   hres_Q2_copy__20->GetXaxis()->SetTitleOffset(1.100000023841858);
   hres_Q2_copy__20->GetXaxis()->SetTitleFont(42);
   hres_Q2_copy__20->GetYaxis()->SetTitle("N_{evts}");
   hres_Q2_copy__20->GetYaxis()->SetLabelFont(42);
   hres_Q2_copy__20->GetYaxis()->SetLabelSize(0.04199999943375587);
   hres_Q2_copy__20->GetYaxis()->SetTitleSize(0.04500000178813934);
   hres_Q2_copy__20->GetYaxis()->SetTitleOffset(1.100000023841858);
   hres_Q2_copy__20->GetYaxis()->SetTitleFont(42);
   hres_Q2_copy__20->GetZaxis()->SetLabelFont(42);
   hres_Q2_copy__20->GetZaxis()->SetLabelSize(0.04199999943375587);
   hres_Q2_copy__20->GetZaxis()->SetTitleSize(0.04500000178813934);
   hres_Q2_copy__20->GetZaxis()->SetTitleOffset(1.100000023841858);
   hres_Q2_copy__20->GetZaxis()->SetTitleFont(42);
   hres_Q2_copy__20->Draw();
   TLatex *tex = new TLatex(0.6, 0.6, "#sigma_{#DeltaQ^{2}} = 101e-5 GeV^{2}");
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
   hres_Q2->Modified();
   hres_Q2->SetSelected(hres_Q2);
}
