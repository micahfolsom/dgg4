#ifndef RUN_HPP
#define RUN_HPP
#include "G4Run.hh"

namespace dgg4 {
class Run : public G4Run {
 public:
  Run();
  ~Run();

  void RecordEvent(G4Event const* event);
  void Merge(G4Run const* from_run);
};
}  // namespace dgg4

#endif
