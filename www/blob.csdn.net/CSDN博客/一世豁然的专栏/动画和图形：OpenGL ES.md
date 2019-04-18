# 动画和图形：OpenGL ES - 一世豁然的专栏 - CSDN博客





2017年05月23日 10:42:04[一世豁然](https://me.csdn.net/Explorer_day)阅读数：2395








Android包括使用Open Graphics Library（OpenGL®），特别是OpenGL ES API支持高性能2D和3D图形。 OpenGL是一个跨平台图形API，用于指定3D图形处理硬件的标准软件界面。 OpenGL ES是面向嵌入式设备的OpenGL规范的风格。 Android支持多种版本的OpenGL ES API：

1、OpenGL ES 1.0和1.1 - 该API规范由Android 1.0及更高版本支持。




2、OpenGL ES 2.0 - 此API规范由Android 2.2（API级别8）及更高版本支持。




3、OpenGL ES 3.0 - 此API规范由Android 4.3（API级别18）及更高版本支持。




4、OpenGL ES 3.1 - 此API规范由Android 5.0（API级别21）及更高版本支持。




注意：在设备上支持OpenGL ES 3.0 API需要由设备制造商提供的此图形管道的实现。 运行Android 4.3或更高版本的设备可能不支持OpenGL ES 3.0 API。 有关在运行时检查支持的OpenGL ES版本的信息，请参阅检查OpenGL ES版本。





注意：由Android框架提供的具体API类似于J2ME JSR239 OpenGL ES API，但并不完全相同。 如果您熟悉J2ME JSR239规范，请注意变化。








一、基础

Android通过其框架API和Native Development Kit（NDK）支持OpenGL。 本主题侧重于Android框架界面。 有关NDK的更多信息，请参阅Android NDK。





Android框架中有两个基础类，可让您使用OpenGL ES API创建和操作图形：GLSurfaceView和GLSurfaceView.Renderer。 如果您的目标是在Android应用程序中使用OpenGL，那么了解如何在活动中实现这些类应该是您的第一个目标。





`[GLSurfaceView](https://developer.android.google.cn/reference/android/opengl/GLSurfaceView.html)`


这个类是一个视图，您可以使用OpenGL API调用绘制和操作对象，并且在功能上与SurfaceView类似。 您可以通过创建GLSurfaceView的实例并将其渲染器添加到该类中来使用此类。 但是，如果要捕获触摸屏事件，您应该扩展GLSurfaceView类来实现触摸侦听器，如OpenGL培训课程“响应触摸事件”所示。





`[GLSurfaceView.Renderer](https://developer.android.google.cn/reference/android/opengl/GLSurfaceView.Renderer.html)`


该界面定义了在GLSurfaceView中绘制图形所需的方法。 您必须将此接口的实现提供为单独的类，并使用GLSurfaceView.setRenderer（）将其附加到GLSurfaceView实例。


GLSurfaceView.Renderer接口要求您实现以下方法：


1、onSurfaceCreated（）：创建GLSurfaceView时，系统会调用此方法一次。 使用此方法执行只需要执行一次的操作，例如设置OpenGL环境参数或初始化OpenGL图形对象。

2、onDrawFrame（）：系统在每次重绘GLSurfaceView时调用此方法。 使用此方法作为绘制（和重新绘制）图形对象的主要执行点。

3、onSurfaceChanged（）：当GLSurfaceView几何更改时，系统调用此方法，包括GLSurfaceView的大小更改或设备屏幕方向的更改。 例如，当设备从纵向更改为横向时，系统会调用此方法。
 使用此方法来响应GLSurfaceView容器中的更改。




一）、OpenGL ES软件包包

一旦您使用GLSurfaceView和GLSurfaceView.Renderer建立了OpenGL ES的容器视图，就可以使用以下类开始调用OpenGL API：


1、OpenGL ES 1.0 / 1.1 API软件包

 1）、android.opengl - 该包为OpenGL ES 1.0 / 1.1类提供了一个静态接口，并且性能优于javax.microedition.khronos包接口。

   I、GLES10

    II、GLES10Ext

    III、GLES11

 
   IV、GLES11Ext

 2）、javax.microedition.khronos.opengles
 - 该包提供OpenGL ES 1.0 / 1.1的标准实现。

 
   I、GL10

 
   II、GL10Ext

 
   III、GL11

 
   IV、GL11Ext

 
   V、GL11ExtensionPack




2、OpenGL
 ES 2.0 API类

 1）、android.opengl.GLES20
 - 此软件包提供了OpenGL ES 2.0的界面，可从Android 2.2（API 8级）开始。




3、OpenGL
 ES 3.0 / 3.1 API软件包

 1）、android.opengl
 - 这个包提供了OpenGL ES 3.0 / 3.1类的接口。 版本3.0可从Android 4.3（API级别18）开始。 版本3.1可从Android 5.0（API级别21）开始。

 
   I、GLES30

 
   II、GLES31

 
   III、GLES31Ext（Android Extension Pack）




如果要立即开始使用OpenGL
 ES构建应用程序，请使用OpenGL ES类显示图形。








二、声明OpenGL要求

如果您的应用程序使用所有设备上不提供的OpenGL功能，则必须在AndroidManifest.xml文件中包含这些要求。
 以下是最常见的OpenGL清单声明：


1、OpenGL
 ES版本要求 - 如果您的应用程序需要特定版本的OpenGL ES，则必须通过向您的清单添加以下设置来声明该要求，如下所示。

 1）、对于OpenGL
 ES 2.0：



```
<!-- Tell the system this app requires OpenGL ES 2.0. -->
<uses-feature android:glEsVersion="0x00020000" android:required="true" />
```


添加此声明将导致Google Play将您的应用程序限制在不支持OpenGL ES 2.0的设备上。 如果您的应用程序专门用于支持OpenGL ES 3.0的设备，那么您还可以在清单中指定此设置：

 2）、对于OpenGL
 ES 3.0：



```
<!-- Tell the system this app requires OpenGL ES 3.0. -->
<uses-feature android:glEsVersion="0x00030000" android:required="true" />
```


3）、对于OpenGL ES 3.1：


```
<!-- Tell the system this app requires OpenGL ES 3.1. -->
<uses-feature android:glEsVersion="0x00030001" android:required="true" />
```


注意：OpenGL ES 3.x API与2.0 API向后兼容，这意味着您可以更灵活地在应用程序中实现OpenGL ES。 通过在您的清单中声明OpenGL ES 2.0 API作为要求，您可以将该API版本作为默认值，在运行时检查3.x API的可用性，然后在设备支持它时使用OpenGL ES 3.x功能。 有关检查设备支持的OpenGL
 ES版本的更多信息，请参阅检查OpenGL ES版本。




2、纹理压缩要求
 - 如果您的应用程序使用纹理压缩格式，则必须使用<supports-gl-texture>声明应用程序在清单文件中支持的格式。 有关可用纹理压缩格式的更多信息，请参阅纹理压缩支持。

在清单中声明纹理压缩要求会隐藏您的应用程序，该用户的设备不支持至少一种已声明的压缩类型。
 有关Google Play过滤如何适用于纹理按压的更多信息，请参阅<supports-gl-texture>文档的Google Play和纹理压缩过滤部分。








三、绘制对象的映射坐标

在Android设备上显示图形的一个基本问题是它们的屏幕尺寸和形状可能会有所不同。
 OpenGL假设一个平方均匀的坐标系，默认情况下，将这些坐标快乐地绘制到您的典型非正方形屏幕上，就好像它是完全正方形。


![](https://img-blog.csdn.net/20170523095137090?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


图1.默认OpenGL坐标系（左）映射到典型的Android设备屏幕（右）。





上图显示了左侧OpenGL框架所采用的统一坐标系，以及这些坐标实际如何映射到右侧横向的典型设备屏幕。
 要解决此问题，您可以应用OpenGL投影模式和摄像机视图来转换坐标，以便您的图形对象在任何显示屏上具有正确的比例。





为了应用投影和摄像机视图，您可以创建投影矩阵和相机视图矩阵并将其应用于OpenGL渲染管线。
 投影矩阵重新计算您的图形的坐标，以便它们正确映射到Android设备屏幕。 相机视图矩阵创建一个从特定眼睛位置呈现对象的转换。





一）、OpenGL
 ES 1.0中的投影和摄像头视图

在ES
 1.0 API中，您可以通过创建每个矩阵，然后将其添加到OpenGL环境来应用投影和摄像机视图。





1、投影矩阵
 - 使用设备屏幕的几何创建投影矩阵，以重新计算对象坐标，以便以正确的比例绘制。 以下示例代码演示如何修改GLSurfaceView.Renderer实现的onSurfaceChanged（）方法，以根据屏幕的宽高比创建投影矩阵，并将其应用于OpenGL渲染环境。



```java
public void onSurfaceChanged(GL10 gl, int width, int height) {
    gl.glViewport(0, 0, width, height);

    // make adjustments for screen ratio
    float ratio = (float) width / height;
    gl.glMatrixMode(GL10.GL_PROJECTION);        // set matrix to projection mode
    gl.glLoadIdentity();                        // reset the matrix to its default state
    gl.glFrustumf(-ratio, ratio, -1, 1, 3, 7);  // apply the projection matrix
}
```


2、相机转换矩阵 - 使用投影矩阵调整坐标系后，还必须应用相机视图。 以下示例代码显示了如何修改GLSurfaceView.Renderer实现的onDrawFrame（）方法以应用模型视图，并使用GLU.gluLookAt（）实用程序来创建模拟摄像头位置的视图转换。



```java
public void onDrawFrame(GL10 gl) {
    ...
    // Set GL_MODELVIEW transformation mode
    gl.glMatrixMode(GL10.GL_MODELVIEW);
    gl.glLoadIdentity();                      // reset the matrix to its default state

    // When using GL_MODELVIEW, you must set the camera view
    GLU.gluLookAt(gl, 0, 0, -5, 0f, 0f, 0f, 0f, 1.0f, 0.0f);
    ...
}
```



二）、OpenGL
 ES 2.0及更高版本中的投影和摄像机视图

在ES
 2.0和3.0 API中，您可以通过首先将矩阵成员添加到图形对象的顶点着色器来应用投影和相机视图。 添加此矩阵成员后，您可以生成并应用投影和相机查看矩阵到您的对象。





1、将矩阵添加到顶点着色器
 - 为视图投影矩阵创建一个变量，并将其包含为着色器位置的乘数。 在下面的示例中，顶点着色器代码，包含的uMVPMatrix成员允许您将投影和相机查看矩阵应用于使用该着色器的对象的坐标。



```java
private final String vertexShaderCode =

    // This matrix member variable provides a hook to manipulate
    // the coordinates of objects that use this vertex shader.
    "uniform mat4 uMVPMatrix;   \n" +

    "attribute vec4 vPosition;  \n" +
    "void main(){               \n" +
    // The matrix must be included as part of gl_Position
    // Note that the uMVPMatrix factor *must be first* in order
    // for the matrix multiplication product to be correct.
    " gl_Position = uMVPMatrix * vPosition; \n" +

    "}  \n";
```


注意：上面的示例定义了顶点着色器中的单个转换矩阵成员，您可以在其中应用组合投影矩阵和相机视图矩阵。 根据您的应用需求，您可能需要在顶点着色器中定义单独的投影矩阵和相机查看矩阵成员，以便您可以独立地更改它们。




2、访问着色器矩阵
 - 在您的顶点着色器中创建一个钩子以应用投影和相机视图后，您可以访问该变量以应用投影和相机查看矩阵。 以下代码显示了如何修改GLSurfaceView.Renderer实现的onSurfaceCreated（）方法来访问上面顶点着色器中定义的矩阵变量。



```java
public void onSurfaceCreated(GL10 unused, EGLConfig config) {
    ...
    muMVPMatrixHandle = GLES20.glGetUniformLocation(mProgram, "uMVPMatrix");
    ...
}
```


3、创建投影和相机查看矩阵 - 生成要应用图形对象的投影和查看矩阵。 以下示例代码显示了如何修改GLSurfaceView.Renderer实现的onSurfaceCreated（）和onSurfaceChanged（）方法，以创建基于设备屏幕宽高比的摄像机视图矩阵和投影矩阵。




4、应用投影和相机观看矩阵
 - 要应用投影和相机视图转换，将矩阵相乘，然后将其设置为顶点着色器。 以下示例代码显示了如何修改GLSurfaceView.Renderer实现的onDrawFrame（）方法来组合在上述代码中创建的投影矩阵和相机视图，然后将其应用于由OpenGL呈现的图形对象。




```java
public void onDrawFrame(GL10 unused) {
    ...
    // Combine the projection and camera view matrices
    Matrix.multiplyMM(mMVPMatrix, 0, mProjMatrix, 0, mVMatrix, 0);

    // Apply the combined projection and camera view transformations
    GLES20.glUniformMatrix4fv(muMVPMatrixHandle, 1, false, mMVPMatrix, 0);

    // Draw objects
    ...
}
```


有关如何使用OpenGL ES 2.0应用投影和摄像头视图的完整示例，请参阅使用OpenGL ES类显示图形。







四、形状面和绕组

在OpenGL中，形状的面是由三维空间中的三个或更多个点定义的表面。
 一组三个或更多的三维点（在OpenGL中称为顶点）具有正面和背面。 你怎么知道哪个面是正面，哪个是背面？ 好问题。 答案与卷曲有关，或者您定义形状点的方向。


![](https://img-blog.csdn.net/20170523100314878?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


图1.坐标列表的图示，其转换为逆时针绘图顺序。





在该示例中，三角形的点以按逆时针方向绘制的顺序来定义。
 绘制这些坐标的顺序定义了形状的卷绕方向。 默认情况下，在OpenGL中，逆时针方向绘制的面是前脸。 定义图1所示的三角形，以便您正在查看形状的正面（由OpenGL解释），另一面是背面。





为什么要知道形状的哪一面是前面？
 答案与OpenGL的常用功能有关，称为面部剔除。 面部剔除是OpenGL环境的一个选择，允许渲染管道忽略（不计算或绘制）形状的背面，节省时间，内存和处理周期：




```java
// enable face culling feature
gl.glEnable(GL10.GL_CULL_FACE);
// specify which faces to not draw
gl.glCullFace(GL10.GL_BACK);
```


如果您尝试使用脸部剔除功能，而不知道您的形状的哪一面是正面和背面，您的OpenGL图形将看起来有点薄，或者可能根本不显示。 因此，始终以逆时针绘图顺序定义OpenGL形状的坐标。




注意：可以设置一个OpenGL环境来将顺时针面部视为正面，但这样做需要更多的代码，并且可能会在经验丰富的OpenGL开发人员询问帮助时混淆。
 所以不要这样做。








五、OpenGL版本和设备兼容性

OpenGL ES 1.0和1.1 API规范自Android 1.0以来一直得到支持。 从Android 2.2（API 8级）开始，该框架支持OpenGL ES 2.0 API规范。 大多数Android设备都支持OpenGL ES 2.0，推荐用于OpenGL开发的新应用。 在提供OpenGL ES 3.0 API实现的设备上，Android
 4.3（API级别18）及更高版本支持OpenGL ES 3.0。 有关支持给定版本的OpenGL ES的Android驱动的设备的相对数量的信息，请参阅OpenGL ES版本仪表板。





使用OpenGL ES 1.0 / 1.1 API的图形编程与使用2.0及更高版本显着不同。 API的1.x版本具有更多的便利方法和固定的图形管道，而OpenGL ES 2.0和3.0 API通过使用OpenGL着色器提供对管道的更直接控制。 您应该仔细考虑图形要求，并选择最适合您的应用程序的API版本。 有关详细信息，请参阅选择OpenGL
 API版本。





OpenGL ES 3.0 API提供了比2.0 API更多的功能和更好的性能，并且还向后兼容。 这意味着您可以编写面向OpenGL ES 2.0的应用程序，并有条件地包含OpenGL ES 3.0图形功能（如果可用）。 有关检查3.0 API可用性的更多信息，请参阅检查OpenGL ES版本。





一）、纹理压缩支持

纹理压缩可以通过减少内存需求并更有效地利用内存带宽，显着提高OpenGL应用程序的性能。 Android框架作为标准功能提供对ETC1压缩格式的支持，包括ETC1Util实用程序类和etc1tool压缩工具（位于Android SDK（<sdk> / tools /））。 有关使用纹理压缩的Android应用程序的示例，请参阅Android
 SDK（<sdk> / samples / <version> / ApiDemos / src / com / example / android / apis / graphics /）中的CompressedTextureActivity代码示例。





注意：大多数Android设备都支持ETC1格式，但不能保证可用。 要检查设备是否支持ETC1格式，请调用ETC1Util.isETC1Supported（）方法。





注意：ETC1纹理压缩格式不支持透明度（Alpha通道）的纹理。 如果您的应用程序需要具有透明度的纹理，则应调查目标设备上可用的其他纹理压缩格式。





当使用OpenGL ES 3.0 API时，ETC2 / EAC纹理压缩格式保证可用。 这种纹理格式提供出色的压缩比，具有高视觉质量，格式也支持透明度（Alpha通道）。





除了ETC格式，Android设备基于GPU芯片组和OpenGL实现，对纹理压缩有着不同的支持。 您应该调查您正在定位的设备上的纹理压缩支持，以确定应用程序应支持哪些压缩类型。 为了确定给定设备支持什么纹理格式，您必须查询设备并查看OpenGL扩展名，这些名称标识了设备支持的纹理压缩格式（以及其他OpenGL特性）。 一些通常支持的纹理压缩格式如下：


1、ATITC（ATC） - ATI纹理压缩（ATITC或ATC）可用于各种设备，并支持具有和不具有Alpha通道的RGB纹理的固定速率压缩。 该格式可以由几个OpenGL扩展名表示，例如：

 1）、GL_AMD_compressed_ATC_texture

 2）、GL_ATI_texture_compression_atitc




2、PVRTC - PowerVR纹理压缩（PVRTC）可用于各种设备，并支持具有或不具有Alpha通道的每像素2位和4位纹理。 此格式由以下OpenGL扩展名称表示：

 1）、GL_IMG_texture_compression_pvrtc




3、S3TC（DXTn / DXTC） - S3纹理压缩（S3TC）具有多种格式变化（DXT1至DXT5），并且不太广泛。 该格式支持RGB纹理与4位alpha或8位alpha通道。 这些格式由以下OpenGL扩展名称表示：

 1）、GL_EXT_texture_compression_s3tc

一些设备只支持DXT1格式变化; 此有限支持由以下OpenGL扩展名称表示：


 2）、GL_EXT_texture_compression_dxt1




4、3DC - 3DC纹理压缩（3DC）是一种不太广泛的可用格式，支持具有Alpha通道的RGB纹理。 此格式由以下OpenGL扩展名称表示：

 1）、GL_AMD_compressed_3DC_texture




警告：所有设备都不支持这些纹理压缩格式。 对这些格式的支持可能因制造商和设备而异。 有关如何确定特定设备上的纹理压缩格式的信息，请参阅下一节。





注意：一旦您决定应用程序将支持哪种纹理压缩格式，请确保使用<supports-gl-texture>在清单中声明它们。 使用此声明可以通过外部服务（如Google Play）进行过滤，以便您的应用程序仅安装在支持应用程序所需格式的设备上。 有关详细信息，请参阅OpenGL清单声明。





二）、确定OpenGL扩展

根据OpenGL ES API的扩展，OpenGL的实现因Android设备而异。 这些扩展包括纹理压缩，但通常还包括OpenGL功能集的其他扩展。





要确定特定设备支持哪些纹理压缩格式和其他OpenGL扩展：


1、在目标设备上运行以下代码，以确定支持哪种纹理压缩格式：



```java
String extensions = javax.microedition.khronos.opengles.GL10.glGetString(
        GL10.GL_EXTENSIONS);
```


警告：此呼叫的结果因设备型号而异！ 您必须在多个目标设备上运行此调用，以确定通常支持哪些压缩类型。




2、查看此方法的输出以确定设备上支持哪些OpenGL扩展。




**Android扩展程序包（AEP）**


AEP确保您的应用程序支持OpenGL 3.1规范中描述的核心集合之上和之外的标准化OpenGL扩展。 将这些扩展包在一起鼓励跨设备的一系列功能，同时允许开发人员充分利用最新的移动GPU设备。





AEP还改进了片段着色器中对图像，着色器存储缓冲区和原子计数器的支持。





为了使您的应用程序能够使用AEP，应用程序的清单必须声明AEP是必需的。 此外，平台版本必须支持它。





在清单中声明AEP要求如下：




```
<uses feature android:name="android.hardware.opengles.aep"
              android:required="true" />
```


要验证平台版本是否支持AEP，请使用hasSystemFeature（String）方法，传入FEATURE_OPENGLES_EXTENSION_PACK作为参数。 以下代码片段显示了如何执行此操作的示例：



```java
boolean deviceSupportsAEP = getPackageManager().hasSystemFeature
     (PackageManager.FEATURE_OPENGLES_EXTENSION_PACK);
```
如果方法返回true，则支持AEP。


有关AEP的更多信息，请访问其位于Khronos OpenGL ES注册表的页面。





三）、检查OpenGL ES版本

OpenGL ES有几个版本可在Android设备上使用。 您可以指定应用程序在清单中需要的API的最低版本，但您可能还需要同时利用较新API中的功能。 例如，OpenGL ES 3.0 API与2.0版本的API向后兼容，因此您可能需要编写应用程序以使其使用OpenGL ES 3.0功能，但如果3.0
 API不是，则可以回到2.0 API可用。





在使用高于应用程序清单要求的最低版本的OpenGL ES功能之前，应用程序应检查设备上可用的API版本。 你可以通过以下两种方法之一来实现：


1、尝试创建更高级别的OpenGL ES上下文（EGLContext）并检查结果。

2、创建最低支持的OpenGL ES上下文并检查版本值。




以下示例代码演示如何通过创建EGLContext并检查结果来检查可用的OpenGL ES版本。 此示例显示如何检查OpenGL ES 3.0版本：




```java
private static double glVersion = 3.0;

private static class ContextFactory implements GLSurfaceView.EGLContextFactory {

  private static int EGL_CONTEXT_CLIENT_VERSION = 0x3098;

  public EGLContext createContext(
          EGL10 egl, EGLDisplay display, EGLConfig eglConfig) {

      Log.w(TAG, "creating OpenGL ES " + glVersion + " context");
      int[] attrib_list = {EGL_CONTEXT_CLIENT_VERSION, (int) glVersion,
              EGL10.EGL_NONE };
      // attempt to create a OpenGL ES 3.0 context
      EGLContext context = egl.eglCreateContext(
              display, eglConfig, EGL10.EGL_NO_CONTEXT, attrib_list);
      return context; // returns null if 3.0 is not supported;
  }
}
```


如果上面显示的createContext（）方法返回null，您的代码应该创建一个OpenGL ES 2.0上下文，而不是仅使用该API。




以下代码示例演示如何通过首先创建最小支持的上下文来检查OpenGL ES版本，然后检查版本字符串：




```java
// Create a minimum supported OpenGL ES context, then check:
String version = javax.microedition.khronos.opengles.GL10.glGetString(
        GL10.GL_VERSION);
Log.w(TAG, "Version: " + version );
// The version format is displayed as: "OpenGL ES <major>.<minor>"
// followed by optional content provided by the implementation.
```


使用这种方法，如果您发现设备支持更高级的API版本，则必须销毁最低的OpenGL ES上下文，并使用更高的可用API版本创建新的上下文。







五、选择OpenGL API版本

OpenGL ES 1.0 API版本（和1.1扩展），版本2.0和3.0都提供了高性能的图形界面，用于创建3D游戏，可视化和用户界面。 OpenGL ES 2.0和3.0的图形编程基本相似，版本3.0代表了具有附加功能的2.0 API的超集。 OpenGL ES 1.0 / 1.1 API与OpenGL
 ES 2.0和3.0的编程显着不同，所以在开始使用这些API之前，开发人员应该仔细考虑以下因素：


1、性能 - 通常，OpenGL ES 2.0和3.0提供比ES 1.0 / 1.1 API更快的图形性能。 然而，由于OpenGL ES图形管道的硬件制造商的实现方面的差异，性能差异可能因OpenGL应用程序运行的Android设备而异。




2、设备兼容性 - 开发人员应考虑其客户的类型，Android版本和OpenGL ES版本。 有关跨设备的OpenGL兼容性的更多信息，请参阅OpenGL版本和设备兼容性部分。




3、编码方便 - OpenGL ES 1.0 / 1.1 API提供了OpenGL ES 2.0或3.0 API中不可用的固定功能管道和便利功能。 OpenGL ES新手的开发人员可能会发现版本1.0 / 1.1的编码更快更方便。




4、图形控制 - OpenGL ES 2.0和3.0 API通过使用着色器提供完全可编程的管道来提供更高程度的控制。 通过更直接地控制图形处理流程，开发人员可以创建使用1.0 / 1.1 API非常难以生成的效果。




5、纹理支持 - OpenGL ES 3.0 API对纹理压缩有最好的支持，因为它保证了支持透明度的ETC2压缩格式的可用性。 1.x和2.0 API实现通常包括对ETC1的支持，但是这种纹理格式不支持透明度，因此您通常必须以您要定位的设备支持的其他压缩格式提供资源。 有关详细信息，请参阅纹理压缩支持。




虽然性能，兼容性，便利性，控制和其他因素可能会影响您的决定，您应该根据您的想法为您的用户提供最佳体验选择一个OpenGL API版本。




