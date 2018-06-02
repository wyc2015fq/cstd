#include <string>
#include "raycastervs.inl"
#include "raycasterfs.inl"
// Forward declarations
//class vtkShaderProgram;
//class vtkTextureObject;
enum MaskTypeT { NoMask, BinaryMaskType, LabelMapMaskType };
enum BlendModeT {
  COMPOSITE_BLEND,
  MAXIMUM_INTENSITY_BLEND,
  MINIMUM_INTENSITY_BLEND,
  AVERAGE_INTENSITY_BLEND,
  ADDITIVE_BLEND
};
enum PassTypes {
  PROCESS_PASS,
  ACTOR_PASS,
  COMPOSITE_INDEX_PASS,
  ID_LOW24,
  ID_MID24,
  ID_HIGH16,
  MAX_KNOWN_PASS = ID_HIGH16,
  MIN_KNOWN_PASS = PROCESS_PASS
};
enum Passes {
  RenderPass,
  DepthPass = 1
};
// DepthTextureCompareFunction values.
enum {
  Lequal = 0, // r=R<=Dt ? 1.0 : 0.0
  Gequal, // r=R>=Dt ? 1.0 : 0.0
  Less, // r=R<D_t ? 1.0 : 0.0
  Greater, // r=R>Dt ? 1.0 : 0.0
  Equal, // r=R==Dt ? 1.0 : 0.0
  NotEqual, // r=R!=Dt ? 1.0 : 0.0
  AlwaysTrue, // r=1.0 // WARNING "Always" is macro defined in X11/X.h...
  Never, // r=0.0
  NumberOfDepthTextureCompareFunctions
};
// ClampToBorder is not supported in ES 2.0
// Wrap values.
#if GL_ES_VERSION_2_0 != 1
enum WrapMode3D {
  ClampToEdge = 0,
  Repeat,
  MirroredRepeat,
  ClampToBorder,
  NumberOfWrapModes
};
#else
enum WrapMode3D {
  ClampToEdge = 0,
  Repeat,
  MirroredRepeat,
  NumberOfWrapModes
};
#endif
// MinificationFilter values.
enum InterpolationTypeT {
  Nearest = 0,
  Linear,
  NearestMipmapNearest,
  NearestMipmapLinear,
  LinearMipmapNearest,
  LinearMipmapLinear,
  NumberOfMinificationModes
};
// Internal depth format
enum {
  Native = 0, // will try to match with the depth buffer format.
  Fixed16,
  Fixed24,
  Fixed32,
  Float32,
  NumberOfDepthFormats
};
// Internal alpha format
enum {
  alpha = 0,
  alpha8,
  alpha16,
  alpha16f,
  alpha32f,
  NumberOfAlphaFormats
};
// Depth mode formats
enum {
  DepthAlpha = 0,
  DepthLuminance,
  DepthIntensity,
  NumberOfDepthModeFormats
};
// NOTE:
// In this code, we referred to various spaces described below:
// Object space: Raw coordinates in space defined by volume matrix
// Dataset space: Raw coordinates
// Eye space: Coordinates in eye space (as referred in computer graphics)
//--------------------------------------------------------------------------
std::string replace(std::string source, const std::string& search,
    const std::string& replace, bool all)
{
  if (replace.empty()) {
    return source;
  }
  std::string::size_type pos = 0;
  bool first = true;
  while ((pos = source.find(search, 0)) != std::string::npos) {
    source.replace(pos, search.length(), replace);
    pos += search.length();
    if (first) {
      first = false;
      if (!all) {
        return source;
      }
    }
  }
  return source;
}
#if 0
#include "VolumeShaderComposer.inl"
//----------------------------------------------------------------------------
int BuildShader(int noOfComponents)
{
  std::string vertexShader(raycastervs);
  std::string fragmentShader(raycasterfs);
  bool hasGradientOpacity = false;
  int LightComplexity = 0;
  int independentComponents = 0;
  bool DepthPass = false;
  bool CompositeBlend = false;
  int MaskType = NoMask;
  int BlendMode = MAXIMUM_INTENSITY_BLEND;
  // Base methods replacements
  //--------------------------------------------------------------------------
  vertexShader = replace(vertexShader, "//VTK::ComputeClipPos::Impl", ComputeClipPositionImplementation(), true);
  vertexShader = replace(vertexShader, "//VTK::ComputeTextureCoords::Impl", ComputeTextureCoordinates(), true);
  vertexShader = replace(vertexShader, "//VTK::Base::Dec", BaseDeclarationVertex(), true);
  fragmentShader = replace(fragmentShader, "//VTK::Base::Dec", BaseDeclarationFragment(LightComplexity, hasGradientOpacity, noOfComponents, independentComponents, DepthPass), true);
  fragmentShader = replace(fragmentShader, "//VTK::Base::Init", BaseInit(LightComplexity, CompositeBlend, hasGradientOpacity), true);
  fragmentShader = replace(fragmentShader, "//VTK::Base::Impl", BaseImplementation(), true);
  fragmentShader = replace(fragmentShader, "//VTK::Base::Exit", BaseExit(), true);
  // Termination methods replacements
  //--------------------------------------------------------------------------
  vertexShader = replace(vertexShader, "//VTK::Termination::Dec", TerminationDeclarationVertex(), true);
  fragmentShader = replace(fragmentShader, "//VTK::Termination::Dec", TerminationDeclarationFragment(), true);
  fragmentShader = replace(fragmentShader, "//VTK::Terminate::Init", TerminationInit(), true);
  fragmentShader = replace(fragmentShader, "//VTK::Terminate::Impl", TerminationImplementation(), true);
  fragmentShader = replace(fragmentShader, "//VTK::Terminate::Exit", TerminationExit(), true);
  // Shading methods replacements
  //--------------------------------------------------------------------------
  vertexShader = replace(vertexShader, "//VTK::Shading::Dec", ShadingDeclarationVertex(), true);
  fragmentShader = replace(fragmentShader, "//VTK::Shading::Dec", ShadingDeclarationFragment(), true);
  fragmentShader = replace(fragmentShader, "//VTK::Shading::Init", ShadingInit(BlendMode), true);
  fragmentShader = replace(fragmentShader, "//VTK::Shading::Impl", ShadingImplementation(MaskType, noOfComponents, independentComponents, BlendMode, DepthPass), true);
  fragmentShader = replace(fragmentShader, "//VTK::Shading::Exit", ShadingExit(noOfComponents, independentComponents, DepthPass, BlendMode), true);
  // Compute methods replacements
  //--------------------------------------------------------------------------
  char** OpacityTablesMap = NULL;
  char** GradientOpacityTablesMap = NULL;
  char** RGBTablesMap = NULL;
  int NumberOfLights = 0;
  fragmentShader = replace(fragmentShader, "//VTK::ComputeOpacity::Dec", ComputeOpacityDeclaration(noOfComponents, independentComponents, OpacityTablesMap), true);
  fragmentShader = replace(fragmentShader, "//VTK::ComputeGradient::Dec", ComputeGradientDeclaration(noOfComponents, independentComponents, GradientOpacityTablesMap, hasGradientOpacity), true);
  fragmentShader = replace(fragmentShader, "//VTK::ComputeColor::Dec", ComputeColorDeclaration(noOfComponents, independentComponents, RGBTablesMap), true);
  fragmentShader = replace(fragmentShader, "//VTK::ComputeLighting::Dec", ComputeLightingDeclaration(noOfComponents, independentComponents, NumberOfLights, LightComplexity, hasGradientOpacity), true);
  fragmentShader = replace(fragmentShader, "//VTK::ComputeRayDirection::Dec", ComputeRayDirectionDeclaration(noOfComponents), true);
  int Cropping = 0;
  // Cropping methods replacements
  //--------------------------------------------------------------------------
  vertexShader = replace(vertexShader, "//VTK::Cropping::Dec", CroppingDeclarationVertex(), true);
  fragmentShader = replace(fragmentShader, "//VTK::Cropping::Dec", CroppingDeclarationFragment(Cropping), true);
  fragmentShader = replace(fragmentShader, "//VTK::Cropping::Init", CroppingInit(Cropping), true);
  fragmentShader = replace(fragmentShader, "//VTK::Cropping::Impl", CroppingImplementation(Cropping), true);
  fragmentShader = replace(fragmentShader, "//VTK::Cropping::Exit", CroppingExit(), true);
  // Clipping methods replacements
  //--------------------------------------------------------------------------
  int ClippingPlanes = 0;
  int ParallelProjection = 0;
  vertexShader = replace(vertexShader, "//VTK::Clipping::Dec", ClippingDeclarationVertex(), true);
  fragmentShader = replace(fragmentShader, "//VTK::Clipping::Dec", ClippingDeclarationFragment(), true);
  fragmentShader = replace(fragmentShader, "//VTK::Clipping::Init", ClippingInit(ClippingPlanes, ParallelProjection), true);
  fragmentShader = replace(fragmentShader, "//VTK::Clipping::Impl", ClippingImplementation(ClippingPlanes), true);
  fragmentShader = replace(fragmentShader, "//VTK::Clipping::Exit", ClippingExit(), true);
  // Masking methods replacements
  //--------------------------------------------------------------------------
  fragmentShader = replace(fragmentShader, "//VTK::BinaryMask::Dec", BinaryMaskDeclaration(MaskType), true);
  fragmentShader = replace(fragmentShader, "//VTK::BinaryMask::Impl", BinaryMaskImplementation(MaskType), true);
  fragmentShader = replace(fragmentShader, "//VTK::CompositeMask::Dec", CompositeMaskDeclarationFragment(MaskType), true);
  fragmentShader = replace(fragmentShader, "//VTK::CompositeMask::Impl", CompositeMaskImplementation(MaskType, noOfComponents), true);
  // Picking replacements
  //--------------------------------------------------------------------------
  int CurrentSelectionPass = 0;
  if (CurrentSelectionPass != (MIN_KNOWN_PASS - 1)) {
    switch (CurrentSelectionPass) {
    case ID_LOW24:
      fragmentShader = replace(fragmentShader, "//VTK::Picking::Exit", PickingIdLow24PassExit(), true);
      break;
    case ID_MID24:
      fragmentShader = replace(fragmentShader, "//VTK::Picking::Exit", PickingIdMid24PassExit(), true);
      break;
    default: // ACTOR_PASS, PROCESS_PASS
      fragmentShader = replace(fragmentShader, "//VTK::Picking::Dec", PickingActorPassDeclaration(), true);
      fragmentShader = replace(fragmentShader, "//VTK::Picking::Exit", PickingActorPassExit(), true);
      break;
    }
  }
  // Render to texture
  //--------------------------------------------------------------------------
  int RenderToImage = 0;
  if (RenderToImage) {
    fragmentShader = replace(fragmentShader, "//VTK::RenderToImage::Init", RenderToImageInit(), true);
    fragmentShader = replace(fragmentShader, "//VTK::RenderToImage::Impl", RenderToImageImplementation(), true);
    fragmentShader = replace(fragmentShader, "//VTK::RenderToImage::Exit", RenderToImageExit(), true);
  }
#if 0
  // Now compile the shader
  //--------------------------------------------------------------------------
  ShaderProgram = ShaderCache->ReadyShaderProgram(vertexShader.c_str(), fragmentShader.c_str(), "");
  if (!ShaderProgram || !ShaderProgram->GetCompiled()) {
    vtkErrorMacro("Shader failed to compile");
  }
#endif
  printf("%s\n", fragmentShader.c_str());
  return 0;
}
#endif
struct UniformNameT {
  char name[32];
  GLint loc;
};
int FindUniformName(UniformNameT* table, int len, const char* name)
{
  int i;
  for (i = 0; i < len; ++i) {
    if (0 == strcmp(name, table[i].name)) {
      return i;
    }
  }
  return -1;
}
GLint ShaderProgramFindUniform(GLuint prog, const char* name)
{
  static UniformNameT table[100] = {0};
  static int len = 0;
  int i = FindUniformName(table, len, name);
  if (i < 0) {
    ASSERT(len < countof(table));
    i = len++;
    //const char *allocStr = strdup(cname);
    strcpy(table[i].name, name);
    table[i].loc = (glGetUniformLocation(prog, (const GLchar*)name));
  }
  return table[i].loc;
}
bool SetUniformf(GLuint prog, const char* name, float f)
{
  GLint location = (GLint)ShaderProgramFindUniform(prog, name);
  if (location == -1) {
    printf("Could not set uniform (does not exist) %s", name);
    return false;
  }
  glUniform1f(location, (GLfloat)(f));
  return true;
}
bool SetUniformi(GLuint prog, const char* name, int i)
{
  GLint location = (GLint)(ShaderProgramFindUniform(prog, name));
  if (location == -1) {
    printf("Could not set uniform (does not exist) %s", name);
    return false;
  }
  glUniform1i(location, (GLint)(i));
  return true;
}
bool SetUniform3fv(GLuint prog, const char* name, const int count, const float(*v)[3])
{
  GLint location = (GLint)(ShaderProgramFindUniform(prog, name));
  if (location == -1) {
    printf("Could not set uniform (does not exist) %s", name);
    return false;
  }
  glUniform3fv(location, count, (const GLfloat*)v);
  return true;
}
bool SetUniform1fv(GLuint prog, const char* name, const int count, const float* v)
{
  GLint location = (GLint)(ShaderProgramFindUniform(prog, name));
  if (location == -1) {
    printf("Could not set uniform (does not exist) %s\!", name);
    return false;
  }
  glUniform1fv(location, count, (const GLfloat*)(v));
  return true;
}
bool SetUniform1iv(GLuint prog, const char* name, const int count, const int* v)
{
  GLint location = (GLint)(ShaderProgramFindUniform(prog, name));
  if (location == -1) {
    printf("Could not set uniform (does not exist) %s", name);
    return false;
  }
  glUniform1iv(location, count, (const GLint*)(v));
  return true;
}

bool SetUniformMatrix(GLuint prog, const char *name, const float16 matrix)
{
  GLint location = (GLint)ShaderProgramFindUniform(prog, name);
  if (location == -1)
  {
    printf("Could not set uniform (does not exist) %s", name);
    return false;
  }
  glUniformMatrix4fv(location, 1, GL_FALSE, matrix);
  return true;
}
bool SetUniformMatrix(GLuint prog, const char *name, const float44 matrix) {
  return SetUniformMatrix(prog, name, *matrix);
}
bool SetUniform4f(GLuint prog, const char *name, const float v[4])
{
  GLint location = (GLint)(ShaderProgramFindUniform(prog, name));
  if (location == -1)
  {
    printf("Could not set uniform (does not exist) %s", name);
    return false;
  }
  glUniform4fv(location, 1, v);
  return true;
}

bool SetUniform2fv(GLuint prog, const char *name, const int count, const float (*f)[2])
{
  GLint location = (GLint)(ShaderProgramFindUniform(prog, name));
  if (location == -1)
  {
    printf("Could not set uniform (does not exist) %s", name);
    return false;
  }
  glUniform2fv(location, count, (const GLfloat *)f);
  return true;
}
