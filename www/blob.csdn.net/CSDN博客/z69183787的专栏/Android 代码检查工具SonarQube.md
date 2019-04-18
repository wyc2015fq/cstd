# Android 代码检查工具SonarQube - z69183787的专栏 - CSDN博客
2016年05月25日 23:02:31[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3002

     代码检查工具能帮我们检查一些隐藏的bug，代码检查工具中sonar是比较好的一个。[官网](http://www.sonarqube.org/)
[](http://www.sonarqube.org/)**Sonar 概述**
        Sonar 是一个用于代码质量管理的开放平台。通过插件机制，Sonar 可以集成不同的测试工具，代码分析工具，以及持续集成工具。与持续集成工具（例如 Hudson/Jenkins 等）不同，Sonar 并不是简单地把不同的代码检查工具结果（例如 FindBugs，PMD 等）直接显示在 Web 页面上，而是通过不同的插件对这些结果进行再加工处理，通过量化的方式度量代码质量的变化，从而可以方便地对不同规模和种类的工程进行代码质量管理。
        在对其他工具的支持方面，Sonar 不仅提供了对 IDE 的支持，可以在 Eclipse 和 IntelliJ IDEA 这些工具里联机查看结果；同时 Sonar 还对大量的持续集成工具提供了接口支持，可以很方便地在持续集成中使用 Sonar。
        此外，Sonar 的插件还可以对 Java 以外的其他编程语言提供支持，对国际化以及报告文档化也有良好的支持。
主要特点
　　· 代码覆盖：通过单元测试，将会显示哪行代码被选中
　　· 改善编码规则
　　· 搜寻编码规则：按照名字，插件，激活级别和类别进行查询
　　· 项目搜寻：按照项目的名字进行查询
　　· 对比数据：比较同一张表中的任何测量的趋势
**Sonar 的安装**
         Sonar 是 Codehaus 上面的一个开源项目，使用的是 LGPL V3 软件许可。我们可以在其官方网站上下载其源代码及安装包。其源代码需要使用分布式版本控制软件 Git 进行检出（Check Out），命令行方式如下：
git clone git://github.com/SonarSource/sonar.git
        本文主要介绍 Sonar 的使用方法和[Android](http://lib.csdn.net/base/15) studio插件安装方法，只需要到 [Sonar
 网站](http://www.sonarsource.org/downloads/)下载最近的发行包即可，本文写作时最新的版本为4.5.1，下载 zip 包后，直接解压到任意目录，由于 Sonar 自带了 Jetty 6 的应用服务器环境，所以不需要额外的安装就可以使用，值得一提的是 Sonar 也支持部署在 Apache Tomcat 应用服务器中。在 windows 环境中，直接启动 Soanr 的 bin 目录下 windows-x86-64\StartSonar.bat 即可。然后在浏览器中访问：http://localhost:9000/
 如图（汉化需要额外下载汉化包）：
![](https://img-blog.csdn.net/20141226140545859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmFpbl9idXR0ZXJmbHk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
文档和一些jar包和插件安装点击run analysis：
![](https://img-blog.csdn.net/20141226141554629?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmFpbl9idXR0ZXJmbHk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
首先登录配置项目：
![](https://img-blog.csdn.net/20141226141840203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmFpbl9idXR0ZXJmbHk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
默认是：admin，admin。
登录后如图：
![](https://img-blog.csdn.net/20141226142148921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmFpbl9idXR0ZXJmbHk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
点击Provisioning，点击创建 如图，输入关键字和项目名称（最好和检查代码的项目名称一致）：
![](https://img-blog.csdn.net/20141226142250245?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmFpbl9idXR0ZXJmbHk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
其他的设置不是必须的，可以略过。
如果不用集成插件比较麻烦且不容易使用，下面介绍下在Android studio中安装使用sonar插件(intellij idea类似)。如图：
![](https://img-blog.csdn.net/20141226143006063?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmFpbl9idXR0ZXJmbHk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
点击安装后重启一下就安装好了。
下面说一下配置如下：
![](https://img-blog.csdn.net/20141226143257102?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmFpbl9idXR0ZXJmbHk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
安装完后点击sonarqube选项，添加一个server，输入默认地址和用户名密码，测试连接，ok。
 右击项目关联后台server项目：
![](https://img-blog.csdn.net/20141226144021547?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmFpbl9idXR0ZXJmbHk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
输入配置的项目名称，ok：
![](https://img-blog.csdn.net/20141226144130327?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmFpbl9idXR0ZXJmbHk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
右击analyze--》inspect code（还可以选择run inspection by name 输入 sonarqube issue），控制台就能看到结果了。
![](https://img-blog.csdn.net/20141226145308709?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmFpbl9idXR0ZXJmbHk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
分析结果：
![](https://img-blog.csdn.net/20141226145341812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmFpbl9idXR0ZXJmbHk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
以上是插件形式，但是没有图形化的展示，下面介绍另一种实现方式：
效果图：
![](https://img-blog.csdn.net/20141226164046387?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmFpbl9idXR0ZXJmbHk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这种是不是就清楚多了，下面是详细步骤：
运行分析有好几种方法，这里使用官方建议的方法，下载[SonarQube
 Runner](http://repo1.maven.org/maven2/org/codehaus/sonar/runner/sonar-runner-dist/2.4/sonar-runner-dist-2.4.zip)
地址：[http://repo1.maven.org/maven2/org/codehaus/sonar/runner/sonar-runner-dist/2.4/sonar-runner-dist-2.4.zip](http://repo1.maven.org/maven2/org/codehaus/sonar/runner/sonar-runner-dist/2.4/sonar-runner-dist-2.4.zip)
解压到任意目录，最好不要包含中文路径。
设置环境变量SONAR_RUNNER_HOME，值设置为你的解压目录。
把解压目录/bin 添加到path环境变量。打开命令行输入sonar-runner
 -h
如果配置成功输出：
```java
usage:
 sonar-runner [options]
```
```java
```
```java
Options:
```
```java
```
```java
-D,--define
 <arg>     Define property
```
```java
```
```java
-e,--errors          
 Produce execution error messages
```
```java
```
```java
-h,--help            
 Display help information
```
```java
```
```java
-v,--version         
 Display version information
```
```java
```
```java
-X,--debug           
 Produce execution debug output
```
在需要调试的项目根目录下添加文件：sonar-project.properties
输入以下内容：
```bash
#
 Required metadata
```
```bash
#
 key 和 name 就是创建项目时输入的字段，这里填写自己的项目name和key，其他不用管。
```
```bash
sonar.projectKey=my:project
```
```bash
sonar.projectName=My
 project
```
```bash
sonar.projectVersion=1.0
```
```bash
#
 Path to the parent source code directory.
```
```bash
#
 Path is relative to the sonar-project.properties file. Replace "\" by "/" on Windows.
```
```bash
#
 Since SonarQube 4.2, this property is optional if sonar.modules is set.
```
```bash
#
 If not set, SonarQube starts looking for source code from the directory containing
```
```bash
#
 the sonar-project.properties file.
```
```bash
sonar.sources=src
```
```bash
#
 Encoding of the source code
```
```bash
sonar.sourceEncoding=UTF-8
```
```bash
#
 Additional parameters
```
```bash
sonar.my.property=value
```
然后命令行进入自己的项目跟目录，执行sonar-runner
例：D:\AndroidStudioWork\wodou>
 sonar-runner 打开控制台就能看到可视化数据了。
![](https://img-blog.csdn.net/20141226170347734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmFpbl9idXR0ZXJmbHk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
如有问题请留言，转载注明出处。
