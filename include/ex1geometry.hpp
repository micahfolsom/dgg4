#ifndef EX1_GEOMETRY_HPP
#define EX1_GEOMETRY_HPP
#include "basegeometry.hpp"

namespace dgg4 {
class DetectorConstruction;
class Ex1Geometry : public BaseGeometry {
 public:
  Ex1Geometry(DetectorConstruction* dc);
  ~Ex1Geometry();

  /// This is nearly a line-for-line copy of the Construct() function in example
  /// B1.
  void build(G4LogicalVolume* mother_log) override final;

 private:
  DetectorConstruction* m_DC;
};
}  // namespace dgg4

#endif
