#ifndef VHDMultiSDRunAction_h
#define VHDMultiSDRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include <vector>


class G4Run;

class VHDMultiSDRunAction : public G4UserRunAction
{
public:
  // constructor and destructor
  VHDMultiSDRunAction();
  virtual ~VHDMultiSDRunAction();

public:
  // virtual method from G4UserRunAction.
  virtual G4Run* GenerateRun();
  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfRunAction(const G4Run*);

public:
  // Utility method for converting segment number of
  // water phantom to copyNo of HitsMap.
  G4int CopyNo(G4int ix, G4int iy, G4int iz)
  {  return (ix + iy*fNx + iz*fNxNy); }
  //void SetRunInfo(G4int count,char dname[],char rname[]);
  void SetRunInfo(char dname[]);

private:
  // Data member 
  // - vector of MultiFunctionalDetecor names.
  std::vector<G4String> theSDName;  

protected:
  // for conversion of sengment number to copyNo.
  G4int fNx, fNy, fNz,fNxNy,NEbin;
  //char runName[700];
  char dirName[700];
  //G4int rcount;

};

//

#endif





