#ifndef RUN_HPP
#define RUN_HPP
#include <deque>

#include "G4Run.hh"
#include "hit.hpp"

namespace dgg4 {
class Run : public G4Run {
 public:
  Run();
  ~Run();

  void RecordEvent(G4Event const* event);
  /// Merge another Run into this Run. Copies the Hits
  void Merge(G4Run const* from_run);

  std::deque<Hit> get_hits() const;

 private:
  std::deque<Hit> m_hits;
};
}  // namespace dgg4

#endif
