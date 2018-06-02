/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkOpenGLGPUVolumeRayCastMapper.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifndef vtkOpenGLGPUVolumeRayCastMapper_h
#define vtkOpenGLGPUVolumeRayCastMapper_h

//#include <vtkGPUVolumeRayCastMapper.h>

// Forward declarations
class vtkOpenGLCamera;
class vtkShaderProgram;
class vtkTextureObject;
class vtkGenericOpenGLResourceFreeCallback;
class vtkImplicitFunction;
class vtkImageData;

#define vtkGetMacro(name,type) \
  virtual type Get##name () { \
  return this->name; \
}

#define vtkSetVector2Macro(name,type) \
virtual void Set##name (type _arg1, type _arg2) { \
  if ((this->name[0] != _arg1)||(this->name[1] != _arg2)) { \
    this->name[0] = _arg1; \
    this->name[1] = _arg2; \
    Modified(this); \
  } \
} \
void Set##name (type _arg[2]) { \
  this->Set##name (_arg[0], _arg[1]); \
}

//----------------------------------------------------------------------------
class vtkOpenGLGPUVolumeRayCastMapper// :  public vtkGPUVolumeRayCastMapper
{
public:

  enum Passes
  {
    RenderPass,
    DepthPass = 1
  };

  vtkTextureObject* GetDepthTexture();
  vtkTextureObject* GetColorTexture();
  void GetDepthImage(vtkImageData* im);
  void GetColorImage(vtkImageData* im);

  // Description:
  // Mapper can have multiple passes and internally it will set
  // the state. The state can not be set externally explicitly
  // but can be set indirectly depending on the options set by
  // the user.
  vtkGetMacro(CurrentPass, int);

  //@{
  /**
   * Sets a user defined function to generate the ray jittering noise.
   * vtkPerlinNoise is used by default with a texture size equivlent to
   * the window size. These settings will have no effect when UseJittering
   * is Off.
   */
  void SetNoiseGenerator(vtkImplicitFunction* generator);
  vtkSetVector2Macro(NoiseTextureSize, int);
  //@}

public:
  vtkOpenGLGPUVolumeRayCastMapper();
  ~vtkOpenGLGPUVolumeRayCastMapper();

  //virtual void ReleaseGraphicsResources(vtkWindow *window);
  vtkGenericOpenGLResourceFreeCallback *ResourceCallback;

  // Description:
  // Build vertex and fragment shader for the volume rendering
  void BuildDepthPassShader(vtkRenderer* ren, vtkVolume* vol,
                            int noOfComponents,
                            int independentComponents);

  // Description:
  // Build vertex and fragment shader for the volume rendering
  void BuildShader(vtkRenderer* ren, vtkVolume* vol, int noOfCmponents);

  // TODO Take these out as these are no longer needed
  // Methods called by the AMR Volume Mapper.
  virtual void PreRender(vtkRenderer * vtkNotUsed(ren),
                         vtkVolume *vtkNotUsed(vol),
                         double vtkNotUsed(datasetBounds)[6],
                         double vtkNotUsed(scalarRange)[2],
                         int vtkNotUsed(noOfComponents),
                         unsigned int vtkNotUsed(numberOfLevels)) {};

  // \pre input is up-to-date
  virtual void RenderBlock(vtkRenderer *vtkNotUsed(ren),
                           vtkVolume *vtkNotUsed(vol),
                           unsigned int vtkNotUsed(level)) {}

  virtual void PostRender(vtkRenderer *vtkNotUsed(ren),
                          int vtkNotUsed(noOfComponents)) {}

  // Description:
  // Rendering volume on GPU
  void GPURender(vtkRenderer *ren, vtkVolume *vol);

  // Description:
  // Method that performs the actual rendering given a volume and a shader
  void DoGPURender(vtkRenderer* ren,
                   vtkVolume* vol,
                   vtkImageData* input,
                   vtkOpenGLCamera* cam,
                   vtkShaderProgram* shaderProgram,
                   int noOfComponents,
                   int independentComponents);

  // Description:
  // Update the reduction factor of the render viewport (this->ReductionFactor)
  // according to the time spent in seconds to render the previous frame
  // (this->TimeToDraw) and a time in seconds allocated to render the next
  // frame (allocatedTime).
  // \pre valid_current_reduction_range: this->ReductionFactor>0.0 && this->ReductionFactor<=1.0
  // \pre positive_TimeToDraw: this->TimeToDraw>=0.0
  // \pre positive_time: allocatedTime>0
  // \post valid_new_reduction_range: this->ReductionFactor>0.0 && this->ReductionFactor<=1.0
  void ComputeReductionFactor(double allocatedTime);

  // Description:
  // Empty implementation.
  void GetReductionRatio(double* ratio)
  {
    ratio[0] = ratio[1] = ratio[2] = 1.0;
  }


  // Description:
  // Empty implementation.
  virtual int IsRenderSupported(vtkRenderWindow *vtkNotUsed(window),
                                vtkVolumeProperty *vtkNotUsed(property))
  {
    return 1;
  }

  double ReductionFactor;
  int    CurrentPass;

private:
  class vtkInternal;
  vtkInternal* Impl;

  vtkImplicitFunction* NoiseGenerator;
  int NoiseTextureSize[2];

  vtkOpenGLGPUVolumeRayCastMapper(
    const vtkOpenGLGPUVolumeRayCastMapper&) VTK_DELETE_FUNCTION;
  void operator=(const vtkOpenGLGPUVolumeRayCastMapper&) VTK_DELETE_FUNCTION;
};

#endif // vtkOpenGLGPUVolumeRayCastMapper_h
