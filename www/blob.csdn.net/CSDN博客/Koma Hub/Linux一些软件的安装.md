# Linux一些软件的安装 - Koma Hub - CSDN博客
2017年09月07日 19:55:52[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：286
                
当然，最直接有效的方法是sudo安装(sudo apt-get install + softwarename)
以下是一些软件的安装过程笔记，还希望有用吧！也给自己留个备份的，哈哈哈。
```
1.sac/sac2000
  
   install in: /opt/sac
####################change the install position#########################
   install in: /usr/local/sac
             procedures:vi .bashrc     
                          export PATH=/usr/local/sac/bin:$PATH
                          export SACAUX=/usr/local/sac/aux 
                        Esc + :wq
                        source .bashrc    #save the .bashrc
########################################################################
---------------------------------------------------------------------
```
```
2.GMT 
    install in: apt-get install GMT
```
```
3.pssac2
   copy the 'pssac2' to '/opt' (#'recf' is the name of the source document)
```
```
4.Installing the source code of the 'recf'
   input the following command: 
    $cd recFunc/RForward/
    $cd Subs
    $make     (modified the 'Makefile' is needed,
                 because 'sac' install in 'usr')
    $cd ../Decon 
    $make     (same with up)
    $cd ../RespKennett
    $make     (same with up)
    $cd ../Utilities
    $make     (same with up)   -----this procedure isn't complete yet.
```
```
5.Creat the document '/opt'
    $cd ~
    $mkdir bin
```
```
6.lupei.o
   $cd ~
   $cp -a /home/rongtao/Desktop/software/recf/lupeiStack bin
     get in the 'bin' you just built  ($cd---)
   $gcc -o lupei.o lupeiStack.c -lm
     copy the 'lupei.o' to 'bin' ,so that other shell can use it.
```
```
7.ttimes and evt2sac
     copy these two to 'bin','ttimes' doesn't need configure,'evt2sac' need.
   $cd evt2sac
   $make
```
```
3.rdseed
   insrall in: /usr/local/rdseed
             procedure:make clean + vi Makefile +
                       change the "#" between 32 and 64 +
                       vi .bashrc +
                       export PATH=/usr/local/rdseed +
                       Esc + :wq
```
```
4.Madagascar   (wrong install,so make installing again,see in the following part)
           sudo apt-get install freeglut3-dev g++ gfortran libgd2-xpm-dev libglew1.5-dev
                            there are some problems about installing (suggested packages:
                                                                         libxcb-doc
                                                                            .....
           sudo apt-get install libjpeg62-dev libx11-dev
           sudo apt-get install libxaw7-dev libnetpbm10-dev swig python-dev python-scipy python-numpy
           sudo apt-get install libtiff4-dev scons units libblas-dev
           sudo apt-get install libcairo2-dev libavcodec-dev libplplot-dev
                            there are some problem too.
           cp -a /Desktop/madagascar /usr
           cd madagascar
           scons    (problem?)
           scons install
           vi .bashrc +
```
```
5.Madagascar(try installing again,follow the 'INSTALL.txt')
           
             (c complier and Python interpreter have not ready yet)
             cd /usr/madagascar
             ./configure --prefix=/usr/madagascar/install 
             ./configure --prefix=/usr/madagascar/install \API fortran-90
             make   ...Done
             make install
             source /usr/madagascar/install/share/madagascar/etc/env.sh
             #source /usr/madagascar/install/share/madagascar/etc/env.csh
             install is Done,the following code is try to test the installing successfully or not.
             (sfin
     sfattr
     sfspike
     sfbandpass
     sfwiggle)
```
```
6.Seismic Unix
    
         sometimes when i use the Madagascar smoothly,and consider the SU is necessary or not...
         2015/10/30
         installing the sesmic unix today,
           修改~/.bashrc
           export CWPROOT=/home/rongtao/su            
           export PATH=$PATH:/home/rongtao/su/bin 
           make install
           make xtinstall
           make finstall
           make mglinstall
       There still some thing wrong!
```
```
7.mpich2 
         installing in '/home/mpi/mpich/src'
        procedure:
           ./configure -prefix=/home/mpi/mpich2
           sudo make
           sudo make install
           modify the '~/.bashrc'  
                 +export MPI_ROOT=/home/mpi/mpich2
                 +export PATH=$MPI_ROOT/bin:$PATH
                 +export MANPATH=$MPI_ROOT/man:$MANPATH
           (then you can configure your own code)
           mpicc -o hello hello.c
           mpirun -np 4 ./hello
           
       How to run the hello?
           cd $HOME
           touch .mpd.conf
           chmod 600 .mpd.conf
          (then )
           mpdboot
           cd ~
           touch .mpd.conf
           chmod 600 .mpd.conf
          (then )
           mpirun -np 4 ./hello
 
        (come from internet: http://www.cnblogs.com/liyanwei/archive/2010/04/26/1721142.html  )
```
*******************there a code you can learn**************************
```cpp
#include "mpi.h"
int main (int argc, char **argv)
{
int myid, numprocs;
int namelen;
char processor_name[MPI_MAX_PROCESSOR_NAME];
MPI_Init (&argc, &argv);
MPI_Comm_rank (MPI_COMM_WORLD, &myid);
MPI_Comm_size (MPI_COMM_WORLD, &numprocs);
MPI_Get_processor_name (processor_name, &namelen);
fprintf (stderr, "Hello World! Process %d of %d on %s\n", myid, numprocs, processor_name);
MPI_Finalize ();
return 0;
}
```
