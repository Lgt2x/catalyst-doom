from paraview.simple import *

paraview.simple._DisableFirstRenderCameraReset()

from paraview.catalyst import get_args, get_execute_params

renderView1 = CreateView('RenderView')
renderView1.ViewSize = [640,400]
renderView1.AxesGrid = 'GridAxes3DActor'
renderView1.StereoType = 'Crystal Eyes'
renderView1.CameraFocalDisk = 1.0

SetActiveView(None)

layout1 = CreateLayout(name='Layout #1')
layout1.AssignView(0,renderView1)
layout1.SetSize(640,400)

SetActiveView(renderView1)

producer = PVTrivialProducer(registrationName="grid")
rep = Show(producer, renderView1, 'GeometryRepresentation')
rep.Representation = 'Point Gaussian'

ResetCamera()


SetActiveSource(producer)

from paraview import catalyst
options = catalyst.Options()
options.GlobalTrigger = 'TimeStep'
options.EnableCatalystLive = 1
options.CatalystLiveTrigger = 'TimeStep'
#
# def catalyst_execute(info):
#     global producer
#
#     producer.UpdatePipeline()
#
#     # get time parameter as example of a parameter changing during the simulation
#     params = get_execute_params()
#     # timeParam = float(params[3].split("=")[1])
#
#     # show that the time parameter matches the time tracked by catalyst
#     # assert (timeParam - info.time) < 1e-12
#
#     print("executing (cycle={}, time={})".format(info.cycle, info.time))
#     print("-----")
#     print("pipeline parameters:")
#     print("\n".join(params))
#     print("-----")
#     print("bounds:", producer.GetDataInformation().GetBounds())
#     print("velocity-magnitude-range:", producer.PointData["pixel"].GetRange(-1))
#     print("===================================")
