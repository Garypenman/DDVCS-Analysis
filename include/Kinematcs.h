#pragma once

#include <ROOT/RDataFrame.hxx>
#include <Math/Vector4D.h>
#include <cmath>
#include <string>

namespace Kin {

  using Vec4 = ROOT::Math::PxPyPzEVector;

  // --------------------------------------------------
  // Beam four-vectors
  // --------------------------------------------------
  inline Vec4 BeamElectron(double E = 18.0)
  {
    return Vec4(0.0, 0.0, -E, E);
  }

  inline Vec4 BeamProton(double p = 275.0)
  {
    constexpr double mp = 0.938272;
    double E = std::sqrt(p*p + mp*mp);
    return Vec4(0.0, 0.0, p, E);
  }

  // --------------------------------------------------
  // Q^2 from scattered electron
  // --------------------------------------------------
  inline ROOT::RDF::RNode
  DefineQ2(ROOT::RDF::RNode df,
           const std::string& px,
           const std::string& py,
           const std::string& pz,
           const std::string& E)
  {
    const auto kBeam = BeamElectron();

    return df.Define(
      "Q2",
      [kBeam](double px, double py, double pz, double E) {
        Vec4 k(px, py, pz, E);
        auto q = kBeam - k;
        return -q.M2();
      },
      {px, py, pz, E}
    );
  }

  // --------------------------------------------------
  // |t| from recoil proton
  // --------------------------------------------------
  inline ROOT::RDF::RNode
  DefineT(ROOT::RDF::RNode df,
          const std::string& px,
          const std::string& py,
          const std::string& pz,
          const std::string& E)
  {
    const auto pBeam = BeamProton();

    return df.Define(
      "t",
      [pBeam](double px, double py, double pz, double E) {
        Vec4 p(px, py, pz, E);
        auto tvec = p - pBeam;
        return std::abs(tvec.M2());
      },
      {px, py, pz, E}
    );
  }

} // namespace Kin
