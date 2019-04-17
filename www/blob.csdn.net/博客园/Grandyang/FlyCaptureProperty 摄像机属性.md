# FlyCaptureProperty 摄像机属性 - Grandyang - 博客园







# [FlyCaptureProperty 摄像机属性](https://www.cnblogs.com/grandyang/p/4053976.html)







An enumeration of the different camera properties that can be set via the API.

Declaration

```
enum FlyCaptureProperty 
{ 
　　FLYCAPTURE_BRIGHTNESS, 
　　FLYCAPTURE_AUTO_EXPOSURE, 
　　FLYCAPTURE_SHARPNESS, 
　　FLYCAPTURE_WHITE_BALANCE, 
　　FLYCAPTURE_HUE, 
　　FLYCAPTURE_SATURATION, 
　　FLYCAPTURE_GAMMA, 
　　FLYCAPTURE_IRIS, 
　　FLYCAPTURE_FOCUS, 
　　FLYCAPTURE_ZOOM, 
　　FLYCAPTURE_PAN, 
　　FLYCAPTURE_TILT, 
　　FLYCAPTURE_SHUTTER, 
　　FLYCAPTURE_GAIN, 
　　FLYCAPTURE_SOFTWARE_WHITEBALANCE, 
}
```


|FLYCAPTURE_AUTO_EXPOSURE|The auto exposure property of the camera.|
|----|----|
|FLYCAPTURE_BRIGHTNES|The brightness property of the camera.|
|FLYCAPTURE_FOCUS|The focus property of the camera.|
|FLYCAPTURE_GAIN|The gain property of the camera.|
|FLYCAPTURE_GAMMA|The gamma property of the camera.|
|FLYCAPTURE_HUE|The hue property of the camera.|
|FLYCAPTURE_IRIS|The iris property of the camera.|
|FLYCAPTURE_PAN|The pan property of the camera.|
|FLYCAPTURE_SATURATION|The saturation property of the camera.|
|FLYCAPTURE_SHARPNESS|The sharpness property of the camera.|
|FLYCAPTURE_SHUTTER|The shutter property of the camera.|
|FLYCAPTURE_SOFTWARE_WHITEBALANCE|Software white balance property. Use this to manipulate the values for software whitebalance. This is only applicable to cameras that do not do on board color processing. On these cameras, hardware white balance is disabled.|
|FLYCAPTURE_TILT|The tilt property of the camera.|
|FLYCAPTURE_WHITE_BALANC|The hardware white balance property of the camera.|
|FLYCAPTURE_ZOOM|The zoom property of the camera.|

Remarks

A lot of these properties are included only for completeness and future expandability, and will have no effect on a PGR single lens Camera.



Reference:

[http://www.cs.unc.edu/Research/stc/FAQs/Cameras_Lenses/PtGrey/pgrFlyCaptureManual-v1.4.pdf](http://www.cs.unc.edu/Research/stc/FAQs/Cameras_Lenses/PtGrey/pgrFlyCaptureManual-v1.4.pdf)














