# 如何创建一个双击可以运行的Jar文件 - z69183787的专栏 - CSDN博客
2014年05月15日 13:53:06[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1731
其实jar就是一个zip压缩，根目录就是默认的classpath。所以运行一个jar除了把对应的class文件按照包格式打包进来，还需要通知JVM需要运行的主类（也就是main在哪）
jar是用一个清单文件(manifest.mf)来管理jar包中的视图结构，主类的声明也在里面。
首先创建一个HelloWorld.java文件
package test;
public class HelloWorld {
public static void main(String [] args) {
System.out.println("HelloWorld");
}
}
然后编译为HelloWorld.class
>javac test/HelloWorld.java
然后在创建manifest.mf文件，内容为
Main-Class: test.HelloWorld
然后用自定义manifest.mf创建jar包
jar cvfm HelloWorld.jar manifest.mf -C test .
之后就可以运行jar包
java -jar HelloWorld.jar
一个可以运行的jar包就完成了。但是我们在Explore里面双击jar，惊奇的发现居然没有什么反应，为什么呢？
我们通过打开方式得到双击jar对应的操作是javaw -jar %s %1
运行的是javaw，而不是我们学习的java。原来javaw是为方便执行GUI而实现的java版本，为了避免GUI程序运行期还有个丑丑的控制台，javaw在启动vm后，就关闭了了控制台。所以如果想实现双击的效果，用swing写个GUI的例子吧。
