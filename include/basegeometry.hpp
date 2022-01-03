#ifndef BASE_GEOMETRY_HPP
#define BASE_GEOMETRY_HPP
#include "G4String.hh"
class G4LogicalVolume;

namespace dgg4 {
/*! \class BaseGeometry
 * A common base for *Geometry objects. These objects should contain the code
 * necessary to build a simulation physical geometry. At a minimum, this is the
 * code typically found in DetectorConstruction::Construct(), to create the
 * solid, logical, and physical volumes. This could also include creating new
 * materials or other geometry-specific initializations.
 * The DetectorConstruction can keep pointers to this base class, allowing one
 * to swap between geometries interactively. */
class BaseGeometry {
 public:
  BaseGeometry(G4String const& name) : m_name(name) {}
  virtual ~BaseGeometry() {}

  inline G4String const& get_name() const { return m_name; }

  /// Create the geometry - this means allocating new G4Solids,
  /// G4LocailVolumes, G4PVPlacements, etc. Geant4 takes care
  /// of the memory for all of these, so we just need to create
  /// them. This will contain code that you typically find in
  /// DetectorConstruction::Construct().
  /// The parent logical volume to this geometry collection must
  /// also be passed in.
  virtual void build(G4LogicalVolume* mother_log) = 0;

 protected:
  /// Base name for this geometric object. _solid, _log, _phys, etc,
  /// are added to this base name for the different components.
  G4String m_name;
};
}  // namespace dgg4

#endif