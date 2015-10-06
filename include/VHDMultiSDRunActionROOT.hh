#ifndef VHDMultiSDRunActionROOT_h
#define VHDMultiSDRunActionROOT_h 1

#include "VHDMultiSDRunAction.hh"
#include "globals.hh"
#include <vector>

//ROOT 
#include "TFile.h"
#include "TH1F.h"
#include "TTree.h"


class G4Run;

class VHDMultiSDRunActionROOT : public VHDMultiSDRunAction
{
public:
  // constructor and destructor
  VHDMultiSDRunActionROOT();
  virtual ~VHDMultiSDRunActionROOT();

public:
  //modify EndOfRunAction in how the data is saved
  virtual void EndOfRunAction(const G4Run*);

};

#endif
