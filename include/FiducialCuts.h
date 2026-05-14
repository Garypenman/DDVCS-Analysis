#pragma once

#include <string>
#include <vector>
#include <cmath>
#include <TString.h>


// --------------------------------------------------
// Helper: join cut strings with AND
// --------------------------------------------------
inline std::string JoinCuts(const std::vector<std::string>& cuts)
{
  std::string out;
  for (size_t i = 0; i < cuts.size(); ++i) {
    out += cuts[i];
    if (i + 1 < cuts.size()) out += " && ";
  }
  return out;
}

// --------------------------------------------------
// Fiducial cuts definition
// --------------------------------------------------
struct FiducialCuts {

  // Enable / disable individual cuts
  bool use_p     = false;
  bool use_eta   = false;
  bool use_theta = false;
  bool use_phi   = false;

  // Cut values
  double p_min = 0.0,     p_max = 0.0;
  double eta_min = 0.0,   eta_max = 0.0;
  double theta_min = 0.0, theta_max = 0.0;
  double phi_min = 0.0,   phi_max = 0.0;

  // ------------------------------------------------
  // Make Cut String
  // ------------------------------------------------
  
  std::string MakeCut(const std::string& p_branch,
                      const std::string& eta_branch,
                      const std::string& theta_branch,
                      const std::string& phi_branch)const
  {
    std::vector<std::string> cuts;
    
    if (use_p)
      cuts.emplace_back(
        Form("(%s > %g && %s < %g)",
             p_branch.c_str(), p_min,
             p_branch.c_str(), p_max));

    if (use_eta)
      cuts.emplace_back(
        Form("abs(%s) > %g && abs(%s) < %g",
             eta_branch.c_str(), eta_min,
             eta_branch.c_str(), eta_max));

    if (use_theta)
      cuts.emplace_back(
        Form("(%s > %g && %s < %g)",
             theta_branch.c_str(), theta_min,
             theta_branch.c_str(), theta_max));

    if (use_phi)
      cuts.emplace_back(
        Form("(%s > %g && %s < %g)",
             phi_branch.c_str(), phi_min,
             phi_branch.c_str(), phi_max));

    return JoinCuts(cuts);
  }
};
