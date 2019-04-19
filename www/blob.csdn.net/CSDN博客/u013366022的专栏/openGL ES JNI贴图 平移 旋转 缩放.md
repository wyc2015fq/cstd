# openGL ES JNI贴图 平移 旋转 缩放 - u013366022的专栏 - CSDN博客
2015年07月07日 20:05:35[slitaz](https://me.csdn.net/u013366022)阅读数：2122
采用demo为NDK sample下的hello-gl2。 
1、修改gVertexShader
```
static const char gVertexShader[] =
    "uniform mat4 uMVPMatrix;   \n"  //变换矩阵
    "attribute vec2 a_position;   \n"
          "attribute vec2 a_texCoord;   \n"
          "varying vec2 v_texCoord;     \n"
          "void main()                  \n"
          "{                            \n"
          "   gl_Position uMVPMatrix*vec4(a_position,0,1); \n"
          "   v_texCoord = a_texCoord;  \n"
          "}                            \n";
```
修改gFragmentShader
```
static const char gFragmentShader[] = 
    "precision mediump float;                            \n"
          "varying vec2 v_texCoord;                            \n"
          "uniform sampler2D s_texture;                        \n"
          "void main()                                         \n"
          "{                                                   \n"
          "  gl_FragColor = texture2D( s_texture, v_texCoord );\n"
          "}                                                   \n";
```
在函数setupGraphics里添加纹理信息
```
bool setupGraphics(int w, int h) {
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);
    LOGI("setupGraphics(%d, %d)", w, h);
    gProgram = createProgram(gVertexShader, gFragmentShader);
    if (!gProgram) {
        LOGE("Could not create program.");
        return false;
    }
    glViewport(0, 0, w, h);
    checkGlError("glViewport");
    gvPositionHandle = glGetAttribLocation(gProgram,"a_position");
    gvTexCoordHandle =  glGetAttribLocation(gProgram,"a_texCoord");
    gvSTexCoordHandle =  glGetUniformLocation(gProgram,"s_texture");
    muMVPMatrixHandle =  glGetUniformLocation(gProgram, "uMVPMatrix");
    glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );
    glGenTextures ( 1, &textureId );
    glBindTexture ( GL_TEXTURE_2D, textureId );
    glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataFromBmp );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,  GL_LINEAR );
    return true;
}
```
4、设置renderFrame
```cpp
void renderFrame() {
    static float grey;
    grey = 0.1f;
    if (grey > 1.0f) {
        grey = 0.0f;
    }
    glClearColor(grey, grey, grey, 1.0f);
    checkGlError("glClearColor");
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");
    glUseProgram(gProgram);
    checkGlError("glUseProgram");
    glVertexAttribPointer(gvPositionHandle,2, GL_FLOAT, GL_FALSE, 0,gTriangleVertices );
    glVertexAttribPointer(gvTexCoordHandle,2, GL_FLOAT, GL_FALSE, 0, gTexCoords);
    glUniformMatrix4fv(muMVPMatrixHandle, 1, GL_FALSE, mat4);
    glEnableVertexAttribArray(gvPositionHandle);
    glEnableVertexAttribArray(gvTexCoordHandle);
    glActiveTexture ( GL_TEXTURE0 );
    glBindTexture ( GL_TEXTURE_2D, textureId );
    glUniform1i ( gvSTexCoordHandle, 0 );
   // glUniform1i(muMVPMatrixHandle,0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
```
5、在GL2JNILib.java里声明NATIVE函数
```java
public class GL2JNILib {
     static {
         System.loadLibrary("gl2jni");
     }
    /**
     * @param width the current view width
     * @param height the current view height
     */
     public static native void init(int width, int height);
     public static native void step();
     public native static void  processBitmap(Bitmap bitmap);
     public native static void rotate(float mat[]);
}
```
6、添加相关控件 
7添加事件 
在GL2JNIActivity里声明如下变量
```
GL2JNIView mView;
    static float[] mMMatrix = new float[16];
    static float[] moveMatrix = new float[16];//随便写的，就临时数组。
    static float xAngle =0;//角度
```
这儿不能直接调用JNI，需要用GL线程来运行。故 
需要调用
```java
mView.queueEvent(new Runnable() {
            @Override
            public void run() {
                GL2JNILib.rotate(mMMatrix);//JNI接口，最终的变换矩阵
            }
        });
```
```java
public void moveRight(View v )
    {
        Matrix.setIdentityM(moveMatrix, 0);
        Matrix.translateM(moveMatrix, 0, 0.1f, 0, 0);
        Matrix.multiplyMM(mMMatrix,0,moveMatrix,0,mMMatrix,0);
        button1.setText("向右");
        mView.queueEvent(new Runnable() {
            @Override
            public void run() {
                GL2JNILib.rotate(mMMatrix);
            }
        });
    }  
    public void moveLeft(View v )
    {
        Matrix.setIdentityM(moveMatrix, 0);
        Matrix.translateM(moveMatrix, 0, -0.1f, 0, 0);
        Matrix.multiplyMM(mMMatrix,0,moveMatrix,0,mMMatrix,0);
        button4.setText("向左");
        mView.queueEvent(new Runnable() {
            @Override
            public void run() {
                GL2JNILib.rotate(mMMatrix);
            }
        });
    } 
    public void zoomOut(View v )
    {
        Matrix.setIdentityM(moveMatrix, 0);
        Matrix.scaleM(moveMatrix,0,1.1f,1.1f,1.1f);
        Matrix.multiplyMM(mMMatrix,0,moveMatrix,0,mMMatrix,0);
        mView.queueEvent(new Runnable() {
            @Override
            public void run() {
                GL2JNILib.rotate(mMMatrix);
            }
        });
        button2.setText("放大");
    }  
    public void zoomIn(View v )
    {
        Matrix.setIdentityM(moveMatrix, 0);
        Matrix.scaleM(moveMatrix, 0, 0.9f, 0.9f, 0.9f);
        Matrix.multiplyMM(mMMatrix,0,moveMatrix,0,mMMatrix,0);
        mView.queueEvent(new Runnable() {
            @Override
            public void run() {
                GL2JNILib.rotate(mMMatrix);
            }
        });
    }
```
大体如此了。
补充： 
需要 填充图片时，我没在JNI上找到现成.cpp 和.h文件来处理图片。所以 采用的是jni传入bitmap的方式。
```
void  Java_com_android_gl2jni_GL2JNILib_processBitmap(JNIEnv *env, jclass obj, jobject bmpObj)
{
    //return ;
    LOGE("-----------------------");
    int ret;
    AndroidBitmapInfo bmpInfo={0};
    if(ret = AndroidBitmap_getInfo(env,bmpObj,&bmpInfo)<0)
    {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return ;
    }
    width = bmpInfo.width;
    height = bmpInfo.height;
    if(ret = AndroidBitmap_lockPixels(env,bmpObj,(void**)&dataFromBmp))
    {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
        return;
    }
    AndroidBitmap_unlockPixels(env, bmpObj);
    return ;
}
```
