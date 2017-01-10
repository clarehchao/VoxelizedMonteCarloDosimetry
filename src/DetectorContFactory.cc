/**
 * @file   DetectorContFactory.cc
 * @brief  abstract factory class for VHDDetectorConstruction-derived concrete classes
 *
 * @date   9th June 2014
 * @author Shih-ying Huang
 * @name   Geant4.9.6-p02
 */

#include "DetectorContFactory.hh"
#include "NestedParamVHDDetectorConstruction.hh"
#include "RegularVHDDetectorConstruction.hh"

DetectorContFactory::DetectorContFactory()
{
;
}

DetectorContFactory::~DetectorContFactory()
{
    G4cout << "delete DetectorFactory.." << G4endl;
}

VHDDetectorConstruction* DetectorContFactory::GetDetectorConstr(G4String& type)
{
  if(type == "NestedParam")
    return new NestedParamVHDDetectorConstruction();
  if(type == "RegParam")
    return new RegularVHDDetectorConstruction();
  return NULL;
}
