#include "hit.hpp"

namespace dgg4 {
G4ThreadLocal G4Allocator<Hit>* HitAllocator = nullptr;

Hit::Hit(int track_id, int parent_id, G4String const& volume,
         G4String const& particle, G4String const& process,
         G4ThreeVector const& position, float energy, double time)
    : m_eventID(-1),
      m_trackID(track_id),
      m_parentID(parent_id),
      m_volume(volume),
      m_particle(particle),
      m_process(process),
      m_position(position),
      m_energy(energy),
      m_time(time) {}

void Hit::set_event_id(int id) {
  m_eventID = id;
  return;
}

int Hit::get_event_id() const { return m_eventID; }

int Hit::get_track_id() const { return m_trackID; }

int Hit::get_parent_id() const { return m_parentID; }

G4String const& Hit::get_volume() const { return m_volume; }

G4String const& Hit::get_particle() const { return m_particle; }

G4String const& Hit::get_process() const { return m_process; }

G4ThreeVector const& Hit::get_position() const { return m_position; }

float Hit::get_energy() const { return m_energy; }

double Hit::get_time() const { return m_time; }
}  // namespace dgg4
