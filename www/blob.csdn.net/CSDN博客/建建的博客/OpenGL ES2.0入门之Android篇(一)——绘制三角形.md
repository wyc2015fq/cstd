# OpenGL ES2.0入门之Android篇(一)——绘制三角形 - 建建的博客 - CSDN博客
2018年02月28日 17:56:57[纪建](https://me.csdn.net/u013898698)阅读数：28
个人分类：[openGL](https://blog.csdn.net/u013898698/article/category/7028208)
转载请注明出处： [http://blog.csdn.net/lb377463323/article/details/52136518](http://blog.csdn.net/lb377463323/article/details/52136518)
## OpenGL ES简介
- OpenGL ES是一个为便携式或嵌入式设备例如：移动电话、监视器等发展的3D绘图API。
- 在Android框架中有两个基础类用于使用OpenGL ES创建和处理图形 
- GLSurfaceView类是OpenGL ES绘制图形的view容器
- GLSurfaceView.Renderer类是用于控制上述view容器中显示什么内容 
## Android使用OpenGL ES绘制三角形
- 整个过程比较繁琐，代码也挺多的。因为 OpenGL ES 2.0 是以可编程着色器为基础的，这意味着你绘制任何图形都必须有一 
个合适的着色器装载和绑定，比使用固定管线的桌面版本有更多代码。
#### 1. 在manifest中声明使用OpenGL ES 2.0 API
```
<uses-feature android:glEsVersion="0x00020000" android:required="true" />
```
- 1
- 2
> 
0x00020000代表OpenGL ES 2.0版本，3.0版本是0x00030000，3.1版本是0x00030001
- 
若应用需要使用纹理压缩（纹理压缩通过减少内存需求和更有效的使用内存带宽使OpenGL ES应用的性能显著增加）功能，还需在manifest声明所支持的压缩格式，如下：
#### 2. 将Activity的ContentView设为GLSurfaceView
```
public class OpenGLES20Activity extends Activity {
    private GLSurfaceView mGLView;
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mGLView = new MyGLSurfaceView(this);
        setContentView(mGLView);
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
#### 3. 构造GLSurfaceView对象
```
class MyGLSurfaceView extends GLSurfaceView {
    private final MyGLRenderer mRenderer;
    public MyGLSurfaceView(Context context){
        super(context);
        // 创建OpenGL ES 2.0的上下文
        setEGLContextClientVersion(2);
        mRenderer = new MyGLRenderer();
        //设置Renderer用于绘图
        setRenderer(mRenderer);
        //只有在绘制数据改变时才绘制view，可以防止GLSurfaceView帧重绘
        setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
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
#### 4. 构造GLSurfaceView.Renderer对象
- 
Renderer类提供三个回调方法供Android系统调用，用来计算在GLSurfaceView中绘制什么以及如何绘制。
- onSurfaceCreated()：仅调用一次，用于设置view的OpenGL ES环境
- onDrawFrame()：每次重绘view时调用
- onSurfaceChanged()：当view的几何形状发生变化时调用，比如设备屏幕方向改变时
public class MyGLRenderer implements GLSurfaceView.Renderer {
public void onSurfaceCreated(GL10 unused, EGLConfig config) { 
    //设置背景色（r,g,b,a） 
    GLES20.glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//白色不透明 
}
public void onDrawFrame(GL10 unused) { 
    //重绘背景色 
    GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT); 
}
public void onSurfaceChanged(GL10 unused, int width, int height) { 
    //绘制窗口 
    GLES20.glViewport(0, 0, width, height); 
} 
}
#### 5. 在定义三角形之前需了解Android和OpenGL ES的坐标系
- Android屏幕左上角为原点，往右为X正方向，往下为Y正方向
- Android屏幕的中心点坐标为OpenGL的原点，方向如下图，并且最大和最小值为1和-1
- 也就是说OpenGL的（-1,1,0）为屏幕的左上角，
![](http://7xrcwc.com1.z0.glb.clouddn.com/Coordinate.png)
#### 6. 定义一个三角形
```
public class Triangle {
    private FloatBuffer vertexBuffer;
    //设置每个顶点的坐标数
    static final int COORDS_PER_VERTEX = 3;
    //设置三角形顶点数组
    static float triangleCoords[] = {   //默认按逆时针方向绘制
         0.0f,  1.0f, 0.0f, // 顶点
        -1.0f, -0.0f, 0.0f, // 左下角
         1.0f, -0.0f, 0.0f  // 右下角
};
// 设置三角形颜色和透明度（r,g,b,a）
float color[] = {0.0f, 1.0f, 0f, 1.0f};//绿色不透明
public Triangle() {
    // 初始化顶点字节缓冲区，用于存放形状的坐标
    ByteBuffer bb = ByteBuffer.allocateDirect(
            //(每个浮点数占用4个字节
            triangleCoords.length * 4);
    //设置使用设备硬件的原生字节序
    bb.order(ByteOrder.nativeOrder());
    //从ByteBuffer中创建一个浮点缓冲区
    vertexBuffer = bb.asFloatBuffer();
    // 把坐标都添加到FloatBuffer中
    vertexBuffer.put(triangleCoords);
    //设置buffer从第一个坐标开始读 
    vertexBuffer.position(0);
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
#### 7. 在MyGLRenderer中初始化三角形形状
```
private Triangle mTriangle;
public void onSurfaceCreated(GL10 unused, EGLConfig config) {
    ...
    //初始化三角形
    mTriangle = new Triangle();
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
#### 8. 绘制三角形形状
使用OpenGL ES 2.0绘制一个定义好的形状需要大量的代码，因为必须提供给图形渲染管道很多细节信息
- VertexShader：用于渲染形状的顶点的OpenGL ES图形代码
- FragmentShader：用于渲染形状的外观（颜色或纹理）的OpenGL ES代码
- Program：一个OpenGL ES对象，包含了你想要用来绘制一个或多个形状的shader
至少需要一个vertex shader来绘制一个形状和一个fragment shader来为形状着色
```
public class Triangle {
    private final String vertexShaderCode =
        "attribute vec4 vPosition;" +
        "void main() {" +
        "  gl_Position = vPosition;" +
        "}";
    private final String fragmentShaderCode =
        "precision mediump float;" +
        "uniform vec4 vColor;" +
        "void main() {" +
        "  gl_FragColor = vColor;" +
        "}";
    ...
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
Shader包含OpenGL Shading Language(GLSL)代码，必须在OpenGL ES环境下先编译再使用。想要编译这些代码，需要在你的Renderer类中创建一个工具类方法：
```
public static int loadShader(int type, String shaderCode){
    //创建一个vertex shader类型(GLES20.GL_VERTEX_SHADER)
    //或一个fragment shader类型(GLES20.GL_FRAGMENT_SHADER)
    int shader = GLES20.glCreateShader(type);
    // 将源码添加到shader并编译它
    GLES20.glShaderSource(shader, shaderCode);
    GLES20.glCompileShader(shader);
    return shader;
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
在三角形类中编译shader代码，并将它们添加到一个OpenGL ES program 对象中，然后链接这个program
```
public class Triangle() {
...
private final int mProgram;
public Triangle() {
    ...
    // 编译shader代码
    int vertexShader = MyGLRenderer.loadShader(GLES20.GL_VERTEX_SHADER,
                                    vertexShaderCode);
    int fragmentShader = MyGLRenderer.loadShader(GLES20.GL_FRAGMENT_SHADER,
                                    fragmentShaderCode);
    // 创建空的OpenGL ES Program
    mProgram = GLES20.glCreateProgram();
    // 将vertex shader添加到program
    GLES20.glAttachShader(mProgram, vertexShader);
    // 将fragment shader添加到program
    GLES20.glAttachShader(mProgram, fragmentShader);
    // 创建可执行的 OpenGL ES program  
    GLES20.glLinkProgram(mProgram);
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
在三角形类中创建一个draw()方法负责绘制形状
```
private int mPositionHandle;
private int mColorHandle;
private final int vertexCount = triangleCoords.length / COORDS_PER_VERTEX;
private final int vertexStride = COORDS_PER_VERTEX * 4; // 4 bytes per vertex
public void draw() {
    // 添加program到OpenGL ES环境中
    GLES20.glUseProgram(mProgram);
    // 获取指向vertex shader的成员vPosition的handle 
    mPositionHandle = GLES20.glGetAttribLocation(mProgram, "vPosition");
    // 启用一个指向三角形的顶点数组的handle
    GLES20.glEnableVertexAttribArray(mPositionHandle);
    //准备三角形的坐标数据
    GLES20.glVertexAttribPointer(mPositionHandle, COORDS_PER_VERTEX,
                                 GLES20.GL_FLOAT, false,
                                 vertexStride, vertexBuffer);
    // 获取指向fragment shader的成员vColor的handle
    mColorHandle = GLES20.glGetUniformLocation(mProgram, "vColor");
    //  绘制三角形
    GLES20.glUniform4fv(mColorHandle, 1, color, 0);
    // Draw the triangle
    GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, vertexCount);
    // 禁用指向三角形的顶点数组 
    GLES20.glDisableVertexAttribArray(mPositionHandle);
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
#### 9. 在Render类中的onDrawFrame()方法中调用draw()方法
```
public void onDrawFrame(GL10 unused) {
    ...
    mTriangle.draw();
}
```
- 1
- 2
- 3
- 4
- 5
- 6
此时运行程序显示如下图： 
![](http://7xrcwc.com1.z0.glb.clouddn.com/Triangle.png)
