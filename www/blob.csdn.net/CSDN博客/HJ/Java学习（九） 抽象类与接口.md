# Java学习（九） 抽象类与接口 - HJ - CSDN博客
2018年03月07日 21:36:29[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：117
个人分类：[Java](https://blog.csdn.net/feizaoSYUACM/article/category/7349915)
所属专栏：[Java学习](https://blog.csdn.net/column/details/19570.html)
一、抽象类
1. 为什么定义抽象类？
当描述一个类的时候，如果不能确定功能函数如何定义，那么该类就可以定义为抽象类，功能函数应该描述为抽象函数。
2. 抽象类的实现方式
使用abstract表明该类为抽象类
描述：定义Dog类，定义Bird类，定义其父类Animal，抽取共性颜色属性和叫的方法
实现代码如下：
```java
abstract class Animal {
    String color;
    abstract void shout();
}
class Dog extends Animal {
	void shout() {
		System.out.println("旺旺");
	}
}
class Bird extends Animal {
	void shout() {
		System.out.println("叽叽喳喳");
	}
}
```
3. 抽象类的特点
（1）有抽象函数的类，该类一定是抽象类；
（2）抽象类中不一定要有抽象函数；
（3）抽象类不能使用new创建对象，原因是创建对象在使用对象的功能时候，抽象类的方法没有方法体；
（4）抽象类主要是为了提高代码的复用性，让子类继承来使用；
（5）编译器强制子类实现抽象类父类的未实现的方法，但如果子类也声明为抽象类就可以不实现；
4. 抽象类疑问解答：
（1）抽象类不能创建对象，那么抽象类中是否有构造函数？
抽象类中一定有构造函数。主要为了初始化抽象类中的属性，通常由子类实现。
（2）Final和abstract是否可以同时修饰一个类？
一定不能同时修饰。
5. 抽象类练习
描述：
（1）定义抽象类MyShape（图形），定义抽象方法获取图形的长度和面积；
（2）定义子类Rect继承父类MyShape，定义自身特有的长和宽，实现父类未实现的函数；
（3）定义子类Circle继承父类MyShape，定义自身特有的半径和圆周率，实现父类未实现的函数；
代码如下：
```java
abstract class MyShape {
	abstract double getLen();
	abstract double getArea();
}
class Rect extends MyShape {
	double width;
	double height;
	Rect() {
	}
	Rect(double width, double height) {
		this.width = width;
		this.height = height;
	}
	double getLen() {
		return 2 * (width + height);
	}
	double getArea() {
		return width * height;
	}
}
class Circle extends MyShape {
	double r;
	public static final double PI = 3.14;
	Circle() {
	}
	Circle(double r) {
		this.r = r;
	}
	double getLen() {
		return 2 * PI * r;
	}
	double getArea() {
		return PI * r * r;
	}
}
class Main {
	public static void main(String[] args) {
		Rect r = new Rect(5, 5);
		System.out.println(r.getLen());
		System.out.println(r.getArea());
		System.out.println();
		Circle c = new Circle(5);
		System.out.println(c.getLen());
		System.out.println(c.getArea());
	}
}
```
6. 抽象类总结
（1）抽象类可以没有抽象方法；
（2）抽象类可以继承普通类与抽象类；
（3）抽象类不能直接使用类名创建实例，但是又构造方法，构造方法是让子类进行初始化；
（4）抽象类一定有构造方法；
（5）Abstract与其他修饰符的关系：
①final与abstract不能共存，final的作用是修饰类代表不可以继承，修饰方法不可重写，abstract修饰类就是用来被继承的，修饰方法就是用来被重写的；
②static与abstract不能共存，static修饰的方法可以用类名调用，而对于abstract修饰的方法没有具体的方法实现，所以不能直接调用；
③private与abstract不能共存，private修饰的只能在本类中使用，而abstract方法是用来被子类进行重写的，互相矛盾；
二、接口
1. 接口出现的原因（Java只能单继承），如图
![](https://img-blog.csdn.net/20180307213422603)
接口：主要是用来拓展定义类的功能，可以弥补Java中单继承的缺点；
2. 接口的定义格式：
```java
interface 接口名{
    属性
    抽象方法
}
```
注意：
（1）接口中的所有属性默认的修饰符是  public static final；
（2）接口中的所有方法默认的修饰符是  public abstract；
（3）类实现接口可以通过implements实现；
3. 接口的特点：
（1）类实现接口的时候必须把接口中的所有方法实现，一个类可以实现多个接口；
（2）接口中定义的所有的属性默认是public static final的，即静态常量，定义的时候必须赋值；
（3）接口中定义的方法不能有方法体；
（4）有抽象函数的类不一定是抽象类，也可以是接口类；
（5）由于接口中的方法默认都是抽象的，所以不能被实例化；
（6）对于接口而言，可以使用子类来实现接口中未被实现的功能函数；
（7）如果实现类中要访问接口中的成员，不能使用super关键字，因为两者之间没有继承关系，况且接口中的成员属性是静态的，可以使用接口名直接访问；
（8）接口中没有构造方法；
4. 接口与类、接口之间的关系
①接口与类之间是实现关系。非抽象类实现接口时，必须把接口里面的所有方法实现。类实现接口用关键之implements，类与接口之间是可以多实现的（即一个类可以实现多个接口）。
```java
interface Eraser {
	public static final String color = "白色";
	public abstract void clean();
}
class Pencil implements Eraser {
	String name;
	Pencil() {
	}
	Pencil(String name) {
		this.name = name;
	}
	void write() {
		System.out.println("写字");
	}
	public void clean() {
		System.out.println("涂改...");
	}
}
```
②接口与接口之间的关系是继承，接口可以多继承接口
```java
interface A{
	public void show();
}
interface B{
	public void print();
}
interface C extends A,B{
	
}
```
