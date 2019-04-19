# QPainter类使用详解 - fanyun的博客 - CSDN博客
2016年12月25日 21:35:40[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：10247标签：[Qt](https://so.csdn.net/so/search/s.do?q=Qt&t=blog)
个人分类：[Qt](https://blog.csdn.net/fanyun_01/article/category/6429937)
          QPainter类提供了许多高度优化的函数去做大部分的GUI绘制工作。它可以画从简单的线到复杂的形状。通常情况下，QPainter的使用是在widget当中的painter事件使用。记得在执行完之后要及时销毁QPainter的对象。
voidSimpleExampleWidget::paintEvent(QPaintEvent *)
 {
    QPainter painter(this);
    painter.setPen(Qt::blue);
     painter.setFont(QFont("Arial",30));
    painter.drawText(rect(), Qt::AlignCenter, "Qt");
 }
你可以使用painter不同的composition mode将图片结合在一起。
一个painter被通过begin（）函数被激活并且使用一个QPainterDevice参数的构造函数进行构造，调用end（）函数和析构函数解除
激活。
结合QPaintDevice和QPaintEngine，我们知道，QPainter执行的是一个画图的操作，QPaintDevice代表的是能被QPainter画上去
的设备。
QPaintEngine提供接口使得QPainter能够画在不同类型的设备上。当painter被激活的时候，device（）函数返回paint设备，paint
Engine（）返回一个当前painter正在绘制的engine。
有时候在一个特殊的QPaintDevice上绘制也是可取的，QPainter提供了这样的函数setRedirected()来做这件事。
当被绘制的设备是一个widget的时候，QPainter只能在paintEvent（）函数中或者被该函数调用的函数当中使用，除非[Qt::WA_](http://blog.csdn.net/madao123/article/details/qt.html#WidgetAttribute-enum)[Paint](http://blog.csdn.net/madao123/article/details/qt.html#WidgetAttribute-enum)
[OutsidePaintEvent](http://blog.csdn.net/madao123/article/details/qt.html#WidgetAttribute-enum)  这个widget属性被设置。而在macosx或者windows当中，无论该属性有没有设置，painter都必须在paint
Event（）函数当中使用。
Settings
QPainter有许多的设置可以用来满足你的个人偏爱。
font（）：字体设置。
brush（）：画刷设置，定义颜色或者模式（pattern）来填充形状。
pen（）：定义颜色和点描来绘制线或者边框。
backgroundMode（）：来表示是否有一个background（）函数。
background（）： 只在backgroundMode（）为Qt::OpaqueMode和 pen（）是点描的时候有效。
brushOrigin（）：定义使用的刷子的来源，通常与widget的背景保持一致。
viewport（），window（），worldTransform（）用来转换坐标变换。
hasClipping（）：表示painter是否绑定在某个设备上，该设备也被绑定。
layoutDirection（）：当绘制文本的时候定义布局方向。
worldMatrixEnable（），viewTransformEnable()描述转换坐标是否允许。
我们可以随时使用save（）函数来保存QPainter的状态，并且用restore（）函数来使他们回退。
Drawing
[drawPoint](http://blog.csdn.net/madao123/article/details/qpainter.html#drawPoint)(),[drawPoints](http://blog.csdn.net/madao123/article/details/qpainter.html#drawPoints)(), [drawLine](http://blog.csdn.net/madao123/article/details/qpainter.html#drawLine)(), [drawRect](http://blog.csdn.net/madao123/article/details/qpainter.html#drawRect)(), [drawRoundedRect](http://blog.csdn.net/madao123/article/details/qpainter.html#drawRoundedRect)(),[drawEllipse](http://blog.csdn.net/madao123/article/details/qpainter.html#drawEllipse)(), [drawArc](http://blog.csdn.net/madao123/article/details/qpainter.html#drawArc)(), [drawPie](http://blog.csdn.net/madao123/article/details/qpainter.html#drawPie)(), 
[drawChord](http://blog.csdn.net/madao123/article/details/qpainter.html#drawChord)(), [drawPolyline](http://blog.csdn.net/madao123/article/details/qpainter.html#drawPolyline)(),[drawPolygon](http://blog.csdn.net/madao123/article/details/qpainter.html#drawPolygon)(), [drawConvexPolygon](http://blog.csdn.net/madao123/article/details/qpainter.html#drawConvexPolygon)()
 和 [drawCubicBezier](http://blog.csdn.net/madao123/article/details/qpainter-qt3.html#drawCubicBezier)().
QPainter类也提供了fillRect（）函数用来调用QBrush填充提供的QRect。
如果你需要画一些很复杂的图形，或者需要做一些重复性的工作，可以考虑建立一个QPainterPath并且使用drawPath（）来调用。
QPainter也提供了fillPath（）和stokePath来绘制外形。
在QPainterPath上使用矢量绘图技术，可以做出许多特殊的图形，也可以使用自定义的patterns来绘制自己的图形。
～有许多的函数可以用来画图（pixmaps/images），我们称之为drawPixmap（），drawImage（）和drawTiledPixmap（）。
drawText（）绘制文本，drawPicture（）会忽视所有的painter设置，而使用picture自己的设置。
Rendering Quality渲染质量
使用QPainter能够优化渲染结果，QPainter：：Antialiasing用来反锯齿等。
Coordinate Transformations坐标转换
最常用的坐标转换有放大缩小，旋转，剪切，移动等，scale（），rotate（），translate（），shear（）函数。
Compositon Mode复合方式
一个像素覆盖在另外一个像素上面的时候的复合方式。常用的两个复合方式是Source和SourceOver，前者用于在设备上绘制不透明的对象，
后者来说，源是透明的并且画在目标像素之上。
注意到复合转换在于对像素的聪明操作，在图像上和图像矩形范围内就有了区别，在矩形边框上的像素alpha=0的话，这些像素就会覆盖在其
他图像的像素之上。
Limitations
用qt基于光栅的坐标系，+/-（2^15）能用。应为用于表示坐标长的是的short int类型。
当用qt的stoker生成的弯曲曲线的外形时，仅仅是一种近似。
Performance
QPainter是一个丰富的框架，允许开发者做许许多多的图像操作，比如倾斜度，复合模式，矢量绘图等，并且QPainter能够这样子在许多的
不相同的硬件和软件栈之上工作。自然的，软件和硬件的基本组合有许多对性能的影响。要使得在这一系列的组合当中所有的绘图操作都尽可
能的快几乎是不可能的，因为需要巨大的工作量，所以，折中的方式是我们选择了一些QPainter API和后端的子集，确保能够在给定的软硬件
组合上尽可能好的使用。
那些被我们所关注的高性能的引擎有：
Raster：该后端在纯软件中实现了所有的渲染，也经常被用在QImages当中渲染，具有优化性能的包括 [QImage::Format_ARGB32_](http://blog.csdn.net/madao123/article/details/qimage.html#Format-enum)[Pre](http://blog.csdn.net/madao123/article/details/qimage.html#Format-enum)
[multiplied](http://blog.csdn.net/madao123/article/details/qimage.html#Format-enum),[QImage::Format_RGB32](http://blog.csdn.net/madao123/article/details/qimage.html#Format-enum)或者 [QImage::Format_RGB16](http://blog.csdn.net/madao123/article/details/qimage.html#Format-enum)，其他的格式化，在性能上都会显著恶化。
OpenGL 2.0（ES）：这是图像硬件加速的主要后端。他能够在支持OpenGL2.0或者OpenGL/ES2.0规范的设备当中或者桌面系统当运
行。该规范得到了最近几年大部分的图形芯片制造厂商的支持。引擎支持QGLWidget上使用QPainter或者输入在底层的系统中输入
-graphicssystem opengl。
OpenVG：支持OpenVG引擎。
这些操作包括:简单的转换，移动，缩放，或者直角的旋转。
drawPixmap（）：在软硬件组合上不透明的，飞光滑转换方式的简单变换。
矩形的实色填充，双色渐变和简单的线性变换。矩形的裁剪。像素组合方式。等
Menber Function Documentation（我挑几个重要的函数翻译）
QPainter：：QPainter（）
构造函数。
QPainter：：QPainter（QPainterDevice*device)
构造一个painter开始在设备上进行绘制。
void MyWidget::paintEvent(QPaintEvent *)
 {
    QPainter p;
    p.begin(this);
    p.drawLine(...);        // drawingcode
    p.end();
/*和QPainter p（this）相同。*/
 }
bool QPainter::begin(QPaintDevice *device)
使用该函数时要注意以下情况：
painter->begin(0); // 绘制设备不可能为0；
 QPixmap image(0, 0);
 painter->begin(&image); // 图像为空，这句话错误
 painter->begin(myWidget);
 painter2->begin(myWidget); //同一个时间内不能够出现两个painter对象。
void QPainter::drawConvexPolygon(constQPointF *points,int pointCount)
绘制多边形函数。
在一些平台上，drawConvexPlygon（）函数能够比drawPloygon（）函数更加快速。
void QPainter::drawEllipse(constQRectF&rectangle)
绘制椭圆。
void QPainter::drawImage(const QRectF&target,const QImage &image,const QRectF&source,Qt::ImageConversion-
Flag flags=Qt::AutoColor)
把矩形部分源的画画到目标的矩形之中。
该函数有许多不相同的构造函数，比如void QPainter::drawImage(const QRectF &rectangle,const QImage&image)
该函数图片会知道所给的矩形当中，并且图片会自己缩放以满足矩形框的大小（除非额外设定）。
类似于上面的函数还有drawPixmap（）。
void QPainter::drawPoint();画点函数。voidQPainter::drawRects(）画矩形。void QPainter::drawText()画文本
等等。在对Qt裁剪的过场当中，要对Qt深入的了解。
