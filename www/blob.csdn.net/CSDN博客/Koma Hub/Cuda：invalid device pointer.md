# Cuda：invalid device pointer - Koma Hub - CSDN博客
2017年09月07日 19:39:33[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：801
个人分类：[CUDA](https://blog.csdn.net/Rong_Toa/article/category/7156216)
cuda编程中调用一个判断cuda是否出错的函数如下：
```cpp
void check_gpu_error (const char *msg) 
{
        cudaError_t err = cudaGetLastError ();
        if (cudaSuccess != err)
        { 
                printf("Cuda error: %s: %s\n", msg, cudaGetErrorString(err)); 
                exit(0);   
        }
}
```
当将GPU与GTK混合编程的时候，每次运行的时候调用这个函数就会出现一下问题：
`Cuda error:  Failed to allocate memory for variables! :  invalid device pointer`
     这个问题来源于每次点图形界面中的按钮都会调用这个函数，所以，在gtk程序部分加上一个runCount就可以解决这个问题，当runCount==0的时候，调用函数，而当runCount~=0的时候，就不调用这个函数，就不会出现这个错误，而且，在第一次进行device初始化后，你会发现，在第二次以及之后运行gpu代码的时候，就不需要程序前几秒的初始化了.
