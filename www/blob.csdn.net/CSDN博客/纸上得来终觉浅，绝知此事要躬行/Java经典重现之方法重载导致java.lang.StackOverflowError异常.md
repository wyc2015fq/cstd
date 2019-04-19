# Java经典重现之方法重载导致java.lang.StackOverflowError异常 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客
2019年04月17日 19:17:58[boonya](https://me.csdn.net/boonya)阅读数：11
由于代码写的不够严谨，经常认为万无一失的地方却出现了重大问题，比如说简单的一个方法重载导致的栈溢出。这种问题已经看到好几次了，虽然代码不是自己写的，还是应该引起重视。
**目录**
[定义接口和实现](#%E5%AE%9A%E4%B9%89%E6%8E%A5%E5%8F%A3%E5%92%8C%E5%AE%9E%E7%8E%B0)
[错误日志](#%E9%94%99%E8%AF%AF%E6%97%A5%E5%BF%97)
## 定义接口和实现
接口：
```java
package com.boonya.stackerror;
public interface Geometry {
    
    /**
     * 工作
     */
    void doWork();
}
```
实现一：
```java
package com.boonya.stackerror;
public class LineString implements Geometry{
	public void doWork() {
		// TODO Auto-generated method stub
		System.out.println("LineString do work.");
	}
}
```
实现二：
```java
package com.boonya.stackerror;
public class Polygon implements Geometry{
	public void doWork() {
		// TODO Auto-generated method stub
		System.out.println("Ploygon do work.");
	}
}
```
 服务方法多重载：
```java
package com.boonya.stackerror;
public class GeometryService {
	
	/**
	 * 栈溢出方法
	 * 
	 * @param geometry
	 * @param polygon
	 */
	public static void  check(LineString geometry,Polygon polygon){
		check(geometry, polygon);
	}
	
	/**
	 * 目标方法
	 * @param geometry
	 * @param geometry2
	 */
    public static void  check(Geometry geometry,Geometry geometry2){
    	geometry.doWork();
    	geometry2.doWork();
	}
}
```
```java
package com.boonya.stackerror;
public class GeometryService {
	
	/**
	 * 栈溢出方法
	 * 
	 * @param geometry
	 * @param polygon
	 */
	public static void  doWork(LineString geometry,Polygon polygon){
		doWork(geometry, polygon);
	}
	
	/**
	 * 目标方法
	 * @param geometry
	 * @param geometry2
	 */
    public static void  doWork(Geometry geometry,Geometry geometry2){
    	geometry.doWork();
    	geometry2.doWork();
	}
}
```
测试类：
```java
package com.boonya.stackerror;
public class StackErrorTest {
	public static void main(String[] args) {
		LineString lineString = new LineString();
		Polygon polygon = new Polygon();
		GeometryService.check(lineString, polygon);
	}
}
```
```java
package com.boonya.stackerror;
public class StackErrorTest {
	public static void main(String[] args) {
		LineString lineString = new LineString();
		Polygon polygon = new Polygon();
		GeometryService.doWork(lineString, polygon);
	}
}
```
## 错误日志
栈溢出的日志会一直打印下去，直到程序被关闭：
```
Exception in thread "main" java.lang.StackOverflowError
	at com.boonya.stackerror.GeometryService.check(GeometryService.java:12)
	at com.boonya.stackerror.GeometryService.check(GeometryService.java:12)
	at com.boonya.stackerror.GeometryService.check(GeometryService.java:12)
	at com.boonya.stackerror.GeometryService.check(GeometryService.java:12)
	at com.boonya.stackerror.GeometryService.check(GeometryService.java:12)
	at com.boonya.stackerror.GeometryService.check(GeometryService.java:12)
	at com.boonya.stackerror.GeometryService.check(GeometryService.java:12)
	at com.boonya.stackerror.GeometryService.check(GeometryService.java:12)
```
```
Exception in thread "main" java.lang.StackOverflowError
	at com.boonya.stackerror.GeometryService.doWork(GeometryService.java:12)
	at com.boonya.stackerror.GeometryService.doWork(GeometryService.java:12)
	at com.boonya.stackerror.GeometryService.doWork(GeometryService.java:12)
	at com.boonya.stackerror.GeometryService.doWork(GeometryService.java:12)
	at com.boonya.stackerror.GeometryService.doWork(GeometryService.java:12)
	at com.boonya.stackerror.GeometryService.doWork(GeometryService.java:12)
	at com.boonya.stackerror.GeometryService.doWork(GeometryService.java:12)
	at com.boonya.stackerror.GeometryService.doWork(GeometryService.java:12)
	at com.boonya.stackerror.GeometryService.doWork(GeometryService.java:12)
	at com.boonya.stackerror.GeometryService.doWork(GeometryService.java:12)
```
这样我们就重现了一个栈溢出的情形。
