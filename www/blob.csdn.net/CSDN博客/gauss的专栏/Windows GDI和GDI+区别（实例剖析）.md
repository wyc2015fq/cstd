# Windows GDI和GDI+区别（实例剖析） - gauss的专栏 - CSDN博客
2013年01月12日 08:42:27[gauss](https://me.csdn.net/mathlmx)阅读数：573
个人分类：[图形与图像处理](https://blog.csdn.net/mathlmx/article/category/605907)

**1、概述**
GDI在全称是Graphics Device Interface，即图形设备接口。是图形显示与实际物理设备之间的桥梁。GDI接口是基于函数，虽然使程序员省力不少，但是编程方式依然显得麻烦。例如显示一张位图，我们需要进行“创建位图，读取位图文件信息，启用场景设备，调色板变化“等一系列操作。然而有了GDI+，繁琐的步骤再次被简化。顾名思义，GDI+就是GDI的增强版，它是微软在Windows 2000以后操作系统中提供的新接口。
**2、GDI+主要功能**
GDI+主要提供以下三种功能：
(1) 二维矢量图形：GDI+提供了存储图形基元自身信息的类(或结构体)、存储图形基元绘制方式信息的类以及实际进行绘制的类；
(2) 图像处理：大多数图片都难以划定为直线和曲线的集合，无法使用二维矢量图形方式进行处理。因此，GDI+为我们提供了Bitmap、Image等类,它们可用于显示、操作和保存BMP、JPG、GIF等图像格式。
(3) 文字显示：GDI+支持使用各种字体、字号和样式来显示文本。
相比于GDI，GDI+是基于C++类的对象化的应用程序接口，因此用起来更为简单。GDI的核心是设备上下文，GDI函数都依赖于设备上下文句柄，其编程方式是基于句柄的；GDI+无需时刻依赖于句柄或设备上下文，用户只需创建一个Graphics 对象，就可以用面向对象的方式调用其成员函数进行图形操作，编程方式是基于对象的。
**3、GDI绘制实例**
GDI在使用设备上下文绘制线条之前，必须先调用SelectObject 以使笔对象和设备上下文关联。其后，在设备上下文中绘制的所有线条均使用该笔，直到选择另一支不同的笔为止。
使用GDI画线代码如下
`1.``// TODO: Add your command handler code here`
`2.`
```
CClientDC clientDC;
```
`//目标DC     `
`3.``　  CPen pen (PS_SOLID, 1, RGB(0, 0, 255));`
`4.``　  clientDC.SelectObject(pen.GetSafeHandle());`
`5.``//开始绘制`
`6.``　  clientDC.MoveTo(0, 0)`
`7.``　  clientDC.LineTo(rect.right, 0);`
`8.``　  clientDC.SelectObject(oldObject);`
从上述代码可以看出：在GDI编程中，几乎所有的操作都围绕设备上下文dc展开。的确，这正是GDI编程的特点！设备上下文是Windows 使用的一种结构，所有GDI操作前都需取得特定设备的上下文，函数中的CClientDC dc (this) 语句完成这一功能。
利用GDI进行图形、图像处理的一般操作步骤为：1. 取得指定窗口的DC。2. 确定使用的坐标系及映射方式。3. 进行图形、图像或文字处理。4. 释放所使用的DC。但是，在GDI+中，只需将Pen对象直接作为参数传递给Graphics类的DrawLine等方法即可，而不必使Pen对象与Graphics对象关联。
**4、GDI+绘制实例**
使用GDI+画线代码如下
`01.`
```
// TODO: Add your command handler code here
```
`02.``　CClientDC clientDC (``this`
```
);
```
`03.``　``//创建Graphics对象`
`04.``　Graphics graphics(clientDC);`
`05.``　``//创建pen`
`06.``　Pen myPen;`
`07.``　myPen.SetWidth(1);`
`08.``　``//画X轴`
`09.``　myPen.SetColor(Color::Blue);`
`10.``　graphics.DrawLine(&myPen, 0, 0, rect.right, 0);`
(1)创建 Graphics 对象：Graphics 对象表示GDI+绘图表面，是用于创建图形图像的对象。
(2)使用 Graphics 对象绘制线条和形状、呈现文本或显示与操作图像。
GDI+的相对与GDI而言，新增了一系列功能：渐变的画刷(Gradient Brushes)、基数样条函数(Cardinal Splines)、持久的路径对象(Persistent Path Objects)、变形和矩阵对象(Transformations &Matrix Object)、可伸缩区域(Scalable Regions)、Alpha混合(Alpha Blending)和丰富的图像格式支持等。下面，我们来逐个用实际代码实现GDI+的新增功能。
**4.1渐变的画刷**
(GDI+提供了用于填充图形、路径和区域的线性渐变画刷和路径渐变画刷。线性渐变画刷使用渐变颜色来填充图形。当用路径渐变画刷填充图形时，可指定从图形的一部分移至另一部分时画刷颜色的变化方式。例如，我们可以只指定图形的中心颜色和边缘颜色，当画刷从图形中间向外边缘移动时，画刷会逐渐从中心颜色变化到边缘颜色。 )
`01.``// TODO: Add your command handler code here`
`02.``CClientDC clientDC (``this``);`
`03.``CRect rect;`
`04.``GetClientRect(&rect);`
`05.``//创建Graphics对象`
`06.``Graphics graphics(clientDC);`
`07.``//创建渐变画刷`
`08.``LinearGradientBrush lgb(Point(0, 0), Point(rect.right, rect.bottom), Color::Blue, Color::Green);`
`09.``//填充`
`10.``graphics.FillRectangle(&lgb, 0, 0, rect.right, rect.bottom);`
**4.2基数样条函数**
(基数样条指的是一连串单独的曲线，这些曲线连接起来形成一条较大的曲线。样条由点(Point结构体)的数组指定，并通过该数组中的每一个点。基数样条平滑地穿过数组中的每一个点(不出现尖角)，因此比用直线连接创建的路径精确。)
`01.``// TODO: Add your command handler code here`
`02.``CClientDC clientDC (``this``);`
`03.``//创建Graphics对象`
`04.``Graphics graphics(clientDC);`
`05.``Point points[] =`
`06.``{`
`07.``　Point(0, 0), Point(100, 200), Point(200, 0), Point(300, 200), Point(400, 00)`
`08.``};`
`09.``//直接画线`
`10.``for``(``int`
```
i = 0; i < 4; i++)
```
`11.``{`
`12.``　graphics.DrawLine(&Pen(Color::Blue, 3), points[i], points[i + 1]);`
`13.``}`
`14.``//利用基数样条画线`
`15.``graphics.DrawCurve(&Pen(Color::Red, 3), points, 5);`
**4.3变形和矩阵对象**
(GDI+提供了Matrix对象，它是一种可以使变形(旋转、平移、缩放等) 简易灵活的强大工具，Matrix对象需与要被变形的对象联合使用。对于GraphicsPath类，我们可以使用其成员函数Transform接收Matrix参数用于变形。)
`01.``// TODO: Add your command handler code here`
`02.``　CClientDC clientDC (``this``);`
`03.``　``//创建Graphics对象`
`04.``　Graphics graphics(clientDC);`
`05.``　GraphicsPath path;`
`06.``　path.AddRectangle(Rect(250, 20, 70, 70));`
`07.`
```
graphics.DrawPath(&Pen(Color::Black, 1), &path);
```
`// 在应用变形矩阵之前绘制矩形`
`08.``　``// 路径变形`
`09.``　Matrix matrix1, matrix2;`
`10.`
`11.`
```
matrix1.Rotate(45.0f);
```
`//旋转顺时针45度`
`12.`
```
path.Transform(&matrix1);
```
`//应用变形`
`13.``　graphics.DrawPath(&Pen(Color::Red, 3), &path);`
`14.`
`15.`
```
matrix2.Scale(1.0f, 0.5f);
```
`//转化成为平行四边形法则`
`16.`
```
path.Transform(&matrix2);
```
`//应用变形`
`17.``　graphics.DrawPath(&Pen(Color::Blue, 3), &path);`
**4.4丰富的图像格式支持**
(GDI +提供了Image、Bitmap 和Metafile 类，方便用户进行图像格式的加载、操作和保存。GDI+支持的图像格式有BMP、GIF、JPEG、EXIF、PNG、TIFF、ICON、WMF、EMF等，几乎涵盖了所有的常用图像格式。)
`01.``// TODO: Add your command handler code here`
`02.``CClientDC clientDC (``this``);`
`03.``//创建Graphics对象`
`04.``Graphics graphics(clientDC);`
`05.`
```
Image image(L
```
`"[d://1.jpg]()"``);`
`06.``//在矩形区域内显示jpg图像`
`07.``Point destPoints1[3] =`
`08.``{`
`09.``　Point(10, 10), Point(220, 10), Point(10, 290)`
`10.`
```
};
```
`11.``graphics.DrawImage(&image, destPoints1, 3);`
`12.``//在平行四边形区域内显示jpg图像`
`13.``Point destPoints2[3] =`
`14.``{`
`15.``　Point(230, 10), Point(440, 10), Point(270, 290)`
`16.``};`
`17.``graphics.DrawImage(&image, destPoints2, 3);`
**4.5Alpha混合**
(Alpha允许将两个物体混合起来显示，在3D气氛和场景渲染等方面有广泛应用。它能"雾化"图像，使得一个图像着色在另一个半透明的图像上，呈现一种朦胧美。我们知道，一个像素可用R，G，B三个维度来表示，我们可以再加上第4个即：Alpha维度(channel)，表征透明程度。)
`01.``// TODO: Add your command handler code here`
`02.``　CClientDC clientDC (``this``);`
`03.``　``//创建Graphics对象`
`04.``　Graphics graphics(clientDC);`
`05.``　``//创建ColorMatrix`
`06.``　ColorMatrix ClrMatrix =`
`07.``　{`
`08.``　　1.0f, 0.0f, 0.0f, 0.0f, 0.0f,`
`09.``　　0.0f, 1.0f, 0.0f, 0.0f, 0.0f,`
`10.``　　0.0f, 0.0f, 1.0f, 0.0f, 0.0f,`
`11.``　　0.0f, 0.0f, 0.0f, 0.5f, 0.0f,`
`12.``　　0.0f, 0.0f, 0.0f, 0.0f, 1.0f`
`13.``　};`
`14.``　``//将ColorMatrix赋给ImageAttributes`
`15.``　ImageAttributes ImgAttr;　ImgAttr.SetColorMatrix(&ClrMatrix,ColorMatrixFlagsDefault,ColorAdjustTypeBitmap);`
`16.``　``//在矩形区域内显示jpg图像`
`17.`
```
Image img1(L
```
`"[d://1.jpg]()"``);`
`18.``　Point destPoints1[3] =`
`19.``　{`
`20.``　　Point(10, 10), Point(220, 10), Point(10, 290)`
`21.``　};`
`22.``　graphics.DrawImage(&img1, destPoints1, 3);`
`23.``　``//Alpha混合`
`24.`
```
Image img2(L
```
`"[d://2.jpg]()"``);`
`25.``　``int``width, height;`
`26.``　width = img2.GetWidth();`
`27.``　height = img2.GetHeight();`
`28.``　graphics.DrawImage(&img2, RectF(10, 10, 210, 280), 0, 0, width, height,UnitPixel, &ImgAttr);`
`29.``　``//在平行四边形区域内显示jpg图像`
`30.``　Point destPoints2[3] =`
`31.``　{`
`32.``　　Point(230, 10), Point(440, 10), Point(270, 290)`
`33.``　};`
`34.``　graphics.DrawImage(&img1, destPoints2, 3);`
`35.``　``//Alpha混合`
`36.``　graphics.DrawImage(&img2, destPoints2, 3,0,0,width,height,UnitPixel,&ImgAttr);`
为了能进行Alpha混合，我们需要使用ImageAttributes类和ColorMatrix矩阵，ImageAttributes可以进行颜色、灰度等调整从而达到控制图像着色方式的目的。ColorMatrix是ImageAttributes类大多数函数的参数，它包含了Alpha、Red、Green、Blue维度的值，以及另一维w，顺序为RGBaw。
**4.6文字输出**
(文字的颜色、字体、填充方式都可以直接作为Graphics类DrawString成员函数的参数进行设置)
`01.``// TODO: Add your command handler code here`
`02.``CClientDC clientDC (``this``);`
`03.``//创建Graphics对象`
`04.``Graphics graphics(clientDC);`
`05.``//创建20号"楷体"字体`
`06.`
```
FontFamily fontFamily1(L
```
`"楷体_GB2312"``); ``// 定义"楷体"字样`
`07.``Font  font1(&fontFamily1, 20, FontStyleRegular, UnitPoint);`
`08.``//定义输出UNICODE字符串`
`09.``WCHAR``string[256];`
`10.`
```
wcscpy(string, L
```
`" vckBase的网友朋友，您好！"``);`
`11.``//以蓝色画刷和20号"楷体"显示字符串`
`12.``graphics.DrawString(string, (``INT`
```
)wcslen(string),
 &font1, PointF(30, 10),&SolidBrush(Color::Blue));
```
`13.``//定义字符串显示画刷`
`14.``LinearGradientBrush linGrBrush(Point(30, 50), Point(100, 50), Color(255, 255,0, 0), Color(255, 0, 0, 255));`
`15.``//以线性渐变画刷和创建的20号"楷体"显示字符串`
`16.``graphics.DrawString(string, (``INT`
```
)wcslen(string),
 &font1, PointF(30, 50),&linGrBrush);
```
`17.``//创建20号"华文行楷"字体`
`18.`
```
FontFamily fontFamily2(L
```
`"华文行楷"``); ``// 定义"楷体"字样`
`19.``Font font2(&fontFamily2, 20, FontStyleRegular, UnitPoint);`
`20.``//以线性渐变画刷和20号"华文行楷"显示字符串`
`21.``graphics.DrawString(string, (``INT`
```
)wcslen(string),
 &font2, PointF(30, 90),&linGrBrush);
```
`22.``//以图像创建画刷`
`23.`
```
Image image(L
```
`"[d://3.jpg]()"``);`
`24.``TextureBrush tBrush(&image);`
`25.``//以图像画刷和20号"华文行楷"显示字符串`
`26.``graphics.DrawString(string, (``INT`
```
)wcslen(string),
 &font2, PointF(30, 130),&tBrush);
```
`27.``//创建25号"华文中宋"字体`
`28.`
```
FontFamily fontFamily3(L
```
`"华文中宋"``); ``// 定义"楷体"字样`
`29.``Font font3(&fontFamily2, 25, FontStyleRegular, UnitPoint);`
`30.``//以图像画刷和20号"华文行楷"显示字符串`
`31.``graphics.DrawString(string, (``INT`
```
)wcslen(string),
 &font3, PointF(30, 170),&tBrush);
```
**5、结论**
鉴于GDI+良好的易用性和其具有的强大功能，我们建议尽快抛弃GDI编程方式，因为我们没有必要将时间浪费在无意义的重复代码的设计上。GDI+对GDI的增强，某种意义上类似于MFC对Windows API的整理和封装。作为一种良好的"生产工具"，它必将大大地促进开发时的"生产力"。
