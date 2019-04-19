# 自定义ClassLoader动态加载Class - 零度的博客专栏 - CSDN博客
2016年04月12日 11:06:45[零度anngle](https://me.csdn.net/zmx729618)阅读数：3068
1、自定义ClassLoader代码如下：
```java
package zmx.classloader.test;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Method;
import java.util.HashSet;
/*
 *  实现热部署，自定义ClassLoader，加载的是.class
 */
public class CustomClassLoader extends ClassLoader {
	private String basedir; // 需要该类加载器直接加载的类文件的基目录
	private HashSet<String> dynaclazns; // 需要由该类加载器直接加载的类名
	public CustomClassLoader(String basedir, String[] clazns) {
		super(null); // 指定父类加载器为 null
		this.basedir = basedir;
		dynaclazns = new HashSet<>();
		loadClassByMe(clazns);
	}
	private void loadClassByMe(String[] clazns) {
		for (int i = 0; i < clazns.length; i++) {
			loadDirectly(clazns[i]);
			dynaclazns.add(clazns[i]);
		}
	}
	private Class loadDirectly(String name) {
		Class cls = null;
		StringBuffer sb = new StringBuffer(basedir);
		String classname = name.replace('.', File.separatorChar) + ".class";
		sb.append(File.separator + classname);
		File classF = new File(sb.toString());
		try {
			cls = instantiateClass(name, new FileInputStream(classF),classF.length());
		} catch (FileNotFoundException e) {			
			e.printStackTrace();
		}
		return cls;
	}
	private Class instantiateClass(String name, InputStream fin, long len) {
		byte[] raw = new byte[(int) len];
		try {
			fin.read(raw);
			fin.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return defineClass(name, raw, 0, raw.length);
	}
	protected Class loadClass(String name, boolean resolve)
			throws ClassNotFoundException {
		Class cls = null;
		cls = findLoadedClass(name);
		if (!this.dynaclazns.contains(name) && cls == null)
			cls = getSystemClassLoader().loadClass(name);
		if (cls == null)
			throw new ClassNotFoundException(name);
		if (resolve)
			resolveClass(cls);
		return cls;
	}
}
/*
 * 每隔500ms运行一次，不断加载class
 */
class Multirun implements Runnable {
	public void run() {
		try {
			while (true) {
				// 每次都创建出一个新的类加载器
				// class需要放在自己package名字的文件夹下
				String url = System.getProperty("user.dir") + "/WebRoot/WEB-INF/classes";  // "D:/projects/testWeb/WebRoot/WEB-INF/classes/zmx/classloader/test/DynamicClass.class";
				CustomClassLoader cl = new CustomClassLoader(url, new String[] { "zmx.classloader.test.DynamicClass" });
				Class cls = cl.loadClass("zmx.classloader.test.DynamicClass");
				Object foo = cls.newInstance();
				// 被调用函数的参数
				Method m = foo.getClass().getMethod("Output", new Class[] {});
				m.invoke(foo, new Object[] {});
				Thread.sleep(500);
			}
		} catch (Exception ex) {
			ex.printStackTrace();
		}
	}
}
```
        使用上述类加载器，加载动态类.
2、DynamicClass.java代码如下：
package zmx.classloader.test;
public class DynamicClass {
 public static double Darts(int n)  
   {  
     int k = 0;  
     double x = 0.0D;     
     double y = 0.0D;     
     for (int i = 0; i < n; i++)  
     {  
       x = Math.random();  
       y = Math.random();     
       if (x * x + y * y <= 1.0D)  
         k++;  
     }  
     return 4 * k / n;  
   }  
   // 本热部署实验中，上面的Darts函数没有用到，请忽略  
   public static void Output() {        
     System.out.println("Frist Class Output");  
   } 
}
3、测试类MainTest.java代码如下：
package zmx.classloader.test;
import java.lang.reflect.Method;
import java.net.URL;
import java.net.URLClassLoader;
public class MainTest {
 public static Method initAddMethod() {
  try {
   Method add = URLClassLoader.class.getDeclaredMethod("addURL",new Class[] { URL.class });
   add.setAccessible(true);
   return add;
  } catch (Exception e) {
   throw new RuntimeException(e);
  }
 }
 public static void main(String[] args) throws Exception{
     /**
      * 通过Jar包，加载class文件
      */
  /*
  URLClassLoader  classloader = (URLClassLoader)ClassLoader.getSystemClassLoader(); 
  String url = "file:"+ System.getProperty("user.dir") + "/WebRoot/WEB-INF/classes/zmx/classloader/test/test.jar";
  Method addURL = URLClassLoader.class.getDeclaredMethod("addURL",new Class[] { URL.class }); 
  addURL.setAccessible(true);
     URL classUrl = new URL(url);      
     addURL.invoke(classloader, new Object[] { classUrl });
     String className = "zmx.classloader.test.DynamicClass";  
     Class<?> c = Class.forName(className);  
     DynamicClass.Output();
     */
  // 热部署测试代码
  Thread t;
  t = new Thread(new Multirun());
  t.start();
 }
}
    程序运行过程中将DynamicClass中的Output()的输出修改为 System.out.println("Second Class Output");重新编译。即 运行时候，程序加载指定文件夹下的.class，即开始输出 Frist Class Output ，然后手动将另外一个.class(输出Second Class Output的.class)覆盖此文件。程序输出为 Second Class Output。
输出结果如下：
Frist Class Output
Frist Class Output
Frist Class Output
Frist Class Output
Frist Class Output
Frist Class Output
Second Class Output
Second Class Output
Second Class Output
Second Class Output
