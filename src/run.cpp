#include "run.hpp"

#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4THitsCollection.hh"
#include "G4UnitsTable.hh"
#include "hit.hpp"

using namespace std;
namespace dgg4 {
Run::Run() : G4Run(), m_hits() { G4cout << "Creating Run object" << G4endl; }

Run::~Run() { G4cout << "Deleting Run object" << G4endl; }

void Run::RecordEvent(G4Event const* event) {
  auto hc_id = G4SDManager::GetSDMpointer()->GetCollectionID("det_sd_hits");
  if (hc_id == -1) {
    G4cerr << "HC ID was < 0; it was not Initialized()'d!" << G4endl;
    return;
  }
  auto hc_ev = event->GetHCofThisEvent();
  if (!hc_ev) {
    G4cerr << "No HitsCollection for this event! ID: " << event->GetEventID()
           << G4endl;
    return;
  }
  auto hc = (G4THitsCollection<Hit>*)hc_ev->GetHC(hc_id);
  if (!hc) {
    G4cerr << "Failed to grab HitsCollection object" << G4endl;
    return;
  }

  G4cout << "Event had " << hc->entries() << " hits" << G4endl;
  // Hit doesn't have a constructor without args, so we use reserve()
  // to pre-allocate, instead of initializing with the # of elements
  m_hits = vector<Hit>();
  m_hits.reserve(hc->entries());
  for (std::size_t ihit = 0; ihit < hc->entries(); ++ihit) {
    auto hit_in = dynamic_cast<Hit*>((*hc)[ihit]);
    hit_in->set_event_id(event->GetEventID());
    // DEBUG
    G4cout << "Event ID: " << hit_in->get_event_id() << "\n";
    G4cout << "Track ID: " << hit_in->get_track_id() << "\n";
    G4cout << "Parent ID: " << hit_in->get_parent_id() << "\n";
    G4cout << "Volume: " << hit_in->get_volume() << "\n";
    G4cout << "Particle: " << hit_in->get_particle() << "\n";
    G4cout << "Creator process: " << hit_in->get_process() << "\n";
    G4cout << "Position: " << G4BestUnit(hit_in->get_position(), "Length")
           << "\n";
    G4cout << "Energy: " << G4BestUnit(hit_in->get_energy(), "Energy") << "\n";
    G4cout << "Time: " << G4BestUnit(hit_in->get_time(), "Time") << "\n"
           << G4endl;
    // DEBUG
    m_hits.emplace_back(*hit_in);
  }
  G4Run::RecordEvent(event);
  return;
}

void Run::Merge(G4Run const* from_run) {
  auto other_run = dynamic_cast<Run const*>(from_run);
  auto other_hits = other_run->get_hits();
  m_hits.insert(m_hits.end(), other_hits.begin(), other_hits.end());
  G4Run::Merge(from_run);
  return;
}

vector<Hit> Run::get_hits() const { return m_hits; }
}  // namespace dgg4
