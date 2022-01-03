#ifndef DETECTOR_CONSTRUCTION_HPP
#define DETECTOR_CONSTRUCTION_HPP
#include <list>

#include "G4VUserDetectorConstruction.hh"
class G4VPhysicalVolume;
class G4LogicalVolume;

namespace dgg4 {
class SensitiveDetector;
class GeometryMessenger;
class DetectorConstruction : public G4VUserDetectorConstruction {
 public:
  DetectorConstruction();
  ~DetectorConstruction();
  G4VPhysicalVolume* Construct() override final;
  void ConstructSDandField() override final;

  inline std::list<G4String> const& get_sd_names() const { return m_SDNames; }
  void set_sd(G4String const& lvname, G4String const& sdname);
  /// Clears out all of the geometry objects - solids, logical volumes, etc.
  /// Construct() and ConstructSDandField() are called once again, so any
  /// updates to the geometry objects will be applied. The visualization is
  /// also reset.
  void update_geometry();
  /// Check if a logical volume exists by looping over the logical volume
  /// store and comparing names.
  bool check_lv_exists(G4String const& lvname) const;

 private:
  /// Have the SDs been created yet?
  bool m_fSDInit;
  /// List of SD names.
  std::list<G4String> m_SDNames;
  /// List of G4LogicalVolumes that each SD has attached to it.
  std::map<G4String, std::list<G4String>> m_SDLVList;
  /// Thread-indexed pointers to the SD objects. These must be
  /// allocated and connected to the LVs in the respective threads.
  /// We do not need to clean them up - the SDManager will take care
  /// of the memory. Conversely, they can't really be destroyed, and
  /// they won't be cleared out when the geometry is reloaded, hence
  /// why we keep the pointers and reconnect to the LVs.
  std::map<G4String, std::vector<SensitiveDetector*>> m_SD;
  /// Geometry messenger.
  std::unique_ptr<GeometryMessenger> m_messenger;
};
}  // namespace dgg4

#endif
