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
