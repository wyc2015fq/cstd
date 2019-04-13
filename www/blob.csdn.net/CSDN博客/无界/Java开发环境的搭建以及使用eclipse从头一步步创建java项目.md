
# Java开发环境的搭建以及使用eclipse从头一步步创建java项目 - 无界 - CSDN博客

置顶2014年01月27日 16:06:02[21aspnet](https://me.csdn.net/21aspnet)阅读数：92601


**原文：出自本人的Linux博客****[http://blog.csdn.net/unix21/article/details/18813173](http://blog.csdn.net/unix21/article/details/18813173)**
**一、Java 开发环境的搭建**
这里主要说windows环境下怎么配置Java环境。如果是Linux环境参考本博客另一篇文章即可：

### [Linux环境安装卸载JDK](http://blog.csdn.net/unix21/article/details/18774417)

**1.首先安装JDK**
java的SDK简称JDK。
去官网下载最新的JDK即可：
[http://www.oracle.com/technetwork/java/javase/downloads/jdk7-downloads-1880260.html](http://www.oracle.com/technetwork/java/javase/downloads/jdk7-downloads-1880260.html)
![](https://img-blog.csdn.net/20140127141044500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
点击下载好的exe文件安装即可。

**2.接下来配置环境变量**
xp右键“我的电脑”--“属性”--“高级”
win7右键”计算机“ -- ”属性“-- “高级系统设置”--“高级”
![](https://img-blog.csdn.net/20140127141826562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
点击“环境变量”
![](https://img-blog.csdn.net/20140127142048093?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
要配置JAVA_HOME,PATH,CLASSPATH三项属性。
注意：环境变量中，不区分变量名的大小写。

**1).配置JAVA_HOME**
找到我们安装 java SDK 的目录：
C:\Program Files\Java\jdk1.7.0_11
复制该路径。
然后点击上图中的“系统变量”下的“新建”：
![](https://img-blog.csdn.net/20140127142403734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)变量名处填写“JAVA_HOME”，变量值处直接粘贴上一步复制的 java SDK的安装目录，也就是：”C:\Program Files\Java\jdk1.7.0_11” 。
然后点击“确定”，JAVA_HOME变量就配置好了。

**2).配置CLASSPATH变量**
和配置JAVA_HOME类似，在“系统变量”点击“新建”，在弹出的窗口中分别输入
变量名：”CLASSPATH”
变量值: ”.;%JAVA_HOME%\lib\dt.jar;%JAVA_HOME%\lib\tools.jar”（取引号之内的值，包括前面的”.” ，这里%JAVA_HOME%就是引用之前配置好的 JAVA_HOME ）
![](https://img-blog.csdn.net/20140127143124156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
填好以后点击“确定”。

**3).配置PATH变量**
查找“系统变量”中是否已经有path变量：
![](https://img-blog.csdn.net/20140127143253859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如上图，我的计算机中已经存在path变量，这里点击“编辑”，如下所示的窗口中，可以看到变量值 已经有值了，我们在变量值最后加上一个半角分号（“;”），然后粘贴Java SDK安装目录下的bin目录的路径。
![](https://img-blog.csdn.net/20140127143318328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**3.测试环境变量配置**
在D盘建一个java文件：

```python
public class Test{
public static void main(String[] args){
	System.out.println("Hello World by http://blog.csdn.net/unix21");
	}
}
```
将该文本文件重命名为：Test.java
然后 点击“开始”->“运行”，输入cmd ，点击“确定”。
输入：” D: ”，回车，进入D盘。
然后，输入javac Test.java
编译 Test.java文件
没有提示，说明已经成功编译！
接着 输入 java Test ,运行编译好的java程序。
输出：Hello World by http://blog.csdn.net/unix21
说明我们的环境变量设置正确。
![](https://img-blog.csdn.net/20140127143621812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
其实是生成了 Test.class文件。

**二、使用eclipse从头一步步创建java项目**
**1.下载eclipse**官网地址：[http://www.eclipse.org/downloads/](http://www.eclipse.org/downloads/)
![](https://img-blog.csdn.net/20140127153012093?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
下载[Eclipse IDE for Java EE Developers](http://www.eclipse.org/downloads/packages/eclipse-ide-java-ee-developers/keplersr1)

**2.解压运行**
解压下载到的eclipse-jee-kepler-SR1-win32-x86_64.zip文件，
这是一个免安装的软件包，直接运行eclipse.exe就可以。

**3.建立项目**
1).建立工作空间
打开eclipse.exe出现的第一个界面：
![](https://img-blog.csdn.net/20140127153435062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
需要设置工作空间，我们放在D盘的workspace目录，点击“OK”。

2).点击工作区右上角 “workbench”链接，进入工作台
![](https://img-blog.csdn.net/20140127153706953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
打开的工作台界面
![](https://img-blog.csdn.net/20140127153833078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

3).创建一个java项目
![](https://img-blog.csdn.net/20140127153807375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
点击File--New--Project
![](https://img-blog.csdn.net/20140127154034390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
弹出的页面选“Java Project”
![](https://img-blog.csdn.net/20140127154221578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
Project name填写自己的项目名即可, 其他的选项采用默认值，点击“Finish”。
![](https://img-blog.csdn.net/20140127154416468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
左边的工作台会显示建好的工程。

4).新建Package包
![](https://img-blog.csdn.net/20140127154547140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
右键点击项目中的“src”包，选择“new”--”Package”
![](https://img-blog.csdn.net/20140127154728781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这里Package的名称没有特别的要求，主要是开发中约定的规范。
这里我们用本博客的名字：blog.csdn.net.unix21
点击“Finish”完成。
![](https://img-blog.csdn.net/20140127155217671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在src目录下出现了我们刚刚新建的Package包。其实也就是文件夹。
例如，Package包名为blog.csdn.net.unix21 ,其实就是在 src目录下创建文件夹blog，又在其他依次创建其他文件夹如图所示：
![](https://img-blog.csdn.net/20140127155417546?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

5).新建类class
右键刚刚创建的Package包，New--Class，新建一个类。
![](https://img-blog.csdn.net/20140127155551078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20140127155603078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
填写好类名称，勾选如图所示的复选框(public static void main(String[] args)),是为了自动生成main方法，点击“Finish”完成。
![](https://img-blog.csdn.net/20140127155921796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
类文件已经建好

6).写代码
![](https://img-blog.csdn.net/20140127160051203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
增加如下代码：

```python
System.out.println("Hello World by http://blog.csdn.net/unix21");
```
7).编译运行
![](https://img-blog.csdn.net/20140127160141015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
点击菜单栏的 Run--Run编译运行，或者直接按Ctrl+F11
启动程序的方法还有很多，eclipse本身有很多快捷按钮。
![](https://img-blog.csdn.net/20140127160153046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
程序编译成功运行后，在Console窗口中输出结果。



