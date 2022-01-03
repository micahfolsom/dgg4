#ifndef GEOMETRY_MESSENGER_HPP
#define GEOMETRY_MESSENGER_HPP
#include "G4UIcmdWithoutParameter.hh"
#include "G4UImessenger.hh"

namespace dgg4 {
class DetectorConstruction;

class GeometryMessenger : public G4UImessenger {
 public:
  GeometryMessenger(DetectorConstruction* dc);
  ~GeometryMessenger();

  void SetNewValue(G4UIcommand* cmd, G4String) override final;

 private:
  DetectorConstruction* m_DC;
  std::unique_ptr<G4UIdirectory> m_directory;
  std::unique_ptr<G4UIcmdWithoutParameter> m_updateCmd;
};
}  // namespace dgg4

#endif  // GEOMETRY_MESSENGER_HPP
