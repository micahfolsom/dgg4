#ifndef HIT_HPP
#define HIT_HPP
#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include "G4VHit.hh"
#include "h5hit.hpp"

namespace dgg4 {
class Hit : public G4VHit {
 public:
  Hit(int trackid, int parent_id, G4String const& volume,
      G4String const& particle, G4String const& process,
      G4ThreeVector const& position, float energy, double time);

  inline void* operator new(std::size_t);
  inline void operator delete(void* hit);

  // Event ID is different because it's set elsewhere (not easy to access in
  // SD::ProcessHits())
  void set_event_id(int id);
  int get_event_id() const;
  int get_track_id() const;
  int get_parent_id() const;
  G4String const& get_volume() const;
  G4String const& get_particle() const;
  G4String const& get_process() const;
  G4ThreeVector const& get_position() const;
  float get_energy() const;
  double get_time() const;

 private:
  // Units are baked in below, where applicable. They're divided out in the IO
  // step at the very end
  /// G4Event ID
  int m_eventID;
  /// G4Track ID
  int m_trackID;
  /// G4Track ID of the parent
  int m_parentID;
  /// Name of the physical volume (G4PVPlacement)
  G4String m_volume;
  /// Particle name
  G4String m_particle;
  /// Process that created this particle
  G4String m_process;
  /// Global position
  G4ThreeVector m_position;
  /// Energy deposited in the step
  float m_energy;
  /// Global time
  double m_time;
};
/*! Alias HitsCollection as G4THitsCollection<Hit>. */
typedef G4THitsCollection<Hit> HitsCollection;
/*! Thread-local pointer for the Hit allocator. */
extern G4ThreadLocal G4Allocator<Hit>* HitAllocator;

// The two functions below must be defined *after* HitAllocator. */
inline void* Hit::operator new(std::size_t) {
  // If there's no allocator yet, make one, then return memory for
  // a new Hit
  if (!HitAllocator) {
    HitAllocator = new G4Allocator<Hit>;
  }
  return (void*)HitAllocator->MallocSingle();
}
inline void Hit::operator delete(void* a_hit) {
  HitAllocator->FreeSingle((Hit*)a_hit);
  return;
}
}  // namespace dgg4

#endif
