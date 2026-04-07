#ifdef __CLING__
#pragma cling optimize(0)
#endif
void hres_TrentoPhi_semi_excl()
{
//=========Macro generated from canvas: hres_TrentoPhi_semi_excl/
//=========  (Fri Feb  6 17:08:49 2026) by ROOT version 6.38.00
   TCanvas *hres_TrentoPhi_semi_excl = new TCanvas("hres_TrentoPhi_semi_excl", "", 0, 0, 700, 500);
   gStyle->SetOptFit(0);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   TColor::SetPalette(57, nullptr);
   hres_TrentoPhi_semi_excl->Range(-1.307692,-0.1518072,1.25641,1.113253);
   hres_TrentoPhi_semi_excl->SetFillColor(0);
   hres_TrentoPhi_semi_excl->SetBorderMode(0);
   hres_TrentoPhi_semi_excl->SetBorderSize(2);
   hres_TrentoPhi_semi_excl->SetLeftMargin(0.12);
   hres_TrentoPhi_semi_excl->SetTopMargin(0.05);
   hres_TrentoPhi_semi_excl->SetBottomMargin(0.12);
   hres_TrentoPhi_semi_excl->SetFrameBorderMode(0);
   hres_TrentoPhi_semi_excl->SetFrameBorderMode(0);
   
   TH1D *hres_TrentoPhi_semi_excl_copy__27 = new TH1D("hres_TrentoPhi_semi_excl_copy", "", 100, -1, 1);
   hres_TrentoPhi_semi_excl_copy__27->SetBinContent(101,514843);
   hres_TrentoPhi_semi_excl_copy__27->SetEntries(514843);
   hres_TrentoPhi_semi_excl_copy__27->SetDirectory(nullptr);
   hres_TrentoPhi_semi_excl_copy__27->SetStats(0);
   hres_TrentoPhi_semi_excl_copy__27->SetLineWidth(2);
   hres_TrentoPhi_semi_excl_copy__27->SetMarkerStyle(20);
   hres_TrentoPhi_semi_excl_copy__27->SetMarkerSize(1.200000047683716);
   hres_TrentoPhi_semi_excl_copy__27->GetXaxis()->SetTitle("#phi_{l} [rad]");
   hres_TrentoPhi_semi_excl_copy__27->GetXaxis()->SetLabelFont(42);
   hres_TrentoPhi_semi_excl_copy__27->GetXaxis()->SetLabelSize(0.04199999943375587);
   hres_TrentoPhi_semi_excl_copy__27->GetXaxis()->SetTitleSize(0.04500000178813934);
   hres_TrentoPhi_semi_excl_copy__27->GetXaxis()->SetTitleOffset(1.100000023841858);
   hres_TrentoPhi_semi_excl_copy__27->GetXaxis()->SetTitleFont(42);
   hres_TrentoPhi_semi_excl_copy__27->GetYaxis()->SetTitle("N_{evts}");
   hres_TrentoPhi_semi_excl_copy__27->GetYaxis()->SetLabelFont(42);
   hres_TrentoPhi_semi_excl_copy__27->GetYaxis()->SetLabelSize(0.04199999943375587);
   hres_TrentoPhi_semi_excl_copy__27->GetYaxis()->SetTitleSize(0.04500000178813934);
   hres_TrentoPhi_semi_excl_copy__27->GetYaxis()->SetTitleOffset(1.100000023841858);
   hres_TrentoPhi_semi_excl_copy__27->GetYaxis()->SetTitleFont(42);
   hres_TrentoPhi_semi_excl_copy__27->GetZaxis()->SetLabelFont(42);
   hres_TrentoPhi_semi_excl_copy__27->GetZaxis()->SetLabelSize(0.04199999943375587);
   hres_TrentoPhi_semi_excl_copy__27->GetZaxis()->SetTitleSize(0.04500000178813934);
   hres_TrentoPhi_semi_excl_copy__27->GetZaxis()->SetTitleOffset(1.100000023841858);
   hres_TrentoPhi_semi_excl_copy__27->GetZaxis()->SetTitleFont(42);
   hres_TrentoPhi_semi_excl_copy__27->Draw();
   TLatex *tex = new TLatex(0.6, 0.6, "#sigma_{#Delta#phi_{l}} = 0.02 rad");
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
   hres_TrentoPhi_semi_excl->Modified();
   hres_TrentoPhi_semi_excl->SetSelected(hres_TrentoPhi_semi_excl);
}
