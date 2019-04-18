# Java运行时动态加载类之ClassLoader加载class及其依赖jar包 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年03月23日 12:00:51[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：7668








需求场景是：通过ClassLoader动态加载外部class文件，class文件又依赖某个具体jar包，需要动态加载jar包，采用URLClassLoader。

1、xml配置文件



```
<?xml version="1.0" encoding="ISO-8859-1"?>
<classes>

 <class name="User"> 
 <jar>ETLEnc.jar</jar>
 <method>say</method> 
 </class> 

</classes>
```


放在D:\\tmp\\目录下；

2、User.class文件放在D:\\tmp\\目录下，依赖ETLEnc.jar也放在D:\\tmp\\目录下，User代码如下：



```java
package cn.fjs;

import com.gddx.enc.ETLEncode;

public class User {
	
	public void say(String name){
		ETLEncode ete=new ETLEncode();
		try {
			System.out.println(ete.encrypt(name));
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}		
	} 
}
```


其中com.gddx.enc.ETLEncode是ETLEnc.jar包中的类。

3、xml文件解析类代码：



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
    
    public Map<String,List<String>> getJars(String fileName) throws SAXException, IOException{  
    	Map<String,List<String>> classes = new HashMap<String, List<String>>();
    	document = db.parse(fileName);  
    	NodeList nList = document.getElementsByTagName("class");
    	for(int i = 0 ; i<nList.getLength();i++){ 
    		Node node = nList.item(i); 
    		Element ele = (Element)node; 
    		if(node.getNodeType() == Element.ELEMENT_NODE){
    		  String clazz = ele.getAttribute("name"); 
    		  List<String> jars = new ArrayList<String>();
    		  String jar = ele.getElementsByTagName("jar").item(0).getTextContent();
    		  jars.add(jar);
    		  classes.put(clazz, jars); 		  
    		}
    	}
    	return classes;
    } 
}
```


4、动态加载jar包工具类：



```java
package cn.fjs;

import java.io.File;  
import java.lang.reflect.Method;  
import java.net.URL;  
import java.net.URLClassLoader;  
import java.util.ArrayList;  
import java.util.List;

public final class JarLoaderUtil {
	/** URLClassLoader的addURL方法 */  
    private static Method addURL = initAddMethod();  
      
    /** 初始化方法 */  
    private static final Method initAddMethod() {  
        try {  
            Method add = URLClassLoader.class  
                .getDeclaredMethod("addURL", new Class[] { URL.class });  
            add.setAccessible(true);  
            return add;  
        } catch (Exception e) {  
            e.printStackTrace();  
        }  
        return null;  
    }  
  
    private static URLClassLoader system = (URLClassLoader) ClassLoader.getSystemClassLoader();  
  
    /** 
     * 循环遍历目录，找出所有的JAR包 
     */  
    private static final void loopFiles(File file, List<File> files) {  
        if (file.isDirectory()) {  
            File[] tmps = file.listFiles();  
            for (File tmp : tmps) {  
                loopFiles(tmp, files);  
            }  
        } else {  
            if (file.getAbsolutePath().endsWith(".jar") || file.getAbsolutePath().endsWith(".zip")) {  
                files.add(file);  
            }  
        }  
    }  
  
    /** 
     * <pre> 
     * 加载JAR文件 
     * </pre> 
     * 
     * @param file 
     */  
    public static final void loadJarFile(File file) {  
        try {  
            addURL.invoke(system, new Object[] { file.toURI().toURL() });  
            //System.out.println("加载JAR包：" + file.getAbsolutePath());  
        } catch (Exception e) {  
            e.printStackTrace();  
        }  
    }  
  
    /** 
     * <pre> 
     * 从一个目录加载所有JAR文件 
     * </pre> 
     * 
     * @param path 
     */  
    public static final void loadJarPath(String path) {  
        List<File> files = new ArrayList<File>();  
        File lib = new File(path);  
        loopFiles(lib, files);  
        for (File file : files) {  
            loadJarFile(file);  
        }  
    }  
}
```

参考：http://blog.csdn.net/fjssharpsword/article/details/64905874

URLClassLoader也可以动态加载jar包的类并执行具体方法。





5、测试类：



```java
package cn.fjs;

import java.io.File;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.List;
import java.util.Map;
import cn.fjs.DynamicClassLoader;
import cn.fjs.DynamicDom;

public class DynamicClassLoaderTest {
	public static String FilePath="D:\\tmp\\";
	public static void main(String[] args) {
		DynamicDom dmo = new DynamicDom();//xml文件解析类
		Map<String, List<String>> classes;
		Map<String, List<String>> jars;
		try {
			//动态记载依赖包
			jars = dmo.getJars(FilePath+"a.xml");
			for(String key:jars.keySet()){
				for(String jar : jars.get(key)){
					JarLoaderUtil.loadJarFile(new File(FilePath+jar));
				}
			}		
			//动态加载类
			DynamicClassLoader loader = new DynamicClassLoader(new String[]{FilePath});
			classes = dmo.getMethods(FilePath+"a.xml");
			for(String key:classes.keySet()){ 
				for(String clazz : classes.get(key)){ 
					Class<?> c =loader.findClass(key);//类名字
					//c.getMethod(clazz).invoke(c.newInstance());//方法名字
					c.getMethod(clazz,String.class).invoke(c.newInstance(),"fjs");//带参数
				}
			} 
		}catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
```






