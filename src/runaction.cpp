#include "runaction.hpp"

#include <hdf5.h>

#include <fstream>

#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include "run.hpp"
#include "runmessenger.hpp"

using namespace std;
namespace dgg4 {
RunAction::RunAction() : m_fSave(false), m_path("hits.h5") {
  G4cout << "Creating RunAction" << G4endl;
  m_messenger = new RunMessenger(this);
}

RunAction::~RunAction() {
  G4cout << "Deleting RunAction" << G4endl;
  delete m_messenger;
}

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
    if (m_fSave) {
      auto our_run = dynamic_cast<Run const*>(run);
      write_h5(our_run->get_hits());
    }
  }
  return;
}

/* Deprecated, but left in case someone finds it useful
void RunAction::write_csv(deque<Hit> const& hits) {
  ofstream out_file(m_path);
  out_file << "eventID,trackID,parentID,particle,creator_process,phys_volume,";
  out_file << "x[cm],y[cm],z[cm],energy[keV],time[ns]\n";
  for (auto const& hit : hits) {
    out_file << hit.get_event_id() << ",";
    out_file << hit.get_track_id() << ",";
    out_file << hit.get_parent_id() << ",";
    out_file << hit.get_particle() << ",";
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
*/
void RunAction::write_h5(deque<Hit> const& hits) {
  // Copy hit data, splitting into separate arrays
  vector<int> event_id(hits.size());
  vector<int> track_id(hits.size());
  vector<int> parent_id(hits.size());
  vector<char const*> volume(hits.size());
  vector<char const*> particle(hits.size());
  vector<char const*> process(hits.size());
  vector<float> x(hits.size());
  vector<float> y(hits.size());
  vector<float> z(hits.size());
  vector<float> energy(hits.size());
  vector<double> ts(hits.size());
  for (size_t i = 0; i < hits.size(); ++i) {
    event_id[i] = hits[i].get_event_id();
    track_id[i] = hits[i].get_track_id();
    parent_id[i] = hits[i].get_parent_id();
    volume[i] = hits[i].get_volume().c_str();
    particle[i] = hits[i].get_particle().c_str();
    process[i] = hits[i].get_process().c_str();
    x[i] = hits[i].get_position().getX() / cm;
    y[i] = hits[i].get_position().getY() / cm;
    z[i] = hits[i].get_position().getZ() / cm;
    energy[i] = hits[i].get_energy() / keV;
    ts[i] = hits[i].get_time() / ns;
  }

  // hdf5 handles
  hid_t hfile, hdataset, hdataspace;
  hsize_t dims[] = {hits.size()};

  // rank = 1 (single dimension/linear arrays)
  hdataspace = H5Screate_simple(1, dims, nullptr);
  hfile = H5Fcreate(m_path.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

  // Variable-length strings
  hid_t string_t = H5Tcopy(H5T_C_S1);
  H5Tset_size(string_t, H5T_VARIABLE);

  // Create a dataset for each item for easy linear access
  hdataset = H5Dcreate2(hfile, "event_id", H5T_NATIVE_INT, hdataspace,
                        H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  H5Dwrite(hdataset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
           event_id.data());
  H5Dclose(hdataset);
  hdataset = H5Dcreate2(hfile, "track_id", H5T_NATIVE_INT, hdataspace,
                        H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  H5Dwrite(hdataset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
           track_id.data());
  H5Dclose(hdataset);
  hdataset = H5Dcreate2(hfile, "parent_id", H5T_NATIVE_INT, hdataspace,
                        H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  H5Dwrite(hdataset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
           parent_id.data());
  H5Dclose(hdataset);
  hdataset = H5Dcreate2(hfile, "volume", string_t, hdataspace, H5P_DEFAULT,
                        H5P_DEFAULT, H5P_DEFAULT);
  H5Dwrite(hdataset, string_t, H5S_ALL, H5S_ALL, H5P_DEFAULT, volume.data());
  H5Dclose(hdataset);
  hdataset = H5Dcreate2(hfile, "particle", string_t, hdataspace, H5P_DEFAULT,
                        H5P_DEFAULT, H5P_DEFAULT);
  H5Dwrite(hdataset, string_t, H5S_ALL, H5S_ALL, H5P_DEFAULT, particle.data());
  H5Dclose(hdataset);
  hdataset = H5Dcreate2(hfile, "process", string_t, hdataspace, H5P_DEFAULT,
                        H5P_DEFAULT, H5P_DEFAULT);
  H5Dwrite(hdataset, string_t, H5S_ALL, H5S_ALL, H5P_DEFAULT, process.data());
  H5Dclose(hdataset);
  hdataset = H5Dcreate2(hfile, "x", H5T_NATIVE_FLOAT, hdataspace, H5P_DEFAULT,
                        H5P_DEFAULT, H5P_DEFAULT);
  H5Dwrite(hdataset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, x.data());
  H5Dclose(hdataset);
  hdataset = H5Dcreate2(hfile, "y", H5T_NATIVE_FLOAT, hdataspace, H5P_DEFAULT,
                        H5P_DEFAULT, H5P_DEFAULT);
  H5Dwrite(hdataset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, y.data());
  H5Dclose(hdataset);
  hdataset = H5Dcreate2(hfile, "z", H5T_NATIVE_FLOAT, hdataspace, H5P_DEFAULT,
                        H5P_DEFAULT, H5P_DEFAULT);
  H5Dwrite(hdataset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, z.data());
  H5Dclose(hdataset);
  hdataset = H5Dcreate2(hfile, "energy", H5T_NATIVE_FLOAT, hdataspace,
                        H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  H5Dwrite(hdataset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
           energy.data());
  H5Dclose(hdataset);
  hdataset = H5Dcreate2(hfile, "timestamp", H5T_NATIVE_FLOAT, hdataspace,
                        H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  H5Dwrite(hdataset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
           ts.data());
  H5Dclose(hdataset);

  // Clean up
  H5Tclose(string_t);
  H5Sclose(hdataspace);
  H5Fclose(hfile);
  return;
}
bool RunAction::save_data() const { return m_fSave; }
void RunAction::save_data(bool save) {
  m_fSave = save;
  return;
}
G4String RunAction::get_path() const { return m_path; }
void RunAction::set_path(G4String const& path) {
  m_path = path;
  return;
}
}  // namespace dgg4
