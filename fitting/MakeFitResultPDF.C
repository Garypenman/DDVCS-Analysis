void MakeFitResultPDF(){

  std::string filename = "fit_out/ResultsBruMcmcCovariance.root";
  TFile *f = new TFile(filename.c_str(),"OPEN");
  
  TCanvas *pad_theta = (TCanvas*) f->Get("_mc_ThetaHel");
  TCanvas *pad_phi = (TCanvas*) f->Get("_mc_PhiHel");
  TCanvas *pad_pol = (TCanvas*) f->Get("_mc_GammaPolCirc");
  TCanvas *pad_qp = (TCanvas*) f->Get("_mc_Qp");
  TCanvas *pad_s = (TCanvas*) f->Get("_mc_s_photo");
  TCanvas *pad_t = (TCanvas*) f->Get("_mc_t_bot");
  TCanvas *pad_deltaT = (TCanvas*) f->Get("_mc_DeltaT");

  pad_theta->Draw();
  gPad->Print("temp00.pdf");
  pad_phi->Draw();
  gPad->Print("temp01.pdf");
  pad_pol->Draw();
  gPad->Print("temp02.pdf");
  pad_qp->Draw();
  gPad->Print("temp03.pdf");
  pad_s->Draw();
  gPad->Print("temp04.pdf");
  pad_t->Draw();
  gPad->Print("temp05.pdf");
  pad_deltaT->Draw();
  gPad->Print("temp06.pdf");
  
  gSystem->Exec("pdfunite temp*.pdf TCSFitResults.pdf");
  gSystem->Exec("rm temp*.pdf");
  gApplication->Terminate();
}
