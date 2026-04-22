#pragma once

#include <string>
#include <vector>
#include <cmath>
#include <TString.h>

// Forward declaration (avoids circular include)
struct ParticleConfig;

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
  // Truth-level cut string
  // ------------------------------------------------
  std::string TruthCut(const ParticleConfig& pc) const
  {
    std::vector<std::string> cuts;

    if (use_p)
      cuts.emplace_back(
        Form("(%s > %g && %s < %g)",
             pc.tru_p.c_str(), p_min,
             pc.tru_p.c_str(), p_max));

    if (use_eta)
      cuts.emplace_back(
        Form("abs(%s) > %g && abs(%s) < %g",
             pc.tru_eta.c_str(), eta_min,
             pc.tru_eta.c_str(), eta_max));

    if (use_theta)
      cuts.emplace_back(
        Form("(%s > %g && %s < %g)",
             pc.tru_theta.c_str(), theta_min,
             pc.tru_theta.c_str(), theta_max));

    if (use_phi)
      cuts.emplace_back(
        Form("(%s > %g && %s < %g)",
             pc.tru_phi.c_str(), phi_min,
             pc.tru_phi.c_str(), phi_max));

    return JoinCuts(cuts);
  }

  // ------------------------------------------------
  // Reco-level cut string
  // ------------------------------------------------
  std::string RecoCut(const ParticleConfig& pc) const
  {
    std::vector<std::string> cuts;

    if (use_p)
      cuts.emplace_back(
        Form("(%s > %g && %s < %g)",
             pc.rec_p.c_str(), p_min,
             pc.rec_p.c_str(), p_max));

    if (use_eta)
      cuts.emplace_back(
        Form("abs(%s) > %g && abs(%s) < %g",
             pc.rec_eta.c_str(), eta_min,
             pc.rec_eta.c_str(), eta_max));

    if (use_theta)
      cuts.emplace_back(
        Form("(%s > %g && %s < %g)",
             pc.rec_theta.c_str(), theta_min,
             pc.rec_theta.c_str(), theta_max));

    if (use_phi)
      cuts.emplace_back(
        Form("(%s > %g && %s < %g)",
             pc.rec_phi.c_str(), phi_min,
             pc.rec_phi.c_str(), phi_max));

    return JoinCuts(cuts);
  }
};
