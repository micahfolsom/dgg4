#ifndef RUN_ACTION_HPP
#define RUN_ACTION_HPP
#include <deque>

#include "G4UserRunAction.hh"
#include "hit.hpp"

namespace dgg4 {
class RunAction : public G4UserRunAction {
 public:
  RunAction();
  ~RunAction();
  G4Run* GenerateRun() override final;
  void BeginOfRunAction(G4Run const*) override final;
  void EndOfRunAction(G4Run const* run) override final;

 private:
  void write_csv(std::deque<Hit> const& hits);
  G4String m_path;
};
}  // namespace dgg4

#endif
