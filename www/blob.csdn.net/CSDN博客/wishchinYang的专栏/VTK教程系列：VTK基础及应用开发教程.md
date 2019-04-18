# VTK教程系列：VTK基础及应用开发教程 - wishchinYang的专栏 - CSDN博客
2013年10月24日 13:31:01[wishchin](https://me.csdn.net/wishchin)阅读数：7337
        由于OpenCV不能使用，只能使用VTK库的图像处理库，暂时还没有找到其他可以全面替代的库；
       CSDN东灵工作室：[http://blog.csdn.net/www_doling_net/article/details/8763686](http://blog.csdn.net/www_doling_net/article/details/8763686)
复制其第一篇：
注：以后将陆续分享一些对经典图像、图形**算法**的介绍，
新浪微博： [@东灵工作室](http://weibo.com/dolingstudio)
邮箱：www_doling_net@163.com
ITK、VTK研究群：209215671
**系列一 《VTK基础及应用开发教程》**
**基础入门篇：**
1.        [从零开始学习VTK](http://blog.csdn.net/www_doling_net/article/details/8532492)
2.       [VTK编译安装](http://blog.csdn.net/www_doling_net/article/details/8532742)
3.        VTK基础概念
> 
1)      [VTK基础概念(1)](http://blog.csdn.net/www_doling_net/article/details/8536376)
> 
2)      [VTK基础概念(2)](http://blog.csdn.net/www_doling_net/article/details/8540007)
4.        VTK可视化管线
> 
1)      [VTK可视化管线(1)](http://blog.csdn.net/www_doling_net/article/details/8540242)
> 
2)      [VTK可视化管线(2)](http://blog.csdn.net/www_doling_net/article/details/8541436)
5.        VTK在图像处理中的应用
> 
1)      [VTK在图像处理中的应用(1)](http://blog.csdn.net/www_doling_net/article/details/8541534)
> 
2)      [VTK在图像处理中的应用(2)](http://blog.csdn.net/www_doling_net/article/details/8547317)
> 
3)      [VTK在图像处理中的应用(3)](http://blog.csdn.net/www_doling_net/article/details/8549717)
> 
4)      [VTK在图像处理中的应用(4)](http://blog.csdn.net/www_doling_net/article/details/8551523)
> 
5)      [VTK在图像处理中的应用(5)](http://blog.csdn.net/www_doling_net/article/details/8553153)
> 
6)      [VTK在图像处理中的应用(6)](http://blog.csdn.net/www_doling_net/article/details/8554210)
> 
7)      [VTK在图像处理中的应用(7)](http://blog.csdn.net/www_doling_net/article/details/8560298)
> 
8)      [VTK在图像处理中的应用(8)](http://blog.csdn.net/www_doling_net/article/details/8560401)
> 
9)      [VTK在图像处理中的应用(9)](http://blog.csdn.net/www_doling_net/article/details/8560491)
6.        VTK基本数据结构
> 
1)      [VTK基本数据结构(1)](http://blog.csdn.net/www_doling_net/article/details/8562826)
> 
2)      [VTK基本数据结构(2)](http://blog.csdn.net/www_doling_net/article/details/8566866)
> 
3)      [VTK基本数据结构(3)](http://blog.csdn.net/www_doling_net/article/details/8567120)
> 
4)      [VTK基本数据结构(4)](http://blog.csdn.net/www_doling_net/article/details/8567520)
> 
5)     [VTK基本数据结构(5)](http://blog.csdn.net/www_doling_net/article/details/8589273)
**高阶应用篇：**
7.      [基于VTK的Qt应用程序开发](http://blog.csdn.net/www_doling_net/article/details/8668870)
8.      [基于VTK&ITK的Qt应用程序开发](http://blog.csdn.net/www_doling_net/article/details/8709741)
9.      基于VTK的MFC应用程序开发
1)      [基于VTK的MFC应用程序开发(1)](http://blog.csdn.net/www_doling_net/article/details/8714804)
2)      [基于VTK的MFC应用程序开发(2)](http://blog.csdn.net/www_doling_net/article/details/8735976)
3)      [基于VTK的MFC应用程序开发(3)](http://blog.csdn.net/www_doling_net/article/details/8939115)
### [基于VTK的Qt应用程序开发](http://blog.csdn.net/www_doling_net/article/details/8668870)
分类： [VTK应用示例](http://blog.csdn.net/www_doling_net/article/category/1346842)2013-03-13
 15:511916人阅读[评论](http://blog.csdn.net/www_doling_net/article/details/8668870#comments)(23)[收藏](http://blog.csdn.net/www_doling_net/article/details/8668870)[举报](http://blog.csdn.net/www_doling_net/article/details/8668870#report)
[VTK](http://blog.csdn.net/tag/details.html?tag=VTK)[Qt](http://blog.csdn.net/tag/details.html?tag=Qt)[CMake](http://blog.csdn.net/tag/details.html?tag=CMake)
目录[(?)](http://blog.csdn.net/www_doling_net/article/details/8668870#)[[+]](http://blog.csdn.net/www_doling_net/article/details/8668870#)
VTK附带的程序示例中大多是基于控制台的，作为可视化开发工具包，VTK也可以与很多流行的GUI开发工具整合，比如MFC、Qt(题外话：Qt已经被Digia从诺基亚手中收购了，Qt现在的链接是：[http://qt-project.org/](http://qt-project.org/)，也有已经编译好的版本：[http://code.google.com/p/qt-msvc-installer/downloads/list](http://code.google.com/p/qt-msvc-installer/downloads/list)直接下载安装。可能因为大学课程里会教授MFC的内容，一些非计算机专业的会偏向于采用MFC，个人觉得，对于非计算机专业而言，如果一定要选择一种GUI工具做开发的话，建议用Qt，容易上手，学习周期短)、FLTK([http://www.fltk.org/](http://www.fltk.org/)，FLTK也是跨平台的，是一种比较轻便的GUI工具，VTK官方发布版本没有提供对FLTK的接口，但可以借助类[vtkFlRenderWindowInteractor](http://cpbotha.net/software/vtkflrenderwindowinteractor/)，来实现VTK与FLTK的整合)等等，VTK的源码目录里(VTK-5.10\Examples\GUI)包含有VTK与Qt、MFC、Tcl等工具的整合。考虑到VTK对Qt的特殊照顾(VTK提供了很多针对Qt的类可以非常方便地与Qt整合)，以及Qt自身的一些性质(如易用性、跨平台等)，我们参考了VTK自带的一些例子，给出了VTK与Qt整合的详细步骤。
## 1.   CMakeLists.txt文件
我们已经知道了VTK工程的管理是用CMake的，而Qt自身有qmake工具，如果对于一些小工程而言，单纯的Qt程序用qmake来构建工程，确实很方便，但如果随着工程复杂度的增加以及工程依赖其他的函数库时，使用CMake来管理工程或许是一个明智的选择。而且随着你对CMake语法的了解，你会发现用CMake来管理工程是一件非常棒的事情。
我们先看看对于单纯的Qt工程，怎么来写CMakeLists.txt脚本文件。
### 1.1 用CMake来管理Qt工程
官方对于这个话题给出的解释在***[这里](http://qt-project.org/quarterly/view/using_cmake_to_build_qt_projects)***。我们引用一下这篇博文的图，然后给出每句CMakeLists.txt脚本的注释，结合这个图以及脚本的注释，相信你应该能明白了。
![](https://img-my.csdn.net/uploads/201303/13/1363161153_1026.png)
**[plain]**[view
 plain](http://blog.csdn.net/www_doling_net/article/details/8668870#)[copy](http://blog.csdn.net/www_doling_net/article/details/8668870#)
- #----------------------------------------------  
- # 下面这两行，没什么好解释的  
- cmake_minimum_required( VERSION 2.8 )  
- project( YourProjectName )  
- 
- #----------------------------------------------  
- # 下面这两行，也没什么好解释的  
- find_package( Qt4 REQUIRED )  
- include( ${QT_USE_FILE} )  
- 
- #----------------------------------------------  
- # 程序所有源文件。<TODO:在此处添加源文件>  
- # 定义变量Project_SRCS，其值为所列的文件列表  
- SET( Project_SRCS  
-     main.cpp  
-   )  
- 
- #----------------------------------------------  
- # 程序所有UI文件。<TODO:在此处添加UI文件>  
- # 定义变量Project_UIS，其值为所列的文件列表  
- SET( Project_UIS  
-     YourQtWindows.ui  
- )  
- 
- #----------------------------------------------  
- # 所有包含Q_OBJECT的头文件。<TODO:在此处添加头文件>  
- # 定义变量Project_MOC_HDRS，其值为所列的文件列表  
- SET( Project_MOC_HDRS  
-     YourQtProjectFiles.h  
- )  
- 
- #-----------------------------------------------  
- # 通过Qt的uic.exe生成UI文件对应的ui_XXXX.h文件  
- # 将生成的ui_XXXX.h文件放在变量Project_UIS_H里，  
- # QT4_WRAP_UI就是干这个事情。  
- QT4_WRAP_UI( Project_UIS_H ${Project_UIS} )  
- 
- #-----------------------------------------------  
- # 通过Qt的moc.exe生成包含Q_OBJECT的头文件对应的  
- # moc_XXXX.cxx文件，将生成的moc_XXXX.cxx文件放在  
- # 变量Project_MOC_SRCS里。QT4_WRAP_CPP就是干这个事情。  
- QT4_WRAP_CPP( Project_MOC_SRCS ${Project_MOC_HDRS} )  
- 
- #-----------------------------------------------  
- # Qt的MOC和UIC程序生成的moc_XXXX.cxx和ui_XXXX.h  
- # 等文件是存放在CMake的“Where to build the binaries"  
- # 里指定的目录里，所以必须都这些路径包含进来。  
- INCLUDE_DIRECTORIES( ${Project_SOURCE_DIR}  
-                      ${CMAKE_CURRENT_BINARY_DIR}  
-                    )  
- 
- #-----------------------------------------------                            
- # Qt程序如果有资源文件(*.qrc)，要包含资源文件，  
- # 然后用Qt的rcc.exe生成相应的qrc_XXXX.cpp文件。  
- # QT4_ADD_RESOURCES就是干这个事情。  
- SET( Project_RCCS YourProject.qrc)  
- QT4_ADD_RESOURCES( Project_RCC_SRCS ${Project_RCCS})  
- 
- #-----------------------------------------------  
- # 根据程序的cpp文件、头文件以及中间生成的ui_XXXX.h、  
- # moc_XXXX.cxx、qrc_XXXX.cxx等生成可执行文件，并链接  
- # Qt的动态库(Qt的动态库都定义在QT_LIBRARIES变量里了)  
- ADD_EXECUTABLE( YourProjectName  
-                 ${Project_SRCS}  
-                 ${Project_UIS_H}  
-                 ${Project_MOC_SRCS}  
-                 ${Project_RCC_SRCS}                             
-               )  
- TARGET_LINK_LIBRARIES ( YourProjectName ${QT_LIBRARIES} )  
### 1.2 用CMake来管理Qt与VTK工程
我们在上面的基础上添加VTK相关的CMake脚本文件，如下：
**[plain]**[view
 plain](http://blog.csdn.net/www_doling_net/article/details/8668870#)[copy](http://blog.csdn.net/www_doling_net/article/details/8668870#)
- #----------------------------------------------------------------------------------  
- cmake_minimum_required( VERSION 2.8 )  
- project( CombineQtAndVTK )  
- 
- #----------------------------------------------------------------------------------  
- find_package( VTK REQUIRED )  
- find_package( Qt4 REQUIRED )  
- 
- include( ${VTK_USE_FILE} )  
- include( ${QT_USE_FILE} )  
- 
- #----------------------------------------------------------------------------------  
- SET( PROJECT_SRCS  
-     main.cpp  
-     ProjectMainWindow.cpp  
-     )  
- 
- SET( PROJECT_UIS  
-     ProjectMainWindow.ui  
- )  
- 
- SET( PROJECT_MOC_HDRS  
-   ProjectMainWindow.h  
- )  
- 
- #----------------------------------------------------------------------------------  
- QT4_WRAP_UI( PROJECT_UIS_H  
-              ${PROJECT_UIS}  
-            )  
- 
- QT4_WRAP_CPP( PROJECT_MOC_SRCS  
-               ${PROJECT_MOC_HDRS}  
-             )  
- 
- #----------------------------------------------------------------------------------  
- INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR}  
-                      ${CMAKE_CURRENT_BINARY_DIR}  
-                      ${VTK_DIR}  
-                    )  
- 
- ADD_EXECUTABLE( CombineQtAndVTK  
-                 ${PROJECT_SRCS}  
-                 ${PROJECT_UIS_H}  
-                 ${PROJECT_MOC_SRCS}  
-               )  
- 
- TARGET_LINK_LIBRARIES ( CombineQtAndVTK  
-   ${VTK_LIBRARIES}  
-   QVTK  
-   )  
以上的脚本除了红色字体标注的跟1.1注释的不太像之外，其他的都一样，不再解释。
### 1.3 CMake脚本里增加工程环境变量的加载
很多非计算机专业的用户在使用VTK进行编程时，经常会碰到类似下图所示的一些错误。
![](https://img-my.csdn.net/uploads/201303/13/1363161153_9548.png)
碰到这样的错误以后，可能很多用户就不知道怎么处理了，其实上面的提示信息已经写得非常清楚了，就是缺少“vtkCommon.dll”文件。但是又有人会说：我的电脑里明明有这个文件存在啊，为什么会找不到呢？
**一般的解决方法**可能是：
方法一：将缺少的dll文件全部拷贝的工程的Debug或者Release目录下(拷贝的时候要注意你编译的VTK是Debug版本的还是Release版本的，如果拷错的话，又会出现其他不可预知的错误了)。但是这个方法是你每建一个工程，运行工程之前得把缺少的动态库文件又要拷贝过去，如果你不嫌麻烦的话，可以采用。
方法二：将缺少的dll文件全部拷贝到Windows系统的目录下，即C:\Windows\system32或者C:\Windows\system目录下，这个方法是你拷贝一次，以后再基于你拷贝的VTK动态库的工程运行的时候问题都解决了。但它同样有一个问题，假如你电脑里的VTK升级成别的版本，重新编译了一份动态库，或者是同时在你电脑里编译了好几个版本的VTK，这个时候就有点凌乱了。
**为什么这两种方法都可以解决问题？**原来动态编译的程序在启动的时候，会搜索程序所在的目录以及系统环境变量PATH所列的目录，如果这些目录有该程序需要的动态库时，就加载它们，如果没有，就提示无法加载相应动态库的错误。
可以在工程的CMakeLists.txt文件里添加一些脚本，把系统的PATH环境变量作一些更改，在工程启动之前加载这些环境变量。也就是(在工程的CMakeLists.txt最后添加)：
**[plain]**[view
 plain](http://blog.csdn.net/www_doling_net/article/details/8668870#)[copy](http://blog.csdn.net/www_doling_net/article/details/8668870#)
- #-----------------------------------------------------------------------------------  
- # Construct a list of paths containing runtime directories for project applications on Windows  
- set(PROJECT_RUNTIME_PATH  "${VTK_LIBRARY_DIRS}/@VS_BUILD_TYPE@;  
- ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/@VS_BUILD_TYPE@"  
-     )  
- if(QT4_FOUND)  
-   set(PROJECT_RUNTIME_PATH "${PROJECT_RUNTIME_PATH};${QT_LIBRARY_DIR}/../bin")  
- endif()  
- 
- include(CreateWindowsBatchScript.cmake)  
- 
- # If we are under Windows, create two batch files which correctly  
- # set up the environment for the application and for Visual Studio  
- if(WIN32)  
-   set(VS_SOLUTION_FILE "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.sln")  
-   foreach(VS_BUILD_TYPE debug release)  
-     CreateWindowsBatchScript("${CMAKE_SOURCE_DIR}/StartVS.bat.in"  
-       ${PROJECT_BINARY_DIR}/StartVS_${VS_BUILD_TYPE}.bat  
-       ${VS_BUILD_TYPE})  
-   endforeach()  
- endif(WIN32)  
以上的脚本也不是特别复杂，但提到了两个文件：**CreateWindowsBatchScript.cmake**以及**StartVS.bat.in**。这两个文件的内容分别是：
**CreateWindowsBatchScript.cmake：**
**[plain]**[view
 plain](http://blog.csdn.net/www_doling_net/article/details/8668870#)[copy](http://blog.csdn.net/www_doling_net/article/details/8668870#)
- function(CreateWindowsBatchScript in out build_type)  
-   if(VTK_DIR)  
-     set(VTK_BIN_DIR "${VTK_DIR}/bin/${build_type}")  
-   else()  
-     set(VTK_BIN_DIR)  
-   endif()  
- 
-   set(VS_BUILD_TYPE ${build_type})  
-   configure_file(${in} ${out} @ONLY)  
-   # substitute again  
-   configure_file(${out} ${out} @ONLY)  
- endfunction()  
**StartVS.bat.in****：**
**[plain]**[view
 plain](http://blog.csdn.net/www_doling_net/article/details/8668870#)[copy](http://blog.csdn.net/www_doling_net/article/details/8668870#)
- @set CL=/D_CRT_SECURE_NO_DEPRECATE /D_CRT_NONSTDC_NO_DEPRECATE  
- @set LINK=/LARGEADDRESSAWARE  
- 
- PATH=@PROJECT_RUNTIME_PATH@;%PATH%  
- "@VS_SOLUTION_FILE@"  
将工程通过CMake的configure->generate以后，即可生成StartVS_debug.bat和StartVS_release.bat两个脚本文件。如果你要编译、运行Debug版本的工程，即双击StartVS_debug.bat文件打开对应的工程，同理，Release版本的也一样。一旦按这种方式打开相应的工程，就不用再担心类似“无法加载***.dll文件”的错误了。如果你的程序还增加了ITK等函数库，也可以照着上面的脚本作相应的修改。
注意：使用时将**CreateWindowsBatchScript.cmake**和**StartVS.bat.in**两个文件与工程的CMakeLists.txt放在同一级目录里。即类似下图的目录结构：
![](https://img-my.csdn.net/uploads/201303/13/1363161162_1187.png)
## 2.   用QVTKWidget整合Qt&VTK
Qt与VTK的整合可以使用VTK提供的类QVTKWidget，看这个类名就知道这个类其实就是一个Qt里的Widget (QVTKWidget派生自QWidget)，所以可以把它当作普通的Qt里的Widget来使用，甚至可以在Qt Designer里像Qt的其他标准控件一样拖来拖去。
### 2.1 在Qt Designer里集成
要实现QVTKWidget在Qt Designer里像Qt的其他标准控件一样拖来拖去，需要把编译生成的QVTKWidgetPlugin.dll/QVTKWidgetPlugin.lib(Release版本)复制到Qt的安装目录里的plugins\designer目录下。完了以后，你会在Qt Designer里面看到如下的控件：
![](https://img-my.csdn.net/uploads/201303/13/1363161153_1140.png)
### 2.2 读入一幅图像，并在Qt界面上显示
接下来，我们来完成一个小功能，就是读入一幅JPG图像，然后在Qt界面上，用VTK来显示。功能非常简单，程序也非常简单。上代码：
ProjectMainWindow.h:
**[cpp]**[view
 plain](http://blog.csdn.net/www_doling_net/article/details/8668870#)[copy](http://blog.csdn.net/www_doling_net/article/details/8668870#)
- #ifndef Project_MainWindow_H
- #define Project_MainWindow_H
- 
- #include <QMainWindow>
- #include "ui_ProjectMainWindow.h"
- 
- #include <vtkSmartPointer.h>
- 
- class vtkImageViewer2;  
- class vtkRenderer;  
- 
- 
- class ProjectMainWindow : public QMainWindow, public Ui::ProjectMainWindow  
- {  
-        Q_OBJECT  
- 
- public:  
-        ProjectMainWindow();  
-        ~ProjectMainWindow();  
- 
- private slots:  
- //响应打开图像文件的槽函数
- void onOpenSlot();  
- 
- private:  
-        vtkSmartPointer< vtkImageViewer2 > m_pImageViewer;  
-        vtkSmartPointer< vtkRenderer > m_pRenderder;  
- };  
- 
- #endif
ProjectMainWindow.cpp:
**[cpp]**[view
 plain](http://blog.csdn.net/www_doling_net/article/details/8668870#)[copy](http://blog.csdn.net/www_doling_net/article/details/8668870#)
- #include "ProjectMainWindow.h"
- 
- #include <QFileDialog>
- #include <QDir>
- 
- #include <vtkRenderWindow.h>
- #include <vtkRenderer.h>
- #include <vtkImageViewer2.h>
- #include <QVTKWidget.h>
- #include <vtkJPEGReader.h>
- #include <vtkImageActor.h>
- 
- ProjectMainWindow::ProjectMainWindow()  
- {  
-        setupUi(this);  
- 
-        m_pImageViewer  = vtkSmartPointer< vtkImageViewer2 >::New();  
-        m_pRenderder      = vtkSmartPointer< vtkRenderer >::New();  
- 
- // 设置m_QVTKWidget的渲染器
-        m_QVTKWidget->GetRenderWindow()->AddRenderer(m_pRenderder);  
- 
- //连接打开的信号与相应的槽
-        connect( m_OpenAction, SIGNAL( triggered() ), this, SLOT( onOpenSlot() ) );  
- }  
- 
- ProjectMainWindow::~ProjectMainWindow()  
- {  
- }  
- 
- void ProjectMainWindow::onOpenSlot()  
- {  
-        QString filter;  
-        filter = "JPEG image file (*.jpg *.jpeg)";  
- 
-        QDir dir;  
-        QString fileName = QFileDialog::getOpenFileName( this,   
-                                  QString(tr("打开图像")), dir.absolutePath() , filter );  
- if ( fileName.isEmpty() == true ) return;  
- 
- // 支持带中文路径的读取
-        QByteArray ba = fileName.toLocal8Bit();  
- constchar *fileName_str = ba.data();  
- 
- // 用vtkJPEGReader读取JPG图像
-        vtkSmartPointer<vtkJPEGReader> reader = vtkSmartPointer<vtkJPEGReader>::New();  
-        reader->SetFileName(fileName_str);  
- 
- // 将reader的输出作为m_pImageViewer的输入，并设置m_pImageViewer与渲染器m_pRenderer的关联
-        m_pImageViewer->SetInput(reader->GetOutput());  
-        m_pImageViewer->UpdateDisplayExtent();  
-        m_pImageViewer->SetRenderWindow(m_QVTKWidget->GetRenderWindow());  
-        m_pImageViewer->SetRenderer(m_pRenderder);  
-        m_pImageViewer->SetupInteractor(m_QVTKWidget->GetRenderWindow()->GetInteractor());  
-        m_pImageViewer->SetSliceOrientationToXY(); //默认就是这个方向的
-        m_pImageViewer->GetImageActor()->InterpolateOff();  
-        m_pRenderder->ResetCamera();  
-        m_pRenderder->DrawOn();  
-        m_QVTKWidget->GetRenderWindow()->Render();  
- }  
程序运行结果：
![](https://img-my.csdn.net/uploads/201303/13/1363161153_4076.png)
### 2.3 用vtkEventQtSlotConnect实现VTK事件与Qt槽的连接
类vtkEventQtSlotConnect可以实现VTK的事件与Qt的槽函数的连接，VTK的事件主要在vtkCommand.h文件里定义，包括鼠标单击、鼠标双击、鼠标移动等等，如：
vtkCommand::ProgressEvent
vtkCommand::ErrorEvent
vtkCommand::WarningEvent
vtkCommand::PickEvent
vtkCommand::StartPickEvent
vtkCommand::EndPickEvent
vtkCommand::CharEvent
vtkCommand::KeyPressEvent
vtkCommand::KeyReleaseEvent
vtkCommand::LeftButtonPressEvent
vtkCommand::LeftButtonReleaseEvent
vtkCommand::MouseMoveEvent
……
具体的代码实现：
**[cpp]**[view
 plain](http://blog.csdn.net/www_doling_net/article/details/8668870#)[copy](http://blog.csdn.net/www_doling_net/article/details/8668870#)
- private slots:  
- //响应鼠标移动的消息，实时输出鼠标的当前位置
- void updateCoords(vtkObject* obj);  
- 
- private:  
-        vtkEventQtSlotConnect* m_Connections;  
源文件：
**[cpp]**[view
 plain](http://blog.csdn.net/www_doling_net/article/details/8668870#)[copy](http://blog.csdn.net/www_doling_net/article/details/8668870#)
- //构造函数里：
-        m_Connections = vtkEventQtSlotConnect::New();  
-        m_Connections->Connect(m_QVTKWidget->GetRenderWindow()->GetInteractor(),  
-               vtkCommand::MouseMoveEvent,  
- this,  
-               SLOT(updateCoords(vtkObject*)));  
- 
- //槽函数的实现
- void ProjectMainWindow::updateCoords(vtkObject* obj)  
- {  
- // 获取交互器
-        vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::SafeDownCast(obj);  
- 
- // 获取鼠标的当前位置
- int event_pos[2];  
-        iren->GetEventPosition(event_pos);  
- 
-        QString str;  
-        str.sprintf("x=%d : y=%d", event_pos[0], event_pos[1]);  
-        m_StatusBar->showMessage(str);  
- }  
程序运行结果：
![](https://img-my.csdn.net/uploads/201303/13/1363161153_4140.png)
**示例代码及该博文文档下载地址：http://download.csdn.net/detail/www_doling_net/5137375**
（3）二维图像处理：[05-VTK在图像处理中的应用(4)](http://blog.csdn.net/www_doling_net/article/details/8551523)
分类： [VTK系列教程](http://blog.csdn.net/www_doling_net/article/category/1331102)2013-01-29
 10:31948人阅读[评论](http://blog.csdn.net/www_doling_net/article/details/8551523#comments)(4)[收藏](http://blog.csdn.net/www_doling_net/article/details/8551523)[举报](http://blog.csdn.net/www_doling_net/article/details/8551523#report)
[VTK](http://blog.csdn.net/tag/details.html?tag=VTK)[命令模式](http://blog.csdn.net/tag/details.html?tag=%e5%91%bd%e4%bb%a4%e6%a8%a1%e5%bc%8f)[图像处理](http://blog.csdn.net/tag/details.html?tag=%e5%9b%be%e5%83%8f%e5%a4%84%e7%90%86)[观察者模式](http://blog.csdn.net/tag/details.html?tag=%e8%a7%82%e5%af%9f%e8%80%85%e6%a8%a1%e5%bc%8f)
目录[(?)](http://blog.csdn.net/www_doling_net/article/details/8551523#)[[+]](http://blog.csdn.net/www_doling_net/article/details/8551523#)
# 5.7 区域提取
### 5.7.1 提取感兴趣区域 
感兴趣区域（Volum of Interest）是指图像内部的一个子区域。在VTK中vtkExtractVOI类实现由用户指定的区域范围提取图像的子图像。该Filter的输入和输出都是一个vtkImageData，因此其结果可以直接作为图像保存。
   1:      vtkSmartPointer<vtkBMPReader> reader =
   2:          vtkSmartPointer<vtkBMPReader>::New();
   3:      reader->SetFileName ( "lena.bmp" );
   4:      reader->Update();
   5:  
   6:      int dims[3];
   7:      reader->GetOutput()->GetDimensions(dims);
   8:  
   9:      vtkSmartPointer<vtkExtractVOI> extractVOI =
  10:          vtkSmartPointer<vtkExtractVOI>::New();
  11:      extractVOI->SetInputConnection(reader->GetOutputPort());
  12:      extractVOI->SetVOI(dims[0]/4.,3.*dims[0]/4.,dims[1]/4.,3.*dims[1]/4., 0, 0);
  13:      extractVOI->Update();
上例代码实现了提取一副图像的子区域。首先读取一个图像，并获取图像的维数。然后定义vtkExtractVOI对象，该对象接收两个输入一个是图像数据，第二个是区域大小。设置区域大小的函数原型：
void SetVOI(int _arg1, int _arg2, int _arg3, int _arg4, int _arg5, int _arg6)
void SetVOI(int _arg[])
其参数是提取的区域各个方向的大小，共6个参数，依次表示x方向最小值，x方向最大值，y方向最小值，y方向最大值，z方向最小值和z方向最大值。上例中由于读取的是二维图像，因此z方向的区域为[0,0]，而在x方向范围为[ dims[0]/4 , 3*dims[0]/4 ]，y方向范围为[ dims[1]/4 , 3*dims[1]/4 ]，即提取图像原图中间1/4图像。执行结果如下：
![](https://img-my.csdn.net/uploads/201301/29/1359426894_9103.png)
图5.18 提取感兴趣区域
### 5.7.2 三维图像切片提取
切片是指三维图像中的一个切面对应的图像。切面可以是过图像内部一点且平行于XY、YZ、XZ平面的平面，也可以是任意的过三维图像内部一点任意方向的平面。通过提取切片可以方便的浏览和分析图像内部组织结构，是医学图像浏览软件中的一个重要的功能。在VTK中vtkImageReslice类实现图像切片提取功能。下面首先看一段切片提取的代码。
1:  vtkSmartPointer<vtkMetaImageReader> reader =
   2:     vtkSmartPointer<vtkMetaImageReader>::New();
   3:  reader->SetFileName ( " brain.mhd" );
   4:  reader->Update();
   5:   
   6:  int extent[6];
   7:  double spacing[3];
   8:  double origin[3];
   9:   
  10:  reader->GetOutput()->GetExtent(extent);
  11:  reader->GetOutput()->GetSpacing(spacing);
  12:  reader->GetOutput()->GetOrigin(origin);
  13:   
  14:  double center[3];
  15:  center[0] = origin[0] + spacing[0] * 0.5 * (extent[0] + extent[1]);
  16:  center[1] = origin[1] + spacing[1] * 0.5 * (extent[2] + extent[3]);
  17:  center[2] = origin[2] + spacing[2] * 0.5 * (extent[4] + extent[5]);
  18:   
  19:  static double axialElements[16] = {
  20:     1, 0, 0, 0,
  21:     0, 1, 0, 0,
  22:     0, 0, 1, 0,
  23:     0, 0, 0, 1 };
  24:   
  25:  vtkSmartPointer<vtkMatrix4x4> resliceAxes =
  26:     vtkSmartPointer<vtkMatrix4x4>::New();
  27:  resliceAxes->DeepCopy(axialElements);
  28:   
  29:  resliceAxes->SetElement(0, 3, center[0]);
  30:  resliceAxes->SetElement(1, 3, center[1]);
  31:  resliceAxes->SetElement(2, 3, center[2]);
  32:   
  33:   
  34:  vtkSmartPointer<vtkImageReslice> reslice =
  35:     vtkSmartPointer<vtkImageReslice>::New();
  36:  reslice->SetInputConnection(reader->GetOutputPort());
  37:  reslice->SetOutputDimensionality(2);
  38:  reslice->SetResliceAxes(resliceAxes);
  39:  reslice->SetInterpolationModeToLinear();
  40:   
  41:  vtkSmartPointer<vtkLookupTable> colorTable =
  42:     vtkSmartPointer<vtkLookupTable>::New();
  43:  colorTable->SetRange(0, 1000);
  44:  colorTable->SetValueRange(0.0, 1.0);
  45:  colorTable->SetSaturationRange(0.0, 0.0);
  46:  colorTable->SetRampToLinear();
  47:  colorTable->Build();
  48:   
  49:  vtkSmartPointer<vtkImageMapToColors> colorMap =
  50:     vtkSmartPointer<vtkImageMapToColors>::New();
  51:  colorMap->SetLookupTable(colorTable);
  52:  colorMap->SetInputConnection(reslice->GetOutputPort());
  53:   
  54:  vtkSmartPointer<vtkImageActor> imgActor =
  55:     vtkSmartPointer<vtkImageActor>::New();
  56:  imgActor->SetInput(colorMap->GetOutput());
  57:   
  58:  vtkSmartPointer<vtkRenderer> renderer =
  59:     vtkSmartPointer<vtkRenderer>::New();
  60:  renderer->AddActor(imgActor);
  61:  renderer->SetBackground(.4, .5, .6);
  62:   
  63:  vtkSmartPointer<vtkRenderWindow> renderWindow =
  64:     vtkSmartPointer<vtkRenderWindow>::New();
  65:  renderWindow->SetSize(500, 500);
  66:  renderWindow->AddRenderer(renderer);
  67:   
  68:  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
  69:     vtkSmartPointer<vtkRenderWindowInteractor>::New();
  70:  vtkSmartPointer<vtkInteractorStyleImage> imagestyle =
  71:     vtkSmartPointer<vtkInteractorStyleImage>::New();
  72:   
  73:  renderWindowInteractor->SetInteractorStyle(imagestyle);
  74:  renderWindowInteractor->SetRenderWindow(renderWindow);
  75:  renderWindowInteractor->Initialize();
  76:   
  77:  renderWindowInteractor->Start();
首先通过vtkMetaImageReader读取一副医学三维图像，并获取得到图像范围（extent），原点和像素间隔；由这三个参数可以计算图像的中心位置center；接下来定义了切面的变换矩阵axialElements，该矩阵的前三列分别表示x、y和z方向向量，第四列为中心点坐标；代码中的axialElements表示切面变换矩阵与当前坐标系一致，且切面为过中心点center，并平行于XY平面的平面。当前，定义该切面时，也可以是其他平面，甚至是任意平面，但是必须要过图像内部点。下面给出了一个常用的变换矩阵：
static double coronalElements[16] = {
 1, 0, 0, 0,
 0, 0, 1, 0,
0,-1, 0, 0,
 0, 0, 0, 1 }; 提取平行于XZ平面的切片
static double sagittalElements[16] = {
 0, 0,-1, 0,
 1, 0, 0, 0,
 0,-1, 0, 0,
 0, 0, 0, 1 }; 提取平行于YZ平面的切片
static double obliqueElements[16] = {
 1, 0, 0, 0,
 0, 0.866025, -0.5, 0,
 0, 0.5, 0.866025, 0,
 0, 0, 0, 1 }; 提取斜切切片
注意使用这些变换矩阵的时候，需要将第四列替换为切片经过图像的一个点坐标，上例中将图像的中心添加到axialElements矩阵，并通过函数SetResliceAxes设置变换矩阵，SetOutputDimensionality(2)指定输出的图像为一个二维图像；而函数SetInterpolationModeToLinear()则指定了切面提取中的差值方式为线性差值，另外该类中还提供了其他的差值方式：
SetInterpolationModeToNearestNeighbor()：最近邻方式
SetInterpolationModeToCubic()：三次线性差值
设置完毕后，执行Update()即可完成切面计算。运行结果如下图：
![](https://img-my.csdn.net/uploads/201301/29/1359426895_8411.png)
图5.19 切片提取
### 5.7.3 扩展
学习三维图像切面的提取后，我们在上节的程序上做一个扩展，实现一个稍微复杂的程序——通过滑动鼠标来切换三维图像切片，这也是医学图像处理软件中一个很基本的功能。实现该功能难点是怎样在VTK中控制鼠标来实时提取图像切片。在前面的章节中已经介绍观察者/命令(Observer/Command)模式，我们也采用这种机制来实现。VTK中鼠标消息是在交互类型对象（interactorstyle）中响应，因此通过为交互类型对象（interactorstyle）添加观察者（observer）来监听相应的消息，当消息触发时，由命令模式执行相应的回调函数。闲话少说，放代码。
1:  class vtkImageInteractionCallback : public vtkCommand
   2:  {
   3:  public:
   4:   
   5:      static vtkImageInteractionCallback *New()
   6:      {
   7:          return new vtkImageInteractionCallback;
   8:      }
   9:   
  10:      vtkImageInteractionCallback()
  11:      {
  12:          this->Slicing = 0;
  13:          this->ImageReslice = 0;
  14:          this->Interactor = 0;
  15:      }
  16:   
  17:      void SetImageReslice(vtkImageReslice *reslice)
  18:      {
  19:          this->ImageReslice = reslice;
  20:      }
  21:   
  22:      vtkImageReslice *GetImageReslice()
  23:      {
  24:          return this->ImageReslice;
  25:      }
  26:   
  27:      void SetInteractor(vtkRenderWindowInteractor *interactor)
  28:      {
  29:          this->Interactor = interactor;
  30:      }
  31:   
  32:      vtkRenderWindowInteractor *GetInteractor()
  33:      {
  34:          return this->Interactor;
  35:      }
  36:   
  37:      virtual void Execute(vtkObject *, unsigned long event, void *)
  38:      {
  39:          vtkRenderWindowInteractor *interactor = this->GetInteractor();
  40:   
  41:          int lastPos[2];
  42:          interactor->GetLastEventPosition(lastPos);
  43:          int currPos[2];
  44:          interactor->GetEventPosition(currPos);
  45:   
  46:          if (event == vtkCommand::LeftButtonPressEvent)
  47:          {
  48:              this->Slicing = 1;
  49:          }
  50:          else if (event == vtkCommand::LeftButtonReleaseEvent)
  51:          {
  52:              this->Slicing = 0;
  53:          }
  54:          else if (event == vtkCommand::MouseMoveEvent)
  55:          {
  56:              if (this->Slicing)
  57:              {
  58:                  vtkImageReslice *reslice = this->ImageReslice;
  59:   
  60:                  // Increment slice position by deltaY of mouse
  61:                  int deltaY = lastPos[1] - currPos[1];
  62:   
  63:                  reslice->Update();
  64:                  double sliceSpacing = reslice->GetOutput()->GetSpacing()[2];
  65:                  vtkMatrix4x4 *matrix = reslice->GetResliceAxes();
  66:                  // move the center point that we are slicing through
  67:                  double point[4];
  68:                  double center[4];
  69:                  point[0] = 0.0;
  70:                  point[1] = 0.0;
  71:                  point[2] = sliceSpacing * deltaY;
  72:                  point[3] = 1.0;
  73:                  matrix->MultiplyPoint(point, center);
  74:                  matrix->SetElement(0, 3, center[0]);
  75:                  matrix->SetElement(1, 3, center[1]);
  76:                  matrix->SetElement(2, 3, center[2]);
  77:                  interactor->Render();
  78:              }
  79:              else
  80:              {
  81:                  vtkInteractorStyle *style = vtkInteractorStyle::SafeDownCast(
  82:                      interactor->GetInteractorStyle());
  83:                  if (style)
  84:                  {
  85:                      style->OnMouseMove();
  86:                  }
  87:              }
  88:          }
  89:      }
  90:   
  91:  private:
  92:      int Slicing;
  93:      vtkImageReslice *ImageReslice;
  94:      vtkRenderWindowInteractor *Interactor;
  95:  };
vtkImageInteractionCallback继承自vtkCommand类，并覆盖父类函数Execute()。该类提供了两个接口：SetImageReslice和SetInteractor。SetImageReslice用以设置vtkImageSlice对象，vtkImageSlice根据设置的变换矩阵提取三维图像切片。SetInteractor用以设置vtkRenderWindowInteractor，vtkRenderWindowInteractor类对象负责每次提取切片后刷新视图。
下面我们重点来看一下Execute函数，该函数提供了具体的切片提取功能。在该函数里面，主要监听了三个消息：
vtkCommand::LeftButtonPressEvent，
vtkCommand::LeftButtonReleaseEvent，
vtkCommand::MouseMoveEvent，
前两个消息分别是鼠标左键的按下和弹起消息。当鼠标左键按下时，就设置切片提取标志为1，而当弹起时，将标志置为0。这样在鼠标移动时，只有在确定切片提取标志为1时，执行切片提取功能。
vtkCommand::MouseMoveEvent即为鼠标移动消息。当检测到该消息时，首先检查切片提取标志，当为1时提取切片。提取切片时，需要为vtkImageSlice对象设置变换矩阵。这里在函数开始时，首先获取了鼠标滑动的前后两次点的位置lastPos和currPos。然后根据两点的Y坐标差deltaY，计算新的中心点center并变换至vtkImageSlice当前变换矩阵中，得到变换中心点，将其设置到原来的变换矩阵matrix中，并设置到vtkImageSlice中，最后执行interactor->Render()即可不断的根据鼠标移动刷新图像。
Command对象定义完毕后，即可为交互对象InteractorStyle添加观察者，响应鼠标消息。这里可以在上节的程序上进行修改，前面代码一致，只需要在最后添加如下代码：
1:      vtkSmartPointer<vtkImageInteractionCallback> callback =
   2:          vtkSmartPointer<vtkImageInteractionCallback>::New();
   3:      callback->SetImageReslice(reslice);
   4:      callback->SetInteractor(renderWindowInteractor);
   5:  
   6:      imagestyle->AddObserver(vtkCommand::MouseMoveEvent, callback);
   7:      imagestyle->AddObserver(vtkCommand::LeftButtonPressEvent, callback);
   8:      imagestyle->AddObserver(vtkCommand::LeftButtonReleaseEvent, callback);
   9:  
  10:      renderWindowInteractor->Start();
这里主要是定义了vtkImageInteractionCallback对象，并设置vtkImageSlice对象和vtkRenderWindowInteractor对象。然后为交互对象vtkInteractorStyle添加观察者来监控相应的消息，这里主要是三个消息：
vtkCommand::LeftButtonPressEvent，
vtkCommand::LeftButtonReleaseEvent，
vtkCommand::MouseMoveEvent，
当响应到这三个消息时，立即执行vtkImageInteractionCallback的Execute函数，以便实现切片的实时提取和更新。完成以后，运行程序，当鼠标在图像上移动时，会发现图像会跟着鼠标的移动而变化，神奇吧？有兴趣的话，还可以实现YZ平面、XZ平面切片提取，甚至是任意方向的切面提取。
# ==========欢迎转载，转载时请保留该声明信息==========
# 版权归[@东灵工作室](http://weibo.com/dolingstudio)所有，更多信息请访问[东灵工作室](http://www.doling.net/)
