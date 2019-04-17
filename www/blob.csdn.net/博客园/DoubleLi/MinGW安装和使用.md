# MinGW安装和使用 - DoubleLi - 博客园







MinGW(Minimalist GNU For Windows)是个精简的Windows平台C/C++、ADA及Fortran编译器，相比Cygwin而言，体积要小很多，使用较为方便。

MinGW最大的特点就是编译出来的可执行文件能够独立在Windows上运行。

MinGW的组成：
- 编译器(支持C、C++、ADA和Fortran)
- GNU工具
- mingw-get(用于Windows平台安装和部署MinGW和MSYS的命令行安装器)
- mingw-get-inst(用于GUI打包)

# 1. MinGW安装

## 1.1. 下载

MinGW官方网址：[http://www.mingw.org/](https://link.jianshu.com?t=http://www.mingw.org/)





![](https://upload-images.jianshu.io/upload_images/145902-4cb8d9635103ea4d.png)





点击“Download Installer”后会跳转到Sourceforge页面进行下载：





![](https://upload-images.jianshu.io/upload_images/145902-39e00b5a95378cea.png)





之后，会下载一个名为`mingw-get-setup.exe`的安装文件。

## 1.2. 安装下载器

运行刚刚下载的安装程序，看到如下界面：





![](https://upload-images.jianshu.io/upload_images/145902-6620383cb892e18c.png)





点击“Install”，进入下面这个界面：





![](https://upload-images.jianshu.io/upload_images/145902-ba5ee85648451e98.png)


选择安装路径


默认安装路径为`C:\MinGW`，我习惯安装在D盘下，点击“Change”修改路径如下：





![](https://upload-images.jianshu.io/upload_images/145902-72e5c89d12cef34a.png)





点"Continue"继续。





![](https://upload-images.jianshu.io/upload_images/145902-8a5412e533c6be6c.png)


安装过程


安装完成后，桌面出现这样一个图标。





![](https://upload-images.jianshu.io/upload_images/145902-4f7bf712405c9093.png)


安装器


这只是个安装器，需要在线下载真正的安装内容，所以速度不会快。

## 1.3. 选择安装组件

运行安装程序，有的版本在安装之后会自动运行，看到下面这个界面：





![](https://upload-images.jianshu.io/upload_images/145902-198c2c6d885ccc8a.png)


选择安装组件


选择需要安装的组件，右键选择“Mark for Installation”,之后选择"Installation -> Apply Changes”。

## 1.4. GCC

这里重点要提到的是GCC组件的安装，如图所示。





![](https://upload-images.jianshu.io/upload_images/145902-ffef7be992dc1a79.png)


选择GCC组件


先选择左边的"MinGW Base System"选项，之后再右边找到mingw-gcc。最好选择bin,dev和lic三个组件进行安装。





![](https://upload-images.jianshu.io/upload_images/145902-614702a0951642ae.png)





之后点击菜单栏中的“Installation”进行安装。

*注意：*在安装程序中，复选框为绿色表示已经安装的项目，复选框为白色表示没有安装的项目。因此如果需要删除，只需要在绿色复选框右键后选择“Mark for removal”即可。

## 1.5 确认安装成功

安装执行完成后，在`D:\MinGW\bin`目录（我们之前配置好的路径）下进行查看。如果找到下面这三个可执行文件，说明安装成功。





![](https://upload-images.jianshu.io/upload_images/145902-0596ccac9bb6ef5e.png)





*注意：*如果你是为了使用Code::Blocks安装MinGW，到这一步就完成了。之后的使用请参考[C语言从零开始（二）-Code::Blocks使用](https://www.jianshu.com/p/d5fa463df77d)。Code::Blocks在配置Debug工具时需要制定MinGW的路径。

# 2. 配置

## 2.1. 设置MinGW环境变量

鼠标右键“我的电脑”->“属性”，选择“高级”选项卡下的“环境变量”，在系统变量里点“新建”，之后填写MinGW的安装路径，如下：





![](https://upload-images.jianshu.io/upload_images/145902-501a3cd5212f3c82.png)


MinGW路径


之后找到Path，在最前面添加下面这段声明，之后点击确定。安装完成后，在MinGW\bin的目录下，会有一个名为gcc.exe的可执行文件。看到这个说明安装成功。

```
%MinGW%\bin;
```

## 2.2. 查看GCC版本信息

打开命令行，输入gcc -v查看编译器版本。如下图：





![](https://upload-images.jianshu.io/upload_images/145902-cf30225f38294c2b.png)


GCC


# 3. MinGW使用

## 3.1. 测试编译

测试文件test.c，代码如下

```cpp
#include <stdio.h>

void main() {
        printf("Hello World!");
}
```

## 3.2. 编译test

编译命令如下：

```bash
gcc test.c -o test
```

输出结果为：test.exe

## 3.3. 执行

输入test回车，结果如下：





![](https://upload-images.jianshu.io/upload_images/145902-f026e9f9421c047b.png)


结果





from：https://www.jianshu.com/p/e9ff7b654c4a









