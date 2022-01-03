#include "geometrymessenger.hpp"

#include "detectorconstruction.hpp"
using namespace std;

namespace dgg4 {
GeometryMessenger::GeometryMessenger(DetectorConstruction* dc) : m_DC(dc) {
  G4cout << "Creating GeometryMessenger" << G4endl;
  // Directory: /dgg4/geometry
  m_directory = make_unique<G4UIdirectory>("/dgg4/geometry/");
  m_directory->SetGuidance("Modify the geometry.");

  // Toggle data saving: /dgg4/geometry/update
  m_updateCmd =
      make_unique<G4UIcmdWithoutParameter>("/dgg4/geometry/update", this);
  m_updateCmd->SetGuidance("Update the geometry [and reload visualization].");
  m_updateCmd->AvailableForStates(G4State_Idle);

  // Changing the selected geometry: /dgg4/geometry/select
  m_selectCmd = make_unique<G4UIcmdWithAString>("/dgg4/geometry/select", this);
  m_selectCmd->SetGuidance("Set the selected simulation geometry.");
  m_selectCmd->SetParameterName("geometry", true);
  m_selectCmd->SetDefaultValue(m_DC->get_geometry());
  m_selectCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

GeometryMessenger::~GeometryMessenger() {
  G4cout << "Deleting GeometryMessenger" << G4endl;
}

void GeometryMessenger::SetNewValue(G4UIcommand* cmd, G4String val) {
  if (cmd == m_updateCmd.get()) {
    m_DC->update_geometry();
  } else if (cmd == m_selectCmd.get()) {
    // Check that the provided value is on the list of valid ones
    auto geom = m_DC->Geometries;
    if (geom.count(val) == 0) {
      G4cout << "Invalid simulation geometry specified. Must be one of:";
      for (auto const& g : geom) {
        G4cout << " " << g;
      }
      G4cout << G4endl;
      return;
    }
    m_DC->set_geometry(val);
    G4cout << "Geometry set to " << val << G4endl;
  }
  // Command didn't match
  return;
}
}  // namespace dgg4
