# Vtk读取并显示保存图像 - wishchinYang的专栏 - CSDN博客
2013年09月27日 10:25:56[wishchin](https://me.csdn.net/wishchin)阅读数：2525
（1）：Vtk读取并显示图像：三种方法   
转载：未知出处
### [转载：用VTK显示平面图片：http://blog.csdn.net/tonylk/article/details/464881](http://blog.csdn.net/tonylk/article/details/464881)
用到vtkJPEGReader类。
 包含的头文件：
```cpp
#include "vtkActor.h" #include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h" #include "vtkImageViewer.h" #include "vtkJPEGReader.h" #include "vtkImageActor.h"
#include "vtkInteractorStyleImage.h" #include "vtkActor2D.h"
```
方法一： 
使用vtkImageMapper和vtkAcotor2D来实现
```cpp
int main(int argc, char **argv)
{
int extent[6];
vtkJPEGReader *m_reader = vtkJPEGReader::New();
m_reader->SetFileName("C:\\Users\\igst\\Documents\\美图图库\\叶.jpg");
 m_reader->SetDataByteOrderToLittleEndian();
m_reader->SetDataScalarTypeToUnsignedChar();
 m_reader->SetFileDimensionality(3);
vtkImageMapper *mapper=vtkImageMapper::New();
 mapper->SetInput(m_reader->GetOutput());
mapper->SetColorLevel(128); mapper->SetColorWindow(256);
vtkActor2D *actor=vtkActor2D::New();
actor->SetMapper(mapper);
vtkRenderer *m_Renderer = vtkRenderer::New();
vtkRenderWindow *m_RenWin = vtkRenderWindow::New();
 m_RenWin->AddRenderer(m_Renderer);
vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
iren->SetRenderWindow(m_RenWin);
vtkInteractorStyleImage *interactor = vtkInteractorStyleImage::New();
 iren->SetInteractorStyle(interactor); m_Renderer->AddActor(actor);
m_Renderer->SetBackground(0.1, 0.2, 0.4); m_RenWin->Render();
m_reader->GetDataExtent(extent);//如果把这条命令放在Render()的前面则无效
m_RenWin->SetSize(extent[1],extent[3]);
iren->Initialize();
iren->Start();
return 0;
 }
```
结果： 
 方法二： 
使用vtkImageActor类，不需要用vktImageMaper
```cpp
int main(int argc, char **argv)
 {
vtkJPEGReader *m_reader = vtkJPEGReader::New();
m_reader->SetFileName("C:\\Users\\igst\\Documents\\美图图库\\荷叶.jpg");
m_reader->SetDataByteOrderToLittleEndian();
m_reader->SetDataScalarTypeToUnsignedChar();
m_reader->SetFileDimensionality(3);
vtkImageActor *m_Actor = vtkImageActor::New();
m_Actor->SetInput(m_reader->GetOutput()); vtkRenderer *m_Renderer = vtkRenderer::New();
vtkRenderWindow *m_RenWin = vtkRenderWindow::New();
m_RenWin->AddRenderer(m_Renderer);
vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
iren->SetRenderWindow(m_RenWin);
vtkInteractorStyleImage *interactor = vtkInteractorStyleImage::New();
iren->SetInteractorStyle(interactor); m_Renderer->AddActor(m_Actor);
m_Renderer->SetBackground(0.1, 0.2, 0.4);
m_RenWin->Render();
iren->Initialize();
iren->Start();
return 0;
 } 
 
方法三：简便快速
使用ImageViewer,需要调节窗宽窗位
ImageViewer将 vtkRenderWindow, vtkRenderer, vtkActor2D and vtkImageMapper集成在了一起，
不需要再对vtkRenderWindow, vtkRenderer, vtkActor2D and vtkImageMapper定义。
 int main() {
   vtkJPEGReader *image=vtkJPEGReader::New();
   image->SetFileName("C:\\Users\\igst\\Documents\\美图图库\\竹林.jpg");
   image->SetDataByteOrderToLittleEndian(); 
   image->SetDataScalarTypeToUnsignedChar();
   vtkImageViewer *pViewer = vtkImageViewer::New();//显示二维图形  
   pViewer->SetColorLevel(128);//设置窗宽窗位， 
   pViewer->SetColorWindow(256);
   pViewer->SetInput(image->GetOutput()); 
   pViewer->Render();
   vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();  
   iren->SetRenderWindow(pViewer->GetRenderWindow()); 
  iren->Initialize(); 
  iren->Start();  
  return 0;
 }
```
（2）：VTK保存图片
