# eclipse安装lombok - 左直拳的马桶_日用桶 - CSDN博客
2019年01月18日 16:15:12[左直拳](https://me.csdn.net/leftfist)阅读数：100
个人分类：[Java																[应用](https://blog.csdn.net/leftfist/article/category/2778939)](https://blog.csdn.net/leftfist/article/category/2412493)
网上搜了一堆教程，给出的方法都是：
1、将lombok.jar拷贝到eclipse下
2、修改eclipse.ini
但都没有效果。也许，这些方法曾经有效，但今时不同往日，软件版本升级了，处理的方法也有所不同。
直接说方法：
0、我的eclipse是
```
Eclipse Java EE IDE for Web Developers.
Version: Neon.3 Release (4.6.3)
```
1、到lombok官网下载最新插件：
[https://projectlombok.org/download](https://projectlombok.org/download)
2、在命令行方式下，运行
```
java -jar c:\soft\lombok.jar
```
弹出的窗口，系统在拼命搜索eclipsede的安装路径。我不等它了，我的eclipse是免安装的，所以直接将路径赋给它，安装成功。
3、打开eclipse，打开项目，红叉叉不见了。
4、安装程序究竟干了些啥？
似乎是：
1）将lombok拷贝到eclipse根目录下
2）修改eclipse.ini，在末尾加了一句：
```
-javaagent:D:\soft\develop\Java\eclipse\lombok.jar
```
只有这一句，而且是绝对路径。
