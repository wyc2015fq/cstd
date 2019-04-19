# MPI_Send和MPI_Recv（初识1） - Koma Hub - CSDN博客
2017年11月24日 12:21:49[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：667
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
 * mpirun -n 2 ./outfile > out.txt
 *
 */
#include "mpi.h"
#include <stdio.h>
#include <math.h>
int main(int argc,char *argv[])
{
    int myid, numprocs;
    char message[20];
    MPI_Status status;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    if(myid == 0)
    {
        strcpy(message, "hello, process I");
        MPI_Send(message, 20, MPI_CHAR, 1, 999, MPI_COMM_WORLD);
    }
    else if(myid == 1)
    {
        MPI_Recv(message, 20, MPI_CHAR, 0, 999, MPI_COMM_WORLD, &status);
        printf("received : %s\n",message);
    }
    MPI_Finalize();
}
/**
MPI_INTEGER 
MPI_REAL 
MPI_DOUBLE_PRECISION 
MPI_COMPLEX 
MPI_LOGICAL 
MPI_CHARACTER 
MPI_BYTE 
MPI_PACKED
 */
```
编译与运行：
```
-bash-4.1$ mpicc -o a Demo_02_MPI_Send_MPI_Recv_01.c
-bash-4.1$ mpirun -n 2 ./a
received : hello, process I
-bash-4.1$
```
再一个例子：
这个例子展示了所有节点互相通信，直至到rank号最大的节点终止
```cpp
#include "mpi.h"
#include <stdio.h>
#include <math.h>
int main(int argc,char *argv[])
{
    int myid, numprocs;
    int value;
    MPI_Status status;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    do{
        if(myid == 0)
        {
            scanf("%d",&value);
            if(numprocs > 1)
            {
                MPI_Send(&value, 1, MPI_INT, myid+1, 0, MPI_COMM_WORLD);
                fprintf(stderr,"%d send (%d) to %d\n",myid, value, myid+1);
            }
        }
        else
        {
            MPI_Recv(&value, 1, MPI_INT, myid-1, 0, MPI_COMM_WORLD, &status);
            fprintf(stderr,"%d receive (%d) to %d\n",myid, value, myid+1);
            if(myid < numprocs-1)
            {
                MPI_Send(&value, 1, MPI_INT, myid+1, 0, MPI_COMM_WORLD);
                fprintf(stderr,"%d send (%d) to %d\n",myid, value, myid+1);
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }while(value >= 0);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}
```
编译与运行：
```
-bash-4.1$ mpicc -o  a Demo_02_MPI_Send_MPI_Recv_02.c
-bash-4.1$ mpirun -n 3 ./a
1
0 send (1) to 1
1 receive (1) to 2
1 send (1) to 2
2 receive (1) to 3
2
0 send (2) to 1
1 receive (2) to 2
1 send (2) to 2
2 receive (2) to 3
-1
0 send (-1) to 1
1 receive (-1) to 2
1 send (-1) to 2
2 receive (-1) to 3
-bash-4.1$
```
