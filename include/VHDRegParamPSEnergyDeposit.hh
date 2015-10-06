#ifndef VHDRegParamPSEnergyDeposit_h
#define VHDRegParamPSEnergyDeposit_h 1

#include "G4PSEnergyDeposit.hh"

//VHDRegParamPSEnergyDeposit is the derived class of G4VPrimitiveScorer
class VHDRegParamPSEnergyDeposit : public G4PSEnergyDeposit
{
   public: // with description
      VHDRegParamPSEnergyDeposit(G4String name,G4int nx,G4int ny, G4int nz);
      virtual ~VHDRegParamPSEnergyDeposit();

  private:
      G4int fNx, fNy, fNz, fNxNy;
};
#endif

