
# window 上安装 Scala - anlcy - 博客园






# [window 上安装 Scala](https://www.cnblogs.com/camilla/p/8117049.html)

### 第一步：Java 设置
检测方法前文已说明，这里不再描述。
如果还为安装，可以参考我们的[Java 开发环境配置](http://www.runoob.com/java/java-environment-setup.html)。
接下来，我们可以从 Scala 官网地址[http://www.scala-lang.org/downloads](http://www.scala-lang.org/downloads)下载 Scala 二进制包(页面底部)，本教程我们将下载*2.11.7*版本，如下图所示：
![](http://www.runoob.com/wp-content/uploads/2015/12/FFFE6958-C5EB-4C7A-B720-8432B770428D.jpg)
下载后，双击 msi 文件，一步步安装即可，安装过程你可以使用默认的安装目录。
安装好scala后，系统会自动提示，单击 finish，完成安装。
右击我的电脑，单击"属性"，进入如图所示页面。下面开始配置环境变量，右击【我的电脑】--【属性】--【高级系统设置】--【环境变量】，如图：
![](http://www.runoob.com/wp-content/uploads/2015/12/scala1.jpg)
设置 SCALA_HOME 变量：单击新建，在变量名栏输入：**SCALA_HOME**: 变量值一栏输入：**D:\Program Files\scala**也就是scala的安装目录，根据个人情况有所不同，如果安装在C盘，将"D"改成"C"即可。
![](http://www.runoob.com/wp-content/uploads/2015/12/scala2.jpg)
设置 Path 变量：找到系统变量下的"Path"如图，单击编辑。在"变量值"一栏的最前面添加如下的路径： %SCALA_HOME%\bin;%SCALA_HOME%\jre\bin;
**注意：**后面的分号**；**不要漏掉。
![](http://www.runoob.com/wp-content/uploads/2015/12/scala3.jpg)
设置 Classpath 变量：找到找到系统变量下的"Classpath"如图，单击编辑，如没有，则单击"新建":
"变量名"：ClassPath
"变量值"：.;%SCALA_HOME%\bin;%SCALA_HOME%\lib\dt.jar;%SCALA_HOME%\lib\tools.jar.;
**注意：**"变量值"最前面的 .; 不要漏掉。最后单击确定即可。
![](http://www.runoob.com/wp-content/uploads/2015/12/scala4.jpg)
检查环境变量是否设置好了：调出"cmd"检查。单击 【开始】，在输入框中输入cmd，然后"回车"，输入 scala，然后回车，如环境变量设置ok，你应该能看到这些信息。
![](http://www.runoob.com/wp-content/uploads/2015/12/scala5.jpg)
以下列出了不同系统放置的目录（可作为参考）：
|系统环境|变量|值 (举例)|
|---|---|---|
|Unix|$SCALA_HOME|/usr/local/share/scala|
|$PATH|$PATH:$SCALA_HOME/bin|
|Windows|%SCALA_HOME%|c:\Progra~1\Scala|
|%PATH%|%PATH%;%SCALA_HOME%\bin|
cmd命令行执行scala文件：
D:\>cd scala\bin  找到scala安装目录
D:\scala\bin>scalac C:\Users\admin\Desktop\HelloWorld.scala  编译HelloWorld.scala
D:\scala\bin>scala HelloWorld  执行HelloWorld





