#pragma once

#include <TSystem.h>
#include <TSystemDirectory.h>
#include <TSystemFile.h>
#include <TRegexp.h>

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



std::vector<std::string> GlobToVector(const std::string& pattern)
{
    std::vector<std::string> out;

    // Split into dir + basename
    std::string dirname  = gSystem->DirName(pattern.c_str());
    std::string basename = gSystem->BaseName(pattern.c_str());

    // Convert glob ? regex
    TString regex = basename.c_str();
    regex.ReplaceAll(".", "\\.");   // escape literal dots
    regex.ReplaceAll("*", ".*");    // * ? .*
    regex.ReplaceAll("?", ".");     // ? ? .
    TRegexp reg(regex);

    // Scan directory
    TSystemDirectory dir(dirname.c_str(), dirname.c_str());
    auto files = dir.GetListOfFiles();
    if (!files) return out;

    TIter next(files);
    while (auto* obj = next()) {
        auto* sf = dynamic_cast<TSystemFile*>(obj);
        if (!sf || sf->IsDirectory()) continue;

        TString fname = sf->GetName();
        if (fname.Index(reg) != kNPOS) {
            out.push_back(dirname + "/" + fname.Data());
        }
    }

    return out;
}
