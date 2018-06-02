//----------------------------------------------------------------------------
void DeleteTransferFunctions(OpenGLGPUVolumeRayCast* s)
{
#if 0
  delete s->RGBTables;
  s->RGBTables = NULL;
  if (s->Mask1RGBTable) {
    s->Mask1RGBTable->Delete();
    s->Mask1RGBTable = NULL;
  }
  if (s->Mask2RGBTable) {
    s->Mask2RGBTable->Delete();
    s->Mask2RGBTable = NULL;
  }
  delete s->OpacityTables;
  s->OpacityTables = NULL;
  delete s->GradientOpacityTables;
  s->GradientOpacityTables = NULL;
#endif
}
// Constructor
//--------------------------------------------------------------------------
void vtkInternal_init(OpenGLGPUVolumeRayCast* s)
{
  s->ValidTransferFunction = false;
  s->LoadDepthTextureExtensionsSucceeded = false;
  s->CameraWasInsideInLastUpdate = false;
  s->InterpolationType = Linear;
  s->TextureWidth = 1024;
  s->ActualSampleDistance = 1.0;
  //s->RGBTables = 0;
  s->Dimensions[0] = s->Dimensions[1] = s->Dimensions[2] = -1;
  s->TextureSize[0] = s->TextureSize[1] = s->TextureSize[2] = -1;
  s->WindowLowerLeft[0] = s->WindowLowerLeft[1] = 0;
  s->WindowSize[0] = s->WindowSize[1] = 0;
  s->LastDepthPassWindowSize[0] = s->LastDepthPassWindowSize[1] = 0;
  s->LastRenderToImageWindowSize[0] = 0;
  s->LastRenderToImageWindowSize[1] = 0;
  s->ScalarsRange[0][0] = s->ScalarsRange[0][1] = 0.0;
  s->ScalarsRange[1][0] = s->ScalarsRange[1][1] = 0.0;
  s->ScalarsRange[2][0] = s->ScalarsRange[2][1] = 0.0;
  s->ScalarsRange[3][0] = s->ScalarsRange[3][1] = 0.0;
  s->CurrentSelectionPass = MIN_KNOWN_PASS - 1;
  s->CellScale[0] = s->CellScale[1] = s->CellScale[2] = 0.0;
  s->NoiseTextureData = NULL;
  s->NumberOfLights = 0;
  s->LightComplexity = 0;
  s->Extents[0] = INT_MAX;
  s->Extents[1] = INT_MIN;
  s->Extents[2] = INT_MAX;
  s->Extents[3] = INT_MIN;
  s->Extents[4] = INT_MAX;
  s->Extents[5] = INT_MIN;
  Identity44(s->CellToPointMatrix);
  s->AdjustedTexMin[0] = s->AdjustedTexMin[1] = s->AdjustedTexMin[2] = 0.0f;
  s->AdjustedTexMin[3] = 1.0f;
  s->AdjustedTexMax[0] = s->AdjustedTexMax[1] = s->AdjustedTexMax[2] = 1.0f;
  s->AdjustedTexMax[3] = 1.0f;
  //s->MaskTextures = 0;
  s->Scale;
  s->Bias;
  //s->ShaderCache = 0;
  s->FBO = 0;
  //s->RTTDepthBufferTextureObject = 0;
  //s->RTTDepthTextureObject = 0;
  //s->RTTColorTextureObject = 0;
  s->RTTDepthTextureType = -1;
  s->DPFBO = 0;
  //s->DPDepthBufferTextureObject = 0;
  //s->DPColorTextureObject = 0;
}
#include "FrameBufferObject2.inl"
// Destructor
//--------------------------------------------------------------------------
void vtkInternal_free(OpenGLGPUVolumeRayCast* s)
{
  FREE(s->NoiseTextureData);
  DestroyTexture(s->NoiseTextureObject);
  DestroyTexture(s->DepthTextureObject);
  DestroyFBO(&s->FBO);
  DestroyTexture(s->RTTDepthBufferTextureObject);
  DestroyTexture(s->RTTDepthTextureObject);
  DestroyTexture(s->RTTColorTextureObject);
  DeleteTransferFunctions(s);
}
//----------------------------------------------------------------------------
void Initialize(OpenGLGPUVolumeRayCast* s, int noOfComponents, int independentComponents)
{
  DeleteTransferFunctions(s);
  // Create RGB lookup table
  if (noOfComponents > 1 && independentComponents) {
    //s->RGBTables = new vtkOpenGLVolumeRGBTables(noOfComponents);
  }
  else {
    //s->RGBTables = new vtkOpenGLVolumeRGBTables(1);
  }
  if (s->MaskInput != 0 && s->MaskType == LabelMapMaskType) {
    if (s->Mask1RGBTable == NULL) {
      //s->Mask1RGBTable = vtkOpenGLVolumeRGBTable::New();
    }
    if (s->Mask2RGBTable == NULL) {
      //s->Mask2RGBTable = vtkOpenGLVolumeRGBTable::New();
    }
  }
  // We support upto four components
  if (noOfComponents > 1 && independentComponents) {
    //s->OpacityTables = new vtkOpenGLVolumeOpacityTables(noOfComponents);
  }
  else {
    //s->OpacityTables = new vtkOpenGLVolumeOpacityTables(1);
  }
  if (noOfComponents > 1 && independentComponents) {
    // Assuming that all four components has gradient opacity for now
    //s->GradientOpacityTables = new vtkOpenGLVolumeGradientOpacityTables(noOfComponents);
  }
  else {
    if (s->HasGradientOpacity) {
      //s->GradientOpacityTables = new vtkOpenGLVolumeGradientOpacityTables(1);
    }
  }
  std::ostringstream numeric;
  for (int i = 0; i < noOfComponents; ++i) {
    numeric << i;
    if (i > 0) {
      s->OpacityTablesMap[i] = std::string("in_opacityTransferFunc") + numeric.str();
      s->RGBTablesMap[i] = std::string("in_colorTransferFunc") + numeric.str();
      s->GradientOpacityTablesMap[i] = std::string("in_gradientTransferFunc") + numeric.str();
    }
    else {
      s->OpacityTablesMap[i] = std::string("in_opacityTransferFunc");
      s->RGBTablesMap[i] = std::string("in_colorTransferFunc");
      s->GradientOpacityTablesMap[i] = std::string("in_gradientTransferFunc");
    }
    numeric.str("");
    numeric.clear();
  }
}
//----------------------------------------------------------------------------
int UpdateInterpolationType(OpenGLGPUVolumeRayCast* s)
{
  return 0;
}
//----------------------------------------------------------------------------
bool LoadVolume(OpenGLGPUVolumeRayCast* s, const int* TextureSize, const void* imageData, int noOfComponents, int scalarType)
{
  // Allocate data with internal format and foramt as (GL_RED)
  GLint internalFormat = 0;
  GLenum format = 0;
  GLenum type = 0;
  s->HandleLargeDataTypes = false;
  DestroyTexture(s->VolumeTextureObject);
  // Get the default choices for format from the texture
  format = GetDefaultFormat(scalarType, noOfComponents, false);
  internalFormat = GetDefaultInternalFormat(scalarType, noOfComponents, false);
  type = GetDefaultDataType(scalarType);
  bool supportsFloat = false;
#if GL_ES_VERSION_2_0 != 1
  if (glewIsSupported("GL_ARB_texture_float") || GetContextSupportsOpenGL32()) {
    supportsFloat = true;
  }
#elif GL_ES_VERSION_3_0 == 1
  supportsFloat = true;
#endif
  // scale and bias
  // NP = P*scale + bias
  // given two point matcvhes a,b to c,d the fomula
  // is scale = (d-c)/(b-a) and
  // bias = c - a*scale
  // for unsigned/float types c is zero
  double* bias = s->Bias;
  double* scale = s->Scale;
  double oglScale = 1.0;
  double oglBias = 0.0;
  switch (scalarType) {
  case VTK_FLOAT:
    if (supportsFloat) {
      switch (noOfComponents) {
      case 1:
        internalFormat = GL_R16F;
        format = GL_RED;
        break;
      case 2:
        internalFormat = GL_RG16F;
        format = GL_RG;
        break;
      case 3:
        internalFormat = GL_RGB16F;
        format = GL_RGB;
        break;
      case 4:
        internalFormat = GL_RGBA16F;
        format = GL_RGBA;
        break;
      }
    }
    else {
      switch (noOfComponents) {
      case 1:
        internalFormat = GL_RED;
        format = GL_RED;
        break;
      case 2:
        internalFormat = GL_RG;
        format = GL_RG;
        break;
      case 3:
        internalFormat = GL_RGB;
        format = GL_RGB;
        break;
      case 4:
        internalFormat = GL_RGBA;
        format = GL_RGBA;
        break;
      }
    }
    break;
  case VTK_UCHAR:
    oglScale = 1.0 / (UCHAR_MAX + 1);
    oglBias = 0.0;
    break;
  case VTK_SCHAR:
  case VTK_CHAR:
    oglScale = 2.0 / (UCHAR_MAX + 1);
    oglBias = -1.0 - CHAR_MIN * oglScale;
    break;
  case VTK_BIT:
    // not supported
    assert("check: impossible case" && 0);
    break;
  case VTK_ID_TYPE:
    // not supported
    assert("check: impossible case" && 0);
    break;
  case VTK_INT:
  case VTK_DOUBLE:
  case VTK_INT64:
  case VTK_LONG:
  case VTK_LONG_LONG:
  case VTK_UINT:
  case VTK_UINT64:
  case VTK_ULONG:
  case VTK_ULONG_LONG:
    s->HandleLargeDataTypes = true;
    type = GL_FLOAT;
    switch (noOfComponents) {
    case 1:
      if (supportsFloat) {
        internalFormat = GL_R16F;
      }
      else {
        internalFormat = GL_RED;
      }
      format = GL_RED;
      break;
    case 2:
      internalFormat = GL_RG;
      format = GL_RG;
      break;
    case 3:
      internalFormat = GL_RGB;
      format = GL_RGB;
      break;
    case 4:
      internalFormat = GL_RGBA;
      format = GL_RGBA;
      break;
    }
    break;
  case VTK_SHORT:
    oglScale = 2.0 / (USHORT_MAX + 1);
    oglBias = -1.0 - SHORT_MIN * oglScale;
    break;
  case VTK_STRING:
    // not supported
    assert("check: impossible case" && 0);
    break;
  case VTK_USHORT:
    oglScale = 1.0 / (USHORT_MAX + 1);
    oglBias = 0.0;
    break;
  default:
    assert("check: impossible case" && 0);
    break;
  }
  for (int n = 0; n < noOfComponents; ++n) {
    double oglA = s->ScalarsRange[n][0] * oglScale + oglBias;
    double oglB = s->ScalarsRange[n][1] * oglScale + oglBias;
    scale[n] = 1.0 / (oglB - oglA);
    bias[n] = 0.0 - oglA * scale[n];
  }
  // Update scale and bias
  //s->Scale = scale;
  //s->Bias = bias;
  // Update texture size
  // imageData->GetExtent(s->Extents);
  s->Extents[0] = 0;
  s->Extents[1] = TextureSize[0] - 1;
  s->Extents[2] = 0;
  s->Extents[3] = TextureSize[1] - 1;
  s->Extents[4] = 0;
  s->Extents[5] = TextureSize[2] - 1;
  // Point data or cell data (or field data, not handled) ?
  int CellFlag = 0;
  if (CellFlag) {
    int i = 1;
    while (i < 6) {
      s->Extents[i]--;
      i += 2;
    }
  }
  int i = 0;
  while (i < 3) {
    s->TextureSize[i] = s->Extents[2 * i + 1] - s->Extents[2 * i] + 1;
    ++i;
  }
  //s->VolumeTextureObject->SetDataType(type);
  //s->VolumeTextureObject->SetFormat(format);
  //s->VolumeTextureObject->SetInternalFormat(internalFormat);
  UpdateInterpolationType(s);
  if (!s->HandleLargeDataTypes) {
    const void* dataPtr = imageData;
    Create3DFromRaw(s->VolumeTextureObject, s->TextureSize[0], s->TextureSize[1], s->TextureSize[2], noOfComponents, scalarType, dataPtr);
    Activate(s->VolumeTextureObject);
    SetWrapS(s->VolumeTextureObject, ClampToEdge);
    SetWrapT(s->VolumeTextureObject, ClampToEdge);
    SetWrapR(s->VolumeTextureObject, ClampToEdge);
    SetMagnificationFilter(s->VolumeTextureObject, s->InterpolationType);
    SetMinificationFilter(s->VolumeTextureObject, s->InterpolationType);
    SetBorderColor(s->VolumeTextureObject, 0.0f, 0.0f, 0.0f, 0.0f);
  }
  else {
    // Convert and send to the GPU, z-slice by z-slice so that we won't allocate
    // memory at once.Allocate memory on the GPU (NULL data pointer with the
    // right dimensions). Here we are assuming that
    // GL_ARB_texture_non_power_of_two is available
    Create3DFromRaw(s->VolumeTextureObject, s->TextureSize[0], s->TextureSize[1], s->TextureSize[2], noOfComponents, scalarType, NULL);
    Activate(s->VolumeTextureObject);
    SetWrapS(s->VolumeTextureObject, ClampToEdge);
    SetWrapT(s->VolumeTextureObject, ClampToEdge);
    SetWrapR(s->VolumeTextureObject, ClampToEdge);
    SetMagnificationFilter(s->VolumeTextureObject, s->InterpolationType);
    SetMinificationFilter(s->VolumeTextureObject, s->InterpolationType);
    SetBorderColor(s->VolumeTextureObject, 0.0f, 0.0f, 0.0f, 0.0f);
#if 0
    // Send the slices one by one to the GPU. We are not sending all of them
    // together so as to avoid allocating big data on the GPU which may not
    // work if the original dataset is big as well.
    float* sliceArray = NULL;
    sliceArray = (float*)malloc(sizeof(float) * noOfComponents * s->TextureSize[0] * s->TextureSize[1]);
    void* slicePtr = sliceArray;
    int k = 0;
    int kInc = (s->Dimensions[0] - s->CellFlag) * (s->Dimensions[1] - s->CellFlag);
    int kOffset = (s->Extents[4] * (s->Dimensions[1] - s->CellFlag) + s->Extents[2]) * (s->Dimensions[0] - s->CellFlag) + s->Extents[0];
    float tupPtr[4];
    ASSERT(noOfComponents <= 4);
    while (k < s->TextureSize[2]) {
      int j = 0;
      int jOffset = 0;
      int jDestOffset = 0;
      while (j < s->TextureSize[1]) {
        i = 0;
        while (i < s->TextureSize[0]) {
          double* scalarPtr = scalars->GetTuple(kOffset + jOffset + i);
          for (int n = 0; n < noOfComponents; ++n) {
            tupPtr[n] = scalarPtr[n] * scale[n] + bias[n];
          }
          sliceArray->SetTuple(jDestOffset + i, tupPtr);
          ++i;
        }
        ++j;
        jOffset += s->Dimensions[0] - s->CellFlag;
        jDestOffset += s->TextureSize[0];
      }
      // Here we are assuming that GL_ARB_texture_non_power_of_two is
      // available
      glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, k,
          s->TextureSize[0], s->TextureSize[1], 1,
          format, type, slicePtr);
      ++k;
      kOffset += kInc;
    }
    free(tupPtr);
    free(sliceArray);
#endif
  }
  // do not tie up the texture unit unless we are activly using it
  // textures can exist without being active
  Deactivate(s->VolumeTextureObject);
  return 1;
}
//-----------------------------------------------------------------------------
bool LoadMask(OpenGLGPUVolumeRayCast* s, ImageData* maskInput, int textureExtent[6])
{
  bool result = true;
#if 0
  if (maskInput &&
      (maskInput->GetMTime() > s->MaskUpdateTime)) {
    // Find the texture.
    std::map<ImageData*, vtkVolumeMask*>::iterator it2 =
        s->MaskTextures->Map.find(maskInput);
    vtkVolumeMask* mask = 0;
    if (it2 == s->MaskTextures->Map.end()) {
      mask = new vtkVolumeMask();
      s->MaskTextures->Map[maskInput] = mask;
    }
    else {
      mask = (*it2).second;
    }
    mask->Update(s,
        maskInput,
        s->CellFlag,
        textureExtent,
        s->ScalarMode,
        s->ArrayAccessMode,
        s->ArrayId,
        s->ArrayName,
        static_cast<vtkIdType>(static_cast<float>(
            s->MaxMemoryInBytes) *
            s->MaxMemoryFraction));
    result = result && mask->IsLoaded();
    s->CurrentMask = mask;
    s->MaskUpdateTime.Modified();
  }
#endif
  return result;
}
//----------------------------------------------------------------------------
void ComputeBounds(OpenGLGPUVolumeRayCast* s, ImageData* input)
{
  double origin[3];
  VEC3CPY(s->CellSpacing, input->spacing);
  VEC3CPY(origin, input->origin);
  GetExtent(input, s->Extents);
  if (s->CellFlag) {
    int i = 1;
    while (i < 6) {
      s->Extents[i]--;
      i += 2;
    }
  }
  int swapBounds[3];
  swapBounds[0] = (s->CellSpacing[0] < 0);
  swapBounds[1] = (s->CellSpacing[1] < 0);
  swapBounds[2] = (s->CellSpacing[2] < 0);
  // Loaded data represents points
  if (!s->CellFlag) {
    // If spacing is negative, we may have to rethink the equation
    // between real point and texture coordinate...
    s->LoadedBounds[0] = origin[0] + (double)(s->Extents[0 + swapBounds[0]]) * s->CellSpacing[0];
    s->LoadedBounds[2] = origin[1] + (double)(s->Extents[2 + swapBounds[1]]) * s->CellSpacing[1];
    s->LoadedBounds[4] = origin[2] + (double)(s->Extents[4 + swapBounds[2]]) * s->CellSpacing[2];
    s->LoadedBounds[1] = origin[0] + (double)(s->Extents[1 - swapBounds[0]]) * s->CellSpacing[0];
    s->LoadedBounds[3] = origin[1] + (double)(s->Extents[3 - swapBounds[1]]) * s->CellSpacing[1];
    s->LoadedBounds[5] = origin[2] + (double)(s->Extents[5 - swapBounds[2]]) * s->CellSpacing[2];
  }
  // Loaded extents represent cells
  else {
    int wholeTextureExtent[6];
    GetExtent(input, wholeTextureExtent);
    int i = 1;
    while (i < 6) {
      wholeTextureExtent[i]--;
      i += 2;
    }
    i = 0;
    while (i < 3) {
      if (s->Extents[2 * i] == wholeTextureExtent[2 * i]) {
        s->LoadedBounds[2 * i + swapBounds[i]] = origin[i];
      }
      else {
        s->LoadedBounds[2 * i + swapBounds[i]] = origin[i] + ((double)(s->Extents[2 * i]) + 0.5) * s->CellSpacing[i];
      }
      if (s->Extents[2 * i + 1] == wholeTextureExtent[2 * i + 1]) {
        s->LoadedBounds[2 * i + 1 - swapBounds[i]] = origin[i] + ((double)(s->Extents[2 * i + 1]) + 1.0) * s->CellSpacing[i];
      }
      else {
        s->LoadedBounds[2 * i + 1 - swapBounds[i]] = origin[i] + ((double)(s->Extents[2 * i + 1]) + 0.5) * s->CellSpacing[i];
      }
      ++i;
    }
  }
}
//----------------------------------------------------------------------------
int UpdateVolume(OpenGLGPUVolumeRayCast* s)
{
  {
    InterpolationTypeT interpolationType = s->InterpolationType;
    UpdateInterpolationType(s);
    if (interpolationType != s->InterpolationType) {
      Activate(s->VolumeTextureObject);
      SetMagnificationFilter(s->VolumeTextureObject, s->InterpolationType);
      SetMinificationFilter(s->VolumeTextureObject, s->InterpolationType);
    }
  }
  return 0;
}
//----------------------------------------------------------------------------
int UpdateColorTransferFunction(OpenGLGPUVolumeRayCast* s, int component)
{
#if 0
  // Build the colormap in a 1D texture.
  // 1D RGB-texture=mapping from scalar values to color values
  // build the table.
  vtkColorTransferFunction* colorTransferFunction =
      s->GetRGBTransferFunction(component);
  // Add points only if its not being added before
  if (colorTransferFunction->GetSize() < 1) {
    colorTransferFunction->AddRGBPoint(s->ScalarsRange[component][0], 0.0, 0.0, 0.0);
    colorTransferFunction->AddRGBPoint(s->ScalarsRange[component][1], 1.0, 1.0, 1.0);
  }
  int filterVal =
      s->GetInterpolationType() == VTK_LINEAR_INTERPOLATION ?
      Linear : Nearest;
  double scalarRange[2];
  for (int i = 0; i < 2; ++i) {
    scalarRange[i] = s->ScalarsRange[component][i];
  }
  s->RGBTables->GetTable(component)->Update(
      s->GetRGBTransferFunction(component),
      scalarRange,
#if GL_ES_VERSION_2_0 != 1
      filterVal,
#else
      Nearest,
#endif
      vtkOpenGLRenderWindow::SafeDownCast(s->GetRenderWindow()));
  if (s->MaskInput != 0 &&
      s->MaskType == LabelMapMaskType) {
    vtkColorTransferFunction* colorTransferFunc =
        s->GetRGBTransferFunction(1);
    s->Mask1RGBTable->Update(colorTransferFunc, scalarRange,
        Nearest,
        vtkOpenGLRenderWindow::SafeDownCast(
            s->GetRenderWindow()));
    colorTransferFunc = s->GetRGBTransferFunction(2);
    s->Mask2RGBTable->Update(colorTransferFunc, scalarRange,
        Nearest,
        vtkOpenGLRenderWindow::SafeDownCast(
            s->GetRenderWindow()));
  }
#endif
  return 0;
}
//----------------------------------------------------------------------------
int UpdateOpacityTransferFunction(OpenGLGPUVolumeRayCast* s, unsigned int component)
{
#if 0
  if (!vol) {
    return 1;
  }
  vtkVolumeProperty* s = vol->GetProperty();
  // Transfer function table index based on whether independent / dependent
  // components. If dependent, use the first scalar opacity transfer function
  unsigned int lookupTableIndex = s->GetIndependentComponents() ?
      component : 0;
  vtkPiecewiseFunction* scalarOpacity =
      s->GetScalarOpacity(lookupTableIndex);
  if (scalarOpacity->GetSize() < 1) {
    scalarOpacity->AddPoint(s->ScalarsRange[component][0], 0.0);
    scalarOpacity->AddPoint(s->ScalarsRange[component][1], 0.5);
  }
  int filterVal =
      s->GetInterpolationType() == VTK_LINEAR_INTERPOLATION ?
      Linear : Nearest;
  double scalarRange[2];
  for (int i = 0; i < 2; ++i) {
    scalarRange[i] = s->ScalarsRange[component][i];
  }
  s->OpacityTables->GetTable(lookupTableIndex)->Update(
      scalarOpacity, s->BlendMode,
      s->ActualSampleDistance,
      scalarRange,
      s->GetScalarOpacityUnitDistance(component),
#if GL_ES_VERSION_2_0 != 1
      filterVal,
#else
      Nearest,
#endif
      vtkOpenGLRenderWindow::SafeDownCast(s->GetRenderWindow()));
#endif
  return 0;
}
//----------------------------------------------------------------------------
int UpdateGradientOpacityTransferFunction(OpenGLGPUVolumeRayCast* s, unsigned int component)
{
#if 0
  if (!vol) {
    return 1;
  }
  vtkVolumeProperty* s = vol->GetProperty();
  // Transfer function table index based on whether independent / dependent
  // components. If dependent, use the first gradient opacity transfer function
  unsigned int lookupTableIndex = s->GetIndependentComponents() ?
      component : 0;
  // TODO Currently we expect the all of the tables will
  // be initialized once and if at that time, the gradient
  // opacity was not enabled then it is not used later.
  if (!s->HasGradientOpacity(lookupTableIndex) ||
      !s->GradientOpacityTables) {
    return 1;
  }
  vtkPiecewiseFunction* gradientOpacity =
      s->GetGradientOpacity(lookupTableIndex);
  if (gradientOpacity->GetSize() < 1) {
    gradientOpacity->AddPoint(s->ScalarsRange[component][0], 0.0);
    gradientOpacity->AddPoint(s->ScalarsRange[component][1], 0.5);
  }
  int filterVal =
      s->GetInterpolationType() == VTK_LINEAR_INTERPOLATION ?
      Linear : Nearest;
  double scalarRange[2];
  for (int i = 0; i < 2; ++i) {
    scalarRange[i] = s->ScalarsRange[component][i];
  }
  s->GradientOpacityTables->GetTable(lookupTableIndex)->Update(
      gradientOpacity,
      s->ActualSampleDistance,
      scalarRange,
      s->GetScalarOpacityUnitDistance(component),
#if GL_ES_VERSION_2_0 != 1
      filterVal,
#else
      Nearest,
#endif
      vtkOpenGLRenderWindow::SafeDownCast(s->GetRenderWindow()));
#endif
  return 0;
}
//----------------------------------------------------------------------------
void CreateNoiseTexture(OpenGLGPUVolumeRayCast* s)
{
#if 0
  vtkOpenGLRenderWindow* glWindow = vtkOpenGLRenderWindow::SafeDownCast(
      s->GetRenderWindow());
  if (!s->NoiseTextureObject) {
    s->NoiseTextureObject = New();
  }
  s->NoiseTextureObject->SetContext(glWindow);
  bool updateSize = false;
  bool useUserSize = s->NoiseTextureSize[0] > 0 &&
      s->NoiseTextureSize[1] > 0;
  if (useUserSize) {
    int const twidth = s->NoiseTextureObject->GetWidth();
    int const theight = s->NoiseTextureObject->GetHeight();
    updateSize = s->NoiseTextureSize[0] != twidth ||
        s->NoiseTextureSize[1] != theight;
  }
  if (!s->NoiseTextureObject->GetHandle() || updateSize ||
      s->NoiseTextureObject->GetMTime() < s->NoiseGenerator->GetMTime()) {
    int* winSize = s->GetRenderWindow()->GetSize();
    int sizeX = useUserSize ? s->NoiseTextureSize[0] : winSize[0];
    int sizeY = useUserSize ? s->NoiseTextureSize[1] : winSize[1];
    int const maxSize = GetMaximumTextureSize(glWindow);
    if (sizeX > maxSize || sizeY > maxSize) {
      sizeX = vtkMath::Max(sizeX, maxSize);
      sizeY = vtkMath::Max(sizeY, maxSize);
    }
    // Allocate buffer. After controlling for the maximum supported size sizeX/Y
    // might have changed, so an additional check is needed.
    int const twidth = s->NoiseTextureObject->GetWidth();
    int const theight = s->NoiseTextureObject->GetHeight();
    bool sizeChanged = sizeX != twidth || sizeY != theight;
    if (sizeChanged || !s->NoiseTextureData) {
      delete[] s->NoiseTextureData;
      s->NoiseTextureData = NULL;
      s->NoiseTextureData = new float[sizeX * sizeY];
    }
    // Generate jitter noise
    if (!s->NoiseGenerator) {
      // Use default settings
      vtkPerlinNoise* perlinNoise = vtkPerlinNoise::New();
      perlinNoise->SetPhase(0.0, 0.0, 0.0);
      perlinNoise->SetFrequency(sizeX, sizeY, 1.0);
      perlinNoise->SetAmplitude(0.5); /* [-n, n] */
      s->NoiseGenerator = perlinNoise;
    }
    int const bufferSize = sizeX * sizeY;
    for (int i = 0; i < bufferSize; i++) {
      int const x = i % sizeX;
      int const y = i / sizeY;
      s->NoiseTextureData[i] = static_cast<float>(
          s->NoiseGenerator->EvaluateFunction(x, y, 0.0) + 0.1);
    }
    // Prepare texture
    s->NoiseTextureObject->Create2DFromRaw(sizeX, sizeY, 1, VTK_FLOAT,
        s->NoiseTextureData);
    s->NoiseTextureObject->SetWrapS(Repeat);
    s->NoiseTextureObject->SetWrapT(Repeat);
    s->NoiseTextureObject->SetMagnificationFilter(Nearest);
    s->NoiseTextureObject->SetMinificationFilter(Nearest);
    s->NoiseTextureObject->SetBorderColor(0.0f, 0.0f, 0.0f, 0.0f);
    s->NoiseTextureObject->Modified();
  }
#endif
}
//----------------------------------------------------------------------------
void CaptureDepthTexture(OpenGLGPUVolumeRayCast* s)
{
#if 0
  // Make sure our render window is the current OpenGL context
  s->GetRenderWindow()->MakeCurrent();
  // Load required extensions for grabbing depth sampler buffer
  if (!s->LoadDepthTextureExtensionsSucceeded) {
    s->LoadRequireDepthTextureExtensions(s->GetRenderWindow());
  }
  // If we can't load the necessary extensions, provide
  // feedback on why it failed.
  if (!s->LoadDepthTextureExtensionsSucceeded) {
    std::cerr << s->ExtensionsStringStream.str() << std::endl;
    return;
  }
  if (!s->DepthTextureObject) {
    s->DepthTextureObject = New();
  }
  s->DepthTextureObject->SetContext(vtkOpenGLRenderWindow::SafeDownCast(
      s->GetRenderWindow()));
  if (!s->DepthTextureObject->GetHandle()) {
    // First set the parameters
    s->DepthTextureObject->SetWrapS(ClampToEdge);
    s->DepthTextureObject->SetWrapT(ClampToEdge);
    s->DepthTextureObject->SetMagnificationFilter(Linear);
    s->DepthTextureObject->SetMinificationFilter(Linear);
    s->DepthTextureObject->AllocateDepth(s->WindowSize[0], s->WindowSize[1], 4);
  }
#if GL_ES_VERSION_2_0 != 1
  // currently broken on ES
  s->DepthTextureObject->CopyFromFrameBuffer(s->WindowLowerLeft[0],
      s->WindowLowerLeft[1],
      0, 0,
      s->WindowSize[0],
      s->WindowSize[1]);
#endif
#endif
}
//----------------------------------------------------------------------------
void SetLightingParameters(OpenGLGPUVolumeRayCast* s, Light* light, int nLight, Camera* cam)
{
  GLuint prog = s->ShaderProgram;
  SetUniformi(prog, "in_twoSidedLighting", s->TwoSidedLighting);
  // for lightkit case there are some parameters to set
  float44 viewTF;
  Identity44(viewTF);
  //vtkTransform* viewTF = cam->GetModelViewTransformObject();
  // Bind some light settings
  int numberOfLights = 0;
  float lightAmbientColor[6][3];
  float lightDiffuseColor[6][3];
  float lightSpecularColor[6][3];
  float lightDirection[6][3];
  for (; numberOfLights < nLight;) {
    float status = light->Switch;
    if (status > 0.0) {
      float* aColor = light->AmbientColor;
      float* dColor = light->DiffuseColor;
      float* sColor = light->DiffuseColor;
      double intensity = light->Intensity;
      lightAmbientColor[numberOfLights][0] = aColor[0] * intensity;
      lightAmbientColor[numberOfLights][1] = aColor[1] * intensity;
      lightAmbientColor[numberOfLights][2] = aColor[2] * intensity;
      lightDiffuseColor[numberOfLights][0] = dColor[0] * intensity;
      lightDiffuseColor[numberOfLights][1] = dColor[1] * intensity;
      lightDiffuseColor[numberOfLights][2] = dColor[2] * intensity;
      lightSpecularColor[numberOfLights][0] = sColor[0] * intensity;
      lightSpecularColor[numberOfLights][1] = sColor[1] * intensity;
      lightSpecularColor[numberOfLights][2] = sColor[2] * intensity;
      // Get required info from light
      float* lfp = light->TransformedFocalPoint;
      float* lp = light->TransformedPosition;
      float3 lightDir;
      VEC3_SUB(lfp, lp, lightDir);
      VEC3_NORM(lightDir);
      float* tDir = lightDirection[numberOfLights];
      TransformNormal(viewTF, lightDir, tDir);
      numberOfLights++;
    }
  }
  SetUniform3fv(prog, "in_lightAmbientColor", numberOfLights, lightAmbientColor);
  SetUniform3fv(prog, "in_lightDiffuseColor", numberOfLights, lightDiffuseColor);
  SetUniform3fv(prog, "in_lightSpecularColor", numberOfLights, lightSpecularColor);
  SetUniform3fv(prog, "in_lightDirection", numberOfLights, lightDirection);
  SetUniformi(prog, "in_numberOfLights", numberOfLights);
  // we are done unless we have positional lights
  if (s->LightComplexity < 3) {
    return;
  }
  // if positional lights pass down more parameters
  float lightAttenuation[6][3];
  float lightPosition[6][3];
  float lightConeAngle[6];
  float lightExponent[6];
  int lightPositional[6];
  numberOfLights = 0;
  for (; numberOfLights < nLight;) {
    float status = light->Switch;
    if (status > 0.0) {
      float* attn = light->AttenuationValues;
      lightAttenuation[numberOfLights][0] = attn[0];
      lightAttenuation[numberOfLights][1] = attn[1];
      lightAttenuation[numberOfLights][2] = attn[2];
      lightExponent[numberOfLights] = light->Exponent;
      lightConeAngle[numberOfLights] = light->ConeAngle;
      float* lp = light->TransformedPosition;
      float3 tlp;
      TransformVector(viewTF, lp, tlp);
      lightPosition[numberOfLights][0] = tlp[0];
      lightPosition[numberOfLights][1] = tlp[1];
      lightPosition[numberOfLights][2] = tlp[2];
      lightPositional[numberOfLights] = light->Positional;
      numberOfLights++;
    }
  }
  SetUniform3fv(prog, "in_lightAttenuation", numberOfLights, lightAttenuation);
  SetUniform1iv(prog, "in_lightPositional", numberOfLights, lightPositional);
  SetUniform3fv(prog, "in_lightPosition", numberOfLights, lightPosition);
  SetUniform1fv(prog, "in_lightExponent", numberOfLights, lightExponent);
  SetUniform1fv(prog, "in_lightConeAngle", numberOfLights, lightConeAngle);
}
//-----------------------------------------------------------------------------
void ComputeCellToPointMatrix(OpenGLGPUVolumeRayCast* s)
{
  Identity44(s->CellToPointMatrix);
  s->AdjustedTexMin[0] = s->AdjustedTexMin[1] = s->AdjustedTexMin[2] = 0.0f;
  s->AdjustedTexMin[3] = 1.0f;
  s->AdjustedTexMax[0] = s->AdjustedTexMax[1] = s->AdjustedTexMax[2] = 1.0f;
  s->AdjustedTexMax[3] = 1.0f;
  if (!s->CellFlag) { // point data
    float delta[3];
    delta[0] = s->Extents[1] - s->Extents[0];
    delta[1] = s->Extents[3] - s->Extents[2];
    delta[2] = s->Extents[5] - s->Extents[4];
    float min[3];
    min[0] = 0.5f / delta[0];
    min[1] = 0.5f / delta[1];
    min[2] = 0.5f / delta[2];
    float range[3]; // max - min
    range[0] = (delta[0] - 0.5f) / delta[0] - min[0];
    range[1] = (delta[1] - 0.5f) / delta[1] - min[1];
    range[2] = (delta[2] - 0.5f) / delta[2] - min[2];
    s->CellToPointMatrix[0][0] = range[0]; // Scale diag
    s->CellToPointMatrix[1][1] = range[1];
    s->CellToPointMatrix[2][2] = range[2];
    s->CellToPointMatrix[0][3] = min[0]; // t vector
    s->CellToPointMatrix[1][3] = min[1];
    s->CellToPointMatrix[2][3] = min[2];
    // Adjust limit coordinates for texture access.
    float const zeros[4] = {0.0f, 0.0f, 0.0f, 1.0f}; // GL tex min
    float const ones[4] = {1.0f, 1.0f, 1.0f, 1.0f}; // GL tex max
    MultiplyPoint(s->CellToPointMatrix, zeros, s->AdjustedTexMin);
    MultiplyPoint(s->CellToPointMatrix, ones, s->AdjustedTexMax);
  }
}
//----------------------------------------------------------------------------
// Test if camera is inside the volume geometry
bool IsCameraInside(OpenGLGPUVolumeRayCast* s, const Camera* cam, const float44 volMatrix)
{
  float44 TempMatrix1;
  Invert(TempMatrix1, volMatrix);
  float camWorldRange[2];
  float camWorldPos[4];
  float camFocalWorldPoint[4];
  float camWorldDirection[4];
  float camPos[4];
  float camPlaneNormal[4];
  VEC3CPY(camWorldPos, cam->Position);
  camWorldPos[3] = 1.0f;
  MultiplyPoint(TempMatrix1, camWorldPos, camPos);
  VEC3CPY(camFocalWorldPoint, cam->FocalPoint);
  camFocalWorldPoint[3] = 1.0;
  // The range (near/far) must also be transformed
  // into the local coordinate system.
  camWorldDirection[0] = camFocalWorldPoint[0] - camWorldPos[0];
  camWorldDirection[1] = camFocalWorldPoint[1] - camWorldPos[1];
  camWorldDirection[2] = camFocalWorldPoint[2] - camWorldPos[2];
  camWorldDirection[3] = 0.0;
  // Compute the normalized near plane normal
  MultiplyPoint(TempMatrix1, camWorldDirection, camPlaneNormal);
  Normalize3(camWorldDirection);
  Normalize3(camPlaneNormal);
  float camNearWorldPoint[4];
  float camNearPoint[4];
  camWorldRange[0] = cam->ClippingRange[0];
  camWorldRange[1] = cam->ClippingRange[1];
  camNearWorldPoint[0] = camWorldPos[0] + camWorldRange[0] * camWorldDirection[0];
  camNearWorldPoint[1] = camWorldPos[1] + camWorldRange[0] * camWorldDirection[1];
  camNearWorldPoint[2] = camWorldPos[2] + camWorldRange[0] * camWorldDirection[2];
  camNearWorldPoint[3] = 1.;
  MultiplyPoint(TempMatrix1, camNearWorldPoint, camNearPoint);
  float tolerance[3] = { 1e-12, 1e-12, 1e-12 };
  if (PointIsWithinBounds(camNearPoint, s->LoadedBounds, tolerance)) {
    return true;
  }
  return false;
}
//----------------------------------------------------------------------------
void RenderVolumeGeometry(OpenGLGPUVolumeRayCast* s, Camera* cam, const float44 volMatrix, ImageData* input)
{
#if 0
  if (IsCameraInside(s, cam, volMatrix) || s->CameraWasInsideInLastUpdate) {
    vtkNew<vtkTessellatedBoxSource> boxSource;
    boxSource->SetBounds(s->LoadedBounds);
    boxSource->QuadsOn();
    boxSource->SetLevel(0);
    vtkNew<vtkDensifyPolyData> densityPolyData;
    if (input->GetMTime() <= s->InputUpdateTime.GetMTime() &&
        s->IsCameraInside(s, vol)) {
      // Normals should be transformed using the transpose of inverse
      // InverseVolumeMat
      s->TempMatrix1->DeepCopy(vol->GetMatrix());
      s->TempMatrix1->Invert();
      Camera* cam = s->GetActiveCamera();
      double camWorldRange[2];
      double camWorldPos[4];
      double camFocalWorldPoint[4];
      double camWorldDirection[4];
      double camPos[4];
      double camPlaneNormal[4];
      cam->GetPosition(camWorldPos);
      camWorldPos[3] = 1.0;
      s->TempMatrix1->MultiplyPoint(camWorldPos, camPos);
      cam->GetFocalPoint(camFocalWorldPoint);
      camFocalWorldPoint[3] = 1.0;
      // The range (near/far) must also be transformed
      // into the local coordinate system.
      camWorldDirection[0] = camFocalWorldPoint[0] - camWorldPos[0];
      camWorldDirection[1] = camFocalWorldPoint[1] - camWorldPos[1];
      camWorldDirection[2] = camFocalWorldPoint[2] - camWorldPos[2];
      camWorldDirection[3] = 0.0;
      // Compute the normalized near plane normal
      s->TempMatrix1->MultiplyPoint(camWorldDirection, camPlaneNormal);
      vtkMath::Normalize(camWorldDirection);
      vtkMath::Normalize(camPlaneNormal);
      double camNearWorldPoint[4];
      double camFarWorldPoint[4];
      double camNearPoint[4];
      double camFarPoint[4];
      cam->GetClippingRange(camWorldRange);
      camNearWorldPoint[0] = camWorldPos[0] + camWorldRange[0] * camWorldDirection[0];
      camNearWorldPoint[1] = camWorldPos[1] + camWorldRange[0] * camWorldDirection[1];
      camNearWorldPoint[2] = camWorldPos[2] + camWorldRange[0] * camWorldDirection[2];
      camNearWorldPoint[3] = 1.;
      camFarWorldPoint[0] = camWorldPos[0] + camWorldRange[1] * camWorldDirection[0];
      camFarWorldPoint[1] = camWorldPos[1] + camWorldRange[1] * camWorldDirection[1];
      camFarWorldPoint[2] = camWorldPos[2] + camWorldRange[1] * camWorldDirection[2];
      camFarWorldPoint[3] = 1.;
      s->TempMatrix1->MultiplyPoint(camNearWorldPoint, camNearPoint);
      s->TempMatrix1->MultiplyPoint(camFarWorldPoint, camFarPoint);
      vtkNew<vtkPlane> nearPlane;
      // We add an offset to the near plane to avoid hardware clipping of the
      // near plane due to floating-point precision.
      // camPlaneNormal is a unit vector, if the offset is larger than the
      // distance between near and far point, it will not work. Hence, we choose
      // a fraction of the near-far distance. However, care should be taken
      // to avoid hardware clipping in volumes with very small spacing where the
      // distance between near and far plane is also very small. In that case,
      // a minimum offset is chosen. This is chosen based on the typical
      // epsilon values on x86 systems.
      double offset = sqrt(vtkMath::Distance2BetweenPoints(
          camNearPoint, camFarPoint)) / 1000.0;
      // Minimum offset to avoid floating point precision issues for volumes
      // with very small spacing
      double minOffset = (double)(
          std::numeric_limits<float>::epsilon()) * 1000.0;
      offset = offset < minOffset ? minOffset : offset;
      camNearPoint[0] += camPlaneNormal[0] * offset;
      camNearPoint[1] += camPlaneNormal[1] * offset;
      camNearPoint[2] += camPlaneNormal[2] * offset;
      nearPlane->SetOrigin(camNearPoint);
      nearPlane->SetNormal(camPlaneNormal);
      vtkNew<vtkPlaneCollection> planes;
      planes->RemoveAllItems();
      planes->AddItem(nearPlane);
      vtkNew<vtkClipConvexPolyData> clip;
      clip->SetInputConnection(boxSource->GetOutputPort());
      clip->SetPlanes(planes);
      densityPolyData->SetInputConnection(clip->GetOutputPort());
      s->CameraWasInsideInLastUpdate = true;
    }
    else {
      densityPolyData->SetInputConnection(boxSource->GetOutputPort());
      s->CameraWasInsideInLastUpdate = false;
    }
    densityPolyData->SetNumberOfSubdivisions(2);
    densityPolyData->Update();
    s->BBoxPolyData = vtkSmartPointer<vtkPolyData>::New();
    s->BBoxPolyData->ShallowCopy(densityPolyData->GetOutput());
    vtkPoints* points = s->BBoxPolyData->GetPoints();
    vtkCellArray* cells = s->BBoxPolyData->GetPolys();
    vtkNew<vtkUnsignedIntArray> polys;
    polys->SetNumberOfComponents(3);
    vtkIdType npts;
    vtkIdType* pts;
    // See if the volume transform is orientation-preserving
    // and orient polygons accordingly
    Matrix4x4* volMat = vol->GetMatrix();
    double det = vtkMath::Determinant3x3(
        volMat->GetElement(0, 0), volMat->GetElement(0, 1), volMat->GetElement(0, 2),
        volMat->GetElement(1, 0), volMat->GetElement(1, 1), volMat->GetElement(1, 2),
        volMat->GetElement(2, 0), volMat->GetElement(2, 1), volMat->GetElement(2, 2));
    bool preservesOrientation = det > 0.0;
    const vtkIdType indexMap[3] = {
      preservesOrientation ? 0 : 2,
      1,
      preservesOrientation ? 2 : 0
    };
    while (cells->GetNextCell(npts, pts)) {
      polys->InsertNextTuple3(pts[indexMap[0]], pts[indexMap[1]], pts[indexMap[2]]);
    }
    // Dispose any previously created buffers
    s->DeleteBufferObjects();
    // Now create new ones
    s->CreateBufferObjects();
    // TODO: should realy use the built in VAO class
    // which handles these apple issues internally
#ifdef __APPLE__
    if (vtkOpenGLRenderWindow::GetContextSupportsOpenGL32())
#endif
    {
      glBindVertexArray(s->CubeVAOId);
    }
    // Pass cube vertices to buffer object memory
    glBindBuffer(GL_ARRAY_BUFFER, s->CubeVBOId);
    glBufferData(GL_ARRAY_BUFFER, points->GetData()->GetDataSize() *
        points->GetData()->GetDataTypeSize(),
        points->GetData()->GetVoidPointer(0), GL_STATIC_DRAW);
    prog->EnableAttributeArray("in_vertexPos");
    prog->UseAttributeArray("in_vertexPos", 0, 0, VTK_FLOAT,
        3, vtkShaderProgram::NoNormalize);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s->CubeIndicesId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, polys->GetDataSize() *
        polys->GetDataTypeSize(), polys->GetVoidPointer(0),
        GL_STATIC_DRAW);
  }
  else {
#ifdef __APPLE__
    if (!vtkOpenGLRenderWindow::GetContextSupportsOpenGL32()) {
      glBindBuffer(GL_ARRAY_BUFFER, s->CubeVBOId);
      prog->EnableAttributeArray("in_vertexPos");
      prog->UseAttributeArray("in_vertexPos", 0, 0, VTK_FLOAT,
          3, vtkShaderProgram::NoNormalize);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s->CubeIndicesId);
    }
    else
