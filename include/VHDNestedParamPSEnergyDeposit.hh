#ifndef VHDNestedParamPSEnergyDeposit_h
#define VHDNestedParamPSEnergyDeposit_h 1

#include "G4PSEnergyDeposit.hh"

//G4PSEnergyDeposit is the derived class of G4VPrimitiveScorer
class VHDNestedParamPSEnergyDeposit : public G4PSEnergyDeposit
{
   public: // with description
      VHDNestedParamPSEnergyDeposit(G4String name,G4int nx,G4int ny, G4int nz);
      virtual ~VHDNestedParamPSEnergyDeposit();

  protected: // with description
      G4int GetIndex(G4Step*);

  private:
      G4int fNx, fNy, fNz, fNxNy;
};
#endif

