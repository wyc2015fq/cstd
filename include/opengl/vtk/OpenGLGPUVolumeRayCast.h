
#include <cassert>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#define vtkNotUsed(x)  (x)
struct ImageData {
  int dim[4];
  int ndim;
  float spacing[4];
  float origin[4];
  uchar* data;
};

struct Volume {
  int asdf;
};

struct Plane {
  float3 Normal;
  float3 Origin;
};

void GetExtent(ImageData* im, int* extents)
{
  int i;

  for (i = 0; i < im->ndim; ++i) {
    extents[2 * i + 0] = 0;
    extents[2 * i + 0] = im->dim[i] - 1;
  }
}

#include "TextureObject.inl"

// Description:
// Empty implementation.
void GetReductionRatio(double* ratio)
{
  ratio[0] = ratio[1] = ratio[2] = 1.0;
}

#define VTK_MAX_VRCOMP 4

//----------------------------------------------------------------------------
struct OpenGLGPUVolumeRayCast { // :  public vtkGPUVolumeRayCastMapper
  Camera cam[1];

  int DisableGradientOpacity[VTK_MAX_VRCOMP];
  //int Shade[VTK_MAX_VRCOMP];
  double Ambient[VTK_MAX_VRCOMP];
  double Diffuse[VTK_MAX_VRCOMP];
  double Specular[VTK_MAX_VRCOMP];
  double SpecularPower[VTK_MAX_VRCOMP];

  int LockSampleDistanceToInputSpacing;
  int    AutoAdjustSampleDistances;
  float  ImageSampleDistance;
  float  MinimumImageSampleDistance;
  float  MaximumImageSampleDistance;

  // Render to texture mode flag
  int RenderToImage;

  // Depth image scalar type
  int DepthImageScalarType;

  // Clamp depth values to the depth of the face at which the ray
  // exits the volume
  int ClampDepthToBackface;

  // Enable / disable stochastic jittering
  int UseJittering;

  // The distance between sample points along the ray
  float  SampleDistance;

  int    SmallVolumeRender;
  double BigTimeToDraw;
  double SmallTimeToDraw;

  float FinalColorWindow;
  float FinalColorLevel;

  // 1 if we are generating the canonical image, 0 otherwise
  int   GeneratingCanonicalView;
  ImageData* CanonicalViewImageData;

  ImageData* MaskInput;
  Plane* ClippingPlanes;
  int nClippingPlanes;
  int ParallelProjection;
  BlendModeT BlendMode;
  bool UseDepthPass;
  int HasGradientOpacity;
  Light* Lights;
  int nLights;
  int IndependentComponents;
  int Shade;

  double ReductionFactor;
  Passes    CurrentPass;

  float          MaskBlendFactor;
  MaskTypeT      MaskType;

  int AMRMode;
  int Cropping;
  int CroppingRegionFlags;

  // Point data or cell data (or field data, not handled) ?
  int CellFlag;

  double         ClippedCroppingRegionPlanes[6];
  double         CroppingRegionPlanes[6];

  //vtkIdType MaxMemoryInBytes;
  float MaxMemoryFraction;

  bool           ReportProgress;

  //ImageData * TransformedInput;

  ImageData* NoiseGenerator;
  int NoiseTextureSize[2];

  // Private member variables
  //--------------------------------------------------------------------------

  bool ValidTransferFunction;
  bool LoadDepthTextureExtensionsSucceeded;
  bool CameraWasInsideInLastUpdate;
  bool HandleLargeDataTypes;

  GLuint CubeVBOId;
  GLuint CubeVAOId;
  GLuint CubeIndicesId;

  InterpolationTypeT InterpolationType;

  Texture VolumeTextureObject[1];
  Texture NoiseTextureObject[1];
  Texture DepthTextureObject[1];

  int TextureWidth;

  double Scale[4];
  double Bias[4];

  float* NoiseTextureData;

  float ActualSampleDistance;

  int LastProjectionParallel;
  int Dimensions[3];
  int TextureSize[3];
  int WindowLowerLeft[2];
  int WindowSize[2];
  int LastDepthPassWindowSize[2];
  int LastRenderToImageWindowSize[2];

  float ScalarsRange[4][2];
  float LoadedBounds[6];
  int Extents[6];
  float DatasetStepSize[3];
  float CellScale[3];
  float CellStep[3];
  float CellSpacing[3];

  int NumberOfLights;
  int LightComplexity;

  std::ostringstream ExtensionsStringStream;

  //vtkOpenGLVolumeRGBTables* RGBTables;
  Texture RGBTables[4];
  std::string RGBTablesMap[4];

  //vtkOpenGLVolumeOpacityTables* OpacityTables;
  Texture OpacityTables[4];
  std::string OpacityTablesMap[4];

  //vtkOpenGLVolumeRGBTable* Mask1RGBTable;
  //vtkOpenGLVolumeRGBTable* Mask2RGBTable;
  Texture Mask1RGBTable[1];
  Texture Mask2RGBTable[1];
  //vtkOpenGLVolumeGradientOpacityTables* GradientOpacityTables;
  Texture GradientOpacityTables[4];
  std::string GradientOpacityTablesMap[4];

  //vtkTimeStamp ShaderBuildTime;

  Matrix4x4 TextureToDataSetMat;
  Matrix4x4 InverseTextureToDataSetMat;

  Matrix4x4 InverseProjectionMat;
  Matrix4x4 InverseModelViewMat;
  Matrix4x4 InverseVolumeMat;

  Matrix4x4 TextureToEyeTransposeInverse;

  Matrix4x4 TempMatrix1;

  Matrix4x4 CellToPointMatrix;
  float AdjustedTexMin[4];
  float AdjustedTexMax[4];

  //vtkSmartPointer<vtkPolyData> BBoxPolyData;

  int MaskTextures[32];
  Texture* CurrentMask;

  int CurrentSelectionPass;
  bool IsPicking;

  GLuint ShaderProgram;
  //vtkOpenGLShaderCache* ShaderCache;

  GLuint FBO;
  Texture RTTDepthBufferTextureObject[1];
  Texture RTTDepthTextureObject[1];
  Texture RTTColorTextureObject[1];
  int RTTDepthTextureType;

  GLuint DPFBO;
  Texture DPDepthBufferTextureObject[1];
  Texture DPColorTextureObject[1];

  //vtkContourFilter*  ContourFilter;
  //vtkPolyDataMapper ContourMapper;
  //vtkActor ContourActor;
  int TwoSidedLighting;
};
