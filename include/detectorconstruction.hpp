#ifndef DETECTOR_CONSTRUCTION_HPP
#define DETECTOR_CONSTRUCTION_HPP
#include "G4VUserDetectorConstruction.hh"
#include "G4PVPlacement.hh"

namespace dgg4 {
  class DetectorConstruction: public G4VUserDetectorConstruction {
    public:
      DetectorConstruction();
      ~DetectorConstruction();
      G4PVPlacement* Construct() override final;
  };
}

#endif
