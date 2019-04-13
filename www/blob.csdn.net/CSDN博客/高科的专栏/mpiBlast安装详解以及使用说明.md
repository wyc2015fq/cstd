
# mpiBlast安装详解以及使用说明 - 高科的专栏 - CSDN博客

2011年09月07日 15:57:15[高科](https://me.csdn.net/pbymw8iwm)阅读数：3127


## Getting mpiblast
`现在下载包文件：`
`wget http://www.mpiblast.org/downloads/files/mpiBLAST-1.6.0-pio.tgz`
`解压包文件：`
`tar xvzf mpiBLAST*.tgz`
`然后下载ncbi：`
`wget ftp://ftp.ncbi.nih.gov/toolbox/ncbi_tools/old/20061015/ncbi.tar.gz`
解压包
`tar xvzf ncbi.tar.gz`

## Building mpiblast
将ncbi文件move:
mv ncbi mpiblast/ncbi/make/
前进至mpiblast目录：
cd mpiblast
`./ncbi/make/makedis.csh`
(**Note:**If you receive an error about CSH, you don't have the C shell installed, but that's easy to fix with`apt-get install csh`.)
The above command (makedis.csh) needs to be run three times. The first time you run, it will finish with an error:
make: *** No rule to make target `ncbimain.o', needed by `libncbi.a'.  Stop.
Fatal error building NCBI core libraries.
Please be sure that you have X11 and Motif libraries installed.
The NCBI toolkit FAQ at ftp://ftp.ncbi.nih.gov/toolbox/FAQ.html may be helpful.Run the same command again,
`./ncbi/make/makedis.csh`even though it finished with an error. This time, it will take quite a bit longer, and it will finish with a different error:
make: *** No rule to make target `ni_debug.o', needed by `libnetcli.a'.  Stop.
FAILURE primary make status = 0, demo = 0, threaded_demo = 0, net = 2
\#\#\#\#\#\#\#
\#        \#\#\#\#\#   \#\#\#\#\#    \#\#\#\#   \#\#\#\#\#
\#        \#    \#  \#    \#  \#    \#  \#    \#
\#\#\#\#\#    \#    \#  \#    \#  \#    \#  \#    \#
\#        \#\#\#\#\#   \#\#\#\#\#   \#    \#  \#\#\#\#\#
\#        \#   \#   \#   \#   \#    \#  \#   \#
\#\#\#\#\#\#\#  \#    \#  \#    \#   \#\#\#\#   \#    \#Run
`./ncbi/make/makedis.csh`one last time, and it should finish successfully:
Put the date stamp to the file ../VERSION
*********************************************************
*The new binaries are located in ./ncbi/build/ directory*
*********************************************************Then it's safe to continue on to building mpiblast itself.
安装mpiblast(安装在/usr/bin)：
`./configure --prefix=/usr/ --with-ncbi=```pwd`/ncbi`--host=x86_64
make
make install
`检查是否安装成功：`
`which mpiblast`
`更新一下：`
`updatedb`
`locate mpiblast | grep -v src`

`进入当前用户的home路径（如果是一root进入，则进入/root，否则进入home）编辑.ncbirc：`
[mpiBLAST]
Shared=/usr/bin/mpiblast
Local=/usr/bin/mpiblast

### Getting Databases:
`wget ftp://ftp.ncbi.nlm.nih.gov/blast/db/FASTA/drosoph.nt.gz`Then you'll need to untar or just ungzip them. If they end in`.tg.gz`, use to tar to untar the file:
`tar xvzf yourfile.tg.gz`If they just end in`.gz`, they're only gzipped, so unzip them with
`gunzip yourfile.gz`安装
1) 下载openmpi
2) tar -xzvf openmpi-1.4.3.tar.gz
3) cd openmpi-1.4.3
4) ./configure --prefix=/usr/ | tee ../install.log
5) make all | tee ../make.log
6) make install | tee ../install2.log
配置环境
7) vi /etc/ld.so.conf  最后增加一行
/usr/bin
保存退出
8） /sbin/ldconfig   回车等待。
9） vi /etc/profile， 最后 增加一行：
PATH=${PATH}:/usr//bin
保存退出
10）执行
. /etc/profile
测试
10）cd examples
11) make
12) mpirun  -np 4 ./hello_f90              可以得到类似ppt中的输出
mpi多机时， 需要有machines文件。文件内容如下：
tp1 1 /home/pact/mpich/examples/basic/cpi
tp2 1 /home/pact/mpich/examples/basic/cpi
tp3 1 /home/pact/mpich/examples/basic/cpi
分别表示机器名/ip，  进程个数， 进程名（进程名可以没有）； 当使用机器名时， 则需要架设DNS服务器。
mpirun -machinefile ./machine.linux -np 4 ./hello_f90

OK:`使者运行一下：`
`mpiformatdb -i drosoph.nt --nfrags=4 -pF`-ispecifies my input file, drosoph.nt
--nfrags=4tells it to make four fragments
-pspecifies whether the file is a protein file or not. I put F for false, since this is a nucleotide file, not an amino acid file.
Mpiblast will be run just using`mpirun`or`mpiexec`, but there are a few important command line arguments. These include
-d- the database to be queried against
-i- the input file
-p- the type of blast query to run, includingblastnfor nucleotides
blastpfor proteins
-o- the name of the file to save the output in
The command run in its complete state should look something like this:
`mpiexec -np 4 /shared/bin/mpiblast -d drosoph.nt -i test.in -p blastn -o results.txt`Of course, if you're using the[
version of mpiexec with Torque functionality](http://debianclusters.org/index.php/MPICH_with_Torque_Functionality), you'll need to wrap this in a[
qsub script](http://debianclusters.org/index.php/Torque_Qsub_Scripts).
When it's finished running, you should have a new`results.txt`file that looks something like this:




