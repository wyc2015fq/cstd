# Java 基础----内部类 - 爱拍凯特的专栏 - CSDN博客
2016年07月25日 23:18:02[爱拍凯特](https://me.csdn.net/apacat)阅读数：206
个人分类：[Java](https://blog.csdn.net/apacat/article/category/1299665)
今天在复习Java的相关知识的时候遇到了内部类这个概念，小小的总结一下，下面就用一句话来总结Java中的内部类：
Java 中内部类包括四种：静态内部类，成员内部类，局部内部类，匿名内部类
那么问题来了：如何区分这几种类呢？
很简单：
1、静态内部类定义在类中，用static关键字修饰，并且只能访问外部类（相对于内部类，自然有个外部类）的静态成员和静态方法。
2、成员内部类定义在类中，和静态内部类的区别就在于么有static修饰，能够访问外部类的全部的属性和方法，不论是否有static修饰。
3、局部内部类定义在类中的方法中，只能访问外部类的final的成员变量和成员方法。
4、匿名内部类定义在表达式中（我是这样认为的），匿名内部类没有类名，没有class关键字也没有extends和implements等关键字修饰，一般情况**会隐式地继承一个父类或实现一个接口。**
**以下是实例：**
**静态内部类：**
**public class Test{private static int id = 1;private String name = "yxq";public String getName(){return this.name;}static class static_class{private static int static_id = 0;public void execute(){System.out.println(static_id+" : "+Test.id);}}**
**｝**
**成员内部类：**
**public class Test{private static int id = 1;private String name = "yxq";public String getName(){return this.name;}class inner_class{private String name = "inner_class";public void execute(){System.out.println(name);}}**
**｝**
**局部内部类：（没有写外部类Test()）**
**        public void localClassMethod(){final int local = 2;class local{private String local_name = "local";public void out(){System.out.println(local+":"+age+":"+local_name);}}new local().out();}**
**匿名内部类：（这个从网上抄了一个没有自己写）**
button.addActionListener(new ActionListener(){
//实现接口中的方法        @Override
publicvoid actionPerformed(ActionEvent e) {
             System.out.println("使用匿名内部类实现ActionListener接口");
        }
    });
**好了，这样概念就介绍完了，问题又来了那就是在外部类的main函数中该如何使用这些类呢？**
**我们继续：**
**使用上面的例子，下面是以上几个类的使用方法：**
** public static void main(String[] args){**
**                //静态内部类Test.static_class ts = new Test.static_class();ts.execute();//成员内部类Test.inner_class ti = new Test().new inner_class();ti.execute();//局部内部类的使用在方法中，上面的例子中已经提到了。//匿名内部类t.localClassMethod();}**
