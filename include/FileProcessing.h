#pragma once

void MakeOutputPDF(const std::string outfile){
  TString pdfoutfile = outfile;
  pdfoutfile = gSystem->BaseName(pdfoutfile);
  pdfoutfile.ReplaceAll(".root",".pdf");
  
  gSystem->Exec(Form("pdfunite temp*.pdf %s",pdfoutfile.Data()));
  gSystem->Exec("rm temp*.pdf");
  
}
