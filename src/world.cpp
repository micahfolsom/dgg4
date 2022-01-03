#include "world.hpp"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

namespace dgg4 {
World::World()
    : BaseGeometry("world"),
      m_dimensions(2 * m, 2 * m, 2 * m),
      m_logical(nullptr),
      m_physical(nullptr) {
  G4cout << "Creating World" << G4endl;
}

World::~World() { G4cout << "Deleting world" << G4endl; }

G4LogicalVolume* World::get_logical() const { return m_logical; }

G4PVPlacement* World::get_physical() const { return m_physical; }

void World::build(G4LogicalVolume* mother_log) {
  G4cout << "Building World volume with name " << m_name << "...";
  auto sname = m_name + "_solid";
  auto box = new G4Box(sname, m_dimensions.x() / 2.0, m_dimensions.y() / 2.0,
                       m_dimensions.z() / 2.0);

  auto nist_man = G4NistManager::Instance();
  auto mat = nist_man->FindOrBuildMaterial("G4_Galactic");
  auto lname = m_name + "_log";
  m_logical = new G4LogicalVolume(box, mat, lname);

  auto pname = m_name + "_phys";
  m_physical = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0), m_logical,
                                 pname, mother_log, false, 0, true);
  G4cout << "...done" << G4endl;
  return;
}
}  // namespace dgg4
