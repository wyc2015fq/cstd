# matlab 中配置libsvm  的方法。。。。 - 家家的专栏 - CSDN博客





2011年03月03日 19:43:00[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：3302标签：[matlab																[compiler																[microsoft																[fortran																[下载工具																[interface](https://so.csdn.net/so/search/s.do?q=interface&t=blog)




下载工具箱[libsvm-mat-2.89-3]
安装方法也很简单,解压文件,把当前工作目录调整到libsvm所在的文件夹下,再在set path里将libsvm所在的文件夹加到里面.然后
在命令行里输入

>> mex-setup
    Usage: 
        MEX [option1 ... optionN] sourcefile1 [... sourcefileN] 
            [objectfile1 ... objectfileN] [libraryfile1 ... libraryfileN] 

      or (to build an Ada S-function): 
        MEX [-v] [-g] -ada <sfcn>.ads 

    Use the -help option for more information, or consult the MATLAB API Guide. 


  D:/MATLAB7/BIN/WIN32/MEX.PL: Error: No file names given. 

??? Undefined function or variable 'setup'.

>> clear all
>> mex -setup
Please choose your compiler for building external interface (MEX) files: 

Would you like mex to locate installed compilers [y]/n? y

Select a compiler: 
[1] Digital Visual Fortran version 6.0 in C:/Program Files/Microsoft Visual Studio 
[2] Lcc C version 2.4 in D:/MATLAB7/sys/lcc 
[3] Microsoft Visual C/C++ version 6.0 in C:/Program Files/Microsoft Visual Studio 

[0] None 

Compiler: 3

Please verify your choices: 

Compiler: Microsoft Visual C/C++ 6.0 
Location: C:/Program Files/Microsoft Visual Studio 

Are these correct?([y]/n): y

Try to update options file: C:/Documents and Settings/Administrator/Application Data/MathWorks/MATLAB/R14/mexopts.bat 
From template:              D:/MATLAB7/BIN/WIN32/mexopts/msvc60opts.bat 

Done . . . 

>> make](https://so.csdn.net/so/search/s.do?q=下载工具&t=blog)](https://so.csdn.net/so/search/s.do?q=fortran&t=blog)](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)](https://so.csdn.net/so/search/s.do?q=compiler&t=blog)](https://so.csdn.net/so/search/s.do?q=matlab&t=blog)




