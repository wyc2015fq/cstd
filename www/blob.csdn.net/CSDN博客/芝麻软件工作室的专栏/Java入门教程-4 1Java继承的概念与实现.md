
# Java入门教程-4.1Java继承的概念与实现 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月25日 08:50:22[seven-soft](https://me.csdn.net/softn)阅读数：119个人分类：[Java																](https://blog.csdn.net/softn/article/category/6242590)



继承是类与类之间的关系，是一个很简单很直观的概念，与现实世界中的继承（例如儿子继承父亲财产）类似。
继承可以理解为一个类从另一个类获取方法和属性的过程。如果类B继承于类A，那么B就拥有A的方法和属性。
继承使用 extends 关键字。
例如我们已经定义了一个类 People：classPeople{
Stringname;
intage;
intheight;
voidsay(){
System.out.println("我的名字是 "+name+"，年龄是 "+age+"，身高是 "+height);
}
}

如果现在需要定义一个类 Teacher，它也有 name、age、height 属性和 say() 方法，另外还需要增加 school、seniority、subject 属性和 lecturing() 方法，怎么办呢？我们要重新定义一个类吗？
完全没必要，可以先继承 People 类的成员，再增加自己的成员即可，例如：classTeacherextendsPeople{
Stringschool;// 所在学校
Stringsubject;// 学科
intseniority;// 教龄
// 覆盖 People 类中的 say() 方法
voidsay(){
System.out.println("我叫"+name+"，在"+school+"教"+subject+"，有"+seniority+"年教龄");
}
voidlecturing(){
System.out.println("我已经"+age+"岁了，依然站在讲台上讲课");
}
}

对程序的说明name 和 age 变量虽然没有在 Teacher 中定义，但是已在 People 中定义，可以直接拿来用。
Teacher 是 People 的子类，People 是Teacher 类的父类。
子类可以覆盖父类的方法。
子类可以继承父类除private以为的所有的成员。
构造方法不能被继承。
继承是在维护和可靠性方面的一个伟大进步。如果在 People 类中进行修改，那么 Teacher 类就会自动修改，而不需要程序员做任何工作，除了对它进行编译。
单继承性：Java 允许一个类仅能继承一个其它类，即一个类只能有一个父类，这个限制被称做单继承性。后面将会学到接口(interface)的概念，接口允许多继承。
最后对上面的代码进行整理：publicclassDemo{
publicstaticvoidmain(String[]args){
Teachert=newTeacher();
t.name="小布";
t.age=70;
t.school="清华大学";
t.subject="Java";
t.seniority=12;
t.say();
t.lecturing();
}
}
classPeople{
Stringname;
intage;
intheight;
voidsay(){
System.out.println("我的名字是 "+name+"，年龄是 "+age+"，身高是 "+height);
}
}
classTeacherextendsPeople{
Stringschool;// 所在学校
Stringsubject;// 学科
intseniority;// 教龄
// 覆盖 People 类中的 say() 方法
voidsay(){
System.out.println("我叫"+name+"，在"+school+"教"+subject+"，有"+seniority+"年教龄");
}
voidlecturing(){
System.out.println("我已经"+age+"岁了，依然站在讲台上讲课");
}
}

运行结果：
我叫小布，在清华大学教Java，有12年教龄
我已经70岁了，依然站在讲台上讲课
注意：构造方法不能被继承，掌握这一点很重要。 一个类能得到构造方法，只有两个办法：编写构造方法，或者根本没有构造方法，类有一个默认的构造方法。

