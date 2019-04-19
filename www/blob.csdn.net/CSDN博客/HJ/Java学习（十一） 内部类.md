# Java学习（十一） 内部类 - HJ - CSDN博客
2018年03月08日 12:26:10[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：106
个人分类：[Java](https://blog.csdn.net/feizaoSYUACM/article/category/7349915)
所属专栏：[Java学习](https://blog.csdn.net/column/details/19570.html)
一、内部类前言
1. 概述
（1）将类定义在另一个类的内部则成为内部类，其实就是类定义的位置发生了变化；
（2）在一个类中，定义在类中的叫成员变量，定义在函数中的叫成员函数，那么根据类定义的位置也可以分为成员内部了和局部内部类；
（3）内部类生成的class文件为“外部类$内部类”，为了表明该内部类是属于具体哪个外部类的；
2. 内部类的优势
首先我们需要明白一个问题：什么时候使用内部类呢？当我们分析事物时，发现事物的内部还有具体的事物，这时则应该定义内部类了；
其优势就在于成员内部类作为外部类的成员，可以访问外部类的任意成员；
3. 内部类代码样例：
```java
class Outer {
	int num = 10;
	
	class Inner {
		int age = 10;
		public void print() {
			System.out.println("内部类")；
		}
	}
}
```
二、成员内部类
1.  成员内部类的访问方式
（1）内部类可以直接访问外部类的成员属性；
（2）外部类需要访问内部类的成员属性时需要创建内部类的对象：
         ①在外部类的成员函数中创建内部类的对象，通过内部类对象直接访问内部类的成员；
         ②在其他类中之间创建内部类的对象：
             Outer.Inner innner = new Outer().new Inner();
2.  成员内部访问特点：
（1）私有的成员内部类不能在其他类中直接创建内部类对象来访问；
（2）如果内部类中包含有静态成员，那么Java规定内部类必须声明为静态的访问静态内部类的形式：Outer.Inner in = new Outer.Inner();
三、局部内部类
包含在外部类的函数中的内部类称之为局部内部类。可以在包含局部内部类的方法中直接创建局部内部类的对象调用局部内部类的成员。；
注意：局部内部类只能访问所在函数的final属性；
```java
class Outer {
	int num = 4;
	
	class MyIn {
	}
	public void method(final int y) {
		final int x = 3;
		class Inner {
			void show() {
				System.out.println("show run -" + Outer.this.num);
				System.out.println("x=" + x);
				System.out.println("y=" + y);
			}
		}
		new Inner().show();
	}
}
class Main {
	public static void main(String[] args) {
		new Outer().method(5);
	}
}
```
四、匿名内部类
匿名内部类就是没有名字的类，它能简化内部类的数学，它的前提是必须继承一个父类或者是实现一个接口；
匿名内部类的格式：
```java
//new 父类或者接口()  {
//
//}
//代码样例：
class Outer{
	class Inner
	{
		public void show(){
			System.out.println("内部类的show方法");
		}
	}
	public void print(){
		new Inner().show();
	}
}
//匿名内部类调用show方法
abstract class Inner{
    abstract void show();
}
class Outer{
    public void print(){
        new Inner(){
            public void show(){
                System.out.println(“内部类的show方法”);
            }
        }.show();
    }
}
```
注意：使用匿名内部类时，如果需要调用匿名内部类的两个方法或者两个方法以上，可以使用变量指向该对象；
