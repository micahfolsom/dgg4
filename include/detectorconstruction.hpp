#ifndef DETECTOR_CONSTRUCTION_HPP
#define DETECTOR_CONSTRUCTION_HPP
#include "G4VUserDetectorConstruction.hh"
class G4VPhysicalVolume;
class G4LogicalVolume;

namespace dgg4 {
class DetectorConstruction : public G4VUserDetectorConstruction {
 public:
  DetectorConstruction();
  ~DetectorConstruction();
  G4VPhysicalVolume* Construct() override final;
  void ConstructSDandField() override final;

 private:
  // For now, we will just keep a list of all logical volumes to
  // attach to the one sensitive detector
  std::vector<G4LogicalVolume*> m_sensitiveVols;
};
}  // namespace dgg4

#endif
