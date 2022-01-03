#ifndef WORLD_HPP
#define WORLD_HPP
#include "G4ThreeVector.hh"
#include "basegeometry.hpp"
class G4LogicalVolume;
class G4PVPlacement;

namespace dgg4 {
class World : public BaseGeometry {
 public:
  World();
  ~World();

  G4LogicalVolume* get_logical() const;
  G4PVPlacement* get_physical() const;
  void build(G4LogicalVolume* mother_log = nullptr) override final;

 private:
  G4ThreeVector m_dimensions;
  G4LogicalVolume* m_logical;
  G4PVPlacement* m_physical;
};
}  // namespace dgg4

#endif
