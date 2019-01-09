#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>

#ifdef __APPLE__
#include <opencl/cl.h>
#else
#include <CL/cl.h>
#endif

#pragma comment (lib,"OpenCL.lib")

const int ARRAY_SIZE = 10000;

//为cpu平台创建上下文
cl_context createContext()
{
  cl_platform_id firstPlatformId = 0;
  cl_uint numPlatforms = 0;
  //这里选择第一个平台
  cl_int errNum = clGetPlatformIDs(1, &firstPlatformId, &numPlatforms);
  if (errNum != CL_SUCCESS || numPlatforms <= 0)
  {
    std::cerr << "Failed to find any opencl platforms." << std::endl;
    return NULL;
  }
  else
  {
    std::cout << "有opencl平台" << std::endl;
  }

  //创建平台的一个上下文，先试图创建一个gpu的，如果没有的话，就创建cpu的
  cl_context_properties contextProperties[] =
  {
    CL_CONTEXT_PLATFORM,
    (cl_context_properties)firstPlatformId,
    0
  };
  cl_context context = clCreateContextFromType(contextProperties, CL_DEVICE_TYPE_GPU, NULL, NULL, &errNum);
  if (errNum != CL_SUCCESS)
  {
    std::cout << "不能创建gpu上下文 ，尝试CPU..." << std::endl;
    context = clCreateContextFromType(contextProperties, CL_DEVICE_TYPE_CPU, NULL, NULL, &errNum);
    if (errNum != CL_SUCCESS)
    {
      std::cerr << "不能创建opencl gpu或者cpu上下文";
      return NULL;
    }
    else
    {
      std::cout << "能创建cpu上下文" << std::endl;
    }
  }
  else
  {
    std::cout << "能创建gpu上下文" << std::endl;
  }
  return context;
}
//选择第一个可用设备，并创建一个命令队列
cl_command_queue createCommandQueue(cl_context context, cl_device_id & device)
{
  size_t deviceBufferSize = -1;
  cl_int errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES, 0, NULL, &deviceBufferSize);
  if (errNum != CL_SUCCESS)
  {
    std::cerr << "不能获取设备缓冲大小" << std::endl;
    return NULL;
  }
  else
  {
    std::cout << "成功获取设备缓冲大小" << std::endl;
  }

  //为设备缓存分配空间
  cl_device_id * devices = new cl_device_id[deviceBufferSize / sizeof(cl_device_id)];
  errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES, deviceBufferSize, devices, NULL);
  if (errNum != CL_SUCCESS)
  {
    std::cerr << "不能得到设备id集合" << std::endl;
    return NULL;
  }
  else
  {
    std::cout << "可以得到设备id集合" << std::endl;
  }

  //这里只选择第一个可用的设备，在该设备创建一个命令队列.这个命令队列用于将程序中要执行的内核排队，并读回结果
  cl_command_queue commandQueue = clCreateCommandQueue(context, devices[0], 0, NULL);
  if (commandQueue == NULL)
  {
    std::cerr << "不能为设备0创建命令队列" << std::endl;
    return NULL;
  }
  else
  {
    std::cout << "能为设备0创建命令队列" << std::endl;
  }
  device = devices[0];
  delete[] devices;
  return commandQueue;

}

//从磁盘加载内核源文件创建和构建一个程序对象
cl_program createProgram(cl_context context, cl_device_id device, const char* fileName)
{
  std::ifstream kernelFile(fileName, std::ios::in);
  if (!kernelFile.is_open())
  {
    std::cerr << "不能打开文件" << fileName << std::endl;
    return NULL;
  }
  else
  {
    std::cout << "成功打开文件" << fileName << std::endl;
  }

  std::ostringstream oss;
  oss << kernelFile.rdbuf();
  std::string srcStdStr = oss.str();
  const char * srcStr = srcStdStr.c_str();
  //创建程序对象
  cl_program program = clCreateProgramWithSource(context, 1, (const char**)&srcStr, NULL, NULL);
  if (program == NULL)
  {
    std::cerr << "不能从源文件创建opencl程序对象" << std::endl;
    return NULL;
  }
  else
  {
    std::cout << "能从源文件创建opencl程序对象" << std::endl;
  }

  //编译内核源码
  cl_int errNum = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
  if (errNum != CL_SUCCESS)
  {
    //判断错误原因
    char buildLog[16384];
    clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buildLog), buildLog, NULL);
    std::cerr << "内核错误:" << std::endl;
    std::cerr << buildLog;
    clReleaseProgram(program);
    return NULL;
  }
  else
  {
    std::cout << "编译内核源码成功" << std::endl;
  }
  return program;
}

//从磁盘加载内核源文件创建和构建一个程序对象
cl_program createProgramFromSrc(cl_context context, cl_device_id device, const char* srcStr)
{
  //创建程序对象
  cl_program program = clCreateProgramWithSource(context, 1, (const char**)&srcStr, NULL, NULL);
  if (program == NULL)
  {
    std::cerr << "不能从源文件创建opencl程序对象" << std::endl;
    return NULL;
  }
  else
  {
    std::cout << "能从源文件创建opencl程序对象" << std::endl;
  }

  //编译内核源码
  cl_int errNum = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
  if (errNum != CL_SUCCESS)
  {
    //判断错误原因
    char buildLog[16384];
    clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buildLog), buildLog, NULL);
    std::cerr << "内核错误:" << std::endl;
    std::cerr << buildLog;
    clReleaseProgram(program);
    return NULL;
  }
  else
  {
    std::cout << "编译内核源码成功" << std::endl;
  }
  return program;
}
//创建内存对象，在设备内存中，可以由内核函数直接访问
bool createMemObjects(cl_context context, cl_mem memObjects[3], float * a, float *b)
{
  memObjects[0] = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float) * ARRAY_SIZE, a, NULL);
  memObjects[1] = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float) * ARRAY_SIZE, b, NULL);
  memObjects[2] = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(float) * ARRAY_SIZE, NULL, NULL);
  if (memObjects[0] == NULL ||
    memObjects[1] == NULL ||
    memObjects[2] == NULL
    )
  {
    std::cerr << "错误创建内存对象" << std::endl;
    return false;
  }
  return true;
}

