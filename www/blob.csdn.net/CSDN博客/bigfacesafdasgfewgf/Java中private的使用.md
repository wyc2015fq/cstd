# Java中private的使用 - bigfacesafdasgfewgf - CSDN博客





2015年03月11日 16:54:34[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：7579标签：[private类型																[封装性																[java																[构造函数](https://so.csdn.net/so/search/s.do?q=构造函数&t=blog)
个人分类：[Java](https://blog.csdn.net/puqutogether/article/category/2595127)







    Java中也有为了数据的封装性而声明的private类型。这篇就使用一下，大致的用法和C++类似。




    Java中引入private的类型，目的是为了防止类中的数据成员，在类的定义之外被修改。也就说，private类型的数据成员只能在定义的时候修改，其余任何地方都不可以，该类的对象也不可以。

    而且，private类型的数据可以作为构造函数的输入。




    不过，我们也可以在类中定义读取和修改private类型数据的public函数。如下程序：






```java
/***
 * 
 * @author Administrator
 *
 */

class Circle3 {
	/**
	 * CLASS CIRCLE3
	 * 
	 * Data: radius
	 * Functions: getRadius, setRadius, getArea
	 */
	
	private double radius; 
	
	Circle3() {
		radius = 0; 
	}
	
	Circle3(double newRadius) {
		radius = newRadius; 
	}
	
	double getRadius() {
		return radius; 
	}
	
	void setRadius(double newRadius) {
		radius = newRadius; 
	}
	
	double getArea() {
		return Math.PI * radius * radius; 
	}
}
public class TestingPrivate {
	
	/**
	 * MAIN CLASS
	 */
	public static void main(String[] args) {
		
		Circle3 circle = new Circle3(); 
		System.out.println("The radius is " + circle.getRadius()); 
		//
		//System.out.println("The radius is " + circle.radius);  // wrong, since the radius is private
		
		circle.setRadius(10.0); 
		System.out.println("The modified radius is " + circle.getRadius()); 
		
		Circle3 c = new Circle3(50.0); 
		System.out.println("The radius is " + c.getRadius());
	}

}
```


运行结果：




![](https://img-blog.csdn.net/20150311170113280?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=封装性&t=blog)](https://so.csdn.net/so/search/s.do?q=private类型&t=blog)




