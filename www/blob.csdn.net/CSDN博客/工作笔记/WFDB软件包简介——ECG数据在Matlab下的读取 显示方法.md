# WFDB软件包简介——ECG数据在Matlab下的读取/显示方法 - 工作笔记 - CSDN博客





2012年01月17日 13:45:48[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：9414








转载自 [http://pfyu.bokee.com/6946657.html](http://pfyu.bokee.com/6946657.html)

**ECG数据在Matlab下的读取/显示方法**





WFDB软件包包含两大组件：

1、WFDB库，支持心电信号读取及分析；

2、WAVE软件，显示、标注及交互。这个开发包都是开源（Open
 Source）的，用C语言开发，且是跨平台的，可以在Windows、Linux、Unix、Mac
 OS等流行的操作系统平台上使用。





WFDB库支持C, C++, Fortran, Java, Matlab, Perl, Python等流行的编程语言。WFDB库支持直接从HTTP服务器或者FTP服务器上读取心电数据。





如果你想在matlab（支持7.0以上的版本，JVM的版本1.4以上）中，使用WFDB库，参见（WFDB
 Toolbox for MATLAB）：

http://www.physionet.org/physiotools/matlab/wfdb-swig-matlab/

安装完毕以后，在matlab中运行demo_wfdb_tools就可以看到使用该库来读取、分析和显示ECG数据的可视化效果。

有关的matlab指令如下：


|rdann|read annotation files for WFDB records|
|----|----|
|rdsamp|read signal files of WFDB records|
|setwfdb|set WFDB paths to default values|
|time2sec|convert WFDB-format time intervals into seconds|
|wfdb_config|return version and compilation information about the WFDB library|
|wfdbdesc|return specifications for signals in WFDB records|
|wfdbwhich|find the location of files belonging to WFDB records|
|wrann|write annotations for WFDB records into annotation files|
|wrsamp|write signal data into WFDB-compatible records|

用户可以在matlab中，通过doc命令，查看这些指令的详细帮助信息。

还可以从这里下载pdf的文档来了解更多的信息：

http://www.physionet.org/physiotools/matlab/wfdb_tools/WFDB_tools/doc/wfdb_tools/wfdb_tools.pdf





如果你想在matlab6.5以上的版本来使用WFDB库，参见：

[http://www.physionet.org/physiotools/matlab/wfdb_tools/](http://www.physionet.org/physiotools/matlab/wfdb_tools/)

但从上述页面中，似乎要注意GCC3.3编译的版本在某些情况下可能会导致Matlab崩溃，而Matlab7.0在某些非Intel的CPU下会遇到问题。解决的办法是：用GCC3.2编译。





WFDB库有关的文档参见：*WFDB Programmer's Guide*







*使用上述的方法来操作*PhysioBank数据库的好处是：如果PhysioBank的数据格式或者读取方式发生变化的话，可以很容易地升级用户的程序。





*另外，在*



[http://www.physionet.org/physiotools/matlab/](http://www.physionet.org/physiotools/matlab/)

还包含了其它用户开发的一些matlab包。这些包没有使用WFDB库。





例如：

[http://www.physionet.org/physiotools/matlab/ECG_Exporter/](http://www.physionet.org/physiotools/matlab/ECG_Exporter/)

用来将ECG数据转换成matlab的.mat数据文件。



