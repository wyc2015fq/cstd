# MPI_Get_processor_name，MPI_Get_version - Koma Hub - CSDN博客
2017年11月24日 12:05:52[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：382
个人分类：[C/C++																[HPC																[MPI](https://blog.csdn.net/Rong_Toa/article/category/7158194)](https://blog.csdn.net/Rong_Toa/article/category/7460912)](https://blog.csdn.net/Rong_Toa/article/category/7156199)
```cpp
/**
 *  <Parallel programming technology for high performance computing>
 *
 *               @Copyright    Du Zhihui        :edited
 *                             Li Sanli         :review
 *                             Chen Yu Liu Peng :proofreading
 *
 *    Rong Tao study note:    2017.10
 *
 */
/**
 * mpicc -o outfile Demo.c 
 * mpirun -n 10 ./outfile > out.txt
 *
 */
#include "mpi.h"
#include <stdio.h>
#include <math.h>
int main(int argc,char *argv[])
{
    int myid, numprocs;
    int namelen;
    int version, subversion, ierr;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Get_processor_name(processor_name,&namelen);
    MPI_Get_version(&version, &subversion);
    fprintf(stdout,"stdout Process %d of %d on %s\n",myid, numprocs, processor_name);//mpirun -n 10 ./a > out.txt
    fprintf(stderr,"stderr Process %d of %d on %s\n",myid, numprocs, processor_name);//screen
    fprintf(stderr,"version %d.%d \n",version, subversion);//screen
    perror("rong");//screen
    MPI_Finalize();
}
```
编译与运行：
```
-bash-4.1$ mpicc -o a Demo_01_MPI_Get_processor_name_MPI_Get_version.c
-bash-4.1$ ./a
stdout Process 0 of 1 on AMAX
stderr Process 0 of 1 on AMAX
version 3.0 
rong: Success
-bash-4.1$ mpirun -n 2 ./a
stdout Process 0 of 2 on AMAX
stdout Process 1 of 2 on AMAX
stderr Process 0 of 2 on AMAX
version 3.0 
rong: Success
stderr Process 1 of 2 on AMAX
version 3.0 
rong: Success
-bash-4.1$
```
