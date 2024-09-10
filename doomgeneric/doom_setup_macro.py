# trace generated using paraview version 5.11.1-RC1-906-gbbaa7a81bd
#import paraview
#paraview.compatibility.major = 5
#paraview.compatibility.minor = 11

#### import the simple module from the paraview
from paraview.simple import *
#### disable automatic camera reset on 'Show'
paraview.simple._DisableFirstRenderCameraReset()

# get active source.
extractframe = GetActiveSource()

# get active view
renderView2 = GetActiveViewOrCreate('RenderView')

# get display properties
extractframeDisplay = GetDisplayProperties(extractframe, view=renderView2)

# change representation type
extractframeDisplay.SetRepresentationType('Surface')

# set scalar coloring
ColorBy(extractframeDisplay, ('POINTS', 'pixel'))

# rescale color and/or opacity maps used to include current data range
extractframeDisplay.RescaleTransferFunctionToDataRange(True, False)

# show color bar/color legend
extractframeDisplay.SetScalarBarVisibility(renderView2, True)

# get color transfer function/color map for 'pixel'
pixelLUT = GetColorTransferFunction('pixel')

# get opacity transfer function/opacity map for 'pixel'
pixelPWF = GetOpacityTransferFunction('pixel')

# get 2D transfer function for 'pixel'
pixelTF2D = GetTransferFunction2D('pixel')

renderView2.ResetActiveCameraToPositiveZ()

# reset view to fit data
renderView2.ResetCamera(False, 0.9)

renderView2.AdjustRoll(-90.0)

renderView2.AdjustRoll(-90.0)

# hide color bar/color legend
extractframeDisplay.SetScalarBarVisibility(renderView2, False)

# Properties modified on extractframeDisplay
extractframeDisplay.MapScalars = 0

#================================================================
# addendum: following script captures some of the application
# state to faithfully reproduce the visualization during playback
#================================================================

# get layout
layout1 = GetLayout()

#--------------------------------
# saving layout sizes for layouts

# layout/tab size in pixels
layout1.SetSize(1285, 783)

#-----------------------------------
# saving camera placements for views

# current camera placement for renderView2
renderView2.InteractionMode = '2D'
renderView2.CameraPosition = [319.5, 199.5, -1455.3422066349567]
renderView2.CameraFocalPoint = [319.5, 199.5, 0.0]
renderView2.CameraViewUp = [4.440892098500626e-16, -1.0, 0.0]
renderView2.CameraParallelScale = 257.27086962547287

#--------------------------------------------
# uncomment the following to render all views
# RenderAllViews()
# alternatively, if you want to write images, you can use SaveScreenshot(...).