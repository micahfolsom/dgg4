#include "runaction.hpp"

#include <fstream>

#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include "run.hpp"

using namespace std;
namespace dgg4 {
RunAction::RunAction() : m_path("hits.csv") {
  G4cout << "Creating RunAction" << G4endl;
}

RunAction::~RunAction() { G4cout << "Deleting RunAction" << G4endl; }

G4Run* RunAction::GenerateRun() { return dynamic_cast<G4Run*>(new Run); }

void RunAction::BeginOfRunAction(G4Run const*) {
  if (IsMaster()) {
    G4cout << "Starting run!" << G4endl;
  }
  return;
}

void RunAction::EndOfRunAction(G4Run const* run) {
  G4cout << "Ending run!" << G4endl;
  auto nevents = run->GetNumberOfEvent();
  if (nevents == 0) {
    return;
  }
  if (IsMaster()) {
    G4cout << "Finishing processing all " << nevents << " in main thread"
           << G4endl;
    auto our_run = dynamic_cast<Run const*>(run);
    write_csv(our_run->get_hits());
  }
  return;
}

void RunAction::write_csv(deque<Hit> const& hits) {
  ofstream out_file(m_path);
  out_file << "eventID,trackID,parentID,particle,creator_process,phys_volume,";
  out_file << "x[cm],y[cm],z[cm],energy[keV],time[ns]\n";
  for (auto const& hit : hits) {
    out_file << hit.get_event_id() << ",";
    out_file << hit.get_track_id() << ",";
    out_file << hit.get_parent_id() << ",";
    out_file << hit.get_process() << ",";
    out_file << hit.get_volume() << ",";
    out_file << hit.get_position().getX() / cm << ",";
    out_file << hit.get_position().getY() / cm << ",";
    out_file << hit.get_position().getZ() / cm << ",";
    out_file << hit.get_energy() / keV << ",";
    out_file << hit.get_time() / ns << "\n";
  }
  out_file.close();
  return;
}
}  // namespace dgg4