#endif
    {
      glBindVertexArray(s->CubeVAOId);
    }
  }
  glDrawElements(GL_TRIANGLES,
      s->BBoxPolyData->GetNumberOfCells() * 3,
      GL_UNSIGNED_INT, 0);
#ifdef __APPLE__
  if (!vtkOpenGLRenderWindow::GetContextSupportsOpenGL32()) {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
  else
#endif
  {
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
#endif
}
//----------------------------------------------------------------------------
void SetCroppingRegions(OpenGLGPUVolumeRayCast* s, GLuint prog)
{
  if (s->Cropping) {
    int cropFlags = s->CroppingRegionFlags;
    double croppingRegionPlanes[6];
    COPY(6, s->CroppingRegionPlanes, croppingRegionPlanes);
    // Clamp it
    croppingRegionPlanes[0] = croppingRegionPlanes[0] < s->LoadedBounds[0] ? s->LoadedBounds[0] : croppingRegionPlanes[0];
    croppingRegionPlanes[0] = croppingRegionPlanes[0] > s->LoadedBounds[1] ? s->LoadedBounds[1] : croppingRegionPlanes[0];
    croppingRegionPlanes[1] = croppingRegionPlanes[1] < s->LoadedBounds[0] ? s->LoadedBounds[0] : croppingRegionPlanes[1];
    croppingRegionPlanes[1] = croppingRegionPlanes[1] > s->LoadedBounds[1] ? s->LoadedBounds[1] : croppingRegionPlanes[1];
    croppingRegionPlanes[2] = croppingRegionPlanes[2] < s->LoadedBounds[2] ? s->LoadedBounds[2] : croppingRegionPlanes[2];
    croppingRegionPlanes[2] = croppingRegionPlanes[2] > s->LoadedBounds[3] ? s->LoadedBounds[3] : croppingRegionPlanes[2];
    croppingRegionPlanes[3] = croppingRegionPlanes[3] < s->LoadedBounds[2] ? s->LoadedBounds[2] : croppingRegionPlanes[3];
    croppingRegionPlanes[3] = croppingRegionPlanes[3] > s->LoadedBounds[3] ? s->LoadedBounds[3] : croppingRegionPlanes[3];
    croppingRegionPlanes[4] = croppingRegionPlanes[4] < s->LoadedBounds[4] ? s->LoadedBounds[4] : croppingRegionPlanes[4];
    croppingRegionPlanes[4] = croppingRegionPlanes[4] > s->LoadedBounds[5] ? s->LoadedBounds[5] : croppingRegionPlanes[4];
    croppingRegionPlanes[5] = croppingRegionPlanes[5] < s->LoadedBounds[4] ? s->LoadedBounds[4] : croppingRegionPlanes[5];
    croppingRegionPlanes[5] = croppingRegionPlanes[5] > s->LoadedBounds[5] ? s->LoadedBounds[5] : croppingRegionPlanes[5];
    float cropPlanes[6] = { static_cast<float>(croppingRegionPlanes[0]),
        static_cast<float>(croppingRegionPlanes[1]),
        static_cast<float>(croppingRegionPlanes[2]),
        static_cast<float>(croppingRegionPlanes[3]),
        static_cast<float>(croppingRegionPlanes[4]),
        static_cast<float>(croppingRegionPlanes[5])
                          };
    SetUniform1fv(prog, "in_croppingPlanes", 6, cropPlanes);
    const int numberOfRegions = 32;
    int cropFlagsArray[numberOfRegions];
    cropFlagsArray[0] = 0;
    int i = 1;
    while (cropFlags && i < 32) {
      cropFlagsArray[i] = cropFlags & 1;
      cropFlags = cropFlags >> 1;
      ++i;
    }
    for (; i < 32; ++i) {
      cropFlagsArray[i] = 0;
    }
    SetUniform1iv(prog, "in_croppingFlags", numberOfRegions, cropFlagsArray);
  }
}
//----------------------------------------------------------------------------
void SetClippingPlanes(OpenGLGPUVolumeRayCast* s, GLuint prog)
{
#if 0
  if (s->ClippingPlanes) {
    std::vector<float> clippingPlanes;
    // Currently we don't have any clipping plane
    clippingPlanes.push_back(0);
    s->ClippingPlanes->InitTraversal();
    vtkPlane* plane;
    while ((plane = s->ClippingPlanes->GetNextItem())) {
      // Planes are in world coordinates
      double planeOrigin[3], planeNormal[3];
      plane->GetOrigin(planeOrigin);
      plane->GetNormal(planeNormal);
      clippingPlanes.push_back(planeOrigin[0]);
      clippingPlanes.push_back(planeOrigin[1]);
      clippingPlanes.push_back(planeOrigin[2]);
      clippingPlanes.push_back(planeNormal[0]);
      clippingPlanes.push_back(planeNormal[1]);
      clippingPlanes.push_back(planeNormal[2]);
    }
    clippingPlanes[0] = clippingPlanes.size() > 1 ?
        static_cast<int>(clippingPlanes.size() - 1) : 0;
    prog->SetUniform1fv("in_clippingPlanes", (int)(clippingPlanes.size()), &clippingPlanes[0]);
  }
#endif
}
// -----------------------------------------------------------------------------
void CheckPickingState(OpenGLGPUVolumeRayCast* s)
{
#if 0
  vtkHardwareSelector* selector = s->GetSelector();
  bool selectorPicking = selector != NULL;
  if (selector) {
    // s mapper currently only supports cell picking
    selectorPicking &= selector->GetFieldAssociation() == vtkDataObject::FIELD_ASSOCIATION_CELLS;
  }
  s->IsPicking = selectorPicking || s->GetRenderWindow()->GetIsPicking();
  if (s->IsPicking) {
    // rebuild the shader on every pass
    s->SelectionStateTime.Modified();
    s->CurrentSelectionPass = selector ? selector->GetCurrentPass() : ACTOR_PASS;
  }
  else if (s->CurrentSelectionPass != MIN_KNOWN_PASS - 1) {
    // return to the regular rendering state
    s->SelectionStateTime.Modified();
    s->CurrentSelectionPass = MIN_KNOWN_PASS - 1;
  }
#endif
}
// -----------------------------------------------------------------------------
void BeginPicking(OpenGLGPUVolumeRayCast* s)
{
#if 0
  vtkHardwareSelector* selector = s->GetSelector();
  if (selector && s->IsPicking) {
    selector->BeginRenderProp();
    if (s->CurrentSelectionPass >= ID_LOW24) {
      selector->RenderAttributeId(0);
    }
  }
#endif
}
//------------------------------------------------------------------------------
void SetPickingId(OpenGLGPUVolumeRayCast* s)
{
#if 0
  float propIdColor[3] = {0.0, 0.0, 0.0};
  vtkHardwareSelector* selector = s->GetSelector();
  if (selector && s->IsPicking) {
    // query the selector for the appropriate id
    selector->GetPropColorValue(propIdColor);
  }
  else { // RenderWindow is picking
    unsigned int const idx = s->GetCurrentPickId();
    Convert(idx, propIdColor);
  }
  s->ShaderProgram->SetUniform3f("in_propId", propIdColor);
#endif
}
// ---------------------------------------------------------------------------
void EndPicking(OpenGLGPUVolumeRayCast* s)
{
#if 0
  vtkHardwareSelector* selector = s->GetSelector();
  if (selector && s->IsPicking) {
    if (s->CurrentSelectionPass >= ID_LOW24) {
      // tell the selector the maximum number of cells that the mapper could render
      unsigned int const numVoxels = (s->Extents[1] - s->Extents[0]) *
          (s->Extents[3] - s->Extents[2]) * (s->Extents[5] - s->Extents[4]);
      selector->RenderAttributeId(numVoxels);
    }
    selector->EndRenderProp();
  }
#endif
}
//----------------------------------------------------------------------------
double SpacingAdjustedSampleDistance(const float inputSpacing[3], const int inputExtent[6])
{
  // compute 1/2 the average spacing
  double dist = (inputSpacing[0] + inputSpacing[1] + inputSpacing[2]) / 6.0;
  double avgNumVoxels = pow(static_cast<double>((inputExtent[1] - inputExtent[0]) *
      (inputExtent[3] - inputExtent[2]) * (inputExtent[5] - inputExtent[4])), 0.333);
  if (avgNumVoxels < 100) {
    dist *= 0.01 + (1 - 0.01) * avgNumVoxels / 100;
  }
  return dist;
}
//----------------------------------------------------------------------------
void UpdateSamplingDistance(OpenGLGPUVolumeRayCast* s, ImageData* input, const float44 volMatrix)
{
  if (!s->AutoAdjustSampleDistances) {
    if (s->LockSampleDistanceToInputSpacing) {
      float const d = (float)(SpacingAdjustedSampleDistance(s->CellSpacing, s->Extents));
      float const sample = s->SampleDistance;
      // ActualSampleDistance will grow proportionally to numVoxels^(1/3) (see
      // vtkVolumeMapper.cxx). Until it reaches 1/2 average voxel size when number of
      // voxels is 1E6.
      s->ActualSampleDistance = (sample / d < 0.999f || sample / d > 1.001f) ?
          d : s->SampleDistance;
      return;
    }
    s->ActualSampleDistance = s->SampleDistance;
  }
  else {
    VEC3CPY(s->CellSpacing, input->spacing);
    const float4* worldToDataset = volMatrix;
    double minWorldSpacing = DOUBLE_MAX;
    int i = 0;
    while (i < 3) {
      double tmp = worldToDataset[0][i];
      double tmp2 = tmp * tmp;
      tmp = worldToDataset[1][i];
      tmp2 += tmp * tmp;
      tmp = worldToDataset[2][i];
      tmp2 += tmp * tmp;
      // We use fabs() in case the spacing is negative.
      double worldSpacing = fabs(s->CellSpacing[i] * sqrt(tmp2));
      if (worldSpacing < minWorldSpacing) {
        minWorldSpacing = worldSpacing;
      }
      ++i;
    }
    // minWorldSpacing is the optimal sample distance in world space.
    // To go faster (reduceFactor<1.0), we multiply s distance
    // by 1/reduceFactor.
    s->ActualSampleDistance = static_cast<float>(minWorldSpacing);
    if (s->ReductionFactor < 1.0 &&
        s->ReductionFactor != 0.0) {
      s->ActualSampleDistance /=
          static_cast<GLfloat>(s->ReductionFactor);
    }
  }
}
//----------------------------------------------------------------------------
void LoadRequireDepthTextureExtensions(OpenGLGPUVolumeRayCast* s)
{
  // Reset the message stream for extensions
  if (GetContextSupportsOpenGL32()) {
    s->LoadDepthTextureExtensionsSucceeded = true;
    return;
  }
  s->ExtensionsStringStream.str("");
  s->ExtensionsStringStream.clear();
#if GL_ES_VERSION_2_0 != 1
  // Check for float texture support. This extension became core
  // in 3.0
  if (!glewIsSupported("GL_ARB_texture_float")) {
    s->ExtensionsStringStream << "Required extension "
        << " GL_ARB_texture_float is not supported";
    return;
  }
#else
#if GL_ES_VERSION_3_0 != 1
  s->ExtensionsStringStream << "Requires ES version 3.0 or later";
  return;
#endif
#endif
  // NOTE: Support for depth sampler texture made into the core since version
  // 1.4 and therefore we are no longer checking for it.
  s->LoadDepthTextureExtensionsSucceeded = true;
}
//----------------------------------------------------------------------------
void CreateBufferObjects(OpenGLGPUVolumeRayCast* s)
{
#ifdef __APPLE__
  if (GetContextSupportsOpenGL32())
#endif
  {
    glGenVertexArrays(1, &s->CubeVAOId);
  }
  glGenBuffers(1, &s->CubeVBOId);
  glGenBuffers(1, &s->CubeIndicesId);
}
//----------------------------------------------------------------------------
void DeleteBufferObjects(OpenGLGPUVolumeRayCast* s)
{
  if (s->CubeVBOId) {
    glBindBuffer(GL_ARRAY_BUFFER, s->CubeVBOId);
    glDeleteBuffers(1, &s->CubeVBOId);
    s->CubeVBOId = 0;
  }
  if (s->CubeIndicesId) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s->CubeIndicesId);
    glDeleteBuffers(1, &s->CubeIndicesId);
    s->CubeIndicesId = 0;
  }
  if (s->CubeVAOId) {
#ifdef __APPLE__
    if (vtkOpenGLRenderWindow::GetContextSupportsOpenGL32())
#endif
    {
      glDeleteVertexArrays(1, &s->CubeVAOId);
    }
    s->CubeVAOId = 0;
  }
}
//----------------------------------------------------------------------------
void ConvertTextureToImageData(OpenGLGPUVolumeRayCast* s, GLuint texture, ImageData* output)
{
#if 0
  if (!texture) {
    return;
  }
  unsigned int tw = texture->GetWidth();
  unsigned int th = texture->GetHeight();
  unsigned int tnc = texture->GetComponents();
  int tt = texture->GetVTKDataType();
  vtkPixelExtent texExt(0U, tw - 1U, 0U, th - 1U);
  int dataExt[6] = {0, 0, 0, 0, 0, 0};
  texExt.GetData(dataExt);
  double dataOrigin[6] = {0, 0, 0, 0, 0, 0};
  ImageData* id = ImageData::New();
  id->SetOrigin(dataOrigin);
  id->SetDimensions(tw, th, 1);
  id->SetExtent(dataExt);
  id->AllocateScalars(tt, tnc);
  vtkPixelBufferObject* pbo = texture->Download();
  vtkPixelTransfer::Blit(texExt, texExt, texExt, texExt, tnc, tt, pbo->MapPackedBuffer(), tnc, tt, id->GetScalarPointer(0, 0, 0));
  pbo->UnmapPackedBuffer();
  pbo->Delete();
  if (!output) {
    output = ImageData::New();
  }
  output->DeepCopy(id);
  id->Delete();
#endif
}
//----------------------------------------------------------------------------
void SetupRenderToTexture(OpenGLGPUVolumeRayCast* s)
{
#if 0
  if (s->RenderToImage && s->CurrentPass == RenderPass) {
    if ((s->LastRenderToImageWindowSize[0] != s->WindowSize[0]) ||
        (s->LastRenderToImageWindowSize[1] != s->WindowSize[1])) {
      s->LastRenderToImageWindowSize[0] = s->WindowSize[0];
      s->LastRenderToImageWindowSize[1] = s->WindowSize[1];
      s->ReleaseRenderToTextureGraphicsResources(s->GetRenderWindow());
    }
    if (!s->FBO) {
      s->FBO = vtkFrameBufferObject2::New();
    }
    s->FBO->SetContext(vtkOpenGLRenderWindow::SafeDownCast(
        s->GetRenderWindow()));
    s->FBO->Bind(GL_FRAMEBUFFER);
    s->FBO->InitializeViewport(s->WindowSize[0], s->WindowSize[1]);
    int depthImageScalarType = s->GetDepthImageScalarType();
    bool initDepthTexture = true;
    // Re-instantiate the depth texture object if the scalar type requested has
    // changed from the last frame
    if (s->RTTDepthTextureObject &&
        s->RTTDepthTextureType == depthImageScalarType) {
      initDepthTexture = false;
    }
    if (initDepthTexture) {
      if (s->RTTDepthTextureObject) {
        s->RTTDepthTextureObject->Delete();
        s->RTTDepthTextureObject = 0;
      }
      s->RTTDepthTextureObject = New();
      s->RTTDepthTextureObject->SetContext(
          vtkOpenGLRenderWindow::SafeDownCast(
              s->GetRenderWindow()));
      s->RTTDepthTextureObject->Create2D(s->WindowSize[0],
          s->WindowSize[1], 1,
          depthImageScalarType, false);
      s->RTTDepthTextureObject->Activate();
      s->RTTDepthTextureObject->SetMinificationFilter(
          Nearest);
      s->RTTDepthTextureObject->SetMagnificationFilter(
          Nearest);
      s->RTTDepthTextureObject->SetAutoParameters(0);
      // Cache the value of the scalar type
      s->RTTDepthTextureType = depthImageScalarType;
    }
    if (!s->RTTColorTextureObject) {
      s->RTTColorTextureObject = New();
      s->RTTColorTextureObject->SetContext(
          vtkOpenGLRenderWindow::SafeDownCast(
              s->GetRenderWindow()));
      s->RTTColorTextureObject->Create2D(s->WindowSize[0],
          s->WindowSize[1], 4,
          VTK_UCHAR, false);
      s->RTTColorTextureObject->Activate();
      s->RTTColorTextureObject->SetMinificationFilter(
          Nearest);
      s->RTTColorTextureObject->SetMagnificationFilter(
          Nearest);
      s->RTTColorTextureObject->SetAutoParameters(0);
    }
    if (!s->RTTDepthBufferTextureObject) {
      s->RTTDepthBufferTextureObject = New();
      s->RTTDepthBufferTextureObject->SetContext(
          vtkOpenGLRenderWindow::SafeDownCast(s->GetRenderWindow()));
      s->RTTDepthBufferTextureObject->AllocateDepth(
          s->WindowSize[0], s->WindowSize[1], Float32);
      s->RTTDepthBufferTextureObject->Activate();
      s->RTTDepthBufferTextureObject->SetMinificationFilter(
          Nearest);
      s->RTTDepthBufferTextureObject->SetMagnificationFilter(
          Nearest);
      s->RTTDepthBufferTextureObject->SetAutoParameters(0);
    }
    s->FBO->Bind(GL_FRAMEBUFFER);
    s->FBO->AddTexDepthAttachment(
        GL_DRAW_FRAMEBUFFER,
        s->RTTDepthBufferTextureObject->GetHandle());
    s->FBO->AddTexColorAttachment(
        GL_DRAW_FRAMEBUFFER, 0U,
        s->RTTColorTextureObject->GetHandle());
    s->FBO->AddTexColorAttachment(
        GL_DRAW_FRAMEBUFFER, 1U,
        s->RTTDepthTextureObject->GetHandle());
    s->FBO->ActivateDrawBuffers(2);
    s->FBO->CheckFrameBufferStatus(GL_FRAMEBUFFER);
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }
#endif
}
//----------------------------------------------------------------------------
void ExitRenderToTexture(OpenGLGPUVolumeRayCast* s)
{
#if 0
  if (s->RenderToImage && s->CurrentPass == RenderPass) {
    s->FBO->RemoveTexDepthAttachment(GL_DRAW_FRAMEBUFFER);
    s->FBO->RemoveTexColorAttachment(GL_DRAW_FRAMEBUFFER, 0U);
    s->FBO->RemoveTexColorAttachment(GL_DRAW_FRAMEBUFFER, 1U);
    s->FBO->DeactivateDrawBuffers();
    s->FBO->UnBind(GL_FRAMEBUFFER);
    s->RTTDepthBufferTextureObject->Deactivate();
    s->RTTColorTextureObject->Deactivate();
    s->RTTDepthTextureObject->Deactivate();
  }
#endif
}
//----------------------------------------------------------------------------
void SetupDepthPass(OpenGLGPUVolumeRayCast* s)
{
#if 0
  if ((s->LastDepthPassWindowSize[0] != s->WindowSize[0]) ||
      (s->LastDepthPassWindowSize[1] != s->WindowSize[1])) {
    s->LastDepthPassWindowSize[0] = s->WindowSize[0];
    s->LastDepthPassWindowSize[1] = s->WindowSize[1];
    s->ReleaseDepthPassGraphicsResources(s->GetRenderWindow());
  }
  if (!s->DPFBO) {
    s->DPFBO = vtkFrameBufferObject2::New();
  }
  s->DPFBO->SetContext(vtkOpenGLRenderWindow::SafeDownCast(
      s->GetRenderWindow()));
  s->DPFBO->Bind(GL_FRAMEBUFFER);
  s->DPFBO->InitializeViewport(s->WindowSize[0], s->WindowSize[1]);
  if (!s->DPDepthBufferTextureObject ||
      !s->DPColorTextureObject) {
    s->DPDepthBufferTextureObject = New();
    s->DPDepthBufferTextureObject->SetContext(
        vtkOpenGLRenderWindow::SafeDownCast(s->GetRenderWindow()));
    s->DPDepthBufferTextureObject->AllocateDepth(
        s->WindowSize[0], s->WindowSize[1], Native);
    s->DPDepthBufferTextureObject->Activate();
    s->DPDepthBufferTextureObject->SetMinificationFilter(
        Nearest);
    s->DPDepthBufferTextureObject->SetMagnificationFilter(
        Nearest);
    s->DPDepthBufferTextureObject->SetAutoParameters(0);
    s->DPDepthBufferTextureObject->Bind();
    s->DPColorTextureObject = New();
    s->DPColorTextureObject->SetContext(
        vtkOpenGLRenderWindow::SafeDownCast(
            s->GetRenderWindow()));
    s->DPColorTextureObject->Create2D(s->WindowSize[0],
        s->WindowSize[1], 4,
        VTK_UCHAR, false);
    s->DPColorTextureObject->Activate();
    s->DPColorTextureObject->SetMinificationFilter(
        Nearest);
    s->DPColorTextureObject->SetMagnificationFilter(
        Nearest);
    s->DPColorTextureObject->SetAutoParameters(0);
    s->DPFBO->AddTexDepthAttachment(
        GL_DRAW_FRAMEBUFFER,
        s->DPDepthBufferTextureObject->GetHandle());
    s->DPFBO->AddTexColorAttachment(
        GL_DRAW_FRAMEBUFFER, 0U,
        s->DPColorTextureObject->GetHandle());
  }
  s->DPFBO->ActivateDrawBuffers(1);
  s->DPFBO->CheckFrameBufferStatus(GL_FRAMEBUFFER);
  // Setup the contour polydata mapper to render to DPFBO
  s->ContourMapper->SetInputConnection(
      s->ContourFilter->GetOutputPort());
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
#endif
}
//----------------------------------------------------------------------------
void ExitDepthPass(OpenGLGPUVolumeRayCast* s)
{
#if 0
  s->DPFBO->DeactivateDrawBuffers();
  s->DPFBO->UnBind(GL_FRAMEBUFFER);
  s->DPDepthBufferTextureObject->Deactivate();
  s->DPColorTextureObject->Deactivate();
  glDisable(GL_DEPTH_TEST);
#endif
}
//----------------------------------------------------------------------------
void ReleaseRenderToTextureGraphicsResources(OpenGLGPUVolumeRayCast* s)
{
#if 0
  vtkOpenGLRenderWindow* rwin = vtkOpenGLRenderWindow::SafeDownCast(win);
  if (rwin) {
    if (s->FBO) {
      s->FBO->Delete();
      s->FBO = 0;
    }
    if (s->RTTDepthBufferTextureObject) {
      s->RTTDepthBufferTextureObject->ReleaseGraphicsResources(win);
      s->RTTDepthBufferTextureObject->Delete();
      s->RTTDepthBufferTextureObject = 0;
    }
    if (s->RTTDepthTextureObject) {
      s->RTTDepthTextureObject->ReleaseGraphicsResources(win);
      s->RTTDepthTextureObject->Delete();
      s->RTTDepthTextureObject = 0;
    }
    if (s->RTTColorTextureObject) {
      s->RTTColorTextureObject->ReleaseGraphicsResources(win);
      s->RTTColorTextureObject->Delete();
      s->RTTColorTextureObject = 0;
    }
  }
#endif
}
//----------------------------------------------------------------------------
void ReleaseDepthPassGraphicsResources(OpenGLGPUVolumeRayCast* s)
{
#if 0
  vtkOpenGLRenderWindow* rwin = vtkOpenGLRenderWindow::SafeDownCast(win);
  if (rwin) {
    if (s->DPFBO) {
      s->DPFBO->Delete();
      s->DPFBO = 0;
    }
    if (s->DPDepthBufferTextureObject) {
      s->DPDepthBufferTextureObject->ReleaseGraphicsResources(win);
      s->DPDepthBufferTextureObject->Delete();
      s->DPDepthBufferTextureObject = 0;
    }
    if (s->DPColorTextureObject) {
      s->DPColorTextureObject->ReleaseGraphicsResources(win);
      s->DPColorTextureObject->Delete();
      s->DPColorTextureObject = 0;
    }
    s->ContourMapper->ReleaseGraphicsResources(win);
  }
#endif
}
//----------------------------------------------------------------------------
void OpenGLGPUVolumeRayCastMapper_init(OpenGLGPUVolumeRayCast* s)
{
  s->ReductionFactor = 1.0;
  s->CurrentPass = RenderPass;
  s->NoiseTextureSize[0] = s->NoiseTextureSize[1] = -1;
  s->NoiseGenerator = NULL;
  // s->ResourceCallback = new vtkOpenGLResourceFreeCallback<OpenGLGPUVolumeRayCast>(s, &ReleaseGraphicsResources);
}
//----------------------------------------------------------------------------
void OpenGLGPUVolumeRayCastMapper_free(OpenGLGPUVolumeRayCast* s)
{
#if 0
  if (s->ResourceCallback) {
    //s->ResourceCallback->Release(); delete s->ResourceCallback;
    s->ResourceCallback = NULL;
  }
  if (s->NoiseGenerator) {
    Delete(s->NoiseGenerator);
    s->NoiseGenerator = NULL;
  }
#endif
}
//----------------------------------------------------------------------------
void ReleaseGraphicsResources(OpenGLGPUVolumeRayCast* s)
{
#if 0
  s->DeleteBufferObjects();
  if (s->VolumeTextureObject) {
    ReleaseGraphicsResources(s->VolumeTextureObject, window);
    s->VolumeTextureObject->Delete();
    s->VolumeTextureObject = 0;
  }
  if (s->NoiseTextureObject) {
    s->NoiseTextureObject->ReleaseGraphicsResources(window);
    s->NoiseTextureObject->Delete();
    s->NoiseTextureObject = 0;
  }
  if (s->DepthTextureObject) {
    s->DepthTextureObject->ReleaseGraphicsResources(window);
    s->DepthTextureObject->Delete();
    s->DepthTextureObject = 0;
  }
  s->ReleaseRenderToTextureGraphicsResources(window);
  s->ReleaseDepthPassGraphicsResources(window);
  if (s->MaskTextures != 0) {
    if (!s->MaskTextures->Map.empty()) {
      std::map<ImageData*, vtkVolumeMask*>::iterator it =
          s->MaskTextures->Map.begin();
      while (it != s->MaskTextures->Map.end()) {
        vtkVolumeMask* texture = (*it).second;
        texture->ReleaseGraphicsResources(window);
        delete texture;
        ++it;
      }
      s->MaskTextures->Map.clear();
    }
  }
  if (s->RGBTables) {
    s->RGBTables->ReleaseGraphicsResources(window);
    delete s->RGBTables;
    s->RGBTables = 0;
  }
  if (s->Mask1RGBTable) {
    s->Mask1RGBTable->ReleaseGraphicsResources(window);
    s->Mask1RGBTable->Delete();
    s->Mask1RGBTable = NULL;
  }
  if (s->Mask2RGBTable) {
    s->Mask2RGBTable->ReleaseGraphicsResources(window);
    s->Mask2RGBTable->Delete();
    s->Mask2RGBTable = NULL;
  }
  if (s->OpacityTables) {
    s->OpacityTables->ReleaseGraphicsResources(window);
    delete s->OpacityTables;
    s->OpacityTables = 0;
  }
  if (s->GradientOpacityTables) {
    s->GradientOpacityTables->ReleaseGraphicsResources(window);
    delete s->GradientOpacityTables;
    s->GradientOpacityTables = 0;
  }
#endif
}
//----------------------------------------------------------------------------
void BuildShader(OpenGLGPUVolumeRayCast* s, Volume* vol, int noOfComponents)
{
  std::string vertexShader(raycastervs);
  std::string fragmentShader(raycasterfs);
  // Every volume should have a property (cannot be NULL);
  int independentComponents = s->IndependentComponents;
  if (s->Shade) {
    int i;
    s->NumberOfLights = 0;
    // Compute light complexity.
    for (i = 0; i < s->nLights; ++i) {
      Light* light = s->Lights + i;
      float status = light->Switch;
      if (status > 0.0) {
        s->NumberOfLights++;
        if (s->LightComplexity == 0) {
          s->LightComplexity = 1;
        }
      }
      if (s->LightComplexity == 1 && (s->NumberOfLights > 1 || light->Intensity != 1.0
          || light->LightType != LIGHT_TYPE_HEADLIGHT)) {
        s->LightComplexity = 2;
      }
      if (s->LightComplexity < 3 && (light->Positional)) {
        s->LightComplexity = 3;
        break;
      }
    }
  }
  // Base methods replacements
  //--------------------------------------------------------------------------
  vertexShader = replace(vertexShader, "//VTK::ComputeClipPos::Impl", ComputeClipPositionImplementation(s), true);
  vertexShader = replace(vertexShader, "//VTK::ComputeTextureCoords::Impl", ComputeTextureCoordinates(s), true);
  vertexShader = replace(vertexShader, "//VTK::Base::Dec", BaseDeclarationVertex(s), true);
  fragmentShader = replace(fragmentShader, "//VTK::Base::Dec", BaseDeclarationFragment(s, s->LightComplexity,
      s->HasGradientOpacity, noOfComponents, independentComponents), true);
  fragmentShader = replace(fragmentShader, "//VTK::Base::Init", BaseInit(s, s->LightComplexity), true);
  fragmentShader = replace(fragmentShader, "//VTK::Base::Impl", BaseImplementation(s), true);
  fragmentShader = replace(fragmentShader, "//VTK::Base::Exit", BaseExit(s), true);
  // Termination methods replacements
  //--------------------------------------------------------------------------
  vertexShader = replace(vertexShader, "//VTK::Termination::Dec", TerminationDeclarationVertex(s), true);
  fragmentShader = replace(fragmentShader, "//VTK::Termination::Dec", TerminationDeclarationFragment(s), true);
  fragmentShader = replace(fragmentShader, "//VTK::Terminate::Init", TerminationInit(s), true);
  fragmentShader = replace(fragmentShader, "//VTK::Terminate::Impl", TerminationImplementation(s), true);
  fragmentShader = replace(fragmentShader, "//VTK::Terminate::Exit", TerminationExit(s), true);
  // Shading methods replacements
  //--------------------------------------------------------------------------
  vertexShader = replace(vertexShader, "//VTK::Shading::Dec", ShadingDeclarationVertex(s), true);
  fragmentShader = replace(fragmentShader, "//VTK::Shading::Dec", ShadingDeclarationFragment(s), true);
  fragmentShader = replace(fragmentShader, "//VTK::Shading::Init", ShadingInit(s), true);
  fragmentShader = replace(fragmentShader, "//VTK::Shading::Impl", ShadingImplementation(s, s->MaskInput, s->CurrentMask,
      s->MaskType, noOfComponents, independentComponents), true);
  fragmentShader = replace(fragmentShader, "//VTK::Shading::Exit", ShadingExit(s, noOfComponents, independentComponents), true);
  // Compute methods replacements
  //--------------------------------------------------------------------------
  fragmentShader = replace(fragmentShader, "//VTK::ComputeOpacity::Dec", ComputeOpacityDeclaration(s, noOfComponents, independentComponents, s->OpacityTablesMap), true);
  fragmentShader = replace(fragmentShader, "//VTK::ComputeGradient::Dec", ComputeGradientDeclaration(s, noOfComponents, independentComponents, s->GradientOpacityTablesMap), true);
  fragmentShader = replace(fragmentShader, "//VTK::ComputeColor::Dec", ComputeColorDeclaration(s, noOfComponents, independentComponents, s->RGBTablesMap), true);
  fragmentShader = replace(fragmentShader, "//VTK::ComputeLighting::Dec", ComputeLightingDeclaration(s, noOfComponents, independentComponents, s->NumberOfLights, s->LightComplexity), true);
  fragmentShader = replace(fragmentShader, "//VTK::ComputeRayDirection::Dec", ComputeRayDirectionDeclaration(s), true);
  // Cropping methods replacements
  //--------------------------------------------------------------------------
  vertexShader = replace(vertexShader, "//VTK::Cropping::Dec", CroppingDeclarationVertex(s), true);
  fragmentShader = replace(fragmentShader, "//VTK::Cropping::Dec", CroppingDeclarationFragment(s), true);
  fragmentShader = replace(fragmentShader, "//VTK::Cropping::Init", CroppingInit(s), true);
  fragmentShader = replace(fragmentShader, "//VTK::Cropping::Impl", CroppingImplementation(s), true);
  fragmentShader = replace(fragmentShader, "//VTK::Cropping::Exit", CroppingExit(s), true);
  // Clipping methods replacements
  //--------------------------------------------------------------------------
  vertexShader = replace(vertexShader, "//VTK::Clipping::Dec", ClippingDeclarationVertex(s), true);
  fragmentShader = replace(fragmentShader, "//VTK::Clipping::Dec", ClippingDeclarationFragment(s), true);
  fragmentShader = replace(fragmentShader, "//VTK::Clipping::Init", ClippingInit(s), true);
  fragmentShader = replace(fragmentShader, "//VTK::Clipping::Impl", ClippingImplementation(s), true);
  fragmentShader = replace(fragmentShader, "//VTK::Clipping::Exit", ClippingExit(s), true);
  // Masking methods replacements
  //--------------------------------------------------------------------------
  fragmentShader = replace(fragmentShader, "//VTK::BinaryMask::Dec", BinaryMaskDeclaration(s, s->MaskInput, s->CurrentMask), true);
  fragmentShader = replace(fragmentShader, "//VTK::BinaryMask::Impl", BinaryMaskImplementation(s, s->MaskInput, s->CurrentMask, s->MaskType), true);
  fragmentShader = replace(fragmentShader, "//VTK::CompositeMask::Dec", CompositeMaskDeclarationFragment(s, s->MaskInput, s->CurrentMask, s->MaskType), true);
  fragmentShader = replace(fragmentShader, "//VTK::CompositeMask::Impl", CompositeMaskImplementation(s, s->MaskInput, s->CurrentMask, s->MaskType, noOfComponents), true);
  // Picking replacements
  //--------------------------------------------------------------------------
  if (s->CurrentSelectionPass != (MIN_KNOWN_PASS - 1)) {
    switch (s->CurrentSelectionPass) {
    case ID_LOW24:
      fragmentShader = replace(fragmentShader, "//VTK::Picking::Exit", PickingIdLow24PassExit(s), true);
      break;
    case ID_MID24:
      fragmentShader = replace(fragmentShader, "//VTK::Picking::Exit", PickingIdMid24PassExit(s), true);
      break;
    default: // ACTOR_PASS, PROCESS_PASS
      fragmentShader = replace(fragmentShader, "//VTK::Picking::Dec", PickingActorPassDeclaration(s), true);
      fragmentShader = replace(fragmentShader, "//VTK::Picking::Exit", PickingActorPassExit(s), true);
      break;
    }
  }
  // Render to texture
  //--------------------------------------------------------------------------
  if (s->RenderToImage) {
    fragmentShader = replace(
        fragmentShader,
        "//VTK::RenderToImage::Init",
        RenderToImageInit(
            s), true);
    fragmentShader = replace(
        fragmentShader,
        "//VTK::RenderToImage::Impl",
        RenderToImageImplementation(
            s), true);
    fragmentShader = replace(
        fragmentShader,
        "//VTK::RenderToImage::Exit",
        RenderToImageExit(
            s), true);
  }
#if 0
  // Now compile the shader
  //--------------------------------------------------------------------------
  s->ShaderProgram = ReadyShaderProgram(s, vertexShader.c_str(), fragmentShader.c_str(), "");
  if (!s->ShaderProgram || !s->ShaderProgram->GetCompiled()) {
    vtkErrorMacro("Shader failed to compile");
  }
#endif
}
//-----------------------------------------------------------------------------
// Update the reduction factor of the render viewport (s->ReductionFactor)
// according to the time spent in seconds to render the previous frame
// (s->TimeToDraw) and a time in seconds allocated to render the next
// frame (allocatedTime).
// \pre valid_current_reduction_range: s->ReductionFactor>0.0 && s->ReductionFactor<=1.0
// \pre positive_TimeToDraw: s->TimeToDraw>=0.0
// \pre positive_time: allocatedTime>0.0
// \post valid_new_reduction_range: s->ReductionFactor>0.0 && s->ReductionFactor<=1.0
//-----------------------------------------------------------------------------
void ComputeReductionFactor(OpenGLGPUVolumeRayCast* s, double allocatedTime)
{
  if (!s->AutoAdjustSampleDistances) {
    s->ReductionFactor = 1.0 / s->ImageSampleDistance;
    return;
  }
#if 0
  if (s->TimeToDraw) {
    double oldFactor = s->ReductionFactor;
    double timeToDraw;
    if (allocatedTime < 1.0) {
      timeToDraw = s->SmallTimeToDraw;
      if (timeToDraw == 0.0) {
        timeToDraw = s->BigTimeToDraw / 3.0;
      }
    }
    else {
      timeToDraw = s->BigTimeToDraw;
    }
    // This should be the case when rendering the volume very first time
    // 10.0 is an arbitrary value chosen which happen to a large number
    // in s context
    if (timeToDraw == 0.0) {
      timeToDraw = 10.0;
    }
    double fullTime = timeToDraw / s->ReductionFactor;
    double newFactor = allocatedTime / fullTime;
    // Compute average factor
    s->ReductionFactor = (newFactor + oldFactor) / 2.0;
    // Discretize reduction factor so that it doesn't cause
    // visual artifacts when used to reduce the sample distance
    s->ReductionFactor = (s->ReductionFactor > 1.0) ? 1.0 :
        (s->ReductionFactor);
    if (s->ReductionFactor < 0.20) {
      s->ReductionFactor = 0.10;
    }
    else if (s->ReductionFactor < 0.50) {
      s->ReductionFactor = 0.20;
    }
    else if (s->ReductionFactor < 1.0) {
      s->ReductionFactor = 0.50;
    }
    // Clamp it
    if (1.0 / s->ReductionFactor > s->MaximumImageSampleDistance) {
      s->ReductionFactor = 1.0 / s->MaximumImageSampleDistance;
    }
    if (1.0 / s->ReductionFactor < s->MinimumImageSampleDistance) {
      s->ReductionFactor = 1.0 / s->MinimumImageSampleDistance;
    }
  }
#endif
}
//----------------------------------------------------------------------------
void DoGPURender(OpenGLGPUVolumeRayCast* s, ImageData* input, Camera* cam, GLuint prog, int noOfComponents, int independentComponents, Matrix4x4 volMatrix)
{
  // Temporary variables
  float fvalue2[2];
  float fvalue3[3];
  float fvalue4[4];
  // Bind textures
  //--------------------------------------------------------------------------
  Activate(s->VolumeTextureObject);
  SetUniformi(prog, "in_volume", s->VolumeTextureObject->Handle);
  // Opacity, color, and gradient opacity samplers / textures
  int numberOfSamplers = (independentComponents ? noOfComponents : 1);
  int i;
  for (i = 0; i < numberOfSamplers; ++i) {
    Activate(s->OpacityTables + i);
    SetUniformi(prog, s->OpacityTablesMap[i].c_str(), s->OpacityTables[i].Handle);
    if (s->BlendMode != ADDITIVE_BLEND) {
      Activate(s->RGBTables + i);
      SetUniformi(prog, s->RGBTablesMap[i].c_str(), s->RGBTables[i].Handle);
    }
    if (s->GradientOpacityTables) {
      Activate(s->GradientOpacityTables + i);
      SetUniformi(prog, s->GradientOpacityTablesMap[i].c_str(), s->GradientOpacityTables[i].Handle);
    }
  }
  if (s->NoiseTextureObject) {
    Activate(s->NoiseTextureObject);
    SetUniformi(prog, "in_noiseSampler", s->NoiseTextureObject->Handle);
  }
  // currently broken on ES
#if GL_ES_VERSION_2_0 != 1
  Activate(s->DepthTextureObject);
  SetUniformi(prog, "in_depthSampler", s->DepthTextureObject->Handle);
#endif
  if (s->CurrentMask) {
    Activate(s->CurrentMask);
    SetUniformi(prog, "in_mask", s->CurrentMask->Handle);
  }
  if (noOfComponents == 1 &&
      s->BlendMode != ADDITIVE_BLEND) {
    if (s->MaskInput != 0 && s->MaskType == LabelMapMaskType) {
      Activate(s->Mask1RGBTable);
      SetUniformi(prog, "in_mask1", s->Mask1RGBTable->Handle);
      Activate(s->Mask2RGBTable);
      SetUniformi(prog, "in_mask2", s->Mask2RGBTable->Handle);
      SetUniformf(prog, "in_maskBlendFactor", s->MaskBlendFactor);
    }
  }
  // Bind light and material properties
  //--------------------------------------------------------------------------
  SetLightingParameters(s, s->Lights, s->nLights, s->cam);
  float ambient[4][3];
  float diffuse[4][3];
  float specular[4][3];
  float specularPower[4];
  for (i = 0; i < numberOfSamplers; ++i) {
    ambient[i][0] = ambient[i][1] = ambient[i][2] = s->Ambient[i];
    diffuse[i][0] = diffuse[i][1] = diffuse[i][2] = s->Diffuse[i];
    specular[i][0] = specular[i][1] = specular[i][2] = s->Specular[i];
    specularPower[i] = s->SpecularPower[i];
  }
  SetUniform3fv(prog, "in_ambient", numberOfSamplers, ambient);
  SetUniform3fv(prog, "in_diffuse", numberOfSamplers, diffuse);
  SetUniform3fv(prog, "in_specular", numberOfSamplers, specular);
  SetUniform1fv(prog, "in_shininess", numberOfSamplers, specularPower);
  float clippingRange[2];
  VEC2CPY(clippingRange, cam->ClippingRange);
  // Bind matrices
  //--------------------------------------------------------------------------
  Matrix4x4 glTransformMatrix;
  Matrix4x4 modelviewMatrix;
  Matrix3x3 normalMatrix;
  Matrix4x4 projectionMatrix;
  GetKeyMatrices(cam, modelviewMatrix, normalMatrix,
      projectionMatrix, glTransformMatrix);
  Copy44(s->InverseProjectionMat, projectionMatrix);
  Invert44(s->InverseProjectionMat);
  SetUniformMatrix(prog, "in_projectionMatrix", *projectionMatrix);
  SetUniformMatrix(prog, "in_inverseProjectionMatrix", *s->InverseProjectionMat);
  Copy44(s->InverseModelViewMat, modelviewMatrix);
  Invert44(s->InverseModelViewMat);
  SetUniformMatrix(prog, "in_modelViewMatrix", modelviewMatrix);
  SetUniformMatrix(prog, "in_inverseModelViewMatrix", s->InverseModelViewMat);
  Copy44(s->TempMatrix1, volMatrix);
  Transpose44(s->TempMatrix1);
  Copy44(s->InverseVolumeMat, s->TempMatrix1);
  Invert44(s->InverseVolumeMat);
  SetUniformMatrix(prog, "in_volumeMatrix", s->TempMatrix1);
  SetUniformMatrix(prog, "in_inverseVolumeMatrix", s->InverseVolumeMat);
  Copy44(s->TempMatrix1, s->TextureToDataSetMat);
  Transpose44(s->TempMatrix1);
  Copy44(s->InverseTextureToDataSetMat, s->TempMatrix1);
  Invert44(s->InverseTextureToDataSetMat);
  SetUniformMatrix(prog, "in_textureDatasetMatrix", s->TempMatrix1);
  SetUniformMatrix(prog, "in_inverseTextureDatasetMatrix", s->InverseTextureToDataSetMat);
  MultiplyMatrix44(s->TempMatrix1, modelviewMatrix, s->TextureToEyeTransposeInverse);
  MultiplyMatrix44(s->TextureToDataSetMat, s->TextureToEyeTransposeInverse, s->TextureToEyeTransposeInverse);
  Invert44(s->TextureToEyeTransposeInverse);
  SetUniformMatrix(prog, "in_texureToEyeIt", s->TextureToEyeTransposeInverse);
  // Bind other misc parameters
  //--------------------------------------------------------------------------
  bool ParallelProjection = true;
  if (ParallelProjection) {
    SetUniform3fv(prog, "in_projectionDirection", 1, &cam->DirectionOfProjection);
  }
  // Pass constant uniforms at initialization
  SetUniformi(prog, "in_noOfComponents", noOfComponents);
  SetUniformi(prog, "in_independentComponents", independentComponents);
  float tscale[4] = {1.0, 1.0, 1.0, 1.0};
  float tbias[4] = {0.0, 0.0, 0.0, 0.0};
  if (!s->HandleLargeDataTypes && (noOfComponents == 1 ||
      noOfComponents == 2 || independentComponents)) {
    for (int i = 0; i < noOfComponents; i++) {
      tscale[i] = s->Scale[i];
      tbias[i] = s->Bias[i];
    }
  }
  SetUniform4f(prog, "in_volume_scale", tscale);
  SetUniform4f(prog, "in_volume_bias", tbias);
  // Step should be dependant on the bounds and not on the texture size
  // since we can have non uniform voxel size / spacing / aspect ratio
  SetUniform3fv(prog, "in_cellStep", 1, &s->CellStep);
  SetUniform3fv(prog, "in_cellScale", 1, &s->CellScale);
  SetUniform3fv(prog, "in_cellSpacing", 1, &s->CellSpacing);
  SetUniformf(prog, "in_sampleDistance", s->ActualSampleDistance);
  float scalarsRange[4][2];
  SetUniform2fv(prog, "in_scalarsRange", 4, s->ScalarsRange);
  SetUniform3fv(prog, "in_cameraPos", 1, &cam->Position);
  ToFloat(s->LoadedBounds[0], s->LoadedBounds[2], s->LoadedBounds[4], fvalue3);
  SetUniform3fv(prog, "in_volumeExtentsMin", 1, &fvalue3);
  ToFloat(s->LoadedBounds[1], s->LoadedBounds[3], s->LoadedBounds[5], fvalue3);
  SetUniform3fv(prog, "in_volumeExtentsMax", 1, &fvalue3);
  ToFloat(s->Extents[0], s->Extents[2], s->Extents[4], fvalue3);
  SetUniform3fv(prog, "in_textureExtentsMin", 1, &fvalue3);
  ToFloat(s->Extents[1], s->Extents[3], s->Extents[5], fvalue3);
  SetUniform3fv(prog, "in_textureExtentsMax", 1, &fvalue3);
  // TODO Take consideration of reduction factor
  ToFloat(s->WindowLowerLeft, fvalue2);
  SetUniform2fv(prog, "in_windowLowerLeftCorner", 1, &fvalue2);
  ToFloat(1.0 / s->WindowSize[0],
      1.0 / s->WindowSize[1], fvalue2);
  SetUniform2fv(prog, "in_inverseOriginalWindowSize", 1, &fvalue2);
  ToFloat(1.0 / s->WindowSize[0],
      1.0 / s->WindowSize[1], fvalue2);
  SetUniform2fv(prog, "in_inverseWindowSize", 1, &fvalue2);
  SetUniformi(prog, "in_useJittering", s->UseJittering);
  SetUniformi(prog, "in_cellFlag", s->CellFlag);
  ToFloat(s->AdjustedTexMin[0],
      s->AdjustedTexMin[1],
      s->AdjustedTexMin[2], fvalue3);
  SetUniform3fv(prog, "in_texMin", 1, &fvalue3);
  ToFloat(s->AdjustedTexMax[0],
      s->AdjustedTexMax[1],
      s->AdjustedTexMax[2], fvalue3);
  SetUniform3fv(prog, "in_texMax", 1, &fvalue3);
  Copy44(s->TempMatrix1, s->CellToPointMatrix);
  Transpose44(s->TempMatrix1);
  SetUniformMatrix(prog, "in_cellToPoint", s->TempMatrix1);
  SetUniformi(prog, "in_clampDepthToBackface", s->ClampDepthToBackface);
  // Bind cropping
  //--------------------------------------------------------------------------
  SetCroppingRegions(s, prog);
  // Bind clipping
  //--------------------------------------------------------------------------
  SetClippingPlanes(s, prog);
  // Bind the prop Id
  //--------------------------------------------------------------------------
  if (s->CurrentSelectionPass < ID_LOW24) {
    SetPickingId(s);
  }
  // Set the scalar range to be considered for average ip blend
  //--------------------------------------------------------------------------
  double avgRange[2];
  AverageIPScalarRange();
  s->GetAverageIPScalarRange(avgRange);
  if (avgRange[1] < avgRange[0]) {
    double tmp = avgRange[1];
    avgRange[1] = avgRange[0];
    avgRange[0] = tmp;
  }
  ToFloat(avgRange[0], avgRange[1], fvalue2);
  SetUniform2fv(prog, "in_averageIPRange", 1, &fvalue2);
  // Finally set the scale and bias for color correction
  //--------------------------------------------------------------------------
  SetUniformf(prog, "in_scale", 1.0 / s->FinalColorWindow);
  SetUniformf(prog, "in_bias",
      (0.5 - (s->FinalColorLevel / s->FinalColorWindow)));
  if (noOfComponents > 1 && independentComponents) {
    for (int i = 0; i < noOfComponents; ++i) {
      fvalue4[i] = static_cast<float>(s->GetComponentWeight(i));
    }
    prog->SetUniform4fv("in_componentWeight", 1, &fvalue4);
  }
#if 0
  // Render volume geometry to trigger render
  //--------------------------------------------------------------------------
  s->RenderVolumeGeometry(s, prog, vol, input);
  // Undo binds and de-activate buffers
  //--------------------------------------------------------------------------
  s->VolumeTextureObject->Deactivate();
  if (s->NoiseTextureObject) {
    s->NoiseTextureObject->Deactivate();
  }
  s->DepthTextureObject->Deactivate();
  for (int i = 0; i < numberOfSamplers; ++i) {
    s->OpacityTables->GetTable(i)->Deactivate();
    if (s->BlendMode != ADDITIVE_BLEND) {
      s->RGBTables->GetTable(i)->Deactivate();
    }
    if (s->GradientOpacityTables) {
      s->GradientOpacityTables->GetTable(i)->Deactivate();
    }
  }
  if (s->CurrentMask) {
    s->CurrentMask->Deactivate();
  }
  if (noOfComponents == 1 &&
      s->BlendMode != ADDITIVE_BLEND) {
    if (s->MaskInput != 0 && s->MaskType == LabelMapMaskType) {
      s->Mask1RGBTable->Deactivate();
      s->Mask2RGBTable->Deactivate();
    }
  }
  OpenGLCheckError("failed after Render");
#endif
}
//----------------------------------------------------------------------------
void GPURender(OpenGLGPUVolumeRayCast* s)
{
#if 0
  vtkOpenGLClearErrorMacro();
  RegisterGraphicsResources(s->ResourceCallback, static_cast<vtkOpenGLRenderWindow*>(s->GetRenderWindow()));
  s->TempMatrix1->Identity44();
  // Make sure the context is current
  vtkOpenGLRenderWindow* renWin =
      vtkOpenGLRenderWindow::SafeDownCast(s->GetRenderWindow());
  renWin->MakeCurrent();
  // Update in_volume first to make sure states are current
  vol->Update();
  // Get the input
  ImageData* input = s->GetTransformedInput();
  // Get the volume property (must have one)
  vtkVolumeProperty* s = vol->GetProperty();
  // Get the camera
  vtkOpenGLCamera* cam = vtkOpenGLCamera::SafeDownCast(s->GetActiveCamera());
  // Check whether we have independent components or not
  int independentComponents = s->GetIndependentComponents();
  // Get window size and corners
  s->GetTiledSizeAndOrigin(
      s->WindowSize, s->WindowSize + 1,
      s->WindowLowerLeft, s->WindowLowerLeft + 1);
  vtkDataArray* scalars = s->GetScalars(input,
      s->ScalarMode,
      s->ArrayAccessMode,
      s->ArrayId,
      s->ArrayName,
      s->CellFlag);
  // How many components are there?
  int noOfComponents = scalars->GetNumberOfComponents();
  // Allocate important variables
  s->Bias.resize(noOfComponents, 0.0);
  {
    s->Initialize(s, vol, noOfComponents, independentComponents);
  }
  // Three dependent components are not supported
  if ((noOfComponents == 3) && !independentComponents) {
    vtkErrorMacro("Three dependent components are not supported");
  }
  for (int n = 0; n < noOfComponents; ++n) {
    double* range = scalars->GetRange(n);
    for (int i = 0; i < 2; ++i) {
      s->ScalarsRange[n][i] = range[i];
    }
  }
  // Update the volume if needed
  bool volumeModified = false;
  if (s->NeedToInitializeResources) {
    volumeModified = true;
    input->GetDimensions(s->Dimensions);
    // Update bounds, data, and geometry
    s->ComputeBounds(input);
    s->LoadVolume(s, input, s, scalars, independentComponents);
    s->ComputeCellToPointMatrix();
    s->LoadMask(s, input, s->MaskInput, s->Extents, vol);
  }
  else {
    s->LoadMask(s, input, s->MaskInput, s->Extents, vol);
    s->UpdateVolume(s);
  }
  // Mask
  vtkVolumeMask* mask = 0;
  if (s->MaskInput != 0) {
    std::map<ImageData*, vtkVolumeMask*>::iterator it2 =
        s->MaskTextures->Map.find(s->MaskInput);
    if (it2 == s->MaskTextures->Map.end()) {
      mask = 0;
    }
    else {
      mask = (*it2).second;
    }
  }
  s->CurrentMask = mask;
  s->ComputeReductionFactor(vol->GetAllocatedRenderTime());
  s->UpdateSamplingDistance(input, s, vol);
  // Update the transfer functions
  if (independentComponents) {
    for (int i = 0; i < noOfComponents; ++i) {
      s->UpdateOpacityTransferFunction(s, vol, i);
      s->UpdateGradientOpacityTransferFunction(s, vol, i);
      s->UpdateColorTransferFunction(s, vol, i);
    }
  }
  else {
    if (noOfComponents == 2 || noOfComponents == 4) {
      s->UpdateOpacityTransferFunction(s, vol, noOfComponents - 1);
      s->UpdateGradientOpacityTransferFunction(s, vol,
          noOfComponents - 1);
      s->UpdateColorTransferFunction(s, vol, 0);
    }
  }
  // Update noise sampler texture
  if (s->UseJittering) {
    s->CreateNoiseTexture(s);
  }
  // Grab depth sampler buffer (to handle cases when we are rendering geometry
  // and in_volume together
  s->CaptureDepthTexture(s, vol);
  // Update sampling distance
  int* loadedExtent = input->GetExtent();
  s->CellStep[0] =
      (1.0 / (double)(loadedExtent[1] - loadedExtent[0]));
  s->CellStep[1] =
      (1.0 / (double)(loadedExtent[3] - loadedExtent[2]));
  s->CellStep[2] =
      (1.0 / (double)(loadedExtent[5] - loadedExtent[4]));
  s->CellScale[0] = (s->LoadedBounds[1] -
      s->LoadedBounds[0]) * 0.5;
  s->CellScale[1] = (s->LoadedBounds[3] -
      s->LoadedBounds[2]) * 0.5;
  s->CellScale[2] = (s->LoadedBounds[5] -
      s->LoadedBounds[4]) * 0.5;
  s->DatasetStepSize[0] = 1.0 / (s->LoadedBounds[1] -
      s->LoadedBounds[0]);
  s->DatasetStepSize[1] = 1.0 / (s->LoadedBounds[3] -
      s->LoadedBounds[2]);
  s->DatasetStepSize[2] = 1.0 / (s->LoadedBounds[5] -
      s->LoadedBounds[4]);
  // Compute texture to dataset matrix
  s->TextureToDataSetMat->Identity44();
  s->TextureToDataSetMat->SetElement(0, 0,
      (1.0 / s->DatasetStepSize[0]));
  s->TextureToDataSetMat->SetElement(1, 1,
      (1.0 / s->DatasetStepSize[1]));
  s->TextureToDataSetMat->SetElement(2, 2,
      (1.0 / s->DatasetStepSize[2]));
  s->TextureToDataSetMat->SetElement(3, 3,
      1.0);
  s->TextureToDataSetMat->SetElement(0, 3,
      s->LoadedBounds[0]);
  s->TextureToDataSetMat->SetElement(1, 3,
      s->LoadedBounds[2]);
  s->TextureToDataSetMat->SetElement(2, 3,
      s->LoadedBounds[4]);
  s->ShaderCache = vtkOpenGLRenderWindow::SafeDownCast(
      s->GetRenderWindow())->GetShaderCache();
  s->CheckPickingState(s);
  if (s->UseDepthPass && s->GetBlendMode() ==
      vtkVolumeMapper::COMPOSITE_BLEND) {
    s->CurrentPass = DepthPass;
    if (s->NeedToInitializeResources ||
        s->GetMTime() > s->DepthPassSetupTime.GetMTime() ||
        s->GetMTime() > s->DepthPassSetupTime.GetMTime() ||
        cam->GetParallelProjection() != s->LastProjectionParallel ||
        s->SelectionStateTime.GetMTime() > s->ShaderBuildTime.GetMTime()) {
      s->LastProjectionParallel =
          cam->GetParallelProjection();
      s->ContourFilter->SetInputData(input);
      for (int i = 0; i < s->GetDepthPassContourValues()->GetNumberOfContours(); ++i) {
        s->ContourFilter->SetValue(i,
            s->DepthPassContourValues->GetValue(i));
      }
      vtkNew<Matrix4x4> newMatrix;
      newMatrix->DeepCopy(vol->GetMatrix());
      s->SetupDepthPass(s);
      s->ContourActor->Render(s,
          s->ContourMapper);
      s->ExitDepthPass(s);
      s->DepthPassSetupTime.Modified();
      s->DepthPassTime.Modified();
      s->CurrentPass = RenderPass;
      s->BuildShader(s, vol, noOfComponents);
    }
    else if (cam->GetMTime() > s->DepthPassTime.GetMTime()) {
      s->SetupDepthPass(s);
      s->ContourActor->Render(s,
          s->ContourMapper);
      s->ExitDepthPass(s);
      s->DepthPassTime.Modified();
      s->CurrentPass = RenderPass;
    }
    // Configure picking begin (changes blending, so needs to be called before
    // vtkVolumeStateRAII)
    if (s->IsPicking) {
      s->BeginPicking(s);
    }
    // Set OpenGL states
    vtkVolumeStateRAII glState;
    if (s->RenderToImage) {
      s->SetupRenderToTexture(s);
    }
    // NOTE: This is a must call or else, multiple viewport
    // rendering would not work. We need s primarily because
    // FBO set it otherwise.
    glViewport(s->WindowLowerLeft[0],
        s->WindowLowerLeft[1],
        s->WindowSize[0],
        s->WindowSize[1]);
    renWin->GetShaderCache()->ReadyShaderProgram(s->ShaderProgram);
    s->DPDepthBufferTextureObject->Activate();
    s->ShaderProgram->SetUniformi("in_depthPassSampler",
        s->DPDepthBufferTextureObject->Handle);
    s->DoGPURender(s, vol, input,
        cam, s->ShaderProgram,
        noOfComponents, independentComponents);
    s->DPDepthBufferTextureObject->Deactivate();
  }
  else {
    // Configure picking begin (changes blending, so needs to be called before
    // vtkVolumeStateRAII)
    if (s->IsPicking) {
      s->BeginPicking(s);
    }
    // Set OpenGL states
    vtkVolumeStateRAII glState;
    // Build shader now
    // First get the shader cache from the render window. This is important
    // to make sure that shader cache knows the state of various shader programs
    // in use.
    if (s->NeedToInitializeResources ||
        s->GetMTime() >
        s->ShaderBuildTime.GetMTime() ||
        s->GetMTime() > s->ShaderBuildTime.GetMTime() ||
        cam->GetParallelProjection() != s->LastProjectionParallel ||
        s->SelectionStateTime.GetMTime() > s->ShaderBuildTime.GetMTime()) {
      s->LastProjectionParallel =
          cam->GetParallelProjection();
      s->BuildShader(s, vol, noOfComponents);
    }
    else {
      // Bind the shader
      s->ShaderCache->ReadyShaderProgram(
          s->ShaderProgram);
    }
    if (s->RenderToImage) {
      s->SetupRenderToTexture(s);
      s->DoGPURender(s, vol, input,
          cam, s->ShaderProgram,
          noOfComponents, independentComponents);
      s->ExitRenderToTexture(s);
    }
    else {
      s->DoGPURender(s, vol, input,
          cam, s->ShaderProgram,
          noOfComponents, independentComponents);
    }
  }
  // Configure picking end
  if (s->IsPicking) {
    EndPicking(s, s);
  }
  glFinish();
#endif
}

