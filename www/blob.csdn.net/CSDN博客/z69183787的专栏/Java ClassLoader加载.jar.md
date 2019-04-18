# Java ClassLoader加载.jar - z69183787的专栏 - CSDN博客
2014年06月07日 21:09:21[OkidoGreen](https://me.csdn.net/z69183787)阅读数：899
个人分类：[Java-知识](https://blog.csdn.net/z69183787/article/category/2175279)

程序如下：
**[java]**[view
 plain](http://blog.csdn.net/ozwarld/article/details/7440937#)[copy](http://blog.csdn.net/ozwarld/article/details/7440937#)
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
- publicstatic Method addURL = null;  
- publicstatic URLClassLoader classloader = null;  
- 
- publicstaticvoid main(String[] args) {  
- 
- /*
-          *  系统加载jar
-          */
- try {  
- 
- // 系统ClassLoader只能加载.jar
- // 动态加载jar
-                  addURL = initAddMethod();  
-                  classloader = (URLClassLoader)ClassLoader.getSystemClassLoader();  
-                  String url = "file:"+ System.getProperty("user.dir") + "/lib/yerasel/GetPI.jar"; // 包路径定义
-                  System.out.println(url);  
-                  URL classUrl = new URL(url);  
-                  addURL.invoke(classloader, new Object[] { classUrl });  
- 
-                  String className = "yerasel.GetPI";  
-                  Class<?> c = Class.forName(className);  
-                  Object obj = c.newInstance();  
- // 被调用函数的参数
-                  Class[] parameterTypes = {};  
-                  Method method2 = c.getDeclaredMethod("Output",parameterTypes);  
-                  method2.invoke(obj, null);  
-         } catch (Exception exp) {  
-             exp.printStackTrace();  
-         }  
-     }  
- }  
运行结果如图：
![](https://img-my.csdn.net/uploads/201204/09/1333954051_6426.png)
.jar文件结构如下：
![](https://img-my.csdn.net/uploads/201204/09/1333954146_6910.png)
这幅图是说，需要在.jar中加入package名字的一个文件夹
是的jar的结构为
-jar含有yerasel(package文件夹)，yerasel中含有.class。
加入的方法是：
1. 新建一个新建 WinRAR ZIP archive.zip，（作为浏览器），双击，进入.jar所在的目录
2. 建立目录yerasel，放入.class
3. 将yerasel目录拖入.jar中，进行压缩。
刚刚生成的jar结果图：
![](https://img-my.csdn.net/uploads/201204/09/1333954637_1772.png)
可以看到.class外面没有包裹package名字的文件夹
加入目录之后的结果图：
![](https://img-my.csdn.net/uploads/201204/09/1333954653_6581.png)
可以看到.class外面包裹着package名字的文件夹。这样.jar才可以被正确加载。
总之，jar就是一种压缩格式，可以用WinRAR来查看，所以可以像上述这样做。
