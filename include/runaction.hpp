#ifndef RUN_ACTION_HPP
#define RUN_ACTION_HPP
#include <deque>

#include "G4UserRunAction.hh"
#include "hit.hpp"

namespace dgg4 {
class RunMessenger;
class RunAction : public G4UserRunAction {
 public:
  RunAction();
  ~RunAction();
  G4Run* GenerateRun() override final;
  void BeginOfRunAction(G4Run const*) override final;
  void EndOfRunAction(G4Run const* run) override final;

  bool save_data() const;
  void save_data(bool save);
  G4String get_path() const;
  void set_path(G4String const& path);

 private:
  void write_csv(std::deque<Hit> const& hits);
  bool m_fSave;
  G4String m_path;
  RunMessenger* m_messenger;
};
}  // namespace dgg4

#endif
