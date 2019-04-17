# Intel Edison 移植boa - V__KING__的专栏 - CSDN博客





2015年12月15日 14:40:39[v__king__](https://me.csdn.net/V__KING__)阅读数：709








参考： [http://blog.csdn.net/v__king__/article/details/50184347](http://blog.csdn.net/v__king__/article/details/50184347)

# Intel Edison 移植boa

## 1. 按照下面网址进行配置

[http://blog.csdn.net/v__king__/article/details/50184347](http://blog.csdn.net/v__king__/article/details/50184347)

## 2. 安装intel Edison 的SDK-交叉编译工具
- 到官网下载sdk
- 解压后，执行里面的安装脚本安装。默认安装在/opt/poky-edison目录下

## 3. 交叉编译boa
- cd boa-0.94.13/src
- 修改Makefile

```
全部修改编译工具为：
CC = i586-poky-linux-gcc -m32 -march=core2 -mtune=core2 -msse3 -mfpmath=sse -mstackrealign -fno-omit-frame-pointer –sysroot=/opt/poky-edison/1.7.2/sysroots/core2-32-poky-linux
```
- 执行make
- cp 到intel Edison板子上面

## 4. 执行出现错误：

Could not open mime.types file, “/etc/mime.types”, for reading
- 从你的pc机上cp到intel edison上面



