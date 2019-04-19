# sbt使用详解 - BlueSky - CSDN博客
2015年11月20日 19:19:01[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：17333
前两天安好的sbt但是在真正的用的时候还是遇到很多问题，所以在系统学习了解下。
## 安装：
我的系统为centos6.5故采用官方提供的在线安装：
```
curl https://bintray.com/sbt/rpm/rpm > bintray-sbt-rpm.repo
sudo mv bintray-sbt-rpm.repo /etc/yum.repos.d/
sudo yum install sbt
```
然后等完毕可以输入 sbt sbt-version来进行查看安装是否成功。（这一步可能会很辛苦因为第一次运行的时候他会下载很多东西，而且大部分资源都是在国外，很可能不成功，所以还是耐心点。我前几篇有文章介绍加快sbt下载的文章可以去看下，但是感觉下载源的时候还是有点帮助。）
## sbt执行步骤：
sbt 完全按照约定工作。sbt 将会自动找到以下内容：
- 
项目根目录下的源文件
- `src/main/scala` 或 `src/main/java` 中的源文件
- `src/test/scala` 或 `src/test/java` 中的测试文件
- `src/main/resources` 或 `src/test/resources` 中的数据文件
- `lib` 中的 jar 文件
默认情况下，sbt 会用和启动自身相同版本的 Scala 来构建项目。 你可以通过执行 `sbt run` 来运行项目或者通过 
```
sbt
 console
```
 进入 [Scala
 REPL](http://www.scala-lang.org/node/2097)。`sbt console` 已经帮你 设置好项目的 classpath，所以可以根据项目的代码尝试实际的 Scala 示例。
## sbt结构说明
### 基础目录[](http://www.scala-sbt.org/0.13/tutorial/zh-cn/Directories.html#%E5%9F%BA%E7%A1%80%E7%9B%AE%E5%BD%95)
在 sbt 的术语里，“基础目录”是包含项目的目录。所以，如果你创建了一个和 [Hello,
 World](http://www.scala-sbt.org/0.13/tutorial/zh-cn/Hello.html) 一样的项目`hello` ，包含 `hello/build.sbt` 和 `hello/hw.scala`， `hello` 就是基础目录。
### 源代码[](http://www.scala-sbt.org/0.13/tutorial/zh-cn/Directories.html#%E6%BA%90%E4%BB%A3%E7%A0%81)
源代码可以像 `hello/hw.scala` 一样的放在项目的基础目录中。然而，大多数人不会在真实的项目中这样做，因为太杂乱了。 sbt 和 [Maven](https://maven.apache.org/) 的默认的源文件的目录结构是一样的（所有的路径都是相对于基础目录的）：
```
src/
  main/
    resources/
       <files to include in main jar here>
    scala/
       <main Scala sources>
    java/
       <main Java sources>
  test/
    resources
       <files to include in test jar here>
    scala/
       <test Scala sources>
    java/
       <test Java sources>
```
`src/` 中其他的目录将被忽略。而且，所有的隐藏目录也会被忽略。
### 构建产品[](http://www.scala-sbt.org/0.13/tutorial/zh-cn/Directories.html#%E6%9E%84%E5%BB%BA%E4%BA%A7%E5%93%81)
构建出来的文件（编译的 classes，打包的 jars，托管文件，caches 和文档）默认写在 `target` 目录中。
### 交互模式[](http://www.scala-sbt.org/0.13/tutorial/zh-cn/Running.html#%E4%BA%A4%E4%BA%92%E6%A8%A1%E5%BC%8F)
在你的项目目录下运行 sbt 不跟任何参数：
```
$ sbt
```
执行 sbt 不跟任何命令行参数将会进入交互模式。交互模式有一个命令行（含有 tab 自动补全功能和历史记录）。
例如，在 sbt 命令行里输入 `compile`：
```
> compile
```
再次 `compile`，只需要按向上的方向键，然后回车。 输入 `run` 来启动程序。
 输入 `exit` 或者 Ctrl+D （Unix）或者 Ctrl+Z （Windows）可以退出交互模式。
### 批处理模式[](http://www.scala-sbt.org/0.13/tutorial/zh-cn/Running.html#%E6%89%B9%E5%A4%84%E7%90%86%E6%A8%A1%E5%BC%8F)
你也可以用批处理模式来运行 sbt，可以以空格为分隔符指定参数。对于接受参数的 sbt 命令，将命令和参数用引号引起来一起传给 sbt。例如：
```
$ sbt clean compile "testOnly TestA TestB"
```
在这个例子中，`testOnly` 有两个参数 `TestA` 和 `TestB`。这个命令会按顺序执行（`clean`， `compile`，
 然后 `testOnly`）。
### 常用命令[](http://www.scala-sbt.org/0.13/tutorial/zh-cn/Running.html#%E5%B8%B8%E7%94%A8%E5%91%BD%E4%BB%A4)
下面是一些非常常用的的 sbt 命令。更加详细的列表请参见 [命令行参考](http://www.scala-sbt.org/0.13/tutorial/docs/Command-Line-Reference.html)。
|clean|删除所有生成的文件 （在 target 目录下）。|
|----|----|
|compile|编译源文件（在 src/main/scala 和 src/main/java 目录下）。|
|test|编译和运行所有测试。|
|console|进入到一个包含所有编译的文件和所有依赖的 classpath 的 Scala 解析器。输入 :quit， Ctrl+D （Unix），或者 Ctrl+Z （Windows） 返回到 sbt。|
|run <参数>*|在和 sbt 所处的同一个虚拟机上执行项目的 main class。|
|package|将 src/main/resources 下的文件和 src/main/scala 以及 src/main/java 中编译出来的 class 文件打包成一个 jar 文件。|
|help <命令>|显示指定的命令的详细帮助信息。如果没有指定命令，会显示所有命令的简介。|
|reload|重新加载构建定义（build.sbt， project/*.scala， project/*.sbt 这些文件中定义的内容)。在修改了构建定义文件之后需要重新加载。|
### 添加依赖库[](http://www.scala-sbt.org/0.13/tutorial/zh-cn/Basic-Def.html#%E6%B7%BB%E5%8A%A0%E4%BE%9D%E8%B5%96%E5%BA%93)
有两种方式添加第三方的依赖。一种是将 jar 文件 放入 `lib/`（非托管的依赖）中，另一种是在`build.sbt` 中添加托管的依赖，像这样：
```
val derby = "org.apache.derby" % "derby" % "10.4.1.3"
lazy val commonSettings = Seq(
  organization := "com.example",
  version := "0.1.0",
  scalaVersion := "2.11.4"
)
lazy val root = (project in file(".")).
  settings(commonSettings: _*).
  settings(
    name := "hello",
    libraryDependencies += derby
  )
```
就是像这样添加版本为 10.4.1.3 的 Apache Derby 库作为依赖。
key `libraryDependencies` 包含两个方面的复杂性：`+=` 方法而不是 `:=`，第二个就是 `%` 方法。`+=`方法是将新的值追加该
 key 的旧值后面而不是替换它，这将在 [更多设置](http://www.scala-sbt.org/0.13/tutorial/zh-cn/More-About-Settings.html) 中介绍。`%` 方法是用来从字符串构造
 Ivy 模块 ID 的，将在 [库依赖](http://www.scala-sbt.org/0.13/tutorial/zh-cn/Library-Dependencies.html) 中介绍。
