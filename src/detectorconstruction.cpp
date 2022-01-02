#include "detectorconstruction.hpp"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4MTRunManager.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include "sensitivedetector.hpp"
using namespace std;

namespace dgg4 {
DetectorConstruction::DetectorConstruction()
    : G4VUserDetectorConstruction(), m_fSDInit(false), m_SDLVList(), m_SD() {
  // Default available SDs
  // The active detector volume (scintillator, semiconductor, etc)
  m_SDNames.emplace_back("detector_sd");
  // If something else is detecting the signal, e.g. a PMT or SiPM
  m_SDNames.emplace_back("readout_sd");
  // Other auxilliary structures such as light guides, detector housing, etc
  m_SDNames.emplace_back("mechanical_sd");
  // Whatever else that's not captured by the above
  m_SDNames.emplace_back("other_sd");
  G4cout << "Creating DetectorConstruction" << G4endl;
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
        G4MTRunManager::GetMasterRunManager()->GetNumberOfThreads() + 1;
    for (auto const& sdname : m_SDNames) {
      m_SD[sdname] = vector<SensitiveDetector*>(NTHREADS, nullptr);
      m_SDLVList[sdname] = list<G4String>();
    }
    m_fSDInit = true;
    G4cout << "Detected " << NTHREADS << " threads, initialized SD maps"
           << G4endl;
  }
  // World
  auto world_solid = new G4Box("world_solid", 0.5 * m, 0.5 * m, 0.5 * m);
  auto nist = G4NistManager::Instance();
  auto world_mat = nist->FindOrBuildMaterial("G4_AIR");
  auto world_log = new G4LogicalVolume(world_solid, world_mat, "world_log");
  auto world_phys =
      new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0), world_log,
                        "world_phys", nullptr, false, 0, true);

  // Detector
  auto det_solid = new G4Box("det_solid", 5 * cm, 5 * cm, 5 * cm);
  auto det_mat = nist->FindOrBuildMaterial("G4_SODIUM_IODIDE");
  auto det_log = new G4LogicalVolume(det_solid, det_mat, "det_log");
  m_SDLVList["detector_sd"].emplace_back("det_log");
  new G4PVPlacement(nullptr, G4ThreeVector(0, 0, -20 * cm), det_log, "det_phys",
                    world_log, false, 0, true);
  return world_phys;
}
void DetectorConstruction::ConstructSDandField() {
  // Allocate and register SDs, if necessary
  auto const ithread = G4Threading::G4GetThreadId() + 1;
  G4cout << "ConstructSDandField() called from thread " << ithread << G4endl;
  auto sd_man = G4SDManager::GetSDMpointer();
  auto lvstore = G4LogicalVolumeStore::GetInstance();
  for (auto const& sdname : m_SDNames) {
    // Was this SD allocated in this thread yet?
    if (!m_SD[sdname][ithread]) {
      auto sd = new SensitiveDetector(sdname);
      m_SD[sdname][ithread] = sd;
      sd_man->AddNewDetector(sd);
      G4cout << "Allocated and registered SD " << sdname << "in thread "
             << ithread << G4endl;
    }
    // Loop over all of the LVs connected to this SD
    for (auto const& lvname : m_SDLVList[sdname]) {
      // Connect SDs and LVs. This is a bit clunky - we need to loop over the
      // LVs to find the pointer to the one with the name we want
      bool found = false;
      for (auto ilv = lvstore->begin(); ilv != lvstore->end(); ++ilv) {
        if ((*ilv)->GetName() == lvname) {
          found = true;
          (*ilv)->SetSensitiveDetector(m_SD[sdname][ithread]);
          G4cout << "Connected SD=" << sdname << " to LV=" << lvname << G4endl;
        }
      }
      if (!found) {
        G4Exception("DetectorConstruction::ConstructSDandField()",
                    "DGG4Warning", JustWarning,
                    "A logical volume name attached to an SD did not exist. "
                    "Probably a typo somewhere?");
      }
    }
  }
  return;
}
}  // namespace dgg4
