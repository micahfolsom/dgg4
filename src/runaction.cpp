#include "runaction.hpp"

#include "globals.hh"
#include "run.hpp"

namespace dgg4 {
RunAction::RunAction() { G4cout << "Creating RunAction" << G4endl; }

RunAction::~RunAction() { G4cout << "Deleting RunAction" << G4endl; }

G4Run* RunAction::GenerateRun() { return dynamic_cast<G4Run*>(new Run); }

void RunAction::BeginOfRunAction(G4Run const*) {
  if (IsMaster()) {
    G4cout << "Starting run!" << G4endl;
  }
  return;
}

void RunAction::EndOfRunAction(G4Run const* run) {
  G4cout << "Ending run!" << G4endl;
  auto nevents = run->GetNumberOfEvent();
  if (nevents == 0) {
    return;
  }
  if (IsMaster()) {
    G4cout << "Finishing processing all " << nevents << " in main thread"
           << G4endl;
  }
  return;
}
}  // namespace dgg4
