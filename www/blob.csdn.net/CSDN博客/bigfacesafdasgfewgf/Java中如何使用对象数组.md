# Java中如何使用对象数组 - bigfacesafdasgfewgf - CSDN博客





2015年03月11日 17:11:53[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：2970










    Java中类的对象可以形成对象数组，然后我们可以定义，作为形参传给函数。需要注意的是对象数组的类型是classNum [] 。对象数组中的每个对象都需要new申请一下。









```java
/***
 * 
 * @author Administrator
 *
 */
public class TestingObjectArray {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub

		Circle4[] circle = new Circle4[5]; 
		for(int i = 0; i < circle.length; i++) {
			circle[i] = new Circle4(Math.random() * 100); 
		}
		
		for(int j = 0; j < circle.length; j++) {
			System.out.println("The area of " + j + " circle is " + circle[j].getArea()); 
		}
		
	}

}

class Circle4 {
	/**
	 * CLASS CIRCLE4
	 * 
	 * Data: radius
	 * Functions: getRadius, setRadius, getArea
	 */
	
	double radius; 
	
	Circle4() {
		radius = 0; 
	}
	
	Circle4(double newRadius) {
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
```



![](https://img-blog.csdn.net/20150311201217897?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)













