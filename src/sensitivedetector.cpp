#include "sensitivedetector.hpp"

#include "G4UnitsTable.hh"
#include "G4VProcess.hh"

namespace dgg4 {
SensitiveDetector::SensitiveDetector(G4String const& name)
    : G4VSensitiveDetector(name), m_id(-1), m_hitsCollection(nullptr) {
  G4String hc_name = name + "_hits";
  collectionName.insert(hc_name);
}

SensitiveDetector::~SensitiveDetector() {}

void SensitiveDetector::Initialize(G4HCofThisEvent* hc) {
  // Check if we have a valid ID
  if (m_id < 0) {
    m_id = GetCollectionID(0);
  }
  // Create a hits collection for this SD
  m_hitsCollection =
      new HitsCollection(SensitiveDetectorName, collectionName[0]);
  // Give the collection to the G4Event, so we can get it at the end
  // in Run::RecordEvent()
  hc->AddHitsCollection(m_id, m_hitsCollection);
  return;
}

bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*) {
  auto track = step->GetTrack();
  // Need class scope because G4VSensitiveDetector has a Hit() member function
  auto hit = new dgg4::Hit(
      track->GetTrackID(), track->GetParentID(), track->GetVolume()->GetName(),
      track->GetDefinition()->GetParticleName(),
      (track->GetCreatorProcess() ? track->GetCreatorProcess()->GetProcessName()
                                  : "source"),
      track->GetPosition(), step->GetTotalEnergyDeposit(),
      track->GetGlobalTime());
  m_hitsCollection->insert(hit);
  return true;
}
}  // namespace dgg4
