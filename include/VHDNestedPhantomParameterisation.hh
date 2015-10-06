#ifndef VHDNESTEDPARAMETERISATION_HH
#define VHDNESTEDPARAMETERISATION_HH

#include <vector>
#include <map>

#include "G4Types.hh"
#include "G4ThreeVector.hh"
#include "G4VNestedParameterisation.hh" 

class G4VPhysicalVolume;
class G4VTouchable; 
class G4VSolid;
class G4Material;
class G4VisAttributes;

// CSG Entities which may be parameterised/replicated
//
class G4Box;
class G4Tubs;
class G4Trd;
class G4Trap;
class G4Cons;
class G4Sphere;
class G4Orb;
class G4Torus;
class G4Para;
class G4Polycone;
class G4Polyhedra;
class G4Hype;

class VHDNestedPhantomParameterisation : public G4VNestedParameterisation
{
  public:

    VHDNestedPhantomParameterisation(const G4ThreeVector& voxelSize,std::vector<G4Material*>& mat);
    virtual ~VHDNestedPhantomParameterisation(); 

    G4Material* ComputeMaterial(G4VPhysicalVolume *currentVol,const G4int repNo,const G4VTouchable *parentTouch );
    
    // Must cope with parentTouch for navigator's SetupHierarchy
    G4int       GetNumberOfMaterials() const;
    G4Material* GetMaterial(G4int idx) const;
      // Needed to define materials for instances of Nested Parameterisation 
      // Current convention: each call should return the materials 
      // of all instances with the same mother/ancestor volume

    //unsigned int GetMaterialIndex( unsigned int nx, unsigned int ny, unsigned int nz) const;
    unsigned int GetMaterialIndex( unsigned int copyNo) const;
    //void SetMaterialIndices( unsigned int* matInd ) { fMaterialIndices = matInd; }
    void SetMaterialIndices( size_t* matInd ) { fMaterialIndices = matInd; }
    void SetNoVoxel( unsigned int nx, unsigned int ny, unsigned int nz );
    void ReadColourData();
    
    void ComputeTransformation(const G4int no,
                                     G4VPhysicalVolume *currentPV) const;

    // Additional standard Parameterisation methods, 
    // which can be optionally defined, in case solid is used.

    void ComputeDimensions(G4Box &, const G4int,
                                    const G4VPhysicalVolume *) const;

    using G4VNestedParameterisation::ComputeMaterial;

  private:  // Dummy declarations to get rid of warnings ...

    void ComputeDimensions (G4Trd&, const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Trap&, const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Cons&, const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Sphere&, const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Orb&, const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Torus&, const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Para&, const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Hype&, const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Tubs&, const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Polycone&, const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Polyhedra&, const G4int,
                            const G4VPhysicalVolume*) const {}
    

  private:

    G4double fdX,fdY,fdZ;
    G4int fnX,fnY,fnZ;
    std::vector<G4Material*> fMaterials;
    size_t* fMaterialIndices; // Index in materials corresponding to each voxel
    // unsigned int* fMaterialIndices; // Index in materials corresponding to each voxel
    std::map<G4String,G4VisAttributes*> fColours;
    //G4int isVis;
};
#endif
