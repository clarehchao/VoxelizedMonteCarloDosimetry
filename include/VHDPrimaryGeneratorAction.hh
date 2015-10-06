#ifndef VHDPrimaryGeneratorAction_h
#define VHDPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include <map>
#include "G4ParticleGun.hh"
#include "G4Event.hh"

//class G4GeneralParticleSource;
//class G4ParticleGun;
//class G4Event;

class VHDPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    //VHDPrimaryGeneratorAction(const G4String& dname, char dirtag[50], char mactag[50],G4int runcount);
  VHDPrimaryGeneratorAction(const G4String& dname, const G4int isSparse);
   ~VHDPrimaryGeneratorAction();

  public:
    void GeneratePrimaries(G4Event*);
    void SetSourceProbMap(const G4String& dirname);
    void SetSourceProbMapSparse(const G4String& dirname);
    void ReadDoseMapFile(const G4String& fname);
    G4ThreeVector GeneratePosition();
    G4ThreeVector GenerateIsotropicMomentum();
    void ErrorFileTooShort(const G4String& fname);
    void ErrorFileNotFound(const G4String& fname);
    G4ParticleGun* GetParticleGun() { return pgun;} ;

  private:
    //G4GeneralParticleSource* pgun;
    G4ParticleGun* pgun;
    G4double MinTheta, MaxTheta, MinPhi,MaxPhi;
    G4ThreeVector position, momentum;

    G4int fNoFiles; // number of DoseMap files
    G4int nfile;
    std::map<G4double,G4int> theProbAccum, probAccum;
    G4double theProbSum;

    G4int NVoxelX;
    G4int NVoxelY;
    G4int NVoxelZ;
    G4int NVoxelXY;
    G4int nVoxels;
    G4double Xmin,Xmax;
    G4double Ymin,Ymax;
    G4double Zmin,Zmax;
    G4double dX,dY,dZ;
    G4double offsetX,offsetY,offsetZ;
    FILE *fpt1, *fpt2, *fpt3;
    char filename1[300],filename2[300],filename3[300];
};
	
#endif


