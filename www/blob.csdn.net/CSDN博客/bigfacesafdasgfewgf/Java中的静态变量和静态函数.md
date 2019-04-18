# Java中的静态变量和静态函数 - bigfacesafdasgfewgf - CSDN博客





2015年03月11日 15:50:23[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：810标签：[静态变量																[静态函数																[Java](https://so.csdn.net/so/search/s.do?q=Java&t=blog)
个人分类：[Java](https://blog.csdn.net/puqutogether/article/category/2595127)







    Java中的静态变量和静态函数（方法）和C++中也类似。有几点需要重新复习一下：




1）如果想让一个类的所有对象共享数据，可以使用静态变量；如果想共享函数，可以使用静态函数。

2）如果某个对象修改了静态变量的值，那么该类的所有对象都会受到影响。

3）静态变量和静态函数在类创建对象之前就可以调用，这点和普通的变量函数很不一样。

4）常量可以声明为final static，所有对象都共享。

5）访问一个类的静态变量和静态函数，一般都使用“类名.静态变量”或者"类名.静态函数"的方式，不适用对象名调用，因为这样可以直接看出是调用的静态变量和函数。




    一个简单的基础代码帮助理解Java中的静态变量和静态函数的使用。






```java
/***
 * 
 * @author Administrator
 *
 */
class Circle2 {
	/**
	 * CLASS CIRCLE2
	 * 
	 * Data: radius, numberOfObjects
	 * 
	 * Functions: getNumberOfObjects2
	 */
	
	double radius; 
	static int numberOfObjects; 
	
	Circle2() {
		radius = 1.0 ; 
		numberOfObjects++; 
	}
	
	Circle2(double newRadius) {
		radius = newRadius; 
		numberOfObjects++; 
	}
	
	static int getNumberOfObjects2() {
		numberOfObjects++; 
		return numberOfObjects; 
	}
}


public class TestingStaticVariable {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub

		System.out.println("The numebr of Objects is " + Circle2.numberOfObjects + " and " + Circle2.getNumberOfObjects2()); 
		
		Circle c1 = new Circle(); 
		System.out.println("The numebr of Objects is " + Circle2.numberOfObjects + " and " + Circle2.getNumberOfObjects2());
		
		Circle c2= new Circle(10.0); 
		System.out.println("The numebr of Objects is " + Circle2.numberOfObjects + " and " + Circle2.getNumberOfObjects2());
		
	}

}
```



结果：



```
The numebr of Objects is 0 and 1
The numebr of Objects is 1 and 2
The numebr of Objects is 2 and 3
```](https://so.csdn.net/so/search/s.do?q=静态函数&t=blog)](https://so.csdn.net/so/search/s.do?q=静态变量&t=blog)




