# Ubuntu11.10下并行程序设计MPI的安装方法和使用 - 三少GG - CSDN博客
2011年11月14日 21:50:19[三少GG](https://me.csdn.net/scut1135)阅读数：2083标签：[ubuntu																[测试](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)
个人分类：[Coding实战](https://blog.csdn.net/scut1135/article/category/909756)
1.安装MPICH
在**新立得**软件包管理器 键入mpi，选择**mpich2、mpich2-doc、libmpich2-dev**等标记安装应用(貌似libmpich2-3也会被安装)
**最新版本为1.4-1**
which mpicc 可查找安装路径
2.测试安装
$ touch hello.c
键入以下内容到hello.c:
#include <mpi.h> 
#include <stdio.h>
int main(int argc, char *argv[]) 
{ 
int npes, myrank; 
MPI_Init(&argc, &argv); 
MPI_Comm_size(MPI_COMM_WORLD, &npes); 
MPI_Comm_rank(MPI_COMM_WORLD, &myrank); 
printf("From process %d out of %d, Hello World!\n", myrank, npes); 
MPI_Finalize();
 }
$ mpicc -o hello hello.c
$ mpirun -np 4 ./hello #应该会输出
From process 2 out of 4, Hello World!
From process 0 out of 4, Hello World!
From process 1 out of 4, Hello World!
From process 3 out of 4, Hello World!
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
第二种安装方法：官网下载源码包，自己编译。
**[mpich2-1.3.2pl.tar.gz下载与安装](http://blog.163.com/lgs_002/blog/static/2873604320114341656372/)**
自己并没有验证，大家可以试下上面这篇文章。
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
参考资料：
1. [http://hi.baidu.com/zhengym02/blog/item/cc2259dc5ce52b4e94ee379f.html](http://hi.baidu.com/zhengym02/blog/item/cc2259dc5ce52b4e94ee379f.html)
2. 安装包官网下载地址： [http://www.mcs.anl.gov.sixxs.org/research/projects/mpich2/downloads/index.php?s=downloads](http://www.mcs.anl.gov.sixxs.org/research/projects/mpich2/downloads/index.php?s=downloads)
3. **MPI安装与运行报告（Win7和Ubuntu） [http://apps.hi.baidu.com/share/detail/32075184](http://apps.hi.baidu.com/share/detail/32075184)**
