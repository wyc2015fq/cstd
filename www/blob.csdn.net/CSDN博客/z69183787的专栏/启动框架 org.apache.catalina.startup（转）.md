# 启动框架 org.apache.catalina.startup（转） - z69183787的专栏 - CSDN博客
2014年02月26日 09:57:09[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3029
 org.apache.catalina.startup.Bootstrap
第一步自然从main函数开始，我们可以从中看出bootstrap的初始化过程
main函数
首先是调用init方法 ，然后根据main的参数，比如start、startd、stop、stopd等等，启动或停止服务器
ps：start和startd的区别，感觉像是前者用于重新启动
init方法
设置系统属性：setCatalinaHome和setCatalinaBase，都是利用System.setProperty方法来实现。user.dir属性指向一个目录，catalina.home属性指向user.dir目录下的bootstrap.jar文件，该文件不存在则指向user.dir本身。catalina.base和catalina.home一样
初始化classloader
创建了三个classloader：common,server,shared ，以server为例，创建过程是：
查找server.loader系统变量，其中包含了以”,“分隔的文件路径（可能是URL、JAR、目录），称为repository。对repository做一些处理后（主要是替换其中的其他环境变量名，以得到完整的绝对路径），利用同一个包里面的另一个类ClassLoaderFactory，来真正产生classloader 。最后将生成的classloader作为mbean保存
Thread.currentThread().setContextClassLoader(catalinaLoader) 把生成的catalinaLoader作为当前线程上下文的classloader
利用反射机制，正式载入org.apache.catalina.startup.Catalina类，创建类的实例，并调用该类的setParentClassLoader方法。最后，将该实例赋值给Bootstrap类的catalinaDaemon对象（这个对象类型是Object），完成初始化
回到main函数
假设命令参数为start，则依次调用bootstrap的setAwait、load、start方法，这三个方法原理都一样，都是通过反射，实际调用刚才初始化生成的catalinaDaemon的同名方法
例：start方法中的反射代码
Method method = catalinaDaemon.getClass().getMethod("start", (Class [] )null);
method.invoke(catalinaDaemon, (Object [])null);
Bootstrap类剩下的几个方法：stop、stopServer，都是和start、load一样，通过反射调用catalinaDaemon的同名方法（其实就是org.apache.catalina.startup.Catalina类）
总结：
1.反射的大量使用
无论是classloader，还是具体的某个类，都是通过反射实现初始化和方法调用的。思路是：配置文件->系统属性->解析属性字符串->得到类路径->反射，得到类->方法调用
这样可以使服务器的实现类很容易被替换，降低耦合性，减少将来代码的改动，的确是一种优秀的设计模式思想
启动类的配置文件比较简单，都是简单的“键-值”，所以直接用了StringToken解析字符串。后面的代码牵涉到xml配置文件时，必定会换成xml解析器。但总体思路应该类似
2.JMX的使用
Bootstrap的JMX用得很少，仅仅对类装载器进行mbean注册，相信后面会有更多JMX的身影
org.apache.catalina.startup.ClassLoaderFactory
顾名思义，是tomcat专门用来产生classloader的工厂类
包括三个重载方法createClassLoader，内容都大同小异，无非是根据传入的类路径参数，产生一个相应的类装载器
tomcat的类路径分为：包含class文件的目录、jar文件、包含jar的目录和URL
以Bootstrap类用到的createClassLoader方法为例
参数为：(String locations[],Integer types[], ClassLoader parent)
locations即为类路径的字符串表示，types为对应的类路径的类型，这两个数组的长度肯定是相同的，parant即为父classloader
方法的处理逻辑很简单，即根据每个location的类型，对location字符串做处理，如取得绝对路径、遍历目录、判断是不是jar文件等等
从代码可以看到，ClassLoaderFactory产生的classloader其实都是org.apache.catalina.loader.StandardClassLoader，这个类装载器留待以后研究
org.apache.catalina.startup.Catalina
Bootstrap中，是通过start方法启动Catalina的
Catalina中包含一个server实例，对应server.xml中的server元素。如果server实现了Lifecycle，start首先将server实例作为生命周期对象进行启动。然后，在JDK的Runtime中注册一个Shutdownhook。最后，如果指定了await参数，则让Catalina停止
start方法代码如下：
public void start() {
        if (server == null) {
            load();
        }
        long t1 = System.nanoTime();
        // Start the new server
        if (server instanceof Lifecycle) {
            try {
                ((Lifecycle) server).start();
            } catch (LifecycleException e) {
                log.error("Catalina.start: ", e);
            }
        }
        long t2 = System.nanoTime();
        if(log.isInfoEnabled())
            log.info("Server startup in " + ((t2 - t1) / 1000000) + " ms");
        try {
            // Register shutdown hook
            if (useShutdownHook) {
                if (shutdownHook == null) {
                    shutdownHook = new CatalinaShutdownHook();
                }
                Runtime.getRuntime().addShutdownHook(shutdownHook);
            }
        } catch (Throwable t) {
            // This will fail on JDK 1.2. Ignoring, as Tomcat can run
            // fine without the shutdown hook.
        }
        if (await) {
            await();
            stop();
        }
    }
Shutdownhook是一个Thread对象，用于给JVM在收到停止命令时（例如Ctrl C），运行这些Thread执行收尾工作，具体用法可以参考JAVA文档
CatalinaShutdownHook在这里就是执行Catalina.stop方法，所以在Catalina.stop方法里面，会手动删除Runtime中的CatalinaShutdownHook，以防止调用两次stop，如下：
// Remove the ShutdownHook first so that server.stop()
            // doesn't get invoked twice
            if (useShutdownHook) {
                Runtime.getRuntime().removeShutdownHook(shutdownHook);
            }
Catalina也有个main函数，不过里头调用的方法都是已废弃的，估计是遗留代码
main函数主要是对命令行参数进行解析，比如可以取代默认的server.xml路径，其余工作主要交给load和start方法
load方法
按以下顺序读取配置文件server.xml，通过Digester类进行解析
1.configFile字段指明的配置文件“conf/server.xml”
2.Catalina的类装载器的getResource方法（具体原理不清楚，似乎很神奇。和装载类的机制一样，propagate方式，父装载器优先）
3.最后是从内置的catalina.jar的server-embed.xml读入配置
一旦读取成功，则后面的不读取，优先级依次降低
load方法最后进行配置和server的初始化，调用server.initialize方法
其中，Digester类似乎很神奇很强大，可以根据一定的规则，对一个xml源文件进行解析，然后执行相应的动作，留待以后研究。这里有一篇文章可以参考
[http://software.ccidnet.com/pub/article/c322_a31671_p2.html](http://software.ccidnet.com/pub/article/c322_a31671_p2.html)
stop方法
// Shut down the server
        if (server instanceof Lifecycle) {
            try {
               ((Lifecycle) server).stop();
            } catch (LifecycleException e) {
                log.error("Catalina.stop", e);
            }
        }
stopserver方法
比起stop方法要复杂得多，不是简单地调用server.stop，而是通过创建一个Digester，根据配置文件的定义进行动作；然后，以非常“原始”的方法：建立到server的socket连接，以字节流的方式，一个个字符地发送tomcat的“SHUTDOWN”指令，如下：
// Stop the existing server
       try {
           String hostAddress = InetAddress.getByName("localhost").getHostAddress();
           Socket socket = new Socket(hostAddress, server.getPort());
           OutputStream stream = socket.getOutputStream();
           String shutdown = server.getShutdown();
           for (int i = 0; i < shutdown.length(); i++)
               stream.write(shutdown.charAt(i));
           stream.flush();
           stream.close();
           socket.close();
       } catch (IOException e) {
           log.error("Catalina.stop: ", e);
           System.exit(1);
       }
补充
在看完StandardServer后，回到Catalina，现在对于Catalina的stop和stopServer方法的区别又多了一些理解。
关于StandardServer的笔记：
Catalina的stop方法，其实是调用StandardServer.stop，进而停止StandardServer内部的各个Service，是名副其实的真正“停止”服务器
而stopServer方法，其实是构造一个socket连接到StandardServer的await方法打开的端口上，并发送SHUTDOWN命令。如前面所述，await只是起到阻塞作用，接受到SHUTDOWN命令后则await结束阻塞，并返回。具体要做哪些操作，还得由调用await的代码来决定。
现在先姑且认为，Catalina的stop方法是单纯的、Catalinal自己定义的一个停止服务器的函数，里面的实现都是封装在Catalina内部的，完成的工作就是停止服务器的socket和相关的Service服务，外人无法改动。
假如有第三方希望在停止server之前做点别的事情（比如释放特定的资源、或者通知其他组件），那么可以利用stopServer方法，待到await返回后，先完成自己要做的事情，最后再回头调用Catalina.stop()，完成最终的“停止” 。
通过这样的设计，tomcat给了外部代码，或者说基于tomcat的二次开发人员很大的灵活性，不必拘泥于tomcat本身的实现，也有利于tomcat自身的功能扩展。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/wangchengsi/archive/2008/09/20/2966850.aspx](http://blog.csdn.net/wangchengsi/archive/2008/09/20/2966850.aspx)
