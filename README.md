# Dynamic Geometry Geant4 (dgg4) #

This is an example of a Geant4 simulation that employs dynamically-modifiable
geometry via UI commands, rather than having to re-run the application. The
user may input a number of geometry changes, then call `/dgg4/geometry/reload`
to update it, which works from either a .mac file or the visualization. These
commands can be run _after `/run/initialize` is called_, instead of having to
do so beforehand!

Behind the scenes, the geometry is deleted and rebuilt from scratch. Then the
sensitive detectors are re-attached from their respective threads to whatever
logical volumes they map to.

Lots TODO here!

## Compilation ##

TODO

## Adding a Detector ##

## UI Commands ##

TODO

## Output Data ##

TODO
