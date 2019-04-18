# ANT安装、配置 - weixin_33985507的博客 - CSDN博客
2004年08月27日 23:37:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
**[J2EE相关](http://www.jieesoft.com/modules.php?name=News&file=categories&op=newindex&catid=6): ANT安装、配置**
发表于 Tuesday, July 27 @ 11:33:03 CST by [joezxh](http://www.jieesoft.com/)
![J2EE Framework:Struts,Turbine,Spring etc](http://www.jieesoft.com/images/topics/newstart.gif)[joe.zhang](http://www.jieesoft.com/modules.php?name=Your_Account&op=userinfo&username=joe.zhang) 投递 "作者： 车东 Email: chedongATbigfoot.com/chedongATchedong.com 
写于：2003/05 最后更新： 
07/27/2004 11:13:47![ant.html](http://www.chedong.com/referer.php?ref=http%3A//www.chedong.com/&self=http://www.chedong.com/tech/ant.html)[Feed Back >>](http://www.chedong.com/guestbook/)[![Creative Commons License](http://www.creativecommons.cn/images/public/somerights.gif)](http://www.creativecommons.cn/licenses/by-sa/1.0/)
版权声明：可以任意转载，转载时请务必以超链接形式标明文章原始出处和作者信息及本声明
[http://www.chedong.com/tech/ant.html](http://www.chedong.com/tech/ant.html)
关键词：ant build.xml javac
内容摘要：
ant是一个基于JAVA的自动化脚本引擎，脚本格式为XML。除了做JAVA编译相关任务外，ANT还可以通过插件实现很多应用的调用。
- 
- ANT的基本概念： 
- ANT的安装：解包，设置路径 
- ANT的使用：最好的学习只不过是一个简单实用的例子起步…… 
ANT的基本概念：Java的Makefile 
当一个代码项目大了以后，每次重新编译，打包，测试等都会变得非常复杂而且重复，因此c语言中有make脚本来帮助这些工作的批量完成。在Java 中应用是平台无关性的，当然不会用平台相关的make脚本来完成这些批处理任务了，ANT本身就是这样一个流程脚本引擎，用于自动化调用程序完成项目的编译，打包，测试等。除了基于JAVA是平台无关的外，脚本的格式是基于XML的，比make脚本来说还要好维护一些。
每个ant脚本（缺省叫build.xml）中设置了一系列任务(target)：比如对于一个一般的项目可能需要有以下任务。 
- 
- 任务1：usage 打印本脚本的帮助信息（缺省） 
- 任务2：clean 任务3：javadoc 任务4：jar 任务5：all 
而多个任务之间往往又包含了一定了依赖关系：比如把整个应用打包任务(jar)的这个依赖于编译任务(build)，而编译任务又依赖于整个环境初始化任务(init)等。
注：我看到很多项目的ant脚本中的命名基本上都是一致的，比如：编译一般叫build或者compile；打包一般叫jar或war；生成文档一般命名为javadoc或javadocs；执行全部任务all。在每个任务的中，ANT会根据配置调用一些外部应用并配以相应参数执行。虽然ANT可调用的外部应用种类非常丰富，但其实最常用的就2，3个：比如javac javadoc jar等。 
ANT的安装解包后在系统可执行路径中加入指向ant的bin的路径就可以了，比如可以在GNU/Linux上把以下配置加入/etc/profile中：
export ANT_HOME=/home/ant
export JAVA_HOME=/usr/java/j2sdk1.4.1
export PATH=$PATH:$JAVA_HOME/bin:$ANT_HOME/bin
这样执行ant 后，如果不指定配置文件ant会缺省找build.xml这个配置文件，并根据配置文件执行任务，缺省的任务设置可以指向最常用的任务，比如： build，或指向打印帮助信息：usage，告诉用户有那些脚本选项可以使用。
ANT的使用
最好的学习过程就是看懂那些open source项目中的build.xml脚本，然后根据自己的需要简化成一个更简单的，ANT和APACHE上很多非常工程派的项目：简单易用，而且适应性非常强，因为这些项目的建立往往来源于开发人员日常最直接的需求。
以下是的一个[WebLucene](http://sourceforge.net/projects/weblucene/)应用的例子：修改自[JDOM](http://www.jdom.org/)的build.xml 
    generates the ${name}.jar file"/>
    compiles the source code"/>
    generates the API documentation"/>
    cleans up the directory"/>
             destdir="${build.dest}"
           debug="${debug}"
           optimize="${optimize}">
               basedir="${build.dest}"
         includes="**"/>
             sourcepath="${build.src}"
             destdir="${build.javadocs}"
             author="true"
             version="true"
             use="true"
             splitindex="true"
             windowtitle="${Name} API"
             doctitle="${Name}">
  缺省任务：usage 打印帮助文档，告诉有那些任务选项：可用的有build, jar, javadoc和clean.
初始化环境变量：init
所有任务都基于一些基本环境变量的设置初始化完成，是后续其他任务的基础，在环境初始化过程中，有2点比较可以方便设置：
1 除了使用却缺省的property设置了JAVA源路径和输出路径外，引用了一个外部的build.properties文件中的设置，
这样大部分简单配置用户只要会看懂build.properties就可以了，毕竟XML比起key value的属性文件还是要可读性差一些。用build.properties也可以方便其他用户从编译的细节中解放出来。
2 CLASSPATH设置：使用了其中的：
   则相当于设置了：CLASSPATH=/path/to/resin/lib/jsdk23.jar; /path/to/project/lib/*.jar;
文件复制：prepare-src
创建临时SRC存放目录和输出目录。
  编译任务：build
编译时的CLASSPATH环境通过一下方式找到引用一个path对象
打包任务：jar
对应用打包生成项目所写名的.jar文件
  basedir="${build.dest}"
         includes="**"/>
生成JAVADOC文档任务: javadoc
                 sourcepath="${build.src}"
             destdir="${build.javadocs}"
             author="true"
             version="true"
             use="true"
             splitindex="true"
             windowtitle="${Name} API"
             doctitle="${Name}">
清空临时编译文件：clean
 TODO：
更多任务/扩展：（样例）
- 
- 测试任务：JUnit测试 
- 代码风格检查任务：CheckStyle，Jalopy等 
- 邮件警报任务：可以把以上这些任务的输出警告发送到制定的用户列表中，这个任务可以设置每天自动运行。 
参考资料：
Jakarta ANT:
[http://ant.apache.org](http://ant.apache.org/)
原文出处：[http://www.chedong.com/tech/ant.html](http://www.chedong.com/tech/ant.html)
" 
