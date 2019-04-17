# ITK 3.20.1 VS2010 Configuration 配置 - Grandyang - 博客园







# [ITK 3.20.1 VS2010 Configuration 配置](https://www.cnblogs.com/grandyang/p/5134916.html)







Download ITK 3.20.1

Download VS2010

Download CMake 3.2.0



I assume you've already installed VS2010 and CMake 3.2.0 correctly.



**Compile the VTK:**

Start CMake 3.2.0, fill the source and destination:

source:** C:/ITK3.20.1/InsightToolkit-3.20.1**

destination: **C:/**ITK3.20.1**/VS2010**

Click **Configure** and use **Visual Studio 10 2010** to complie.



When first configure is done, select **Grouped** and **Advanced**.

```
BUILD_EXAMPLES          [ON]  
BUILD_TEST              [ON]  
CMAKE_INSTALL_PREFIX    [C:/ITK3.20.1/VS2010_install]  
CMAKE_USE_PTHREADS        [OFF]
CMAKE_USE_WIN32_THREADS [ON]
```



click **Configure** again.

After configure is done (No errors show), click **Generate**.



Go back to your build folder, open **ITK.sln**, right click **ALL_BUILD**, choose **Rebuild**.

**Wait a long time for make process, have a cup of coffee :)**

After rebuild is done, right click **INSTALL**, choose **Build**.



**Now, the configuration is done, enjoy it :)**



Create a new empty project,



**Add in the system Path:**

C:\ITK3.20.1\VS2010_install\bin



**Project->Project Property->Configuration Properties->VC++Directories ->Include Directories:**

C:\ITK3.20.1\VS2010_install\include\InsightToolkit
C:\ITK3.20.1\VS2010_install\include\InsightToolkit\Algorithms
C:\ITK3.20.1\VS2010_install\include\InsightToolkit\BasicFilters
C:\ITK3.20.1\VS2010_install\include\InsightToolkit\Common
C:\ITK3.20.1\VS2010_install\include\InsightToolkit\gdcm\src
C:\ITK3.20.1\VS2010_install\include\InsightToolkit\IO
C:\ITK3.20.1\VS2010_install\include\InsightToolkit\Numerics
C:\ITK3.20.1\VS2010_install\include\InsightToolkit\SpatialObject
C:\ITK3.20.1\VS2010_install\include\InsightToolkit\Utilities
C:\ITK3.20.1\VS2010_install\include\InsightToolkit\Utilities\vxl\vcl
C:\ITK3.20.1\VS2010_install\include\InsightToolkit\Utilities\vxl\core



**Project->Project Property->Configuration Properties->VC++Directories ->Library Directories:**

C:\ITK3.20.1\VS2010_install\lib\InsightToolkit



**Project->Project Property->Configuration Properties->Linker->Input:**

ITKAlgorithms.lib
ITKBasicFilters.lib
ITKCommon.lib
ITKDICOMParser.lib
ITKEXPAT.lib
ITKFEM.lib
itkgdcm.lib
ITKIO.lib
itkjpeg8.lib
itkjpeg12.lib
itkjpeg16.lib
ITKMetaIO.lib
itkNetlibSlatec.lib
ITKniftiio.lib
ITKNrrdIO.lib
ITKNumerics.lib
itkopenjpeg.lib
itkpng.lib
ITKSpatialObject.lib
ITKStatistics.lib
itksys.lib
itktiff.lib
itkv3p_lsqr.lib
itkv3p_netlib.lib
itkvcl.lib
itkvnl.lib
itkvnl_algo.lib
itkvnl_inst.lib
itkzlib.lib
ITKznz.lib
snmpapi.lib
rpcrt4.lib
wsock32.lib 

**Create a test.cpp file as below:**

```
#include"itkImage.h"  
#include<iostream>  

int main(){  

    typedef itk::Image< unsigned short, 3>ImageType;  
    ImageType::Pointer image= ImageType::New();  

    int i;  

    std::cout<<"ITK hello world !"<<std::endl;  
    std::cin>>i;  
    return 0;  
}
```














