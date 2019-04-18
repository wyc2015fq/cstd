# CUDA 编程实例:计算点云法线 - wishchinYang的专栏 - CSDN博客
2014年01月09日 17:02:04[wishchin](https://me.csdn.net/wishchin)阅读数：1906
程序参考文章：[http://blog.csdn.net/gamesdev/article/details/17535755](http://blog.csdn.net/gamesdev/article/details/17535755)  程序优化2
**简介：**CUDA ，MPI，Hadoop都是并行运算的工具。CUDA是基于NVIDIA GPU芯片计算。
阐述：GPU有很多个核(几百个)，每个核可以跑一个线程，多个线程组成一个单位叫做块。
**举个例子：**
有三个向量 int a, b, c; 我们要计算a和b的向量之和存放到c中。
一般C语言：for(int i=0; i<10; i++)  c = a + b; 这个程序是顺序执行的！
CUDA编程做法：
**GPU中的每个线程(核)有一个独立序号叫index，那么只要序号从0到9的线程执行c[index] = a[index] + b[index]; 就可以实现以上的for循环。**
**GPU的可贵之处就是，可以并发运行多个线程，相当于一个时间内赋值10次。**
```cpp
////////////////////////
cuda.cu
////////////////////////
#include <stdio.h>
#include <cuda_runtime.h>
//运行在GPU
__global__ void vectorADD(int* a, int* b, int* c)
{
     int index = threadIdx.x; //获得当前线程的序号
     if(index < blockDim.x)
         c = a + b;
}
int main ()
{
        //定义10个GPU运算线程
        int N = 10;     
        // 本地开辟三个数组存放我们要计算的内容
        int* h_a = (int*) malloc (N * sizeof(int));
        int* h_b = (int*) malloc (N * sizeof(int));
        int* h_c = (int*) malloc (N * sizeof(int));
        // 初始化数组A, B和C
        for(int i=0; i<N; i++) {
                h_a = i;
                h_b = i;
                h_c = 0;
        }
        // 计算10个int型需要的空间
        int size = N * sizeof(int);   
        // 在GPU上分配同样大小的三个数组
        int* d_a;
        int* d_b;
        int* d_c;
        cudaMalloc((void**)&d_a, size);
        cudaMalloc((void**)&d_b, size);
        cudaMalloc((void**)&d_c, size);
        // 把本地的数组拷贝进GPU内存
        cudaMemcpy(d_a, h_a, size, cudaMemcpyHostToDevice);
        cudaMemcpy(d_b, h_b, size, cudaMemcpyHostToDevice);
        cudaMemcpy(d_c, h_c, size, cudaMemcpyHostToDevice);
        
        // 定义一个GPU运算块 由 10个运算线程组成
        dim3 DimBlock = N;
        // 通知GPU用10个线程执行函数vectorADD
        vectorADD<<<1, DimBlock>>>(d_a, d_b, d_c);
        //  将GPU运算完的结果复制回本地
        cudaMemcpy(h_c, d_c, size, cudaMemcpyDeviceToHost);        
        // 释放GPU的内存
        cudaFree(d_a);
        cudaFree(d_b);
        cudaFree(d_c);
        //  验证计算结果
        for(int j=0; j<N; j++)
                printf("%d ", h_c);
        printf("\n");
}
```
警告！：这个例子是编译不通过的；
首先：对 threadidx的使用，只能在CU文件里面；
其次：在cu文件里初始化数组是错误的： int * a ； a = new int [x]；是错误的；  并且 malloc也是不可以的；
再者：文件路径里面不能包含中文，否则会出现 MSB8791 这种错误！
**2. 利用CUDA并行计算点云法线**
两个函数都存在于CU文件里! 通过外部CPP文件函数进行调用 
```cpp
void normalEstimate(
    pcl::PointCloud<pcl::PointXYZRGB> &input ,
    pcl::PointCloud<pcl::PointXYZRGB> &output,
    int  k_,
    float search_parameter_,
    int THREAD_NUM
    )
```
//运行在GPU//cal the Normal
```cpp
__global__ void normalEstimateSingle(pcl::PointCloud<pcl::PointXYZRGB> &input ,pcl::PointCloud<pcl::PointXYZRGB> &output, int* nn_indices ,int*   nn_dists, int Gap, float search_parameter_)
{  
    const size_t computeSize =input.size() / Gap;  
    const size_t tID = size_t(threadIdx.x );         
    int Mark;
    clock_t startTime; // 开始计时
    if ( tID == 0 ) startTime =clock( );// 选择任意一个线程进行计时     
    //Thread loop!//循环发现邻域！寻找法线！
    for ( size_t idx = tID *computeSize; idx < ( tID + 1 ) * computeSize && idx < input.size(); ++idx )  {  
      // pOut[threadIdx.x] += pIn[i] * pIn[i];  
       Mark = pcl::searchForNeighbors (idx, search_parameter_, nn_indices, nn_dists);//对第IDX个建立索引!
       if (Mark == 0){
                output.points[idx].normal[0] = output.points[idx].normal[1] = output.points[idx].normal[2] = output.points[idx].curvature = std::numeric_limits<float>::quiet_NaN ();
                continue;
            }
       else {
                if (!isFinite (input[idx]) || Mark == 0){
                    output.points[idx].normal[0] = output.points[idx].normal[1] = output.points[idx].normal[2] = output.points[idx].curvature = std::numeric_limits<float>::quiet_NaN ();
                    continue;
                }
            }
       pcl::computePointNormal (input, nn_indices,output.points[idx].normal[0], output.points[idx].normal[1], output.points[idx].normal[2], output.points[idx].curvature);
       pcl::flipNormalTowardsViewpoint (input_->points[idx], vpx_, vpy_, vpz_,
       output.points[idx].normal[0], output.points[idx].normal[1], output.points[idx].normal[2]);
    }   
    if ( tID == 0 ) *pElapsed =clock( ) - startTime;// 结束计时，返回至主程序  
}
```
//运行在CPU端！
```cpp
// as the input
extern "C" void normalEstimate(
    pcl::PointCloud<pcl::PointXYZRGB> &input ,
    pcl::PointCloud<pcl::PointXYZRGB> &output,
    int  k_,
    float search_parameter_,
    int THREAD_NUM
    )
{
     // 在GPU上分配同样大小的三个数组
      pcl::PointCloud<pcl::PointXYZRGB> &inputX ;
      pcl::PointCloud<pcl::PointXYZRGB> &outputX;
       int* nn_indices ;
       int*   nn_dists;
       
    // 1、设置设备  
    cudaError_t cudaStatus = cudaSetDevice( 0 );// 只要机器安装了英伟达显卡，那么会调用成功  
    if ( cudaStatus != cudaSuccess )  
    {  
       fprintf( stderr, "调用cudaSetDevice()函数失败！" );  
       return ;//false;  
    }  
    // 使用CUDA内存分配器分配host端  
    //cudaError_t cudaStatus = cudaMallocHost( &inputX, input.size() * sizeof( pcl::pointXYZRGB ) );  
    //cudaError_t cudaStatus = cudaMallocHost( &outputX, output.size() * sizeof( pcl::Normal ) );  
    // 2、分配显存空间  
    cudaError_t cudaStatus  = cudaMalloc( &inputX, input.size() * sizeof( pcl::pointXYZRGB ) );  
    cudaError_t cudaStatusX = cudaMalloc( &outputX, output.size() * sizeof( pcl::Normal ) );
      // cudaStatus = cudaMalloc( (void**)&pData, DataSize * sizeof( int) );  
       if ( cudaStatus != cudaSuccess)  
       {  
           fprintf( stderr, "调用cudaMalloc()函数初始化显卡中数组时失败！" );  
           break;  
       }  
        // 3、将宿主程序数据复制到显存中  
       cudaError_t cudaStatus2  = cudaMemcpy( inputX, input, input.size() * sizeof( pcl::pointXYZRGB ),cudaMemcpyHostToDevice );  
       cudaError_t cudaStatusX2 = cudaMemcpy(outputX,output,output.size() * sizeof( pcl::pointXYZRGB ),cudaMemcpyHostToDevice );
       if ( cudaStatus != cudaSuccess)  
       {  
           fprintf( stderr, "调用cudaMemcpy()函数初始化宿主程序数据数组到显卡时失败！" );  
           break;  
       }  
       //cudaMalloc( (void**)&nn_dists,   k_ * sizeof( int) );  
       //cudaMalloc( (void**)&nn_indices, k_ * sizeof( int) );  
       //cudaMalloc( (void**)&Normal3f,  3 * sizeof( float) );   
     // 4、执行程序，宿主程序等待显卡执行完毕  
       normalEstimateSingle<<<1, THREAD_NUM>>>( inputX,outputX, nn_indices, nn_dists, THREAD_NUM ,search_parameter_);
       //normalEstimateSingle(pcl::PointCloud<pcl::PointXYZRGB> &input ,pcl::PointCloud<pcl::PointXYZRGB> &output, int* nn_indices ,int*   nn_dists, int Gap)
       
       // 5、查询内核初始化的时候是否出错  
       cudaStatus = cudaGetLastError( );  
       if ( cudaStatus != cudaSuccess)  
       {  
           fprintf( stderr, "显卡执行程序时失败！" );  
           break;  
       }
     // 6、与内核同步等待执行完毕  
       cudaStatus = cudaDeviceSynchronize( );  
       if ( cudaStatus != cudaSuccess)  
       {  
           fprintf( stderr, "在与内核同步的过程中发生问题！" );  
           break;  
       }  
   
       // 7、获取数据  //只复制出法线即可！
       cudaStatus = cudaMemcpy(output,outputX,output.size() * sizeof( pcl::pointXYZRGB ),cudaMemcpyHostToDevice );  
       if ( cudaStatus != cudaSuccess)  
       {  
           fprintf( stderr, "在将结果数据从显卡复制到宿主程序中失败！" );  
           break;  
       }  
       cudaFree( outputX );
       cudaFree(  inputX );
}
```
注意事项：运行在GPU的函数，只能是原子函数，详情请见 《高性能并行编程实践》
