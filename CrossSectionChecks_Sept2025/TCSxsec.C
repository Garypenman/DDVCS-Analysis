#include "/home/garyp/eic/epic_plot_macro/ePIC_style.C"
#include "/home/garyp/eic/EPICStyle/EPICStyle.C"

void TCSxsec(){
  
  set_ePIC_style(); //official epic
  //SetEPICStyle(); //my version
  //style corrections needed for official epic
  gStyle->SetOptTitle(0);
  gStyle->SetOptFit(0);
  gStyle->SetPadRightMargin(0.1);

  double tcs_sig[10] = {5.64, 4.83, 5.03, 6.60, 4.97, 5.95, 6.32, 5.64, 6.43, 4.68};
  double tcs_unc[10] = {0.85, 0.61, 0.26, 1.29, 0.30, 0.52, 0.75, 0.66, 2.21, 0.36};
  
  double tcs_safe_sig[10] = {0.95, 0.87, 0.91, 0.88, 0.85, 1.00, 0.82, 1.03, 0.89, 0.89};
  double tcs_safe_unc[10] = {0.03, 0.02, 0.03, 0.02, 0.02, 0.08, 0.02, 0.09, 0.02, 0.02};
  
  double ddvcs_sig[10] = {5.24, 4.83, 4.81, 4.68, 4.44, 5.07, 5.47, 4.71, 4.59, 4.63};
  double ddvcs_unc[10] = {0.85, 0.38, 0.30, 0.40, 0.25, 0.52, 1.28, 0.47, 0.25, 0.24};
  
  double ddvcs_safe_sig[10] = {0.87, 0.87, 0.87, 0.93, 0.91, 0.92, 0.96, 0.88, 0.94, 0.91};
  double ddvcs_safe_unc[10] = {0.02, 0.03, 0.02, 0.04, 0.08, 0.04, 0.12, 0.03, 0.09, 0.05};
  
  double ntest[10] = {1., 2., 3., 4., 5., 6., 7., 8., 9., 10.};
  double entest[10] = {0.0};
  
  
  ///DDVCS
  TGraphErrors *g_ddvcs = new TGraphErrors(10,ntest,ddvcs_sig,entest,ddvcs_unc);
  TGraphErrors *g_ddvcs_safe = new TGraphErrors(10,ntest,ddvcs_safe_sig,entest,ddvcs_safe_unc);
  
  g_ddvcs->SetTitle("DDVCS Full Kinematic Range;Test Number;Integrated Cross Section [nb]");
  g_ddvcs->SetMarkerStyle(8);
  g_ddvcs->SetMarkerColor(kRed);
  
  g_ddvcs_safe->SetTitle("DDVCS #pi/6 < #theta_{l} < 5#pi/6;Test Number;Integrated Cross Section [nb]");
  g_ddvcs_safe->SetMinimum(0);
  g_ddvcs_safe->SetMaximum(2);
  g_ddvcs_safe->SetMarkerStyle(8);
  g_ddvcs_safe->SetMarkerColor(kBlue);
  
  new TCanvas();
  TMultiGraph *mg_ddvcs = new TMultiGraph();
  mg_ddvcs->SetMinimum(0);
  mg_ddvcs->SetMaximum(10);
  mg_ddvcs->SetTitle("Testing DDVCS Foam Stability;Test Number;Integrated Cross Section [nb]");
  mg_ddvcs->Add(g_ddvcs);
  mg_ddvcs->Add(g_ddvcs_safe);
  mg_ddvcs->Draw("ap");
  TLegend *l_ddvcs = new TLegend(0.15,0.8,0.3,0.9);
  l_ddvcs->AddEntry(g_ddvcs);
  l_ddvcs->AddEntry(g_ddvcs_safe);
  l_ddvcs->Draw();
  gPad->Print("mg_ddvcs.png");
  
  new TCanvas();
  g_ddvcs_safe->Draw("ap");
  gPad->Print("g_ddvcs_safe.png");
  
  ///TCS
  TGraphErrors *g_tcs = new TGraphErrors(10,ntest,tcs_sig,entest,tcs_unc);
  TGraphErrors *g_tcs_safe = new TGraphErrors(10,ntest,tcs_safe_sig,entest,tcs_safe_unc);
  
  g_tcs->SetTitle("TCS Full Kinematic Range;Test Number;Integrated Cross Section [nb]");
  g_tcs->SetMarkerStyle(8);
  g_tcs->SetMarkerColor(kRed);
  
  g_tcs_safe->SetTitle("TCS #pi/6 < #theta_{l} < 5#pi/6;Test Number;Integrated Cross Section [nb]");
  g_tcs_safe->SetMinimum(0);
  g_tcs_safe->SetMaximum(2);
  g_tcs_safe->SetMarkerStyle(8);
  g_tcs_safe->SetMarkerColor(kBlue);
  
  new TCanvas();
  TMultiGraph *mg_tcs = new TMultiGraph();
  mg_tcs->SetMinimum(0);
  mg_tcs->SetMaximum(10);
  mg_tcs->SetTitle("Testing TCS Foam Stability;Test Number;Integrated Cross Section [nb]");
  mg_tcs->Add(g_tcs);
  mg_tcs->Add(g_tcs_safe);
  mg_tcs->Draw("ap");
  TLegend *l_tcs = new TLegend(0.15,0.8,0.3,0.9);
  l_tcs->AddEntry(g_tcs);
  l_tcs->AddEntry(g_tcs_safe);
  l_tcs->Draw();
  gPad->Print("mc_tcs.png");

  new TCanvas();
  g_tcs_safe->Draw("ap");
  gPad->Print("g_tcs_safe.png");
  
}
    
