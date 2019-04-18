# Java Class的热替换 自定义ClassLoader加载.class - z69183787的专栏 - CSDN博客
2014年06月07日 21:08:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6113
本文是java热替换的实验，参考了
# Java 类的热替换 —— 概念、设计与实现[http://www.ibm.com/developerworks/cn/java/j-lo-hotswapcls/index.html](http://www.ibm.com/developerworks/cn/java/j-lo-hotswapcls/index.html)
之前的错误是，没有集成ClassLoader，而是直接使用了
**[java]**[view
 plain](http://blog.csdn.net/ozwarld/article/details/7440868#)[copy](http://blog.csdn.net/ozwarld/article/details/7440868#)
- classloader = (URLClassLoader)ClassLoader.getSystemClassLoader();  
这个语句只适用于加载.jar
如果加载.class的话需要使用自定义的ClassLoader
**[java]**[view
 plain](http://blog.csdn.net/ozwarld/article/details/7440868#)[copy](http://blog.csdn.net/ozwarld/article/details/7440868#)
- package yerasel;  
- 
- import java.io.BufferedReader;  
- import java.io.File;  
- import java.io.FileInputStream;  
- import java.io.FileNotFoundException;  
- import java.io.IOException;  
- import java.io.InputStream;  
- import java.io.InputStreamReader;  
- import java.lang.reflect.Field;  
- import java.lang.reflect.InvocationTargetException;  
- import java.lang.reflect.Method;  
- 
- import java.net.MalformedURLException;  
- import java.net.URL;  
- import java.net.URLClassLoader;  
- import java.util.HashSet;  
- 
- /*
-  *  实现热部署，自定义ClassLoader，加载的是.class
-  */
- class HowswapCL extends ClassLoader {  
- 
- private String basedir; // 需要该类加载器直接加载的类文件的基目录
- private HashSet dynaclazns; // 需要由该类加载器直接加载的类名
- 
- public HowswapCL(String basedir, String[] clazns) {  
- super(null); // 指定父类加载器为 null
- this.basedir = basedir;  
-         dynaclazns = new HashSet();  
-         loadClassByMe(clazns);  
-     }  
- 
- privatevoid loadClassByMe(String[] clazns) {  
- for (int i = 0; i < clazns.length; i++) {  
-             loadDirectly(clazns[i]);  
-             dynaclazns.add(clazns[i]);  
-         }  
-     }  
- 
- private Class loadDirectly(String name) {  
-         Class cls = null;  
-         StringBuffer sb = new StringBuffer(basedir);  
-         String classname = name.replace('.', File.separatorChar) + ".class";  
-         sb.append(File.separator + classname);  
-         File classF = new File(sb.toString());  
- try {  
-             cls = instantiateClass(name, new FileInputStream(classF),  
-                     classF.length());  
-         } catch (FileNotFoundException e) {  
- // TODO Auto-generated catch block
-             e.printStackTrace();  
-         }  
- return cls;  
-     }  
- 
- private Class instantiateClass(String name, InputStream fin, long len) {  
- byte[] raw = newbyte[(int) len];  
- try {  
-             fin.read(raw);  
-             fin.close();  
-         } catch (IOException e) {  
- // TODO Auto-generated catch block
-             e.printStackTrace();  
-         }  
- 
- return defineClass(name, raw, 0, raw.length);  
-     }  
- 
- protected Class loadClass(String name, boolean resolve)  
- throws ClassNotFoundException {  
-         Class cls = null;  
-         cls = findLoadedClass(name);  
- if (!this.dynaclazns.contains(name) && cls == null)  
-             cls = getSystemClassLoader().loadClass(name);  
- if (cls == null)  
- thrownew ClassNotFoundException(name);  
- if (resolve)  
-             resolveClass(cls);  
- return cls;  
-     }  
- }  
- 
- /*
-  * 每隔500ms运行一次，不断加载class
-  */
- class Multirun implements Runnable {  
- publicvoid run() {  
- try {  
- while (true) {  
- // 每次都创建出一个新的类加载器
- // class需要放在自己package名字的文件夹下
-                 String url = System.getProperty("user.dir") + "/lib";// "/lib/yerasel/GetPI.jar";
-                 HowswapCL cl = new HowswapCL(url,  
- new String[] { "yerasel.GetPI" });  
-                 Class cls = cl.loadClass("yerasel.GetPI");  
-                 Object foo = cls.newInstance();  
- // 被调用函数的参数
-                 Method m = foo.getClass().getMethod("Output", new Class[] {});  
-                 m.invoke(foo, new Object[] {});  
-                 Thread.sleep(500);  
-             }  
- 
-         } catch (Exception ex) {  
-             ex.printStackTrace();  
-         }  
-     }  
- }  
- 
- publicclass Test {  
- 
- publicstatic Method initAddMethod() {  
- try {  
-             Method add = URLClassLoader.class.getDeclaredMethod("addURL",  
- new Class[] { URL.class });  
-             add.setAccessible(true);  
- return add;  
-         } catch (Exception e) {  
- thrownew RuntimeException(e);  
-         }  
-     }  
- 
- publicstaticvoid main(String[] args) {  
- 
- // 热部署测试代码
-         Thread t;  
-         t = new Thread(new Multirun());  
-         t.start();  
-     }  
- }  
GetPI.java内容：
**[java]**[view
 plain](http://blog.csdn.net/ozwarld/article/details/7440868#)[copy](http://blog.csdn.net/ozwarld/article/details/7440868#)
- package yerasel;  
- 
- import java.io.PrintStream;  
- 
- publicclass GetPI  
- {  
- publicstaticdouble Darts(int n)  
-   {  
- int k = 0;  
- double x = 0.0D;  
- double y = 0.0D;  
- for (int i = 0; i < n; i++)  
-     {  
-       x = Math.random();  
-       y = Math.random();  
- 
- if (x * x + y * y <= 1.0D)  
-         k++;  
-     }  
- return4 * k / n;  
-   }  
- // 本热部署实验中，上面的Darts函数没有用到，请忽略
- publicstaticvoid Output() {  
-     System.out.println("Output");  
-   }  
- }  
另一个GetPI.java只在输出时候改为aaaOutput
生成jar包的命令
**[java]**[view
 plain](http://blog.csdn.net/ozwarld/article/details/7440868#)[copy](http://blog.csdn.net/ozwarld/article/details/7440868#)
- jar cvf GetPI.jar GetPI.class
一定要注意的是：class需要放在package名字的文件夹下
本文中.class放在/lib(自定义路径)/yerasel(package名字)下。
运行时候，程序加载指定文件夹下的.class，即开始输出aaaOutput，然后手动将另外一个.class(输出Output的.class)覆盖此文件，
程序立即开始输出Output
运行结果图：
![](https://img-my.csdn.net/uploads/201204/09/1333953682_8140.png)
