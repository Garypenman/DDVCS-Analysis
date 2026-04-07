#ifdef __CLING__
#pragma cling optimize(0)
#endif
void hacc_theta_scatele()
{
//=========Macro generated from canvas: hacc_theta_scatele/
//=========  (Fri Feb  6 17:08:48 2026) by ROOT version 6.38.00
   TCanvas *hacc_theta_scatele = new TCanvas("hacc_theta_scatele", "", 0, 0, 700, 500);
   gStyle->SetOptFit(0);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   TColor::SetPalette(57, nullptr);
   hacc_theta_scatele->Range(-0.483322,-0.1445783,3.544361,1.060241);
   hacc_theta_scatele->SetFillColor(0);
   hacc_theta_scatele->SetBorderMode(0);
   hacc_theta_scatele->SetBorderSize(2);
   hacc_theta_scatele->SetLeftMargin(0.12);
   hacc_theta_scatele->SetTopMargin(0.05);
   hacc_theta_scatele->SetBottomMargin(0.12);
   hacc_theta_scatele->SetFrameBorderMode(0);
   hacc_theta_scatele->SetFrameBorderMode(0);
   
   TH1D *hacc_theta_scatele_copy__9 = new TH1D("hacc_theta_scatele_copy", "", 50, 0, 3.141592653589793);
   hacc_theta_scatele_copy__9->SetMinimum(0);
   hacc_theta_scatele_copy__9->SetMaximum(1);
   hacc_theta_scatele_copy__9->SetDirectory(nullptr);
   hacc_theta_scatele_copy__9->SetStats(0);
   hacc_theta_scatele_copy__9->SetLineWidth(2);
   hacc_theta_scatele_copy__9->SetMarkerStyle(7);
   hacc_theta_scatele_copy__9->SetMarkerSize(1.200000047683716);
   hacc_theta_scatele_copy__9->GetXaxis()->SetTitle("#theta_{e} [rad]");
   hacc_theta_scatele_copy__9->GetXaxis()->SetLabelFont(42);
   hacc_theta_scatele_copy__9->GetXaxis()->SetLabelSize(0.04199999943375587);
   hacc_theta_scatele_copy__9->GetXaxis()->SetTitleSize(0.04500000178813934);
   hacc_theta_scatele_copy__9->GetXaxis()->SetTitleOffset(1.100000023841858);
   hacc_theta_scatele_copy__9->GetXaxis()->SetTitleFont(42);
   hacc_theta_scatele_copy__9->GetYaxis()->SetTitle("Acceptance");
   hacc_theta_scatele_copy__9->GetYaxis()->SetLabelFont(42);
   hacc_theta_scatele_copy__9->GetYaxis()->SetLabelSize(0.04199999943375587);
   hacc_theta_scatele_copy__9->GetYaxis()->SetTitleSize(0.04500000178813934);
   hacc_theta_scatele_copy__9->GetYaxis()->SetTitleOffset(1.100000023841858);
   hacc_theta_scatele_copy__9->GetYaxis()->SetTitleFont(42);
   hacc_theta_scatele_copy__9->GetZaxis()->SetLabelFont(42);
   hacc_theta_scatele_copy__9->GetZaxis()->SetLabelSize(0.04199999943375587);
   hacc_theta_scatele_copy__9->GetZaxis()->SetTitleSize(0.04500000178813934);
   hacc_theta_scatele_copy__9->GetZaxis()->SetTitleOffset(1.100000023841858);
   hacc_theta_scatele_copy__9->GetZaxis()->SetTitleFont(42);
   hacc_theta_scatele_copy__9->Draw();
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
   hacc_theta_scatele->Modified();
   hacc_theta_scatele->SetSelected(hacc_theta_scatele);
}