//清空资源
void cleanUp(cl_context context, cl_command_queue commandQueue, cl_program program, cl_kernel theKernel, cl_mem memObjects[3])
{
  for (size_t i = 0; i < 3; i++)
  {
    if (memObjects[i] != 0)
    {
      clReleaseMemObject(memObjects[i]);
    }
  }
  if (commandQueue != 0)
  {
    clReleaseCommandQueue(commandQueue);
  }

  if (theKernel != 0)
  {
    clReleaseKernel(theKernel);
  }
  if (program != 0)
  {
    clReleaseProgram(program);
  }
  if (context != 0)
  {
    clReleaseContext(context);
  }
}


const char* test_str = //
"__kernel void hello_kernel(__global const float * a,"
"  __global const float * b,"
"  __global float* result)"
"{"
"  int gid = get_global_id(0);"
"  result[gid] = a[gid] + b[gid];"
"}";


int test_opencl(int argc, char ** argv)
{
  time_t startTime1;
  time_t endTime1;
  time_t endTime2;
  time(&startTime1);
  std::string fileName = "test.cl";
  cl_mem memObjects[3] = { 0, 0, 0 };
  cl_context theContext = createContext();
  cl_device_id  device = NULL;
  cl_command_queue theQueue = createCommandQueue(theContext, device);
#if 0
  cl_program theProgram = createProgram(theContext, device, fileName.c_str());
#else
  cl_program theProgram = createProgramFromSrc(theContext, device, test_str);
#endif
  //创建opencl内核
  cl_kernel theKernel = clCreateKernel(theProgram, "hello_kernel", NULL);
  if (theKernel == NULL)
  {
    std::cerr << "不能创建内核" << std::endl;
    cleanUp(theContext, theQueue, theProgram, theKernel, memObjects);
    return 1;
  }
  else
  {
    std::cout << "成功创建内核" << std::endl;
  }

  float result[ARRAY_SIZE];
  float a[ARRAY_SIZE];
  float b[ARRAY_SIZE];
  for (size_t i = 0; i < ARRAY_SIZE; i++)
  {
    a[i] = (float)i;
    b[i] = (float)(i * 2);
  }
  if (!createMemObjects(theContext, memObjects, a, b))
  {
    std::cout << "创建内存对象失败" << std::endl;
    cleanUp(theContext, theQueue, theProgram, theKernel, memObjects);
    return 1;
  }
  else
  {
    std::cout << "创建内存对象成功" << std::endl;
  }

  //建立内核参数
  cl_int errNum = clSetKernelArg(theKernel, 0, sizeof(cl_mem), &memObjects[0]);
  errNum |= clSetKernelArg(theKernel, 1, sizeof(cl_mem), &memObjects[1]);
  errNum |= clSetKernelArg(theKernel, 2, sizeof(cl_mem), &memObjects[2]);
  if (errNum != CL_SUCCESS)
  {
    std::cerr << "错误设置内核参数" << std::endl;
    cleanUp(theContext, theQueue, theProgram, theKernel, memObjects);
    return 1;
  }
  else
  {
    std::cout << "正确设置内核参数" << std::endl;
  }

  //使用命令队列使将在设备上执行的内核排队
  size_t globalWorkSize[1] = { ARRAY_SIZE };
  size_t localWorkSize[1] = { 1 };
  errNum = clEnqueueNDRangeKernel(theQueue, theKernel, 1, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);
  if (errNum != CL_SUCCESS)
  {
    std::cerr << "错误的执行队列内核" << std::endl;
    cleanUp(theContext, theQueue, theProgram, theKernel, memObjects);
    return 1;
  }
  else
  {
    std::cout << "生成正确的执行队列内核" << std::endl;
  }

  //从内核读回结果
  errNum = clEnqueueReadBuffer(theQueue, memObjects[2], CL_TRUE, 0, ARRAY_SIZE * sizeof(float), result, 0, NULL, NULL);
  if (errNum != CL_SUCCESS)
  {
    std::cerr << "错误读取结果缓冲" << std::endl;
    cleanUp(theContext, theQueue, theProgram, theKernel, memObjects);
    return 1;
  }

  cleanUp(theContext, theQueue, theProgram, theKernel, memObjects);
  time(&endTime1);
  double deltaTime1 = difftime(endTime1, startTime1);

  //串行
  float result2[ARRAY_SIZE];
  float a2[ARRAY_SIZE];
  float b2[ARRAY_SIZE];
  for (size_t i = 0; i < ARRAY_SIZE; i++)
  {
    a2[i] = (float)i;
    b2[i] = (float)(i * 2);
    result2[i] = a2[i] + b2[i];
  }

  time(&endTime2);
  double deltaTime2 = difftime(endTime2, endTime1);
  std::cout << "并行时间：" << deltaTime1 << std::endl;
  std::cout << "串行时间：" << deltaTime2 << std::endl;

  return 0;
}

