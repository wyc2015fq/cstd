# 在Android中用ndk层egl/opengl es显示java层打开的图像 - 建建的博客 - CSDN博客
2018年02月28日 18:11:22[纪建](https://me.csdn.net/u013898698)阅读数：189
本文介绍了如何在Android的java层打开一张图片，并在native层进行渲染的方法。可以分为两个部分： 
1. 在java层打开图片并通过jni接口将图像数据传给native层； 
2. 在ndk层通过egl和opengl es来渲染图像；
首先，我们建立一个空的工程，并在activity_main.xml文件中添加**SurfaceView**:
```xml
<SurfaceView android:id="@+id/SurfaceView01"
      android:layout_width="match_parent"
      android:layout_height="match_parent"/>
```
- 1
- 2
- 3
然后，在工程目录下创建jni目录，并添加如下文件： 
**Application.mk**
`APP_ABI := all`- 1
Android.mk
```
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE    := eglbitmapdemo
LOCAL_SRC_FILES := eglbitmapdemo-jni.cpp \
                   EGLEngine.cpp \
                   GLRender.cpp
LOCAL_LDLIBS    := -llog -lGLESv2 -lEGL -landroid -ljnigraphics
include $(BUILD_SHARED_LIBRARY)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
**eglbitmapdemo-jni.cpp**
```
#include <jni.h>
#include <android/bitmap.h>
#include "EGLEngine.h"
#include "JNILog.h"
EGLEngine *eglEngine = NULL;
#ifdef __cplusplus
extern "C" {
#endif
int
Java_com_example_eglbitmapdemo_MainActivity_setupGraphic( JNIEnv* env, jobject thiz, jobject jsurface)
{
    int ret = 0;
    ANativeWindow *nativeWindow = NULL;
    LOGI("Java_com_example_hellojni_HelloJni_setupGraphic enter");
    nativeWindow = ANativeWindow_fromSurface(env, jsurface);
    if (eglEngine) {
        return 0;
    }
    eglEngine = new EGLEngine();
    if (!eglEngine) {
        goto failed;
    }
    ret = eglEngine->init(nativeWindow);
    if (ret) {
        goto failed;
    }
    return 0;
failed:
    if (eglEngine)
        delete eglEngine;
    return -1;
}
int
Java_com_example_eglbitmapdemo_MainActivity_showBitmap(JNIEnv* env, jobject thiz, jobject jbitmap)
{
    int ret = 0;
    EGLEngine *egl = NULL;
    AndroidBitmapInfo bitmapInfo;
    void *pixels = NULL;
    int imgWidth = 2;
    int imgHeight = 2;
    LOGI("Java_com_example_hellojni_HelloJni_showBitmap enter");
    if ((ret = AndroidBitmap_getInfo(env, jbitmap, &bitmapInfo)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return -1;
    }
    LOGI("Java_com_example_hellojni_HelloJni_showBitmap width %d, height %d, format %d",
            bitmapInfo.width, bitmapInfo.height, bitmapInfo.format);
    imgWidth = bitmapInfo.width;
    imgHeight = bitmapInfo.height;
    if (bitmapInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Java_com_example_hellojni_HelloJni_showBitmap invalid rgb format");
        return -1;
    }
    if ((ret = AndroidBitmap_lockPixels(env, jbitmap, &pixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }
    eglEngine->loadImage(pixels, imgWidth, imgHeight);
    eglEngine->draw();
    AndroidBitmap_unlockPixels(env, jbitmap);
    return 0;
}
#ifdef __cplusplus
}
#endif
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
**EGLEngine.cpp**
```
/*
 * EGLEngine.cpp
 *
 *  Created on: 2015年10月3日
 *      Author: leon
 */
#include "EGLEngine.h"
EGLEngine::EGLEngine() {
    mWin = NULL;
}
EGLEngine::~EGLEngine() {
}
int EGLEngine::init(ANativeWindow * surface) {
    if (!surface) {
        return -1;
    }
    mWin = surface;
    GLint majorVersion;
    GLint minorVersion;
    mEglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if(EGL_NO_DISPLAY == mEglDisplay) {
        return -1;
    }
    if(!eglInitialize(mEglDisplay, &majorVersion, &minorVersion)) {
        return -1;
    }
    EGLint config_attribs[] = {
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_NONE
    };
    int num_configs = 0;
    EGLConfig   eglConfig;
    if(!eglChooseConfig(mEglDisplay, config_attribs, &eglConfig, 1, &num_configs))
    {
        return -1;
    }
    mEglSurface = eglCreateWindowSurface(mEglDisplay, eglConfig, mWin, NULL);
    if(EGL_NO_SURFACE == mEglSurface) {
        return -1;
    }
    if(!eglQuerySurface(mEglDisplay, mEglSurface, EGL_WIDTH, &mSurfaceWidth) ||
       !eglQuerySurface(mEglDisplay, mEglSurface, EGL_HEIGHT, &mSurfaceHeight)) {
        return -1;
    }
    EGLint context_attrib[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };
    mEglContext = eglCreateContext(mEglDisplay, eglConfig, EGL_NO_CONTEXT, context_attrib);
    if(EGL_NO_CONTEXT == mEglContext) {
        return -1;
    }
    if(!eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface, mEglContext)) {
        return -1;
    }
    glViewport(0, 0, mSurfaceWidth, mSurfaceHeight);
    glClearColor(1.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_CULL_FACE);
    LOGI("EGLEngine init success: surface width %d, surface height %d", mSurfaceWidth, mSurfaceHeight);
    imageRender = new GLRender();
    return 0;
}
void EGLEngine::release() {
    if (mEglDisplay != EGL_NO_DISPLAY) {
        eglMakeCurrent(mEglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (mEglContext != EGL_NO_CONTEXT) {
            eglDestroyContext(mEglDisplay, mEglContext);
        }
        if (mEglSurface != EGL_NO_SURFACE) {
            eglDestroySurface(mEglDisplay, mEglSurface);
        }
        if (!eglTerminate(mEglDisplay)) {
        }
    }
    mEglDisplay = EGL_NO_DISPLAY;
    mEglContext = EGL_NO_CONTEXT;
    mEglSurface = EGL_NO_SURFACE;
}
int EGLEngine::loadImage(void *pixels, int width, int height) {
    int ret = 0;
    LOGI("EGLEngine::loadImage width %d, height %d", width, height);
    ret = imageRender->setup(pixels, width, height);
    if (ret) {
        LOGE("EGLEngine::loadImage failed");
        return -1;
    }
    LOGI("EGLEngine::loadImage done");
    return 0;
}
int EGLEngine::draw() {
    int ret = 0;
    ret = imageRender->draw();
    if (ret) {
        LOGE("EGLEngine::draw failed");
        return -1;
    }
    eglSwapBuffers(mEglDisplay, mEglSurface);
    LOGI("EGLEngine::draw done");
    return 0;
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
- 102
- 103
- 104
- 105
- 106
- 107
- 108
- 109
- 110
- 111
- 112
- 113
- 114
- 115
- 116
- 117
- 118
- 119
- 120
- 121
- 122
- 123
- 124
- 125
- 126
- 127
- 128
- 129
- 130
- 131
- 132
- 133
- 134
- 135
- 136
- 137
- 138
- 139
- 140
**GLRender.cpp**
```cpp
/*
 * GLRender.cpp
 *
 *  Created on: 2015年10月4日
 *      Author: leon
 */
#include "GLRender.h"
GLRender::GLRender():
mIndices(NULL),
mVertex(NULL),
mVertexSize(0),
mIndicesSize(0){
    mVertexShader = STRINGIZE(
        attribute vec4 a_position;
        attribute vec2 a_tex_coord_in;
        varying vec2 v_tex_coord_out;
        void main(void) {
            v_tex_coord_out = a_tex_coord_in;
            gl_Position = a_position;
        }
    );
    mFragmentShader = STRINGIZE(
        precision highp float;
        varying highp vec2 v_tex_coord_out;
        uniform sampler2D s_texture;
        void main() {
            vec4 mzs = texture2D(s_texture, v_tex_coord_out);
            gl_FragColor = vec4(mzs.rgb, 0.0);
        }
    );
}
GLRender::~GLRender() {
    if (mGlVShader) {
        glDetachShader(mGlProgram, mGlVShader);
        glDeleteShader(mGlVShader);
        mGlVShader = 0;
    }
    if (mGlFShader) {
        glDetachShader(mGlProgram, mGlFShader);
        glDeleteShader(mGlFShader);
        mGlFShader = 0;
    }
    if (mGlProgram) {
        glDeleteProgram(mGlProgram);
        mGlProgram = 0;
    }
}
int GLRender::setup(void *pixels, int width, int height) {
    LOGI("GLRender::setup enter, pixels %x, width %d, height %d", pixels, width, height);
    mGlProgram = createProgram(mVertexShader, mFragmentShader);
    if (!mGlProgram) {
        LOGE("GLRender::setup createProgram failed");
        return -1;
    }
    glUseProgram(mGlProgram);
    if (setupVertex()) {
        LOGE("GLRender::setup setupVertex failed");
        return -1;
    }
    if (setupTexture()) {
        LOGE("GLRender::setup setupTexture failed");
        return -1;
    }
    if (loadImage((uint8_t *)pixels, width, height)) {
        LOGE("GLRender::setup loadImage failed");
        return -1;
    }
    LOGI("GLRender::setup done");
    return 0;
}
GLuint GLRender::createProgram(const char *vshaderSrc, const char *fshaderSrc) {
    GLuint programObject;
    GLint  linked;
    mGlVShader = loadShader(GL_VERTEX_SHADER, vshaderSrc);
    if (!mGlVShader) {
        LOGE("vertex shader initialize wrong. ");
        return (0);
    }
    mGlFShader = loadShader(GL_FRAGMENT_SHADER, fshaderSrc);
    if (!mGlFShader)
    {
        LOGE("fragment shader initialize wrong. ");
        return (0);
    }
    //Create the program object
    programObject = glCreateProgram();
    if (programObject == 0)
        return (0);
    glAttachShader(programObject, mGlVShader);
    glAttachShader(programObject, mGlFShader);
    //Link the program
    glLinkProgram(programObject);
    //Check the link status
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
    if(!linked)
    {
        GLint infoLen;
        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);
        LOGI("Error linking length: %d", infoLen);
        if(infoLen > 1)
        {
            char* infoLog = (char*)malloc(infoLen);
            glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
            LOGE("Error linking program: %s, %s", infoLog, glGetString(glGetError()));
            free(infoLog);
        }
        glDeleteProgram(programObject);
        return (0);
    }
    //Store the program object
    return (programObject);
}
GLuint GLRender::loadShader(GLenum type, const char *shaderSrc) {
    GLuint shader;
    shader = glCreateShader(type);
    if (shader) {
        glShaderSource(shader,1, &shaderSrc, NULL);
        glCompileShader(shader);
        GLint compile;
        compile = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compile);
        if (!compile) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if(infoLen > 1)
            {
                char* infoLog = (char*)malloc(infoLen);
                glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
                LOGI( "Error compiling shader: %s", infoLog);
                free(infoLog);
            }
            glDeleteShader(shader);
            return (0);
        }
    } else
    {
        return (0);
    }
    return (shader);
}
int GLRender::setupVertex() {
    Vertex vertices[4];
    GLubyte indices[] = {0, 1, 2, 2, 3, 0};
    GLuint a_position;
    GLuint a_tex_coord_in;
    mVertex = (Vertex *)calloc(sizeof(Vertex)*4, 1);
    if (!mVertex) {
        goto failed;
    }
    mIndices = (GLubyte *)calloc(sizeof(GLubyte)*6, 1);
    if (!mIndices) {
        goto failed;
    }
    vertices[0] = { {1.0f, -1.0f, 0.0f}, {1 ,1}};
    vertices[1] = { {1.0f, 1.0f, 0.0f}, {1,0}};
    vertices[2] = { {-1.0f, 1.0f, 0.0f}, {0,0}};
    vertices[3] = { {-1.0f, -1.0f, 0.0f}, {0,1}};
    memcpy(mVertex, vertices, sizeof(vertices));
    memcpy(mIndices, indices, sizeof(indices));
    mVertexSize = sizeof(vertices);
    mIndicesSize = sizeof(indices);
    glGenBuffers(1, &mVertexBuf);
    glGenBuffers(1, &mIndicesBuf);
    a_position = glGetAttribLocation(mGlProgram, "a_position");
    a_tex_coord_in = glGetAttribLocation(mGlProgram, "a_tex_coord_in");
    glEnableVertexAttribArray(a_position);
    glEnableVertexAttribArray(a_tex_coord_in);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuf);
    glBufferData(GL_ARRAY_BUFFER, mVertexSize, mVertex, GL_STATIC_DRAW);
    glVertexAttribPointer(a_position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(a_tex_coord_in, 2, GL_FLOAT, GL_TRUE, sizeof(Vertex),
            (GLvoid*) (sizeof(float) * 3));
    return 0;
failed:
    if (mVertex) {
        free(mVertex);
        mVertex = NULL;
    }
    if (mIndices) {
        free(mIndices);
        mIndices = NULL;
    }
    return -1;
}
int GLRender::setupTexture() {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &mTextureId);
    glBindTexture(GL_TEXTURE_2D, mTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    return 0;
}
int GLRender::loadImage(uint8_t *pixels, int width, int height) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    GLuint location = glGetUniformLocation(mGlProgram, "s_texture");
    glUniform1i(location, 0);
    return 0;
}
int GLRender::draw() {
    glUseProgram(mGlProgram);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    return 0;
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
- 102
- 103
- 104
- 105
- 106
- 107
- 108
- 109
- 110
- 111
- 112
- 113
- 114
- 115
- 116
- 117
- 118
- 119
- 120
- 121
- 122
- 123
- 124
- 125
- 126
- 127
- 128
- 129
- 130
- 131
- 132
- 133
- 134
- 135
- 136
- 137
- 138
- 139
- 140
- 141
- 142
- 143
- 144
- 145
- 146
- 147
- 148
- 149
- 150
- 151
- 152
- 153
- 154
- 155
- 156
- 157
- 158
- 159
- 160
- 161
- 162
- 163
- 164
- 165
- 166
- 167
- 168
- 169
- 170
- 171
- 172
- 173
- 174
- 175
- 176
- 177
- 178
- 179
- 180
- 181
- 182
- 183
- 184
- 185
- 186
- 187
- 188
- 189
- 190
- 191
- 192
- 193
- 194
- 195
- 196
- 197
- 198
- 199
- 200
- 201
- 202
- 203
- 204
- 205
- 206
- 207
- 208
- 209
- 210
- 211
- 212
- 213
- 214
- 215
- 216
- 217
- 218
- 219
- 220
- 221
- 222
- 223
- 224
- 225
- 226
- 227
- 228
- 229
- 230
- 231
- 232
- 233
- 234
- 235
- 236
- 237
- 238
- 239
- 240
- 241
- 242
- 243
- 244
- 245
- 246
- 247
- 248
- 249
- 250
- 251
- 252
- 253
- 254
- 255
- 256
- 257
- 258
- 259
- 260
- 261
- 262
- 263
用ndk-build编译出jni动态库文件。
将要显示的图像拷贝到assets目录下。
编辑MainActivity.java代码如下：
```java
package com.example.eglbitmapdemo;
import java.io.IOException;
import java.io.InputStream;
import android.app.Activity;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
public class MainActivity extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        SurfaceView surfaceView = (SurfaceView) findViewById(R.id.SurfaceView01);
        SurfaceHolder surfaceHolder = surfaceView.getHolder();
        surfaceHolder.addCallback(new SurfaceHolder.Callback() {
            public void surfaceChanged(SurfaceHolder holder, int format,
                    int width, int height) {
            }
            public void surfaceCreated(SurfaceHolder holder) {
                AssetManager manager = getAssets();
                InputStream open;
                Bitmap bitmap;
                try {
                    open = manager.open("dog.jpg");
                    bitmap = BitmapFactory.decodeStream(open);
                    setupGraphic(holder.getSurface());                  
                    showBitmap(bitmap);
                } catch (IOException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                }                           
            }
            public void surfaceDestroyed(SurfaceHolder holder) {
            }
        });
    }
    public native int setupGraphic(Surface surface);    
    public native int showBitmap(Bitmap bitmap);
    static {
        System.loadLibrary("eglbitmapdemo");
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
至此，我们的主要代码就结束了。 
它的工作原理是，通过java代码生成图像的Bitmap对象，通过jni接口将SurfaceView对象和Bitmap对象传给native代码，native代码从传入的Bitmap对象中获取rgb数据，并通过egl和opengl es进行渲染。
