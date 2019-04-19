# 实现自定义类加载器-ClassLoader - 零度的博客专栏 - CSDN博客
2015年08月07日 17:16:03[零度anngle](https://me.csdn.net/zmx729618)阅读数：1306
JAVA启动后，是经过JVM各级ClassLoader来加载各个类到内存。为了更加了解加载过程，我通过分析和写了一个简单的ClassLoader来粗浅的分析它的原理。
JVM的ClassLoader分三层，分别为Bootstrap ClassLoader，Extension ClassLoader，System ClassLoader，他们不是类继承的父子关系，是逻辑上的上下级关系。
Bootstrap ClassLoader是启动类加载器，它是用C++编写的，从%jre%/lib目录中加载类，或者运行时用-Xbootclasspath指定目录来加载。
Extension ClassLoader是扩展类加载器，从%jre%/lib/ext目录加载类，或者运行时用-Djava.ext.dirs制定目录来加载。
System ClassLoader，系统类加载器，它会从系统环境变量配置的classpath来查找路径，环境变量里的.表示当前目录，是通过运行时-classpath或-Djava.class.path指定的目录来加载类。
一般自定义的Class Loader可以从java.lang.ClassLoader继承，不同classloader加载相同的类，他们在内存也不是相等的，即它们不能互相转换，会直接抛异常。java.lang.ClassLoader的核心加载方法是loadClass方法，如：
**protectedsynchronized** Class<?> loadClass(String name,
**boolean** resolve)
**throws** ClassNotFoundException
    {
// First, check if the class has already been loaded
         Class c = findLoadedClass(name);
**if** (c ==
**null**) {
**try** {
**if** (parent
 != **null**) {
                       c =
parent.loadClass(name,
**false**);
                   }
**else** {
                       c = findBootstrapClass0(name);
                   }
             }
**catch** (ClassNotFoundException e) {
// If still not found, then invoke findClass in order
// to find the class.
                 c = findClass(name);
             }
         }
**if** (resolve) {
             resolveClass(c);
         }
**return** c;
    }
通过上面加载过程，我们能知道JVM默认是双亲委托加载机制，即首先判断缓存是否有已加载的类，如果缓存没有，但存在父加载器，则让父加载器加载，如果不存在父加载器，则让Bootstrap ClassLoader去加载，如果父类加载失败，则调用本地的findClass方法去加载。（注：如果不想破坏双亲委托机制的类加载方式，自定义的classloader应该重写findClass(name)，而不是直接重写loadClass）
可以通过下面三条语句，输入现在加载的各个classloader的加载路径：
        System.*out*.println("sun.boot.class.path:"
 + System.*getProperty*("sun.boot.class.path"));   
        System.*out*.println("java.ext.dirs:"
 + System.*getProperty*("java.ext.dirs"));   
        System.*out*.println("java.class.path:"
 +System.*getProperty*("java.class.path"));
        ClassLoader cl = Thread.*currentThread*().getContextClassLoader();//ClassLoader.getSystemClassLoader()
        System.*out*.println("getContextClassLoader:"
 +cl.toString()); 
        System.*out*.println("getContextClassLoader.parent:"
 +cl.getParent().toString()); 
        System.*out*.println("getContextClassLoader.parent2:"
 +cl.getParent().getParent());
输出结果为：
sun.boot.class.path:C:\Program Files\Java\jre7\lib\resources.jar;C:\Program Files\Java\jre7\lib\rt.jar;C:\Program Files\Java\jre7\lib\sunrsasign.jar;C:\Program Files\Java\jre7\lib\jsse.jar;C:\Program
 Files\Java\jre7\lib\jce.jar;C:\Program Files\Java\jre7\lib\charsets.jar;C:\Program Files\Java\jre7\classes
java.ext.dirs:C:\Program Files\Java\jre7\lib\ext;C:\Windows\Sun\Java\lib\ext
java.class.path:E:\MyProjects\workspace\TestConsole\bin
getContextClassLoader:sun.misc.Launcher$AppClassLoader@19dbc3b
getContextClassLoader.parent:sun.misc.Launcher$ExtClassLoader@b103dd
getContextClassLoader.parent2:null
从上面的运行结果可以看出逻辑上的层级继承关系。双亲委托机制的作用是防止系统jar包被本地替换，因为查找方法过程都是从最底层开始查找。因此，一般我们自定义的classloader都需要采用这种机制，我们只需要继承java.lang.ClassLoader实现findclass即可，如果需要更多控制，自定义的classloader就需要重写loadClass方法了，比如tomcat的加载过程，这个比较复杂，可以通过其他文档资料查看相关介绍。
各个ClassLoader加载相同的类后，他们是不互等的，这个当涉及多个ClassLoader，并且有通过当前线程上线文获取ClassLoader后转换特别需要注意，可以通过线程的setContextClassLoader设置一个ClassLoader线程上下文，然后再通过Thread.currentThread().getContextClassLoader()获取当前线程保存的Classloader。但是自定义的类文件，放到Bootstrap
 ClassLoader加载目录，是不会被Bootstrap ClassLoader加载的，因为作为启动类加载器，它不会加载自己不熟悉的jar包的，并且类文件必须打包成jar包放到加载器加载的根目录，才可能被扩展类加载器所加载。
下面我自定义一个简单的classloader：
**publicclass** TestClassLoader
**extends** ClassLoader {
//定义文件所在目录
**privatestaticfinal** String
*DEAFAULTDIR*="E:\\MyProjects\\workspace\\TestConsole\\bin\\";
**public** Class<?> findClass(String name)
**throws** ClassNotFoundException {
**byte**[] b =
**null**;  
**try** {  
            b = loadClassData(*GetClassName*(name));  
        }
**catch** (Exception e) {  
            e.printStackTrace();  
        } 
**return** defineClass(name, b, 0, b.length);    
    }   
@Override
**protectedsynchronized** Class<?> loadClass(String name,
**boolean** resolve)
**throws** ClassNotFoundException {
**if**(name.startsWith("java.")){**try**
 {
**returnsuper**.loadClass(name,
**false**);    
        }
**catch** (ClassNotFoundException e) {
            e.printStackTrace();  
        }
        }
**byte**[] b =
**null**;  
**try** {  
           b = loadClassData(*GetClassName*(name));  
       }
**catch** (Exception e) {  
           e.printStackTrace();  
       } 
**return** defineClass(name, b, 0, b.length);  
    }  
**privatebyte**[] loadClassData(String filepath)
**throws** Exception {  
**int** n =0;  
        BufferedInputStream br =
**new** BufferedInputStream(  
**new** FileInputStream(  
**new** File(filepath)));  
        ByteArrayOutputStream bos=
**new** ByteArrayOutputStream();  
**while**((n=br.read())!=-1){  
                bos.write(n);  
            }  
            br.close();  
**return** bos.toByteArray();  
    }
**publicstatic** String GetClassName(String name){
**return***DEAFAULTDIR*+name.replace('.','/')+".class";
    }
}
这个自定义的ClassLoader重写了loadclass方法，但不用默认的双亲委托，比如java.lang包下面的都无法解析，这里我简单的判断如果是java.开始的包则用父类去解析，能简单的满足双亲委托机制，但是其他相关非系统类加载也没有用父类加载了。
测试代码如：
        TestClassLoader liuloader =
**new** TestClassLoader();
        Myrunner
runner = **new** Myrunner();
runner.setContextClassLoader(liuloader);
runner.start();
Myrunner是我自定义继承自Thread的线程，通过设置线程上下文的classloader后，线程内部测试代码如：
        ClassLoader cl1 = Thread.*currentThread*().getContextClassLoader();
        System.*out*.println(cl1);
它将会输出：
[com.liu.ClassLoader.TestClassLoader@347cdb](mailto:com.liu.ClassLoader.TestClassLoader@347cdb)，说明已经为当前线程上下文设置了自定义的Classloader了，如果这个线程内部通过这个classloader加载一个类，再转换成当前的类，如代码：
Class c = cl1.loadClass("com.liu.ClassLoader.TestLoader2");    TestLoader2 tloader = (TestLoader2)c.newInstance();
则为抛java.lang.ClassCastException异常:com.liu.ClassLoader.TestLoader2 cannot be cast to com.liu.ClassLoader.TestLoader2。
因为cl1当前是
 TestClassLoader加载的，而这个TestLoader2的类还是默认由AppClassLoader加载，因此它们不能隐式转换，Classloader加载相同的类，内存认为它们是没有关系的对象。
如果把我自定义的TestClassLoader里的LoadClass方法去掉，则采用了双亲委托机制，这样我们除了指定的类以外，其他都会优先用父类来加载。这样可以解决刚才的java.lang.ClassCastException异常问题，为加载的对象建立一个抽象父类，自定义的Classloader负责加载子类，父类统一交给AppClassLoader或父加载器来加载，这样线程内部可以使用类试：
Class c = cl1.loadClass("com.liu.ClassLoader.TestLoader2");
        BaseTest tloader = (BaseTest)c.newInstance();
BaseTest是TestLoader2的父类，因为BaseTest都是AppClassLoader或父加载器加载的，因此可以达到成功隐式转换的目的。
对于Tomcat等几个处理的Classloader都是自定义并重写了loadclass方法，内部会更复杂处理。
