#ifndef VHDMultiSDEventAction_h
#define VHDMultiSDEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"  //need to include this class when using G4 variables,e.g. G4int, G4double!!

class G4Event;

//
class VHDMultiSDEventAction : public G4UserEventAction
{
  public:
    VHDMultiSDEventAction();
   ~VHDMultiSDEventAction();

  public:
    void BeginOfEventAction(const G4Event*);
    void EndOfEventAction(const G4Event*);
    G4int GetEvtID() const {return evtID;}
    
  protected:
    G4int evtID;
};

//

#endif

    
