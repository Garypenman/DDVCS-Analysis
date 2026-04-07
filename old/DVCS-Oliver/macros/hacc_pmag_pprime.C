#ifdef __CLING__
#pragma cling optimize(0)
#endif
void hacc_pmag_pprime()
{
//=========Macro generated from canvas: hacc_pmag_pprime/
//=========  (Fri Feb  6 17:08:48 2026) by ROOT version 6.38.00
   TCanvas *hacc_pmag_pprime = new TCanvas("hacc_pmag_pprime", "", 14, 35, 700, 500);
   gStyle->SetOptFit(0);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   TColor::SetPalette(57, nullptr);
   hacc_pmag_pprime->Range(-46.15385,-0.1445783,338.4615,1.060241);
   hacc_pmag_pprime->SetFillColor(0);
   hacc_pmag_pprime->SetBorderMode(0);
   hacc_pmag_pprime->SetBorderSize(2);
   hacc_pmag_pprime->SetLeftMargin(0.12);
   hacc_pmag_pprime->SetTopMargin(0.05);
   hacc_pmag_pprime->SetBottomMargin(0.12);
   hacc_pmag_pprime->SetFrameBorderMode(0);
   hacc_pmag_pprime->SetFrameBorderMode(0);
   
   TH1D *hacc_pmag_pprime_copy__12 = new TH1D("hacc_pmag_pprime_copy", "", 50, 0, 300);
   hacc_pmag_pprime_copy__12->SetBinContent(7,8);
   hacc_pmag_pprime_copy__12->SetBinContent(8,0.5789473684210527);
   hacc_pmag_pprime_copy__12->SetBinContent(9,0.125);
   hacc_pmag_pprime_copy__12->SetBinContent(10,0.09174311926605505);
   hacc_pmag_pprime_copy__12->SetBinContent(11,0.04186046511627907);
   hacc_pmag_pprime_copy__12->SetBinContent(12,0.6379310344827587);
   hacc_pmag_pprime_copy__12->SetBinContent(13,0.5643648763853367);
   hacc_pmag_pprime_copy__12->SetBinContent(14,0.7098289164709829);
   hacc_pmag_pprime_copy__12->SetBinContent(15,0.5355687291589477);
   hacc_pmag_pprime_copy__12->SetBinContent(16,0.3898349111059801);
   hacc_pmag_pprime_copy__12->SetBinContent(17,0.4313310605062556);
   hacc_pmag_pprime_copy__12->SetMinimum(0);
   hacc_pmag_pprime_copy__12->SetMaximum(1);
   hacc_pmag_pprime_copy__12->SetEntries(12.10641048091365);
   hacc_pmag_pprime_copy__12->SetDirectory(nullptr);
   hacc_pmag_pprime_copy__12->SetStats(0);
   hacc_pmag_pprime_copy__12->SetLineWidth(2);
   hacc_pmag_pprime_copy__12->SetMarkerStyle(7);
   hacc_pmag_pprime_copy__12->SetMarkerSize(1.200000047683716);
   hacc_pmag_pprime_copy__12->GetXaxis()->SetTitle("p_{p'} [GeV/c]");
   hacc_pmag_pprime_copy__12->GetXaxis()->SetLabelFont(42);
   hacc_pmag_pprime_copy__12->GetXaxis()->SetLabelSize(0.04199999943375587);
   hacc_pmag_pprime_copy__12->GetXaxis()->SetTitleSize(0.04500000178813934);
   hacc_pmag_pprime_copy__12->GetXaxis()->SetTitleOffset(1.100000023841858);
   hacc_pmag_pprime_copy__12->GetXaxis()->SetTitleFont(42);
   hacc_pmag_pprime_copy__12->GetYaxis()->SetTitle("Acceptance");
   hacc_pmag_pprime_copy__12->GetYaxis()->SetLabelFont(42);
   hacc_pmag_pprime_copy__12->GetYaxis()->SetLabelSize(0.04199999943375587);
   hacc_pmag_pprime_copy__12->GetYaxis()->SetTitleSize(0.04500000178813934);
   hacc_pmag_pprime_copy__12->GetYaxis()->SetTitleOffset(1.100000023841858);
   hacc_pmag_pprime_copy__12->GetYaxis()->SetTitleFont(42);
   hacc_pmag_pprime_copy__12->GetZaxis()->SetLabelFont(42);
   hacc_pmag_pprime_copy__12->GetZaxis()->SetLabelSize(0.04199999943375587);
   hacc_pmag_pprime_copy__12->GetZaxis()->SetTitleSize(0.04500000178813934);
   hacc_pmag_pprime_copy__12->GetZaxis()->SetTitleOffset(1.100000023841858);
   hacc_pmag_pprime_copy__12->GetZaxis()->SetTitleFont(42);
   hacc_pmag_pprime_copy__12->Draw();
   TLatex *tex = new TLatex(0.15, 0.88, "ePIC Performance");
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
   hacc_pmag_pprime->Modified();
   hacc_pmag_pprime->SetSelected(hacc_pmag_pprime);
}
