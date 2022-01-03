#include "runmessenger.hpp"

#include "runaction.hpp"
using namespace std;

namespace dgg4 {
RunMessenger::RunMessenger(RunAction* runaction) : m_runAction(runaction) {
  G4cout << "Creating RunMessenger" << G4endl;
  // Directory: /dgg4/run
  m_directory = make_unique<G4UIdirectory>("/dgg4/run/");
  m_directory->SetGuidance("Change parameters of the run.");

  // Toggle data saving: /dgg4/run/save_data
  m_saveDataCmd = make_unique<G4UIcmdWithABool>("/dgg4/run/save_data", this);
  m_saveDataCmd->SetGuidance("Toggle saving data to file.");
  m_saveDataCmd->SetParameterName("save_data", true);
  m_saveDataCmd->SetDefaultValue(m_runAction->save_data());
  m_saveDataCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  // Save file path: /dgg4/run/save_path
  m_savePathCmd = make_unique<G4UIcmdWithAString>("/dgg4/run/save_path", this);
  m_savePathCmd->SetGuidance("Save file path.");
  m_savePathCmd->SetParameterName("save_path", true);
  m_savePathCmd->SetDefaultValue(m_runAction->get_path());
  m_savePathCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

RunMessenger::~RunMessenger() { G4cout << "Deleting RunMessenger" << G4endl; }

void RunMessenger::SetNewValue(G4UIcommand* cmd, G4String val) {
  if (cmd == m_saveDataCmd.get()) {
    bool parsed_val = m_saveDataCmd->GetNewBoolValue(val);
    m_runAction->save_data(parsed_val);
    G4cout << "Save data set to " << (parsed_val ? "true" : "false") << G4endl;
  } else if (cmd == m_savePathCmd.get()) {
    m_runAction->set_path(val);
    G4cout << "Save file path set to " << val << G4endl;
  }
  // Command didn't match
  return;
}
}  // namespace dgg4
