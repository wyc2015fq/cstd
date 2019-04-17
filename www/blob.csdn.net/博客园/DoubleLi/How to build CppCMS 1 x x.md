# How to build CppCMS 1.x.x - DoubleLi - 博客园






# How to build CppCMS 1.x.x
- [Requirements](http://cppcms.com/wikipp/en/page/cppcms_1x_build#Requirements)
- - [Mandatory Requirements](http://cppcms.com/wikipp/en/page/cppcms_1x_build#Mandatory.Requirements)
- [Recommended Dependencies](http://cppcms.com/wikipp/en/page/cppcms_1x_build#Recommended.Dependencies)
- [Suggested](http://cppcms.com/wikipp/en/page/cppcms_1x_build#Suggested)
- [Dependencies for Common Linux Distributions](http://cppcms.com/wikipp/en/page/cppcms_1x_build#Dependencies.for.Common.Linux.Distributions)
- [Notes for Microsoft Visual C++ users](http://cppcms.com/wikipp/en/page/cppcms_1x_build#Notes.for.Microsoft.Visual.C...users)

- [Getting The Sources](http://cppcms.com/wikipp/en/page/cppcms_1x_build#Getting.The.Sources)
- [Build Process](http://cppcms.com/wikipp/en/page/cppcms_1x_build#Build.Process)
- [Build Options](http://cppcms.com/wikipp/en/page/cppcms_1x_build#Build.Options)
- [Examples](http://cppcms.com/wikipp/en/page/cppcms_1x_build#Examples)
- - [POSIX Operating Systems](http://cppcms.com/wikipp/en/page/cppcms_1x_build#POSIX.Operating.Systems)
- [Microsoft Windows](http://cppcms.com/wikipp/en/page/cppcms_1x_build#Microsoft.Windows)
- [Cross Compiling](http://cppcms.com/wikipp/en/page/cppcms_1x_build#Cross.Compiling)


## Requirements

In order to build CppCMS you need:

### Mandatory Requirements
- Modern C++ Compiler -- GCC, MSVC 9, Intel. See [supported compilers and platforms](http://cppcms.com/wikipp/en/page/cppcms_1x_platforms)
- CMake 2.6 and above, 2.8.x is recommended.
- Zlib library
- PCRE library.
- Python >=2.4 (but not 3)

### Recommended Dependencies
- ICU Library 3.6 and above -- support of advanced localization features.
- 
gcrypt or OpenSSL library -- for support of encrypted session cookies.

If both available gcrypt would be used.


### Suggested
- iconv library (if libc does not provide iconv interface)

### Dependencies for Common Linux Distributions

Debian Based (Debian, Ubuntu):

Packages: cmake libpcre3-dev zlib1g-dev libgcrypt11-dev libicu-dev python

Getting:



[view plain](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)[print](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)[?](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)

- aptitude install cmake libpcre3-dev zlib1g-dev libgcrypt11-dev libicu-dev python  


RPM Based (RadHat, CentOS, Fedora, Suse):

Packages: cmake gcc-c++ gcc make zlib-devel pcre-devel libicu-devel libgcrypt-devel

Getting:



[view plain](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)[print](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)[?](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)

- yum install cmake gcc-c++ gcc make zlib-devel pcre-devel libicu-devel libgcrypt-devel  


### Notes for Microsoft Visual C++ users

Building cppcms with Visual Studio projects is not supported due to complexity of debug and release mode.

You should use nmake as shown in the example.

## Getting The Sources

Download the latest cppcms-1.x.x.tar.bz2 from [sourceforge](https://sourceforge.net/projects/cppcms/files/) and extract it:



[view plain](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)[print](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)[?](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)

- tar -xjf cppcms-1.0.4.tar.bz2  


If you want to get latest version You need git to get the sources:



[view plain](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)[print](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)[?](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)

- git clone https://github.com/artyom-beilis/cppcms.git cppcms    


## Build Process

Go to the cppcms directory you created and create build directory and go to it:



[view plain](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)[print](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)[?](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)

- mkdir build  
- cd build  


Now configure the library with CMake



[view plain](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)[print](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)[?](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)

- cmake ..  


Or



[view plain](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)[print](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)[?](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)

- cmake various_build_options ..  


Then run



[view plain](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)[print](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)[?](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)

- make  
- make test  
- make install  


## Build Options
- `-DDISABLE_STATIC=ON` -- disable building of static version of cppcms library
- `-DDISABLE_SHARED=ON` -- disable building of shared version of cppcms library
- `-DDISABLE_ICONV=ON` -- disable usage of iconv (ICU would be used instead)
- `-DDISABLE_GCRYPT=ON` -- disable usage of gcrypt library.
- `-DDISABLE_OPENSSL=ON` -- disable usage of OpenSSL.
- `-DUSE_WINDOWS6_API=ON` -- use Windows Vista, Windows 7 API if possible, allows CppCMS using native Windows Vista/7 conditional variables and other advanced API. By default disabled.
- `-DLIBDIR=lib64` - use alternative name for library directory, for example use lib64 on Red Hat based distributions.

*Generic Size Optimization Options for Embedded Builds:*
- `-DDISABLE_FCGI=ON` -- build without FastCGI Server API.
- `-DDISABLE_SCGI=ON` -- build without SCGI Server API.
- `-DDISABLE_HTTP=ON` -- build without internal HTTP server.
- `-DDISABLE_ICU_LOCALE=ON` -- do not use ICU for localization but rather C++ `std::locale`, Windows API or POSIX API based localization -- many localization features would not be available, but it may be useful for embedded builds.
- `-DDISABLE_PREFORK_CACHE=ON` - disable cache support for preforking modes (process shared cache)
- `-DDISABLE_TCPCACHE=ON` - disable distributed cache support (memcached-like solution support)
- `-DDISABLE_CACHE=ON` - disable caching system at all.
- `-DDISABLE_GZIP=ON` - disable output gzip compression support (eliminates dependency on zlib)

*Generic useful CMake options:*
- 
`-DCMAKE_BUILD_TYPE=(Debug|Release|RelWithDebInfo|MinSizeRel)` -- release type. RelWithDebInfo is default, unless using MSVC where Debug is default.

- 
`-DCMAKE_INCLUDE_PATH=/path/to/include` -- path to location of libraries headers, provide it in order to find libraries headers installed in non-standard locations. You almost always need to provide it under Windows.

- `-DCMAKE_LIBRARY_PATH=/path/to/lib` -- path to location of libraries, provide it in order to find libraries installed in non-standard locations. You almost always need to provide it under Windows.
- `-DCMAKE_INSTALL_PREFIX=/usr/local` -- Installation prefix (similar to autoconf --prefix). Defaults to /usr/local

## Examples

### POSIX Operating Systems

I assume that you are in a terminal in the build directory inside the CppCMS source directory.

Build under Linux, FreeBSD and Cygwin:



[view plain](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)[print](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)[?](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)

- cmake ..  
- make  
- make test   
- make install  


Build under OpenSolaris with SunStudio



[view plain](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)[print](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)[?](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)

- cmake -DCMAKE_C_COMPILER=/usr/bin/suncc -DCMAKE_CXX_COMPILER=/usr/bin/sunCC ..  
- make  
- make test   
- make install  


Build under OpenSolaris with GCC, where ICU installed in /opt/icu



[view plain](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)[print](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)[?](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)

- cmake -DCMAKE_INCLUDE_PATH=/opt/icu/include -DCMAKE_LIBRARY_PATH=/opt/icu/lib ..  
- make  
- make test   
- make install  


### Microsoft Windows

We assume that 3rd part libraries installed in `c:\3rd_part` and sources are placed in`d:\projects\cppcms`

*Note:* You need to setup correct PATH variables in order to let system find all DLLs it needs.

Under cmd.exe:



[view plain](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)[print](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)[?](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)

- set PATH=c:\3rd_part\lib;%PATH%  
- set PATH=d:\projects\cppcms\build\booster;%PATH%   


Under bash:



[view plain](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)[print](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)[?](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)

- export PATH=/c/3rd_part/lib:"$PATH"  
- export PATH=/d/projects/cppcms/build/booster:"$PATH"  


*MinGW Builds:*

For mingw builds you also need to pass a path to the mingw's library directory, otherwise it may not find winsock library correctly. Assuming you have mingw installed in `c:\mingw`

Open MinGW Shell terminal



[view plain](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)[print](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)[?](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)

- cmake -G "MSYS Makefiles" -DCMAKE_INCLUDE_PATH=c:/3rd_part/include -DCMAKE_LIBRARY_PATH="c:/3rd_part/lib;c:/mingw/lib" -DCMAKE_INSTALL_PREFIX=c:/mingw ..  
- make  
- make test   
- make install  


*MSVC Builds:*

Open MSVC Shell terminal (All Programs > Microsoft Visual Studio 2008 > Visual Studio Tools > Visual Studio 2008 Command Prompt)



[view plain](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)[print](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)[?](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)

- cmake -G "NMake Makefiles" -DCMAKE_INCLUDE_PATH=c:/3rd_part/include -DCMAKE_LIBRARY_PATH=c:/3rd_part/lib -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=c:/cppcms ..  
- nmake  
- nmake test   
- nmake install  


### Cross Compiling

The build is just ordinary CMake cross-compilation procedure. For more information read[http://www.cmake.org/Wiki/CMake_Cross_Compiling](http://www.cmake.org/Wiki/CMake_Cross_Compiling)

We would provide an example for ARM under Linux, without ICU.

Create ToolChain.cmake



[view plain](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)[print](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)[?](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)

- SET(CMAKE_SYSTEM_NAME Linux)  
- SET(CMAKE_C_COMPILER  /usr/bin/arm-linux-gnueabi-gcc)  
- SET(CMAKE_CXX_COMPILER /usr/bin/arm-linux-gnueabi-g++)  
- SET(CMAKE_FIND_ROOT_PATH  /usr/arm-linux-gnueabi)  
- SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)  
- SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)  
- SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)  


Configure



[view plain](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)[print](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)[?](http://cppcms.com/wikipp/en/page/cppcms_1x_build#)

- cmake -DCMAKE_TOOLCHAIN_FILE=ToolChain.cmake -DDISABLE_ICU_LOCALIZATION=ON  -DCMAKE_ISTALL_PREFIX=/usr/arm-linux-eabi ..  
- make  
- make install  










