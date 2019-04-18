# Java中-classpath和路径的使用 - z69183787的专栏 - CSDN博客
2014年02月25日 16:56:06[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1132
个人分类：[Java-知识](https://blog.csdn.net/z69183787/article/category/2175279)
javac -classpath的使用：
javac：如果当前你要编译的java文件中引用了其它的类(比如说：继承)，但该引用类的.class文件不在当前目录下，这种情况下就需要在javac命令后面加上-classpath参数，通过使用以下三种类型的方法 来指导编译器在编译的时候去指定的路径下查找引用类。
(1).绝对路径：javac -classpath c:/junit3.8.1/junit.jar   Xxx.java
(2).相对路径：javac -classpath ../junit3.8.1/Junit.javr  Xxx.java
(3).系统变量：javac -classpath %CLASSPATH% Xxx.java (注意：%CLASSPATH%表示使用系统变量CLASSPATH的值进行查找，这里假设Junit.jar的路径就包含在CLASSPATH系统变量中)
**javac 绝对路径的使用：**
javac：假设你要编译的类文件名叫：HelloWorld.java，其完全路径为：D:/java/HelloWorld.java。但你所在的当前目录是：C:/Documents and Settings/peng>。如果想在这里执行编译，会有什么结果呢？
(1).C:/Documents and Settings/peng> javac HelloWorld.java 这时编译器会给出如下的错误提示信息：
error: cannot read: HelloWorld.java
这是因为默认情况下javac是在当前目录下查找类文件，很明显这个路径不是我们存放类文件的地方，所以就会报错了
(2).C:/Documents and Settings/peng>javac D:/java/HelloWorld.java 
这时编译成功。
所以，只要你执行javac命令的目录不是类文件存放的目录，你就必须在javac命令中显式地指定类文件的路径。
**java -classpath的使用：**
java：假设我们的CLASSPATH设置为：D:/peng/java/pro ，在该目录下有三个文件：HelloWorld.java / HelloWorldExtendsTestCase / HelloWorldExtendsHelloWorld。这三个文件的类声明分别如下：
HelloWorld.java ：public class HelloWorld 
HelloWorldExtendsHelloWorld.java ：public class HelloWorldExtendsHelloWorld extends HelloWorld
HelloWorldExtendsTestCase.java：public class HelloWorldExtendsTestCase extends junit.framework.TestCase 
      假设我们已经按照上面关于javac -classpath和javac 绝对路径的使用，顺利地完成了三个文件地编译。现在我们在C:/Documents and Settings/peng>目录下执行这三个.class文件
(1).C:/Documents and Settings/peng>java  HelloWorld 
      Hello World
可以看到执行成功。为什么我们在C:/Documents and Settings/peng>执行命令，JVM能够找到D:/peng/java/pro/HelloWorld.class文件呢？这是因为我们配置了系统变量CLASSPATH，并且指向了目录：D:/peng/java/pro
 。所以JVM会默认去该目录下加载类文件，而不需要指定.class文件的绝对路径了。
(2).C:/Documents and Settings/peng>java HelloWorldExtendsHelloWorld
Hello World
可以看到执行成功了。HelloWorldExtendsHelloWorld继承了HelloWorld类，所以在执行时JVM会先查找在CLASSPATH下是否存在一个HelloWorld.class文件，因为我们已经成功编译了HelloWorld 类了，所以可以成功执行HelloWorldExtendsHelloWorld.class
(3).C:/Documents and Settings/peng>java HelloWorldExtendsTestCase
     Exception in thread "main" java.lang.NoClassDefFoundError: junit/framework/TestCase
可以看到程序抛出异常了，提示找不到junit.framework.TestCase文件。为什么同样在:/peng/java/pro 下，HelloWorldExtendsHelloWorld.class就可以成功执行，而这个就不行了呢？这是因为：            junit.framework.TestCase.class文件并不存在于当前目录下，所以为了能够让程序成功运行，我们必须通过指定CLASSPATH的方式，让JVM可以找到junit.framework.TestCase这个类，如(4)：
(4). C:/Documents and Settings/peng>java -classpath %CLASSPATH% HelloWorldExtendsTestCase
      Hello World
总结：
**(1).何时需要使用-classpath：当你要编译或执行的类引用了其它的类，但被引用类的.class文件不在当前目录下时，就需要通过-classpath来引入类(2).何时需要指定路径：当你要编译的类所在的目录和你执行javac命令的目录不是同一个目录时，就需要指定源文件的路径(CLASSPATH是用来指定.class路径的，不是用来指定.java文件的路径的)**
