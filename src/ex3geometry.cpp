#include "ex3geometry.hpp"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

namespace dgg4 {
Ex3Geometry::Ex3Geometry() : BaseGeometry("example3") {
  G4cout << "Creating Ex3Geometry" << G4endl;
  define_materials();
}
Ex3Geometry::~Ex3Geometry() { G4cout << "Deleting Ex3Geometry" << G4endl; }
void Ex3Geometry::build(G4LogicalVolume* mother_log) {
  G4cout << "Building Ex3Geometry with name " << m_name << "...";
  // Gamma detector Parameters
  //
  G4double cryst_dX = 6 * cm, cryst_dY = 6 * cm, cryst_dZ = 3 * cm;
  G4int nb_cryst = 32;
  G4int nb_rings = 9;
  //
  G4double dPhi = CLHEP::twopi / nb_cryst, half_dPhi = 0.5 * dPhi;
  G4double cosdPhi = std::cos(half_dPhi);
  G4double tandPhi = std::tan(half_dPhi);
  //
  G4double ring_R1 = 0.5 * cryst_dY / tandPhi;
  G4double ring_R2 = (ring_R1 + cryst_dZ) / cosdPhi;
  //
  G4double detector_dZ = nb_rings * cryst_dX;
  //
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* default_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* cryst_mat = nist->FindOrBuildMaterial(m_name + "_Lu2SiO5");

  //
  // ring
  //
  auto sname = m_name + "_ring_solid";
  G4Tubs* solidRing =
      new G4Tubs(sname, ring_R1, ring_R2, 0.5 * cryst_dX, 0., CLHEP::twopi);

  auto lname = m_name + "_ring_log";
  G4LogicalVolume* logicRing = new G4LogicalVolume(solidRing,    // its solid
                                                   default_mat,  // its material
                                                   "Ring");      // its name

  //
  // define crystal
  //
  G4double gap = 0.5 * mm;  // a gap for wrapping
  G4double dX = cryst_dX - gap, dY = cryst_dY - gap;
  sname = m_name + "_crystal_solid";
  G4Box* solidCryst = new G4Box(sname, dX / 2, dY / 2, cryst_dZ / 2);

  lname = m_name + "_crystal_log";
  G4LogicalVolume* logicCryst = new G4LogicalVolume(solidCryst,  // its solid
                                                    cryst_mat,   // its material
                                                    lname);      // its name

  // place crystals within a ring
  //
  for (G4int icrys = 0; icrys < nb_cryst; icrys++) {
    G4double phi = icrys * dPhi;
    G4RotationMatrix rotm;
    rotm.rotateY(90 * deg);
    rotm.rotateZ(phi);
    G4ThreeVector uz = G4ThreeVector(std::cos(phi), std::sin(phi), 0.);
    G4ThreeVector position = (ring_R1 + 0.5 * cryst_dZ) * uz;
    G4Transform3D transform = G4Transform3D(rotm, position);

    auto pname = m_name + "_crystal_phys";
    new G4PVPlacement(transform,   // rotation,position
                      logicCryst,  // its logical volume
                      pname,       // its name
                      logicRing,   // its mother  volume
                      false,       // no boolean operation
                      icrys,       // copy number
                      true);       // checking overlaps
  }

  //
  // full detector
  //
  sname = m_name + "_detector_solid";
  G4Tubs* solidDetector =
      new G4Tubs(sname, ring_R1, ring_R2, 0.5 * detector_dZ, 0., CLHEP::twopi);

  lname = m_name + "_detector_log";
  G4LogicalVolume* logicDetector =
      new G4LogicalVolume(solidDetector,  // its solid
                          default_mat,    // its material
                          lname);         // its name

  //
  // place rings within detector
  //
  G4double OG = -0.5 * (detector_dZ + cryst_dX);
  for (G4int iring = 0; iring < nb_rings; iring++) {
    OG += cryst_dX;
    auto pname = m_name + "_ring_phys";
    new G4PVPlacement(0,                        // no rotation
                      G4ThreeVector(0, 0, OG),  // position
                      logicRing,                // its logical volume
                      pname,                    // its name
                      logicDetector,            // its mother  volume
                      false,                    // no boolean operation
                      iring,                    // copy number
                      true);                    // checking overlaps
  }

  //
  // place detector in world
  //
  auto pname = m_name + "_detector_phys";
  new G4PVPlacement(0,                // no rotation
                    G4ThreeVector(),  // at (0,0,0)
                    logicDetector,    // its logical volume
                    pname,            // its name
                    mother_log,       // its mother  volume
                    false,            // no boolean operation
                    0,                // copy number
                    true);            // checking overlaps
  //
  // patient
  //
  G4double patient_radius = 8 * cm;
  G4double patient_dZ = 10 * cm;
  G4Material* patient_mat = nist->FindOrBuildMaterial("G4_BRAIN_ICRP");

  sname = m_name + "_patient_solid";
  G4Tubs* solidPatient =
      new G4Tubs(sname, 0., patient_radius, 0.5 * patient_dZ, 0., CLHEP::twopi);

  lname = m_name + "_patient_log";
  G4LogicalVolume* logicPatient =
      new G4LogicalVolume(solidPatient,  // its solid
                          patient_mat,   // its material
                          lname);        // its name

  //
  // place patient in world
  //
  pname = m_name + "_patient_phys";
  new G4PVPlacement(0,                // no rotation
                    G4ThreeVector(),  // at (0,0,0)
                    logicPatient,     // its logical volume
                    pname,            // its name
                    mother_log,       // its mother  volume
                    false,            // no boolean operation
                    0,                // copy number
                    true);            // checking overlaps

  // Visualization attributes
  //
  logicRing->SetVisAttributes(G4VisAttributes::GetInvisible());
  logicDetector->SetVisAttributes(G4VisAttributes::GetInvisible());

  // Print materials
  // G4cout << *(G4Material::GetMaterialTable()) << G4endl;
  G4cout << "...done" << G4endl;
  return;
}

void Ex3Geometry::define_materials() {
  G4NistManager* man = G4NistManager::Instance();
  G4bool isotopes = false;

  G4Element* O = man->FindOrBuildElement("O", isotopes);
  G4Element* Si = man->FindOrBuildElement("Si", isotopes);
  G4Element* Lu = man->FindOrBuildElement("Lu", isotopes);
  // Careful about names, since materials are global
  G4Material* LSO = new G4Material(m_name + "_Lu2SiO5", 7.4 * g / cm3, 3);
  LSO->AddElement(Lu, 2);
  LSO->AddElement(Si, 1);
  LSO->AddElement(O, 5);
  return;
}
}  // namespace dgg4
