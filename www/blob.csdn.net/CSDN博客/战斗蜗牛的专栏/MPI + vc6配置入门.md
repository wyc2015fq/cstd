# MPI + vc6配置入门 - 战斗蜗牛的专栏 - CSDN博客





2012年12月25日 19:10:36[vbskj](https://me.csdn.net/vbskj)阅读数：2955









MPI的全称是Message Passing Interface即标准消息传递界面，可以用于并行计算。MPI的具体实现一般采用MPICH。下面介绍如何在Windows XP系统下VC6中搭建MPI环境来编写MPI程序。


一.准备工作

1.1   安装MPI的SDK——MPICH2。

mpich2-1.4.1p1-win-ia32安装程序的下载地址：

[http://www.mcs.anl.gov/research/projects/mpich2/downloads/tarballs/1.4.1p1/mpich2-1.4.1p1-win-ia32.msi](http://www.mcs.anl.gov/research/projects/mpich2/downloads/tarballs/1.4.1p1/mpich2-1.4.1p1-win-ia32.msi)


本文以设置安装在C:\Program Files\MPICH2目录下为例。


测试所安装的MPICH2


测试前首先需要注册一个用户，具体操作如下：“开始”按钮-->所有程序-->MPICH2-->wmpiregister.exe。输入用户名、密码。有一点需要说明：该用户名须为有效的操作系统管理员账户，密码对应为系统登录密码。如图所示：

![\](http://www.2cto.com/uploadfile/2011/0927/20110927085405498.jpg)


接下来选择开始-->所有程序-->MPICH2-->wmpiexec.exe；


选择Application为 c:\program files\mpich2\examples\cpi.exe (就是自带的一个计算圆周率的例子程序)。在Number of processes的数量选择2表示用二个进程来协同完成。选中“run in separate windw”选项。再点击Excute就可以执行了。


然后在控制台窗口下提示输入number of intervals ，随便输入个大点的数字（50000，5000000）就可以看到求的的圆周率值。如下图：

![\](http://www.2cto.com/uploadfile/2011/0927/20110927085423535.jpg)


注意到上图中的Show Comman中的字符串，可以在控制台下输入应该字符串得到类似结果，这里就不再介绍了。


1.2 在VC6中添加MPICH2

先在VC6.0中加入mpi的include和lib。VC6.0程序菜单中“Tools” --> “Optains”-->“Directories”然后添加，如下图所示：



![\](http://www.2cto.com/uploadfile/2011/0927/20110927085437503.jpg)






二．第一个MPI程序

加入之后，新建Win32 Console Application工程，加入如下代码：


view plain

#include <mpi.h> 

#include <stdlib.h> 

#include <stdio.h> 

#include <conio.h> 


#pragma comment (lib, "mpi.lib")  


int main(int argc, char* argv[]) 

{ 

    int myid,numprocs; 

    int namelen; 

    char processor_name[MPI_MAX_PROCESSOR_NAME]; 


    MPI_Init(&argc, &argv); 


    //用MPI_Comm_rank 获得进程的rank，该rank值为0到p-1间的整数,相当于进程的ID 

    MPI_Comm_rank(MPI_COMM_WORLD, &myid); 

    //用MPI_Comm_size 获得进程个数  int MPI_Comm_size(MPI_Comm comm, int *size); 

    MPI_Comm_size(MPI_COMM_WORLD, &numprocs); 


    MPI_Get_processor_name(processor_name, &namelen); 


    printf("Hello World! by MoreWindows\nProcess %d of %d on %s\n", myid, numprocs, processor_name); 


    MPI_Finalize(); 


    if (myid == 1) 

    { 

        printf("\nPress a key and exit.\n"); 

        getch(); 

    } 

    return 0; 

} 

编译运行，第一个MPI程序就诞生了。下面是我电脑上的运行结果：

![\](http://www.2cto.com/uploadfile/2011/0927/20110927085453117.jpg)






三．多台电脑上协同运行MPI

只有多台电脑集群后才能更好的发挥并行计算的威力，当然由于MPI的帮助，这个工作将变得异常简单。下面以二台PC协同运行前面提到过的cpi.exe为例。


3.1 多台电脑运行MPI的条件

第一．参加并行计算的机器须至少注册一个相同的账户，如两台PC上都注册了一个 mpi 帐号。


第二．创建JOB目录。每台机器上须有一个路径相同的目录，用于存放待执行的JOB（即 exe 程序），如创建的目录是 d:\MPI\，再将测试用的可执行文件复制到二台PC的该目录下。




3.2 联机执行命令解释


可以在控制台下输入"C:\Program Files\MPICH2\bin\mpiexec.exe" -help2来查看其所以参数，这里不一一介绍了，重点介绍下用于联机操作的命令：


联机执行命令： -hosts n Node_1 Node_2  ...  Node_n  -noprompt ExePath


“-hosts”表示要联机运行，n 表示有n台机器参加运算，Node_1（及Node_2 ...）为各机器的计算机名或IP。ExePath为exe的文件路径如d:\MPI\cpi.exe。


3.3 联机效果示范

![\](http://www.2cto.com/uploadfile/2011/0927/20110927085520115.jpg)


可以这个结果与上面单机运行的结果作个比较。




不难发现，由于MPI的帮助，使用多台电脑来进行并行计算其实也是一件比较容易的事件。



