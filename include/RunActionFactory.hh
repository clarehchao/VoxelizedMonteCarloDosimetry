#ifndef RunActionFactory_h
#define RunActionFactory_h 1

#include "VHDMultiSDRunAction.hh"
#include "VHDMultiSDRunActionROOT.hh"

class RunActionFactory
{

  public:
    RunActionFactory();
    ~RunActionFactory();
    VHDMultiSDRunAction* GetRunAction(G4String& type);

};


#endif

