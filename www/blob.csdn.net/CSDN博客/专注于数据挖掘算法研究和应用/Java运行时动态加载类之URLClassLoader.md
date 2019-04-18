# Java运行时动态加载类之URLClassLoader - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年03月22日 10:48:07[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2986








需求场景：通过URLClassLoader从jar文件中加载类并创建实例，可实现运行时动态加载

1、要加载的jar：

1）接口类IC



```java
package cn.fjs;

public interface IC {
	public void action();
}
```


2）实例类C



```java
package cn.fjs;

public class C implements IC{
	public void action(){
		System.out.println("Hi i am A class.");
	}
}
```


export到jar，放在指定目录，如D:\\tmp\\C.jar

2、动态加载：

1）接口类IC



```java
package cn.fjs;

public interface IC {
	public void action();
}
```


2）调度类Test



```java
package cn.fjs;

import java.io.File;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.Scanner;

public class Test implements Runnable{
	public Test(){
		new Thread(this).start();
	}
	
	public void run(){
		System.out.print("Pls enter the jar directory:like D\\tmp\\C.jar \n");
		Scanner sc = new Scanner(System.in);
		String dir = sc.next();
		System.out.print("Pls enter the class name:like cn.fjs.C \n");
		String cname = sc.next();
		load(dir,cname);
		System.out.println("\n\n");
	}
	
	private void load(String dir,String cname){
		String jarName = "file:"+dir;
		System.out.println(jarName);
		try{
			File file = new File(jarName);
			URL url = file.toURL();
			URLClassLoader loader = new URLClassLoader(new URL[]{url});
			Class aClass = loader.loadClass(cname);
			//利用Java反射机制创建实例测试方法
			IC ic = (IC)aClass.newInstance();
			ic.action();
		}catch (Exception e){
			e.printStackTrace();
		}
	}
	
	public static void main(String[] args){
		new Test();
	}
}
```







