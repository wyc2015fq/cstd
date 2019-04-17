# Java运行时动态加载类之ClassLoader - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年03月22日 17:13:20[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：6374








需求场景：动态加载类ClassLoaderd，在xml文件中配置加载类名称和方法，：

一、准备

1）在D:\\tmp\\目录下配置a.xml文件：



```
<?xml version="1.0" encoding="ISO-8859-1"?>
<classes>

 <class name="User"> 
 <method>say</method> 
 </class> 

 <class name="map"> 
 <method>add</method> 
 </class> 

</classes>
```


2）要动态加载的类：



```java
package dx;

public class map {
	public void add(){
		System.out.println("1+1=2");
	}
}
```


```java
package cn.fjs;

public class User {
	
	public void say(){
		System.out.println(" hello ...");
	}
 
}
```


对这两个类进行编译后，将class文件复制到D:\\tmp\\路径下。




二、参考代码如下


1、重载ClassLoader类：



```java
package cn.fjs;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;

public class DynamicClassLoader extends ClassLoader{
	
	private static final String SUFFIX = ".class";
	public String[] paths;

	public DynamicClassLoader(String[] paths) {
		this.paths = paths;
	}
	
	public DynamicClassLoader(ClassLoader parent,String[] paths){
		super(parent);
		this.paths = paths;
	}

	@SuppressWarnings("deprecation")
	@Override
	protected Class<?> findClass(String className) throws ClassNotFoundException { 
		String classPath = getClassPath(className);
		if(classPath != null){
			byte[] clazz = loadClazz(classPath);
			return defineClass(clazz, 0, clazz.length); 
		}else{
			System.out.println("class is not found !");
			return null;
		}
	}
 
	public byte[] loadClazz(String classPath) { 
		try { 
			FileInputStream in = new FileInputStream(new File(classPath));
			ByteArrayOutputStream baos = new ByteArrayOutputStream();
			int b;
			while((b = in.read()) != -1){
				baos.write(b);
			}
			in.close();
			return baos.toByteArray();
		} catch (Exception e) {
			System.out.println(e);
		}
		return null;
	}
	
	public String getClassPath(String className){
		for(String path : paths){
			if(className.contains(".")){
				className = className.replaceAll(".", File.separator);
			}
			String classPath = path + className + SUFFIX; 
			File classFile = new File(classPath);
			if(classFile.exists()){
				return classPath;
			}
		}
		return null;
	}
}
```


2、解析xml文件



```java
package cn.fjs;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.Element; 
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;
 
//解析xml文件，获取类和方法
public class DynamicDom {
	private static DocumentBuilderFactory dbFactory = null;  
    private static DocumentBuilder db = null;  
    private static Document document = null;  
    
    static{  
        try {  
            dbFactory = DocumentBuilderFactory.newInstance();  
            db = dbFactory.newDocumentBuilder();  
        } catch (ParserConfigurationException e) {  
            e.printStackTrace();  
        }  
    }  
    
   
    public Map<String,List<String>> getMethods(String fileName) throws SAXException, IOException{  
    	Map<String,List<String>> classes = new HashMap<String, List<String>>();
    	document = db.parse(fileName);  
    	NodeList nList = document.getElementsByTagName("class");
    	for(int i = 0 ; i<nList.getLength();i++){ 
    		Node node = nList.item(i); 
    		Element ele = (Element)node; 
    		if(node.getNodeType() == Element.ELEMENT_NODE){
    		  String clazz = ele.getAttribute("name"); 
    		  List<String> methods = new ArrayList<String>();
    		  String method = ele.getElementsByTagName("method").item(0).getTextContent();
    		  methods.add(method);
    		  classes.put(clazz, methods); 		  
    		}
    	}
    	return classes;
    } 
}
```


3、测试类：



```java
package cn.fjs;

import java.util.List;
import java.util.Map;
import cn.fjs.DynamicClassLoader;
import cn.fjs.DynamicDom;

public class DynamicClassLoaderTest {
	public static void main(String[] args) {
		DynamicDom dmo = new DynamicDom();//xml文件解析类
		Map<String, List<String>> classes;
	    //重载ClassLoader类
		DynamicClassLoader loader = new DynamicClassLoader(new String[]{"D:\\tmp\\"}); 		
		try {
			classes = dmo.getMethods("D:\\tmp\\a.xml");
			for(String key:classes.keySet()){ 
				for(String clazz : classes.get(key)){ 
					Class<?> c =loader.findClass(key);//类名字
					c.getMethod(clazz).invoke(c.newInstance());//方法名字
				}
			} 
		}catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
```




执行结果：



```
hello ...
1+1=2
```






