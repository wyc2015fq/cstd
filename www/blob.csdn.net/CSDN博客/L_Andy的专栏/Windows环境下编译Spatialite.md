# Windows环境下编译Spatialite - L_Andy的专栏 - CSDN博客

2018年12月17日 16:37:01[卡哥](https://me.csdn.net/L_Andy)阅读数：88


主要参考http://www.gaia-gis.it/gaia-sins/msvc_how_to.html

开始前，先使用OSGEO4W提供的工具下载必须的其它lib，主要包括

zlib,libxml2,proj,iconv

步骤1:编译freexl1.0.2，其根目录为D:\develop\sqlite\freexl-1.0.2

修改源码，否则无法通过编译

src/freexl.c的93-109行，注释掉

#if defined(_WIN32) && !defined(__MINGW32__)

/* MSVC compiler doesn't support lround() at all */

/*

static double

__declspec(dllexport) round (double num)

{

double integer = ceil (num);

if (num > 0)

return integer - num > 0.5 ? integer - 1.0 : integer;

return integer - num >= 0.5 ? integer - 1.0 : integer;

}

static long

__declspec(dllexport) lround (double num)

{

long integer = (long) round (num);

return integer;

}*/

#endif

build步骤

cd D:\develop\sqlite\freexl-1.0.2

nmake /f makefile.vc

nmake /f makefile.vc install

这一步的主要目的是生成freexl_i.lib, freexl.lib

步骤2：geos-3.5.0

下载geo-3.5.0,解压后，根目录为D:\develop\sqlite\geos-3.5.0

-- 运行autogen.bat

-- 修改nmake.opt

打开nmake.opt,定位到第25行，在行尾增加_NMAKE_VER=$(_NMAKE_VER)

$(MAKE) /f makefile.vc _NMAKE_VER=$(_NMAKE_VER)

build步骤

cd D:\develop\sqlite\geos-3.5.0

nmake /f makefile.vc _NMAKE_VER=14.00.23026.0

nmake /f makefile.vc install _NMAKE_VER=14.00.23026.0

这一步的目的是生成geos_c_i.lib，供第三步使用

步骤3：libspatialite-amalgamation-3.0.1

下载并解压，根目录为D:\develop\sqlite\libspatialite-amalgamation-3.0.1

直接执行下列命令行

cd

nmake /f makefile.vc

nmake /f makefile.vc install

NMake编译Spatialite

记录Windows平台下面使用NMake编译Spatialite库的详细过程,以及需要进行修改的地方.

-----------

