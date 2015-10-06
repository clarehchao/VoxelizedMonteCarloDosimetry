#ifndef VHDRegParamCellFlux_h
#define VHDRegParamCellFlux_h 1

#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"
#include "G4Material.hh"

class G4VSolid;

//very similar definition to G4PSCellFlux class except ProcessHits class, which involves accessing the EvtMap (a private variable)
//so i can't just derive a class from G4PSCellFlux class but need to write another class that is derived from G4VPrimitiveScorer where i can access the EvtMap!
class VHDRegParamCellFlux: public G4VPrimitiveScorer
{
   public:
	VHDRegParamCellFlux(G4String name,G4int nx, G4int ny, G4int nz,G4int depth=0);
	VHDRegParamCellFlux(G4String name, const G4String& unit,G4int depth=0);
	virtual ~VHDRegParamCellFlux();
	
	inline void Weighted(G4bool flg=true){ weighted = flg;}  //multiply track weight
	void SetMaterialsOfInterest(std::vector<G4Material*> moi) {MaterialsOfInterest = moi;}
		
   protected:
   	virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
   	virtual G4double ComputeVolume(G4Step*,G4int idx);
   	
   public:
   	virtual void Initialize(G4HCofThisEvent*);
   	virtual void EndOfEvent(G4HCofThisEvent*);
   	virtual void clear();
   	virtual void DrawAll();
   	virtual void PrintAll();

	virtual void SetUnit(const G4String& unit);
	
   protected:
   	virtual void DefineUnitAndCategory();
   	
   private:
   	G4int HCID;
   	G4THitsMap<G4double>* EvtMap;
   	G4bool weighted;
   	G4int fNx,fNy,fNz,fNxNy;
   	std::vector<G4Material*> MaterialsOfInterest;
};

#endif
