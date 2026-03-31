#pragma once

#include "ReactionKinematics.h"
#include "CommonDefines.h" // Ensures RVecResultType is defined
#include "TMath.h"
#include <algorithm> // For std::clamp

namespace rad {
  namespace physics {
    
    /**
     * @brief Calculates the electroproduction mandelstam s = (k + p)^2.
     * @param react The fixed ReactionMap.
     * @param px, py, pz, m The consolidated momentum component vectors.
     * @return ResultType_t The s value (always positive).
     */
    inline ResultType_t ElS_s_electro(const RVecIndexMap& react, 
			       const RVecResultType& px, const RVecResultType& py, 
			       const RVecResultType& pz, const RVecResultType& m) 
    {
      auto ion = FourVector(react[consts::OrderBeams()][consts::OrderBeamIon()], px, py, pz, m); 
      auto ebeam = FourVector(react[consts::OrderBeams()][consts::OrderBeamEle()], px, py, pz, m);
      return (ebeam+ion).M2();
    }
    
    /**
     * @brief Calculates the photoproduction s = W^2 = (p + q)^2.
     * @param react The fixed ReactionMap.
     * @param px, py, pz, m The consolidated momentum component vectors.
     * @return ResultType_t The s value (always positive).
     */
    inline ResultType_t ElS_s_photo(const RVecIndexMap& react, 
			       const RVecResultType& px, const RVecResultType& py, 
				    const RVecResultType& pz, const RVecResultType& m) 
    {
      auto ion = FourVector(react[consts::OrderBeams()][consts::OrderBeamIon()], px, py, pz, m); 
      auto phot = PhotoFourVector(react, px, py, pz, m);
      return (ion+phot).M2();
    }
    
    /**
     * @brief Calculates the transverse component of momentum transfer to the baryon (top vertex) deltaT = (q-qp).Perp.
     * @param react The fixed ReactionMap.
     * @param px, py, pz, m The consolidated momentum component vectors.
     * @return ResultType_t The deltaT value (always positive).
     */
    inline ResultType_t DeltaTTop(const RVecIndexMap& react, 
				       const RVecResultType& px, const RVecResultType& py, 
				       const RVecResultType& pz, const RVecResultType& m) 
    {
      auto phot = PhotoFourVector(react, px, py, pz, m);
      auto mes = MesonFourVector(react, px, py, pz, m);
      return sqrt((phot-mes).Perp2());
    }
    
    /**
     * @brief Calculates the transverse component of momentum transfer to the baryon (bottom vertex) deltaT = (p-pp).Perp.
     * @param react The fixed ReactionMap.
     * @param px, py, pz, m The consolidated momentum component vectors.
     * @return ResultType_t The deltaT value (always positive).
     */
    inline ResultType_t DeltaTBot(const RVecIndexMap& react, 
			       const RVecResultType& px, const RVecResultType& py, 
				    const RVecResultType& pz, const RVecResultType& m) 
    {
      auto ion = FourVector(react[consts::OrderBeams()][consts::OrderBeamIon()], px, py, pz, m); 
      auto baryon = FourVector(react[consts::OrderBaryons()], px, py, pz, m);
      return sqrt((ion-baryon).Perp2());
    }
    
    
    
  }
}
