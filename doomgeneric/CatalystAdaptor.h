#ifndef CatalystAdaptor_h
#define CatalystAdaptor_h

#include "doomgeneric.h"
#include <catalyst.hpp>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

namespace CatalystAdaptor {

void Initialize(int argc, char *argv[]) {
  conduit_cpp::Node node;
  node["catalyst/scripts/script"].set_string("../catalyst_pipeline.py");
  node["catalyst/proxies/proxy"].set_string("../catalyst_proxy.xml");

  catalyst_status err = catalyst_initialize(conduit_cpp::c_node(&node));
  if (err != catalyst_status_ok) {
    std::cerr << "ERROR: Failed to initialize Catalyst: " << err << std::endl;
  }
}

void AddSteerableChannel(conduit_cpp::Node &exec_params) {
  auto steerable = exec_params["catalyst/channels/steerable"];
  steerable["type"].set("mesh");

  auto steerable_mesh = steerable["data"];
  steerable_mesh["coordsets/coords/type"].set_string("explicit");
  steerable_mesh["coordsets/coords/values/x"].set_float64_vector({1});
  steerable_mesh["coordsets/coords/values/y"].set_float64_vector({2});
  steerable_mesh["coordsets/coords/values/z"].set_float64_vector({3});
  steerable_mesh["topologies/mesh/type"].set("unstructured");
  steerable_mesh["topologies/mesh/coordset"].set("coords");
  steerable_mesh["topologies/mesh/elements/shape"].set("point");
  steerable_mesh["topologies/mesh/elements/connectivity"].set_int32_vector({0});
  steerable_mesh["fields/steerable/association"].set("vertex");
  steerable_mesh["fields/steerable/topology"].set("mesh");
  steerable_mesh["fields/steerable/volume_dependent"].set("false");
  steerable_mesh["fields/steerable/values"].set_int32_vector({2});
}

void Execute(int cycle, uint32_t *DG_ScreenBuffer) {
  conduit_cpp::Node exec_params;

  auto state = exec_params["catalyst/state"];
  state["timestep"].set(cycle);
  state["time"].set((double)cycle);

  AddSteerableChannel(exec_params);

  auto channel = exec_params["catalyst/channels/frame"];

  channel["type"].set("mesh");
  auto mesh = channel["data"];

  mesh["coordsets/coords/type"].set("uniform");

  mesh["coordsets/coords/dims/i"].set(DOOMGENERIC_RESX);
  mesh["coordsets/coords/dims/j"].set(DOOMGENERIC_RESY);

  mesh["topologies/mesh/type"].set("uniform");
  mesh["topologies/mesh/coordset"].set("coords");

  auto fields = mesh["fields"];
  fields["pixel/association"].set("vertex");
  fields["pixel/topology"].set("mesh");
  fields["pixel/volume_dependent"].set("false");
  fields["pixel/values"].set_external(DG_ScreenBuffer,
                                      DOOMGENERIC_RESY * DOOMGENERIC_RESX, 1,
                                      2 * sizeof(uint32_t));

  catalyst_status err = catalyst_execute(conduit_cpp::c_node(&exec_params));
  if (err != catalyst_status_ok) {
    std::cerr << "ERROR: Failed to execute Catalyst: " << err << std::endl;
  }
}

int Steer(unsigned int timeStep) {
  conduit_cpp::Node results;
  catalyst_status err = catalyst_results(conduit_cpp::c_node(&results));
  if (err != catalyst_status_ok) {
    std::cerr << "Failed to execute Catalyst: " << err << std::endl;
    return -1;
  }

  const std::string direction_path =
      "catalyst/steerable/fields/direction/values";
  const std::string fire_path = "catalyst/steerable/fields/fire/values";

  int direction, fire;
  if (results.has_path(direction_path) && results.has_path(fire_path)) {

    direction = results[direction_path].as_int();
    fire = results[fire_path].as_int();

  } else {
    // Waiting for user to click on "SteerableParameters" in the pipeline browser
    return -1;
  }

  return 5*(fire%2) + direction;
}

} // namespace CatalystAdaptor

#endif
