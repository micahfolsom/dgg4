#ifndef PGA_HPP
#define PGA_HPP
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"

namespace dgg4 {
class PGA : public G4VUserPrimaryGeneratorAction {
 public:
  PGA();
  ~PGA();

  void GeneratePrimaries(G4Event* event) override final;

 private:
  G4GeneralParticleSource m_gun;
};
}  // namespace dgg4

#endif
