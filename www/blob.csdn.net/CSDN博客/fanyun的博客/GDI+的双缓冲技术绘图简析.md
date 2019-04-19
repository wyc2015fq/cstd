# GDI+的双缓冲技术绘图简析 - fanyun的博客 - CSDN博客
2016年11月08日 09:49:55[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：582
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
一、GDI+简介      
        Windows XP拥有全新的用户界面，其中主要的原因是采用了全新的图形界面引擎：GDI+。使用该图形系统，图形和艺术字将具有更平滑的轮廓以及可调整的用真实颜色调和的透明度级别，在调整图片大小时，图片的显示将更加清晰。GDI+是Windows 2000及以后版本的Windows系统中的一个子系统，现在微软在它的.NET Framework中也集成了GDI+。
二、GDI+编程基础
        同GDI一样，GDI+主要是负责对输出设备如显示器和打印机进行管理。GDI+提供了编程时使用的应用程序接口（API），通过这些API，GDI+让程序员在设计程序时不必考虑到具体的显示器或打印机，直接使用GDI+提供的类的方法便可以调用显示器或打印机，真正做到与设备无关。
        GDI+和GDI、DirectX一样，三者都是作为开发图形应用程序的一种方式，GDI+实际上是对GDI的高层次的封装。二者的主要功能都是服务于图形、图像和文本的输出，唯一不同的是，GDI+在功能上要强大些。另外一个在编程应用上的本质区别是GDI+用一个无状态的模型取代了GDI中把选中项目放到设备环境（DC）对象上的主要状态模型，每个绘图操作都是相互独立的，Graphics对象是绘图操作中唯一保留的对象。
        熟悉GDI编程的人对设备环境（devicecontext：DC）不会陌生，DC的数据结构存储了与显示器的性能、属性之类的详细信息，这些数据信息决定了程序如何在显示器上进行输出。所有的输出都是通过DC来实现的，比如想在窗口中画一个矩形，首先要通过GetDC得到DC，然后让DC去执行具体操作。
       使用GDI+之后，再也没有必要去考虑句柄、设备环境的概念了。只需要简单的创建一个图形对象（Graphics object），然后直接调用该对象的方法（methods）进行绘图即可。图形对象和DC有许多相似的地方，在使用上遵循着相同的使用规则，但其有着本质上的区别：一个是基于句柄的GDI，一个是基于对象模型的GDI+。
       同DC一样，图形对象也是和具体的窗口或屏幕结合在一起的，图形对象中也包括了许多具体属性，这些具体的属性值指明了进行绘图的方法。图形对象中没有了在DC中所绑定的具体的画笔、画刷、路径之类的对象。在GDI中绘图，首先要为DC选入一个画笔对象（而且，只有一支画笔是可以使用的），这支画笔一直使用到下次新选定画笔为止。在GDI+中，只需要在图形对象中调用画直线时将画笔作为画直线的一个参数就可以了，也就是说，可以同时使用多个画笔进行不同的绘图操作。
下面是两段画直线的实例，分别使用GDI和GDI+编程。使用GDI编程画直线：
```cpp
HPEN hRedPen,hBluePen,hPenOld;
hRedPen = CreatePen(PS_SOLID,3,RGB(255,0,0));
hBluePen =CreatePen(PS_SOLID,3,RGB(0,0,255));
CDC* pDC = GetDC();
hPenOld =(HPEN)pDC->SelectObject(hRedPen);//选入红色画笔对象
pDC->MoveTo(50,50);
pDC->LineTo(400,50);
pDC->SelectObject(hPenOld);//释放红色画笔对象
hPenOld = (HPEN)pDC->SelectObject(hBluePen);//选入蓝色画笔对象
pDC->MoveTo(50,60);
pDC->LineTo(400,60);
pDC->SelectObject(hPenOld);//释放蓝色画笔对象
使用GDI+编程画直线：
CDC* pDC=GetDC();
Graphics gr(pDC->m_hDC);
Pen red(Color(255,255,0,0),3);
Pen blue(Color(255,0,0,255),3);
gr.DrawLine(&red,50,100,400,100);//使用红色画笔画线
gr.DrawLine(&blue,50,150,400,150);//使用蓝色画笔画线
```
         从上面的两个例子可以看出，GDI+对画笔（以及其他对象）的管理更具灵活性。在GDI+的实例中，使用了Graphics类，它是使用GDI+编程时的一个基本类，使用它可以进行绘制文本、填充矩形、椭圆以及绘制线条等形状。在上面的代码中，使用图形对象画了一条红色直线后，紧接着用蓝色画笔画了一条直线，如果使用GDI编程实现，需要的步骤就是“选入蓝色画笔－>画线->释放蓝色画笔对象”。
        上例中还使用了颜色对象Color，它的原型为：
Color(BYTE a, BYTE r, BYTE g, BYTE b)
        其中，参数a指定了颜色的透明度，可以任意设置绘制图形的透明度，这在GDI中是相当麻烦的，而在GDI+中只是改变一个参数而已，所以相比之下，GDI+让编程变得更简单灵活。
        在使用GDI+编程之前要确保系统中有运行GDI+程序所必须具备的软件环境：GDIplus.dll。微软在新推出的.NETFramework中集成了GDI+，如果安装了VS.NET，就可以直接使用GDI+进行编程。如果还没有升级到.NET，可以去微软网站下载最新的Platform SDK，SDK为GDI+提供了头文件及库文件，它支持我们在VC++6.0中使用GDI+。
