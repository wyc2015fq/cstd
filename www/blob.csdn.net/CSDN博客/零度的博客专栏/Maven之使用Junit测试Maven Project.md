# Maven之使用Junit测试Maven Project - 零度的博客专栏 - CSDN博客
2016年07月14日 15:42:39[零度anngle](https://me.csdn.net/zmx729618)阅读数：1878
       每个开发人员都会对自己的代码进行自定义的测试，可以是把项目run起来，手动点点页面按钮，看看操作场景和步骤点是否符合业务需要，是否存在UE上的问题。也有自己写几个测试类，把service类的输入输出是否符合标准都测试一番，这两大类，其实就是包括了前后端的测试工作，分工各有不同。在maven中集成了junit测试包，应该说maven可以集成任何你想的到或者想不到工具插件。
       在maven project-01项目中引入junit插件并不困难，只需要在pom.xml中配置好依赖包即可（IDE环境下，依赖项会自动生成）。剩下的测试代码的编写并没有任何不同。另外，test类文件需要按照/src/main/java/的文件结构（main改为test）。
        test文件夹结构如下图：
![](https://img-blog.csdn.net/20131002193344968?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGZzZnh5OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        还需要修改pom.xml文件，增加dependencies项，改后的pom.xml内容如下：
**[html]**[view
 plain](http://blog.csdn.net/lfsfxy9/article/details/12201033#)[copy](http://blog.csdn.net/lfsfxy9/article/details/12201033#)[print](http://blog.csdn.net/lfsfxy9/article/details/12201033#)[?](http://blog.csdn.net/lfsfxy9/article/details/12201033#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/193662/fork)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <projectxmlns="http://maven.apache.org/POM/4.0.0"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
- <modelVersion>4.0.0</modelVersion>
- 
- <groupId>jizg.study.maven.hello</groupId>
- <artifactId>hello-first</artifactId>
- <version>0.0.1-SNAPSHOT</version>
- 
- <dependencies>
- <dependency>
- <groupId>junit</groupId>
- <artifactId>junit</artifactId>
- <version>4.10</version>
- <scope>test</scope>
- </dependency>
- </dependencies>
- </project>
![](http://static.blog.csdn.net/images/save_snippets.png)
      接下来在/src/test/java/jizg/study/maven/hello 文件夹下创建TestHello.java文件，这里需要注意，test的包结构可以自定义，要注意必备的路径为/src/test/java/，TestHello.java内容如下：
**[html]**[view
 plain](http://blog.csdn.net/lfsfxy9/article/details/12201033#)[copy](http://blog.csdn.net/lfsfxy9/article/details/12201033#)[print](http://blog.csdn.net/lfsfxy9/article/details/12201033#)[?](http://blog.csdn.net/lfsfxy9/article/details/12201033#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/193662/fork)
- package jizg.study.maven.hello;  
- 
- import org.junit.*;  
- import static junit.framework.Assert.*;  
- import jizg.study.maven.hello.*;  
- 
- public class TestHello{  
- 
-      @Test  
-      public void testHello(){       
-           Hello h = new Hello();  
-           assertEquals(h.sayHello("jizg"),"hello :jizg");  
-      }  
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
   最后，改好pom.xml和test类文件之后，可以输入mvn test命令，这会重新把项目build出来，并且输出TestHello.java中的test信息。
控制台输出如下：
**[html]**[view
 plain](http://blog.csdn.net/lfsfxy9/article/details/12201033#)[copy](http://blog.csdn.net/lfsfxy9/article/details/12201033#)[print](http://blog.csdn.net/lfsfxy9/article/details/12201033#)[?](http://blog.csdn.net/lfsfxy9/article/details/12201033#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/193662/fork)
- D:\study\maven\01>mvn test  
- [INFO] Scanning for projects...  
- [INFO]  
- [INFO] ------------------------------------------------------------------------  
- [INFO] Building hello-first 0.0.1-SNAPSHOT  
- [INFO] ------------------------------------------------------------------------  
- [INFO]  
- [INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ hello-firs  
- t ---  
- [WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e  
- . build is platform dependent!  
- [INFO] skip non existing resourceDirectory D:\study\maven\01\src\main\resources  
- [INFO]  
- [INFO] --- maven-compiler-plugin:2.5.1:compile (default-compile) @ hello-first -  
- --  
- [INFO] Nothing to compile - all classes are up to date  
- [INFO]  
- [INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ he  
- llo-first ---  
- [WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e  
- . build is platform dependent!  
- [INFO] skip non existing resourceDirectory D:\study\maven\01\src\test\resources  
- [INFO]  
- [INFO] --- maven-compiler-plugin:2.5.1:testCompile (default-testCompile) @ hello  
- -first ---  
- [WARNING] File encoding has not been set, using platform encoding GBK, i.e. buil  
- d is platform dependent!  
- [INFO] Compiling 1 source file to D:\study\maven\01\target\test-classes  
- [INFO]  
- [INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ hello-first ---  
- [INFO] Surefire report directory: D:\study\maven\01\target\surefire-reports  
- 
- -------------------------------------------------------  
- T E S T S  
- -------------------------------------------------------  
- Running test.TestHello.TestHello  
- Tests run: 1, Failures: 0, Errors: 0, Skipped: 0, Time elapsed: 0.151 sec  
- 
- Results :  
- 
- Tests run: 1, Failures: 0, Errors: 0, Skipped: 0  
- 
- [INFO] ------------------------------------------------------------------------  
- [INFO] BUILD SUCCESS  
- [INFO] ------------------------------------------------------------------------  
- [INFO] Total time: 5.346s  
- [INFO] Finished at: Wed Oct 02 18:23:23 CST 2013  
- [INFO] Final Memory: 9M/22M  
- [INFO] ------------------------------------------------------------------------  
- D:\study\maven\01>
![](http://static.blog.csdn.net/images/save_snippets.png)
     细心的朋友还会发现，在project-01目录下，新生成了一个target（项目输出）文件夹，下面包括surefire-reports（测试结果）和编译过后的class文件。mvn test可以很好的支持单元测试，maven下的好多命令可以完成其中奇葩怪异的任务，并且mvn 命令支持串行执行。比如，mvn  install、mvn clean build等等。
