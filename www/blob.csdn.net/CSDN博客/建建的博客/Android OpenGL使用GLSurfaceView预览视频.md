# Android OpenGL使用GLSurfaceView预览视频 - 建建的博客 - CSDN博客
2018年03月03日 16:40:37[纪建](https://me.csdn.net/u013898698)阅读数：208
# 前言
一年之前做过一些即时通信视频相关的工作，主要是做视频渲染这一部分的工作，由于2016毕业来到了华为，华为对研究生的安排就是“哪里需要去哪里”，和你专业和擅长的没有太大的关系，所以一直在适应当下的工作，现在基本上可以胜任现在的工作，可以抽出一些时间来总结一下之前了解过的OpenGL相关知识。
# 第一章 相关知识介绍
在介绍具体的功能之前，先对一些主要的类和方法进行一些介绍，这样可以更好的理解整个程序
## 1.1 GLSurfaceView
在谷歌的官方文档中是这样解释GLSurfaceView的：
> 
An implementation of SurfaceView that uses the dedicated surface for displaying OpenGL rendering.
大意是GLSurfaceView是一个继承了SurfaceView类，它是专门用来显示OpenGL的渲染。通俗的来说，GLSurfaceView可以用来显示视频、图像和3D模型等视图，在接下来的章节中主要是使用它来显示Camera视频数据，大家可能会有一些问题，SurfaceView也可用来预览Camera，那么这两者有什么区别吗？GLSurfaceView能够真正做到让Camera的数据和显示分离，我们就可以在此基础上对视频数据做一些处理，例如美图，增加特效等。
## 1.2 GLSurfaceView.Renderer
如果说GLSurfaceView是画布，那么仅仅有一张白纸是没用的，我们还需要一支画笔，Renderer的功能就是这里说的画笔。Renderer是一个接口，主要包含3个抽象的函数：`onSurfaceCreated`、`onDrawFrame`、`onSurfaceChanged`，从名字就可以看出，分别是在SurfaceView创建、视图大小发生改变和绘制图形时调用。
## 1.3 Camera
从Android 5.0开始（API Level 21），可以完全控制安卓设别相机的新API `Camera2(android.hardware.Camera2)`被引进来了。虽然新的Camera2不管在功能上还是友好度上都强于旧的Camera，但是我们这里还是使用的旧的Camera，由于新的Camera2暂时还没有找到可以获取视频帧的接口，因为后面肯能会对Canmera视频帧做一些处理，所以这里暂时还是使用旧的Camera。
# 第二章 开始绘制
## 2.1 CameraGLSurfaceView
```java
public class CameraGLSurfaceView extends GLSurfaceView implements Renderer, SurfaceTexture.OnFrameAvailableListener {
    private Context mContext;
    private SurfaceTexture mSurface;
    private int mTextureID = -1;
    private DirectDrawer mDirectDrawer;
    public CameraGLSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        mContext = context;
        // 设置OpenGl ES的版本为2.0
        setEGLContextClientVersion(2);
        // 设置与当前GLSurfaceView绑定的Renderer
        setRenderer(this);
        // 设置渲染的模式
        setRenderMode(RENDERMODE_WHEN_DIRTY);
    }
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        // TODO Auto-generated method stub
        LOG.logI("onSurfaceCreated...");
        mTextureID = GlUtil.createTextureID();
        mSurface = new SurfaceTexture(mTextureID);
        mSurface.setOnFrameAvailableListener(this);
        mDirectDrawer = new DirectDrawer(mTextureID);
        CameraCapture.get().openBackCamera();
    }
    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        // TODO Auto-generated method stub
        LOG.logI("onSurfaceChanged...");
        // 设置OpenGL场景的大小,(0,0)表示窗口内部视口的左下角，(w,h)指定了视口的大小
        GLES20.glViewport(0, 0, width, height);
        if (!CameraCapture.get().isPreviewing()) {
            CameraCapture.get().doStartPreview(mSurface);
        }
    }
    @Override
    public void onDrawFrame(GL10 gl) {
        // TODO Auto-generated method stub
        LOG.logI("onDrawFrame...");
        // 设置白色为清屏
        GLES20.glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        // 清除屏幕和深度缓存
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);
        // 更新纹理
        mSurface.updateTexImage();
        mDirectDrawer.draw();
    }
    @Override
    public void onPause() {
        // TODO Auto-generated method stub
        super.onPause();
        CameraCapture.get().doStopCamera();
    }
    @Override
    public void onFrameAvailable(SurfaceTexture surfaceTexture) {
        // TODO Auto-generated method stub
        LOG.logI("onFrameAvailable...");
        this.requestRender();
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
这个类主要做了以下几件事情：
- 实现Renderer这个接口，并且实现GLSurfaceView的初始化。在`CameraGLSurfaceView`的构造函数中设置了GLSurfaceView的版本：`setEGLContextClientVersion(2)`，如果没有这个设置，GLSurfaceView是什么也绘制不出来的，因为Android支持OpenGL ES1.1、2.0以及3.+等版本，而且版本间的差别很大，不声明版本号，GLSurfaceView是不知道使用哪个版本进行渲染；设置Renderer与当前的View绑定，然后再设置渲染的模式为`RENDERMODE_WHEN_DIRTY`。渲染模式的设置也很关键，渲染模式有两种：`RENDERMODE_WHEN_DIRTY`和`RENDERMODE_CONTINUOUSLY`。DIRYT的含义是只有当被通知的时候才会去渲染视图，而CONTINUOUSLY的含义是视频会一直连续的渲染。
- 在`onSurfaceCreated()`函数中，创建一个渲染的纹理，这个纹理就是用来显示Camera的图像，所以需要新创建的`SurfaceTexture`绑定在一起，而`SurfaceTexture`是作为渲染的载体，另一方面需要和`DirectDrawer`绑定在一起，`DirectDrawer`是用来绘制图像的，下面会具体介绍。最后是初始化Camera。
- 因为在初始化的时候这是了渲染的模式为`RENDERMODE_WHEN_DIRTY`，所以我们就通知GLSurfaceView什么时候需要渲染图像，而接口`SurfaceTexture.OnFrameAvailableListener`完成这项工作，函数`onFrameAvailable()`在有新数据到来时，会被调用，在其中调用requestRender()，就可以完成新数据的渲染。
- 在`onSurfaceChanged()`函数中，设置了OpenGL窗口的大小,(0,0)表示窗口内部视口的左下角，(w,h)指定了视口的大小；打开Camera的预览。
- 最后，在`onDrawFrame()`函数中绘制更新的纹理。
## 2.2 DirectDrawer
这个类非常重要，负责将SurfaceTexture（纹理的句柄）内容绘制到屏幕上。
```java
public class DirectDrawer {
    private FloatBuffer vertexBuffer, mTextureCoordsBuffer;
    private ShortBuffer drawListBuffer;
    private final int mProgram;
    private int mPositionHandle;
    private int mTextureCoordHandle;
    private int mMVPMatrixHandle;
    private short drawOrder[] = {0, 2, 1, 0, 3, 2}; // order to draw vertices
    // number of coordinates per vertex in this array
    private final int COORDS_PER_VERTEX = 2;
    private final int vertexStride = COORDS_PER_VERTEX * 4; // 4 bytes per vertex
    private float mVertices[] = new float[8];
    private float mTextureCoords[] = new float[8];
    private float mTextHeightRatio = 0.1f;
    private int texture;
    public float[] mMVP = new float[16];
    public void resetMatrix() {
        mat4f_LoadOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, mMVP);
    }
    public DirectDrawer(int texture) {
        String vertextShader = TextResourceReader.readTextFileFromResource(MyApplication.getContext()
                , R.raw.video_vertex_shader);
        String fragmentShader = TextResourceReader.readTextFileFromResource(MyApplication.getContext()
                , R.raw.video_normal_fragment_shader);
        mProgram = GlUtil.createProgram(vertextShader, fragmentShader);
        if (mProgram == 0) {
            throw new RuntimeException("Unable to create program");
        }
        //get handle to vertex shader's vPosition member
        mPositionHandle = GLES20.glGetAttribLocation(mProgram, "vPosition");
        GlUtil.checkLocation(mPositionHandle, "vPosition");
        mTextureCoordHandle = GLES20.glGetAttribLocation(mProgram, "inputTextureCoordinate");
        GlUtil.checkLocation(mTextureCoordHandle, "inputTextureCoordinate");
        mMVPMatrixHandle = GLES20.glGetUniformLocation(mProgram, "uMVPMatrix");
        GlUtil.checkLocation(mMVPMatrixHandle, "uMVPMatrix");
        this.texture = texture;
        // initialize vertex byte buffer for shape coordinates
        updateVertices();
        setTexCoords();
        // initialize byte buffer for the draw list
        ByteBuffer dlb = ByteBuffer.allocateDirect(drawOrder.length * 2);
        dlb.order(ByteOrder.nativeOrder());
        drawListBuffer = dlb.asShortBuffer();
        drawListBuffer.put(drawOrder);
        drawListBuffer.position(0);
        mat4f_LoadOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, mMVP);
    }
    public void draw() {
        GLES20.glUseProgram(mProgram);
        GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
        GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, texture);
        // get handle to vertex shader's vPosition member
        // Enable a handle to the triangle vertices
        GLES20.glEnableVertexAttribArray(mPositionHandle);
        // Prepare the <insert shape here> coordinate data
        GLES20.glVertexAttribPointer(mPositionHandle, COORDS_PER_VERTEX, GLES20.GL_FLOAT, false, vertexStride, vertexBuffer);
        GLES20.glEnableVertexAttribArray(mTextureCoordHandle);
        GLES20.glVertexAttribPointer(mTextureCoordHandle, COORDS_PER_VERTEX, GLES20.GL_FLOAT, false, vertexStride, mTextureCoordsBuffer);
        // Apply the projection and view transformation
        GLES20.glUniformMatrix4fv(mMVPMatrixHandle, 1, false, mMVP, 0);
        GLES20.glDrawElements(GLES20.GL_TRIANGLES, drawOrder.length, GLES20.GL_UNSIGNED_SHORT, drawListBuffer);
        // Disable vertex array
        GLES20.glDisableVertexAttribArray(mPositionHandle);
        GLES20.glDisableVertexAttribArray(mTextureCoordHandle);
    }
    public static void mat4f_LoadOrtho(float left, float right, float bottom, float top, float near, float far, float[] mout) {
        float r_l = right - left;
        float t_b = top - bottom;
        float f_n = far - near;
        float tx = -(right + left) / (right - left);
        float ty = -(top + bottom) / (top - bottom);
        float tz = -(far + near) / (far - near);
        mout[0] = 2.0f / r_l;
        mout[1] = 0.0f;
        mout[2] = 0.0f;
        mout[3] = 0.0f;
        mout[4] = 0.0f;
        mout[5] = 2.0f / t_b;
        mout[6] = 0.0f;
        mout[7] = 0.0f;
        mout[8] = 0.0f;
        mout[9] = 0.0f;
        mout[10] = -2.0f / f_n;
        mout[11] = 0.0f;
        mout[12] = tx;
        mout[13] = ty;
        mout[14] = tz;
        mout[15] = 1.0f;
    }
    public void updateVertices() {
        final float w = 1.0f;
        final float h = 1.0f;
        mVertices[0] = -w;
        mVertices[1] = h;
        mVertices[2] = -w;
        mVertices[3] = -h;
        mVertices[4] = w;
        mVertices[5] = -h;
        mVertices[6] = w;
        mVertices[7] = h;
        vertexBuffer = ByteBuffer.allocateDirect(mVertices.length * 4).order(ByteOrder.nativeOrder())
                .asFloatBuffer().put(mVertices);
        vertexBuffer.position(0);
    }
    public void setTexCoords() {
        mTextureCoords[0] = 0;
        mTextureCoords[1] = 1 - mTextHeightRatio;
        mTextureCoords[2] = 1;
        mTextureCoords[3] = 1 - mTextHeightRatio;
        mTextureCoords[4] = 1;
        mTextureCoords[5] = 0 + mTextHeightRatio;
        mTextureCoords[6] = 0;
        mTextureCoords[7] = 0 + mTextHeightRatio;
        mTextureCoordsBuffer = ByteBuffer.allocateDirect(mTextureCoords.length * 4).order(ByteOrder.nativeOrder())
                .asFloatBuffer().put(mTextureCoords);
        mTextureCoordsBuffer.position(0);
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
这个类的主要功能就是绘制图像。
(1) 定义**Vertex Shader（顶点着色器，用来绘制图形的形状）**、**Fragment Shader（片段着色器，用来绘制图形的颜色或者纹理）**和**Program（OpenGL ES对象，包含了用来绘制一个或者多个形状的shader）**，然后接下来都是围绕着这三个变量，最后通过调用OpenGL方法进行绘制。具体的过程可以参考前面的博客 **[使用OpenGL ES显示图形](http://blog.csdn.net/a296777513/article/details/54729264)**
(2) 既然我们需要预览Camera的视频数据，那么我们可以知道现实的区域的形状大部分都是四边形，但是在OpenGL中只有提供了绘制三角形的方法，我们就需要将两个三角形拼接成一个正方形，所以需要定义一个大小为8的数组，如下面代码所示：
```java
static float squareCoords[] = {  
       -1.0f,  1.0f,  // 左上点
       -1.0f, -1.0f,  // 左下点
        1.0f, -1.0f,  // 右下点
        1.0f,  1.0f,  // 有上点
    };
```
- 1
- 2
- 3
- 4
- 5
- 6
此时，我们就有了一个四边形的4个点的数据了。但是，OpenGL并不是对数组的数据直接进行操作的，而是在直接内存中，即操作的数据需要保存到NIO里面的Buffer对象中。而我们上面生命的float[]对象保存在数组中，因此我们需要将float[]对象转换为Java.nio.Buffer对象，代码如下：
```java
public void updateVertices() {
        final float w = 1.0f;
        final float h = 1.0f;
        mVertices[0] = -w;
        mVertices[1] = h;
        mVertices[2] = -w;
        mVertices[3] = -h;
        mVertices[4] = w;
        mVertices[5] = -h;
        mVertices[6] = w;
        mVertices[7] = h;
        vertexBuffer = ByteBuffer.allocateDirect(mVertices.length * 4).order(ByteOrder.nativeOrder())
                .asFloatBuffer().put(mVertices);
        vertexBuffer.position(0);
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
注意，ByteBuffer和FloatBuffer以及IntBuffer都是继承自抽象类java.nio.Buffer。 
另外，OpenGL在底层的实现是C语言，与Java默认的数据存储字节顺序可能不同，即大端小端问题。因此，为了保险起见，在将数据传递给OpenGL之前，我们需要指明使用本机的存储顺序。 
此时，我们顺利地将float[]转为了FloatBuffer，后面绘制三角形的时候，直接通过成员变量mTriangleBuffer即可。
(3) 最后就是将准备好的数据绘制到屏幕上，OpenGL 提供了两个绘制的方法`glDrawArrays(int mode, int first, int count)`和`glDrawElements(int mode,int count, int type, Buffer indices)`两个方法，在这里我们使用的第二种绘制的方法，关于mode有几种模式供我们选择：
- `GL_POINTS`：绘制独立的点到屏幕 
![](http://www.apkbus.com/data/attachment/forum/201202/05/10462818p1duzedh4znmnz.png)
- `GL_LINE_STRIP`：连续的连线，第n个顶点与第n-1个顶点绘制一条直线 
![](http://www.apkbus.com/data/attachment/forum/201202/05/104629mb5577z7rn5mw0vz.png)
- `GL_LINE_LOOP`：与上一个相同，但是需要首尾相联接 
![](http://www.apkbus.com/data/attachment/forum/201202/05/104630eey4add99xicssaa.png)
- `GL_LINES`：形成对的独立的线段 
![](http://www.apkbus.com/data/attachment/forum/201202/05/104630u25qj5xjj13c3jw3.png)
- `GL_TRIANGLE_STRIP`：绘制一系列的三角形，先是顶点v0，v1，v2，然后是v2，v1，v3（注意规律），然后v2，v3，v4等。该规律确保所有的三角形都以相同的方向绘制 
![](http://www.apkbus.com/data/attachment/forum/201202/05/104632zxyaswxkawaaex5x.png)
- `GL_TRIANGLE_FAN`和`GL_TRANGLE_STRIP`类似，但其县绘制v0，v1，v2，再是v0，v2，v3，然后v0，v3，v4等。 
![](http://www.apkbus.com/data/attachment/forum/201202/05/1046336cusrgcgkygt1geu.png)
(4) 需要注意的是，在这个类中，定义了mMVP这个数组，这个数组的功能是对视频帧数据进行转换的，例如旋转图像等。
# 第三章 总结
到此为止，使用GLSurfaceView预览Camera的介绍就完了，这篇文章，仅仅介绍了`CameraGLSurfaceView`和`DirectDrawer`这两个类，但是如何对Camera进行操作的并没有介绍，这不是本文的重点，所以就省略了。接下来还会介绍一些有关GLSurfaceView的文章。
[Android OpenGL渲染双视频](http://blog.csdn.net/a296777513/article/details/70495534)
# [下载代码](http://download.csdn.net/detail/a296777513/9786330)
