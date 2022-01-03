#ifndef EX3_GEOMETRY_HPP
#define EX3_GEOMETRY_HPP
#include "basegeometry.hpp"

namespace dgg4 {
class Ex3Geometry : public BaseGeometry {
 public:
  Ex3Geometry();
  ~Ex3Geometry();

  /// This is nearly a line-for-line copy of the Construct() function in example
  /// B3b.
  void build(G4LogicalVolume* mother_log) override final;

 private:
  /// Custom G4Material definitions for this geometry.
  void define_materials();
};
}  // namespace dgg4

#endif
