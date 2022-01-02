#ifndef DETECTOR_CONSTRUCTION_HPP
#define DETECTOR_CONSTRUCTION_HPP
#include <list>

#include "G4VUserDetectorConstruction.hh"
class G4VPhysicalVolume;
class G4LogicalVolume;

namespace dgg4 {
class SensitiveDetector;
class DetectorConstruction : public G4VUserDetectorConstruction {
 public:
  DetectorConstruction();
  ~DetectorConstruction();
  G4VPhysicalVolume* Construct() override final;
  void ConstructSDandField() override final;

  inline std::list<G4String> get_sd_names() const { return m_SDNames; }

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
};
}  // namespace dgg4

#endif
