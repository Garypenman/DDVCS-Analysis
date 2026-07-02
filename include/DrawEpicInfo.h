#pragma once

void DrawEpicText(){

  TLatex text_ePIC;
   text_ePIC.SetTextSize(0.05);
   text_ePIC.SetTextFont(62);
   text_ePIC.DrawLatexNDC(.15,.88,"ePIC Performance");  // performance plot
   //text_ePIC.DrawLatexNDC(.15,.88,"ePIC Internal");  // for internal use only
   //text_ePIC.DrawLatexNDC(.15,.88,"ePIC Preliminary"); // preliminary released version 
   //text_ePIC.DrawLatexNDC(.15,.88,"ePIC Work in Progress"); // work in progress to be shown outside
   //text_ePIC.DrawLatexNDC(.15,.88,"ePIC"); // final published version
   //return text_ePIC;

}

void DrawEpicCOMText(int Ecom){

  TLatex text_com;
  text_com.SetTextAlign(13);  //align at top
  text_com.SetTextSize(0.04);
  text_com.SetTextFont(42);
  text_com.DrawLatexNDC(.15,.85,Form("e+p, #sqrt{s} = %d GeV",Ecom));
}

void DrawEpicEnergyText(string Eset){

  TLatex tex;
  tex.SetTextAlign(13);  //align at top
  tex.SetTextSize(0.04);
  tex.SetTextFont(42);
  tex.DrawLatexNDC(.15,.85,Form("e+p, %s GeV",Eset.c_str()));

}

void DrawLumiText(int lumi = 10){
  
  TLatex tex;
  tex.SetTextAlign(13);  //align at top
  tex.SetTextSize(0.04);
  tex.SetTextFont(42);
  tex.DrawLatexNDC(.15,.8,Form("L_{proj} = %d fb^{-1}",lumi));
  
}

void DrawSimText(std::string campaign = "XX.XX.X", double xpos=0.65, double ypos=1.0){
  
  TLatex tex;
  tex.SetTextAlign(13);
  tex.SetTextSize(0.04);
  tex.SetTextFont(42);
  tex.DrawLatexNDC(xpos,ypos-0.08,Form("%s Campaign",campaign.c_str()));
  tex.DrawLatexNDC(xpos,ypos-0.12,"EpIC 1.1.8");
  tex.DrawLatexNDC(xpos,ypos-0.16,"EpIC ep #rightarrow e'e^{+}e^{-}p");
  
}

void DrawEPIC(){

  // ===== Add ePIC logo to the figure if desired ======
  TImage *logo = TImage::Open("/../epic_plot_macro/EPIC-logo_black_small.png");
  TPad *pad2 = new TPad("pad2", "Pad 2", 0.8, 0.8, 0.93, 0.93); // Create a new pad and then draw the image in it
  pad2->Draw();
  pad2->cd(); // Enter the new pad
  logo->Draw();
}
