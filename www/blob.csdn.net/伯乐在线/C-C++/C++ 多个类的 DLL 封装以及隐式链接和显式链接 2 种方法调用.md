# C++ 多个类的 DLL 封装以及隐式链接和显式链接 2 种方法调用 - 文章 - 伯乐在线
原文出处： [waterbinbin](http://blog.csdn.net/waterbinbin/article/details/52625508)
将OpenCL和OpenCV一些简单的方法封装成DLL，具体OpenCL和OpenCV的配置方法可以参考本人的博客。
[VS2015下安装与编译OpenCV源码并在VS2015下配置OpenCV环境](http://blog.csdn.net/waterbinbin/article/details/52487611)
[VS上运行CUDA，并在NVDIA显卡安装的CUDA中运行OpenCL](http://blog.csdn.net/waterbinbin/article/details/52238519)
**1.  DLL封装**
**（1）  创建DLL工程**
新建一个Win32控制台应用程序，项目名称为buildopencldll，选择DLL和空项目。
![](http://img.blog.csdn.net/20160922180529139?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**（2）新建头文件**
头文件：createopencldll.h
向动态链接库添加类的核心代码：


```
#ifdef CREATEOPENCLDLL_EXPORTS  
#define CREATEOPENCLDLL_API __declspec(dllexport)   
#else  
#define CREATEOPENCLDLL_API __declspec(dllimport)   
#endif
```
为显式链接做准备的导出函数：

```
extern "C"  
{  
    CREATEOPENCLDLL_API CreateOpenCLDLL* GetCreateOpenCLDLL(void);  
    typedef CreateOpenCLDLL* (*PFNGetCreateOpenCLDLL)(void);  
}
```
创建类时必须加上CREATEOPENCLDLL_API，如写成class CREATEOPENCLDLL_APICreateOpenCLDLL 要不然生成不了lib文件
完整的头文件代码如下：


```
#pragma once  
#ifndef _CREATEOPENCLDLL_H_  
#define _CREATEOPENCLDLL_H_  
  
#ifdef CREATEOPENCLDLL_EXPORTS  
#define CREATEOPENCLDLL_API __declspec(dllexport)   
#else  
#define CREATEOPENCLDLL_API __declspec(dllimport)   
#endif  
  
#ifdef __APPLE__  
#include <OpenCL/cl.h>  
#else  
#include <CL/cl.h>  
#endif  
  
  
class CREATEOPENCLDLL_API CreateOpenCLDLL  
{  
public:  
    CreateOpenCLDLL();  
    ~CreateOpenCLDLL();  
    //  选择平台并创建上下文    
    cl_context CreateContext();  
    cl_command_queue CreateCommandQueue(cl_context context, cl_device_id *device);  
    cl_program CreateProgram(cl_context context, cl_device_id device, const char* fileName);  
    bool CreateMemObjects(cl_context context, cl_mem memObjects[3], float *a, float *b);  
    void Cleanup(cl_context context, cl_command_queue commandQueue,  
        cl_program program, cl_kernel kernel, cl_mem memObjects[3]);  
    void Calcaute();  
  
private:  
    cl_context m_context;  
    cl_command_queue m_commandQueue;  
    cl_program m_program;  
    cl_device_id m_device;  
    cl_kernel m_kernel;  
    cl_mem m_memObjects[3];  
    cl_int m_errNum;  
};  
/* 导出函数声明 */  
extern "C"  
{  
    CREATEOPENCLDLL_API CreateOpenCLDLL* GetCreateOpenCLDLL(void);  
    typedef CreateOpenCLDLL* (*PFNGetCreateOpenCLDLL)(void);  
}  
  
#endif
```
**（3）Cpp文件：createopencldll.cpp**
需要添加：#define CREATEOPENCLDLL_EXPORTS
完整的cpp代码如下：

```
#define CREATEOPENCLDLL_EXPORTS  
  
#include <iostream>  
#include <fstream>  
#include <string>  
#include <vector>  
#include <sstream>  
  
#include "createopencldll.h"  
  
using namespace std;  
  
const int KArraySize = 1000;  
  
CreateOpenCLDLL::CreateOpenCLDLL()  
{  
    m_context = 0;  
    m_commandQueue = 0;  
    m_program = 0;  
    m_device = 0;  
    m_kernel = 0;  
    m_memObjects[0] = 0;  
    m_memObjects[1] = 0;  
    m_memObjects[2] = 0;  
    m_errNum = 0;  
}  
  
CreateOpenCLDLL::~CreateOpenCLDLL()  
{  
  
}  
  
//  选择平台并创建上下文  
cl_context CreateOpenCLDLL::CreateContext()  
{  
    cl_int errNum;  
    cl_uint numPlatforms;  
    cl_platform_id firstPlatformId;  
    cl_context context = NULL;  
  
    //选择第一个可用的平台    
    errNum = clGetPlatformIDs(1, &firstPlatformId, &numPlatforms);  
    if (errNum != CL_SUCCESS || numPlatforms <= 0)  
    {  
        std::cerr << "Failed to find any OpenCL platforms." << std::endl;  
        return NULL;  
    }  
  
    // 创建一个opencl上下文，成功则使用GUP上下文，否则使用cpu    
    cl_context_properties contextProperties[] =  
    {  
        CL_CONTEXT_PLATFORM,  
        (cl_context_properties)firstPlatformId,  
        0  
    };  
    context = clCreateContextFromType(contextProperties, CL_DEVICE_TYPE_GPU,  
        NULL, NULL, &errNum);  
    if (errNum != CL_SUCCESS)  
    {  
        std::cout << "Could not create GPU context, trying CPU..." << std::endl;  
        context = clCreateContextFromType(contextProperties, CL_DEVICE_TYPE_CPU,  
            NULL, NULL, &errNum);  
        if (errNum != CL_SUCCESS)  
        {  
            std::cerr << "Failed to create an OpenCL GPU or CPU context." << std::endl;  
            return NULL;  
        }  
    }  
  
    return context;  
}  
  
//选择第一个可用的设备并创建一个命令队列  
cl_command_queue CreateOpenCLDLL::CreateCommandQueue(cl_context context, cl_device_id *device)  
{  
    cl_int errNum;  
    cl_device_id *devices;  
    cl_command_queue commandQueue = NULL;  
    size_t deviceBufferSize = -1;  
  
    //这个clGetContextInfo获得设备缓冲区的大小    
    errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES, 0, NULL, &deviceBufferSize);  
    if (errNum != CL_SUCCESS)  
    {  
        std::cerr << "Failed call to clGetContextInfo(...,GL_CONTEXT_DEVICES,...)";  
        return NULL;  
    }  
  
    if (deviceBufferSize <= 0)  
    {  
        std::cerr << "No devices available.";  
        return NULL;  
    }  
  
    //为设备缓冲区分配内存，这个clGetContextInfo用来获得上下文中所有可用的设备    
    devices = new cl_device_id[deviceBufferSize / sizeof(cl_device_id)];  
    errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES, deviceBufferSize, devices, NULL);  
    if (errNum != CL_SUCCESS)  
    {  
        delete[] devices;  
        std::cerr << "Failed to get device IDs";  
        return NULL;  
    }  
    char    deviceName[512];  
    char    deviceVendor[512];  
    char    deviceVersion[512];  
    errNum = clGetDeviceInfo(devices[0], CL_DEVICE_VENDOR, sizeof(deviceVendor),  
        deviceVendor, NULL);  
    errNum |= clGetDeviceInfo(devices[0], CL_DEVICE_NAME, sizeof(deviceName),  
        deviceName, NULL);  
    errNum |= clGetDeviceInfo(devices[0], CL_DEVICE_VERSION, sizeof(deviceVersion),  
        deviceVersion, NULL);  
  
    printf("OpenCL Device Vendor = %s,  OpenCL Device Name = %s,  OpenCL Device Version = %s\n", deviceVendor, deviceName, deviceVersion);  
    // 在这个例子中，我们只选择第一个可用的设备。在实际的程序，你可能会使用所有可用的设备或基于OpenCL设备查询选择性能最高的设备    
    commandQueue = clCreateCommandQueue(context, devices[0], 0, NULL);  
    if (commandQueue == NULL)  
    {  
        delete[] devices;  
        std::cerr << "Failed to create commandQueue for device 0";  
        return NULL;  
    }  
  
    *device = devices[0];  
    delete[] devices;  
    return commandQueue;  
}  
  
//从磁盘加载内核源文件并创建一个程序对象  
cl_program CreateOpenCLDLL::CreateProgram(cl_context context, cl_device_id device, const char* fileName)  
{  
    cl_int errNum;  
    cl_program program;  
  
    std::ifstream kernelFile(fileName, std::ios::in);  
    if (!kernelFile.is_open())  
    {  
        std::cerr << "Failed to open file for reading: " << fileName << std::endl;  
        return NULL;  
    }  
  
    std::ostringstream oss;  
    oss << kernelFile.rdbuf();  
  
    std::string srcStdStr = oss.str();  
    const char *srcStr = srcStdStr.c_str();  
    //创建程序对象    
    program = clCreateProgramWithSource(context, 1,  
        (const char**)&srcStr,  
        NULL, NULL);  
    if (program == NULL)  
    {  
        std::cerr << "Failed to create CL program from source." << std::endl;  
        return NULL;  
    }  
    //编译内核源代码    
    errNum = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);  
    if (errNum != CL_SUCCESS)  
    {  
        // 编译失败可以通过clGetProgramBuildInfo获取日志    
        char buildLog[16384];  
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG,  
            sizeof(buildLog), buildLog, NULL);  
  
        std::cerr << "Error in kernel: " << std::endl;  
        std::cerr << buildLog;  
        clReleaseProgram(program);  
        return NULL;  
    }  
  
    return program;  
}  
  
//创建内存对象   
bool CreateOpenCLDLL::CreateMemObjects(cl_context context, cl_mem memObjects[3],float *a, float *b)  
{  
    //创建内存对象    
    memObjects[0] = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,  
        sizeof(float)* KArraySize, a, NULL);  
    memObjects[1] = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,  
        sizeof(float)* KArraySize, b, NULL);  
    memObjects[2] = clCreateBuffer(context, CL_MEM_READ_WRITE,  
        sizeof(float)* KArraySize, NULL, NULL);  
  
    if (memObjects[0] == NULL || memObjects[1] == NULL || memObjects[2] == NULL)  
    {  
        std::cerr << "Error creating memory objects." << std::endl;  
        return false;  
    }  
  
    return true;  
}  
  
//清理任何创建OpenCL的资源   
void CreateOpenCLDLL::Cleanup(cl_context context, cl_command_queue commandQueue,  
    cl_program program, cl_kernel kernel, cl_mem memObjects[3])  
{  
    for (int i = 0; i < 3; i++)  
    {  
        if (memObjects[i] != 0)  
            clReleaseMemObject(memObjects[i]);  
    }  
    if (commandQueue != 0)  
        clReleaseCommandQueue(commandQueue);  
  
    if (kernel != 0)  
        clReleaseKernel(kernel);  
  
    if (program != 0)  
        clReleaseProgram(program);  
  
    if (context != 0)  
        clReleaseContext(context);  
}  
  
void CreateOpenCLDLL::Calcaute()  
{  
    // 创建opencl上下文和第一个可用平台    
    m_context = CreateContext();  
    if (m_context == NULL)  
    {  
        std::cerr << "Failed to create OpenCL context." << std::endl;  
    }  
  
    // 在创建的一个上下文中选择第一个可用的设备并创建一个命令队列    
    m_commandQueue = CreateCommandQueue(m_context, &m_device);  
    if (m_commandQueue == NULL)  
    {  
        Cleanup(m_context, m_commandQueue, m_program, m_kernel, m_memObjects);  
    }  
  
    // 创建一个程序对象 HelloWorld.cl kernel source    
    m_program = CreateProgram(m_context, m_device, "HelloWorld.cl");  
    if (m_program == NULL)  
    {  
        Cleanup(m_context, m_commandQueue, m_program, m_kernel, m_memObjects);  
    }  
  
    // 创建内核    
    m_kernel = clCreateKernel(m_program, "hello_kernel", NULL);  
    if (m_kernel == NULL)  
    {  
        std::cerr << "Failed to create kernel" << std::endl;  
        Cleanup(m_context, m_commandQueue, m_program, m_kernel, m_memObjects);  
    }  
  
    // 创建一个将用作参数内核内存中的对象。首先创建将被用来将参数存储到内核主机存储器阵列    
    float result[KArraySize];  
    float a[KArraySize];  
    float b[KArraySize];  
    for (int i = 0; i < KArraySize; i++)  
    {  
        a[i] = (float)i;  
        b[i] = (float)(i * 2);  
    }  
  
    if (!CreateMemObjects(m_context, m_memObjects, a, b))  
    {  
        Cleanup(m_context, m_commandQueue, m_program, m_kernel, m_memObjects);  
    }  
  
    // 设置内核参数、执行内核并读回结果    
    m_errNum = clSetKernelArg(m_kernel, 0, sizeof(cl_mem), &m_memObjects[0]);  
    m_errNum |= clSetKernelArg(m_kernel, 1, sizeof(cl_mem), &m_memObjects[1]);  
    m_errNum |= clSetKernelArg(m_kernel, 2, sizeof(cl_mem), &m_memObjects[2]);  
    if (m_errNum != CL_SUCCESS)  
    {  
        std::cerr << "Error setting kernel arguments." << std::endl;  
        Cleanup(m_context, m_commandQueue, m_program, m_kernel, m_memObjects);  
    }  
  
    size_t globalWorkSize[1] = { KArraySize };//让之等于数组的大小    
    size_t localWorkSize[1] = { 1 };  //让之等于1    
  
                                      // 利用命令队列使将在设备执行的内核排队    
  
    m_errNum = clEnqueueNDRangeKernel(m_commandQueue, m_kernel, 1, NULL,  
        globalWorkSize, localWorkSize,  
        0, NULL, NULL);  
    if (m_errNum != CL_SUCCESS)  
    {  
        std::cerr << "Error queuing kernel for execution." << std::endl;  
        Cleanup(m_context, m_commandQueue, m_program, m_kernel, m_memObjects);  
    }  
  
    std::cout << "Executed program succesfully." << std::endl;  
    // Read the output buffer back to the Host    
  
  
    m_errNum = clEnqueueReadBuffer(m_commandQueue, m_memObjects[2], CL_TRUE,  
        0, KArraySize * sizeof(float), result,  
        0, NULL, NULL);  
    if (m_errNum != CL_SUCCESS)  
    {  
        std::cerr << "Error reading result buffer." << std::endl;  
        Cleanup(m_context, m_commandQueue, m_program, m_kernel, m_memObjects);  
    }  
  
  
    //输出结果    
    for (int i = 0; i < KArraySize; i++)  
    {  
        std::cout << result[i] << " ";  
    }  
    std::cout << std::endl;  
    std::cout << "Executed program succesfully." << std::endl;  
    Cleanup(m_context, m_commandQueue, m_program, m_kernel, m_memObjects);  
  
}  
/* 导出函数定义 */  
CreateOpenCLDLL* GetCreateOpenCLDLL(void)  
{  
    CreateOpenCLDLL* pCreateOpenCLDLL = new CreateOpenCLDLL();  
    return pCreateOpenCLDLL;  
}
```
**（4）生成代码**
按生成后，在的debug或者release文件夹中，会生成相应的DLL文件和LIB文件，如下图生成buildopencldll.dll，buildopencldll.lib
![](http://img.blog.csdn.net/20160922181432268?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**2. C++ 多个类的 DLL 封装**
封装好一个类之后，在后面的类可以调用这个类生成的dll，再封装新类的dll。具体过程可以参考第1步。
在工程中添加头文件createopencldll.h到新的工程目录。在debug或者release文件中复制buildopencldll.lib到新的工程目录如下：
![](http://img.blog.csdn.net/20160922181601644?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center) 在项目中，属性->连接器->输入->附加依赖项中，添加buildopencldll.lib
![](http://img.blog.csdn.net/20160922181635769?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
将buildopencldll.dll复制到新工程的debug或者release目录下。
具体新项目的createopencvdll.h代码如下：


```
#pragma once  
#ifdef CREATEOPENCVDLL_EXPORTS  
#define CREATEOPENCVDLL_API __declspec(dllexport)   
#else  
#define CREATEOPENCVDLL_API __declspec(dllimport)   
#endif  
  
#include "createopencldll.h"  
  
#include "opencv2\opencv_modules.hpp"  
#include "opencv2\highgui\highgui.hpp"  
#include "opencv2\core\core.hpp"  
  
using namespace cv;  
  
class CREATEOPENCVDLL_API CreateOpenCVDLL  
{  
public:  
    CreateOpenCVDLL();  
    ~CreateOpenCVDLL();  
    void showImage();  
  
private:  
    Mat m_image;  
};
```
createopencvdll.cpp代码如下：


```
#define CREATEOPENCVDLL_EXPORTS  
  
#include "createopencvdll.h"  
  
CreateOpenCVDLL::CreateOpenCVDLL()  
{  
  
}  
  
CreateOpenCVDLL::~CreateOpenCVDLL()  
{  
  
}  
  
void CreateOpenCVDLL::showImage()  
{  
    m_image = imread("1.jpg");  
    namedWindow("image", CV_WINDOW_AUTOSIZE);  
    imshow("image", m_image);  
    cvWaitKey(0);  
}
```
**3. DLL 的调用**
新建一个win32控制台应用程序，选择控制台应用程序，工程名字为：testdll，新建main函数。
![](http://img.blog.csdn.net/20160922181945298?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**（1）隐式链接**
方法1：
在工程中添加头文件createopencldll.h到新的工程目录。在debug或者release文件中复制buildopencldll.lib到新的工程目录如下：
![](http://img.blog.csdn.net/20160922182133363?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
在项目中，属性->连接器->输入->附加依赖项中，添加buildopencldll.lib
![](http://img.blog.csdn.net/20160922182155723?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
将buildopencldll.dll复制到新工程的debug或者release目录下。
测试代码如下：


```
#include "createopencldll.h"  
  
int main()  
{  
    CreateOpenCLDLL createopencldll;  
    createopencldll.Calcaute();  
    return 0;  
}
```
结果图如下：
![](http://img.blog.csdn.net/20160922182628445?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
方法2：
在方法1的基础下，不添加buildopencldll.lib到附加依赖项中，使用代码的方式添加，即在原来的代码下多添加：#pragmacomment(lib, “buildopencldll.lib”)代码如下：


```
#pragma comment(lib, "buildopencldll.lib")  
  
#include "createopencldll.h"  
  
int main()  
{  
    CreateOpenCLDLL createopencldll;  
    createopencldll.Calcaute();  
    return 0;  
}
```
**（2）显式链接**
参考博客：[http://blog.csdn.net/chollima/article/details/5324808](http://blog.csdn.net/chollima/article/details/5324808)
这个博客先定义虚基类，然后到基类等的实现，这样比较好。
这种方法不需要createopencldll.lib，将createopencldll.dll复制到debug或者release目录。
具体实现代码如下：


```
#include <windows.h>    
#include <cstdio>    
#include "createopencldll.h"  
int main()  
{  
    HMODULE hDll = ::LoadLibrary("buildopencldll.dll");  
    if (NULL != hDll)  
    {  
        PFNGetCreateOpenCLDLL pFun = (PFNGetCreateOpenCLDLL)::GetProcAddress(hDll, "GetCreateOpenCLDLL");  
        if (NULL != pFun)  
        {  
            CreateOpenCLDLL* createOpenCLDLL = (*pFun)();  
            if (NULL != createOpenCLDLL)  
            {  
                createOpenCLDLL->Calcaute();  
                delete createOpenCLDLL;  
            }  
        }  
        ::FreeLibrary(hDll);  
    }  
    return 0;  
}
```
如果出现错误：
错误    C2664       “HMODULELoadLibraryW(LPCWSTR)”: 无法将参数 1 从“const char [19]”转换为“LPCWSTR”    bulidopecvdll    f:\openclproject\bulidopecvdll\bulidopecvdll\main.cpp  18
进入属性->常规->字符集，将使用Unicode字符集改为未设置即可。
参考博客：
[http://blog.csdn.net/xiaowei_cqu/article/details/7737682](http://blog.csdn.net/xiaowei_cqu/article/details/7737682)
[https://msdn.microsoft.com/zh-cn/library/ms235636](https://msdn.microsoft.com/zh-cn/library/ms235636)
[http://www.cnblogs.com/weidiao/p/5013889.html](http://www.cnblogs.com/weidiao/p/5013889.html)
[http://blog.csdn.net/xiamentingtao/article/details/51052918](http://blog.csdn.net/xiamentingtao/article/details/51052918)
[http://www.cnblogs.com/laogao/archive/2012/12/07/2806528.html](http://www.cnblogs.com/laogao/archive/2012/12/07/2806528.html)
[http://blog.csdn.net/chollima/article/details/5324808](http://blog.csdn.net/chollima/article/details/5324808)
