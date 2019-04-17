# Ubuntu(16.04LTS) OSG 编译 - TwinkleStar0121 - CSDN博客





2018年11月26日 16:52:04[TwinkleStar0121](https://me.csdn.net/jvandc)阅读数：276











### 文章目录
- [Ubuntu Build OSG](#Ubuntu_Build_OSG_2)
- [1、下载OSG](#1OSG_3)
- [2、编译OSG](#2OSG_5)
- [3、编译/运行 OSG Examples](#3_OSG_Examples_15)
- [4、安装OSG依赖包](#4OSG_39)
- [Reference](#Reference_108)




### Ubuntu Build OSG

#### 1、下载OSG

从Github上下载对应的OSG版本：[OSG Github](https://github.com/openscenegraph/OpenSceneGraph).我选版本的是3.6.3。

#### 2、编译OSG

Ubuntu上编译OSG也比较简单，执行以下命令：

```
cd ${OSG_ROOT_DIR}
cmake .
make
sudo make install
```

在执行 cmake 的时候观察log会发现有许多依赖库找不到，这个我们后面解决。

#### 3、编译/运行 OSG Examples

设置环境变量，编辑~/.bashrc文件，添加如下内容(vim ~/.bashrc)：

```
export PATH=${PATH}:/home/myaccount/OpenSceneGraph/bin
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/home/myaccount/OpenSceneGraph/lib
export OSG_FILE_PATH=/home/myaccount/OpenSceneGraph-Data:/home/myaccount/OpenSceneGraph-Data/Images
```

编译 OSG Examples：
- 首先从Github上下载osg-data：[osg-data Github](https://github.com/openscenegraph/osg-data)，这里需注意一点，osg-data目录中的内容必须拷贝到OpenSceneGraph-data目录下，否则示例执行会出错。
- 执行如下命令：

```
cd ${OSG_ROOT_DIR}
cmake . -DBUILD_OSG_EXAMPLES=1
make
sudo make install
```

运行OSG Examples：
- 执行命令

```
bash ./runexamples.bat
```

#### 4、安装OSG依赖包

执行命令：

```
apt-get build-dep openscenegraph
```

在我机器上执行这个命令，返回的结果是需要自己在source.list中添加source URIs，试了下另外一个命令，尝试安装依赖库：

```
apt-get install openscenegraph
```

安装完成后，执行cmake，依然会返回找不到以下第三方库：

```
-- Could NOT find EGL (missing:  EGL_LIBRARY EGL_INCLUDE_DIR)
-- Could NOT find Freetype (missing:  FREETYPE_LIBRARY FREETYPE_INCLUDE_DIRS)
-- Could NOT find JPEG (missing:  JPEG_LIBRARY JPEG_INCLUDE_DIR)
-- Could NOT find Jasper (missing:  JASPER_LIBRARIES JASPER_INCLUDE_DIR JPEG_LIBRARIES)
-- Could NOT find LibXml2 (missing:  LIBXML2_LIBRARIES LIBXML2_INCLUDE_DIR)
-- Could NOT find ZLIB (missing:  ZLIB_LIBRARY ZLIB_INCLUDE_DIR)
-- Could NOT find ZLIB (missing:  ZLIB_LIBRARY ZLIB_INCLUDE_DIR)
-- Could NOT find GDAL (missing:  GDAL_LIBRARY GDAL_INCLUDE_DIR)
-- Checking for module 'gta'
--   No package 'gta' found
-- Could NOT find CURL (missing:  CURL_LIBRARY CURL_INCLUDE_DIR)
-- Trying to find DCMTK expecting DCMTKConfig.cmake
-- Trying to find DCMTK expecting DCMTKConfig.cmake - failed
-- Trying to find DCMTK relying on FindDCMTK.cmake
-- Please set DCMTK_DIR and re-run configure (missing:  DCMTK_config_INCLUDE_DIR DCMTK_dcmdata_INCLUDE_DIR DCMTK_dcmimage_INCLUDE_DIR DCMTK_dcmimgle_INCLUDE_DIR DCMTK_dcmjpeg_INCLUDE_DIR DCMTK_dcmjpls_INCLUDE_DIR DCMTK_dcmnet_INCLUDE_DIR DCMTK_dcmpstat_INCLUDE_DIR DCMTK_dcmqrdb_INCLUDE_DIR DCMTK_dcmsign_INCLUDE_DIR DCMTK_dcmsr_INCLUDE_DIR DCMTK_dcmtls_INCLUDE_DIR DCMTK_ofstd_INCLUDE_DIR DCMTK_oflog_INCLUDE_DIR DCMTK_LIBRARIES)
-- Trying to find DCMTK relying on FindDCMTK.cmake - ok
-- Could NOT find GStreamer (missing:  GSTREAMER_INCLUDE_DIRS GSTREAMER_LIBRARIES GSTREAMER_VERSION GSTREAMER_BASE_INCLUDE_DIRS GSTREAMER_BASE_LIBRARIES GSTREAMER_APP_INCLUDE_DIRS GSTREAMER_APP_LIBRARIES GSTREAMER_PBUTILS_INCLUDE_DIRS GSTREAMER_PBUTILS_LIBRARIES)
-- Could NOT find SDL2 (missing:  SDL2_LIBRARY SDL2_INCLUDE_DIR)
-- Could NOT find SDL (missing:  SDL_LIBRARY SDL_INCLUDE_DIR)
-- Checking for module 'cairo'
--   No package 'cairo' found
-- Checking for module 'poppler-glib'
--   No package 'poppler-glib' found
-- Checking for module 'librsvg-2.0>=2.35'
--  
-- Checking for module 'cairo'
--   No package 'cairo' found
-- Checking for module 'gtk+-2.0'
--   No package 'gtk+-2.0' found
-- Checking for module 'gtkglext-x11-1.0'
--   No package 'gtkglext-x11-1.0' found
-- Could NOT find FLTK (missing:  FLTK_LIBRARIES FLTK_FLUID_EXECUTABLE)
-- Could NOT find wxWidgets (missing:  wxWidgets_LIBRARIES wxWidgets_INCLUDE_DIRS)
-- Could NOT find JPEG (missing:  JPEG_LIBRARY JPEG_INCLUDE_DIR)
-- Could NOT find ZLIB (missing:  ZLIB_LIBRARY ZLIB_INCLUDE_DIR)
-- Could NOT find PNG (missing:  PNG_LIBRARY PNG_PNG_INCLUDE_DIR)
-- Could NOT find TIFF (missing:  TIFF_LIBRARY TIFF_INCLUDE_DIR)
-- g++ version 5.4.0
-- Configuring done
-- Generating done
```

这样，只能挨个自己安装依赖库了。
- egl:libgles2-mesa-dev
- freetype:libfreetype6-dev
- jpeg:libjpeg-dev
- fltk:fltk1.3-dev
- curl:libopenssl-dev
- gstreamer:libgstreamer-plugins-base1.0-dev
- gdal:libgdal-dev
- sdl2:libsdl2-dev
- sdl:libsdl1.2-dev
- wxWidget:libwxgtk3.0-dev
- tiff:libtiff-dev

执行命令 sudo apt-get install “package” 安装完上面的第三方库之后，再执行cmake，就只剩下“cairo”、“gtk”等package找不到的错误了。

#### Reference

[[1].Quick steps for OpenSceneGraph installation on Linux](https://vicrucann.github.io/tutorials/osg-linux-quick-install/)
[[2].openscenegraph-on-linux](https://bhattigurjot.wordpress.com/2013/12/27/openscenegraph-on-linux/)
[[3]openscenegraph dependencies](http://openscenegraph.sourceforge.net/documentation/OpenSceneGraph/doc/dependencies.html)
[[4].Ubuntu下安装OSG](https://blog.csdn.net/Sweeping_monk_ren/article/details/79766791)



