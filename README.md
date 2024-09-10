# Catalyst-Doom

PoC of Doom running on Paraview-Catalyst. Someone had to do it.

To run this, you'll need :
 - A version of Paraview built with Catalyst enabled (pre-built binaries are fine)
 - [Catalyst2](https://catalyst-in-situ.readthedocs.io/en/latest/build_and_install.html)

## Build the project
```bash
mkdir build && cd build
cmake ../doomgeneric -Dcatalyst_DIR=<path_to_Catalyst2> && cmake --build .
cp ../DOOM1.WAD .
```

## Run the game
```bash
CATALYST_IMPLEMENTATION_NAME=paraview CATALYST_IMPLEMENTATION_PATHS=<path_to_Paraview_Catalyst> ./doomgeneric <path_to_catalyst_pipeline.py> <absolute_path_to_catalyst_proxy.xml>
```

Launch Paraview, load macro `macro.py` using `Macro > Import new Macro...` and proxy `catalyst_proxy.xml` using `Tools > Manage plugins > Load New`.

Start Paraview-Catalyst using the menu `Catalyst > Connect`, click the left icon associated with "catalyst: frame" in the pipeline browser, click the eye and apply the loaded macro.

You can control the game by selecting SteeringParameters in the pipeline browser and changing direction or fire values, and clicking on "Apply".
