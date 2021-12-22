#ifndef RUN_ACTION_HPP
#define RUN_ACTION_HPP
#include "G4UserRunAction.hh"

namespace dgg4 {
class RunAction : public G4UserRunAction {
 public:
   RunAction();
   ~RunAction();
  G4Run* GenerateRun() override final;
  void BeginOfRunAction(G4Run const*) override final;
  void EndOfRunAction(G4Run const* run) override final;
};
}  // namespace dgg4

#endif