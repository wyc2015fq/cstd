
# java入门教程-4.5Java instanceof 运算符 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月25日 08:54:50[seven-soft](https://me.csdn.net/softn)阅读数：117个人分类：[Java																](https://blog.csdn.net/softn/article/category/6242590)



多态性带来了一个问题，就是如何判断一个变量所实际引用的对象的类型 。 C++使用runtime-type information(RTTI)，Java 使用 instanceof 操作符。
instanceof 运算符用来判断一个变量所引用的对象的实际类型，注意是它引用的对象的类型，不是变量的类型。请看下面的代码：publicfinalclassDemo{
publicstaticvoidmain(String[]args){
// 引用 People 类的实例
Peopleobj=newPeople();
if(objinstanceofObject){
System.out.println("我是一个对象");
}
if(objinstanceofPeople){
System.out.println("我是人类");
}
if(objinstanceofTeacher){
System.out.println("我是一名教师");
}
if(objinstanceofPresident){
System.out.println("我是校长");
}
System.out.println("-----------");// 分界线
// 引用 Teacher 类的实例
obj=newTeacher();
if(objinstanceofObject){
System.out.println("我是一个对象");
}
if(objinstanceofPeople){
System.out.println("我是人类");
}
if(objinstanceofTeacher){
System.out.println("我是一名教师");
}
if(objinstanceofPresident){
System.out.println("我是校长");
}
}
}
classPeople{}
classTeacherextendsPeople{}
classPresidentextendsTeacher{}

运行结果：
我是一个对象
我是人类
-----------
我是一个对象
我是人类
我是一名教师
可以看出，如果变量引用的是当前类或它的子类的实例，instanceof 返回 true，否则返回 false。

