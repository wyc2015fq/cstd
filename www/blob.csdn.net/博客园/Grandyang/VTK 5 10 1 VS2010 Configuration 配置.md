# VTK 5.10.1 VS2010 Configuration 配置 - Grandyang - 博客园







# [VTK 5.10.1 VS2010 Configuration 配置](https://www.cnblogs.com/grandyang/p/5134894.html)







Download VTK 5.10.1

Download VS2010

Download CMake 3.2.0



I assume you've already installed VS2010 and CMake 3.2.0 correctly.



**Compile the VTK:**

Start CMake 3.2.0, fill the source and destination:

source:** C:/VTK5.10.1/VTK5.10.1**

destination: **C:/VTK5.10.1/VS2010**

Click **Configure** and use **Visual Studio 10 2010** to complie.



When first configure is done, select **Grouped** and **Advanced**.

```
BUILD_EXAMPLES          [ON]  
BUILD_TEST              [ON]  
CMAKE_INSTALL_PREFIX    [C:/VTK5.10.1/VS2010_install]  
CMAKE_USE_PTHREADS      [OFF]
CMAKE_USE_WIN32_THREADS [ON]
```



click **Configure** again.

After configure is done (No errors show), click **Generate**.



Go back to your build folder, open **VTK.sln**, right click **ALL_BUILD**, choose **Rebuild**.

**Wait a long time for make process, have a cup of coffee :)**

After rebuild is done, right click **INSTALL**, choose **Build**.



**Now, the configuration is done, enjoy it :)**



Create a new empty project,



**Project->Project Property->Configuration Properties->VC++Directories ->Include Directories:**

C:\VTK5.10.1\VS2010
C:\VTK5.10.1\VS2010\bin
C:\VTK5.10.1\VTK5.10.1\GenericFiltering
C:\VTK5.10.1\VTK5.10.1\Common
C:\VTK5.10.1\VTK5.10.1\IO
C:\VTK5.10.1\VTK5.10.1\Filtering
C:\VTK5.10.1\VTK5.10.1\Rendering
C:\VTK5.10.1\VTK5.10.1\Graphics
C:\VTK5.10.1\VTK5.10.1\olumeRendering
C:\VTK5.10.1\VTK5.10.1\Imaging
C:\VTK5.10.1\VTK5.10.1\Hybrid
C:\VTK5.10.1\VTK5.10.1\GUISupport
C:\VTK5.10.1\VTK5.10.1\Utilities
C:\VTK5.10.1\VS2010\Utilities
C:\VTK5.10.1\VTK5.10.1\Parallel
C:\VTK5.10.1\VTK5.10.1\Widgets
C:\VTK5.10.1\VS2010_install\include\vtk-5.10



**Project->Project Property->Configuration Properties->VC++Directories ->Library Directories:**

C:\VTK5.10.1\VS2010\bin\Release



**Project->Project Property->Configuration Properties->Linker->Input:**

opengl32.lib 
kernel32.lib 
user32.lib 
gdi32.lib 
winspool.lib 
comdlg32.lib 
advapi32.lib 
shell32.lib 
ole32.lib 
oleaut32.lib 
uuid.lib 
odbc32.lib 
vtkCommon.lib 
vtkDICOMParser.lib 
vtkexoIIc.lib 
vtkexpat.lib 
vtkFiltering.lib 
vtkfreetype.lib 
vtkftgl.lib 
vtkGenericFiltering.lib 
vtkGraphics.lib 
vtkHybrid.lib 
vtkImaging.lib 
vtkIO.lib 
vtkjpeg.lib 
vtkNetCDF.lib 
vtkpng.lib 
vtkRendering.lib 
vtksys.lib 
vtktiff.lib 
vtkVolumeRendering.lib 
vtkWidgets.lib 
vtkzlib.lib  



**Create a test.cpp file as below:**

```
#include <vtkConeSource.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

int main(int, char *[])
{
    //Create a cone
    vtkSmartPointer<vtkConeSource> coneSource =
        vtkSmartPointer<vtkConeSource>::New();
    coneSource->Update();

    //Create a mapper and actor
    vtkSmartPointer<vtkPolyDataMapper> mapper =
        vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(coneSource->GetOutputPort());

    vtkSmartPointer<vtkActor> actor =
        vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    //Create a renderer, render window, and interactor
    vtkSmartPointer<vtkRenderer> renderer =
        vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<vtkRenderWindow> renderWindow =
        vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);

    //Add the actors to the scene
    renderer->AddActor(actor);
    renderer->SetBackground(.3, .2, .1); // Background color dark red

    //Render and interact
    renderWindow->Render();
    renderWindowInteractor->Start();

    return EXIT_SUCCESS;
}
```



If error LNK2038: mismatch detected for '_ITERATOR_DEBUG_LEVEL': value '0' doesn't match value '2'[](http://www.cnblogs.com/grandyang/p/4449120.html) occurs, please see solution [here](http://www.cnblogs.com/grandyang/p/4449120.html).














