#ifndef VHDPhysicsList_h
#define VHDPhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"
#include "VHDPhysicsListMessenger.hh"
#include "G4VPhysicsConstructor.hh"

//Software note:
// - changed SetCut() from 'virtual void SetCut()' to just 'void SetCut()' since the function is not being derived again

class VHDPhysicsList: public G4VModularPhysicsList
{
public:
  VHDPhysicsList();
  virtual ~VHDPhysicsList();

  // SetCuts()
  void SetDefaultCutValue(G4double cut) {defaultCutValue = cut;};
  void SetCuts();
  void AddPhysicsList(const G4String& name);
  
private:
   VHDPhysicsListMessenger* pMessenger;
   G4String emName;
   G4VPhysicsConstructor* emPhysicsList;
   G4PhysConstVector::iterator itr;
};


#endif



