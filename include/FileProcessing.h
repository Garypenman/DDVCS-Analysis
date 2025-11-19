#pragma once

void MakeOutputPDF(const std::string outfile){
  TString pdfoutfile = outfile;
  pdfoutfile = gSystem->BaseName(pdfoutfile);
  pdfoutfile.ReplaceAll(".root",".pdf");
  
  gSystem->Exec(Form("pdfunite temp*.pdf %s",pdfoutfile.Data()));
  gSystem->Exec("rm temp*.pdf");
  
}

bool checkFileExists(const std::string &filename) {
  if (gSystem->AccessPathName(filename.c_str())) {
        std::cout << "File does NOT exist: " << filename << std::endl;
	return false;
    } else {
        std::cout << "File exists: " << filename << std::endl;
	return true;
    }
}
