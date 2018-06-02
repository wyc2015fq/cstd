/***************************************************************************
 * ImageViewer for displaying hdr and panoramic images
 * Copyright (C) 2014 Michael Freundorfer
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program-> If not, see <http://www.gnu.org/licenses/>.
 **************************************************************************/
#include "glviewport.h"
#include "panohdrviewer.h"
#include "displayslot.h"
#include "image.h"
#include <QDebug>
#include <QMap>
#include <QtMath>
#include <QSettings>
#include <QOpenGLShaderProgram>
#include <QOffscreenSurface>
#include <QOpenGLFunctions_3_0>
#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
#define USE_QOPENGL 1
#endif
#if USE_QOPENGL
#include <QOpenGLWidget>
#define GLWidget QOpenGLWidget
#else
#include <QGLWidget>
#define GLWidget QGLWidget
#endif
struct GLFormat {
  GLint internalFormat;
  GLenum format;
  GLenum type;
};
enum ChannelState {
  ChannelOff = 0,
  ChannelOn = 1,
  ChannelExclusive = 2
};
class GLViewportWidget : public GLWidget, protected QOpenGLFunctions_3_0
{
public:
  const DisplaySlot* slot;
  bool interpolate;
  bool isInitialized;
  bool requiresTextureUpdate;
  GLuint imageTexture;
  GLuint vbo;
  GLuint vao;
  QByteArray fragmentSource;
  QMap<quint32, QOpenGLShaderProgram*> drawingProgramVariations;
  GLViewportWidget(QWidget* parent = 0);
  ~GLViewportWidget();
  void drawScreenquad();
  void drawImage();
  void updateTexture();
protected:
  void initializeGL();
  void resizeGL(int w, int h);
  void paintGL();
};
static int maxTextureSize = -1;
//static void checkGLError(QString location)
//{
// GLenum error = glGetError();
// if (error != GL_NO_ERROR) {
// qDebug() << "OpenGL Error:" << error << "at" << location;
// //qDebug() << (const char *)gluErrorString(error) << "at" << location;
// }
//}
static GLFormat glFormatFromImageFormat(ImageFormat format)
{
  GLFormat ret;
  switch (format) {
  case ImageFormat::RGB8:
    ret.internalFormat = GL_RGB8;
    ret.format = GL_BGR;
    ret.type = GL_UNSIGNED_BYTE;
    break;
  case ImageFormat::RGBA8:
    ret.internalFormat = GL_RGBA8;
    ret.format = GL_BGRA;
    ret.type = GL_UNSIGNED_BYTE;
    break;
  case ImageFormat::RGB16:
    ret.internalFormat = GL_RGB16;
    ret.format = GL_RGB;
    ret.type = GL_UNSIGNED_SHORT;
    break;
  case ImageFormat::RGBA16:
    ret.internalFormat = GL_RGBA16;
    ret.format = GL_RGBA;
    ret.type = GL_UNSIGNED_SHORT;
    break;
    //for the floatingpoint format use 16bit to save some memory on the graphics card
    //conversion time makes pretty much no difference compared to 32bit
  case ImageFormat::RGB32F:
    ret.internalFormat = GL_RGB16F;
    ret.format = GL_RGB;
    ret.type = GL_FLOAT;
    break;
  case ImageFormat::RGBA32F:
    ret.internalFormat = GL_RGBA16F;
    ret.format = GL_RGBA;
    ret.type = GL_FLOAT;
    break;
  default:
    ret.internalFormat = GL_INVALID_ENUM;
    ret.format = GL_INVALID_ENUM;
    ret.type = GL_INVALID_ENUM;
    break;
  }
  return ret;
}
GLViewport::GLViewport(QWidget* parent)
{
  m_widget = new GLViewportWidget(parent);
}
GLViewport::~GLViewport()
{
  delete m_widget;
}
void GLViewport::display(const DisplaySlot* displaySlot)
{
  m_widget->slot = displaySlot;
  m_widget->requiresTextureUpdate = true;
  m_widget->update();
}
void GLViewport::clear()
{
  m_widget->slot = nullptr;
  m_widget->requiresTextureUpdate = true;
  m_widget->update();
}
void GLViewport::redraw()
{
  m_widget->update();
}
QWidget* GLViewport::widget()
{
  return m_widget;
}
GLViewportWidget::GLViewportWidget(QWidget* parent) :
  GLWidget(parent),
  slot(nullptr),
  isInitialized(false),
  requiresTextureUpdate(false),
  imageTexture(0)
{
#if USE_QOPENGL
  QSurfaceFormat format;
  format.setSwapBehavior(QSurfaceFormat::SingleBuffer);
  setFormat(format);
#else
  QGLFormat format;// = QGLFormat(QGL::NoStencilBuffer | QGL::NoDepthBuffer | QGL::SingleBuffer);
  setFormat(format);
  context()->create();
#endif
  QFile fragmentFile(":/shaders/fragment.glsl");
  if (fragmentFile.open(QFile::ReadOnly | QFile::Text)) {
    fragmentSource = fragmentFile.readAll();
  }
  fragmentSource.append('\0');
}
GLViewportWidget::~GLViewportWidget()
{
  makeCurrent();
  if (imageTexture != 0) {
    glDeleteTextures(1, &imageTexture);
  }
  foreach(QOpenGLShaderProgram * program, drawingProgramVariations.values())
  delete program;
  doneCurrent();
}
void GLViewportWidget::initializeGL()
{
  if (!initializeOpenGLFunctions()) {
    qDebug() << "Failed to initialize OpenGL Functions";
    return;
  }
  qDebug() << "OpenGL Version:" << (const char*)glGetString(GL_VERSION);
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
  qDebug() << "Maximum Texture Size:" << maxTextureSize;
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glDisable(GL_BLEND);
  static const float vertexData[] = {
    // screenquad vertices
    -1.0f, 1.0f,
    1.0f, 1.0f,
    -1.0f, -1.0f,
    1.0f, -1.0f,
    // screeenquad uvs
    0.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    // image vertices
    0.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    // image uvs
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f
  };
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glGenVertexArrays(1, &vao);
  isInitialized = true;
  //checkGLError("init");
}
void GLViewportWidget::resizeGL(int w, int h)
{
  Q_UNUSED(w);
  Q_UNUSED(h);
}
void GLViewportWidget::paintGL()
{
  if (!isInitialized) {
    return;
  }
  updateTexture();
  QSettings settings;
  QColor backgroundColour = settings.value(BackgroundColourSetting).value<QColor>();
  glViewport(0, 0, width(), height());
  glClearColor(backgroundColour.redF(), backgroundColour.greenF(), backgroundColour.blueF(), 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  if (imageTexture && slot && slot->image) {
    bool useChecker = settings.value(UseCheckerSetting).toBool();
    int checkerSize = settings.value(CheckerSizeSetting).toInt();
    QColor checkerColour1 = settings.value(CheckerColour1Setting).value<QColor>();
    QColor checkerColour2 = settings.value(CheckerColour2Setting).value<QColor>();
    bool useProjection = slot->useProjection;
    SampleProjection sampleType = slot->sample;
    CameraProjection cameraType = slot->camera;
    float aspect = (float)width() / (float)height();
    float fov = slot->fov / 360.0 * 2.0 * M_PI;
    float yaw = slot->yaw;
    float pitch = slot->pitch;
    QTransform flipRotate = slot->flipRotate;
    bool isHDR = slot->image->isHDR;
    bool tonemap = isHDR && settings.value(UseTonemappingSetting).toBool();
    float tonemapArg1 = isHDR ? slot->tonemapArg1 : 1.0f;
    float tonemapArg2 = isHDR ? slot->tonemapArg2 : 1.0f;
    QSize textureSize = QSize(slot->image->width, slot->image->height);
    bool useInterpolation = settings.value(InterpolatePixelsSetting).toBool();
    int channelRed;
    int channelGreen;
    int channelBlue;
    int channelAlpha;
    if ((slot->channels & ChannelNoAlpha) == ChannelRed) {
      channelRed = ChannelExclusive;
    }
    else {
      channelRed = (slot->channels & ChannelRed) ? ChannelOn : ChannelOff;
    }
    if ((slot->channels & ChannelNoAlpha) == ChannelGreen) {
      channelGreen = ChannelExclusive;
    }
    else {
      channelGreen = (slot->channels & ChannelGreen) ? ChannelOn : ChannelOff;
    }
    if ((slot->channels & ChannelNoAlpha) == ChannelBlue) {
      channelBlue = ChannelExclusive;
    }
    else {
      channelBlue = (slot->channels & ChannelBlue) ? ChannelOn : ChannelOff;
    }
    if (slot->channels == ChannelAlpha) {
      channelAlpha = ChannelExclusive;
    }
    else {
      channelAlpha = (slot->channels & ChannelAlpha) ? ChannelOn : ChannelOff;
    }
    //Fisheye simply get's double the fov
    if (cameraType == CameraFisheye) {
      fov *= 2;
    }
    //identity matrix for projected, special view matrix for regular
    //getViewMatrix returns a QTransform, but having a 4x4 matrix simplifies the shader code
    QMatrix4x4 matrix;
    if (!useProjection) {
      QTransform toNDC;
      toNDC.translate(-1, 1)
      .scale(1.0 / width() * 2.0, -1.0 / height() * 2.0);
      matrix = QTransform().scale(textureSize.width(), textureSize.height()) * getViewMatrix(slot, textureSize, size()) * toNDC;
    }
    quint32 shaderVariant = useProjection ? ((quint32)sampleType << 16) | ((quint32)cameraType) : 0;
    QOpenGLShaderProgram* program;
    if (drawingProgramVariations.contains(shaderVariant)) {
      program = drawingProgramVariations[shaderVariant];
    }
    else {
      program = new QOpenGLShaderProgram(this);
      program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertex.glsl");
      QString preprocDirectives = "#version 130\n";
      if (useProjection) {
        preprocDirectives.append(
            QString("#define USE_PROJECTION 1\n"
                "#define CAMERA_TYPE %1\n"
                "#define SAMPLE_TYPE %2\n")
            .arg((int)cameraType)
            .arg((int)sampleType)
        );
      }
      program->addShaderFromSourceCode(QOpenGLShader::Fragment, preprocDirectives.toLatin1() + fragmentSource);
      program->link();
      drawingProgramVariations[shaderVariant] = program;
    }
    program->bind();
    program->setUniformValue("matrix", matrix);
    program->setUniformValue("texSampler", 0);
    if (useProjection) {
      if (cameraType == CameraRectilinear || cameraType == CameraFisheye) {
        program->setUniformValue("fov", fov);
        program->setUniformValue("aspectRatio", aspect);
      }
      program->setUniformValue("yaw", yaw);
      program->setUniformValue("pitch", pitch);
      program->setUniformValue("flipRotate", flipRotate);
    }
    program->setUniformValue("isHDR", isHDR);
    program->setUniformValue("tonemap", tonemap);
    program->setUniformValue("tonemapArg1", tonemapArg1);
    program->setUniformValue("tonemapArg2", tonemapArg2);
    program->setUniformValue("useInterpolation", useInterpolation);
    program->setUniformValue("textureSize", (float)textureSize.width(), (float)textureSize.height());
    program->setUniformValue("channelRed", channelRed);
    program->setUniformValue("channelGreen", channelGreen);
    program->setUniformValue("channelBlue", channelBlue);
    program->setUniformValue("channelAlpha", channelAlpha);
    program->setUniformValue("matrix", matrix);
    program->setUniformValue("useChecker", useChecker);
    program->setUniformValue("checkerSize", checkerSize);
    program->setUniformValue("backgroundColour",
        powf(backgroundColour.redF(), 2.2f),
        powf(backgroundColour.greenF(), 2.2f),
        powf(backgroundColour.blueF(), 2.2f),
        backgroundColour.alphaF());
    program->setUniformValue("checkerColour1",
        powf(checkerColour1.redF(), 2.2f),
        powf(checkerColour1.greenF(), 2.2f),
        powf(checkerColour1.blueF(), 2.2f),
        checkerColour1.alphaF());
    program->setUniformValue("checkerColour2",
        powf(checkerColour2.redF(), 2.2f),
        powf(checkerColour2.greenF(), 2.2f),
        powf(checkerColour2.blueF(), 2.2f),
        checkerColour2.alphaF());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, imageTexture);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    if (useProjection) {
      glVertexAttribPointer(program->attributeLocation("attr_position"), 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
      glVertexAttribPointer(program->attributeLocation("attr_texcoord"), 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(float) * 8));
    }
    else {
      glVertexAttribPointer(program->attributeLocation("attr_position"), 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(float) * 16));
      glVertexAttribPointer(program->attributeLocation("attr_texcoord"), 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(float) * 24));
    }
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
#ifndef USE_QOPENGL
    swapBuffers();
