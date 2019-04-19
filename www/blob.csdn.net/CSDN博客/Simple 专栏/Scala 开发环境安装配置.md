# Scala  开发环境安装配置 - Simple 专栏 - CSDN博客
2018年09月14日 16:30:15[Simple_Zz](https://me.csdn.net/love284969214)阅读数：4718
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
*Scala*可以安装在任何基于UNIX/Linux或基于Windows的系统上。在您的机器上开始安装Scala之前，必须在计算机上安装*Java 1.8*或更高版本。
## 基于命令行安装Scala
从 [http://www.scala-lang.org/downloads](http://www.scala-lang.org/downloads) 下载*Scala*安装包。有两种安装方式：
- IDE安装包
- 命令行安装包(本教程使用的安装方式)
在撰写本教程的时候，我下载的是*“sbt-1.0.0.msi”*，下载完成后安装即可(注：确保您具有管理员权限以继续)。现在，双击下载好的*“sbt-1.0.0.msi”*执行安装 -
![](http://www.yiibai.com/uploads/images/201708/2808/129090802_36631.png)
*第2步：*接受安装协议 -
![](http://www.yiibai.com/uploads/images/201708/2808/373090803_46945.png)
*第3步：*选择安装位置(*D:\Program Files (x86)\sbt\*) -
![](http://www.yiibai.com/uploads/images/201708/2808/117090805_61990.png)
*第4步：* 开始安装 -
![](http://www.yiibai.com/uploads/images/201708/2808/477090805_87083.png)
*第5步：* 安装完成 -
![](http://www.yiibai.com/uploads/images/201708/2808/819090806_70870.png)
最后，打开一个新的命令提示符并键入:`sbt -version`，并按Enter键。应该看到以下内容 -
```
F:\worksp\scala\helloworld>sbt
"C:\Users\Administrator\.sbt\preloaded\org.scala-sbt\sbt\"1.0.0"\jars\sbt.jar"
Java HotSpot(TM) 64-Bit Server VM warning: ignoring option MaxPermSize=256m; support was removed in 8.0
```
**测试程序** -
要使用*sbt*从命令行运行代码，首先创建一个Scala创建一个示例项目(*helloworld*)。创建一个目录：*F:\worksp\scala\helloworld*，并在此目录下创建一个存放源文件的目录：*F:\worksp\scala\helloworld\main\scala*，创建一个scala源文件：*helloworld.scala*，其代码如下 -
```
object helloworld {
  def main(args: Array[String]) = println("Hi,This is my first Scala App !")
}
```
在这个例子中，sbt 完全按照约定工作。sbt 将会自动找到以下内容：
项目根目录下的源文件 -
`src/main/scala` 或 `src/main/java` 中的源文件
`src/test/scala` 或 `src/test/java` 中的测试文件
`src/main/resource`s 或 `src/test/resources` 中的数据文件
`lib` 中的 `jar` 文件
默认情况下，sbt 会用和启动自身相同版本的 Scala 来构建项目。
执行上面示例代码，如下所示 -
```
F:\worksp\scala\helloworld> sbt
"C:\Users\Administrator\.sbt\preloaded\org.scala-sbt\sbt\"1.0.0"\jars\sbt.jar"
Java HotSpot(TM) 64-Bit Server VM warning: ignoring option MaxPermSize=256m; support was removed in 8.0
[warn] No sbt.version set in project/build.properties, base directory: F:\worksp\scala\helloworld
[info] Set current project to helloworld (in build file:/F:/worksp/scala/helloworld/)
[info] sbt server started at 127.0.0.1:5893
sbt:helloworld> run
[info] Running helloworld
Hi,This is my first Scala App !
[success] Total time: 2 s, completed 2017-8-28 21:31:24
sbt:helloworld>
```
### 基于IntelliJ IDE行安装Scala
- 确保你有Java 8 JDK(也称为1.8)
- 在命令行上运行`javac -version`，并确保看到类似 *javac 1.8.xxx*
- 
如果您没有安装*JDK 1.8*或更高版本，请安装JDK。
- 
安装 IntelliJ 社区版(参考： [http://www.jetbrains.com/idea/download/](http://www.jetbrains.com/idea/download/) )
**安装Scala插件**
- 如何安装 IntelliJ 插件？
首先，打开 *IntelliJ IDEA* 中的设置(*Setting*)，在主菜单 *File* -> *Settings…*，打开后如下图所示 -
![](http://www.yiibai.com/uploads/images/201708/2808/353100802_14629.png)
在左侧选项中，查找 *Plugins* 选项，并点击下方的 *Browse repositories…*，在弹出的对话框中搜索 *scala*后点击安装，如下图所示 -
![](http://www.yiibai.com/uploads/images/201708/2808/350100805_68574.png)
安装完成后，重启 *IntelliJ IDEA*。
**创建新的Scala项目**
打开IntelliJ，然后单击：*File => New => Project*，在左侧面板中，选择*Scala*。 在右侧面板上，再次选择*Scala*。将项目命名为：*ScalaDemo*，如下图所示 -
![](http://www.yiibai.com/uploads/images/201708/2808/303100810_30126.png)
假设这是您第一次使用*IntelliJ IDEA*创建*Scala*项目，则需要安装Scala SDK。 在Scala SDK字段的右侧，单击创建按钮。
选择最高版本号(例如*2.12.3*)，然后单击下载。 这可能需要几分钟，但后续项目可以使用相同的SDK。创建SDK后，单击完成。
![](http://www.yiibai.com/uploads/images/201708/2808/240110851_19507.png)
当创建项目时，我们将安装最新版本的*Scala*。 注意：如果要打开现有的Scala项目，可以在启动 *IntelliJ*时单击打开。
**编写代码**
在左侧的“项目”窗格中，右键单击*src => main => scala *，然后选择*New => Scala Class*创建一个类。将此类文件命名为：*Hello* ， 如下图所示 -
![](http://www.yiibai.com/uploads/images/201708/2908/180120825_58124.png)
将类中的代码更改为以下内容：
```
object Hello extends App {
  println("Hello, World!")
}
```
**执行代码**
右键点击代码源文件：*Hello.scala*，然后选择*Run’Hello’*。执行结果如下 -
![](http://www.yiibai.com/uploads/images/201708/2908/637120828_28065.png)
至此，有关Scala开发环境安装配置演示和讲解完成~！
