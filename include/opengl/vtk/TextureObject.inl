const char* vtkTextureObjectVS =
    "//VTK::System::Dec\n"
    "\n"
    "\n"
    "attribute vec4 vertexMC;\n"
    "\n"
    "attribute vec2 tcoordMC;\n"
    "varying vec2 tcoordVC;\n"
    "\n"
    "void main()\n"
    "{\n"
    " tcoordVC = tcoordMC;\n"
    " gl_Position = vertexMC;\n"
    "}\n"
    "\n";
const char* vtkTextureObjectFS =
    "//VTK::System::Dec\n"
    "\n"
    "// Fragment shader used by the gaussian blur filter render pass.\n"
    "\n"
    "varying vec2 tcoordVC;\n"
    "uniform sampler2D source;\n"
    "\n"
    "// the output of this shader\n"
    "//VTK::Output::Dec\n"
    "\n"
    "void main(void)\n"
    "{\n"
    " gl_FragData[0] = texture2D(source,tcoordVC);\n"
    "}\n"
    "\n";
#define BUFFER_OFFSET(i) (static_cast<char *>(NULL) + (i))
// Mapping from DepthTextureCompareFunction values to OpenGL values.
//----------------------------------------------------------------------------
static GLint OpenGLDepthTextureCompareFunction[8] = {
  GL_LEQUAL,
  GL_GEQUAL,
  GL_LESS,
  GL_GREATER,
  GL_EQUAL,
  GL_NOTEQUAL,
  GL_ALWAYS,
  GL_NEVER
};
//----------------------------------------------------------------------------
static const char* DepthTextureCompareFunctionAsString[8] = {
  "Lequal",
  "Gequal",
  "Less",
  "Greater",
  "Equal",
  "NotEqual",
  "AlwaysTrue",
  "Never"
};
// Mapping from Wrap values to OpenGL values
#if GL_ES_VERSION_2_0 != 1
//--------------------------------------------------------------------------
static GLint OpenGLWrap[4] = {
  GL_CLAMP_TO_EDGE,
  GL_REPEAT,
  GL_MIRRORED_REPEAT,
  GL_CLAMP_TO_BORDER
};
//--------------------------------------------------------------------------
static const char* WrapAsString[4] = {
  "ClampToEdge",
  "Repeat",
  "MirroredRepeat",
  "ClampToBorder"
};
//----------------------------------------------------------------------------
static GLenum OpenGLAlphaInternalFormat[5] = {
  GL_R8,
  GL_R8,
  GL_R16,
  GL_R16F,
  GL_R32F
};
#else
//--------------------------------------------------------------------------
static GLint OpenGLWrap[3] = {
  GL_CLAMP_TO_EDGE,
  GL_REPEAT,
  GL_MIRRORED_REPEAT
};
//--------------------------------------------------------------------------
static const char* WrapAsString[3] = {
  "ClampToEdge",
  "Repeat",
  "MirroredRepeat"
};
//----------------------------------------------------------------------------
// Should be GL_RED but that requires an extension for ES 2.0
static GLenum OpenGLAlphaInternalFormat[5] = {
  GL_LUMINANCE,
  GL_LUMINANCE,
  GL_LUMINANCE,
  GL_LUMINANCE,
  GL_LUMINANCE
};
#endif
// Mapping MinificationFilter values to OpenGL values.
//----------------------------------------------------------------------------
static GLint OpenGLMinFilter[6] = {
  GL_NEAREST,
  GL_LINEAR,
  GL_NEAREST_MIPMAP_NEAREST,
  GL_NEAREST_MIPMAP_LINEAR,
  GL_LINEAR_MIPMAP_NEAREST,
  GL_LINEAR_MIPMAP_LINEAR
};
// Mapping MagnificationFilter values to OpenGL values.
//----------------------------------------------------------------------------
static GLint OpenGLMagFilter[6] = {
  GL_NEAREST,
  GL_LINEAR
};
//----------------------------------------------------------------------------
static const char* MinMagFilterAsString[6] = {
  "Nearest",
  "Linear",
  "NearestMipmapNearest",
  "NearestMipmapLinear",
  "LinearMipmapNearest",
  "LinearMipmapLinear"
};
//----------------------------------------------------------------------------
static GLenum OpenGLDepthInternalFormat[5] = {
  GL_DEPTH_COMPONENT,
  GL_DEPTH_COMPONENT16,
#ifdef GL_DEPTH_COMPONENT24
  GL_DEPTH_COMPONENT24,
#else
  GL_DEPTH_COMPONENT16,
#endif
#ifdef GL_DEPTH_COMPONENT32
  GL_DEPTH_COMPONENT32,
#else
  GL_DEPTH_COMPONENT16,
#endif
#ifdef GL_DEPTH_COMPONENT32F
  GL_DEPTH_COMPONENT32F
#else
  GL_DEPTH_COMPONENT16
#endif
};
//----------------------------------------------------------------------------
static GLenum OpenGLDepthInternalFormatType[5] = {
  GL_UNSIGNED_INT,
  GL_UNSIGNED_INT,
  GL_UNSIGNED_INT,
  GL_UNSIGNED_INT,
#ifdef GL_DEPTH_COMPONENT32F
  GL_FLOAT
#else
  GL_UNSIGNED_INT
#endif
};
/*
static const char *DepthInternalFormatFilterAsString[6]=
{
 "Native",
 "Fixed16",
 "Fixed24",
 "Fixed32",
 "Float32"
};
*/
inline
const char* vtkOpenGLStrError(unsigned int code)
{
  switch (static_cast<GLenum>(code)) {
  case GL_NO_ERROR:
    return "No error";
  case GL_INVALID_ENUM:
    return "Invalid enum";
  case GL_INVALID_VALUE:
    return "Invalid value";
  case GL_INVALID_OPERATION:
    return "Invalid operation";
  case GL_OUT_OF_MEMORY:
    return "Out of memory";
#if GL_ES_VERSION_2_0 != 1
  case GL_STACK_OVERFLOW:
    return "Stack overflow";
  case GL_STACK_UNDERFLOW:
    return "Stack underflow";
  case GL_TABLE_TOO_LARGE:
    return "Table too large";
  case GL_INVALID_FRAMEBUFFER_OPERATION_EXT:
    return "Invalid framebuffer operation";
#endif
  }
  return "Unknown error";
}
inline
int vtkGetOpenGLErrors(int maxNum,
    unsigned int* errCode,
    const char** errDesc)
{
  int i = 0;
  GLenum code = glGetError();
  if (i < maxNum) {
    errCode[i] = static_cast<unsigned int>(code);
    errDesc[i] = vtkOpenGLStrError(code);
  }
  while (code != GL_NO_ERROR && i < maxNum) {
    i++;
    code = glGetError();
    if (i < maxNum) {
      errCode[i] = static_cast<unsigned int>(code);
      errDesc[i] = vtkOpenGLStrError(code);
    }
  }
  return i;
}
inline
void vtkPrintOpenGLErrors(int maxErrors, int numErrors, unsigned int* errCode, const char** errDesc)
{
  printf("%d OpenGL errors detected\n", numErrors);
  for (int i = 0; (i < numErrors) && (i < maxErrors); ++i) {
    printf(" %d : (%d) %s\n", i, errCode[i], errDesc[i]);
  }
  if (numErrors > maxErrors) {
    printf("More than %d detected! The remainder are not reported\n", maxErrors);
  }
}
int OpenGLCheckError(const char* message)
{
  const int maxErrors = 16;
  unsigned int errCode[maxErrors] = {0};
  const char* errDesc[maxErrors] = {NULL};
  int numErrors = vtkGetOpenGLErrors(maxErrors, errCode, errDesc);
  if (numErrors) {
    printf("%s ", message);
    vtkPrintOpenGLErrors(maxErrors, numErrors, errCode, errDesc);
  }
  return 0;
}
typedef struct Texture {
  GLenum Target;
  GLuint Handle;
};
//----------------------------------------------------------------------------
void UnBind(Texture* tex)
{
  if (tex) {
    glBindTexture(tex->Handle, 0);
    OpenGLCheckError("failed at glBindTexture(0)");
  }
}
int TextureManager(Texture* tex, bool create, bool del)
{
  static GLuint TextureResourceIds[32] = {0};
  static int TextureResourceIdsLen = 0;
  int i = 0;
  if (del) {
    for (i = 0; i < TextureResourceIdsLen; ++i) {
      if (TextureResourceIds[i] == tex->Handle) {
        TextureResourceIds[i] = 0;
        return i;
      }
    }
    return 0;
  }
  for (i = 0; i < TextureResourceIdsLen; ++i) {
    if (TextureResourceIds[i] == tex->Handle) {
      return i;
    }
  }
  if (create) {
    for (i = 0; i < TextureResourceIdsLen; ++i) {
      if (TextureResourceIds[i] == 0) {
        TextureResourceIds[i] = tex->Handle;
        return i;
      }
    }
    ASSERT(TextureResourceIdsLen < countof(TextureResourceIds));
    TextureResourceIds[TextureResourceIdsLen++] = tex->Handle;
    return i;
  }
  return -1;
}
int ActivateTexture(Texture* tex)
{
  // Only add if it isn't already there
  int i = TextureManager(tex, true, false);
  if (i < 0) {
    vtkErrorMacro("Hardware does not support the number of textures defined.");
    return 0;
  }
  glActiveTexture(GL_TEXTURE0 + i);
  return i;
}
void Bind(Texture* tex)
{
  glBindTexture(tex->Target, tex->Handle);
  OpenGLCheckError("failed at glBindTexture");
}
//---------------------------------------------------------------------------
void Activate(Texture* tex)
{
  // activate a free texture unit for this texture
  ActivateTexture(tex);
  Bind(tex);
}
//---------------------------------------------------------------------------
void Deactivate(Texture* tex)
{
  if (tex) {
    ActivateTexture(tex);
    UnBind(tex);
    TextureManager(tex, false, true);
  }
}
//----------------------------------------------------------------------------
void DestroyTexture(Texture* tex)
{
  if (tex) {
    Deactivate(tex);
    glDeleteTextures(1, &tex->Handle);
    OpenGLCheckError("failed at glDeleteTexture");
    tex->Handle = 0;
  }
}
#if 0
//----------------------------------------------------------------------------
bool IsSupported(bool requireTexFloat, bool requireDepthFloat, bool requireTexInt)
{
#if GL_ES_VERSION_2_0 != 1
  if (vtkOpenGLRenderWindow::GetContextSupportsOpenGL32()) {
    return true;
  }
  bool texFloat = true;
  if (requireTexFloat) {
    texFloat = (glewIsSupported("GL_ARB_texture_float") != 0
        && glewIsSupported("GL_ARB_texture_rg") != 0);
  }
  bool depthFloat = true;
  if (requireDepthFloat) {
    depthFloat = (glewIsSupported("GL_ARB_depth_buffer_float") != 0);
  }
  bool texInt = true;
  if (requireTexInt) {
    texInt = (glewIsSupported("GL_EXT_texture_integer") != 0);
  }
#else
  bool texFloat = true;
  bool depthFloat = !requireDepthFloat;
  bool texInt = !requireTexInt;
#if GL_ES_VERSION_3_0 == 1
  texFloat = true;
  depthFloat = true; // I think this is the case
  texInt = true;
#endif
#endif
  return texFloat && depthFloat && texInt;
}
//----------------------------------------------------------------------------
bool vtkTextureObject::LoadRequiredExtensions(vtkOpenGLRenderWindow* renWin)
{
#if GL_ES_VERSION_2_0 != 1
  if (vtkOpenGLRenderWindow::GetContextSupportsOpenGL32()) {
    this->SupportsTextureInteger = true;
    this->SupportsTextureFloat = true;
    this->SupportsDepthBufferFloat = true;
  }
  else {
    this->SupportsTextureInteger = (glewIsSupported("GL_EXT_texture_integer") != 0);
    this->SupportsTextureFloat =
        (glewIsSupported("GL_ARB_texture_float") != 0 &&
            glewIsSupported("GL_ARB_texture_rg") != 0);
    this->SupportsDepthBufferFloat =
        (glewIsSupported("GL_ARB_depth_buffer_float") != 0);
  }
#else
  // some of these may have extensions etc for ES 2.0
  // setting to false right now as I do not know
  this->SupportsTextureInteger = false;
  this->SupportsTextureFloat = true;
  this->SupportsDepthBufferFloat = false;
#if GL_ES_VERSION_3_0 == 1
  this->SupportsTextureInteger = true;
  this->SupportsTextureFloat = true;
  this->SupportsDepthBufferFloat = true;
#endif
#endif
  return this->IsSupported(renWin,
      this->RequireTextureFloat,
      this->RequireDepthBufferFloat,
      this->RequireTextureInteger);
}
#endif
#if GL_ES_VERSION_2_0 != 1
#define SupportsTextureInteger() (glewIsSupported("GL_EXT_texture_integer") != 0)
#else
#if GL_ES_VERSION_3_0 == 1
#define SupportsTextureInteger() (true)
#define SupportsTextureFloat() (true)
#define SupportsDepthBufferFloat() (true)
#else
#define SupportsTextureInteger() (false)
#define SupportsTextureFloat() (true)
#define SupportsDepthBufferFloat() (false)
#endif
#endif
//----------------------------------------------------------------------------
unsigned int GetDefaultFormat(int vtktype, int numComps, bool shaderSupportsTextureInt)
{
  if (vtktype == VTK_VOID) {
    return GL_DEPTH_COMPONENT;
  }
#if GL_ES_VERSION_2_0 != 1
  if (SupportsTextureInteger() && shaderSupportsTextureInt && (vtktype == VTK_CHAR || vtktype == VTK_UCHAR ||
      vtktype == VTK_SHORT || vtktype == VTK_USHORT || vtktype == VTK_INT || vtktype == VTK_UINT)) {
    switch (numComps) {
    case 1:
      return GL_RED_INTEGER;
    case 2:
      return GL_RG_INTEGER;
    case 3:
      return GL_RGB_INTEGER_EXT;
    case 4:
      return GL_RGBA_INTEGER_EXT;
    }
  }
  else {
    switch (numComps) {
    case 1:
      return GL_RED;
    case 2:
      return GL_RG;
    case 3:
      return GL_RGB;
    case 4:
      return GL_RGBA;
    }
  }
#else
  {
    switch (numComps) {
#ifdef GL_RED
    case 1:
      return GL_RED;
    case 2:
      return GL_RG;
#else
    case 1:
      return GL_LUMINANCE;
    case 2:
      return GL_LUMINANCE_ALPHA;
#endif
    case 3:
      return GL_RGB;
    case 4:
      return GL_RGBA;
    }
  }
#endif
  return GL_RGB;
}
#define SetContextSupportsOpenGL32(val) OpenGLRenderWindowContextSupportsOpenGL32 = val
#define GetContextSupportsOpenGL32() OpenGLRenderWindowContextSupportsOpenGL32
static int OpenGLRenderWindowContextSupportsOpenGL32 = 0;
static double MaximumHardwareLineWidth = 0;
static int LineSmoothing = 0;
void OpenGLInitContext()
{
  // When a new OpenGL context is created, force an update
  static int Initialized = 0;
  if (!Initialized) {
#ifdef GLEW_OK
    GLenum result = glewInit();
    bool m_valid = (result == GLEW_OK);
    if (!m_valid) {
      vtkErrorMacro("GLEW could not be initialized.");
      return;
    }
    if (!GLEW_VERSION_3_2 && !GLEW_VERSION_3_1) {
      if (!GLEW_EXT_gpu_shader4) {
        vtkErrorMacro("GL version 2.1 with the gpu_shader4 extension is not "
            "supported by your graphics driver but is required for the new "
            "OpenGL rendering backend. Please update your OpenGL driver. "
            "If you are using Mesa please make sure you have version 10.6.5 or "
            "later and make sure your driver in Mesa supports OpenGL 3.2.");
        return;
      }
      vtkWarningMacro(
          "VTK is designed to work with OpenGL version 3.2 but it appears "
          "it has been given a context that does not support 3.2. VTK will "
          "run in a compatibility mode designed to work with earlier versions "
          "of OpenGL but some features may not work.");
    }
    else {
      OpenGLRenderWindowContextSupportsOpenGL32 = (true);
    }
#endif
    Initialized = true;
    // get this system's supported maximum line width
    // we do it here and store it to avoid repeated glGet
    // calls when the result should not change
    GLfloat lineWidthRange[2];
    MaximumHardwareLineWidth = 1.0;
#if defined(GL_SMOOTH_LINE_WIDTH_RANGE) && defined(GL_ALIASED_LINE_WIDTH_RANGE)
    if (LineSmoothing) {
      glGetFloatv(GL_SMOOTH_LINE_WIDTH_RANGE, lineWidthRange);
      if (glGetError() == GL_NO_ERROR) {
        MaximumHardwareLineWidth = lineWidthRange[1];
      }
    }
    else {
      glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, lineWidthRange);
      if (glGetError() == GL_NO_ERROR) {
        MaximumHardwareLineWidth = lineWidthRange[1];
      }
    }
#endif
  }
}
int GetDefaultTextureInternalFormat(int vtktype, int numComponents, bool needInt, bool needFloat)
{
  static int TextureInternalFormats[VTK_UNICODE_STRING][3][5] = {0};
  // 0 = none
  // 1 = float
  // 2 = int
  static int inited = 0;
  if (inited == 0) {
    inited = 1;
    TextureInternalFormats[VTK_VOID][0][1] = GL_DEPTH_COMPONENT;
#ifdef GL_R8
    TextureInternalFormats[VTK_UCHAR][0][1] = GL_R8;
    TextureInternalFormats[VTK_UCHAR][0][2] = GL_RG8;
    TextureInternalFormats[VTK_UCHAR][0][3] = GL_RGB8;
    TextureInternalFormats[VTK_UCHAR][0][4] = GL_RGBA8;
#else
    TextureInternalFormats[VTK_UCHAR][0][1] = GL_LUMINANCE;
    TextureInternalFormats[VTK_UCHAR][0][2] = GL_LUMINANCE_ALPHA;
    TextureInternalFormats[VTK_UCHAR][0][3] = GL_RGB;
    TextureInternalFormats[VTK_UCHAR][0][4] = GL_RGBA;
#endif
#ifdef GL_R16
    TextureInternalFormats[VTK_USHORT][0][1] = GL_R16;
    TextureInternalFormats[VTK_USHORT][0][2] = GL_RG16;
    TextureInternalFormats[VTK_USHORT][0][3] = GL_RGB16;
    TextureInternalFormats[VTK_USHORT][0][4] = GL_RGBA16;
#endif
#ifdef GL_R8_SNORM
    TextureInternalFormats[VTK_CHAR][0][1] = GL_R8_SNORM;
    TextureInternalFormats[VTK_CHAR][0][2] = GL_RG8_SNORM;
    TextureInternalFormats[VTK_CHAR][0][3] = GL_RGB8_SNORM;
    TextureInternalFormats[VTK_CHAR][0][4] = GL_RGBA8_SNORM;
#endif
#ifdef GL_R16_SNORM
    TextureInternalFormats[VTK_SHORT][0][1] = GL_R16_SNORM;
    TextureInternalFormats[VTK_SHORT][0][2] = GL_RG16_SNORM;
    TextureInternalFormats[VTK_SHORT][0][3] = GL_RGB16_SNORM;
    TextureInternalFormats[VTK_SHORT][0][4] = GL_RGBA16_SNORM;
#endif
    bool haveFloatTextures = false;
    bool haveIntTextures = false;
#if GL_ES_VERSION_2_0 != 1
    if (GetContextSupportsOpenGL32()) {
      haveFloatTextures = true;
      haveIntTextures = true;
    }
    else {
      haveFloatTextures = (glewIsSupported("GL_ARB_texture_float") != 0
          && glewIsSupported("GL_ARB_texture_rg") != 0);
      haveIntTextures = (glewIsSupported("GL_EXT_texture_integer") != 0);
    }
#else
    haveFloatTextures = true;
#if GL_ES_VERSION_3_0 == 1
    haveIntTextures = true;
#endif
#endif
    if (haveIntTextures) {
#ifdef GL_R8I
      TextureInternalFormats[VTK_CHAR][2][1] = GL_R8I;
      TextureInternalFormats[VTK_CHAR][2][2] = GL_RG8I;
      TextureInternalFormats[VTK_CHAR][2][3] = GL_RGB8I;
      TextureInternalFormats[VTK_CHAR][2][4] = GL_RGBA8I;
      TextureInternalFormats[VTK_UCHAR][2][1] = GL_R8UI;
      TextureInternalFormats[VTK_UCHAR][2][2] = GL_RG8UI;
      TextureInternalFormats[VTK_UCHAR][2][3] = GL_RGB8UI;
      TextureInternalFormats[VTK_UCHAR][2][4] = GL_RGBA8UI;
      TextureInternalFormats[VTK_SHORT][2][1] = GL_R16I;
      TextureInternalFormats[VTK_SHORT][2][2] = GL_RG16I;
      TextureInternalFormats[VTK_SHORT][2][3] = GL_RGB16I;
      TextureInternalFormats[VTK_SHORT][2][4] = GL_RGBA16I;
      TextureInternalFormats[VTK_USHORT][2][1] = GL_R16UI;
      TextureInternalFormats[VTK_USHORT][2][2] = GL_RG16UI;
      TextureInternalFormats[VTK_USHORT][2][3] = GL_RGB16UI;
      TextureInternalFormats[VTK_USHORT][2][4] = GL_RGBA16UI;
      TextureInternalFormats[VTK_INT][2][1] = GL_R32I;
      TextureInternalFormats[VTK_INT][2][2] = GL_RG32I;
      TextureInternalFormats[VTK_INT][2][3] = GL_RGB32I;
      TextureInternalFormats[VTK_INT][2][4] = GL_RGBA32I;
      TextureInternalFormats[VTK_UINT][2][1] = GL_R32UI;
      TextureInternalFormats[VTK_UINT][2][2] = GL_RG32UI;
      TextureInternalFormats[VTK_UINT][2][3] = GL_RGB32UI;
      TextureInternalFormats[VTK_UINT][2][4] = GL_RGBA32UI;
#endif
    }
    // on mesa we may not have float textures even though we think we do
    // this is due to Mesa being iompacted by a patent issue with SGI
#if GL_ES_VERSION_2_0 != 1
    if (haveFloatTextures) {
      const char* glVersion =
          reinterpret_cast<const char*>(glGetString(GL_VERSION));
      if (glVersion && strstr(glVersion, "Mesa") != NULL &&
          !GLEW_ARB_texture_float) {
        haveFloatTextures = false;
        // mesa without float support cannot even use
        // uchar textures with underlying float data
        // so pretty much anything with float data
        // is out of luck so return
        return 0;
      }
    }
#endif
    if (haveFloatTextures) {
#ifdef GL_R32F
      TextureInternalFormats[VTK_FLOAT][1][1] = GL_R32F;
      TextureInternalFormats[VTK_FLOAT][1][2] = GL_RG32F;
      TextureInternalFormats[VTK_FLOAT][1][3] = GL_RGB32F;
      TextureInternalFormats[VTK_FLOAT][1][4] = GL_RGBA32F;
      TextureInternalFormats[VTK_SHORT][1][1] = GL_R32F;
      TextureInternalFormats[VTK_SHORT][1][2] = GL_RG32F;
      TextureInternalFormats[VTK_SHORT][1][3] = GL_RGB32F;
      TextureInternalFormats[VTK_SHORT][1][4] = GL_RGBA32F;
#endif
    }
  }
  // 0 = none
  // 1 = float
  // 2 = int
  if (vtktype >= VTK_UNICODE_STRING) {
    return 0;
  }
  if (needInt) {
    return TextureInternalFormats[vtktype][2][numComponents];
  }
  if (needFloat) {
    return TextureInternalFormats[vtktype][1][numComponents];
  }
  return TextureInternalFormats[vtktype][0][numComponents];
}
//----------------------------------------------------------------------------
unsigned int GetDefaultInternalFormat(int vtktype, int numComps, bool shaderSupportsTextureInt)
{
  GLenum result = 0;
  // if shader supports int textures try that first
  if (shaderSupportsTextureInt) {
    result = GetDefaultTextureInternalFormat(vtktype, numComps, true, false);
    if (!result) {
      vtkDebugMacro("Unsupported internal texture type!");
    }
    return result;
  }
  // try default next
  result = GetDefaultTextureInternalFormat(vtktype, numComps, false, false);
  if (result) {
    return result;
  }
  // try floating point
  result = GetDefaultTextureInternalFormat(vtktype, numComps, false, true);
  if (!result) {
    vtkDebugMacro("Unsupported internal texture type!");
    vtkDebugMacro("Unable to find suitable internal format for T=%d NC= SSTI=%d", vtktype, numComps, shaderSupportsTextureInt);
  }
  return result;
}
//----------------------------------------------------------------------------
int GetDefaultDataType(int vtk_scalar_type)
{
  // DON'T DEAL with VTK_CHAR as this is platform dependent.
  switch (vtk_scalar_type) {
  case VTK_CHAR:
    return GL_BYTE;
  case VTK_UCHAR:
    return GL_UNSIGNED_BYTE;
  case VTK_SHORT:
    return GL_SHORT;
  case VTK_USHORT:
    return GL_UNSIGNED_SHORT;
  case VTK_INT:
    return GL_INT;
  case VTK_UINT:
    return GL_UNSIGNED_INT;
  case VTK_FLOAT:
  case VTK_VOID: // used for depth component textures.
    return GL_FLOAT;
  }
  return 0;
}
//----------------------------------------------------------------------------
unsigned int GetMinificationFilterMode(InterpolationTypeT vtktype)
{
  switch (vtktype) {
  case Nearest:
    return GL_NEAREST;
  case Linear:
    return GL_LINEAR;
  case NearestMipmapNearest:
    return GL_NEAREST_MIPMAP_NEAREST;
  case NearestMipmapLinear:
    return GL_NEAREST_MIPMAP_LINEAR;
  case LinearMipmapNearest:
    return GL_LINEAR_MIPMAP_NEAREST;
  case LinearMipmapLinear:
    return GL_LINEAR_MIPMAP_LINEAR;
  default:
    return GL_NEAREST;
  }
}
//----------------------------------------------------------------------------
unsigned int GetMagnificationFilterMode(InterpolationTypeT vtktype)
{
  switch (vtktype) {
  case Nearest:
    return GL_NEAREST;
  case Linear:
    return GL_LINEAR;
  default:
    return GL_NEAREST;
  }
}
//----------------------------------------------------------------------------
unsigned int GetWrapMode(WrapMode3D vtktype)
{
  switch (vtktype) {
  case ClampToEdge:
    return GL_CLAMP_TO_EDGE;
  case Repeat:
    return GL_REPEAT;
#ifdef GL_CLAMP_TO_BORDER
  case ClampToBorder:
    return GL_CLAMP_TO_BORDER;
#endif
  case MirroredRepeat:
    return GL_MIRRORED_REPEAT;
  default:
    return GL_CLAMP_TO_EDGE;
  }
}
//----------------------------------------------------------------------------
void CreateTexture(Texture* tex, int Target, InterpolationTypeT MinificationFilter, InterpolationTypeT MagnificationFilter,
    WrapMode3D WrapS, WrapMode3D WrapT, WrapMode3D WrapR)
{
  // reuse the existing handle if we have one
  if (tex) {
    glGenTextures(1, &tex->Handle);
    OpenGLCheckError("failed at glGenTextures");
#if defined(GL_TEXTURE_BUFFER)
    if (Target && Target != GL_TEXTURE_BUFFER)
#else
    if (Target)
#endif
    {
      tex->Target = Target;
      glBindTexture(Target, tex->Handle);
      OpenGLCheckError("failed at glBindTexture");
      // See: http://www.opengl.org/wiki/Common_Mistakes#Creating_a_complete_texture
      // turn off mip map filter or set the base and max level correctly. here
      // both are done.
      glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GetMinificationFilterMode(MinificationFilter));
      glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, GetMagnificationFilterMode(MagnificationFilter));
      glTexParameteri(Target, GL_TEXTURE_WRAP_S, GetWrapMode(WrapS));
      glTexParameteri(Target, GL_TEXTURE_WRAP_T, GetWrapMode(WrapT));
#if defined(GL_TEXTURE_3D)
      if (Target == GL_TEXTURE_3D) {
        glTexParameteri(Target, GL_TEXTURE_WRAP_R, GetWrapMode(WrapR));
      }
#endif
#ifdef GL_TEXTURE_BASE_LEVEL
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
#endif
#ifdef GL_TEXTURE_MAX_LEVEL
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
#endif
      glBindTexture(Target, 0);
    }
  }
}
int TexWrap(Texture* tex, int dim, WrapMode3D Wrap)
{
  int pname = 0;
  switch (dim) {
  case 'S':
    pname = GL_TEXTURE_WRAP_S;
    break;
  case 'T':
    pname = GL_TEXTURE_WRAP_T;
    break;
#if defined(GL_TEXTURE_3D)
  case 'R':
    if (GL_TEXTURE_3D == tex->Target) {
      pname = GL_TEXTURE_WRAP_R;
    }
    break;
#endif
  }
  if (pname) {
    glTexParameteri(tex->Target, pname, GetWrapMode(Wrap));
    return 1;
  }
  return 0;
}
//----------------------------------------------------------------------------
bool Create3DFromRaw(Texture* tex, unsigned int width, unsigned int height, unsigned int depth, int numComps, int dataType, const void* data)
{
  // Now, detemine texture parameters using the arguments.
  int Type = GetDefaultDataType(dataType);
  int InternalFormat = GetDefaultInternalFormat(dataType, numComps, false);
  int Format = GetDefaultFormat(dataType, numComps, false);
  if (!InternalFormat || !Format || !Type) {
    vtkErrorMacro("Failed to detemine texture parameters.");
    return false;
  }
  int Target = GL_TEXTURE_3D;
  //int Components = numComps;
  //int NumberOfDimensions = 3;
  CreateTexture(tex, Target, Nearest, Nearest, ClampToEdge, ClampToEdge, ClampToEdge);
  ActivateTexture(tex);
  // Source texture data from the PBO.
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage3D(GL_TEXTURE_3D, 0, InternalFormat, (GLsizei)(width), (GLsizei)(height), (GLsizei)(depth),
      0, Format, Type, (const GLvoid*)(data));
  OpenGLCheckError("failed at glTexImage2D");
  return true;
}
void SetWrapS(Texture* tex, WrapMode3D WrapS)
{
  glTexParameteri(tex->Target, GL_TEXTURE_WRAP_S, GetWrapMode(WrapS));
}
void SetWrapT(Texture* tex, WrapMode3D WrapS)
{
  glTexParameteri(tex->Target, GL_TEXTURE_WRAP_T, GetWrapMode(WrapS));
}
void SetWrapR(Texture* tex, WrapMode3D WrapS)
{
  glTexParameteri(tex->Target, GL_TEXTURE_WRAP_R, GetWrapMode(WrapS));
}
void SetMinificationFilter(Texture* tex, InterpolationTypeT MinificationFilter)
{
  glTexParameteri(tex->Target, GL_TEXTURE_MIN_FILTER, GetMinificationFilterMode(MinificationFilter));
}
void SetMagnificationFilter(Texture* tex, InterpolationTypeT MagnificationFilter)
{
  glTexParameteri(tex->Target, GL_TEXTURE_MAG_FILTER, OpenGLMagFilter[MagnificationFilter]);
}
void SetBorderColorfv(Texture* tex, const float* BorderColor)
{
  glTexParameterfv(tex->Target, GL_TEXTURE_BORDER_COLOR, BorderColor);
}
void SetBorderColor(Texture* tex, float r, float g, float b, float a)
{
  float BorderColor[] = {r, g, b, a};
  glTexParameterfv(tex->Target, GL_TEXTURE_BORDER_COLOR, BorderColor);
}
//---------------------------------------------------------------------------
void ReleaseGraphicsResources(Texture* tex)
{
  // Ensure that the context is current before releasing any graphics
  // resources tied to it.
  if (tex->Handle) {
    ActivateTexture(tex);
    UnBind(tex);
    Deactivate(tex);
    glDeleteTextures(1, &tex->Handle);
    tex->Handle = 0;
    tex->Target = 0;
    //tex->NumberOfDimensions = 0;
    //tex->InternalFormat = 0;
    //tex->Format = 0;
    //tex->Type = 0;
    //tex->Components = 0;
    //tex->Width = tex->Height = tex->Depth = 0;
  }
}
#if 0
//----------------------------------------------------------------------------
vtkTextureObject::~vtkTextureObject()
{
  if (this->ResourceCallback) {
    this->ResourceCallback->Release();
    delete this->ResourceCallback;
    this->ResourceCallback = NULL;
  }
  if (this->ShaderProgram) {
    delete this->ShaderProgram;
    this->ShaderProgram = NULL;
  }
}
//----------------------------------------------------------------------------
void vtkTextureObject::SetContext(vtkOpenGLRenderWindow* renWin)
{
  this->ResourceCallback->RegisterGraphicsResources(renWin);
  // avoid pointless reassignment
  if (this->Context == renWin) {
    return;
  }
  this->ResetFormatAndType();
  this->Context = NULL;
  this->Modified();
  // all done if assigned null
  if (!renWin) {
    return;
  }
  if (!this->LoadRequiredExtensions(renWin)) {
    vtkErrorMacro("Required OpenGL extensions not supported by the context.");
    return;
  }
  // initialize
  this->Context = renWin;
  this->Context->MakeCurrent();
}
//---------------------------------------------------------------------------
int vtkTextureObject::GetTextureUnit()
{
  if (this->Context) {
    return this->Context->GetTextureUnitForTexture(this);
  }
  return -1;
}
//----------------------------------------------------------------------------
bool vtkTextureObject::IsBound()
{
  bool result = false;
  if (this->Context && this->Handle) {
    GLenum target = 0; // to avoid warnings.
    switch (this->Target) {
#if defined(GL_TEXTURE_1D) && defined(GL_TEXTURE_BINDING_1D)
    case GL_TEXTURE_1D:
      target = GL_TEXTURE_BINDING_1D;
      break;
#endif
    case GL_TEXTURE_2D:
      target = GL_TEXTURE_BINDING_2D;
      break;
#if defined(GL_TEXTURE_3D) && defined(GL_TEXTURE_BINDING_3D)
    case GL_TEXTURE_3D:
      target = GL_TEXTURE_BINDING_3D;
      break;
#endif
#if defined(GL_TEXTURE_BUFFER) && defined(GL_TEXTURE_BINDING_BUFFER)
    case GL_TEXTURE_BUFFER:
      target = GL_TEXTURE_BINDING_BUFFER;
      break;
#endif
#if defined(GL_TEXTURE_CUBE_MAP) && defined(GL_TEXTURE_BINDING_CUBE_MAP)
    case GL_TEXTURE_CUBE_MAP:
      target = GL_TEXTURE_BINDING_CUBE_MAP;
      break;
#endif
    default:
      assert("check: impossible case" && 0);
      break;
    }
    GLint objectId;
    glGetIntegerv(target, &objectId);
    result = static_cast<GLuint>(objectId) == this->Handle;
  }
  return result;
}
//----------------------------------------------------------------------------
void vtkTextureObject::SendParameters()
{
  assert("pre: is_bound" && this->IsBound());
#if defined(GL_TEXTURE_BUFFER)
  if (this->Target == GL_TEXTURE_BUFFER) {
    return;
  }
#endif
  glTexParameteri(this->Target, GL_TEXTURE_WRAP_S, OpenGLWrap[this->WrapS]);
  glTexParameteri(this->Target, GL_TEXTURE_WRAP_T, OpenGLWrap[this->WrapT]);
#ifdef GL_TEXTURE_WRAP_R
  glTexParameteri(this->Target, GL_TEXTURE_WRAP_R, OpenGLWrap[this->WrapR]);
#endif
  glTexParameteri(this->Target, GL_TEXTURE_MIN_FILTER, OpenGLMinFilter[this->MinificationFilter]);
  glTexParameteri(this->Target, GL_TEXTURE_MAG_FILTER, OpenGLMagFilter[this->MagnificationFilter]);
#if GL_ES_VERSION_2_0 != 1 || GL_ES_VERSION_3_0 == 1
#if GL_ES_VERSION_3_0 != 1
  glTexParameterfv(this->Target, GL_TEXTURE_BORDER_COLOR, this->BorderColor);
  if (this->DepthTextureCompare) {
    glTexParameteri(this->Target, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
  }
  else {
    glTexParameteri(this->Target, GL_TEXTURE_COMPARE_MODE, GL_NONE);
  }
#endif
  glTexParameterf(this->Target, GL_TEXTURE_MIN_LOD, this->MinLOD);
  glTexParameterf(this->Target, GL_TEXTURE_MAX_LOD, this->MaxLOD);
  glTexParameteri(this->Target, GL_TEXTURE_BASE_LEVEL, this->BaseLevel);
  glTexParameteri(this->Target, GL_TEXTURE_MAX_LEVEL, this->MaxLevel);
  glTexParameteri(this->Target, GL_TEXTURE_COMPARE_FUNC, OpenGLDepthTextureCompareFunction[this->DepthTextureCompareFunction]);
#endif
  OpenGLCheckError("failed after SendParameters");
  this->SendParametersTime.Modified();
}
//----------------------------------------------------------------------------
unsigned int vtkTextureObject::GetInternalFormat(int vtktype, int numComps,
    bool shaderSupportsTextureInt)
{
  if (this->InternalFormat) {
    return this->InternalFormat;
  }
  // pre-condition
  if (vtktype == VTK_VOID && numComps != 1) {
    vtkErrorMacro("Depth component texture must have 1 component only (" <<
        numComps << " requested");
    this->InternalFormat = 0;
    return this->InternalFormat;
  }
  this->InternalFormat =
      this->GetDefaultInternalFormat(vtktype, numComps, shaderSupportsTextureInt);
  if (!this->InternalFormat) {
    vtkDebugMacro("Unable to find suitable internal format for T="
        << vtktype << " NC=" << numComps << " SSTI="
        << shaderSupportsTextureInt);
  }
  return this->InternalFormat;
}
//----------------------------------------------------------------------------
void vtkTextureObject::SetInternalFormat(unsigned int glInternalFormat)
{
  if (this->InternalFormat != glInternalFormat) {
    this->InternalFormat = glInternalFormat;
    this->Modified();
  }
}
//----------------------------------------------------------------------------
static int vtkGetVTKType(GLenum gltype)
{
  // DON'T DEAL with VTK_CHAR as this is platform dependent.
  switch (gltype) {
  case GL_BYTE:
    return VTK_CHAR;
  case GL_UBYTE:
    return VTK_UCHAR;
  case GL_SHORT:
    return VTK_SHORT;
  case GL_USHORT:
    return VTK_USHORT;
  case GL_INT:
    return VTK_INT;
  case GL_UNSIGNED_INT:
    return VTK_UINT;
  case GL_FLOAT:
    return VTK_FLOAT;
  }
  return 0;
}
void vtkTextureObject::GetShiftAndScale(float& shift, float& scale)
{
  shift = 1.0;
  scale = 1.0;
  // check to see if this is an int format
  GLenum iresult = this->Context->GetDefaultTextureInternalFormat(
      vtkGetVTKType(this->Type), this->Components, true, false);
  // using an int texture format, no shift scale
  if (iresult == this->InternalFormat) {
    return;
  }
  // for all float type internal formats
  switch (this->Type) {
  case GL_BYTE:
    scale = (VTK_CHAR_MAX - VTK_CHAR_MIN) / 2.0;
    shift = scale + VTK_CHAR_MIN;
    break;
  case GL_UBYTE:
    scale = VTK_UCHAR_MAX;
    shift = 0.0;
    break;
  case GL_SHORT:
    // this may be off a tad
    scale = (VTK_SHORT_MAX - VTK_SHORT_MIN) / 2.0;
    shift = scale + VTK_SHORT_MIN;
    break;
  case GL_USHORT:
    scale = VTK_USHORT_MAX;
    shift = 0.0;
    break;
  case GL_INT:
    // this may be off a tad
    scale = (1.0 * VTK_INT_MAX - VTK_INT_MIN) / 2.0;
    shift = scale + VTK_INT_MIN;
    break;
  case GL_UNSIGNED_INT:
    scale = VTK_UINT_MAX;
    shift = 0.0;
    break;
  case GL_FLOAT:
  default:
    break;
  }
}
//----------------------------------------------------------------------------
unsigned int vtkTextureObject::GetFormat(int vtktype, int numComps,
    bool shaderSupportsTextureInt)
{
  if (!this->Format) {
    this->Format = this->GetDefaultFormat(vtktype,
        numComps, shaderSupportsTextureInt);
  }
  return this->Format;
}
//----------------------------------------------------------------------------
void vtkTextureObject::SetFormat(unsigned int glFormat)
{
  if (this->Format != glFormat) {
    this->Format = glFormat;
    this->Modified();
  }
}
//----------------------------------------------------------------------------
void vtkTextureObject::ResetFormatAndType()
{
  this->Format = 0;
  this->InternalFormat = 0;
  this->Type = 0;
}
//----------------------------------------------------------------------------
int vtkTextureObject::GetDefaultDataType(int vtk_scalar_type)
{
  // DON'T DEAL with VTK_CHAR as this is platform dependent.
  switch (vtk_scalar_type) {
  case VTK_CHAR:
    return GL_BYTE;
  case VTK_UCHAR:
    return GL_UBYTE;
  case VTK_SHORT:
    return GL_SHORT;
  case VTK_USHORT:
    return GL_USHORT;
  case VTK_INT:
    return GL_INT;
  case VTK_UINT:
    return GL_UNSIGNED_INT;
  case VTK_FLOAT:
  case VTK_VOID: // used for depth component textures.
    return GL_FLOAT;
  }
  return 0;
}
//----------------------------------------------------------------------------
int vtkTextureObject::GetVTKDataType()
{
  return ::vtkGetVTKType(this->Type);
}
//----------------------------------------------------------------------------
int vtkTextureObject::GetDataType(int vtk_scalar_type)
{
  if (!this->Type) {
    this->Type = this->GetDefaultDataType(vtk_scalar_type);
  }
  return this->Type;
}
//----------------------------------------------------------------------------
void vtkTextureObject::SetDataType(unsigned int glType)
{
  if (this->Type != glType) {
    this->Type = glType;
    this->Modified();
  }
}
//----------------------------------------------------------------------------
unsigned int vtkTextureObject::GetMagnificationFilterMode(int vtktype)
{
  switch (vtktype) {
  case Nearest:
    return GL_NEAREST;
  case Linear:
    return GL_LINEAR;
  default:
    return GL_NEAREST;
  }
}
//----------------------------------------------------------------------------
unsigned int vtkTextureObject::GetWrapSMode(int vtktype)
{
  switch (vtktype) {
  case ClampToEdge:
    return GL_CLAMP_TO_EDGE;
  case Repeat:
    return GL_REPEAT;
#ifdef GL_CLAMP_TO_BORDER
  case ClampToBorder:
    return GL_CLAMP_TO_BORDER;
#endif
  case MirroredRepeat:
    return GL_MIRRORED_REPEAT;
  default:
    return GL_CLAMP_TO_EDGE;
  }
}
//----------------------------------------------------------------------------
unsigned int vtkTextureObject::GetWrapTMode(int vtktype)
{
  return this->GetWrapSMode(vtktype);
}
//----------------------------------------------------------------------------
unsigned int vtkTextureObject::GetWrapRMode(int vtktype)
{
  return this->GetWrapSMode(vtktype);
}
// 1D textures are not supported in ES 2.0 or 3.0
#if GL_ES_VERSION_2_0 != 1
//----------------------------------------------------------------------------
bool vtkTextureObject::Create1D(int numComps,
    vtkPixelBufferObject* pbo,
    bool shaderSupportsTextureInt)
{
  assert(this->Context);
  assert(pbo->GetContext() == this->Context.GetPointer());
  GLenum target = GL_TEXTURE_1D;
  // Now, detemine texture parameters using the information from the pbo.
  // * internalFormat depends on number of components and the data type.
  GLenum internalFormat = this->GetInternalFormat(pbo->GetType(), numComps,
      shaderSupportsTextureInt);
  // * format depends on the number of components.
  GLenum format = this->GetFormat(pbo->GetType(), numComps,
      shaderSupportsTextureInt);
  // * type if the data type in the pbo
  GLenum type = this->GetDefaultDataType(pbo->GetType());
  if (!internalFormat || !format || !type) {
    vtkErrorMacro("Failed to detemine texture parameters.");
    return false;
  }
  this->Target = target;
  this->Context->ActivateTexture(this);
  this->CreateTexture();
  this->Bind();
  pbo->Bind(vtkPixelBufferObject::UNPACKED_BUFFER);
  // Source texture data from the PBO.
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage1D(target, 0, static_cast<GLint>(internalFormat),
      static_cast<GLsizei>(pbo->GetSize() /
          static_cast<unsigned int>(numComps)),
      0, format,
      type, BUFFER_OFFSET(0));
  OpenGLCheckError("failed at glTexImage1D");
  pbo->UnBind();
  this->Deactivate();
  this->Target = target;
  this->Format = format;
  this->Type = type;
  this->Components = numComps;
  this->Width = pbo->GetSize();
  this->Height = 1;
  this->Depth = 1;
  this->NumberOfDimensions = 1;
  return true;
}
//----------------------------------------------------------------------------
bool vtkTextureObject::Create1DFromRaw(unsigned int width, int numComps,
    int dataType, void* data)
{
  assert(this->Context);
  // Now determine the texture parameters using the arguments.
  this->GetDataType(dataType);
  this->GetInternalFormat(dataType, numComps, false);
  this->GetFormat(dataType, numComps, false);
  if (!this->InternalFormat || !this->Format || !this->Type) {
    vtkErrorMacro("Failed to determine texture parameters.");
    return false;
  }
  GLenum target = GL_TEXTURE_1D;
  this->Target = target;
  this->Components = numComps;
  this->Width = width;
  this->Height = 1;
  this->Depth = 1;
  this->NumberOfDimensions = 1;
  this->Context->ActivateTexture(this);
  this->CreateTexture();
  this->Bind();
  glTexImage1D(this->Target,
      0,
      this->InternalFormat,
      static_cast<GLsizei>(this->Width),
      0,
      this->Format,
      this->Type,
      static_cast<const GLvoid*>(data));
  OpenGLCheckError("failed at glTexImage1D");
  this->Deactivate();
  return true;
}
// ----------------------------------------------------------------------------
// Description:
// Create a 1D alpha texture using a raw pointer.
// This is a blocking call. If you can, use PBO instead.
bool vtkTextureObject::CreateAlphaFromRaw(unsigned int width,
    int internalFormat,
    int rawType,
    void* raw)
{
  assert("pre: context_exists" && this->GetContext() != 0);
  assert("pre: raw_exists" && raw != 0);
  assert("pre: valid_internalFormat" && internalFormat >= 0
      && internalFormat < NumberOfAlphaFormats);
  // Now, detemine texture parameters using the arguments.
  this->GetDataType(rawType);
  if (!this->InternalFormat) {
    this->InternalFormat
      = OpenGLAlphaInternalFormat[internalFormat];
  }
  if (!this->InternalFormat || !this->Type) {
    vtkErrorMacro("Failed to detemine texture parameters.");
    return false;
  }
  this->Target = GL_TEXTURE_1D;
  this->Format = GL_RED;
  this->Width = width;
  this->Height = 1;
  this->Depth = 1;
  this->NumberOfDimensions = 1;
  this->Components = 1;
  this->Context->ActivateTexture(this);
  this->CreateTexture();
  this->Bind();
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage1D(this->Target, 0, static_cast<GLint>(this->InternalFormat),
      static_cast<GLsizei>(this->Width), 0,
      this->Format, this->Type, raw);
  OpenGLCheckError("failed at glTexImage1D");
  this->Deactivate();
  return true;
}
// Description:
// Create a texture buffer basically a 1D texture that can be
// very large for passing data into the fragment shader
bool vtkTextureObject::CreateTextureBuffer(unsigned int numValues, int numComps,
    int dataType, vtkOpenGLBufferObject* bo)
{
  assert(this->Context);
  // Now, detemine texture parameters using the arguments.
  this->GetDataType(dataType);
  this->GetInternalFormat(dataType, numComps, false);
  this->GetFormat(dataType, numComps, false);
  if (!this->InternalFormat || !this->Format || !this->Type) {
    vtkErrorMacro("Failed to detemine texture parameters.");
    return false;
  }
  this->Target = GL_TEXTURE_BUFFER;
  this->Components = numComps;
  this->Width = numValues;
  this->Height = 1;
  this->Depth = 1;
  this->NumberOfDimensions = 1;
  this->BufferObject = bo;
  this->Context->ActivateTexture(this);
  this->CreateTexture();
  this->Bind();
  // Source texture data from the PBO.
  glTexBuffer(
      this->Target,
      this->InternalFormat,
      this->BufferObject->GetHandle());
  OpenGLCheckError("failed at glTexBuffer");
  this->Deactivate();
  return true;
}
#else
// Description:
// Create a texture buffer basically a 1D texture that can be
// very large for passing data into the fragment shader
bool vtkTextureObject::CreateTextureBuffer(unsigned int numValues, int numComps,
    int dataType, vtkOpenGLBufferObject* bo)
{
  assert(this->Context);
  vtkErrorMacro("TextureBuffers not supported in OPenGL ES");
  // TODO: implement 1D and Texture buffers using 2D textures
  return false;
}
#endif // not ES 2.0 or 3.0
#if GL_ES_VERSION_2_0 != 1 || GL_ES_VERSION_3_0 == 1
//----------------------------------------------------------------------------
bool vtkTextureObject::Create2D(unsigned int width, unsigned int height,
    int numComps, vtkPixelBufferObject* pbo,
    bool shaderSupportsTextureInt)
{
  assert(this->Context);
  assert(pbo->GetContext() == this->Context.GetPointer());
  if (pbo->GetSize() < width * height * static_cast<unsigned int>(numComps)) {
    vtkErrorMacro("PBO size must match texture size.");
    return false;
  }
  // Now, detemine texture parameters using the information from the pbo.
  // * internalFormat depends on number of components and the data type.
  // * format depends on the number of components.
  // * type if the data type in the pbo
  int vtktype = pbo->GetType();
  GLenum type = this->GetDefaultDataType(vtktype);
  GLenum internalFormat
    = this->GetInternalFormat(vtktype, numComps, shaderSupportsTextureInt);
  GLenum format
    = this->GetFormat(vtktype, numComps, shaderSupportsTextureInt);
  if (!internalFormat || !format || !type) {
    vtkErrorMacro("Failed to detemine texture parameters.");
    return false;
  }
  GLenum target = GL_TEXTURE_2D;
  this->Target = target;
  this->Context->ActivateTexture(this);
  this->CreateTexture();
  this->Bind();
  // Source texture data from the PBO.
  pbo->Bind(vtkPixelBufferObject::UNPACKED_BUFFER);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(
      target,
      0,
      internalFormat,
      static_cast<GLsizei>(width),
      static_cast<GLsizei>(height),
      0,
      format,
      type,
      BUFFER_OFFSET(0));
  OpenGLCheckError("failed at glTexImage2D");
  pbo->UnBind();
  this->Deactivate();
  this->Target = target;
  this->Format = format;
  this->Type = type;
  this->Components = numComps;
  this->Width = width;
  this->Height = height;
  this->Depth = 1;
  this->NumberOfDimensions = 2;
  return true;
}
// ----------------------------------------------------------------------------
// Description:
// Create a 2D depth texture using a PBO.
bool vtkTextureObject::CreateDepth(unsigned int width,
    unsigned int height,
    int internalFormat,
    vtkPixelBufferObject* pbo)
{
  assert("pre: context_exists" && this->GetContext() != 0);
  assert("pre: pbo_context_exists" && pbo->GetContext() != 0);
  assert("pre: context_match" && this->GetContext() == pbo->GetContext());
  assert("pre: sizes_match" && pbo->GetSize() == width * height);
  assert("pre: valid_internalFormat" && internalFormat >= 0
      && internalFormat < NumberOfDepthFormats);
  GLenum inFormat = OpenGLDepthInternalFormat[internalFormat];
  GLenum type = this->GetDefaultDataType(pbo->GetType());
  this->Target = GL_TEXTURE_2D;
  this->Format = GL_DEPTH_COMPONENT;
  this->Type = type;
  this->Width = width;
  this->Height = height;
  this->Depth = 1;
  this->NumberOfDimensions = 2;
  this->Components = 1;
  this->Context->ActivateTexture(this);
  this->CreateTexture();
  this->Bind();
  pbo->Bind(vtkPixelBufferObject::UNPACKED_BUFFER);
  // Source texture data from the PBO.
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(this->Target, 0, static_cast<GLint>(inFormat),
      static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0,
      this->Format, this->Type, BUFFER_OFFSET(0));
  OpenGLCheckError("failed at glTexImage2D");
  pbo->UnBind();
  this->Deactivate();
  return true;
}
//----------------------------------------------------------------------------
bool vtkTextureObject::Create3D(unsigned int width, unsigned int height,
    unsigned int depth, int numComps,
    vtkPixelBufferObject* pbo,
    bool shaderSupportsTextureInt)
{
#ifdef GL_TEXTURE_3D
  assert(this->Context);
  assert(this->Context.GetPointer() == pbo->GetContext());
  if (pbo->GetSize() != width * height * depth * static_cast<unsigned int>(numComps)) {
    vtkErrorMacro("PBO size must match texture size.");
    return false;
  }
  GLenum target = GL_TEXTURE_3D;
  // Now, detemine texture parameters using the information from the pbo.
  // * internalFormat depends on number of components and the data type.
  GLenum internalFormat = this->GetInternalFormat(pbo->GetType(), numComps,
      shaderSupportsTextureInt);
  // * format depends on the number of components.
  GLenum format = this->GetFormat(pbo->GetType(), numComps,
      shaderSupportsTextureInt);
  // * type if the data type in the pbo
  GLenum type = this->GetDefaultDataType(pbo->GetType());
  if (!internalFormat || !format || !type) {
    vtkErrorMacro("Failed to detemine texture parameters.");
    return false;
  }
  this->Target = target;
  this->Context->ActivateTexture(this);
  this->CreateTexture();
  this->Bind();
  pbo->Bind(vtkPixelBufferObject::UNPACKED_BUFFER);
  // Source texture data from the PBO.
  glTexImage3D(target, 0, static_cast<GLint>(internalFormat),
      static_cast<GLsizei>(width), static_cast<GLsizei>(height),
      static_cast<GLsizei>(depth), 0, format, type,
      BUFFER_OFFSET(0));
  OpenGLCheckError("failed at glTexImage3D");
  pbo->UnBind();
  this->Deactivate();
  this->Target = target;
  this->Format = format;
  this->Type = type;
  this->Components = numComps;
  this->Width = width;
  this->Height = height;
  this->Depth = depth;
  this->NumberOfDimensions = 3;
  return true;
#else
  return false;
#endif
}
//----------------------------------------------------------------------------
vtkPixelBufferObject* vtkTextureObject::Download()
{
  assert(this->Context);
  assert(this->Handle);
  vtkPixelBufferObject* pbo = vtkPixelBufferObject::New();
  pbo->SetContext(this->Context);
  int vtktype = ::vtkGetVTKType(this->Type);
  if (vtktype == 0) {
    vtkErrorMacro("Failed to determine type.");
    return 0;
  }
  unsigned int size = this->Width * this->Height * this->Depth;
  // doesn't matter which Upload*D method we use since we are not really
  // uploading any data, simply allocating GPU space.
  if (!pbo->Upload1D(vtktype, NULL, size, this->Components, 0)) {
    vtkErrorMacro("Could not allocate memory for PBO.");
    pbo->Delete();
    return 0;
  }
  pbo->Bind(vtkPixelBufferObject::PACKED_BUFFER);
  this->Bind();
#if GL_ES_VERSION_2_0 != 1
  glGetTexImage(this->Target, 0, this->Format, this->Type, BUFFER_OFFSET(0));
#else
  // you can do something with glReadPixels and binding a texture as a FBO
  // I believe for ES 2.0
#endif
  OpenGLCheckError("failed at glGetTexImage");
  this->Deactivate();
  pbo->UnBind();
  pbo->SetComponents(this->Components);
  return pbo;
}
#endif
//----------------------------------------------------------------------------
bool vtkTextureObject::Create2DFromRaw(unsigned int width, unsigned int height,
    int numComps, int dataType, void* data)
{
  assert(this->Context);
  // Now determine the texture parameters using the arguments.
  this->GetDataType(dataType);
  this->GetInternalFormat(dataType, numComps, false);
  this->GetFormat(dataType, numComps, false);
  if (!this->InternalFormat || !this->Format || !this->Type) {
    vtkErrorMacro("Failed to determine texture parameters. IF="
        << this->InternalFormat << " F=" << this->Format << " T=" << this->Type);
    return false;
  }
  GLenum target = GL_TEXTURE_2D;
  this->Target = target;
  this->Components = numComps;
  this->Width = width;
  this->Height = height;
  this->Depth = 1;
  this->NumberOfDimensions = 2;
  this->Context->ActivateTexture(this);
  this->CreateTexture();
  this->Bind();
  // Source texture data from the PBO.
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(
      this->Target,
      0,
      this->InternalFormat,
      static_cast<GLsizei>(this->Width),
      static_cast<GLsizei>(this->Height),
      0,
      this->Format,
      this->Type,
      static_cast<const GLvoid*>(data));
  OpenGLCheckError("failed at glTexImage2D");
  this->Deactivate();
  return true;
}
//----------------------------------------------------------------------------
bool vtkTextureObject::CreateCubeFromRaw(unsigned int width, unsigned int height,
    int numComps, int dataType, void* data[6])
{
  assert(this->Context);
  // Now determine the texture parameters using the arguments.
  this->GetDataType(dataType);
  this->GetInternalFormat(dataType, numComps, false);
  this->GetFormat(dataType, numComps, false);
  if (!this->InternalFormat || !this->Format || !this->Type) {
    vtkErrorMacro("Failed to determine texture parameters. IF="
        << this->InternalFormat << " F=" << this->Format << " T=" << this->Type);
    return false;
  }
  GLenum target = GL_TEXTURE_CUBE_MAP;
  this->Target = target;
  this->Components = numComps;
  this->Width = width;
  this->Height = height;
  this->Depth = 1;
  this->NumberOfDimensions = 2;
  this->Context->ActivateTexture(this);
  this->CreateTexture();
  this->Bind();
  // Source texture data from the PBO.
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  for (int i = 0; i < 6; i++) {
    if (data[i]) {
      glTexImage2D(
          GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
          0,
          this->InternalFormat,
          static_cast<GLsizei>(this->Width),
          static_cast<GLsizei>(this->Height),
          0,
          this->Format,
          this->Type,
          static_cast<const GLvoid*>(data[i]));
      OpenGLCheckError("failed at glTexImage2D");
    }
  }
  this->Deactivate();
  return true;
}
// ----------------------------------------------------------------------------
// Description:
// Create a 2D depth texture using a raw pointer.
// This is a blocking call. If you can, use PBO instead.
bool vtkTextureObject::CreateDepthFromRaw(unsigned int width,
    unsigned int height,
    int internalFormat,
    int rawType,
    void* raw)
{
  assert("pre: context_exists" && this->GetContext() != 0);
  assert("pre: raw_exists" && raw != 0);
  assert("pre: valid_internalFormat" && internalFormat >= 0
      && internalFormat < NumberOfDepthFormats);
  // Now, detemine texture parameters using the arguments.
  this->GetDataType(rawType);
  if (!this->InternalFormat) {
    this->InternalFormat
      = OpenGLDepthInternalFormat[internalFormat];;
  }
  if (!this->InternalFormat || !this->Type) {
    vtkErrorMacro("Failed to detemine texture parameters.");
    return false;
  }
  this->Target = GL_TEXTURE_2D;
  this->Format = GL_DEPTH_COMPONENT;
  this->Width = width;
  this->Height = height;
  this->Depth = 1;
  this->NumberOfDimensions = 2;
  this->Components = 1;
  this->Context->ActivateTexture(this);
  this->CreateTexture();
  this->Bind();
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(this->Target, 0, static_cast<GLint>(this->InternalFormat),
      static_cast<GLsizei>(this->Width),
      static_cast<GLsizei>(this->Height), 0,
      this->Format, this->Type, raw);
  OpenGLCheckError("failed at glTexImage2D");
  this->Deactivate();
  return true;
}
// ----------------------------------------------------------------------------
bool vtkTextureObject::AllocateDepth(unsigned int width, unsigned int height,
    int internalFormat)
{
  assert("pre: context_exists" && this->GetContext() != 0);
  assert("pre: valid_internalFormat" && internalFormat >= 0
      && internalFormat < NumberOfDepthFormats);
  this->Target = GL_TEXTURE_2D;
  this->Format = GL_DEPTH_COMPONENT;
  // Try to match vtk type to internal fmt
  if (!this->Type) {
    this->Type = OpenGLDepthInternalFormatType[internalFormat];
  }
  if (!this->InternalFormat) {
    this->InternalFormat = OpenGLDepthInternalFormat[internalFormat];
  }
  this->Width = width;
  this->Height = height;
  this->Depth = 1;
  this->NumberOfDimensions = 2;
  this->Components = 1;
  this->Context->ActivateTexture(this);
  this->CreateTexture();
  this->Bind();
  glTexImage2D(
      this->Target,
      0,
      static_cast<GLint>(this->InternalFormat),
      static_cast<GLsizei>(this->Width),
      static_cast<GLsizei>(this->Height),
      0,
      this->Format,
      this->Type,
      0);
  OpenGLCheckError("failed at glTexImage2D");
  this->Deactivate();
  return true;
}
// ----------------------------------------------------------------------------
bool vtkTextureObject::Allocate1D(unsigned int width, int numComps,
    int vtkType)
{
#ifdef GL_TEXTURE_1D
  assert(this->Context);
  this->Target = GL_TEXTURE_1D;
  this->GetDataType(vtkType);
  this->GetInternalFormat(vtkType, numComps, false);
  this->GetFormat(vtkType, numComps, false);
  this->Components = numComps;
  this->Width = width;
  this->Height = 1;
  this->Depth = 1;
  this->NumberOfDimensions = 1;
  this->Context->ActivateTexture(this);
  this->CreateTexture();
  this->Bind();
  glTexImage1D(this->Target, 0, static_cast<GLint>(this->InternalFormat),
      static_cast<GLsizei>(this->Width), 0, this->Format,
      this->Type, 0);
  OpenGLCheckError("failed at glTexImage1D");
  this->Deactivate();
  return true;
#else
  return false;
#endif
}
// ----------------------------------------------------------------------------
// Description:
// Create a 2D color texture but does not initialize its values.
// Internal format is deduced from numComps and vtkType.
bool vtkTextureObject::Allocate2D(unsigned int width, unsigned int height,
    int numComps, int vtkType)
{
  assert(this->Context);
  this->Target = GL_TEXTURE_2D;
  this->GetDataType(vtkType);
  this->GetInternalFormat(vtkType, numComps, false);
  this->GetFormat(vtkType, numComps, false);
  this->Components = numComps;
  this->Width = width;
  this->Height = height;
  this->Depth = 1;
  this->NumberOfDimensions = 2;
  this->Context->ActivateTexture(this);
  this->CreateTexture();
  this->Bind();
  glTexImage2D(this->Target, 0, static_cast<GLint>(this->InternalFormat),
      static_cast<GLsizei>(this->Width),
      static_cast<GLsizei>(this->Height),
      0, this->Format, this->Type, 0);
  OpenGLCheckError("failed at glTexImage2D");
  this->Deactivate();
  return true;
}
// ----------------------------------------------------------------------------
// Description:
// Create a 3D color texture but does not initialize its values.
// Internal format is deduced from numComps and vtkType.
bool vtkTextureObject::Allocate3D(unsigned int width, unsigned int height,
    unsigned int depth, int numComps,
    int vtkType)
{
#ifdef GL_TEXTURE_3D
  this->Target = GL_TEXTURE_3D;
  if (this->Context == 0) {
    vtkErrorMacro("No context specified. Cannot create texture.");
    return false;
  }
  this->GetInternalFormat(vtkType, numComps, false);
  this->GetFormat(vtkType, numComps, false);
  this->GetDataType(vtkType);
  this->Components = numComps;
  this->Width = width;
  this->Height = height;
  this->Depth = depth;
  this->NumberOfDimensions = 3;
  this->Context->ActivateTexture(this);
  this->CreateTexture();
  this->Bind();
  glTexImage3D(this->Target, 0,
      static_cast<GLint>(this->InternalFormat),
      static_cast<GLsizei>(this->Width),
      static_cast<GLsizei>(this->Height),
      static_cast<GLsizei>(this->Depth), 0,
      this->Format, this->Type, 0);
  OpenGLCheckError("failed at glTexImage3D");
  this->Deactivate();
  return true;
#else
  return false;
#endif
}
//----------------------------------------------------------------------------
bool vtkTextureObject::Create2D(unsigned int width, unsigned int height,
    int numComps, int vtktype,
    bool shaderSupportsTextureInt)
{
  assert(this->Context);
  GLenum target = GL_TEXTURE_2D;
  // Now, detemine texture parameters using the information provided.
  this->GetDataType(vtktype);
  this->GetInternalFormat(vtktype, numComps, shaderSupportsTextureInt);
  this->GetFormat(vtktype, numComps, shaderSupportsTextureInt);
  if (!this->InternalFormat || !this->Format || !this->Type) {
    vtkErrorMacro("Failed to detemine texture parameters.");
    return false;
  }
  this->Target = target;
  this->Components = numComps;
  this->Width = width;
  this->Height = height;
  this->Depth = 1;
  this->NumberOfDimensions = 2;
  this->Context->ActivateTexture(this);
  this->CreateTexture();
  this->Bind();
  // Allocate space for texture, don't upload any data.
  glTexImage2D(target, 0,
      static_cast<GLint>(this->InternalFormat),
      static_cast<GLsizei>(this->Width),
      static_cast<GLsizei>(this->Height),
      0, this->Format, this->Type, NULL);
  OpenGLCheckError("failed at glTexImage2D");
  this->Deactivate();
  return true;
}
//----------------------------------------------------------------------------
bool vtkTextureObject::Create3D(unsigned int width, unsigned int height,
    unsigned int depth,
    int numComps, int vtktype,
    bool shaderSupportsTextureInt)
{
#ifdef GL_TEXTURE_3D
  assert(this->Context);
  GLenum target = GL_TEXTURE_3D;
  // Now, detemine texture parameters using the information provided.
  this->GetInternalFormat(vtktype, numComps, shaderSupportsTextureInt);
  this->GetFormat(vtktype, numComps, shaderSupportsTextureInt);
  this->GetDataType(vtktype);
  if (!this->InternalFormat || !this->Format || !this->Type) {
    vtkErrorMacro("Failed to detemine texture parameters.");
    return false;
  }
  this->Target = target;
  this->Components = numComps;
  this->Width = width;
  this->Height = height;
  this->Depth = depth;
  this->NumberOfDimensions = 3;
  this->Context->ActivateTexture(this);
  this->CreateTexture();
  this->Bind();
  // Allocate space for texture, don't upload any data.
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage3D(this->Target, 0,
      static_cast<GLint>(this->InternalFormat),
      static_cast<GLsizei>(this->Width),
      static_cast<GLsizei>(this->Height),
      static_cast<GLsizei>(this->Depth), 0,
      this->Format, this->Type, NULL);
  OpenGLCheckError("falied at glTexImage3D");
  this->Deactivate();
  return true;
#else
  return false;
#endif
}
// ----------------------------------------------------------------------------
void vtkTextureObject::CopyToFrameBuffer(
    vtkShaderProgram* program, vtkOpenGLVertexArrayObject* vao)
{
  // the following math really only works when texture
  // and viewport are of the same dimensions
  float minXTexCoord = static_cast<float>(
      static_cast<double>(0.5) / this->Width);
  float minYTexCoord = static_cast<float>(
      static_cast<double>(0.5) / this->Height);
  float maxXTexCoord = static_cast<float>(
      static_cast<double>(this->Width - 0.5) / this->Width);
  float maxYTexCoord = static_cast<float>(
      static_cast<double>(this->Height - 0.5) / this->Height);
  float tcoords[] = {
    minXTexCoord, minYTexCoord,
    maxXTexCoord, minYTexCoord,
    maxXTexCoord, maxYTexCoord,
    minXTexCoord, maxYTexCoord
  };
  float verts[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    -1.0f, 1.0f, 0.0f
  };
  this->CopyToFrameBuffer(tcoords, verts, program, vao);
}
// ----------------------------------------------------------------------------
void vtkTextureObject::CopyToFrameBuffer(
    int srcXmin, int srcYmin,
    int srcXmax, int srcYmax,
    int dstXmin, int dstYmin,
    int dstSizeX, int dstSizeY,
    vtkShaderProgram* program, vtkOpenGLVertexArrayObject* vao)
{
  float dstXmax = static_cast<float>(dstXmin + srcXmax - srcXmin);
  float dstYmax = static_cast<float>(dstYmin + srcYmax - srcYmin);
  this->CopyToFrameBuffer(srcXmin, srcYmin, srcXmax, srcYmax,
      dstXmin, dstYmin, dstXmax, dstYmax, dstSizeX, dstSizeY,
      program, vao);
}
// ----------------------------------------------------------------------------
void vtkTextureObject::CopyToFrameBuffer(
    int srcXmin, int srcYmin,
    int srcXmax, int srcYmax,
    int dstXmin, int dstYmin,
    int dstXmax, int dstYmax,
    int dstSizeX, int dstSizeY,
    vtkShaderProgram* program, vtkOpenGLVertexArrayObject* vao)
{
  assert("pre: positive_srcXmin" && srcXmin >= 0);
  assert("pre: max_srcXmax" &&
      static_cast<unsigned int>(srcXmax) < this->GetWidth());
  assert("pre: increasing_x" && srcXmin <= srcXmax);
  assert("pre: positive_srcYmin" && srcYmin >= 0);
  assert("pre: max_srcYmax" &&
      static_cast<unsigned int>(srcYmax) < this->GetHeight());
  assert("pre: increasing_y" && srcYmin <= srcYmax);
  assert("pre: positive_dstXmin" && dstXmin >= 0);
  assert("pre: positive_dstYmin" && dstYmin >= 0);
  float minXTexCoord = static_cast<float>(
      static_cast<double>(srcXmin + 0.5) / this->Width);
  float minYTexCoord = static_cast<float>(
      static_cast<double>(srcYmin + 0.5) / this->Height);
  float maxXTexCoord = static_cast<float>(
      static_cast<double>(srcXmax + 0.5) / this->Width);
  float maxYTexCoord = static_cast<float>(
      static_cast<double>(srcYmax + 0.5) / this->Height);
  GLint saved_viewport[4];
  glGetIntegerv(GL_VIEWPORT, saved_viewport);
  glViewport(0, 0, dstSizeX, dstSizeY);
  float tcoords[] = {
    minXTexCoord, minYTexCoord,
    maxXTexCoord, minYTexCoord,
    maxXTexCoord, maxYTexCoord,
    minXTexCoord, maxYTexCoord
  };
  float verts[] = {
    2.0f * dstXmin / dstSizeX - 1.0f, 2.0f * dstYmin / dstSizeY - 1.0f, 0.0f,
    2.0f * (dstXmax + 1.0f) / dstSizeX - 1.0f, 2.0f * dstYmin / dstSizeY - 1.0f, 0.0f,
    2.0f * (dstXmax + 1.0f) / dstSizeX - 1.0f, 2.0f * (dstYmax + 1.0f) / dstSizeY - 1.0f, 0.0f,
    2.0f * dstXmin / dstSizeX - 1.0f, 2.0f * (dstYmax + 1.0f) / dstSizeY - 1.0f, 0.0f
  };
  this->CopyToFrameBuffer(tcoords, verts, program, vao);
  glViewport(saved_viewport[0], saved_viewport[1], saved_viewport[2],
      saved_viewport[3]);
}
void vtkTextureObject::CopyToFrameBuffer(float* tcoords, float* verts,
    vtkShaderProgram* program, vtkOpenGLVertexArrayObject* vao)
{
  vtkOpenGLClearErrorMacro();
  // if no program or VAO was provided, then use
  // a simple pass through program and bind this
  // texture to it
  if (!program || !vao) {
    if (!this->ShaderProgram) {
      this->ShaderProgram = new vtkOpenGLHelper;
      // build the shader source code
      std::string VSSource = vtkTextureObjectVS;
      std::string FSSource = vtkTextureObjectFS;
      std::string GSSource;
      // compile and bind it if needed
      vtkShaderProgram* newShader =
          this->Context->GetShaderCache()->ReadyShaderProgram(
              VSSource.c_str(),
              FSSource.c_str(),
              GSSource.c_str());
      // if the shader changed reinitialize the VAO
      if (newShader != this->ShaderProgram->Program) {
        this->ShaderProgram->Program = newShader;
        this->ShaderProgram->VAO->ShaderProgramChanged(); // reset the VAO as the shader has changed
      }
      this->ShaderProgram->ShaderSourceTime.Modified();
    }
    else {
      this->Context->GetShaderCache()->ReadyShaderProgram(
          this->ShaderProgram->Program);
    }
    // bind and activate this texture
    this->Activate();
    int sourceId = this->GetTextureUnit();
    this->ShaderProgram->Program->SetUniformi("source", sourceId);
    vtkOpenGLRenderUtilities::RenderQuad(verts, tcoords, this->ShaderProgram->Program,
        this->ShaderProgram->VAO);
    this->Deactivate();
  }
  else {
    vtkOpenGLRenderUtilities::RenderQuad(verts, tcoords, program, vao);
  }
  OpenGLCheckError("failed after CopyToFrameBuffer")
}
//----------------------------------------------------------------------------
// Description:
// Copy a sub-part of a logical buffer of the framebuffer (color or depth)
// to the texture object. src is the framebuffer, dst is the texture.
// (srcXmin,srcYmin) is the location of the lower left corner of the
// rectangle in the framebuffer. (dstXmin,dstYmin) is the location of the
// lower left corner of the rectangle in the texture. width and height
// specifies the size of the rectangle in pixels.
// If the logical buffer is a color buffer, it has to be selected first with
// glReadBuffer().
// \pre is2D: GetNumberOfDimensions()==2
void vtkTextureObject::CopyFromFrameBuffer(int srcXmin,
    int srcYmin,
    int vtkNotUsed(dstXmin),
    int vtkNotUsed(dstYmin),
    int width,
    int height)
{
  assert("pre: is2D" && this->GetNumberOfDimensions() == 2);
  this->Activate();
  glCopyTexImage2D(this->Target, 0, this->Format, srcXmin, srcYmin, width, height, 0);
  OpenGLCheckError("failed at glCopyTexImage2D " << this->Format);
}
//----------------------------------------------------------------------------
int vtkTextureObject::GetMaximumTextureSize(vtkOpenGLRenderWindow* context)
{
  int maxSize = -1;
  if (context && context->IsCurrent()) {
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);
  }
  return maxSize;
}
//----------------------------------------------------------------------------
void vtkTextureObject::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "Width: " << this->Width << endl;
  os << indent << "Height: " << this->Height << endl;
  os << indent << "Depth: " << this->Depth << endl;
  os << indent << "Components: " << this->Components << endl;
  os << indent << "Handle: " << this->Handle << endl;
  os << indent << "Target: ";
  switch (this->Target) {
#ifdef GL_TEXTURE_1D
  case GL_TEXTURE_1D:
    os << "GL_TEXTURE_1D" << endl;
    break;
#endif
  case GL_TEXTURE_2D:
    os << "GL_TEXTURE_2D" << endl;
    break;
#ifdef GL_TEXTURE_3D
  case GL_TEXTURE_3D:
    os << "GL_TEXTURE_3D" << endl;
    break;
#endif
  default:
    os << "unknown value: 0x" << hex << this->Target << dec << endl;
    break;
  }
  os << indent << "NumberOfDimensions: " << this->NumberOfDimensions << endl;
  os << indent << "WrapS: " << WrapAsString[this->WrapS] << endl;
  os << indent << "WrapT: " << WrapAsString[this->WrapT] << endl;
  os << indent << "WrapR: " << WrapAsString[this->WrapR] << endl;
  os << indent << "MinificationFilter: "
      << MinMagFilterAsString[this->MinificationFilter] << endl;
  os << indent << "MagnificationFilter: "
      << MinMagFilterAsString[this->MagnificationFilter] << endl;
  os << indent << "MinLOD: " << this->MinLOD << endl;
  os << indent << "MaxLOD: " << this->MaxLOD << endl;
  os << indent << "BaseLevel: " << this->BaseLevel << endl;
  os << indent << "MaxLevel: " << this->MaxLevel << endl;
  os << indent << "DepthTextureCompare: " << this->DepthTextureCompare
      << endl;
  os << indent << "DepthTextureCompareFunction: "
      << DepthTextureCompareFunctionAsString[this->DepthTextureCompareFunction]
      << endl;
  os << indent << "GenerateMipmap: " << this->GenerateMipmap << endl;
}
#endif

