
#define C_DBG
#define CL_USE_DEPRECATED_OPENCL_1_1_APIS
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include "cstd.h"
#include "cfile.h"
#pragma warning( disable : 4996 )
#pragma comment(lib,"OpenCL.lib")

#include <iostream>  
#include <stdio.h>  
#include <string.h>  
#include <string>  
#include <vector>
#include <CL/cl.h>//包含CL的头文件
#include "cstd.h"

//OpenCl 2.1  
//Solve problem: error C4996: 'clCreateCommandQueue': 被声明为已否决  
#pragma warning( disable : 4996 )  

using namespace std;

//从外部文件获取cl内核代码  
bool GetFileData(const char* fname, string& str)
{
	FILE* fp = fopen(fname, "r");
	if (fp == NULL)
	{
		printf("no found filen");
		return false;
	}

	int n = 0;
	while (feof(fp) == 0)
	{
		str += fgetc(fp);
	}

	return true;
}


typedef float test_type;

void vecAdd_cpu(const test_type* HostVector1, const test_type* HostVector2, test_type* HostOutputVector, int n) {
	for (int i = 0; i < n; i++)
	{
		HostOutputVector[i] = (HostVector1[i]* HostVector1[i] + HostVector2[i]* HostVector2[i]);
	}
}

// #include "cv1/core/matrix_decomp.inl"

// <limits>
#define CC_EPSILON(_Tp)  (sizeof(_Tp)==sizeof(float) ? FLT_EPSILON : DBL_EPSILON)

