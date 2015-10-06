#ifndef VHDDetectorConstruction_h
#define VHDDetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "VHDPhantomZSliceHeader.hh"
#include <map>
#include "G4MultiFunctionalDetector.hh"

class G4Material;
class G4Box;
class G4LogicalVolume;

class VHDDetectorConstruction : public G4VUserDetectorConstruction
{
public:

  VHDDetectorConstruction();
  virtual ~VHDDetectorConstruction();

  G4VPhysicalVolume* Construct();
  // trigger the construction of the geometry

  G4int GetNX() const {return nVoxelX;}
  G4int GetNY() const {return nVoxelY;}
  G4int GetNZ() const {return fNoFiles;}  //number of file is the same as the number of voxels in the z-direction
  G4int GetNEngbin() const {return NEngbin;}
  void SetDirName(G4String name) {dirname = name;}
  void SetParticleFlag(G4int isElectron, G4int isPhoton);
  void DefineMaterialsOfInterest();
  void SetEnergyBinOption (G4int ieng) {ebin = ieng;};
  void SetOOItype(G4String type)  {OOItype = type;};

private:
  void InitialisationOfMaterials();
  // create the original materials

  void ReadPhantomData();
  // read the DICOM files describing the phantom

  void ReadPhantomDataFile(const G4String& fname);
  // read one of the DICOM files describing the phantom (usually one per Z slice). Build a VHDPhantomZSliceHeader for each file

  void MergeZSliceHeaders();
  // merge the slice headers of all the files

  void ConstructPhantomContainer();
  // construct the voxel container of the phantom after reading the volume given its voxel dimension

protected:
  virtual void ConstructPhantom() = 0;  //syntax "=0" indicates that ConstructPhantom() is an abstract member function!!
  // construct the phantom volumes. This method should be implemented for a derived class
 
  virtual void SetMultiSensDet(G4LogicalVolume* voxel_logic) = 0;
  // Set up the multisensitive detectors.  This method should be implemented for a derived class

private:
  G4int fNoFiles; // number of DICOM files
  std::map<unsigned int,unsigned int> Organtag2MatIndx;
  std::vector<VHDPhantomZSliceHeader*> fZSliceHeaders; // list of z slice header (one per DICOM files)
  VHDPhantomZSliceHeader* fZSliceHeaderMerged; // z slice header resulted from merging all z slice headers
  G4double totDensity;
  G4String OOItype;  //Organ of interest type: skel or all

protected:   //define all "private"-like variables in "protected" since there is class inheritence going on in this class
  G4Material* air;
  G4Box* world_solid;
  G4LogicalVolume* world_logic;
  G4VPhysicalVolume* world_phys;
  G4Box* container_solid;
  G4LogicalVolume* container_logic;
  G4VPhysicalVolume* container_phys;
  std::vector<G4Material*> fOriginalMaterials;  // list of original materials
  std::vector<G4Material*> fMaterialsOfInterest; // the materials of interest for CellFlux scorer 
  size_t* fMateIDs; // index of material of each voxel
  G4String dirname;
  G4MultiFunctionalDetector* MFDet;
  G4int ebin;  //flag to select the energy bin used in the simulation
  G4bool electronflag, photonflag;  //flag to see if one wants to track electron or gamma rays or not in SetMultiSensDet function

  G4int nVoxelX, nVoxelY, nVoxelZ, NEngbin;
  G4double voxelHalfDimX,  voxelHalfDimY, voxelHalfDimZ;
  
  
  G4VPhysicalVolume* param_phys;

};

#endif

