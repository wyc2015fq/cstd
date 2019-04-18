# 如何将maven项目打包成可执行的jar - z69183787的专栏 - CSDN博客
2014年08月04日 13:10:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4513
[maven项目如何执行main方法](http://blog.csdn.net/symgdwyh/article/details/6094586)
请参考[http://mojo.codehaus.org/exec-maven-plugin/](http://mojo.codehaus.org/exec-maven-plugin/)
方法一：将项目及所依赖的所有jar包打包成一个jar。
1、pom.xml添加assembly插件
![](http://hi.csdn.net/attachment/201012/23/0_1293091608s84q.gif)
2、执行mvn assembly:assembly
3、生成如下文件
![](http://hi.csdn.net/attachment/201012/17/0_1292552112Qu9j.gif)
4、执行java -jar quickstart-1.0.0-jar-with-dependencies.jar
备注：第2步的命令也可以改成mvn package，但需要更改pom.xml：
![](http://hi.csdn.net/attachment/201012/23/0_12930906982eRb.gif)
方法二：将项目依赖的jar复制到一个目录中并在MANIFEST文件中添加Class-Path和Main-Class。
1、在pom.xml中添加jar和dependency插件
![](http://hi.csdn.net/attachment/201012/23/0_1293091014R9Z9.gif)
2、执行mvn package
3、生成如下文件
![](http://hi.csdn.net/attachment/201012/23/0_1293091859pEbl.gif)
4、执行java -jar quickstart-1.0.0.jar