#endif
  }
  //checkGLError("draw");
}
void GLViewportWidget::updateTexture()
{
  if (requiresTextureUpdate) {
    requiresTextureUpdate = false;
    qDebug() << "Update Texture";
    if (!slot || !slot->image) {
      glDeleteTextures(1, &imageTexture);
      imageTexture = 0;
    }
    else {
      GLFormat format = glFormatFromImageFormat(slot->image->format);
      GLsizei width = slot->image->width;
      GLsizei height = slot->image->height;
      if (imageTexture == 0) {
        glGenTextures(1, &imageTexture);
      }
      glBindTexture(GL_TEXTURE_2D, imageTexture);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexImage2D(GL_TEXTURE_2D, 0, format.internalFormat, width, height, 0, format.format, format.type, slot->image->pixels);
      glBindTexture(GL_TEXTURE_2D, 0);
      //checkGLError("load");
    }
  }
}
bool hasRequiredOpenGLCapabilities()
{
  static bool once = true;
  static bool capable;
  if (once) {
    once = false;
    capable = true;
    QOffscreenSurface srf;
    QOpenGLContext ctx;
    srf.create();
    ctx.create();
    ctx.makeCurrent(&srf);
    QSurfaceFormat format = ctx.format();
    QStringList extensions;
    if (format.majorVersion() >= 3) {
      capable = true;
    }
    else {
      capable = false;
    }
#if 0
    QOpenGLFunctions_3_0* funcs = ctx.versionFunctions<QOpenGLFunctions_3_0>();
    if (funcs->initializeOpenGLFunctions()) {
      int numExtensions;
      glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
      extensions.reserve(numExtensions);
      for (int i = 0; i < numExtensions; i++) {
        extensions.append(QString((const char*)(funcs->glGetStringi(GL_EXTENSIONS, i))));
      }
    }
    else {
      extensions = QString((const char*)(glGetString(GL_EXTENSIONS))).split(" ");
    }
  }
  else {
    extensions = QString((const char*)(glGetString(GL_EXTENSIONS))).split(" ");
  }
  //check for multitexture
  if (format.majorVersion() == 1 && format.minorVersion() < 3 && !extensions.contains("GL_ARB_multitexture")) {
    capable = false;
  }
  //check for float textures
  if (!extensions.contains("GL_ARB_texture_float")) {
    capable = false;
  }
  //check for non power of two textures
  if (!extensions.contains("GL_ARB_texture_non_power_of_two")) {
    capable = false;
  }
#endif
  ctx.doneCurrent();
}
return capable;
}

