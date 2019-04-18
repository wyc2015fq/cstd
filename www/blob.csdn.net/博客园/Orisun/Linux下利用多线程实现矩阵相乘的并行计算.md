# Linux下利用多线程实现矩阵相乘的并行计算 - Orisun - 博客园







# [Linux下利用多线程实现矩阵相乘的并行计算](https://www.cnblogs.com/zhangchaoyang/articles/1853822.html)





现代的线程库允许不同的线程运行在不同的处理器芯片上，从而实现真正意义上的并行。换句话说，如果你的机子是单核的，用多线程也没不会提高执行效率。

我的电脑是多核的，并行计算耗时0.56秒，但按照常规的串行计算方法只需要0.07秒。

分析一下原因，在我的程序里计算200×300和300×200的两个矩阵的乘积，分了300个子线程去分头完成。线程数太多，每个线程的工作量太小，CPU把时间都花在线程调度上了，所以并行的结果反而还不如串行。

并行程序代码：

```
#include<stdio.h>
#include<time.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<memory.h>
 
/*定义矩阵中元素的上限，避免相乘后溢出*/
#define RANGE 100
/*矩阵A有M行N列，矩阵B有N行M列*/
#define M 200
#define N 300
 
void gene_matrix();
void read_matrix();
int matrixA[M][N];
int matrixB[N][M];
int arr[M][M][N];
void *func(void *arg);
 
main()
{
    gene_matrix();                                      //用随机数产生两个待相乘的矩阵，并分别存入两个文件中
    read_matrix();                                      //从两个文件中读出数据赋给matrixA和matrixB
    clock_t start=clock();                              //开始计时
    int res[M][N]={0};                                  //保存矩阵相乘的结果。非全局变量一定要显示初始化为0,否则为随机的一个数
    int i,j,k;
    pthread_t tids[N];
    for(i=0;i<N;i++)
    { 
        if(pthread_create(&tids[i],NULL,func,(void *)&i))   //分。产生线程，去完成矩阵相乘的部分工作量
        {
            perror("pthread_create");
            exit(1);
        }
    }
    for(i=0;i<N;i++)
        pthread_join(tids[i],NULL);                         //合之前一定要等所有的子线程计算结束
    for(i=0;i<M;i++)                                 //合。
        for(j=0;j<M;j++)
            for(k=0;k<N;k++)
                res[i][j]+=arr[i][j][k];                
    clock_t finish=clock();                             //结束计算
    printf("并行计算用时%.2f秒\n",(long)(finish-start)/1E6);
    exit(0);
}
 
void *func(void *arg)                                   //每个子线程要完成的任务
{
    int k=*(int *)arg;
    int i,j;
    for(i=0;i<M;i++)
        for(j=0;j<M;j++)
            arr[i][j][k]=matrixA[i][k]*matrixB[k][j];
    pthread_exit(NULL);
}
 
void gene_matrix()
{
    FILE *file1,*file2;
    if((file1=fopen("/home/orisun/develop/matrixA","wt"))==NULL)
    {
        perror("fopen");
        exit(1);
    }
    if((file2=fopen("/home/orisun/develop/matrixB","wt"))==NULL)
    {
        perror("fopen");
        exit(1);
    }
    int i,j;
    srand((unsigned)time(NULL));
    for(i=0;i<M;i++)
    {
        for(j=0;j<N;j++)
            fprintf(file1,"%-8d",rand()%RANGE);
        fprintf(file1,"\n");
    }
    fclose(file1);
    for(i=0;i<N;i++)
    {
        for(j=0;j<M;j++)
            fprintf(file2,"%-8d",rand()%RANGE);
        fprintf(file2,"\n");
    }
    fclose(file2);  
}
 
void read_matrix()
{
    FILE *file1,*file2;
    if((file1=fopen("/home/orisun/develop/matrixA","rt"))==NULL)
    {
        perror("fopen");
        exit(1);
    }
    if((file2=fopen("/home/orisun/develop/matrixB","rt"))==NULL)
    {
        perror("fopen");
        exit(1);
    }
    int i,j;
    for(i=0;i<M;i++)
        for(j=0;j<N;j++)
            fscanf(file1,"%d",&matrixA[i][j]);
    fclose(file1);
    for(i=0;i<N;i++)
        for(j=0;j<M;j++)
            fscanf(file2,"%d",&matrixB[i][j]);
    fclose(file2);
}
```

串行代码更改为下：

```
main()
{
    gene_matrix();                                      //用随机数产生两个待相乘的矩阵，并分别存入两个文件中
    read_matrix();                                      //从两个文件中读出数据赋给matrixA和matrixB
    clock_t start=clock();                              //开始计时
    int res[M][N]={0};                                  //保存矩阵相乘的结果。非全局变量一定要显示初始化为0,否则为随机的一个数
    int i,j,k;
    for(i=0;i<M;i++)                                 
        for(j=0;j<M;j++)
            for(k=0;k<N;k++)
                res[i][j]+=matrixA[i][k]*matrixB[k][j];                
    clock_t finish=clock();                             //结束计算
    printf("串行计算用时%.2f秒\n",(long)(finish-start)/1E6);
    exit(0);
}
```

想更多了解并行计算的朋友可以看一下这位老兄的博客[http://blog.csdn.net/drzhouweiming/archive/2009/04/20/4093624.aspx](http://blog.csdn.net/drzhouweiming/archive/2009/04/20/4093624.aspx)《OpenMP编程指南》

```
#include<stdio.h>
#include<time.h>
#include<omp.h>

void main(){
    CRITICAL_SECTION cs;
    int i=0;
    clock_t t1,t2;

    InitializeCriticalSection(&cs);
    t1=clock();
    for(i=0;i<10000000;i++){
        EnterCriticalSection(&cs);
        LeaveCriticalSection(&cs);
    }
    t2=clock();
    printf("One Task,CriticalSection 1,000,000 times,time=%ldms\n",t2-t1);


    t1=clock();
#pragma omp parallel for num_threads(2)
    for(i=0;i<10000000;i++){
        EnterCriticalSection(&cs);
        LeaveCriticalSection(&cs);
    }
    t2=clock();
    printf("Two Task,CriticalSection 1,000,000 times,time=%ldms\n",t2-t1);
    DeleteCriticalSection(&cs);
}
```












