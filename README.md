# Dynamic Geometry Geant4 (dgg4) #

This is an example of a Geant4 simulation that employs dynamically-modifiable
geometry via UI commands, rather than having to re-run the application. The
user may input a number of geometry changes, then call `/dgg4/geometry/update`
to update it, which works from either a .mac file or the visualization. These
commands can be run _after `/run/initialize` is called_, instead of having to
do so beforehand!

Behind the scenes, the geometry is deleted and rebuilt from scratch. Then the
sensitive detectors are re-attached from their respective threads to whatever
logical volumes they map to.

Lots TODO here!

## Compilation ##

Make a build directory and run `cmake`, then run `make`

## Adding a Detector ##

TODO

## UI Commands ##

`/dgg4/run/save_data`<br>
Description: toggle saving hit data to disk<br>
Argument: bool<br>
Default: false<br>

`/dgg4/run/save_path`<br>
Description: set the path of the output data file<br>
Argument: string<br>
Default: hits.csv<br>

`/dgg4/geometry/update`<br>
Description: update/reload/reinitialize the geometry<br>
Argument: none<br>

## Output Data ##

Currently just a CSV. TODO: HDF5