template<typename _Tp> static inline bool
CholImpl(_Tp* A, size_t astep, int m, _Tp* b, size_t bstep, int n)
{
    _Tp* L = A;
    int i, j, k;
    double s;
    //_Tp epsilon = CC_EPSILON(_Tp);
    //astep /= sizeof(A[0]);
    //bstep /= sizeof(b[0]);

    for( i = 0; i < m; i++ )
    {
        for( j = 0; j < i; j++ )
        {
            s = A[i*astep + j];
            for( k = 0; k < j; k++ )
                s -= L[i*astep + k]*L[j*astep + k];
            L[i*astep + j] = (_Tp)(s*L[j*astep + j]);
        }
        s = A[i*astep + i];
        for( k = 0; k < j; k++ )
        {
            double t = L[i*astep + k];
            s -= t*t;
        }
        //if( s < epsilon )            return false;
        L[i*astep + i] = (_Tp)(1./sqrt(s+0.1));
    }

    if (!b)
    {
        for( i = 0; i < m; i++ )
             L[i*astep + i]=1/L[i*astep + i];
        return true;
    }

#if 0
    // LLt x = b
    // 1: L y = b
    // 2. Lt x = y

    /*
     [ L00             ]  y0   b0
     [ L10 L11         ]  y1 = b1
     [ L20 L21 L22     ]  y2   b2
     [ L30 L31 L32 L33 ]  y3   b3

     [ L00 L10 L20 L30 ]  x0   y0
     [     L11 L21 L31 ]  x1 = y1
     [         L22 L32 ]  x2   y2
     [             L33 ]  x3   y3
     */

    for( i = 0; i < m; i++ )
    {
        for( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            for( k = 0; k < i; k++ )
                s -= L[i*astep + k]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }

    for( i = m-1; i >= 0; i-- )
    {
        for( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            for( k = m-1; k > i; k-- )
                s -= L[k*astep + i]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }
    for( i = 0; i < m; i++ )
            L[i*astep + i]=1/L[i*astep + i];
#endif
    return true;
}


int test_cl_add()
{
#include "ocl.txt"
	//先读外部CL核心代码，如果失败则退出。  
	//代码存buf_code里面
	//声明CL所需变量。  
	cl_device_id device;
	cl_platform_id *platform_id = NULL;
	cl_context context;
	cl_command_queue cmdQueue;
	cl_mem bufferA, bufferB, bufferC;
	cl_program program;
	cl_kernel kernel = NULL;
	int elements = 1000000;

	//我们使用的是一维向量  
	//设定向量大小（维数）  
	size_t globalWorkSize[1];
	globalWorkSize[0] = elements;

	cl_int err;

	//定义输入变量和输出变量，并设定初值
	test_type* buf_A = new test_type[elements];
	test_type* buf_B = new test_type[elements];
	test_type* buf_C = new test_type[elements];
	test_type* buf_D = new test_type[elements];

	test_type* buf_A1 = new test_type[elements];
	test_type* buf_B1 = new test_type[elements];
	test_type* buf_C1 = new test_type[elements];
	test_type* buf_D1 = new test_type[elements];

	size_t datasize = sizeof(test_type) * elements;
	int i;
	for (i = 0; i < elements; i++)
	{
		buf_A1[i] = buf_A[i] = (test_type)i;
		buf_B1[i] = buf_B[i] = (test_type)i + 1.0;
		buf_C1[i] = buf_C[i] = (test_type)i + 2.0;
		buf_D1[i] = buf_D[i] = (test_type)i + 3.0;
	}

	//step 1:初始化OpenCL  
	cl_uint num_platform;
	err = clGetPlatformIDs(0, NULL, &num_platform);

	platform_id = new cl_platform_id[num_platform];
	err = clGetPlatformIDs(num_platform, platform_id, NULL);

	if (err != CL_SUCCESS)
	{
		cout << "clGetPlatformIDs error" << endl;
		return 0;
	}

	//博主计算机三个plantform，platform_id[2]为CPU，根据情况来改  
	//clGetDeviceIDs(platform_id[1], CL_DEVICE_TYPE_CPU, 1, &device, NULL);
	clGetDeviceIDs(platform_id[0], CL_DEVICE_TYPE_GPU, 1, &device, NULL);

	//step 2:创建上下文  
	context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);

	//step 3:创建命令队列  
	cmdQueue = clCreateCommandQueue(context, device, 0, NULL);

	//step 4:创建数据缓冲区  
	bufferA = clCreateBuffer(context, CL_MEM_READ_ONLY, datasize, NULL, NULL);
	bufferB = clCreateBuffer(context, CL_MEM_READ_ONLY, datasize, NULL, NULL);
	bufferC = clCreateBuffer(context, CL_MEM_WRITE_ONLY, datasize, NULL, NULL);

	//step 5:将数据上传到缓冲区  
	clEnqueueWriteBuffer(cmdQueue, bufferA, CL_FALSE, 0, datasize, buf_A, 0, NULL, NULL);
	clEnqueueWriteBuffer(cmdQueue, bufferB, CL_FALSE, 0, datasize, buf_B, 0, NULL, NULL);

  int times = 100;
  //set_dbg_out(stdout);
  if (0) {
    //char* buf_code = vecAdd_progstr;
	  //step 6:加载编译代码,创建内核调用函数  
	  program = clCreateProgramWithSource(context, 1, (const char**)&vecAdd_progstr, NULL, NULL);
    clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    kernel = clCreateKernel(program, "vecAdd", NULL);
  
    //step 7:设置参数，执行内核
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufferA);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufferB);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufferC);
    tic;
    for (i = 0; i < times; ++i) {
      // 执行内核
      clEnqueueNDRangeKernel(cmdQueue, kernel, 1, NULL, globalWorkSize, NULL, 0, NULL, NULL);
    }
    toc;
      //step 8:取回计算结果
    clEnqueueReadBuffer(cmdQueue, bufferC, CL_TRUE, 0, datasize, buf_C, 0, NULL, NULL);
    
    
    tic;
    for (i = 0; i < times; ++i) {
      vecAdd_cpu(buf_A, buf_B, buf_D, elements);
    }
    toc;
  }
  if (1) {
	  program = clCreateProgramWithSource(context, 1, (const char**)&CholImpl_progstr, NULL, NULL);
    clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    kernel = clCreateKernel(program, "CholImpl", NULL);
  
    //step 7:设置参数，执行内核  
    int argi = 0;
    int m = 255, n = 255, astep = 255, bstep = 255;
    clSetKernelArg(kernel, argi++, sizeof(cl_mem), &bufferA);
    clSetKernelArg(kernel, argi++, sizeof(astep), &astep);
    clSetKernelArg(kernel, argi++, sizeof(m), &m);
    clSetKernelArg(kernel, argi++, sizeof(cl_mem), &bufferB);
    clSetKernelArg(kernel, argi++, sizeof(bstep), &bstep);
    clSetKernelArg(kernel, argi++, sizeof(n), &n);
    //clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufferC);
    times = 1;
    tic;
    for (i = 0; i < times; ++i) {
      //step 8:取回计算结果
      clEnqueueNDRangeKernel(cmdQueue, kernel, 1, NULL, globalWorkSize, NULL, 0, NULL, NULL);
    }
    toc;
    clEnqueueReadBuffer(cmdQueue, bufferA, CL_TRUE, 0, datasize, buf_A, 0, NULL, NULL);
    clEnqueueReadBuffer(cmdQueue, bufferB, CL_TRUE, 0, datasize, buf_B, 0, NULL, NULL);
    
    tic;
    for (i = 0; i < times; ++i) {
      CholImpl(buf_A1, astep, m, buf_B, bstep, n);
    }
    toc;
    double err = 0;
    for (i=0; i<m*n; ++i) {
      double t = buf_A1[i] - buf_A[i];
      err += t*t;
    }
    printf("err = %f\n", err);
  }
	//输出验证结果
	cout << buf_A[0] << "+" << buf_B[0] << "=" << buf_C[0] << endl;
	cout << buf_A[0] << "+" << buf_B[0] << "=" << buf_D[0] << endl;
	cout << buf_A[elements - 1] << "+" << buf_B[elements - 1] << "=" << buf_C[elements - 1] << endl;
	cout << buf_A[elements - 1] << "+" << buf_B[elements - 1] << "=" << buf_D[elements - 1] << endl;

	//释放所有调用和内存  
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(cmdQueue);
	clReleaseMemObject(bufferA);
	clReleaseMemObject(bufferB);
	clReleaseMemObject(bufferC);
	clReleaseContext(context);

	delete[]platform_id;
	delete[]buf_A;
	delete[]buf_B;
	delete[]buf_C;
	delete[]buf_D;
  tictoc;
	system("pause");
	return 0;
}