#ifndef DetectorContFactory_h
#define DetectorContFactory_h 1

#include "VHDDetectorConstruction.hh"
#include "NestedParamVHDDetectorConstruction.hh"
#include "RegularVHDDetectorConstruction.hh"


class DetectorContFactory
{

  public:
    DetectorContFactory();
    ~DetectorContFactory();
    VHDDetectorConstruction* GetDetectorConstr(G4String& type);

};


#endif

