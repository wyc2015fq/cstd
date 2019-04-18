# Java运行时动态加载类之ClassLoader方法带参数 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年03月23日 11:07:37[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2778








和该文环境一样http://blog.csdn.net/fjssharpsword/article/details/64922083

不同的是，在load出的class调用具体方法时传递参数。

1、User.class代码



```java
package cn.fjs;

public class User {
	
	public void say(String name){
		System.out.println(name);
	}
 
}
```


2、DynamicClassLoaderTest代码



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





