# VTKMY 3.3 VS 2010 Configuration 配置 - Grandyang - 博客园







# [VTKMY 3.3 VS 2010 Configuration 配置](https://www.cnblogs.com/grandyang/p/5143922.html)







Download VTKMY 3.3

Download VS2010

Download CMake 3.2.0



I assume you've already installed VS2010 and CMake 3.2.0 correctly.



**Compile the VTKMY:**

Start CMake 3.2.0, fill the source and destination:

source:** C:/VTKMY/vtkMy-3.3**

destination: **C:/VTKMY/VS2010**

Click **Configure** and use **Visual Studio 10 2010** to complie.



When first configure is done, select **Grouped** and **Advanced**.

```
CMAKE_INSTALL_PREFIX    [C:/VTKMY/VS2010_install]
```



click **Configure** again.

After configure is done (No errors show), click **Generate**.



Go back to your build folder, open **VTKMY.sln**, choose **Release** mode, right click **ALL_BUILD**, choose **Rebuild**.

**Wait a long time for make process, have a cup of coffee :)**

After rebuild is done, right click **INSTALL**, choose **Build**.



**Now, the configuration is done, enjoy it :)**



Create a new empty project,



**Project->Project Property->Configuration Properties->VC++Directories ->Include Directories:**

C:\VTKMY\VS2010_install\include\vtkMy
C:\VTKMY\VS2010_install\include\vtk
C:\VTKMY\vtkMy-3.3\Common
C:\VTKMY\vtkMy-3.3\FEM
C:\VTKMY\vtkMy-3.3\Hybrid
C:\VTKMY\vtkMy-3.3\Graphics
C:\VTKMY\vtkMy-3.3\Utilities\FastRBF
C:\VTKMY\vtkMy-3.3\Imaging
C:\VTKMY\vtkMy-3.3\IO
C:\VTKMY\vtkMy-3.3\SPMESHUtils



**Project->Project Property->Configuration Properties->VC++Directories ->Library Directories:**

C:\VTKMY\VS2010_install\lib\Release
C:\VTKMY\vtkMy-3.3\Utilities\FastRBF



**Project->Project Property->Configuration Properties->Linker->Input:**

ANN.lib
vtkmyCommon.lib
vtkmyFEM.lib
vtkmyGraphics.lib
vtkmyHybrid.lib
vtkmyImaging.lib
vtkmyIO.lib
vtkmySPMESHUtils.lib
fastrbf.lib












