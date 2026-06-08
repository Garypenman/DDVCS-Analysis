#include "GetFormFactors.C"

void PrintFormFactors(double q2){

  auto ff = GetFormFactors(q2);
  cout << "Form Factors for -q2 = " << q2 << endl;
  cout << "F1p = " << ff.F1p << " +- " << ff.dF1p << endl;
  cout << "F2p = " << ff.F2p << " +- " << ff.dF2p << endl;
  cout << "F1n = " << ff.F1n << " +- " << ff.dF1n << endl;
  cout << "F2n = " << ff.F2n << " +- " << ff.dF2n << endl;
  cout << endl;
  cout << "GEp = " << ff.GEp << " +- " << ff.dGEp << endl;
  cout << "GMp = " << ff.GMp << " +- " << ff.dGMp << endl;
  cout << "GEn = " << ff.GEn << " +- " << ff.dGEn << endl;
  cout << "GMn = " << ff.GMn << " +- " << ff.dGMn << endl;
  
}
