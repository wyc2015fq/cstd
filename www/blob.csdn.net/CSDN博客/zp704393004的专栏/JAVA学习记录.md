# JAVA学习记录 - zp704393004的专栏 - CSDN博客





2018年09月10日 21:57:56[原来未知](https://me.csdn.net/zp704393004)阅读数：58








在菜鸟学习里面的java学习中，finalize() 方法是如何回收代码的，比如为什么：

public class FinalizationDemo

{

public static void main(String[] args)

{

Cake c1 = new Cake(1);

Cake c2 = new Cake(2);

Cake c3 = new Cake(3);

c2 = c3 = null;

System.gc(); //调用Java垃圾收集器

}

}

class Cake extends Object

{

private int id;

public Cake(int id)

{ this.id = id; System.out.println("Cake Object " + id + "is created"); }

protected void finalize() throws java.lang.Throwable

{ super.finalize(); System.out.println("Cake Object " + id + "is disposed"); } }

运行以上代码，输出结果如下：

$ javac FinalizationDemo.java 
$ java FinalizationDemo
Cake Object 1is created
Cake Object 2is created
Cake Object 3is created
Cake Object 3is disposed
Cake Object 2is disposed


