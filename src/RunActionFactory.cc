/**
 * @file   RunActionFactory.cc
 * @brief  abstract factory class for VHDDetectorConstruction-derived concrete classes
 *
 * @date   9th June 2014
 * @author Shih-ying Huang
 * @name   Geant4.9.6-p02
 */

#include "RunActionFactory.hh"

RunActionFactory::RunActionFactory()
{
;
}

RunActionFactory::~RunActionFactory()
{
;
}

VHDMultiSDRunAction* RunActionFactory::GetRunAction(G4String& type)
{
  if(type == "Reg")
    return new VHDMultiSDRunAction();
  if(type == "Root")
    return new VHDMultiSDRunActionROOT();
  return NULL;
}
