# Java探针-Java Agent技术 - z69183787的专栏 - CSDN博客
2019年03月23日 17:25:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：25
个人分类：[Java-JavaAgent](https://blog.csdn.net/z69183787/article/category/8787879)
[https://www.cnblogs.com/aspirant/p/8796974.html](https://www.cnblogs.com/aspirant/p/8796974.html)
最近面试阿里，面试官先是问我类加载的流程，然后问了个问题，能否在加载类的时候，对字节码进行修改
我懵逼了，答曰不知道，面试官说可以的，使用Java探针技术，能够实现
我查了一下关于探针技术的知识：
2. 基于javaAgent和Java字节码注入技术的java探针工具技术原理
![](https://images2018.cnblogs.com/blog/137084/201804/137084-20180411170500895-1256431982.jpg)
图0-0：动态代理功能实现说明
我们利用javaAgent和ASM字节码技术开发java探针工具，实现原理如下：
jdk1.5以后引入了javaAgent技术，javaAgent是运行方法之前的拦截器。我们利用javaAgent和ASM字节码技术，在JVM加载class二进制文件的时候，利用ASM动态的修改加载的class文件，在监控的方法前后添加计时器功能，用于计算监控方法耗时，同时将方法耗时及内部调用情况放入处理器，处理器利用栈先进后出的特点对方法调用先后顺序做处理，当一个请求处理结束后，将耗时方法轨迹和入参map输出到文件中，然后根据map中相应参数或耗时方法轨迹中的关键代码区分出我们要抓取的耗时业务。最后将相应耗时轨迹文件取下来，转化为xml格式并进行解析，通过浏览器将代码分层结构展示出来，方便耗时分析，如图0-1所示。
![](https://images2018.cnblogs.com/blog/137084/201804/137084-20180411170508252-1331977522.jpg)
图0-1：java探针工具原理图
Java探针工具功能点:
1、支持方法执行耗时范围抓取设置，根据耗时范围抓取系统运行时出现在设置耗时范围的代码运行轨迹。
2、支持抓取特定的代码配置，方便对配置的特定方法进行抓取，过滤出关系的代码执行耗时情况。
3、支持APP层入口方法过滤，配置入口运行前的方法进行监控，相当于监控特有的方法耗时，进行方法专题分析。
4、支持入口方法参数输出功能，方便跟踪耗时高的时候对应的入参数。
5、提供WEB页面展示接口耗时展示、代码调用关系图展示、方法耗时百分比展示、可疑方法凸显功能。
下面看个例子：
第一篇：
JavaAgent 是JDK 1.5 以后引入的，也可以叫做Java代理。
JavaAgent 是运行在 main方法之前的拦截器，它内定的方法名叫 premain ，也就是说先执行 premain 方法然后再执行 main 方法。
那么如何实现一个 JavaAgent 呢？很简单，只需要增加 premain 方法即可。
看下面的代码和代码中的注释说明：
先写一个premain方法： 
![复制代码](https://common.cnblogs.com/images/copycode.gif)
package agent;
import java.lang.instrument.Instrumentation;
public class pre_MyProgram {
     /**
     * 该方法在main方法之前运行，与main方法运行在同一个JVM中
     * 并被同一个System ClassLoader装载
     * 被统一的安全策略(security policy)和上下文(context)管理
     *
     * @param agentOps
     * @param inst
     * @author SHANHY
     * @create  2016年3月30日
     */
    public static void    premain(String agentOps,Instrumentation inst){
        
        System.out.println("====premain 方法执行");
        System.out.println(agentOps);
    }
    
     /**
     * 如果不存在 premain(String agentOps, Instrumentation inst) 
     * 则会执行 premain(String agentOps)
     *
     * @param agentOps
     * @author SHANHY
     * @create  2016年3月30日
     */
   public static void premain(String agentOps){
       
       System.out.println("====premain方法执行2====");
       System.out.println(agentOps);
   }
    public static void main(String[] args) {
        // TODO Auto-generated method stub   
        
    }
}
 
![复制代码](https://common.cnblogs.com/images/copycode.gif)
写完这个类后，我们还需要做一步配置工作。
在 src 目录下添加 META-INF/MANIFEST.MF 文件，内容按如下定义：
Manifest-Version: 1.0
Premain-Class: agent.pre_MyProgram
Can-Redefine-Classes: true
要特别注意，一共是四行，第四行是空行，还有就是冒号后面的一个空格，如下截图： 
![](https://images2018.cnblogs.com/blog/137084/201804/137084-20180411191012939-365841553.png)
然后我们打包代码为 pre_MyProgram.jar
注意打包的时候选择我们自己定义的 MANIFEST.MF ,这是导出步骤：
(1)
![](https://images2018.cnblogs.com/blog/137084/201804/137084-20180411191159076-1396805896.png)
（2） 注意选择pre的MF文件
![](https://images2018.cnblogs.com/blog/137084/201804/137084-20180411191120407-665786262.png)
接着我们在创建一个带有main方法的主程序工程，截图如下： 
![](https://images2018.cnblogs.com/blog/137084/201804/137084-20180411191332322-175594761.png)
这时候别忘了：
main函数也有MF文件：别写错了，不然导出报错：No main manifest attribute(说明MF文件写错了)
Manifest-Version: 1.0
Main-Class: alibaba.MyProgram
![](https://images2018.cnblogs.com/blog/137084/201804/137084-20180411191408822-1266099180.png)
按同样的方法导出main的jar包命名为：MyProgram.jar
 如下：
![](https://images2018.cnblogs.com/blog/137084/201804/137084-20180411191451187-900021035.png)
选择它的MF文件：
![](https://images2018.cnblogs.com/blog/137084/201804/137084-20180411191537061-1329451524.png)
如何执行 MyProgram.jar ？我们通过 -javaagent 参数来指定我们的Java代理包，值得一说的是 -javaagent 这个参数的个数是不限的，如果指定了多个，则会按指定的先后执行，执行完各个 agent 后，才会执行主程序的 main 方法。
命令如下：
C:\WINDOWS\system32>java -javaagent:C:\Users\z003fe9c\Desktop\tessdata\agent\pre
_MyProgram.jar=Hello1 -javaagent:C:\Users\z003fe9c\Desktop\tessdata\agent\pre_My
Program.jar=Hello2 -jar C:\Users\z003fe9c\Desktop\tessdata\agent\MyProgram.jar
输出结果： 
====premain 方法执行
Hello1
====premain 方法执行
Hello2
=========main方法执行====
特别提醒：
（1）如果你把 -javaagent 放在 -jar 后面，则不会生效。也就是说，放在主程序后面的 agent 是无效的。
比如执行：
java -javaagent:G:\myagent.jar=Hello1 -javaagent:G:\myagent.jar=Hello2 -jar myapp.jar -javaagent:G:\myagent.jar=Hello3
(2)如果main函数忘了选择MF文件或是MF文件选择的不对，就会报错：
只会有前个生效，第三个是无效的。  
命令中的Hello1为我们传递给 premain 方法的字符串参数。
至此，我们会使用 javaagent 了，但是单单看这样运行的效果，好像没有什么实际意义嘛。
我们可以用 javaagent 做什么呢？下篇文章我们来介绍如何在项目中应用 javaagent。
最后说一下，还有一种，在main方法执行后再执行代理的方法，因为不常用，而且主程序需要配置 Agent-Class，所以不常用，如果需要自行了解下 agentmain(String agentArgs, Instrumentation inst) 方法。
第二篇：
从此处开始，到最后，是我直接复制了其他人员的，因为我自己的一直没有调试出来，不过思路清楚了：
第二篇可以直接看别人的 [JavaAgent 应用（spring-loaded 热部署）](https://blog.csdn.net/catoop/article/details/51034778)，以下的可以忽略掉：
上一篇文章简单介绍了 javaagent ，想了解的可以移步 “[JavaAgent](http://blog.csdn.net/catoop/article/details/51034739%20JavaAgent)”
本文重点说一下，JavaAgent 能给我们带来什么？
- 自己实现一个 JavaAgent xxxxxx
- 基于 JavaAgent 的 spring-loaded 实现 jar 包的热更新，也就是在不重启服务器的情况下，使我们某个更新的 jar 被重新加载。
一、基于 JavaAgent 的应用实例
JDK5中只能通过命令行参数在启动JVM时指定javaagent参数来设置代理类，而JDK6中已经不仅限于在启动JVM时通过配置参数来设置代理类，JDK6中通过 Java Tool API 中的 attach 方式，我们也可以很方便地在运行过程中动态地设置加载代理类，以达到 instrumentation 的目的。 
Instrumentation 的最大作用，就是类定义动态改变和操作。
最简单的一个例子，计算某个方法执行需要的时间，不修改源代码的方式，使用Instrumentation 代理来实现这个功能，给力的说，这种方式相当于在JVM级别做了AOP支持，这样我们可以在不修改应用程序的基础上就做到了AOP，是不是显得略吊。
- 创建一个 ClassFileTransformer 接口的实现类 MyTransformer 
	实现 ClassFileTransformer 这个接口的目的就是在class被装载到JVM之前将class字节码转换掉，从而达到动态注入代码的目的。那么首先要了解MonitorTransformer 这个类的目的，就是对想要修改的类做一次转换，这个用到了javassist对字节码进行修改，可以暂时不用关心jaavssist的原理，用ASM同样可以修改字节码，只不过比较麻烦些。
接着上一篇文章的2个工程，分别添加下面的类。 
MyTransformer.java 添加到 MyAgent 工程中。
![复制代码](https://common.cnblogs.com/images/copycode.gif)
package com.shanhy.demo.agent;
import java.lang.instrument.ClassFileTransformer;
import java.lang.instrument.IllegalClassFormatException;
import java.security.ProtectionDomain;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import javassist.ClassPool;
import javassist.CtClass;
import javassist.CtMethod;
import javassist.CtNewMethod;
/**
 * 检测方法的执行时间
 *
 * @author 单红宇(365384722)
 * @myblog http://blog.csdn.net/catoop/
 * @create 2016年3月30日
 */
public class MyTransformer implements ClassFileTransformer {
    final static String prefix = "\nlong startTime = System.currentTimeMillis();\n";
    final static String postfix = "\nlong endTime = System.currentTimeMillis();\n";
    // 被处理的方法列表
    final static Map<String, List<String>> methodMap = new HashMap<String, List<String>>();
    public MyTransformer() {
        add("com.shanhy.demo.TimeTest.sayHello");
        add("com.shanhy.demo.TimeTest.sayHello2");
    }
    private void add(String methodString) {
        String className = methodString.substring(0, methodString.lastIndexOf("."));
        String methodName = methodString.substring(methodString.lastIndexOf(".") + 1);
        List<String> list = methodMap.get(className);
        if (list == null) {
            list = new ArrayList<String>();
            methodMap.put(className, list);
        }
        list.add(methodName);
    }
    @Override
    public byte[] transform(ClassLoader loader, String className, Class<?> classBeingRedefined,
            ProtectionDomain protectionDomain, byte[] classfileBuffer) throws IllegalClassFormatException {
        className = className.replace("/", ".");
        if (methodMap.containsKey(className)) {// 判断加载的class的包路径是不是需要监控的类
            CtClass ctclass = null;
            try {
                ctclass = ClassPool.getDefault().get(className);// 使用全称,用于取得字节码类<使用javassist>
                for (String methodName : methodMap.get(className)) {
                    String outputStr = "\nSystem.out.println(\"this method " + methodName
                            + " cost:\" +(endTime - startTime) +\"ms.\");";
                    CtMethod ctmethod = ctclass.getDeclaredMethod(methodName);// 得到这方法实例
                    String newMethodName = methodName + "$old";// 新定义一个方法叫做比如sayHello$old
                    ctmethod.setName(newMethodName);// 将原来的方法名字修改
                    // 创建新的方法，复制原来的方法，名字为原来的名字
                    CtMethod newMethod = CtNewMethod.copy(ctmethod, methodName, ctclass, null);
                    // 构建新的方法体
                    StringBuilder bodyStr = new StringBuilder();
                    bodyStr.append("{");
                    bodyStr.append(prefix);
                    bodyStr.append(newMethodName + "($$);\n");// 调用原有代码，类似于method();($$)表示所有的参数
                    bodyStr.append(postfix);
                    bodyStr.append(outputStr);
                    bodyStr.append("}");
                    newMethod.setBody(bodyStr.toString());// 替换新方法
                    ctclass.addMethod(newMethod);// 增加新方法
                }
                return ctclass.toBytecode();
            } catch (Exception e) {
                System.out.println(e.getMessage());
                e.printStackTrace();
            }
        }
        return null;
    }
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
TimeTest.java 添加到 MyProgram 工程中。
![复制代码](https://common.cnblogs.com/images/copycode.gif)
package com.shanhy.demo;
/**
 * 被测试类
 *
 * @author   单红宇(365384722)
 * @myblog  http://blog.csdn.net/catoop/
 * @create    2016年3月30日
 */
public class TimeTest {
    public static void main(String[] args) {
        sayHello();
        sayHello2("hello world222222222");
    }
    public static void sayHello() {
        try {
            Thread.sleep(2000);
            System.out.println("hello world!!");
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
    public static void sayHello2(String hello) {
        try {
            Thread.sleep(1000);
            System.out.println(hello);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
修改MyAgent.java 的 permain 方法，如下：
![复制代码](https://common.cnblogs.com/images/copycode.gif)
 public static void premain(String agentOps, Instrumentation inst) {
        System.out.println("=========premain方法执行========");
        System.out.println(agentOps);
        // 添加Transformer
        inst.addTransformer(new MyTransformer());
    }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
修改MANIFEST.MF内容，增加 Boot-Class-Path 如下：
Manifest-Version: 1.0
Premain-Class: com.shanhy.demo.agent.MyAgent
Can-Redefine-Classes: true
Boot-Class-Path: javassist-3.18.1-GA.jar
对2个工程分别打包为 myagent.jar 和 myapp.jar 然后将 javassist-3.18.1-GA.jar 和 myagent.jar 放在一起。
最后执行命令测试，结果如下：
![复制代码](https://common.cnblogs.com/images/copycode.gif)
G:\>java -javaagent:G:\myagent.jar=Hello1 -jar myapp.jar
=========premain方法执行========
Hello1
hello world!!
this method sayHello cost:2000ms.
hello world222222222
this method sayHello2 cost:1000ms.
![复制代码](https://common.cnblogs.com/images/copycode.gif)
二、使用 spring-loaded 实现 jar 包热部署
在项目开发中我们可以把一些重要但又可能会变更的逻辑封装到某个 logic.jar 中，当我们需要随时更新实现逻辑的时候，可以在不重启服务的情况下让修改后的 logic.jar 被重新加载生效。
spring-loaded是一个开源项目，项目地址:[https://github.com/spring-projects/spring-loaded](https://github.com/spring-projects/spring-loaded)
使用方法：
`在启动主程序之前指定参数`
在启动主程序之前指定参数
-javaagent:C:/springloaded-1.2.5.RELEASE.jar -noverify
如果你想让 Tomat 下面的应用自动热部署，只需要在 catalina.sh 中添加：
set JAVA_OPTS=-javaagent:springloaded-1.2.5.RELEASE.jar -noverify
这样就完成了 spring-loaded 的安装，它能够自动检测Tomcat 下部署的webapps ，在不重启Tomcat的情况下，实现应用的热部署。
通过使用 -noverify 参数，关闭 Java 字节码的校验功能。 
使用参数 -Dspringloaded=verbose;explain;watchJars=tools.jar 指定监视的jar （verbose;explain; 非必须），多个jar用“冒号”分隔，如 watchJars=tools.jar:utils.jar:commons.jar
当然，它也有一些小缺限： 
1. 目前官方提供的1.2.4 版本在linux上可以很好的运行，但在windows还存在bug，官网已经有人提出：[https://github.com/spring-projects/spring-loaded/issues/145](https://github.com/spring-projects/spring-loaded/issues/145)
2. 对于一些第三方框架的注解的修改，不能自动加载，比如：spring mvc的@RequestMapping 
3. log4j的配置文件的修改不能即时生效。
参考：[利用JAVA探针分析复杂代码运维实践](http://baijiahao.baidu.com/s?id=1590013867745948773&wfr=spider&for=pc)
参考：[JavaAgent 简单例子](https://blog.csdn.net/catoop/article/details/51034739)
参考： [JavaAgent 应用（spring-loaded 热部署）](https://blog.csdn.net/catoop/article/details/51034778)
