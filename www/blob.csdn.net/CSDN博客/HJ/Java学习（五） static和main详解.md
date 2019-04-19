# Java学习（五） static和main详解 - HJ - CSDN博客
2018年02月08日 13:22:20[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：345
所属专栏：[Java学习](https://blog.csdn.net/column/details/19570.html)
一、static 静态修饰符
1.static特点：
①静态会随着类的加载而加载，随着类的消失而消失。它的生命周期和很长；
②优先于对象存在。静态是先存在，对象是后存在；
③被所有实例（对象）所共享；
④可以直接被类名调用；
2.静态变量（类变量）和实例变量的区别：
①存放位置：类变量随着类的加载而加载，存在于方法区中；而实例变量随着对象的建立而存在于堆内存中；
②生命周期：类的变量生命周期最长，随着类的消失而消失；实例变量生命周期随着对象的消失而消失；
3.static修饰成员变量：如果有数据需要被共享给所有对象使用时，那么就可以使用static修饰。静态成员变量的访问方式有两种：
①使用对象进行访问，如对象名.变量名；
②使用类名进行访问，如类名.变量名；
注意：
（1）非静态的成员只能使用对象进行访问，不能使用类名进行访问；
（2）千万不要为了方便访问数据而使用static修饰成员变量，只有成员变量的的数据是真正需要被共享的时候才使用static 修饰；
4.static修饰成员变量的应用情况
如果一个数据需要被所有对象共享使用的时候，这时候即可使用static修饰
如下示例代码描述：
```java
classStudent{
   static Stringname;  //非静态成员变量
   static String  country = "中国";   //静态的成员变量
   public Student(String name){
      this.name = name;
   }
}
class Main{
   public static void main(String[] args)
   {
      Student s1 = new Student("狗娃");
      Student s2 = new Student("狗剩");
      //System.out.println("国籍："+ Student.country);
      System.out.println("名字："+ s1.name);
      System.out.println("名字："+ s2.name);
   }
}
```
练习：统计一个类使用了多少次创建对象，该类对外显示被创建的次数。
```java
classEmp{
   //非静态的成员变量。
   static intcount = 0;  //计数器
   String name;
   //构造代码块
   {
      count++;
   }
   public Emp(String name){
      this.name = name;
   }
   public Emp(){ //每创建一个对象的时候都会执行这里 的代码
 
   }
   public void showCount(){
      System.out.println("创建了"+ count+"个对象");
   }
}
class Main
{
   public static void main(String[] args)
   {
      Emp e1 = new Emp();
      Emp e2 = new Emp();
      Emp e3 =new Emp();
      e3.showCount();
   }
}
```
5.静态函数
注意：
（1）静态函数中不能访问非静态成员变量，只能访问静态变量；
（2）静态方法不可以定义super关键字；
（3）因为静态优先于对象存在，静态方法中不可以出现this；
另：静态函数中不能使用非静态变量，但非静态函数可以访问静态变量。
特点：静态代码块随着类的加载而加载，且只执行一次，优先于主函数。用于给类进行初始化。
二、main方法详解
定义：主函数是一个特殊的函数，作为程序的入口，它是静态的，可以被jvm识别
```java
public static void main(String[] args){
}
```
①public:代表该函数的访问权限是最大的；
②static：代表主函数随着类的加载，就已经存在了；
③void：主函数没有具体的返回值；
④main：不是关键字，是一个特殊的单词可以被jvm识别；
⑤(String[] args)：函数的参数，参数类型是一个数组，该数组中的元素是字符串。字符串类型的数组；args是arguments的缩写，担心某些程序启动需要参数；
