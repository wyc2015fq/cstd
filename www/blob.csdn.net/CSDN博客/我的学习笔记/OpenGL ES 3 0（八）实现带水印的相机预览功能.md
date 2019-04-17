# OpenGL ES 3.0（八）实现带水印的相机预览功能 - 我的学习笔记 - CSDN博客





2018年09月01日 23:34:40[zouzhiheng](https://me.csdn.net/u011330638)阅读数：188








# 着色器代码

这次准备在 Android 上实现一个带水印的相机预览功能，因此需要两个纹理，一个用于相机预览，一个用于显示水印，顶点着色器如下：

```
#version 300 es

layout(location=0) in vec4 aPosition;
layout(location=1) in vec4 aCameraTexCoord;
layout(location=2) in vec4 aWatermarkTexCoord;

uniform mat4 mCameraMatrix;
uniform mat4 mWatermarkMatrix;

out vec2 vCameraTexCoord;
out vec2 vWatermarkTexCoord;

void main() {
    vCameraTexCoord = (mCameraMatrix * aCameraTexCoord).xy;
    vWatermarkTexCoord = (mWatermarkMatrix * aWatermarkTexCoord).xy;
    gl_Position = aPosition;
}
```

片段着色器要注意两个纹理的叠加方式，通过 alpha 值判断即可：

```
#version 300 es
#extension GL_OES_EGL_image_external_essl3 : require

precision highp float;

uniform samplerExternalOES sCameraTexture;
uniform sampler2D sWatermarkTexture;

in vec2 vCameraTexCoord;
in vec2 vWatermarkTexCoord;

layout(location=0) out vec4 fragColor;

void main() {
    vec4 camera = texture(sCameraTexture, vCameraTexCoord);
    vec4 watermark = texture(sWatermarkTexture, vWatermarkTexCoord);
    // 水印之外的区域显示为相机预览图
    float r = watermark.r + (1.0 - watermark.a) * camera.r;
    float g = watermark.g + (1.0 - watermark.a) * camera.g;
    float b = watermark.b + (1.0 - watermark.a) * camera.b;
    fragColor = vec4(r, g, b, 1.0);
}
```

# OpenGL 代码

首先设置顶点坐标、纹理坐标等：

```cpp
const static GLfloat VERTICES[] = {
        -1.0f, -1.0f,
        -1.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, -1.0f
};

const static GLfloat CAMERA_COORDS[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
};

const static GLfloat WATERMARK_COORD[] = {
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
};

const static GLushort INDICES[] = {
        0, 1, 2,
        0, 2, 3
};

const static GLuint ATTRIB_POSITION = 0;
const static GLuint ATTRIB_CAMERA_COORD = 1;
const static GLuint ATTRIB_WATERMARK_COORD = 2;
const static GLuint VERTEX_POS_SIZE = 2;
const static GLuint CAMERA_COORD_SIZE = 2;
const static GLuint WATERMARK_COORD_SIZE = 2;
const static GLuint INDEX_NUMBER = 6;
```

接着初始化 OpenGL 环境，并返回 sCameraTexture 的 id 给 Java 层，以便打开 Camera 预览：

```cpp
int Watermark::init() {
    if (!mEGLCore->buildContext(mWindow)) {
        return -1;
    }

    std::string *vShader = readShaderFromAsset(mAssetManager, "watermark.vert");
    std::string *fShader = readShaderFromAsset(mAssetManager, "watermark.frag");
    mProgram = loadProgram(vShader->c_str(), fShader->c_str());

    glGenTextures(1, &mTextureOes);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, mTextureOes);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, 0);

    glGenTextures(1, &mTexture2D);
    glBindTexture(GL_TEXTURE_2D, mTexture2D);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWatermarkWidth, mWatermarkHeight, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, mWatermarkPixel);

    mCameraMatrixLoc = glGetUniformLocation(mProgram, "mCameraMatrix");
    mWatermarkMatrixLoc = glGetUniformLocation(mProgram, "mWatermarkMatrix");
    mCameraTextureLoc = glGetUniformLocation(mProgram, "sCameraTexture");
    mWatermarkTextureLoc = glGetUniformLocation(mProgram, "sWatermarkTexture");

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    delete vShader;
    delete fShader;

    return mTextureOes;
}
```

最后绘制即可：

```
void Watermark::draw(GLfloat *cameraMatrix, GLfloat *watermarkMatrix) {
    glViewport(0, 0, mWidth, mHeight);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(mProgram);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, mTextureOes);
    glUniform1i(mCameraTextureLoc, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mTexture2D);
    glUniform1i(mWatermarkTextureLoc, 1);

    glUniformMatrix4fv(mCameraMatrixLoc, 1, GL_FALSE, cameraMatrix);
    glUniformMatrix4fv(mWatermarkMatrixLoc, 1, GL_FALSE, watermarkMatrix);

    glEnableVertexAttribArray(ATTRIB_POSITION);
    glVertexAttribPointer(ATTRIB_POSITION, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, 0, VERTICES);

    glEnableVertexAttribArray(ATTRIB_CAMERA_COORD);
    glVertexAttribPointer(ATTRIB_CAMERA_COORD, CAMERA_COORD_SIZE, GL_FLOAT, GL_FALSE, 0,
                          CAMERA_COORDS);

    glEnableVertexAttribArray(ATTRIB_WATERMARK_COORD);
    glVertexAttribPointer(ATTRIB_WATERMARK_COORD, WATERMARK_COORD_SIZE, GL_FLOAT, GL_FALSE, 0,
                          WATERMARK_COORD);

//    glDrawArrays(GL_TRIANGLE_STRIP, 0, VERTEX_NUM);
    glDrawElements(GL_TRIANGLES, INDEX_NUMBER, GL_UNSIGNED_SHORT, INDICES);

    glDisableVertexAttribArray(ATTRIB_POSITION);
    glDisableVertexAttribArray(ATTRIB_CAMERA_COORD);
    glDisableVertexAttribArray(ATTRIB_WATERMARK_COORD);

    glFlush();
    mEGLCore->swapBuffer();
}
```

# Java 代码

Java 代码很简单，UI 只需要一个 SurfaceView，打开 Camera 后，把 OpenGL 返回回来的纹理 id 设置给 Camera 即可实现预览：

```
private void initOpenGL(Surface surface, int width, int height, byte[] watermark,
                                int watermarkWidth, int watermarkHeight) {
            mExecutor.execute(() -> {
                int textureId = _init(surface, width, height, watermark, watermark.length,
                        watermarkWidth, watermarkHeight, getAssets());
                if (textureId < 0) {
                    Log.e(TAG, "surfaceCreated init OpenGL ES failed!");
                    return;
                }
                mSurfaceTexture = new SurfaceTexture(textureId);
                mSurfaceTexture.setOnFrameAvailableListener(surfaceTexture -> drawOpenGL());
                try {
                    mCamera.setPreviewTexture(mSurfaceTexture);
                    mCamera.startPreview();
                } catch (IOException e) {
                    Log.e(TAG, "onSurfaceCreated exception: " + e.getLocalizedMessage());
                }
            });
        }

        private void drawOpenGL() {
            mExecutor.execute(() -> {
                if (mSurfaceTexture != null) {
                    mSurfaceTexture.updateTexImage(); // 必须运行在 OpenGL 线程环境中
                    mSurfaceTexture.getTransformMatrix(mCameraMatrix);
                    _draw(mCameraMatrix, mWatermarkMatrix);
                }
            });
        }

        private void releaseOpenGL() {
            mExecutor.execute(() -> {
                if (mSurfaceTexture != null) {
                    mSurfaceTexture.release();
                    mSurfaceTexture = null;
                }
                _release();
            });
        }
```

其中 “_” 开头的是 native 方法，至于水印，可以是图片：

```
private void makeImageWatermark() {
            BitmapFactory.Options options = new BitmapFactory.Options();
            options.inPreferredConfig = Bitmap.Config.ARGB_8888;
            Bitmap bitmap = BitmapFactory.decodeResource(getResources(), R.mipmap.ic_launcher, options);
            int byteCount = bitmap.getByteCount();
            ByteBuffer buffer = ByteBuffer.allocate(byteCount);
            bitmap.copyPixelsToBuffer(buffer);
            buffer.position(0);
            mWatermark = buffer.array();
            mWatermarkWidth = bitmap.getWidth();
            mWatermarkHeight = bitmap.getHeight();
            bitmap.recycle();

            Matrix.scaleM(mWatermarkMatrix, 0, 2.5f, 2.5f, 2.5f);
        }
```

也可以是文字：

```
private void makeTextWatermark(int width, int height) {
            Bitmap textBitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
            Canvas canvas = new Canvas(textBitmap);
            Paint paint = new Paint();
            paint.setColor(Color.argb(255, 255, 0, 0));
            paint.setTextSize(28);
            paint.setAntiAlias(true);
            paint.setTextAlign(Paint.Align.CENTER);
            Rect rect = new Rect(0, 0, width, height);
            Paint.FontMetricsInt fontMetrics = paint.getFontMetricsInt();
            // 将文字绘制在矩形区域的正中间
            int baseline = (rect.bottom + rect.top - fontMetrics.bottom - fontMetrics.top) / 2;
            canvas.drawText("作者: zouzhiheng", rect.centerX(), baseline, paint);

            int capacity = width * height * 4;
            ByteBuffer buffer = ByteBuffer.allocate(capacity);
            textBitmap.copyPixelsToBuffer(buffer);
            buffer.position(0);
            mWatermark = buffer.array();
            mWatermarkWidth = textBitmap.getWidth();
            mWatermarkHeight = textBitmap.getHeight();
            textBitmap.recycle();

            Matrix.scaleM(mWatermarkMatrix, 0, 2f, 2f, 2f);
        }
```

# 疑问

最后，还是有几个没搞明白的问题： 

1) CAMERA_COORDS、WATERMARK_COORD 的坐标向量方向是相反的，这样在手机上显示的时候，它们才不会倒转 

2) 制作水印时，调用了 Matrix.scaleM，想要缩小水印，却要传一个放大的值(2f) 

3) 如果调用 Matrix.rotateM，水印就会消失不见，无论是 90°，还是 180°
希望对这方面有了解的指导一下，源码已上传到 [GitHub](https://github.com/zouzhiheng/AVGraphics)。




