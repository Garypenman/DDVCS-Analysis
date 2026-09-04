#pragma once

const std::vector<EColor> kp6 = {kP6Violet, kP6Gray, kP6Grape, kP6Red, kP6Yellow, kP6Blue};

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

void DrawLine(TH1 *h, double x,
              Color_t color = kGreen+2,
              Style_t style = 2,
              Width_t width = 2)
{
  double ymax = h->GetMaximum();

  TLine *line = new TLine(x, 0, x, ymax);

  line->SetLineColor(color);
  line->SetLineStyle(style);
  line->SetLineWidth(width);

  line->Draw();
}

std::string FormatValue(double x) {
  return (std::abs(x) >= 0.01)
    ? Form("%.2f", x)
    : Form("%.*f", int(std::ceil(-std::log10(std::abs(x)))) + 1, x);
}

void DrawOneSplitTwo(TH1D* all, TH1D* h1, TH1D* h2){
  
  all->SetLineColor(kBlack);
  //h1->SetLineColor(0);
  h1->SetFillColorAlpha(kp6[0],0.8);
  h1->SetFillStyle(3004);
  //h2->SetLineColor(0);
  h2->SetFillColorAlpha(kp6[1],0.8);
  h2->SetFillStyle(3005);
  //all->SetMaximum(1.25*all->GetMaximum());
  all->DrawCopy("hist p");
  h1->DrawCopy("same");
  h2->DrawCopy("same");
  
}


TH1D* AcceptancePlot(TH1D* tru, TH1D* rec, std::string name){

  TH1D* acc = (TH1D*)rec->Clone(name.c_str());
  acc->Divide(tru);
  acc->SetMinimum(0);
  acc->SetMaximum(1);
  acc->GetYaxis()->SetTitle("Acceptance");
  acc->SetMarkerStyle(7);
  return acc;
  
}


TH1D* XSecPlot(TH1D* h, double scale, std::string name, TH1D* acc=nullptr){

  TH1D* sig = (TH1D*)h->Clone(name.c_str());
  if(!(acc==nullptr)) sig->Divide(acc);
  sig->Scale(scale,"width");
  
  return sig;

}

void DrawCutLine(const std::string& cut_expr,
                 double ymin, double ymax)
{
  // Extract number from expression (simple parsing)
  double cut_val = 0;

  if (cut_expr.find("<") != std::string::npos) {
    cut_val = std::stod(cut_expr.substr(cut_expr.find("<") + 1));
  }

  // +cut
  TLine *line1 = new TLine(cut_val, ymin, cut_val, ymax);
  line1->SetLineColor(kGreen+2);
  line1->SetLineWidth(2);
  line1->SetLineStyle(2);
  line1->Draw();

  // -cut (for abs variables)
  if (cut_expr.find("abs") != std::string::npos) {
    TLine *line2 = new TLine(-cut_val, ymin, -cut_val, ymax);
    line2->SetLineColor(kGreen+2);
    line2->SetLineWidth(2);
    line2->SetLineStyle(2);
    line2->Draw();
  }
}
