#pragma once

void DrawUnityLine(TH1* h)
{
  double xmin = h->GetXaxis()->GetXmin();
  double xmax = h->GetXaxis()->GetXmax();

  TLine* l = new TLine(xmin, 1.0, xmax, 1.0);
  l->SetLineColor(kRed);
  l->SetLineStyle(2);
  l->SetLineWidth(2);
  l->Draw("same");
}

std::string FormatValue(double x) {
  return (std::abs(x) >= 0.01)
    ? Form("%.2f", x)
    : Form("%.*f", int(std::ceil(-std::log10(std::abs(x)))) + 1, x);
}
