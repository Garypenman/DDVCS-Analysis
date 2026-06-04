#pragma once

#include <string>
#include <TString.h>

struct DerivedObservable {
 
  std::string name;     // "Q2", "t"
  std::string column;   // "Q2", "t_bot"
  std::string label;    // axis label
  
  double xmin, xmax;
  double xmin_res, xmax_res;
  
  DerivedObservable(std::string fname,
		    std::string fcol,
		    std::string flabel,
		    double fxmin, double fxmax,
		    double fxminr, double fxmaxr
		    )
    : name(fname), column(fcol), label(flabel),
      xmin(fxmin), xmax(fxmax),
      xmin_res(fxminr), xmax_res(fxmaxr)
  {
    
  }
};
