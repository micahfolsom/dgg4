#include "detectorconstruction.hpp"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include "sensitivedetector.hpp"

namespace dgg4 {
DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction() {
  G4cout << "Creating DetectorConstruction" << G4endl;
}

DetectorConstruction::~DetectorConstruction() {
  G4cout << "Deleting DetectorConstruction" << G4endl;
}

G4VPhysicalVolume* DetectorConstruction::Construct() {
  // World
  auto world_solid = new G4Box("world_solid", 0.5 * m, 0.5 * m, 0.5 * m);
  auto nist = G4NistManager::Instance();
  auto world_mat = nist->FindOrBuildMaterial("G4_AIR");
  auto world_log = new G4LogicalVolume(world_solid, world_mat, "world_log");
  auto world_phys =
      new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0), world_log,
                        "world_phys", nullptr, false, 0, true);

  // Detector
  auto det_solid = new G4Box("det_solid", 5 * cm, 5 * cm, 5 * cm);
  auto det_mat = nist->FindOrBuildMaterial("G4_SODIUM_IODIDE");
  auto det_log = new G4LogicalVolume(det_solid, det_mat, "det_log");
  m_sensitiveVols.push_back(det_log);
  new G4PVPlacement(nullptr, G4ThreeVector(0, 0, -20 * cm), det_log, "det_phys",
                    world_log, false, 0, true);
  return world_phys;
}
void DetectorConstruction::ConstructSDandField() {
  auto sd = new SensitiveDetector("det_sd");
  G4SDManager::GetSDMpointer()->AddNewDetector(sd);
  for (auto& log : m_sensitiveVols) {
    SetSensitiveDetector(log, sd);
  }
  return;
}

}  // namespace dgg4
