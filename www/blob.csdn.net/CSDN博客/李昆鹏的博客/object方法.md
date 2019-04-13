
# object方法 - 李昆鹏的博客 - CSDN博客


2018年03月07日 10:37:25[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：269


Object的方法
|boolean
|equals|(|Object|obj)
|指示其他某个对象是否与此对象“相等”。
|
public boolean equals(Object obj) {
return (this == obj);
}
---------------------------------------------------------------------------------------
class Student2 {
private String name;
private int age;
private int gender;
public void setName(String name){
this.name = name;
}
public String getName(){
return name;
}
public void setAge(int age){
this.age = age;
}
public int getAge(){
return age;
}
public void setGender(int gender){
this.gender = gender;
}
public int getGender(){
return gender;
}
**//equals****方法一般会进行重写**
**//instanceof****判断两个对象是否是同一种类型**
**publicboolean equals(Object obj) {**
**booleanresult = false;**
**//****判断****object****的实列的类型是否是****Student2**
**if (obj instanceof Student2){**
**Student2 student = (Student2)obj;**
**//****在本类中可以访问****private****的属性**
**if(this.name.equals(student.name)&&(this.age == student.age)&& (this.gender == student.gender)){**
**result = true;**
**}**
**}**
**return result;**
**}**
**/****
*******在重写****equals****方法时最好也重写****hashCode****方法**
***/**
**public int hashCode(){**
**return 1;**
**}**
}
classObjectDmeo1{
public static void main(String[] args){
Student2 s = new Student2();
Student2 s1 = new Student2();
//直接打印对象时是默认的在调用父类继承下来的toString
System.out.println(s);
System.out.println(s1);
//比较两个对象的地址
System.out.println(s == s1);
//判断对象是否相等不能使用"=="
//因为"=="是判断的两个对象的地址，两个对象的地址永远不能相等
//判断两个对象是否相等一般使用equals方法
//instanceof判断两个对象是否是同一种类型
s.setName("张三");
s.setAge(10);
s.setGender(1);
s1.setName("张三");
s1.setAge(10);
s1.setGender(1);
System.out.println("学生s和学生s1是否相等：" +s.equals(s1));
System.out.println("-------------------------------------");
boolean isEqual = s.equals(s1);
System.out.println(isEqual);
}
}
![](https://img-blog.csdn.net/20180307103507554?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
将s1的gender属性改为1和s的gender想等后输出结果为
s1.setGender(1);
![](https://img-blog.csdn.net/20180307103513392?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
---------------------------------instanceof关键字-------------------------------------------------------------
如果object是class的一个实例，则instanceof运算符返回true。如果object不是指定类的一个实例，或者object是null，则返回false。
## 例子
编辑
package com.instanceoftest;
interface A{ }
class B implements A{ }
class C extends B { }
class instanceoftest {
public static void main(String[] args){
A a=null;
B b=null;
boolean res;
System.out.println("instanceoftest test case 1:------------------");
res = a instanceof A;
System.out.println("a instanceof A: " + res);
res= b instanceof B;
System.out.println("binstanceof B: " + res);
System.out.println("\ninstanceoftest test case 2:------------------");
a=new B();
b=new B();
res = a instanceof A;
System.out.println("a instanceof A: " + res);
res = a instanceof B;
System.out.println("a instanceof B: " + res);
res = b instanceof A;
System.out.println("b instanceof A: " + res);
res = b instanceof B;
System.out.println("b instanceof B: " + res);
System.out.println("\ninstanceoftest test case 3:------------------");
B b2=new C();
res = b2 instanceof A;
System.out.println("b2 instanceof A: " + res);
res = b2 instanceof B;
System.out.println("b2 instanceof B: " + res);
res = b2 instanceof C;
System.out.println("b2 instanceof C: " + res);
}
}
/*result:
instanceoftest test case 1: ------------------
a instanceof A: false
b instanceof B: false
instanceoftest test case 2: ------------------
a instanceof A: true
a instanceof B: true
b instanceof A: true
b instanceof B: true
instanceoftest test case 3: ------------------
b2 instanceof A: true
b2 instanceof B: true
b2 instanceof C: true
--------------------------------Object中的finalize方法介绍---------------------------------------------------------------------
### finalize
protected void**finalize**()throwsThrowable当垃圾回收器确定不存在对该对象的更多引用时，由对象的垃圾回收器调用此方法。子类重写`finalize`方法，以配置系统资源或执行其他清除。
finalize的常规协定是：当 JavaTM虚拟机已确定尚未终止的任何线程无法再通过任何方法访问此对象时，将调用此方法，除非由于准备终止的其他某个对象或类的终结操作执行了某个操作。finalize方法可以采取任何操作，其中包括再次使此对象对其他线程可用；不过，finalize的主要目的是在不可撤消地丢弃对象之前执行清除操作。例如，表示输入/输出连接的对象的 finalize 方法可执行显式 I/O 事务，以便在永久丢弃对象之前中断连接。
Object类的finalize方法执行非特殊性操作；它仅执行一些常规返回。Object的子类可以重写此定义。
Java 编程语言不保证哪个线程将调用某个给定对象的finalize方法。但可以保证在调用 finalize 时，调用 finalize 的线程将不会持有任何用户可见的同步锁定。如果 finalize 方法抛出未捕获的异常，那么该异常将被忽略，并且该对象的终结操作将终止。
在启用某个对象的finalize方法后，将不会执行进一步操作，直到 Java 虚拟机再次确定尚未终止的任何线程无法再通过任何方法访问此对象，其中包括由准备终止的其他对象或类执行的可能操作，在执行该操作时，对象可能被丢弃。
对于任何给定对象，Java 虚拟机最多只调用一次finalize方法。
`finalize`方法抛出的任何异常都会导致此对象的终结操作停止，但可以通过其他方法忽略它。
**抛出：**
`Throwable`- 此方法抛出的`Exception`
--------------------------------------
Class A{
}
Class B extendsA{
}
A a = new B();
![](https://img-blog.csdn.net/20180307103607661?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQxNTQ3NDg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
------------------------------------------
说一下
Final, finally , finalize的区别
**1.Final****关键字可以修饰属性、可以修饰方法、可以修饰类。**
**在修饰属性时值时不能被修改的。**
**在修饰方法时方法不能被重写。**
**在修饰类的时候类不能被继承。**
Final用于修饰类、成员变量和成员方法。final修饰的类，不能被继承（String、StringBuilder、StringBuffer、Math，不可变类），其中所有的方法都不能被重写，所以不能同时用abstract和final修饰类（abstract修饰的类是抽象类，抽象类是用于被子类继承的，和final起相反的作用）；Final修饰的方法不能被重写，但是子类可以用父类中final修饰的方法；Final修饰的成员变量是不可变的，如果成员变量是基本数据类型，初始化之后成员变量的值不能被改变，如果成员变量是引用类型，那么它只能指向初始化时指向的那个对象，不能再指向别的对象，但是对象当中的内容是允许改变的。
方法内声明的类或者方法内的匿名内部类，访问该方法内定义的变量，该变量必须要用final修饰。当内部类访问局部变量时，会扩大局部变量的作用域，如果局部变量不用 final 修饰，我们就可以在内部类中**随意修改**该局部变量值，而且是在该局部变量的作用域范围之外可以看到这些修改后的值，会出现安全问题。
------------
**2.Finalize****（法了兹）是用于作垃圾回收的一个方法，当一个对象在堆中没有任何引用的时候，会自动的被****jvm****自动调用****finalize****方法回收这个对象。**
Finalize是object类中的一个方法，子类可以重写finalize()方法实现对资源的回收。垃圾回收只负责回收内存，并不负责资源的回收，资源回收要由程序员完成，Java虚拟机在垃圾回收之前会先调用垃圾对象的finalize方法用于使对象释放资源（如关闭连接、关闭文件），之后才进行垃圾回收，这个方法一般不会显示的调用，在垃圾回收时垃圾回收器会主动调用。
**3.Finally****通常和try catch****搭配使用，保证不管有没有发生异常，资源都能够被释放（释放连接、关闭IO流）。**
finally是在异常处理时提供finally块来执行任何清除操作。不管有没有异常被抛出、捕获，finally块都会被执行。try块中的内容是在无异常时执行到结束。catch块中的内容，是在try块内容发生catch所声明的异常时，跳转到catch块中执行。finally块则是无论异常是否发生，都会执行finally块的内容，所以在代码逻辑中有需要无论发生什么都必须执行的代码，就可以放在finally块中。


