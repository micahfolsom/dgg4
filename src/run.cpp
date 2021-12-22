#include "run.hpp"

#include "G4Event.hh"
#include "run.hpp"

namespace dgg4 {
Run::Run() : G4Run() { G4cout << "Creating Run object" << G4endl; }

Run::~Run() { G4cout << "Deleting Run object" << G4endl; }

void Run::RecordEvent(G4Event const* event) {
  G4Run::RecordEvent(event);
  return;
}

void Run::Merge(G4Run const* from_run) {
  G4Run::Merge(from_run);
  return;
}
}  // namespace dgg4
