#include "ex1geometry.hpp"

#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Trd.hh"

namespace dgg4 {
Ex1Geometry::Ex1Geometry() : BaseGeometry("example1") {
  G4cout << "Creating Ex1Geometry" << G4endl;
}
Ex1Geometry::~Ex1Geometry() { G4cout << "Deleting Ex1Geometry" << G4endl; }
void Ex1Geometry::build(G4LogicalVolume* mother_log) {
  G4cout << "Building Ex1Geometry with name " << m_name << "...";
  G4NistManager* nist = G4NistManager::Instance();
  // Envelope parameters
  G4double env_sizeXY = 20 * cm, env_sizeZ = 30 * cm;
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_WATER");

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //
  // Envelope
  //
  auto sname = m_name + "_envelope_solid";
  G4Box* solidEnv = new G4Box(sname,  // its name
                              0.5 * env_sizeXY, 0.5 * env_sizeXY,
                              0.5 * env_sizeZ);  // its size

  auto lname = m_name + "_envelope_log";
  G4LogicalVolume* logicEnv = new G4LogicalVolume(solidEnv,  // its solid
                                                  env_mat,   // its material
                                                  lname);    // its name

  auto pname = m_name + "_envelope_phys";
  new G4PVPlacement(nullptr,          // no rotation
                    G4ThreeVector(),  // at (0,0,0)
                    logicEnv,         // its logical volume
                    pname,            // its name
                    mother_log,       // its mother  volume
                    false,            // no boolean operation
                    0,                // copy number
                    checkOverlaps);   // overlaps checking

  //
  // Shape 1
  //
  G4Material* shape1_mat = nist->FindOrBuildMaterial("G4_A-150_TISSUE");
  G4ThreeVector pos1 = G4ThreeVector(0, 2 * cm, -7 * cm);

  // Conical section shape
  G4double shape1_rmina = 0. * cm, shape1_rmaxa = 2. * cm;
  G4double shape1_rminb = 0. * cm, shape1_rmaxb = 4. * cm;
  G4double shape1_hz = 3. * cm;
  G4double shape1_phimin = 0. * deg, shape1_phimax = 360. * deg;
  sname = m_name + "_shape1_solid";
  G4Cons* solidShape1 =
      new G4Cons(sname, shape1_rmina, shape1_rmaxa, shape1_rminb, shape1_rmaxb,
                 shape1_hz, shape1_phimin, shape1_phimax);

  lname = m_name + "_shape1_log";
  G4LogicalVolume* logicShape1 =
      new G4LogicalVolume(solidShape1,  // its solid
                          shape1_mat,   // its material
                          lname);       // its name

  pname = m_name + "_shape1_phys";
  new G4PVPlacement(nullptr,         // no rotation
                    pos1,            // at position
                    logicShape1,     // its logical volume
                    pname,           // its name
                    logicEnv,        // its mother  volume
                    false,           // no boolean operation
                    0,               // copy number
                    checkOverlaps);  // overlaps checking

  //
  // Shape 2
  //
  G4Material* shape2_mat = nist->FindOrBuildMaterial("G4_BONE_COMPACT_ICRU");
  G4ThreeVector pos2 = G4ThreeVector(0, -1 * cm, 7 * cm);

  // Trapezoid shape
  G4double shape2_dxa = 12 * cm, shape2_dxb = 12 * cm;
  G4double shape2_dya = 10 * cm, shape2_dyb = 16 * cm;
  G4double shape2_dz = 6 * cm;
  sname = m_name + "_shape2_solid";
  G4Trd* solidShape2 =
      new G4Trd(sname,  // its name
                0.5 * shape2_dxa, 0.5 * shape2_dxb, 0.5 * shape2_dya,
                0.5 * shape2_dyb, 0.5 * shape2_dz);  // its size

  lname = m_name + "_shape2_log";
  G4LogicalVolume* logicShape2 =
      new G4LogicalVolume(solidShape2,  // its solid
                          shape2_mat,   // its material
                          lname);       // its name

  pname = m_name + "_shape2_phys";
  new G4PVPlacement(nullptr,         // no rotation
                    pos2,            // at position
                    logicShape2,     // its logical volume
                    pname,           // its name
                    logicEnv,        // its mother  volume
                    false,           // no boolean operation
                    0,               // copy number
                    checkOverlaps);  // overlaps checking
  G4cout << "...done" << G4endl;
  return;
}
}  // namespace dgg4
