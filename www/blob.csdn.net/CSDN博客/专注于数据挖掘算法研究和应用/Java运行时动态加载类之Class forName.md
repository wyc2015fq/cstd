# Java运行时动态加载类之Class.forName - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年03月22日 11:31:26[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2629








需求场景：通过Class.forName加载类并创建实例。

1、接口类：



```java
package cn.fjs;

public interface IC {
	public void action();
}
```


2、实例类：



```java
package cn.fjs;

public class C implements IC{
	public void action(){
		System.out.println("Hi i am A class.");
	}
}
```


3、测试类：



```java
package cn.fjs;

import java.util.Scanner;

public class demo {
	public static void main(String[] args){
		System.out.print("\nPls enter the class name:like cn.fjs.C \n");
		Scanner sc = new Scanner(System.in);
		String cname = sc.next();
		try{
			Class aClass = Class.forName(cname);
			IC ic = (IC)aClass.newInstance();			
			ic.action();
		}catch (Exception e){
			e.printStackTrace();
		}
	}
}
```





