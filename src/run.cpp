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
  for (std::size_t ihit = 0; ihit < hc->entries(); ++ihit) {
    auto hit_in = dynamic_cast<Hit*>((*hc)[ihit]);
    hit_in->set_event_id(event->GetEventID());
    m_hits.push_back(*hit_in);
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

deque<Hit> Run::get_hits() const { return m_hits; }
}  // namespace dgg4
