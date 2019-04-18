# Java中的类和对象 - bigfacesafdasgfewgf - CSDN博客





2015年03月11日 14:52:03[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：589










    Java中类和对象和C++中类似，只不过在具体使用的时候有几个地方需要额外注意的。这里我列出来，几个主要的，后面如果发现了，或者理解更加深入了，再添加。




    这篇博文还有一个重要的作用，就是确定Java编程的习惯，类在定义的时候该写什么注释，这些都要有一个较好的习惯。




1）一个java文件中，可以存在多个class，但是只能有一个public class + 和文件名相同的类名。这个类是主类，名字一定要定义的和文件名一致。

2）只能在主类中定义public static void main(string [] args) {}. 有main方法的才是主类，才能运行。

3）类在定义的时候，构造函数也是可以重载的。




    一个基础的代码如下：






```java
/**
 * 
 * @author Powered by Zhu Yangping
 *
 */

class Circle {
	/**
	 * CIRCLE CLASS            注意格式
	 * 
	 * Data: radius
	 * 
	 * Functions: getCircum, getArea
	 */
	
	// data
	double radius; 
	
	// constructor function 1
	Circle() {
		radius = 1.0; 
	}
	
	// constructor function 2
	Circle(double newRadius) {
		radius = newRadius; 
	}
	
	// getCircum function
	double getCircum() {
		return 2 * radius * Math.PI; 
	}
	
	// getArea function
	double getArea() {
		return radius * radius * Math.PI; 
	}
}




public class TestCircle {

	/**
	 * @param args          注意格式
	 * 
	 * MAIN CLASS
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub

		Circle circle1 = new Circle(); 
		System.out.println("The circum of circle1 is " + circle1.getCircum() + ", and its area is " + circle1.getArea()); 
		
		Circle circle2 = new Circle(25.0);
		System.out.println("The circum of circle1 is " + circle2.getCircum() + ", and its area is " + circle2.getArea());
		
		Circle circle3= new Circle(10);
		System.out.println("The circum of circle1 is " + circle3.getCircum() + ", and its area is " + circle3.getArea());
		
		
		
	}

}
```

















