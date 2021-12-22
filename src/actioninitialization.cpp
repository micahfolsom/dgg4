#include "actioninitialization.hpp"

#include "eventaction.hpp"
#include "globals.hh"
#include "pga.hpp"
#include "runaction.hpp"

namespace dgg4 {
ActionInitialization::ActionInitialization() : G4VUserActionInitialization() {
  G4cout << "Creating ActionInitialization" << G4endl;
}

ActionInitialization::~ActionInitialization() {
  G4cout << "Deleting ActionInitialization" << G4endl;
}

void ActionInitialization::BuildForMaster() const {
  SetUserAction(new RunAction);
  return;
}

void ActionInitialization::Build() const {
  SetUserAction(new RunAction);
  SetUserAction(new PGA);
  SetUserAction(new EventAction);
  return;
}
}  // namespace dgg4
