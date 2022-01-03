#include "geometrymessenger.hpp"

#include "detectorconstruction.hpp"
using namespace std;

namespace dgg4 {
GeometryMessenger::GeometryMessenger(DetectorConstruction* dc) : m_DC(dc) {
  // Directory: /dgg4/run
  m_directory = new G4UIdirectory("/dgg4/geometry/");
  m_directory->SetGuidance("Modify the geometry.");

  // Toggle data saving: /dgg4/geometry/update
  m_updateCmd =
      make_unique<G4UIcmdWithoutParameter>("/dgg4/geometry/update", this);
  m_updateCmd->SetGuidance("Update the geometry [and reload visualization].");
  m_updateCmd->AvailableForStates(G4State_Idle);
}

GeometryMessenger::~GeometryMessenger() { delete m_directory; }

void GeometryMessenger::SetNewValue(G4UIcommand* cmd, G4String) {
  if (cmd == m_updateCmd.get()) {
    m_DC->update_geometry();
  }
  // Command didn't match
  return;
}
}  // namespace dgg4