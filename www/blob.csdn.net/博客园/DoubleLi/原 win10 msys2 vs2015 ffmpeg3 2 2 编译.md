# 原 win10 msys2 vs2015 ffmpeg3.2.2 编译 - DoubleLi - 博客园






## 01 环境

win10x64企业版。vs2015update3企业版。git（git version 2.10.0.windows.1）。

## 02 下载ffmpeg代码

```
git clone https://git.ffmpeg.org/ffmpeg.git
git tag #目前最新稳定版本3.2.2
git checkout -b release3.2.2 n3.2.2
```
- 1
- 2
- 3

## 03 下载msys2

[http://msys2.github.io](http://msys2.github.io/)
[msys2-x86_64-20161025.exe](http://repo.msys2.org/distrib/x86_64/msys2-x86_64-20161025.exe)
![这里写图片描述](https://img-blog.csdn.net/20170206105537109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbG9uZ2pp/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 03.01 [msys2-x86_64-20161025.exe](http://repo.msys2.org/distrib/x86_64/msys2-x86_64-20161025.exe)

[http://msys2.github.io](http://msys2.github.io/). 选择第二个:msys2-x86_64-20161025.exe 
下载后，安装默认安装到C:/msys64。 
msys2-x86_64-20161025.exe的下载可能比较慢，中途可能断掉，建议使用firefox下载，或者使用支持断点续传的工具下载。

### 03.02 安装编译程序

安装后，到msys2窗口执行，安装编译程序：

```
pacman -S make gcc diffutils pkg-config
# 可能需要多尝试几次，我安装的时候gcc更新了几次才成功，这个更新过程支持断点续传。
# 如果不按照pkg-config，运行./configure的时候，会出现如下warning
# WARNING: pkg-config not found, library detection may fail.
```
- 1
- 2
- 3
- 4

![这里写图片描述](https://img-blog.csdn.net/20170206105610504?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbG9uZ2pp/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 03.03 重命名link.exe

重命名C:/msys64/usr/bin/link.exe 为C:/msys64/usr/bin/link.bak, 避免和MSVC 的link.exe抵触。

### 03.04 下载[YASM](http://yasm.tortall.net/Download.html)

YASM下载地址：[http://yasm.tortall.net/Download.html](http://yasm.tortall.net/Download.html)，下载其64位版本Win64 .exe (64 位 Windows 通用)。 
下载后，将下载回来的yasm-1.3.0-win64.exe 改名为yasm.exe，并放置于 MSYS2 安装目录中。就是放置到c:/msys64/usr/bin/ 中。

## 04 配置msys2

### 04.01 编辑C:/msys64/msys2_shell.cmd,

```
将其中的：
rem set MSYS2_PATH_TYPE=inherit
“rem”注释删除掉，成为：
set MSYS2_PATH_TYPE=inherit
```
- 1
- 2
- 3
- 4

### 04.02 打开vs2015关联的msys2窗口

在命令提示符窗口输入：

```
# 切换到c:/msys64目录
cd c:/msys64
# 切换vs2015 64位环境
"C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\amd64\vcvars64.bat"
#打开msys2的mingw64窗口
msys2_shell.cmd -mingw64
```
- 1
- 2
- 3
- 4
- 5
- 6

![这里写图片描述](https://img-blog.csdn.net/20171017082200372?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbG9uZ2pp/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 04.03 检查工具组件

```
which cl link yasm cpp
#显示如下内容
/c/Program Files (x86)/Microsoft Visual Studio 14.0/VC/BIN/amd64/cl
/c/Program Files (x86)/Microsoft Visual Studio 14.0/VC/BIN/amd64/link
/usr/bin/yasm
/usr/bin/cpp
```
- 1
- 2
- 3
- 4
- 5
- 6

![这里写图片描述](https://img-blog.csdn.net/20170206111524305?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbG9uZ2pp/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 04.04 修改msys2窗口代码页为GBK，避免编译时显示乱码。

在窗口上右击鼠标，选择[Options]菜单，选择[Text]，locale选择：zh_CN，Character set 选择 GBK。 
![这里写图片描述](https://img-blog.csdn.net/20170206111653064?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbG9uZ2pp/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 05 编译ffmpeg

```
#在msys2窗口:
cd d:/git/ffmpeg
#进入下载ffmpeg目录中
./configure  --toolchain=msvc --arch=x86 --enable-yasm --enable-asm --enable-shared --disable-static
```
- 1
- 2
- 3
- 4

![这里写图片描述](https://img-blog.csdn.net/20170206112955374?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbG9uZ2pp/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```
如有：WARNING: pkg-config not found, library detection may fail.
#安装pkg-config 
pacman -S pkg-config
```
- 1
- 2
- 3

![这里写图片描述](https://img-blog.csdn.net/20170206113023343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbG9uZ2pp/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```
# make 过程会有很多waring提示，不需理会
make
make install
```
- 1
- 2
- 3

这样以后，编译生产的执行文件ffmpeg.exe和DLL文件在目录C:/msys64/usr/local/bin中，开发头文件在C:/msys64/usr/local/include中。 
![这里写图片描述](https://img-blog.csdn.net/20170206113158117?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbG9uZ2pp/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 06 编译ffplay.exe

### 01 更新msys64

```
# 更新mingw-w64-x86_64-toolchain工具链
pacman -S mingw-w64-x86_64-toolchain
```
- 1
- 2

### 02 下载SDL和X264

下载[SDL2-2.0.5.tar.gz](http://www.libsdl.org/release/SDL2-2.0.5.tar.gz)版本，并编译 
[http://www.libsdl.org/download-2.0.php](http://www.libsdl.org/download-2.0.php)
[http://www.libsdl.org/release/SDL2-2.0.5.tar.gz](http://www.libsdl.org/release/SDL2-2.0.5.tar.gz)

```
tar -zxvf SDL2-2.0.5.tar.gz
cd SDL2-2.0.5
./configure --prefix=/d/git/ffmpeg/build
make
make install
```
- 1
- 2
- 3
- 4
- 5

下载x264，并编译

```
git clone http://git.videolan.org/git/x264.git
git checkout -b stable remotes/origin/stable
./configure --prefix=/d/git/ffmpeg/build --host=x86_64-w64-mingw32 --enable-shared
make
make install
```
- 1
- 2
- 3
- 4
- 5

编译带ffplay.exe的ffmpeg

```
export PKG_CONFIG_PATH=/d/git/ffmpeg/build/lib/pkgconfig/
# 如果不正确设置PKG_CONFIG_PATH环境变量，带ffplay的ffmpeg ./configure 时，会出现Warning
# WARNING: using libx264 without pkg-config
./configure --prefix=/d/git/ffmpeg/build --enable-libx264 --enable-shared --enable-static --arch=x86 --extra-cflags='-I/d/git/ffmpeg/build/include -I/d/git/ffmpeg/build/include/SDL' --extra-ldflags='-L/d/git/ffmpeg/build/lib' --enable-yasm --enable-asm --enable-version3 --enable-gpl --enable-memalign-hack --enable-ffplay
make 
make install
```









