#ifndef RUN_MESSENGER_HPP
#define RUN_MESSENGER_HPP
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UImessenger.hh"

namespace dgg4 {
class RunAction;

class RunMessenger : public G4UImessenger {
 public:
  RunMessenger(RunAction* runaction);
  ~RunMessenger();

  void SetNewValue(G4UIcommand* cmd, G4String val) override final;

 private:
  RunAction* m_runAction;
  G4UIdirectory* m_directory;
  G4UIcmdWithABool* m_saveDataCmd;
  G4UIcmdWithAString* m_savePathCmd;
};
}  // namespace dgg4

#endif  // RUN_MESSENGER_HPP
