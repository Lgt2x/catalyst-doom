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
        std::cerr << "ERROR: Failed to initialize Catalyst: " << err << std::endl;
    }

    std::cout << "Error ? " << err << std::endl;

}

void Execute(int cycle, uint32_t* DG_ScreenBuffer)
{

//    std::cout << "Exec" << std::endl;
    conduit_cpp::Node exec_params;

    auto state = exec_params["catalyst/state"];
    state["timestep"].set(cycle);
    state["time"].set((double)cycle);

    auto channel = exec_params["catalyst/channels/grid"];

    channel["type"].set("mesh"); // Conduit type
    auto mesh = channel["data"];

    // start with coordsets (of course, the sequence is not important, just make
    // it easier to think in this order).
    mesh["coordsets/coords/type"].set("uniform");

//    mesh["coordsets/coords/dims/i"].set(DOOMGENERIC_RESX);
//    mesh["coordsets/coords/dims/j"].set(DOOMGENERIC_RESY);

    mesh["coordsets/coords/dims/i"].set(DOOMGENERIC_RESX);
    mesh["coordsets/coords/dims/j"].set(DOOMGENERIC_RESY);

    // Next, add topology
    mesh["topologies/mesh/type"].set("uniform");
    mesh["topologies/mesh/coordset"].set("coords");

    // Finally, add fields.
    auto fields = mesh["fields"];
    fields["pixel/association"].set("vertex");
    fields["pixel/topology"].set("mesh");
    fields["pixel/volume_dependent"].set("false");

//    int values[9] = {1,2,3,4,5,6,7,8,9};
//    fields["pixel/values"].set_external(values, 9,0,sizeof(int));
    fields["pixel/values"].set_external(DG_ScreenBuffer, DOOMGENERIC_RESY*DOOMGENERIC_RESX, 1, 2*sizeof(uint32_t));
//    fields["pixel/values/y"].set_external(DG_ScreenBuffer, DOOMGENERIC_RESX*DOOMGENERIC_RESY, 1, 4);
//    fields["pixel/values/z"].set_external(DG_ScreenBuffer, DOOMGENERIC_RESX*DOOMGENERIC_RESY, 3, 4);

//    std::cout <<exec_params.to_yaml() << std::endl;
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
