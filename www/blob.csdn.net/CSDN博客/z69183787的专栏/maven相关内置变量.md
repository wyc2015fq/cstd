# maven相关内置变量 - z69183787的专栏 - CSDN博客
2014年05月15日 14:33:31[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2346
 1、Maven内置变量说明：
- ${basedir} 项目根目录
- ${project.build.directory} 构建目录，缺省为target
- ${project.build.outputDirectory} 构建过程输出目录，缺省为target/classes
- ${project.build.finalName} 产出物名称，缺省为${project.artifactId}-${project.version}
- ${project.packaging} 打包类型，缺省为jar
- ${project.xxx} 当前pom文件的任意节点的内容
2、一些命令：
- 
```java
生成eclipse工程文件：mvn eclipse:clean eclipse:eclipse -DdownloadSources
```
- 
```java
如果执行单元测试出错，用该命令可以在console输出失败的单元测试及相关信息：mvn **-Dsurefire.useFile=false**
```
- 
```java
有时候，希望在mvn install时，对项目中的单元测试进行调试，使用该命令：mvn install **-Dmaven.surefire.debug** 。在使用该命令前，在你将要调试的代码中设置好断点，然后运行该命令。该命令执行一会之后，它的build过程会显示信息：Listening for transport dt_socket at address : 5005 。看到这个提示信息之后，在eclipse中，在Debug Configuration窗口，新建Remote Java Application，port设置为5005，然后点击“debug”按钮。之后，刚才的mvn命令将继续往下执行，直到运行到有断点的代码，则停留下来，这时在eclipse中可以调试运行的代码。
```
 3. maven jetty 插件的一些命令：（资料：[http://wiki.eclipse.org/Jetty/Feature/Jetty_Maven_Plugin](http://wiki.eclipse.org/Jetty/Feature/Jetty_Maven_Plugin)）
- 启动：mvn jetty:run    默认的端口号是8080。
- 启动时，设置端口号： mvn -Djetty.port=9999 jetty:run
- debug的方式启动：mvnDebug jetty:run    启动之后，会显示具体的debug端口
