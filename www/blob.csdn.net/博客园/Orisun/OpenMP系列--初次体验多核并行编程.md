# OpenMP系列--初次体验多核并行编程 - Orisun - 博客园







# [OpenMP系列--初次体验多核并行编程](https://www.cnblogs.com/zhangchaoyang/articles/2230436.html)





新版的GCC编译器都支持OpenMP。在程序中需要引入omp.h头文件，另外在编译时需要指定-fopenmp选项。

#include<stdio.h>
#include<omp.h>
#include<time.h>

void test(){
    int a=0;
    clock_t t1=clock();
    for(int i=0;i<10000000;i++){
        a=i+1;
    }
    clock_t t2=clock();
    printf("Test Time=%ld\n",t2-t1);
}

int main(int argc,char *argv[]){
    clock_t t1=clock();
#pragma omp parallel for
    for(int j=0;j<2;j++){
        test();
    }
    clock_t t2=clock();
    printf("Total time=%ld\n",t2-t1);

    test();
    return 0;
}


orisun@zcypc:~$ g++ test.c -o ly
orisun@zcypc:~$ ./ly
Test Time=30000
Test Time=30000
Total time=60000
Test Time=30000

可以看到编译时不指定-fopenmp选项，跟串行编程是一样的效果。

orisun@zcypc:~$ g++ test.c -o ly -fopenmp
orisun@zcypc:~$ ./ly
Test Time=70000
Test Time=70000
Total time=70000
Test Time=50000

使用默认的线程数，单独运行test()函数需要50000毫秒，并行再次运行test()函数需要70000毫秒。.

可以显式指定线程数：

orisun@zcypc:~$ export OMP_NUM_THREADS=2
orisun@zcypc:~$ ./ly
Test Time=60000
Test Time=70000
Total time=70000
Test Time=60000

orisun@zcypc:~$ export OMP_NUM_THREADS=4
orisun@zcypc:~$ ./ly
Test Time=60000
Test Time=60000
Total time=70000
Test Time=30000

如何查看默认情况下使用的线程数呢？

#include<stdio.h>
#include<omp.h>

void main(int argc,char * argv[]){
#pragma omp parallel
    {
        printf("Hello OpenMP!ThreadID=%d\n",omp_get_thread_num());
    }
}


orisun@zcypc:~$ gcc num.c -o num -fopenmp
orisun@zcypc:~$ ./num
Hello OpenMP!ThreadID=0
Hello OpenMP!ThreadID=1

说明在我的机子上默认使用2个线程。

或者在程序中指定线程数：

#pragma omp parallel num_threads(8);

这里就有一个问题了：根据第一次运行结果可知不使用OpenMP的话text()函数一次运行只需30000毫秒，而使用OpenMP后，不论线程数设为多少，两次运行test()函数都是需要70000毫秒。----并行反而降慢了速度？

补充一个小技巧--修改gcc的默认编译选项：

在.bashrc中添加一行：export CFLAGS="-O3 -march=core2" && CXXFLAGS=$CFLAGS

这里的core2是<cpu类型>，其实你cat /proc/cpuinfo只要是
cpu family : 6
model : 15, 23,26,37
就可以用core2












