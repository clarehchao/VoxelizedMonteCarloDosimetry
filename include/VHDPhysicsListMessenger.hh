#ifndef VHDPhysicsListMessenger_h
#define VHDPhysicsListMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class VHDPhysicsList;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;

class VHDPhysicsListMessenger: public G4UImessenger
{
  public:
  
    VHDPhysicsListMessenger(VHDPhysicsList* );
   ~VHDPhysicsListMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
  
    VHDPhysicsList* pPhysicsList;
    G4UIdirectory*             physDir; 
    G4UIcmdWithADoubleAndUnit* allCutCmd;
    G4UIcmdWithAString*        pListCmd;
};

#endif

