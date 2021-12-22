#ifndef SENSITIVE_DETECTORER_HPP
#define SENSITIVE_DETECTORER_HPP

#include "G4VSensitiveDetector.hh"
#include "hit.hpp"

namespace dgg4 {
class SensitiveDetector : public G4VSensitiveDetector {
 public:
  SensitiveDetector(G4String const& name);
  ~SensitiveDetector();

  void Initialize(G4HCofThisEvent* hc);
  bool ProcessHits(G4Step* step, G4TouchableHistory*);

 private:
  int m_id;
  HitsCollection* m_hitsCollection;
};
}  // namespace dgg4

#endif
