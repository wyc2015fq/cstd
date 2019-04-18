# QT+VTK 对接使用 - wishchinYang的专栏 - CSDN博客
2013年10月16日 21:08:42[wishchin](https://me.csdn.net/wishchin)阅读数：3023
### 由于MFC和pcl的不兼容问题，只能用QT和VTK进行程序开发，确实是一件蛋疼的事！
出自于QT与VTK结合系列：[http://blog.csdn.net/tonylk/article/details/341068](http://blog.csdn.net/tonylk/article/details/341068)
这个教程非常详细.
### [QT与VTK的结合开发](http://blog.csdn.net/tonylk/article/details/341068)
原文链接：[http://blog.csdn.net/tonylk/article/details/341068](http://blog.csdn.net/tonylk/article/details/341068)
         VTK全名是VisualizationToolKit，它是一套跨平台的C＋＋库，对OpenGL作了较全面的封装，将各种三维模型的存储，运算，显示，交互等内容全都以类的方式封装起来了，并且提供比OpenGL强大得多的功能。可惜VTK最大的缺点是，没有免费的教程，它只提供免费的API手册，其中只是对各个类的功能罗列而已，而参考书籍则需要花几十美元去购买，所以学习VTK只能依靠它的大量Example了。
        由于我的项目需要兼故未来跨平台的可能（目前只在Windows下），所以必须使用一套跨平台的开发库，在VTK/Examples/GUI的例子里，Windows平台下只有SDK，MFC，C++ Builder这几个例子，另外还有Motif，Python，Tcl，选择是不少，但是Motif听说编程比较麻烦，我也从来没有接触过，Tcl和Python作为脚本语言，其性能和安全性则有些令人担忧，也就是说，这里面没有一个是我能使用的。。。说起跨平台，由于单位里项目的关系，我接触得比较多的就是QT了，所以，在未选定VTK之前，其实我是打算使用QT＋OpenGL的组合方式来开发这个软件的，其实，如果不考虑跨平台，我还是会选择QT，因为它的事件处理方式对于用惯Delphi的我而言，更为顺手，那么现在使用了VTK，是否还能将VTK和QT组合起来使用呢。。
         作为试验，我仿照VTK/Examples/GUI/Win32/SampleMFC，制作了以下这个小程序，很顺利，结果证明了我的猜想，QT和VTK是可以很方便的结合起来的，下面就跟我来一步步制作这个程序吧：
对于新建立的QT对话框程序，源码如下：
```cpp
#include <qapplication.h>
#include "testwindow.h"
int main(int argc, char** argv)
{
 QApplication app(argc, argv);
 TestWindow testwin;
 testwin.show();
 app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
 return app.exec();
}
```
其中的testwindow.h，是程序的主窗口TestWindow的头文件，一会将会建立该文件。
这段程序所做的是，初始化QApplication，并将TestWindow显示出来，并进入主程序的消息循环app.exec()。
下面实现TestWindow类，分别建立testwindow.h和testwindow.cpp
```cpp
testwindow.h:
#include <qmainwindow.h>
class TestWindow: public QMainWindow
{
 Q_OBJECT
public:
 TestWindow();
 ~TestWindow();
};
testwindow.cpp:
#include "testwindow.h"
#include "moc_testwindow.h"
TestWindow::TestWindow()
{
}
TestWindow::~TestWindow()
{
}
```
你是否注意到了testwindow.cpp中的#include "moc_testwindow.h"一行，这个moc_testwindow.h将会建立TestWindow的RTTI信息，并且绑定消息等，它并不需要我们自己实现，而是由qt的一个名为moc的程序建立，在VC的FileView中，右键点击testwindow.h，并选择Settings，然后在打开的Project Settings对话框中选择Custom Build页，在Commands中填入：
moc -i testwindow.h -o moc_testwindow.h
意思是调用moc程序，根据testwindow.h的内容，自动生成一个名为moc_testwindow.h的moc文件。
在Outputs中填入：
moc_testwindow.h
![](http://blog.csdn.net/images/blog_csdn_net/tonylk/50124/r_1.gif)
然后，在Project Settings中选中projct，在Link页的Object/library modules中加入：qt-mt334.lib。
![](http://blog.csdn.net/images/blog_csdn_net/tonylk/50124/r_2.gif)
编译程序，如果顺利，一个简单的QT程序就写好了，它会在启动后显示一个空白的窗口。
3）加入VTK的代码，这些代码都可以参考Examples/GUI/Win32/SampleMFC程序，将testwindow.h改造成如下：
```cpp
#include <qmainwindow.h>
#include "vtkRenderer.h"
#include "vtkWin32OpenGLRenderWindow.h"
#include "vtkWin32RenderWindowInteractor.h"
class TestWindow: public QMainWindow
{
 Q_OBJECT
public:
 TestWindow();
 ~TestWindow();
protected:
 virtual void paintEvent(QPaintEvent *);
 virtual bool winEvent(MSG *);
private:
 vtkRenderer *Renderer;
 vtkWin32OpenGLRenderWindow *RenderWindow;
 vtkWin32RenderWindowInteractor *Interactor;
};
```
testwindow.cpp改造成如下：
```cpp
#include "testwindow.h"
#include "moc_testwindow.h"
#include "vtkActor2D.h"
#include "vtkTextMapper.h"
#include "vtkTextProperty.h"
#include "vtkDataSetReader.h"
#include "vtkDataSetMapper.h"
#include "vtkCommand.h"
#include "vtkCamera.h"
#include "vtkWin32RenderWindowInteractor.h"
#include "vtkInteractorStyleTrackballCamera.h"
TestWindow::TestWindow()
{
 this->Renderer = vtkRenderer::New();
 this->Renderer->SetBackground(0.3, 0.5, 0.1);
 this->RenderWindow = vtkWin32OpenGLRenderWindow::New();
 this->RenderWindow->AddRenderer(this->Renderer);
 this->Interactor = vtkWin32RenderWindowInteractor::New();
 vtkActor2D *actor2d = vtkActor2D::New();
 vtkTextMapper *txt = vtkTextMapper::New();
 actor2d->SetMapper(txt);
 txt->SetInput("Hello World");
 txt->GetTextProperty()->SetFontSize(24);
 this->Renderer->AddProp(actor2d);
 txt->Delete();
 actor2d->Delete();
 vtkActor *actor = vtkActor::New();
 vtkDataSetReader *reader = vtkDataSetReader::New();
 reader->SetFileName("weldedSpheres.vtk");
 vtkDataSetMapper *mapper = vtkDataSetMapper::New();
 mapper->SetInput(reader->GetOutput());
 actor->SetMapper(mapper);
 this->Renderer->AddProp(actor);
 mapper->Delete();
 reader->Delete();
 actor->Delete();
}
TestWindow::~TestWindow()
{
 if (this->Interactor) {
  this->Interactor->Delete();
 }
 if (this->Renderer) {
  this->Renderer->SetRenderWindow(NULL);
 }
 if (this->RenderWindow) {
  this->RenderWindow->Delete();
 }
 if (this->Renderer) {
  this->Renderer->Delete();
 }
}
void TestWindow::paintEvent(QPaintEvent *e)
{
 if (! this->Interactor->GetInitialized()) {
  this->RenderWindow->SetWindowId(this->winId());
  this->RenderWindow->WindowInitialize();
  this->Interactor->SetRenderWindow(this->RenderWindow);
  this->Interactor->Initialize();
 }
 this->RenderWindow->Render();
}
bool TestWindow::winEvent(MSG *msg)
{
 switch (msg->message) {
  case WM_LBUTTONDOWN:
  case WM_LBUTTONUP:
  case WM_MBUTTONDOWN:
  case WM_MBUTTONUP:
  case WM_RBUTTONDOWN:
  case WM_RBUTTONUP:
  case WM_MOUSEMOVE:
  case WM_CHAR:
  case WM_TIMER:
   if (this->Interactor->GetInitialized()) {
    vtkHandleMessage2(msg->hwnd, msg->message, msg->lParam, msg->wParam, this->Interactor);
   }
 }
 return false;
}
```
其中用到了一个模型文件weldedSpheres.vtk，可以在VTK中找到。可能你需要修改它的路径。
然后，再次打开Proect Settings对话框，在C/C++页中，选择Category：Preprocessor,在Additional include directories:中加入：
D:/VTK,D:/VTK/Parallel,D:/VTK/Hybrid,D:/VTK/Patented,D:/VTK/Rendering,D:/VTK/IO,D:/VTK/Imaging,D:/VTK/Graphics,D:/VTK/Filtering,D:/VTK/Common
![](http://blog.csdn.net/images/blog_csdn_net/tonylk/50124/r_3.gif)
选择Link页，选择Category：Input，在Object/library modules:中加入：
vtkRendering.lib vtkGraphics.lib vtkImaging.lib vtkIO.lib vtkFiltering.lib vtkCommon.lib vtkftgl.lib glu32.lib opengl32.lib glu32.lib opengl32.lib vtkfreetype.lib vtkpng.lib vtktiff.lib vtkzlib.lib vtkjpeg.lib vtkexpat.lib
在Addtional library path中加入：
D:/VTK/bin/debug
![](http://blog.csdn.net/images/blog_csdn_net/tonylk/50124/r_4.gif)
以上都假设VTK安装在D盘下，如果你安装在其它目录，请修改以上的路径。
好了，重新编译程序，运行，你将看到如下所示的VTK界面。
![](http://blog.csdn.net/images/blog_csdn_net/tonylk/50124/r_5.gif)
以上程序只是VTK和QT结合的简单应用，你完全可以将VTK的显示窗封装在一个QTWidget中，然后将其显示在整个程序中的一个区域中。
