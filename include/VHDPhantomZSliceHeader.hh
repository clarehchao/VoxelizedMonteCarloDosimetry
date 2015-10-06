#ifndef VHDPhantomZSliceHeader_h
#define VHDPhantomZSliceHeader_h 1

#include "globals.hh"
#include "G4ios.hh"
class G4material;
#include <fstream>
#include <vector>


class VHDPhantomZSliceHeader 
{
public:

  VHDPhantomZSliceHeader( const VHDPhantomZSliceHeader& rhs );
  // build object copying an existing one (except Z dimensions)

  VHDPhantomZSliceHeader( std::ifstream& fin );
  // build object reading data from a file

  ~VHDPhantomZSliceHeader(){};

  // Get and set methods
  G4int GetNoVoxelX() const { return fNoVoxelX; };
  G4int GetNoVoxelY() const { return fNoVoxelY; };
  G4int GetNoVoxelZ() const { return fNoVoxelZ; };
  G4int GetNoVoxels() const { return fNoVoxelX*fNoVoxelY*fNoVoxelZ; };

  G4double GetMinX() const { return fMinX; };
  G4double GetMinY() const { return fMinY; };
  G4double GetMinZ() const { return fMinZ; };
  G4double GetMaxX() const { return fMaxX; };
  G4double GetMaxY() const { return fMaxY; };
  G4double GetMaxZ() const { return fMaxZ; };

  G4double GetVoxelHalfX() const { return (fMaxX-fMinX)/fNoVoxelX/2.; };
  G4double GetVoxelHalfY() const { return (fMaxY-fMinY)/fNoVoxelY/2.; };
  G4double GetVoxelHalfZ() const { return (fMaxZ-fMinZ)/fNoVoxelZ/2.; };

  //G4double GetVoxelHalfX() const { return (fMaxX-fMinX)/(fNoVoxelX-1)/2.; };
  //G4double GetVoxelHalfY() const { return (fMaxY-fMinY)/(fNoVoxelY-1)/2.; };
  //G4double GetVoxelHalfZ() const { return (fMaxZ-fMinZ)/fNoVoxelZ/2.; };

  std::vector<G4String> GetMaterialNames() const { return fMaterialNames; };
 

  void SetNoVoxelX(const G4int val) { fNoVoxelX = val; }
  void SetNoVoxelY(const G4int val) { fNoVoxelY = val; }
  void SetNoVoxelZ(const G4int val) { fNoVoxelZ = val; }

  void SetMinX(const G4double val) { fMinX = val; };
  void SetMaxX(const G4double val) { fMaxX = val; };
  void SetMinY(const G4double val) { fMinY = val; };
  void SetMaxY(const G4double val) { fMaxY = val; };
  void SetMinZ(const G4double val) { fMinZ = val; };
  void SetMaxZ(const G4double val) { fMaxZ = val; };

  void SetMaterialNames(std::vector<G4String>& mn ){ fMaterialNames = mn; }


  void operator+=( const VHDPhantomZSliceHeader& rhs );
  VHDPhantomZSliceHeader operator+( const VHDPhantomZSliceHeader& rhs );
  // add two slices that have the same dimensions, merging them in Z 

private:
  G4bool CheckMaterialExists( const G4String& mateName );
  // check that material read exists as a G4Material

private:
  G4int fNoVoxelX, fNoVoxelY, fNoVoxelZ;  // number of voxels in each dimensions
  G4double fMinX,fMinY,fMinZ; // minimum extension of voxels (position of wall)
  G4double fMaxX,fMaxY,fMaxZ; // maximum extension of voxels (position of wall)
  std::vector<G4String> fMaterialNames; // list of material names
};

#endif
