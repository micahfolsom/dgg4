#include "pga.hpp"

#include "G4Gamma.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

namespace dgg4 {
PGA::PGA()
    : G4VUserPrimaryGeneratorAction(),
      m_gun() {
  G4cout << "Creating PGA (G4GeneralParticleSource)" << G4endl;
}
PGA::~PGA() {
  G4cout << "Deleting PGA" << G4endl;
}

void PGA::GeneratePrimaries(G4Event* event) {
  m_gun.GeneratePrimaryVertex(event);
  return;
}
}  // namespace dgg4
