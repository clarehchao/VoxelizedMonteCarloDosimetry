#ifndef NestedParamVHDDetectorConstruction_h
#define NestedParamVHDDetectorConstruction_h 1

#include "globals.hh"
#include "VHDDetectorConstruction.hh"
#include "VHDNestedPhantomParameterisation.hh"


class NestedParamVHDDetectorConstruction : public VHDDetectorConstruction
{
public:

  NestedParamVHDDetectorConstruction();
  virtual ~NestedParamVHDDetectorConstruction();

protected:

  virtual void ConstructPhantom();
  virtual void SetMultiSensDet(G4LogicalVolume* voxel_logic);
  VHDNestedPhantomParameterisation* param;

};

#endif

