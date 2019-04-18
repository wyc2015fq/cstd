# Qt开发：Qt GUI模块——简介 - 一世豁然的专栏 - CSDN博客





2018年05月17日 08:16:25[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1680








Qt GUI模块为窗口系统集成，事件处理，OpenGL和OpenGL ES集成，2D图形，基本图像，字体和文本提供类。 这些类由Qt的用户界面技术内部使用，也可以直接使用，例如使用低级别的OpenGL ES图形API编写应用程序。

对于编写用户界面的应用程序开发人员来说，Qt提供了更高层次的API，比如Qt Quick，比Qt GUI模块中的启动器更合适。







一、概述

要包含模块类的定义，请使用以下指令：


```cpp
#include <QtGui>
```

如果您使用qmake构建项目，则默认包含Qt GUI。 要禁用Qt GUI，请将以下行添加到.pro文件中：
` QT -= gui`






二、Application Windows

Qt GUI模块中最重要的类是QGuiApplication和QWindow。 想要在屏幕上显示内容的Qt应用程序需要使用这些应用程序。 QGuiApplication包含主事件循环，其中来自窗口系统和其他源的所有事件都被处理和分派。 它还处理应用程序的初始化和最终化。


QWindow类表示底层窗口系统中的一个窗口。 它提供了许多虚拟函数来处理来自窗口系统的事件（QEvent），如触摸输入，曝光，焦点，击键和几何变化。








三、2D Graphics

Qt GUI模块包含2D图形，图像，字体和高级版式的类。

用曲面类型QSurface :: RasterSurface创建的QWindow可以与QBackingStore和QPainter，Qt高度优化的2D矢量图形API结合使用。 QPainter支持绘制线，多边形，矢量路径，图像和文本。 有关更多信息，请参见绘制系统和栅格窗口示例。


Qt可以使用QImage和QPixmap类加载和保存图像。 默认情况下，Qt支持最常见的图像格式，包括JPEG和PNG等。 用户可以通过QImageIOPlugin类添加对其他格式的支持。 有关更多信息，请参阅读写图像文件。

Qt中的排版是用QTextDocument完成的，QTextDocument使用QPainter API和Qt的字体类，主要是QFont。 可以使用喜欢更多低级别API的文本和字体处理的应用程序，例如QRawFont和QGlyphRun等。








四、OpenGL and OpenGL ES Integration

QWindow支持使用OpenGL和OpenGL ES进行渲染，具体取决于平台支持的内容。 通过将QWindow的表面类型设置为QSurface :: OpenGLSurface，使用QSurfaceFormat选择格式属性，然后创建QOpenGLContext来管理本机OpenGL上下文，可以启用OpenGL渲染。 另外，Qt还有QOpenGLPaintDevice，它可以使用OpenGL加速的QPainter渲染，以及简化OpenGL代码编写的便利类，并且隐藏扩展处理的复杂性以及OpenGL ES 2和桌面OpenGL之间的差异。 便利类包括QOpenGL函数，它允许应用程序使用桌面OpenGL上的所有OpenGL ES 2函数，而无需手动解析OpenGL函数指针，从而允许跨平台开发针对移动或嵌入式设备的应用程序，以及一些包装本机OpenGL的类 功能更简单的Qt API：

QOpenGLBuffer

QOpenGLFramebufferObject

QOpenGLShaderProgram



QOpenGLTexture

QOpenGLDebugLogger

QOpenGLTimerQuery

QOpenGLVertexArrayObject

最后，为了更好地支持OpenGL的新版本（3.0及更高版本），还提供了一个版本化的函数包装机制：QOpenGLFunction_N_N类的系列公开了给定OpenGL版本和配置文件中的所有函数，可以轻松开发 桌面应用程序依赖于现代桌面型OpenGL功能。


有关更多信息，请参阅OpenGL窗口示例。


Qt GUI模块还包含一些数学类，以帮助进行与3D图形相关的最常见的数学运算。 这些类包括QMatrix4x4，QVector4D和QQuaternion

使用QSurface :: OpenGLSurface创建的QWindow可以与QPainter和QOpenGLPaintDevice结合使用，通过牺牲一些视觉质量来让OpenGL硬件加速2D图形。








五、Qt GUI Prior to Qt 5.0

在Qt 5.0之前，Qt GUI模块是Qt中与图形用户界面有关的所有东西的整体容器，包括Qt构件集，项目视图，图形视图框架以及打印。 从Qt 5开始，这些类已被移至Qt Widgets模块。 打印已转移到Qt打印支持模块。 请注意，这些模块可以从Qt安装中排除。


Qt GUI现在只包含一小组启动器，这些启动器通常对所有图形应用程序都有用。







