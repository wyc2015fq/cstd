# Java —— JDK 的安装 - Alex_McAvoy的博客 - CSDN博客





2018年09月10日 10:34:04[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：54
个人分类：[Java学习笔记](https://blog.csdn.net/u011815404/article/category/8863016)

所属专栏：[Java 学习笔记](https://blog.csdn.net/column/details/34317.html)









# 【JDK 是什么】

JDK 是 Java 语言的软件开发工具包，主要用于移动设备、嵌入式设备上的 JAVA 应用程序，是整个 JAVA 开发的核心，它包含了JAVA 的运行环境（JVM+ 虚拟机和 Java 系统类库）和开发工具（编译工具 javac.exe、打包工具 jar.exe等）。

# 【安装流程】

## 1.确定操作系统是 32 位还是 64 位

可在系统属性中看到自己的操作系统是 32 位还是 64 位

![](https://img-blog.csdn.net/20180910101654122?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 2.删除先前版本的JDK

如果是第一次安装可以直接跳过这步，因为先前版本的 JDK 不卸载，可能会出现安装过程卡死的情况

控制面板 -> 程序 -> Java SE Development 等全部卸载

![](https://img-blog.csdn.net/20180910101353598?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 3.下载相关 JDK

JDK 分为 JAVA EE 与 JAVA SE 两个版本，前者是企业版，多用与企业级项目的开发，后者是标准版，多用于一般的 JAVA 项目开发。JAVA SE 可视为 JAVA EE 的子集。 

可在 JAVA 官网，根据具体需要下载相关 JDK，官网：[点击这里](https://www.oracle.com/technetwork/java/index.html)

## 4.安装

下载完所需 JDK，进行安装即可，安装目录选好后需要记住，后面配置环境变量会用到

![](https://img-blog.csdn.net/20180910102316611?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 5.配置环境变量

此电脑 -> 属性  -> 高级系统设置  -> 环境变量

![](https://img-blog.csdn.net/20180910102545443?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

打开后，在下面的系统变量点击 新建 ，变量名填：JAVA_HOME，地址为上述的 JDK 地址

![](https://img-blog.csdn.net/20180910102724645?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

然后设置 CLASSPATH 属性，对其进行编辑，变量值设为：.;%JAVA_HOME%\lib\dt.jar;%JAVA_HOME%\lib\tools.jar;

![](https://img-blog.csdn.net/20180910102914157?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

最后设置 path 变量属性，将 %java_home%\bin;%java_home%\jre\bin; 添加进变量值即可

![](https://img-blog.csdn.net/20180910103150949?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 6.检测安装是否成功

打开 cmd 后，依次输入 javac 和 java -version，如果出现下图所示的界面，则安装成功

![](https://img-blog.csdn.net/20180910103339845?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180910103352401?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



