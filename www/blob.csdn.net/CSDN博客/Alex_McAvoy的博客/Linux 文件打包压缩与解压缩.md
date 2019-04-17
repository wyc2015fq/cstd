# Linux 文件打包压缩与解压缩 - Alex_McAvoy的博客 - CSDN博客





2018年06月20日 15:27:22[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：1966








## 【概述】

### 1.基本概念

1）打包：将一大堆文件或目录变成一个总的文件。

2）压缩：将一个大的文件通过一些压缩算法变成一个小文件。

3）解压缩：是压缩的反过程，是将一个通过软件压缩的文档、文件等各种东西恢复到压缩之前的样子

打包与压缩：在 Linux 中很多压缩程序只能针对一个文件进行压缩，因此，当想要压缩一大堆文件时，要先将这一大堆文件先打成一个包（ tar 命令），然后再用压缩程序进行压缩（ gzip、bzip2 命令）。

### 2.常用的压缩包文件格式
|文件后缀名|说明|
|----|----|
|*.zip|zip 程序打包压缩的文件|
|*.rar|rar 程序压缩的文件|
|*.7z|7zip 程序压缩的文件|
|*.tar|tar 程序打包，未压缩的文件|
|*.gz|gzip 程序（GNU zip）压缩的文件|
|*.xz|xz 程序压缩的文件|
|*.bz2|bzip2 程序压缩的文件|
|*.tar.gz|tar 打包，gzip 程序压缩的文件|
|*.tar.xz|tar 打包，xz 程序压缩的文件|
|*tar.bz2|tar 打包，bzip2 程序压缩的文件|
|*.tar.7z|tar 打包，7z 程序压缩的文件|

## 【zip 打包压缩与解压缩】

### 1.文件打包压缩

命令格式：**zip [选项] 创建的文件名.zip 要打包压缩的绝对路径**

常用选项：
|-r|表示递归打包包含子目录的全部内容（打包目录时选用）|
|----|----|
|-q|表示为安静模式，即不向屏幕输出信息|
|-o|输出文件，需在其后紧跟打包输出文件名|
|-[1-9]|设置压缩效率，1 表示最快压缩但体积大，9 表示体积最小但耗时最久|
|-e|设置密码，创建加密压缩包|
|-l|压缩文件时，把LF字符置换成LF+CR字符|
|-ll|压缩文件时，把LF+cp字符置换成LF字符|

![](https://img-blog.csdn.net/20180620142403254?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

注：因为 Windows 系统与 Linux 系统在文本文件格式上存在兼容问题，比如换行符（为不可见字符），在 Windows 为 CR+LF（Carriage-Return+Line-Feed：回车加换行），而在 Linux 上为 LF（换行），所以在压缩文件时可以使用选项 -l 或 -ll 进行转换。

### 2.解压缩

使用 unzip 命令可将由 zip 命令压缩的 .zip 压缩包解压。

命令格式：**unzip [选项] 压缩包名.zip**

常用选项：
|-q|表示为安静模式，即不向屏幕输出信息|
|----|----|
|-l|显示压缩文件内所包含的文件，但不解压|
|-d<目录>|指定文件解压缩后所要存储的目录（如果指定目录不存在会自动创建目录）|

![](https://img-blog.csdn.net/20180620142743427?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 【tar 打包工具】

tar 是一个打包工具，只是同时还实现了对 7z、gzip、xz、bzip2 等工具的支持，这些压缩工具本身只能实现对文件或目录（单独压缩目录中的文件）的压缩，没有实现对文件的打包压缩。

tar 的解压和压缩都是同一个命令，只是选项不同。

### 1.基本操作

**1）创建一个 tar 包**

命令格式：**tar -c -f 创建的文件名.tar 要打包的绝对路径**

**注：-c 表示创建一个 tar 包文件，-f 用于指定创建的文件名，且文件名必须紧跟在 -f 之后**

打包的常用选项：

    -v 以可视的方式输出打包的文件，会自动去掉表示绝对路径的 / ；

    -P 保留绝对路径符。

![](https://img-blog.csdn.net/20180620151920429?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



**2）解包一个文件到指定路径的已存在目录**



命令格式：**tar -x -f 要解压的文件名.tar -C 已存在的目录**



**注：-x 表示解压一个 tar 包文件，-f 用于指定要解压的文件名，-C 用于指定特定的解压目录**

**![](https://img-blog.csdn.net/20180620152509521?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**

**3）只查看不解压**

命令格式：**tar -t -f 要查看的文件名.tar**

**![](https://img-blog.csdn.net/20180620152542959?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**

**4）保留文件属性和跟随链接（符号链接或软链接）**

有时使用 tar 备份文件，当在其他主机还原时希望保留文件的属性和备份链接指向的源文件。

此时可用到如下两个选项：

    -p：保留文件的属性

    -h：备份链接指向的源文件而不是链接本身

![](https://img-blog.csdn.net/20180620152711106?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 2.创建不同的压缩格式的文件

要使用其它的压缩工具创建或解压相应的文件，只需在 tar 命令上加一个选项即可。
|压缩文件格式|选项|
|----|----|
|*.tar.gz|-z|
|*.tar.xz|-J|
|*tar.bz2|-j|

如下图，以 *.tar.gz 为例：

文件打包压缩

![](https://img-blog.csdn.net/2018062015350366?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

文件解压

![](https://img-blog.csdn.net/20180620153755945?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



