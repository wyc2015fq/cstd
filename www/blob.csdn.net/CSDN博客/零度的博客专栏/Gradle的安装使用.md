# Gradle的安装使用 - 零度的博客专栏 - CSDN博客
2016年08月09日 10:54:18[零度anngle](https://me.csdn.net/zmx729618)阅读数：597标签：[Gradle](https://so.csdn.net/so/search/s.do?q=Gradle&t=blog)
个人分类：[Gradle](https://blog.csdn.net/zmx729618/article/category/6356217)
一 Gradle安装和命令介绍 ：
2. Gradle基本使用：
   新建一个项目后，在项目文件夹下创建build.gradle文件,并加入内容：
      apply plugin: 'eclipse'。
   然后在终端运行gradle eclipse即可构建eclipse IDE的开发环境。
   gradle默认值：gradle有些目录是有默认值存在,建议项目的配置，承袭了maven的风格，如：
        java的源码目录：src/main/java
        java的测试代码目录：src/test/java
        资源文件目录：src/main/resources
        测试的资源目录：src/test/resources
        WEB项目的目录：src/main/webapp   可通过webAppDirName变量更改  ex: webAppDirName="webapp"（此属性官方已不建议使用）
Java代码
- apply plugin: 'maven'
-     apply plugin:  'war'
-     apply plugin:  'eclipse-wtp'
-     apply plugin:  'jetty'
- 
-     webAppDirName =  'webapp'
- 
-     repositories {  
-         mavenCentral()  
-     }  
- 
-     dependencies {  
-         compile (  
- "javax.servlet:javax.servlet-api:3.1-b07"
,  
- "org.slf4j:slf4j-log4j12:1.7.5",  
- "org.slf4j:slf4j-jdk14:1.7.5",  
- "mysql:mysql-connector-java:5.1.24"
-         )  
-     }  
- 
-     task copyHbm(type: Copy){  
-         from  'src/main/java/com/base/model'
-         into  'build/classes/main/com/base/model'
-         exclude  '**/*.java'
-     }  
build.gradle文件常用任务解析：
**apply plugin: 'maven'：** 使用maven做为jar包的信赖管理，通过mave仓库下载项目所需的信赖包
**apply plugin: 'war'：** 指定web项目，项目编译(在项目提示符下执行：gradle build)时生成项目的war包。
**apply plugin: 'java':** 指定项目为java项目，项目编译(在项目提示符下执行：gradle build)时生成项目的jar包。
**apply plugin: 'eclipse-wtp'：** 插件将构建web项目的开发环境，生成所需要的.project,.classpath等文件。因为我web开发使用的是eclipse-j2ee版本，所以指定为wtp环境。
**apply plugin: 'eclipse'：** java项目的eclipse开发环境构建.生成所需要的.project,.classpath等文件。
**apply plugin: 'jetty'：** 加入jetty的支持，代码修改后直接执行命令gradle jettyRun即可运行web项目。
**repositories:** 指定仓库使用。 
**dependencies：** 项目依赖定义，compile为编译级别依赖，还有testCompile为测试级别的依赖等。
*原文*[http://blog.csdn.net/joeyon1985/article/details/38534833](http://blog.csdn.net/joeyon1985/article/details/38534833)
Java代码
- compile group: 'org.hibernate', name: 'hibernate-core', version: '3.6.7.Final'
- testCompile group:  'junit', name: 'junit', version: '4.+'
**task copyHbm(type: Copy)：** 是自定义的一个任务，目的是将hibernate的映射文件也拷贝到正确的目录，gradle默认源码包下只对java文件名进行打包，xml文件则被忽略了。在项目提示符下gradle copyHbm即可执行任务定义的动作。
gradle常用命令：
        gradle eclipse : 构建开发环境，下载定义的jar包
        gradle build : 编译项目，生成build文件夹，并生成相应的jar或war包。
        gradle clean : 与build相反，删除build文件夹。
二,gradle构建web项目 
1.在build.gradle文件中添加jetty插件支持：apply plugin : 'jetty'
2.设置web访问参数：
  httpPort = 8080
  stopPort = 8089
  stopKey  = 'ss'
3.启动jetty：右击工程选择run as-》gradle build...，执行gradle jettyRun或者jettyRunWar任务，即启动jetty服务
4.关闭jetty：右击工程选择run as-》gradle build...，执行gradle jettyStop，不加任何设置，但是必须有第2步的stopKey的设置，即可关闭jetty。
