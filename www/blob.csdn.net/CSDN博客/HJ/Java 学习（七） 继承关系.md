# Java 学习（七） 继承关系 - HJ - CSDN博客
2018年02月08日 14:34:12[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：134
个人分类：[Java](https://blog.csdn.net/feizaoSYUACM/article/category/7349915)
所属专栏：[Java学习](https://blog.csdn.net/column/details/19570.html)
继承 
一、extends关键字
1.特点：
①描述类和类之间的关系；
②降低类和类之间的重复代码；
③降低对象和对象之间的代码重复使用静态变量；
④降低类和类之间代码重复使用继承；
2.继承使用extends关键字实现。 
继承的格式：
```java
class 类名1 extends 类名2{
}
```
代码样例：
```java
class Person {
	String name;
	int age;
	// 静态变量（类变量）对象和对象之间的代码重复使用静态变量
	static String country = "CN";
	Person() {
	}
	void speak() {
		System.out.println(name + ":哈哈，我是人！！！");
	}
}
// 让学生类和人类产生关系，发现学生is a 人，就可以使用继承
class Student extends Person {
	Student() {
	}
	void study() {
		System.out.println("姓名：" + name + "年纪：" + age + ":好好学习");
	}
}
class Worker extends Person {
	void work() {
		System.out.println(name + ":好好工作，好好挣钱。");
	}
}
class Main {
	public static void main(String[] args) {
		Student stu = new Student();
		stu.name = "jack";
		stu.age = 20;
		stu.study();
		stu.speak();
		System.out.println(stu.country);
		System.out.println(Student.country);
		Worker worker = new Worker();
		worker.name = "rose";
		System.out.println(worker.country);
		worker.work();
		worker.speak();
		System.out.println();
	}
}
```
3.继承要注意的事项：
①类名的设定：被继承的类称之为父类（基类），继承的类称之为子类；
②千万不要为了减少重复代码而去继承，只有真正存在着继承关系的时候才去继承；
③子类并不能继承父类中所有的成员：父类私有成员、构造函数不能被子类继承；
③创建子类对象时会默认先调用父类无参的构造函数；
代码样例：
```java
class Person{
	String name;
	private int age;
	public  Person(String name){
		this.name = name;
	}
	public Person(){
		System.out.println("Person类的构造方法被调用了....");
	}
	public void eat(){
		System.out.println(name+"在吃饭...");
	}
}
//学生类
class Student extends Person {  // Student 就称作为Person类的子类， Person类就称作为Student的父类(超类、基类)
	int num; //学号
	public Student(){
		System.out.println("Student类的构造方法被调用了....");
	}
	public void study(){
		System.out.println(name+"good good study , day day up");
	}	
}
class Main {
	public static void main(String[] args) {
		Student s = new Student();
	}
}
```
二.super关键字
1.super关键字的作用：
①主要存在于子类方法中，用于指向子类对象中父类对象；
②用于访问父类的属性、函数和构造函数；
描述：定义一个Father类，定义Son类，创建Son对象。
代码样例：
```java
class Father {
	int x = 1;
	Father() {
		System.out.println("这是父类无参构造");
	}
	Father(int x) {
		this.x = x;
		System.out.println("这是父类有参构造");
	}
	void speak() {
		System.out.println("我是父亲");
	}                  
}
class Son extends Father {
	int y = 1;
	Son() {
		System.out.println("这是子类的无参构造");
	}
	Son(int y) {
		this.y = y + x;
		System.out.println("这是子类的有参构造");
	}
	void run() {
		super.speak(); // 访问父类的函数
		System.out.println("我是儿子");
	}
}
class Main {
	public static void main(String[] args) {
		Son s = new Son(3);
		System.out.println(s.y);// 4
	}
}
```
2.子类对象为什么可以访问父类的成员？
```java
this.y =y + x;
```
其中有一个隐式的super.x
3.super注意:
this和super很像，this指向的是当前对象的调用，super指向的是当前调用对象的父亲。Main类被加载，执行main方法，Son.class加载，发现有父类Father类，于是Father了也加载进内存。类加载完毕，创建对象，父类的构造方法会被调用（默认自动无参），然后执行子类相应构造创建了一个子类对象，该子类对象还包含了一个父类对象。该父类对象在子类对象内部。this super只能在有对象的前提下使用，不能在静态上下文使用。
4.子类显式调用父类构造函数：
在子类构造函数第一行通过super关键字调用父类任何构造函数。如果显式调用父类构造函数，编译器自动添加的调用父类无参数的构造函数就消失。构造函数间的调用只能放在第一行，只能调用一次。super()和this()不能同时存在构造函数的第一行。
```java
Son(int y) {
	super(y);// 子类显式调用父类构造函数
	this.y = y + x;
	System.out.println("这是子类的有参构造");
}
Son(int y) {
	this();  //不能同时存在构造函数第一行  
	super(y);
	this.y = y + x;
	System.out.println("这是子类的有参构造");
}
```
5.super思考：
如果开发者自定义了一个类，没有显示的进行类的继承，那么该类中成员函数是否可以使用super关键字？可以使用，继承了Object类，Object类是所有类的父类。
```java
class Main {
	public  void print(){
		System.out.println(super.toString());
	}
	public static void main(String[] args){
		new Main().print();
		System.out.println();
     }
}
```
三、重写（Override）
1.定义：在继承中，子类可以定义和父类相同的名称且参数列表一样的函数，将这种函数称之为函数的覆盖。
代码样例：
```java
class Father {
	String name;
	void eat() {
		System.out.println("吃窝窝");
	}
}
class Son extends Father {
	public void eat() { // 继承可以使得子类增强父类的方法
		System.out.println("来俩小菜");
		System.out.println("来两杯");
		System.out.println("吃香喝辣");
			System.out.println("来一根");
	}
}
class Main {
	public static void main(String[] args) {
		Son s = new Son();
        //执行子类的方法
		s.eat();
	}
}
```
2.细节要求：
①必须要有继承关系；
②函数名、参数列表必须相同；
③子类重写父亲的函数的时候，函数的访问权限必须大于等于父类的函数的访问权限，否则编译报错；
④子类重写父类的函数的时候，返回值类型必须是父类函数的返回值类型或该返回值类型的子类。不能返回比父类更大的数据类型：如子类函数返回值类型是Object；
3.特点：
①当子类重写了父类的函数，那么子类的对象如果调用该函数，一定调用的是重写过后的函数，可以通过super关键字进行父类的重写函数的调用；
②继承可以使得子类增强父类的方法；
4.子类对象查找属性或方法时的顺序：就近原则
如果子类的对象调用方法，默认先使用this进行查找，如果当前对象没有找到属性或方法，找当前对象中维护的super关键字指向的对象，如果还没有找到便会编译报错，找到的话直接调用。
5.重载和重写的区别：
①前提：所有的重载函数必须在同一个类中；而重写只要求是继承关系，可以在多个类中出现；
②特点：重载要求函数名相同，参数列表不同，与其他的无关（访问控制符、返回值类型）；而重写要求函数名相同、参数列表相同，子类返回值类型要等于或者小于父类的返回值；
6.重写的练习代码：
描述：定义动物类、定义狗继承动物重写父类吃和叫的方法、定义猫继承动物重写父类吃喝叫的方法；
样例代码：
```java
class Animal{
	int x=1;
	String name;
	void eat(){
		System.out.println("吃东西");
	}
	void shout(){
		System.out.println("我是动物");
	}
}
class Dog extends Animal{
	void eat(){
		System.out.println("啃骨头");
	}
	void shout(){
		System.out.println("旺旺");
	}
	void eat(String food){
		System.out.println("吃："+food);
	}
}
class Cat extends Animal{
	void eat(){
		System.out.println("吃老鼠");
	}
	void shout(){
		System.out.println("喵喵");
	}
}
class Main{
	public static void main(String[] args){
		Dog d=new Dog();
		d.shout();
		d.eat();
		Cat c=new Cat();
		c.shout();
		c.eat();
		System.out.println();
     }
}
```

