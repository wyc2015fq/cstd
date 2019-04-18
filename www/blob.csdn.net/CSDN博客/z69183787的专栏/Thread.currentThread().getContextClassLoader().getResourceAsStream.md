# Thread.currentThread().getContextClassLoader().getResourceAsStream - z69183787的专栏 - CSDN博客
2014年04月02日 06:49:47[OkidoGreen](https://me.csdn.net/z69183787)阅读数：29538
个人分类：[资源文件读取-Java																[Java-知识](https://blog.csdn.net/z69183787/article/category/2175279)](https://blog.csdn.net/z69183787/article/category/2175913)
Java路径
　　Java中使用的路径，分为两种：绝对路径和相对路径。具体而言，又分为四种：
　　一、URI形式的绝对资源路径
　　如：file:/D:/java/eclipse32/workspace/jbpmtest3/bin/aaa.b
　　URL是URI的特例。URL的前缀/协议，必须是Java熟悉的。URL可以打开资源，而URI则不行。
　　URL和URI对象可以互相转换，使用各自的toURI(),toURL()方法即可！
　　二、本地系统的绝对路径
　　D:/java/eclipse32/workspace/jbpmtest3/bin/aaa.b
　　Java.io包中的类，需要使用这种形式的参数。
　　但是，它们一般也提供了URI类型的参数，而URI类型的参数，接受的是URI样式的String。因此，通过URI转换，还是可以把URI样式的绝对路径用在java.io包中的类中。
　　三、相对于classpath的相对路径
　　如：相对于
　　file:/D:/java/eclipse32/workspace/jbpmtest3/bin/这个路径的相对路径。其中，bin是本项目的classpath。所有的Java源文件编译后的.class文件复制到这个目录中。
　　四、相对于当前用户目录的相对路径
　　就是相对于System.getProperty("user.dir")返回的路径。
　　对于一般项目，这是项目的根路径。对于JavaEE服务器，这可能是服务器的某个路径。这个并没有统一的规范！
　　所以，绝对不要使用"相对于当前用户目录的相对路径"。然而：
　　默认情况下，java.io 包中的类总是根据当前用户目录来分析相对路径名。此目录由系统属性 user.dir 指定，通常是 Java 虚拟机的调用目录。
　　这就是说，在使用java.io包中的类时，最好不要使用相对路径。否则，虽然在J2SE应用程序中可能还算正常，但是到了J2EE程序中，一定会出问题！而且这个路径，在不同的服务器中都是不同的！
　　相对路径最佳实践
　　推荐使用相对于当前classpath的相对路径
　　因此，我们在使用相对路径时，应当使用相对于当前classpath的相对路径。
　　ClassLoader类的getResource(String name),getResourceAsStream(String name)等方法，使用相对于当前项目的classpath的相对路径来查找资源。
　　读取属性文件常用到的ResourceBundle类的getBundle(String path)也是如此。
　　通过查看ClassLoader类及其相关类的源代码，我发现，它实际上还是使用了URI形式的绝对路径。通过得到当前classpath的 URI形式的绝对路径，构建了相对路径的URI形式的绝对路径。（这个实际上是猜想，因为JDK内部调用了SUN的源代码，而这些代码不属于JDK，不是开源的。） 相对路径本质上还是绝对路径
　　因此，归根结底，Java本质上只能使用绝对路径来寻找资源。所有的相对路径寻找资源的方法，都不过是一些便利方法。不过是API在底层帮助我们构建了绝对路径，从而找到资源的！
　　得到classpath和当前类的绝对路径的一些方法
　　下面是一些得到classpath和当前类的绝对路径的一些方法。你可能需要使用其中的一些方法来得到你需要的资源的绝对路径。
　　1.FileTest.class.getResource("")
　　得到的是当前类FileTest.class文件的URI目录。不包括自己！
　　如：file:/D:/java/eclipse32/workspace/jbpmtest3/bin/com/test/
　　2.FileTest.class.getResource("/")
　　得到的是当前的classpath的绝对URI路径。
　　如：file:/D:/java/eclipse32/workspace/jbpmtest3/bin/
　　3.Thread.currentThread().getContextClassLoader().getResource("")
　　得到的也是当前ClassPath的绝对URI路径。
　　如：file:/D:/java/eclipse32/workspace/jbpmtest3/bin/
　　4.FileTest.class.getClassLoader().getResource("")
　　得到的也是当前ClassPath的绝对URI路径。
　　如：file:/D:/java/eclipse32/workspace/jbpmtest3/bin/
　　5.ClassLoader.getSystemResource("")
　　得到的也是当前ClassPath的绝对URI路径。
　　如：file:/D:/java/eclipse32/workspace/jbpmtest3/bin/
　　我推荐使用Thread.currentThread().getContextClassLoader().getResource("")来得到当前的classpath的绝对路径的URI表示法。
　　Web应用程序中资源的寻址
　　上文中说过，当前用户目录，即相对于System.getProperty("user.dir")返回的路径。
　　对于JavaEE服务器，这可能是服务器的某个路径，这个并没有统一的规范！
　　而不是我们发布的Web应用程序的根目录！
　　这样，在Web应用程序中，我们绝对不能使用相对于当前用户目录的相对路径。
　　在Web应用程序中，我们一般通过ServletContext.getRealPath("/")方法得到Web应用程序的根目录的绝对路径。
　　这样，我们只需要提供相对于Web应用程序根目录的路径，就可以构建出定位资源的绝对路径。
　　这是我们开发Web应用程序时一般所采取的策略。
推荐使用Thread.currentThread().getContextClassLoader().getResource("")来得到当前的classpath的绝对路径的URI表示法。
Application可以通过new FileInputStream("xx.properties");直接在classes一级获取。关键是有时我们需要通过web修改配置文件，我们不 能将路径写死了。经过测试觉得有以下心得：
1.servlet中读写。如果运用Struts 或者Servlet可以直接在初始化参数中配置，调用时根据servlet的getRealPath("/")获取真实路径，再根据String file = this.servlet.getInitParameter("abc");获取相对的WEB-INF的相对路径。
例：
InputStream input =Thread.currentThread().getContextClassLoader().getResourceAsStream("abc.properties");
Properties prop = new Properties();
prop.load(input);
input.close();
prop.setProperty("abc", “test");
prop.store(new FileOutputStream(path), “–test–");
out.close();
2.直接在jsp中操作，通过jsp内置对象获取可操作的绝对地址。
例：
// jsp页面
String path = pageContext.getServletContext().getRealPath("/");
String realPath = path+"/WEB-INF/classes/abc.properties";
//java 程序
InputStream in = getClass().getClassLoader().getResourceAsStream("abc.properties"); // abc.properties放在webroot/WEB-INF/classes/目录下
prop.load(in);
in.close();
OutputStream out = new FileOutputStream(path); // path为通过页面传入的路径
prop.setProperty("abc", “abcccccc");
prop.store(out, “–test–");
out.close();
3.只通过Java程序操作资源文件
InputStream in = new FileInputStream("abc.properties"); // 相对路径,项目下的路径
OutputStream out = new FileOutputStream("abc.properties");
