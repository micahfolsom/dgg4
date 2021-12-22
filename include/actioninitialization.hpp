#ifndef ACTION_INITIALIZATION_HPP
#define ACTION_INITIALIZATION_HPP
#include "G4VUserActionInitialization.hh"

namespace dgg4 {
  class ActionInitialization: public G4VUserActionInitialization {
    public:
      ActionInitialization();
      ~ActionInitialization();
      void BuildForMaster() const override final;
      void Build() const override final;
  };
}

#endif
