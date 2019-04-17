# 重拾C#教程：继承 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年04月10日 13:00:56[boonya](https://me.csdn.net/boonya)阅读数：69








原文地址：[http://www.runoob.com/csharp/csharp-inheritance.html](http://www.runoob.com/csharp/csharp-inheritance.html)




继承是面向对象程序设计中最重要的概念之一。继承允许我们根据一个类来定义另一个类，这使得创建和维护应用程序变得更容易。同时也有利于重用代码和节省开发时间。

当创建一个类时，程序员不需要完全重新编写新的数据成员和成员函数，只需要设计一个新的类，继承了已有的类的成员即可。这个已有的类被称为的**基类**，这个新的类被称为**派生类**。

继承的思想实现了 **属于（IS-A）** 关系。例如，哺乳动物 **属于（IS-A）** 动物，狗 **属于（IS-A）** 哺乳动物，因此狗 **属于（IS-A）** 动物。


## 基类和派生类

一个类可以派生自多个类或接口，这意味着它可以从多个基类或接口继承数据和函数。

C# 中创建派生类的语法如下：
<acess-specifier> class <base_class>
{
 ...
}
class <derived_class> : <base_class>
{
 ...
}
假设，有一个基类 Shape，它的派生类是 Rectangle：
usingSystem;namespaceInheritanceApplication{classShape{publicvoid setWidth(int w){
         width = w;}publicvoid setHeight(int h){
         height = h;}protectedint width;protectedint height;}// 派生类classRectangle:Shape{publicint getArea(){return(width * height);}}classRectangleTester{staticvoidMain(string[] args){RectangleRect=newRectangle();Rect.setWidth(5);Rect.setHeight(7);// 打印对象的面积Console.WriteLine("总面积： {0}",Rect.getArea());Console.ReadKey();}}}
当上面的代码被编译和执行时，它会产生下列结果：
总面积：35
## 基类的初始化

派生类继承了基类的成员变量和成员方法。因此父类对象应在子类对象创建之前被创建。您可以在成员初始化列表中进行父类的初始化。

下面的程序演示了这点：
usingSystem;namespaceRectangleApplication{classRectangle{// 成员变量protecteddouble length;protecteddouble width;publicRectangle(double l,double w){
         length = l;
         width = w;}publicdoubleGetArea(){return length * width;}publicvoidDisplay(){Console.WriteLine("长度： {0}", length);Console.WriteLine("宽度： {0}", width);Console.WriteLine("面积： {0}",GetArea());}}//end class Rectangle  classTabletop:Rectangle{privatedouble cost;publicTabletop(double l,double w):base(l, w){}publicdoubleGetCost(){double cost;
         cost =GetArea()*70;return cost;}publicvoidDisplay(){base.Display();Console.WriteLine("成本： {0}",GetCost());}}classExecuteRectangle{staticvoidMain(string[] args){Tabletop t =newTabletop(4.5,7.5);
         t.Display();Console.ReadLine();}}}
当上面的代码被编译和执行时，它会产生下列结果：
长度：4.5宽度：7.5面积：33.75成本：2362.5
## C# 多重继承

多重继承指的是一个类别可以同时从多于一个父类继承行为与特征的功能。与单一继承相对，单一继承指一个类别只可以继承自一个父类。

**C# 不支持多重继承**。但是，您可以使用接口来实现多重继承。下面的程序演示了这点：
usingSystem;namespaceInheritanceApplication{classShape{publicvoid setWidth(int w){
         width = w;}publicvoid setHeight(int h){
         height = h;}protectedint width;protectedint height;}// 基类 PaintCostpublicinterfacePaintCost{int getCost(int area);}// 派生类classRectangle:Shape,PaintCost{publicint getArea(){return(width * height);}publicint getCost(int area){return area *70;}}classRectangleTester{staticvoidMain(string[] args){RectangleRect=newRectangle();int area;Rect.setWidth(5);Rect.setHeight(7);
         area =Rect.getArea();// 打印对象的面积Console.WriteLine("总面积： {0}",Rect.getArea());Console.WriteLine("油漆总成本： ${0}",Rect.getCost(area));Console.ReadKey();}}}
当上面的代码被编译和执行时，它会产生下列结果：
总面积：35油漆总成本： $2450



