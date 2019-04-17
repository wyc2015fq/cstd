# OpenGL ES 3.0（九）实现美颜相机功能 - 我的学习笔记 - CSDN博客





2018年09月02日 00:07:23[zouzhiheng](https://me.csdn.net/u011330638)阅读数：454








# 着色器代码

虽然知道磨皮应该用双边滤波算法，但实际上我并不懂具体的代码编写过程，因此着色器代码用的是 GitHub 上找的一个项目：[WSLiveDemo](https://github.com/WangShuo1143368701/WSLiveDemo) ，顶点着色器如下：

```
#version 300 es

layout(location=0) in vec4 aPosition;
layout(location=1) in vec4 aTexCoord;

uniform mat4 mMatrix;

out vec2 vTexCoord;

void main() {
    vTexCoord = (mMatrix * aTexCoord).xy;
    gl_Position = aPosition;
}
```

片段着色器如下：

```
#version 300 es
#extension GL_OES_EGL_image_external_essl3 : require

precision highp float;

in highp vec2 vTexCoord;

uniform samplerExternalOES sTexture;

uniform highp vec2 singleStepOffset;
uniform highp vec4 params;
uniform highp float brightness;

const highp vec3 W = vec3(0.299, 0.587, 0.114);
const highp mat3 saturateMatrix = mat3(
        1.1102, -0.0598, -0.061,
        -0.0774, 1.0826, -0.1186,
        -0.0228, -0.0228, 1.1772);

highp vec2 blurCoordinates[24];

highp float hardLight(highp float color) {
    if (color <= 0.5)
        color = color * color * 2.0;
    else
        color = 1.0 - ((1.0 - color)*(1.0 - color) * 2.0);
    return color;
}

layout(location=0) out vec4 fragColor;

void main() {
    highp vec3 centralColor = texture(sTexture, vTexCoord).rgb;
    blurCoordinates[0] = vTexCoord.xy + singleStepOffset * vec2(0.0, -10.0);
    blurCoordinates[1] = vTexCoord.xy + singleStepOffset * vec2(0.0, 10.0);
    blurCoordinates[2] = vTexCoord.xy + singleStepOffset * vec2(-10.0, 0.0);
    blurCoordinates[3] = vTexCoord.xy + singleStepOffset * vec2(10.0, 0.0);
    blurCoordinates[4] = vTexCoord.xy + singleStepOffset * vec2(5.0, -8.0);
    blurCoordinates[5] = vTexCoord.xy + singleStepOffset * vec2(5.0, 8.0);
    blurCoordinates[6] = vTexCoord.xy + singleStepOffset * vec2(-5.0, 8.0);
    blurCoordinates[7] = vTexCoord.xy + singleStepOffset * vec2(-5.0, -8.0);
    blurCoordinates[8] = vTexCoord.xy + singleStepOffset * vec2(8.0, -5.0);
    blurCoordinates[9] = vTexCoord.xy + singleStepOffset * vec2(8.0, 5.0);
    blurCoordinates[10] = vTexCoord.xy + singleStepOffset * vec2(-8.0, 5.0);
    blurCoordinates[11] = vTexCoord.xy + singleStepOffset * vec2(-8.0, -5.0);
    blurCoordinates[12] = vTexCoord.xy + singleStepOffset * vec2(0.0, -6.0);
    blurCoordinates[13] = vTexCoord.xy + singleStepOffset * vec2(0.0, 6.0);
    blurCoordinates[14] = vTexCoord.xy + singleStepOffset * vec2(6.0, 0.0);
    blurCoordinates[15] = vTexCoord.xy + singleStepOffset * vec2(-6.0, 0.0);
    blurCoordinates[16] = vTexCoord.xy + singleStepOffset * vec2(-4.0, -4.0);
    blurCoordinates[17] = vTexCoord.xy + singleStepOffset * vec2(-4.0, 4.0);
    blurCoordinates[18] = vTexCoord.xy + singleStepOffset * vec2(4.0, -4.0);
    blurCoordinates[19] = vTexCoord.xy + singleStepOffset * vec2(4.0, 4.0);
    blurCoordinates[20] = vTexCoord.xy + singleStepOffset * vec2(-2.0, -2.0);
    blurCoordinates[21] = vTexCoord.xy + singleStepOffset * vec2(-2.0, 2.0);
    blurCoordinates[22] = vTexCoord.xy + singleStepOffset * vec2(2.0, -2.0);
    blurCoordinates[23] = vTexCoord.xy + singleStepOffset * vec2(2.0, 2.0);

    highp float sampleColor = centralColor.g * 22.0;
    sampleColor += texture(sTexture, blurCoordinates[0]).g;
    sampleColor += texture(sTexture, blurCoordinates[1]).g;
    sampleColor += texture(sTexture, blurCoordinates[2]).g;
    sampleColor += texture(sTexture, blurCoordinates[3]).g;
    sampleColor += texture(sTexture, blurCoordinates[4]).g;
    sampleColor += texture(sTexture, blurCoordinates[5]).g;
    sampleColor += texture(sTexture, blurCoordinates[6]).g;
    sampleColor += texture(sTexture, blurCoordinates[7]).g;
    sampleColor += texture(sTexture, blurCoordinates[8]).g;
    sampleColor += texture(sTexture, blurCoordinates[9]).g;
    sampleColor += texture(sTexture, blurCoordinates[10]).g;
    sampleColor += texture(sTexture, blurCoordinates[11]).g;
    sampleColor += texture(sTexture, blurCoordinates[12]).g * 2.0;
    sampleColor += texture(sTexture, blurCoordinates[13]).g * 2.0;
    sampleColor += texture(sTexture, blurCoordinates[14]).g * 2.0;
    sampleColor += texture(sTexture, blurCoordinates[15]).g * 2.0;
    sampleColor += texture(sTexture, blurCoordinates[16]).g * 2.0;
    sampleColor += texture(sTexture, blurCoordinates[17]).g * 2.0;
    sampleColor += texture(sTexture, blurCoordinates[18]).g * 2.0;
    sampleColor += texture(sTexture, blurCoordinates[19]).g * 2.0;
    sampleColor += texture(sTexture, blurCoordinates[20]).g * 3.0;
    sampleColor += texture(sTexture, blurCoordinates[21]).g * 3.0;
    sampleColor += texture(sTexture, blurCoordinates[22]).g * 3.0;
    sampleColor += texture(sTexture, blurCoordinates[23]).g * 3.0;

    sampleColor = sampleColor / 62.0;

    highp float highPass = centralColor.g - sampleColor + 0.5;

    for (int i = 0; i < 5; i++) {
        highPass = hardLight(highPass);
    }
    highp float lumance = dot(centralColor, W);

    highp float alpha = pow(lumance, params.r);

    highp vec3 smoothColor = centralColor + (centralColor-vec3(highPass))*alpha*0.1;

    smoothColor.r = clamp(pow(smoothColor.r, params.g), 0.0, 1.0);
    smoothColor.g = clamp(pow(smoothColor.g, params.g), 0.0, 1.0);
    smoothColor.b = clamp(pow(smoothColor.b, params.g), 0.0, 1.0);

    highp vec3 lvse = vec3(1.0)-(vec3(1.0)-smoothColor)*(vec3(1.0)-centralColor);
    highp vec3 bianliang = max(smoothColor, centralColor);
    highp vec3 rouguang = 2.0*centralColor*smoothColor + centralColor*centralColor - 2.0*centralColor*centralColor*smoothColor;

    fragColor = vec4(mix(centralColor, lvse, alpha), 1.0);
    fragColor.rgb = mix(fragColor.rgb, bianliang, alpha);
    fragColor.rgb = mix(fragColor.rgb, rouguang, params.b);

    highp vec3 satcolor = fragColor.rgb * saturateMatrix;
    fragColor.rgb = mix(fragColor.rgb, satcolor, params.a);
    fragColor.rgb = vec3(fragColor.rgb + vec3(brightness));
}
```

# OpenGL 代码

先创建顶点坐标等数据：

```cpp
const static GLfloat VERTICES[] = {
        -1.0f, 1.0f,
        1.0f, 1.0f,
        -1.0f, -1.0f,
        1.0f, -1.0f
};

const static GLfloat TEX_COORDS[] = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f
};

const static GLushort INDICES[] = {
        0, 1, 2,
        1, 2, 3
};

const static GLuint ATTRIB_POSITION = 0;
const static GLuint ATTRIB_TEX_COORD = 1;
const static GLuint VERTEX_POS_SIZE = 2;
const static GLuint INDEX_NUMBER = 6;
const static GLuint TEX_COORD_POS_SIZE = 2;
```

接着加载着色器、使用 VBO 、VAO 缓存顶点数据：

```cpp
int Beauty::init(AAssetManager *manager, ANativeWindow *window, int width, int height) {
    mWindow = window;
    resize(width, height);

    if (!mEGLCore->buildContext(window)) {
        LOGE("buildContext failed");
        return -1;
    }

    std::string *vShader = readShaderFromAsset(manager, "beauty.vert");
    std::string *fShader = readShaderFromAsset(manager, "beauty.frag");

    mProgram = loadProgram(vShader->c_str(), fShader->c_str());
    if (!mProgram) {
        LOGE("loadProgram failed!");
        return -1;
    }

    // 设置默认帧缓冲区纹理
    glGenTextures(1, &mTexOes);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, mTexOes);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, 0);

    initVbo();
    initVao();
    initPbo();

    mMatrixLoc = glGetUniformLocation(mProgram, "mMatrix");
    mTexLoc = glGetUniformLocation(mProgram, "sTexture");
    mParamsLoc = glGetUniformLocation(mProgram, "params");
    mBrightnessLoc = glGetUniformLocation(mProgram, "brightness");
    mSingleStepOffsetLoc = glGetUniformLocation(mProgram, "singleStepOffset");

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    delete vShader;
    delete fShader;

    return mTexOes;
}

void Beauty::initVbo() {
    // 缓存顶点坐标、纹理坐标、索引数据到缓冲区中
    glGenBuffers(3, mVboIds);
    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TEX_COORDS), TEX_COORDS, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIds[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES, GL_STATIC_DRAW);
}

void Beauty::initVao() {
    glGenVertexArrays(1, &mVao);
    // 使用缓冲区的数据设置顶点属性，并绑定至 vao
    glBindVertexArray(mVao);
    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[0]);
    glEnableVertexAttribArray(ATTRIB_POSITION);
    glVertexAttribPointer(ATTRIB_POSITION, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[1]);
    glEnableVertexAttribArray(ATTRIB_TEX_COORD);
    glVertexAttribPointer(ATTRIB_TEX_COORD, TEX_COORD_POS_SIZE, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIds[2]);

    glBindVertexArray(0);
}

void Beauty::initPbo() {
    // 生成 pbo
    glGenBuffers(2, mPboIds);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, mPboIds[0]);
    glBufferData(GL_PIXEL_PACK_BUFFER, mWidth * mHeight * 4, nullptr, GL_DYNAMIC_COPY);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, mPboIds[1]);
    glBufferData(GL_PIXEL_PACK_BUFFER, mWidth * mHeight * 4, nullptr, GL_DYNAMIC_COPY);
}
```

然后绘制即可：

```cpp
void
Beauty::draw(GLfloat *matrix, GLfloat beauty, GLfloat saturate, GLfloat bright, bool recording) {
    glViewport(0, 0, mWidth, mHeight);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(mProgram);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, mTexOes);
    glUniform1i(mTexLoc, 0);

    glUniformMatrix4fv(mMatrixLoc, 1, GL_FALSE, matrix);
    glUniform4fv(mParamsLoc, 1, getParams(beauty, saturate));
    glUniform1f(mBrightnessLoc, getBright(bright));
    glUniform2fv(mSingleStepOffsetLoc, 1, getSingleStepOffset(mWidth, mHeight));

    // 从 vao 中读取数据并渲染
    glBindVertexArray(mVao);
    glDrawElements(GL_TRIANGLES, INDEX_NUMBER, GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);

    // 下面这段代码块是配合 ffmpeg 使用的，用于将美颜后的图像发送到 ffmpeg 中以录制视频，可以忽略
    if (recording) {
        int rgbSize = mWidth * mHeight * 4;
        glReadBuffer(GL_COLOR_ATTACHMENT0);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, mPboIds[mPboReadIndex]);
        // 从默认帧缓冲区读取数据到 PBO 中，PBO 可以加快数据传输速度
        glReadPixels(0, 0, mWidth, mHeight, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        if (mPboMapIndex >= 0) {
            glBindBuffer(GL_PIXEL_PACK_BUFFER, mPboIds[mPboMapIndex]);
            uint8_t *mapData = (uint8_t *) glMapBufferRange(GL_PIXEL_PACK_BUFFER, 0, rgbSize,
                                                            GL_MAP_READ_BIT);
            uint8_t *rgb = new uint8_t[rgbSize];
            memcpy(rgb, mapData, (size_t) rgbSize);
            recordImage(rgb, rgbSize, mWidth, mHeight, PIXEL_FORMAT_ABGR); // ffmpeg 视频录制接口
            glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
        }
        mPboMapIndex = mPboReadIndex;
        mPboReadIndex = 1 - mPboReadIndex;
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    }

    glFlush();
    mEGLCore->swapBuffer();
}

GLfloat *Beauty::getParams(const GLfloat beauty, const GLfloat saturate) {
    GLfloat *value = new GLfloat[4];
    value[0] = 1.6f - 1.2f * beauty;
    value[1] = 1.3f - 0.6f * beauty;
    value[2] = -0.2f + 0.6f * saturate;
    value[3] = -0.2f + 0.6f * saturate;
    return value;
}

GLfloat Beauty::getBright(const GLfloat bright) {
    return 0.6f * (-0.5f + bright);
}

GLfloat *Beauty::getSingleStepOffset(const GLfloat width, const GLfloat height) {
    GLfloat *value = new GLfloat[2];
    value[0] = 2.0f / width;
    value[1] = 2.0f / height;
    return value;
}
```

# Java 代码

Java 代码很简单，只需要一个 SurfaceView，把 OpenGL 返回的纹理 ID 设置给 Camera 以开启预览即可，需要注意的是，更新预览图的代码需要和 OpenGL 运行在主线程之外的同一线程中：

```
private SurfaceTexture initOpenGL(Surface surface, int width, int height)
                throws ExecutionException, InterruptedException {
            Future<SurfaceTexture> future = mExecutor.submit(() -> {
                AssetManager manager = getContext().getAssets();
                int textureId = _init(surface, width, height, manager);
                if (textureId < 0) {
                    Log.e(TAG, "surfaceCreated init OpenGL ES failed!");
                    mIsBeautyOpen = false;
                    return null;
                }
                SurfaceTexture surfaceTexture = new SurfaceTexture(textureId);
                surfaceTexture.setOnFrameAvailableListener(surfaceTexture1 -> drawOpenGL());
                return surfaceTexture;
            });
            return future.get();
        }

        private void drawOpenGL() {
            mExecutor.execute(() -> {
                if (mSurfaceTexture != null) {
                    mSurfaceTexture.updateTexImage(); // 必须运行在 OpenGL 线程环境中
                    mSurfaceTexture.getTransformMatrix(mMatrix);
                    _draw(mMatrix, mBeautyLevel, mSaturateLevel, mBrightLevel, mIsRecording);
                }
            });
        }

        private void releaseOpenGL() {
            mExecutor.execute(() -> {
                if (mSurfaceTexture != null) {
                    mSurfaceTexture.release();
                    mSurfaceTexture = null;
                }
                _stop();
            });
        }
```

“_” 开头的是 native 方法，可以通过 native 接口设置磨皮、饱和度等参数：

```java
private static native int _init(Surface surface, int width, int height, AssetManager manager);

    private static native void _draw(float[] matrix, float beauty, float saturate, float bright,
                                     boolean recording);

    private static native void _stop();
```

源码已上传到 [GitHub](https://github.com/zouzhiheng/AVGraphics)。



