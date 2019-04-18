# **PCL：嵌入VTK/QT显示（Code^_^） - wishchinYang的专栏 - CSDN博客
2014年07月08日 14:57:21[wishchin](https://me.csdn.net/wishchin)阅读数：4327
中国人真是太不知道分享了，看看这个老外的博客，[启发性链接](http://www.pcl-users.org/QT-PCLVisualizer-mostly-working-td3285187.html)。
[http://www.pcl-users.org/](http://www.pcl-users.org/)
**1. 这个是可用的源代码：**
**原文**：I saw a thread with links to a non-vtk qt visualizer of pcl pointclouds, but I want to reuse as much of PCLVisualizer as possible and minimize novel code.  My hacky solution is below but it suffers
 from the problem of creating an unused non-qt window in addition to the desired qt window that displays the point cloud and accepts mouse input.
I'd be fine with figuring out how to suppress that window or at least close it soon after it opens (though not if it requires changing vtk code), but a another solution might be to break out what addPointCloud() & fromHandlersToScreen() are doing into
 accessible utility functions.
Any suggestions for how to proceed?  
Thanks,
-Lucas 
I installed QVTKWidget on Ubuntu with the package libvtk5-qt4-dev.
I added this to PCLVisualizer:
vtkSmartPointer<vtkRenderWindow> getRenderWindow() {
  return win_;
}
and my code looks like this (it's based off [http://www.itk.org/Wiki/VTK/Examples/Cxx/Qt/RenderWindowNoUiFile](http://www.itk.org/Wiki/VTK/Examples/Cxx/Qt/RenderWindowNoUiFile)):
///////////////////////////////////////////////使用RenderWindow
The complete modified source:
```cpp
#include <QApplication>
       #include <pcl/io/pcd_io.h>
       #include <pcl/features/normal_3d.h>
       #include <pcl/sample_consensus/sac_model_plane.h>
       #include <pcl/visualization/cloud_viewer.h>
       #include <pcl/common/common.h>
       #include <QVTKWidget.h>
       int main(int argc, char** argv)
       {
          QApplication app(argc, argv);
          QVTKWidget widget;
          widget.resize(512, 256);
          {
            pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_xyz (new pcl::PointCloud<pcl::PointXYZ>);
            {
              for (float y = -0.5f; y <= 0.5f; y += 0.01f)
              {
                for (float z = -0.5f; z <= 0.5f; z += 0.01f)
                {
                  pcl::PointXYZ point;
                  point.x = 2.0f - y;
                  point.y = y;
                  point.z = z;
                  cloud_xyz->points.push_back (point);
                }
              }
              cloud_xyz->width = cloud_xyz->points.size ();
              cloud_xyz->height = 1;
            }
            // this creates and displays a window named "test_viz"
            // upon calling PCLVisualizerInteractor interactor_->Initialize ();
            // how to disable that?
            pcl::visualization::PCLVisualizer pviz ("test_viz", false);
            // not sure why but it is necessary to set the render window before modifying pviz
            vtkSmartPointer<vtkRenderWindow> renderWindow = pviz.getRenderWindow ();
            widget.SetRenderWindow (renderWindow);
            // these are useful to add to make the controls more like pcd_viewer
            pviz.setupInteractor (widget.GetInteractor (), widget.GetRenderWindow ());
            pviz.getInteractorStyle ()->setKeyboardModifier (pcl::visualization::INTERACTOR_KB_MOD_SHIFT);
            pviz.addPointCloud<pcl::PointXYZ>(cloud_xyz);
            pviz.setBackgroundColor(0, 0, 0.1);
          }
          widget.show();
          app.exec();
          return EXIT_SUCCESS;
       }
```
、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、
**出现的错误**：
c:\Dev\VTK5.8.0\include\vtk-5.8\vtkSmartPointer.h:75: 错误:C2440: “static_cast”: 无法从“vtkObjectBase *const ”转换为“vtkRenderWindow *”
与指向的类型无关；转换要求 reinterpret_cast、C 样式转换或函数样式转换
c:\Dev\VTK5.8.0\include\vtk-5.8\vtkSmartPointer.h(74): 编译类 模板 成员函数“vtkSmartPointer<T>::operator T(void) const”时
```cpp
with
[
    T=vtkRenderWindow
]
c:\PCL_1.7.1\include\pcl/visualization/common/ren_win_interact_map.h(70): 参见对正在编译的类 模板 实例化“vtkSmartPointer<T>”的引用
with
[
    T=vtkRenderWindow
]
```
**2.这个可以实现JPEG图像读取显示**
**原文：**RenderWindowNoUiFile.cxx
```cpp
#include <QApplication>
 
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkImageViewer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkJPEGReader.h>
 
#include <QVTKWidget.h>
 
int main(int argc, char** argv)
{
  QApplication app(argc, argv);
  QVTKWidget widget;
  widget.resize(256,256);
 
  // Setup sphere
  vtkSmartPointer<vtkSphereSource> sphereSource = 
      vtkSmartPointer<vtkSphereSource>::New();
  sphereSource->Update();
  vtkSmartPointer<vtkPolyDataMapper> sphereMapper = 
      vtkSmartPointer<vtkPolyDataMapper>::New();
  sphereMapper->SetInputConnection(sphereSource->GetOutputPort());
  vtkSmartPointer<vtkActor> sphereActor = 
      vtkSmartPointer<vtkActor>::New();
  sphereActor->SetMapper(sphereMapper);
 
  // Setup window
  vtkSmartPointer<vtkRenderWindow> renderWindow = 
      vtkSmartPointer<vtkRenderWindow>::New();
 
  // Setup renderer
  vtkSmartPointer<vtkRenderer> renderer = 
      vtkSmartPointer<vtkRenderer>::New();
  renderWindow->AddRenderer(renderer);
 
  renderer->AddActor(sphereActor);
  renderer->ResetCamera();
 
  widget.SetRenderWindow(renderWindow);
  widget.show();
  app.exec();
  return EXIT_SUCCESS;
}
CMakeLists.txt
cmake_minimum_required(VERSION 2.8)
 
if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()
 
PROJECT(RenderWindowNoUiFile)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
if(${VTK_VERSION} VERSION_GREATER "6" AND VTK_QT_VERSION VERSION_GREATER "4")
  # Instruct CMake to run moc automatically when needed.
  set(CMAKE_AUTOMOC ON)
  find_package(Qt5Widgets REQUIRED QUIET)
else()
  find_package(Qt4 REQUIRED)
  include(${QT_USE_FILE})
endif()
 
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
 
file(GLOB UI_FILES *.ui)
file(GLOB QT_WRAP *.h)
file(GLOB CXX_FILES *.cxx)
 
if(${VTK_VERSION} VERSION_GREATER "6" AND VTK_QT_VERSION VERSION_GREATER "4")
  qt5_wrap_ui(UISrcs ${UI_FILES} )
  # CMAKE_AUTOMOC in ON so the MocHdrs will be automatically wrapped.
  add_executable(RenderWindowNoUiFile MACOSX_BUNDLE
    ${CXX_FILES} ${UISrcs} ${QT_WRAP})
  qt5_use_modules(RenderWindowNoUiFile Core Gui)
  target_link_libraries(RenderWindowNoUiFile ${VTK_LIBRARIES})
else()
  QT4_WRAP_UI(UISrcs ${UI_FILES})
  QT4_WRAP_CPP(MOCSrcs ${QT_WRAP})
  add_executable(RenderWindowNoUiFile MACOSX_BUNDLE ${CXX_FILES} ${UISrcs} ${MOCSrcs})
 
  if(VTK_LIBRARIES)
    if(${VTK_VERSION} VERSION_LESS "6")
      target_link_libraries(RenderWindowNoUiFile ${VTK_LIBRARIES} QVTK)
    else()
      target_link_libraries(RenderWindowNoUiFile ${VTK_LIBRARIES})
    endif()
  else()
    target_link_libraries(RenderWindowNoUiFile vtkHybrid QVTK vtkViews ${QT_LIBRARIES})
  endif()
endif()
```
## Download and Build RenderWindowNoUiFile
Click [here to download RenderWindowNoUiFile](http://gitorious.org/vtkwikiexamplestarballs/vtkwikiexamplestarballs/raw/master:RenderWindowNoUiFile.tar). and its *CMakeLists.txt* file.
Once the tarball *RenderWindowNoUiFile.tar* has been downloaded and extracted,
cd RenderWindowNoUiFile/build
This example requires Qt and VTK.
- If VTK and Qt are installed:
cmake ..
- If VTK is not installed but compiled on your system, you will need to specify the path to your VTK build:
cmake -DVTK_DIR:PATH=/home/me/vtk_build ..
- If Qt is not found on your system, you will need to tell CMake where to find qmake:
cmake -DQT_QMAKE_EXECUTABLE:FILEPATH=/usr/something/qmake  ..
Build the project:
make
and run it:
./RenderWindowNoUiFile
