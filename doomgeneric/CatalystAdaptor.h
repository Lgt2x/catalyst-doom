#ifndef CatalystAdaptor_h
#define CatalystAdaptor_h

#include <catalyst.hpp>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdint>

namespace CatalystAdaptor
{

void Initialize(int argc, char* argv[])
{
    std::cout << "INIT !";
    conduit_cpp::Node node;
    node["catalyst/scripts/script"].set_string("../catalyst_pipeline.py");

    catalyst_status err = catalyst_initialize(conduit_cpp::c_node(&node));
    if (err != catalyst_status_ok)
    {
        std::cout << "ERROR: Failed to initialize Catalyst: " << err << std::endl;
    }

    std::cout << "Error ? " << err << std::endl;

}

void Execute(int cycle, double time, uint32_t* DG_ScreenBuffer)
{

    std::cout << "Exec ?" << std::endl;
    conduit_cpp::Node exec_params;

    auto state = exec_params["catalyst/state"];
    state["timestep"].set(cycle);
    state["time"].set(time);

    auto channel = exec_params["catalyst/channels/frame"];

    channel["type"].set("mesh"); // Conduit type
    auto mesh = channel["data"];

    // start with coordsets (of course, the sequence is not important, just make
    // it easier to think in this order).
    mesh["coordsets/coords/type"].set("uniform");

    mesh["coordsets/coords/dims/i"].set(DOOMGENERIC_RESX);
    mesh["coordsets/coords/dims/j"].set(DOOMGENERIC_RESY);
    mesh["coordsets/coords/dims/k"].set(1);

    // Next, add topology
    mesh["topologies/mesh/type"].set("uniform");
    mesh["topologies/mesh/coordset"].set("coords");

    // Finally, add fields.
    auto fields = mesh["fields"];
    fields["pixel/association"].set("point");
    fields["pixel/topology"].set("mesh");
    fields["pixel/volume_dependent"].set("false");

    // velocity is stored in non-interlaced form (unlike points).
    fields["velocity/values/x"].set_external(DG_ScreenBuffer, DOOMGENERIC_RESX*DOOMGENERIC_RESY, 0, 4);
    fields["velocity/values/y"].set_external(DG_ScreenBuffer, DOOMGENERIC_RESX*DOOMGENERIC_RESY, 1, 4);
    fields["velocity/values/z"].set_external(DG_ScreenBuffer, DOOMGENERIC_RESX*DOOMGENERIC_RESY, 3, 4);

    catalyst_status err = catalyst_execute(conduit_cpp::c_node(&exec_params));
    if (err != catalyst_status_ok)
    {
        std::cerr << "ERROR: Failed to execute Catalyst: " << err << std::endl;
    }
}

void Finalize()
{
    conduit_cpp::Node node;
    catalyst_status err = catalyst_finalize(conduit_cpp::c_node(&node));
    if (err != catalyst_status_ok)
    {
        std::cerr << "ERROR: Failed to finalize Catalyst: " << err << std::endl;
    }
}
}

#endif
