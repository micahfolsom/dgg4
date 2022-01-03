#include "detectorconstruction.hpp"

#include <algorithm>

#include "G4Box.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4MTRunManager.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "ex1geometry.hpp"
#include "geometrymessenger.hpp"
#include "globals.hh"
#include "sensitivedetector.hpp"
#include "world.hpp"
using namespace std;

namespace dgg4 {
DetectorConstruction::DetectorConstruction()
    : G4VUserDetectorConstruction(), m_fSDInit(false), m_SDLVList(), m_SD() {
  G4cout << "Creating DetectorConstruction" << G4endl;
  // Default available SDs
  // The active detector volume (scintillator, semiconductor, etc)
  m_SDNames.emplace_back("detector_sd");
  // If something else is detecting the signal, e.g. a PMT or SiPM
  m_SDNames.emplace_back("readout_sd");
  // Other auxilliary structures such as light guides, detector housing, etc
  m_SDNames.emplace_back("mechanical_sd");
  // Whatever else that's not captured by the above
  m_SDNames.emplace_back("other_sd");

  m_messenger = make_unique<GeometryMessenger>(this);

  m_geometries["world"] = make_unique<World>();
  m_geometries["example1"] = make_unique<Ex1Geometry>();
}

DetectorConstruction::~DetectorConstruction() {
  G4cout << "Deleting DetectorConstruction" << G4endl;
}

G4VPhysicalVolume* DetectorConstruction::Construct() {
  G4cout << "DetectorConstruction::Construct()" << G4endl;
  // Initialize maps if we need to - only needs to be done once
  // We do so here because the constructor is a bit too early and doesn't
  // give the user the chance to change the number of threads via macro
  // command, whereas ConstructSDandField() is too late, since it's called
  // after Construct()
  if (!m_fSDInit) {
    auto const NTHREADS =
        G4MTRunManager::GetMasterRunManager()->GetNumberOfThreads();
    for (auto const& sdname : m_SDNames) {
      m_SD[sdname] = vector<SensitiveDetector*>(NTHREADS, nullptr);
      m_SDLVList[sdname] = list<G4String>();
    }
    m_fSDInit = true;
    G4cout << "Detected " << NTHREADS << " threads, initialized SD maps"
           << G4endl;
  }
  auto world = dynamic_pointer_cast<World>(m_geometries["world"]);
  world->build();
  auto selected = dynamic_pointer_cast<Ex1Geometry>(m_geometries["example1"]);
  selected->build(world->get_logical());
  return world->get_physical();
}
void DetectorConstruction::ConstructSDandField() {
  // Allocate and register SDs, if necessary
  auto const ithread = G4Threading::G4GetThreadId() + 1;
  G4cout << "ConstructSDandField() called from thread " << ithread << G4endl;
  auto sd_man = G4SDManager::GetSDMpointer();
  for (auto const& sdname : m_SDNames) {
    // Allocate and register with Geant4 - it will take care of the memory
    if (!m_SD[sdname][ithread]) {
      auto sd = new SensitiveDetector(sdname);
      m_SD[sdname][ithread] = sd;
      sd_man->AddNewDetector(sd);
      G4cout << "Allocated and registered SD " << sdname << " in thread "
             << ithread << G4endl;
    }
    // Loop over all of the LVs connected to this SD
    for (auto const& lvname : m_SDLVList[sdname]) {
      // If the LV doesn't exist (it should, Construct() was already called)
      // then we'll get a fatal exception here, which is right. That is a
      // hard-coded bug - any user input LV should be checked on the UI side
      // before being brought in
      SetSensitiveDetector(lvname, m_SD[sdname][ithread]);
    }
  }
  return;
}
void DetectorConstruction::set_sd(G4String const& lvname,
                                  G4String const& sdname) {
  // A bit easier than using std::find() on the list of SD names
  if (!m_SDLVList.contains(sdname)) {
    G4String msg = "The SD specified (" + sdname + ") does not exist";
    G4Exception("DetectorConstruction::set_sd()", "DGG4Warning", JustWarning,
                msg);
  }
  // Check if the LV is already on the list
  if (std::find(m_SDLVList[sdname].begin(), m_SDLVList[sdname].end(), lvname) !=
      m_SDLVList[sdname].end()) {
    G4String msg = "The LV " + lvname + " is already connected to " + sdname;
    G4Exception("DetectorConstruction::set_sd()", "DGG4Warning", JustWarning,
                msg);
  }
  m_SDLVList[sdname].push_back(lvname);
  G4cout << "Connected " << lvname << " to " << sdname << G4endl;
  return;
}
void DetectorConstruction::update_geometry() {
  G4cout << "Reloading geometry. This may take a moment...";
  auto rman = G4RunManager::GetRunManager();
  rman->ReinitializeGeometry(true);
  // Clear out Solids, LVs, and PVs. Also calls GeometryHasBeenModified()
  // Note that ReinitializeGeometry() does not catch the optical borders
  G4LogicalSkinSurface::CleanSurfaceTable();
  G4LogicalBorderSurface::CleanSurfaceTable();

  // Reset SD to LV links - geometric objects should connect LVs to SDs in
  // their build functions
  for (auto& [key, val] : m_SDLVList) {
    val.clear();
  }

  // If we are in vis mode, we need to re-run /run/initialize and run
  // some /vis commands to get back to a clean state where we are showing
  // the geometry. In batch mode, the RunManager will be re-initialized
  // when /run/beamOn is called, so it's not necessary to call explicitly
  if (G4VVisManager::GetConcreteInstance()) {
    G4UImanager::GetUIpointer()->ApplyCommand("/run/initialize");
    G4UImanager::GetUIpointer()->ApplyCommand("/vis/drawVolume");
    G4UImanager::GetUIpointer()->ApplyCommand("/vis/scene/add/trajectories");
    G4UImanager::GetUIpointer()->ApplyCommand("/vis/scene/add/hits");
    G4UImanager::GetUIpointer()->ApplyCommand(
        "/vis/scene/add/axes 0 0 0 10 cm");
    G4UImanager::GetUIpointer()->ApplyCommand(
        "/vis/scene/endOfEventAction accumulate 100");
  }
  G4cout << "...geometry reload complete." << G4endl;
  return;
}
bool DetectorConstruction::check_lv_exists(G4String const& lvname) const {
  // Connect SDs and LVs. This is a bit clunky - we need to loop over the
  // LVs to find the pointer to the one with the name we want
  auto lvstore = G4LogicalVolumeStore::GetInstance();
  bool found = false;
  for (auto ilv = lvstore->begin(); ilv != lvstore->end(); ++ilv) {
    if ((*ilv)->GetName() == lvname) {
      found = true;
    }
  }
  return found;
}
}  // namespace dgg4
