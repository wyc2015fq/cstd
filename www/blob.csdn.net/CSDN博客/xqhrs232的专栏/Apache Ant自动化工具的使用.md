# Apache Ant自动化工具的使用 - xqhrs232的专栏 - CSDN博客
2018年09月30日 11:22:17[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：55
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://blog.csdn.net/gfd54gd5f46/article/details/54619234](https://blog.csdn.net/gfd54gd5f46/article/details/54619234)
相关文章
1、Apache Ant 简介----[https://blog.csdn.net/jubincn/article/details/4897610](https://blog.csdn.net/jubincn/article/details/4897610)
2、Unsupported major.minor version 52.0解决办法----[https://blog.csdn.net/superit401/article/details/72731381](https://blog.csdn.net/superit401/article/details/72731381)
3、Apache Ant的使用（1）——基础使用教程----[https://blog.csdn.net/yubo_725/article/details/52326746](https://blog.csdn.net/yubo_725/article/details/52326746)
## **自动化编译工具Ant 简单介绍**
      Ant,是一个将软件编译、测试、部署等步骤联系在一起加以自动化的一个工具，大多用于Java环境中的软件开发。由Apache软件基金会所提供。
**Ant 的优点：**
- **自动化部署、跨平台、利于团队合作**
**下载Ant：**
> 
Apache官网 : [http://ant.apache.org/](http://ant.apache.org/)
![1](https://img-blog.csdn.net/20170119202605932?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ2ZkNTRnZDVmNDY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**点击 Download 下的 Binary Distributions 就可以看到下载列表了**
![2](https://img-blog.csdn.net/20170119202613163?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ2ZkNTRnZDVmNDY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**Windows下就不演示了**
**我们在Linux下进行下载，复制链接地址**
![3](https://img-blog.csdn.net/20170119202620601?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ2ZkNTRnZDVmNDY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- **Linux下载命令：**
`wget  从网络下载一个文件 wget+url`- 1
`wget http://mirrors.hust.edu.cn/apache//ant/binaries/apache-ant-1.10.0-bin.tar.gz`- 1
**下载之后解压**
![4](https://img-blog.csdn.net/20170119202628182?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ2ZkNTRnZDVmNDY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**配置环境变量（跟配置java差不多）**
```
#2017-1-19
#ANT
export ANT_HOME=/root/apache-ant-1.10.0
export PATH=.:$ANT_HOME/bin:$PATH
```
- 1
- 2
- 3
- 4
**重启配置**
`source /etc/profile`- 1
**测试**
`ant –version 查看版本信息`- 1
![5](https://img-blog.csdn.net/20170119202635088?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ2ZkNTRnZDVmNDY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## **ANT自动化插件常用的一些命令**
**Ant命令**
```
ant clean       清理命令
ant run         运行命令
ant compile     编译命令
ant build       生成jar包
```
- 1
- 2
- 3
- 4
**＜project>标签**
**       每个构建文件对应一个项目。＜project>标签是构建文件的根标签。它可以有多个内在属性， 就如代码中所示，其各个属性的含义分别如下。**
- **1、 default表示默认的运行目标，即指定默认的target（即任务）。这个属性是必须的。**
- **2、basedir表示项目的基准目录。**
- **3、name表示项目名。**
- **4、description表示项目的描述。**
**每个构建文件都对应于一个项目，但是大型项目经常包含大量的子项目，每一个子项目都可以有自己的构建文件。**
**＜target>标签**
**       一个项目标签下可以有一个或多个target标签。一个target标签可以依赖其他的target标签。例 如，有一个target用于编译程序，另一个target用于生成可执行文件。在生成可执行文件之前必须先编译该文件，因此可执行文件的target依赖于编译程序的target。Target的所有属性如下。**
- **1、name表示目标名，这个属性是必须的。**
- **2、depends表示依赖的目标。**
- **3、if表示仅当属性设置时才执行。**
- **4、unless表示当属性没有设置时才执行。**
- **5、description表示项目的描述。**
**Ant的depends属性指定了target的执行顺序。**
**       Ant会依照depends属性中target出现顺序，依次执行 每个target。在执行之前，首先需要执行它所依赖的target。程序中的名为run的target的 depends属性compile，而名为compile的target的depends属性是prepare，所以这几个target执 行的顺序是prepare->compile->run。一个target只能被执行一次，即使有多个target依赖于它。如果没有if或 unless属性target总 会被执行。**
**＜mkdir>标签**
**该标签用于创建一个目录，它有一个属性dir用来指定所创建的目录名，其代码如下：**
`<mkdir dir=”${class.root}”/>`- 1
**通过以上代码就创建了一个目录，这个目录已经被前面的property标签所指定。**
**＜jar>标签**
**该标签用来生成一个JAR文件，其属性如下。**
- **1、destfile表示JAR文件名。**
- **2、basedir表示被归档的文件名。**
- **3、 includes表示被归档的文件模式。**
- **4、excludes表示被排除的文件模式。**
**＜javac标签>**
**该标签用于编译一个或一组java文件，其属性如下。**
- **1、srcdir表示源程序的目录。**
- **2、destdir表示class文件的输出目录。**
- **3、include表示被编译的文件的模式。**
- **4、excludes表示被排除的文件的模式。**
- **5、classpath表示所使用的类路径。**
- **6、debug表示包含的调试信息。**
- **7、optimize表示是否使用优化。**
- **8、verbose 表示提供详细的输出信息。**
- **9、fileonerror表示当碰到错误就自动停止。**
**＜java>标签**
**该标签用来执行编译生成的.class文件，其属性如下。**
- **1、classname 表示将执行的类名。**
- **2、jar表示包含该类的JAR文件名。**
- **3、classpath所表示用到的类路径。**
- **4、fork表示在一个新的虚拟机中运行该类。**
- **5、failonerror表示当出现错误时自动停止。**
- **6、output 表示输出文件。**
- **7、append表示追加或者覆盖默认文件。**
**＜delete>标签**
**该标签用于删除一个文件或一组文件，去属性如下。**
- **1、/file表示要删除的文件。**
- **2、dir表示要删除的目录。**
- **3、includeEmptyDirs 表示指定是否要删除空目录，默认值是删除。**
- **4、failonerror 表示指定当碰到错误是否停止，默认值是自动停止。**
- **5、verbose表示指定是否列出所删除的文件，默认值为不列出。**
**＜copy>标签**
**该标签用于文件或文件集的拷贝，其属性如下。**
- **1、file 表示源文件。**
- **2、tofile 表示目标文件。**
- **3、todir 表示目标目录。**
- **4、overwrite 表示指定是否覆盖目标文件，默认值是不覆盖。**
- **5、includeEmptyDirs 表示制定是否拷贝空目录，默认值为拷贝。**
- **6、failonerror 表示指定如目标没有发现是否自动停止，默认值是停止。**
- **7、verbose 表示制定是否显示详细信息，默认值不显示。**
**执行文件：**
```
<exec>
    <execexecutable="${base.dir}/email.bat" >
</exec>
```
- 1
- 2
- 3
## **实际操作：**
**Linux下创建一个项目**
```
Hello            项目根目录
    src/             一级文件夹目录
        com/            二级包目录   
            lingdu/        三级包目录
                Hello.java    存放的java类文件
    build.xml    ant配置文件
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
![1](https://img-blog.csdn.net/20170211145935450?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ2ZkNTRnZDVmNDY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**项目就是一个文件夹，然后放了一堆文件**
- **编写一个.java文件**
```
package com.lingdu;  
  public class Hello {  
        public static void main(String[] args) {  
            System.out.println("Hello!");  
        }  
  }
```
- 1
- 2
- 3
- 4
- 5
- 6
**如果按照平时我们的习惯，会先使用javac编译文件，然后用java运行**
**那我们能不能把这样的重复工作放到一个命令下呢？**答案是可以的。****
**在项目目录(src)下创建一个build.xml文件**
```
<?xml version="1.0" encoding="UTF-8"?>
<project name="Hello" basedir=".">
<property name="src" value="src"/>
<!-- 定义名字dest，值是:classes  -->
<property name="dest" value="classes"/>
<!-- 定义一个命令，作用是创建一个文件夹  -->
<target name="init">
<!-- Linux创建文件的命令${引用的目录} -->
<mkdir dir="${dest}"/>
</target>
</project>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
**保存运行**
`ant init  (“init是自己定义的名字”)`- 1
![6](https://img-blog.csdn.net/20170119202642554?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ2ZkNTRnZDVmNDY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**这样就成功创建了一个文件夹**
![7](https://img-blog.csdn.net/20170119202649851?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ2ZkNTRnZDVmNDY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**使用ls查看一下确实创建了 classes**
![8](https://img-blog.csdn.net/20170119202656948?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ2ZkNTRnZDVmNDY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**删除一个文件夹**
```
<!-- 定义一个命令 -->
<target name="del">
<!-- 删除一个文件夹 -->
<delete dir="${dest}"/>
</target>
```
- 1
- 2
- 3
- 4
- 5
![9](https://img-blog.csdn.net/20170119202705120?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ2ZkNTRnZDVmNDY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**使用ls查看一下确实被删除了**
![10](https://img-blog.csdn.net/20170119202713008?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ2ZkNTRnZDVmNDY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**接下来我们继续配置build.xml**
**定义javac 命令**
```
<!-- 编译src目录下的java源程序，依赖init引用 -->
<target name="compile" depends="init">
<!-- 编译src目录下的源程序，编译到dest引用下,包涵ant的运行时 -->
<javac srcdir="${src}" destdir="${dest}" includeantruntime="on"/>
</target>
```
- 1
- 2
- 3
- 4
- 5
**保存运行**
![11](https://img-blog.csdn.net/20170119202720883?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ2ZkNTRnZDVmNDY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**然后到classes目录下查看一下，确实生成了.class文件**
![12](https://img-blog.csdn.net/20170119202728667?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ2ZkNTRnZDVmNDY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**运行一个java 程序**
```
<!-- 运行java文件，在运行之前先删除一下classes的文件 -->
<target name="runfile" depends="del,compile">
<!-- classname="源程序的包"，classpath=“引用dest的目录”-->
<java classname="com.lingdu.Hello" classpath="${dest}"/>
</target>
```
- 1
- 2
- 3
- 4
- 5
- 6
![13](https://img-blog.csdn.net/20170119202735902?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ2ZkNTRnZDVmNDY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**打包成jar文件**
```
<!-- 把classes目录下的文件打包成一个jar -->
<target name="jar" depends="compile">
<!-- 打包的名字，打包到那个目录下 -->
<jar jarfile="${jarfile}" basedir="${dest}"/>
</target>
```
- 1
- 2
- 3
- 4
- 5
- 6
![14](https://img-blog.csdn.net/20170119202742775?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ2ZkNTRnZDVmNDY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**运行一个jar文件**
```
<!-- 运行一个jar文件 -->
<target name="runjar" depends="jar">
<java classname="com.lingdu.Hello" classpath="${jarfile}"/>
</target>
```
- 1
- 2
- 3
- 4
- 5
![15](https://img-blog.csdn.net/20170119202749447?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ2ZkNTRnZDVmNDY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**无痕模式运行**
```
<!-- 无痕模式，先编译运行，再打包成运行jar 然后删除 -->
<target name="deljar" depends="runjar">
<!-- 删除classes目录，删除jar包文件 -->
<delete dir="${dest}"/>
<delete file="${jarfile}"/>
</target>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
![16](https://img-blog.csdn.net/20170119202756027?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ2ZkNTRnZDVmNDY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## **windows下使用ant也是差不多的步骤。**
--------------------- 本文来自 Lingdu丶 的CSDN 博客 ，全文地址请点击：https://blog.csdn.net/gfd54gd5f46/article/details/54619234?utm_source=copy
