# Java学习（十） 多态 - HJ - CSDN博客
2018年03月07日 21:54:57[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：122
所属专栏：[Java学习](https://blog.csdn.net/column/details/19570.html)
多态
1. 多态的特性
（1）当父类和子类具有相同的成员变量，那么在多态下访问的是父类的成员变量；
（2）当父类和子类具有相同的非静态方法（就是子类重写父类方法），多态下访问的是子类的成员方法；
（3）当父类和子类具有相同的静态成员法（就是子类重写父类静态方法），多态下访问的是父类的静态方法；
2. 多态的体现
（1）父类引用变量指向了子类的对象；
（2）父类引用也可以接受自己的子类对象；
3. 多态的总结
（1）多态的前提：类与类之间有关系，继承或者实现；
（2）多态的弊端：提高扩展性，但是只能是使用父类引用指向父类成员；
（3）多态在编译时期，参考引用型变量所属的类是否有调用的方法，如果有编译通过，没有则编译失败；
（4）多态在运行时期，参考对象所属的类是否有调用方法；
（5）成员行数在多态调用时，编译看左边，运行看右边；
4. 多态练习
（1）多态作为形参，接受范围更广的对象，避免函数重载过度使用
描述：定义功能，根据输出任何图形的面积和周长；
```java
abstract class MyShape{
	public abstract double getArea();
	public abstract double getLen();
}
class  Rect extends MyShape{
	double width ;
	double height;
	Rect(){
	}
	Rect(double width ,double height){
		this.width=width;
		this.height=height;
	}
	public double getArea(){
		return width*height;
	}
	public  double getLen(){
		return 2*(width+height);
	}
	
}
class Circle extends MyShape{
	double r;
	public static final double PI=3.14;
	 
	Circle(){
	 
	}
	 
	Circle(double r){
	 	this.r=r;
	}
	public double getLen(){
	 	return 2*PI*r;
	}
	
	public double getArea(){
	 	return PI*r*r;
	}
}
class Main{
	public static void main(String[] args){
		System.out.println();
		print(new Rect(3,4)); //MyShape m =new Rect(3,4);
		print(new Circle(3));
     }
     
     //根据用户传入的图形对象，计算出该图形的面积和周长
     //1：多态可以作为形参，接受范围更广的对象，避免函数重载过度使用。 
     public static void print(MyShape m){  
     	System.out.println(m.getLen());
     	System.out.println(m.getArea());
     }   	
}
```
（2）作为返回值类型
描述：获取任意一辆车对象，定义汽车类，有名字和颜色，提供有参合无参构造，有运行的行为；
```java
class Car {
	String name;
	String color;
	Car() {
	}
	Car(String name, String color) {
		this.name = name;
		this.color = color;
	}
	void run() {
		System.out.println("跑跑。。。。");
	}
}
class Bmw extends Car {
	Bmw() {
	}
	Bmw(String name, String color) {
		super(name, color);
	}
	void run() {
		System.out.println("宝马很拉风。。。。");
	}
}
class Benz extends Car {
	Benz() {
	}
	Benz(String name, String color) {
		super(name, color);
	}
	void run() {
		System.out.println("奔驰商务首选。。。。");
	}
}
class Bsj extends Car {
	Bsj() {
	}
	Bsj(String name, String color) {
		super(name, color);
	}
	void run() {
		System.out.println("泡妞首选。。。。");
	}
}
class Main {
	public static void main(String[] args) {
		int x = 0;
		while (x < 100) {
			Car c = CarFactory();
			c.run();
			x++;
		}
	}
	// 定义静态方法，汽车工厂，随机生产汽车。使用多态定义方法返回值类型。
	// 使用随机数，0.1.2 if 0 bsj 1 bmw 2 bc
	public static Car CarFactory() {
		int x = (int) Math.round(Math.random() * 2);
		if (0 == x) {
			return new Bmw("宝马x6", "红色");
		} else if (1 == x) {
			return new Benz("奔驰", "黑色");
		} else if (2 == x) {
			return new Bsj("保时捷", "棕色");
		} else {
			return new Benz("Smart", "红色");
		}
	}
}
```
（3）多态类型转型
类类型转换规则：
①自动：  子类转父类
②强转：  父类转子类
```java
class Father {
	void method1() {
		System.out.println("这是父类1");
	}
	void method2() {
		System.out.println("这是父类2");
	}
}
class Son extends Father {
	void method1() {
		System.out.println("这是子类1");
	}
	void method3() {
		System.out.println("这是子类3");
	}
}
class Main {
	public static void main(String[] args) {
		Father f = new Son();
		f.method1(); // 这是子类1
		f.method2(); // 这是父类2
		// f.method3(); //编译报错。
		// 多态弊端，只能使用父类引用指向父类成员。
		// 类类型转换
		Son s = (Son) f;
		s.method3();
		System.out.println();
	}
}
```
