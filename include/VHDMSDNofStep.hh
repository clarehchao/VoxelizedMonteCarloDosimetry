#ifndef VHDMSDNofStep_h
#define VHDMSDNofStep_h 1

#include "G4PSNofStep.hh"

//G4PSEnergyDeposit is the derived class of G4VPrimitiveScorer

class VHDMSDNofStep : public G4PSNofStep
{
   public: // with description
      VHDMSDNofStep(G4String name,G4int nx,G4int ny, G4int nz);
      virtual ~VHDMSDNofStep();

  protected: // with description
      G4int GetIndex(G4Step*);

  private:
      G4int fNx, fNy, fNz, fNxNy;
};
#endif

