# Windows编译Caffe-Windows 含Matlab支持 - DumpDoctorWang的博客 - CSDN博客





2019年02月26日 11:36:14[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：68











### 目录
- [一、环境说明](#_1)
- [二、编译Caffe-Windows](#CaffeWindows_25)
- [1、下载资源](#1_26)
- [2、编辑CMakeLiss.txt](#2CMakeLisstxt_38)
- [3、编辑build_win.cmd](#3build_wincmd_67)
- [4、执行build_win.cmd](#4build_wincmd_134)
- [5、编译Caffe](#5Caffe_216)
- [三、使用Caffe](#Caffe_229)
- [1、Matlab](#1Matlab_230)
- [2、Python](#2Python_242)
- [3、其他](#3_244)




# 一、环境说明

也是安装顺序。特别强调的是**除VS2015以外，其他软件的安装路径都最好不要包含空格。**

1、Windows 64位系统。

2、Visual Studio 2015(VS2015, 对应VC14)。

3、Matlab 2017a。Matlab的版本倒不是很重要，只要支持Matlab 2015a之后的版本都应该支持。所以如果安装了其他版本的Matlab，可以先不用安装2017a。安装完之后要把其中的bin目录和runtime\bin目录加入环境变量Path。
```
D:\Program Files\MATLAB\R2017a\bin
D:\Program Files\MATLAB\R2017a\runtime\win64
```

4、CMake 3.14。安装时要选择将CMake添加到环境变量Path，然后下面的路径就会自动添加到环境变量Path。

```
C:\Program Files\CMake\bin
```

5、[Miniconda2](https://conda.io/en/latest/miniconda.html)的[Python2.7](https://repo.anaconda.com/miniconda/Miniconda2-latest-Windows-x86_64.exe)版本。安装的时候记得勾选add anaconda to environment path。然后下面的路径就会自动添加到环境变量Path。安装完conda之后，记得把conda的源换成国内的源，否则下载库会特别慢。

```
D:\Libs\MiniConda
D:\Libs\MiniConda\Library\mingw-w64\bin
D:\Libs\MiniConda\Library\usr\bin
D:\Libs\MiniConda\Library\bin
D:\Libs\MiniConda\Scripts
```

安装完成后，打开命令行，输入cmake和python --version，检查cmake是否工作以及python版本是否正确。如果Python的版本不是2.7，请在环境变量中删除其他Python版本的路径。

# 二、编译Caffe-Windows

## 1、下载资源
- Caffe-Windows

打开[https://github.com/BVLC/caffe](https://github.com/BVLC/caffe)，下载源码（可直接[点击下载](https://github.com/BVLC/caffe/archive/master.zip)）。
- libraries_v140_x64_py27_1.1.0.tar.bz2

这个是caffe要依赖的库，[点击下载](https://github.com/willyd/caffe-builder/releases/download/v1.1.0/libraries_v140_x64_py27_1.1.0.tar.bz2)。v140对应VS2015，x64对应系统架构，py27对应python版本。下载完成之后，打开libraries_v140_x64_py27_1.1.0.tar.bz2所在目录，按住Shift键，右键单击空白处，然后选择“在此处打开powershell窗口”，打开命令行，执行下面的命令：

```
cmd
md %USERPROFILE%\.caffe\dependencies\download
copy libraries_v140_x64_py27_1.1.0.tar.bz2 %USERPROFILE%\.caffe\dependencies\download
```

执行上面的命令就是把libraries_v140_x64_py27_1.1.0.tar.bz2复制到%USERPROFILE%\.caffe\dependencies\download这个目录里面去。%USERPROFILE%是当前用户的目录。可在命令行中使用`echo %USERPROFILE%`来查看。

## 2、编辑CMakeLiss.txt
- 解压caffe-windows，打开caffe-windows\CMakeLiss.txt，在：

```
# ---[ Caffe project
project(Caffe C CXX)
```

之前加入如下几行：

```
# ---[Install directory: D:\Libs\Caffe
set(CMAKE_INSTALL_PREFIX "D:/Libs/Caffe")

# ---[Set Matlab_DIR
set(Matlab_DIR "D:/Program Files/MATLAB/R2017a")
```

CMAKE_INSTALL_PREFIX 指定Caffe的安装路径，Matlab_DIR 指定Matlab的根目录。
- 打开caffe-windows\cmake\Utils.cmake，把357行的：

```
message(FATAL_ERROR "Logic error. Need to update cmake script")
```

改为：

```
message(WARNING "Logic error. Need to update cmake script")
```

## 3、编辑build_win.cmd
- 打开caffe-windows\scripts\build_win.cmd，如下就是编译选项部分：

```
:: Default values
if DEFINED APPVEYOR (
    echo Setting Appveyor defaults
    if NOT DEFINED MSVC_VERSION set MSVC_VERSION=14
   ...
```

在if DEFINED APPVEYOR之前添加如下几行：

```
:: Default values
set WITH_NINJA=0
set BUILD_MATLAB=1  
set CPU_ONLY=1    
set PYTHON_VERSION=2
set BUILD_PYTHON_LAYER=1
if DEFINED APPVEYOR (
    echo Setting Appveyor defaults
    if NOT DEFINED MSVC_VERSION set MSVC_VERSION=14
    if NOT DEFINED WITH_NINJA set WITH_NINJA=0
    ...
```

WITH_NINJA：不使用NINJA编译；BUILD_MATLAB：编译Matlab模块，CPU_ONLY：不适用CUDA加速；PYTHON_VERSION：指定python版本。BUILD_PYTHON_LAYER：编译pycaffe模块。
- 然后设置conda的安装目录，把

```
:: Set python 2.7 with conda as the default python
    if !PYTHON_VERSION! EQU 2 (
        set CONDA_ROOT=C:\Miniconda-x64
    )
```

修改为

```
:: Set python 2.7 with conda as the default python
    if !PYTHON_VERSION! EQU 2 (
        set CONDA_ROOT=D:\Libs\MiniConda
    )
```

D:\Libs\MiniConda是Conda的安装路径。
- 然后在

```
if ERRORLEVEL 1 (
  echo ERROR: Configure failed
  exit /b 1
)
```

后面添加一行

```
if ERRORLEVEL 1 (
  echo ERROR: Configure failed
  exit /b 1
)

exit /b 1

:: Lint
...
```

`exit /b 1`是退出cmd的命令，添加这行的目的是观察cmake的结果。

## 4、执行build_win.cmd
- 安装pycaffe的依赖：

```
conda install --yes pip
pip install ninja numpy scipy protobuf==3.1.0 six scikit-image pyyaml pydotplus graphviz
```

在执行这一步之前，建议先把conda的源换成国内的源。
- 进入caffe-windows\scripts目录，空白处按住shift+点鼠标右键，然后打开powershell，输入：

```
cmd
build_win.cmd
build_win.cmd
```

对，build_win.cmd就是要执行两遍，因为第一遍的安装目录不正确。输出类似为（我把需要相同的部分标记出来， #后面就是我的注释）：

```
......
#  需要找到Matlab
-- Found Matlab: D:/Program Files/MATLAB/R2017a/extern/include (found version "9.2") found components:  MAIN_PROGRAM MX_LIBRARY   
-- Found Git: C:/Program Files/Git/cmd/git.exe (found version "2.19.1.windows.1")
--
-- ******************* Caffe Configuration Summary *******************
-- General:
--   Version           :   1.0.0
--   Git               :   unknown
--   System            :   Windows
--   C++ compiler      :   C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/bin/x86_amd64/cl.exe
--   Release CXX flags :   /MD /O2 /Ob2 /DNDEBUG /DWIN32 /D_WINDOWS /W3 /GR /EHsc
--   Debug CXX flags   :   /MDd /Zi /Ob0 /Od /RTC1 /DWIN32 /D_WINDOWS /W3 /GR /EHsc
--   Build type        :   Release    #  Release模式
--
--   BUILD_SHARED_LIBS :   0    #  需要相同
--   BUILD_python      :   1    #  需要相同
--   BUILD_matlab      :   1    #  需要相同
--   BUILD_docs        :
--   CPU_ONLY          :   1    #  需要相同
--   USE_OPENCV        :   ON    #  需要相同
--   USE_LEVELDB       :   ON    #  需要相同
--   USE_LMDB          :   ON    #  需要相同
--   USE_NCCL          :   0    #  需要相同
--   ALLOW_LMDB_NOLOCK :   OFF    #  需要相同
--
-- Dependencies:
--   BLAS              :   Yes (Open)    #  需要相同
--   Boost             :   Yes (ver. 1.61)    #  需要相同
--   glog              :   Yes    #  需要相同
--   gflags            :   Yes    #  需要相同
--   protobuf          :   Yes (ver. 3.1.0)    #  需要相同
--   lmdb              :   Yes (ver. 0.9.70)    #  需要相同
--   LevelDB           :   Yes (ver. 1.18)    #  需要相同
--   Snappy            :   Yes (ver. 1.1.1)    #  需要相同
--   OpenCV            :   Yes (ver. 3.1.0)    #  需要相同
--   CUDA              :   No    #  需要相同
--
-- Python:    #  这里需要找到Python
--   Interpreter       :   D:/Libs/MiniConda/python.exe (ver. 2.7.15)
--   Libraries         :   D:/Libs/MiniConda/libs/python27.lib (ver 2.7.15)
--   NumPy             :   D:/Libs/MiniConda/lib/site-packages/numpy/core/include (ver 1.13.1)
--
-- Matlab:
--   Matlab            :   Yes (,    #  需要找到Matlab
--   Octave            :   No
--
-- Install:
--   Install path      :   D:/Libs/Caffe    #  需要相同
--
-- Configuring done
-- Generating done
......
```

得到上面的结果之后，删除在build_win.cmd添加的那行`exit /b 1`，然后重新执行build_win.cmd，开始编译依赖库。编译完成后，把下面的几条加入环境变量Path。

```
%USERPROFILE%\.caffe\dependencies\libraries_v140_x64_py27_1.1.0\libraries\bin
%USERPROFILE%\.caffe\dependencies\libraries_v140_x64_py27_1.1.0\libraries\lib
%USERPROFILE%\.caffe\dependencies\libraries_v140_x64_py27_1.1.0\libraries\x64\vc14\bin
%USERPROFILE%\.caffe\dependencies\libraries_v140_x64_py27_1.1.0\libraries\x64\vc14\lib
%USERPROFILE%\.caffe\dependencies\libraries_v140_x64_py27_1.1.0\libraries\x64\vc14\staticlib
```

## 5、编译Caffe

打开caffe-windows\script\Caffe.sln，使用VS2015打开。把编译模式切换为Release模式，然后右键单击INSTALL子项目，右键单击，选择“生成”，然后就开始编译Caffe了，编译完会自动安装到D:\Libs\Caffe。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190226111746674.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n,size_16,color_FFFFFF,t_70)

编译完成后，把D:\Libs\Caffe里面的bin、lib、matlab+caffe\private和matlab+caffe\private\Release加入到环境变量Path。下面是我添加的环境变量：
```
D:\Libs\Caffe\bin
D:\Libs\Caffe\lib
D:\Libs\Caffe\matlab\+caffe\private
D:\Libs\Caffe\matlab\+caffe\private\Release
```

至此，caffe编译完成。

# 三、使用Caffe

## 1、Matlab

在.m文件的最前面加入如下代码：

```matlab
PATH_TO_CAFFE_MATLAB='D:/Libs/Caffe/matlab/';
addpath(genpath(PATH_TO_CAFFE_MATLAB));
```

PATH_TO_CAFFE_MATLAB是前面install的安装路径。添加上面的代码之后，Matlab就能找到Caffe。**还有最关键的一步是**，把D:\Libs\Caffe\matlab+caffe\private\Release\caffe_.mexw64复制到和.m文件同一级目录下，否则会出现如下错误：

```
未定义函数或变量 'caffe_'。
```

## 2、Python

TODO

## 3、其他

请参考[https://blog.csdn.net/xiamentingtao/article/details/78132836](https://blog.csdn.net/xiamentingtao/article/details/78132836)





