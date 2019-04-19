# C#继承和多态 - Big Smile - CSDN博客
2015年10月11日 16:59:21[王啸tr1912](https://me.csdn.net/tr1912)阅读数：1261
所属专栏：[C#学习基础](https://blog.csdn.net/column/details/csharp-wx.html)
        在C#的面向对象的大家庭里，我感觉继承算是很重要的了，因为有很多的东西都是依赖于继承和多态的，所以单独的在第三篇博客里做总结和升华。
        继承在面相对象的地方是一个很是重要的东西，可以说在面向对象的世界里没有了继承就没有了这么多的东西，而没有了多态就没有了多彩的世界。
# 一、继承
        类的继承是最为普遍的事情了，可是类又有很多类型可分，比如抽象类，用new声明的类等等，他们的基本继承方法是一样的，都是用的“：”来继承的，只不过是父类中的方法重写规则是不一样的，下面来分情况概述：
        C#中子类重写父类方法的几种情况，关键字：virtual、abstract、override、new
        1）、virtual：标识可能但不是必须被子类重写的方法，父类必须给出默认实现，子类可以重写（使用override,new,或无特殊标识的普通方法），也可以不重写该方法。
        2）、abstract：标识必须被子类重写的方法，父类不给出实现，子类必须用override关键字重写该方法。
        3）、override：标识重写父类的方法，父类方法必须是用abstract，virtual，override之一声明，运行时将根据实例的类型而不是引用的类型调用对象的方法。
        4）、new：标识重写父类的方法，父类方法可以用virtual、override、new之一声明，也可以是没有特殊标识的普通方法，运行时会根据引 用的类型选择调用父类还是子类的方法，重写父类方法时，使用new关键字与使用没有特殊标识的普通方法是等效的，但是后者会给出一个编译警告。
        综上，在非抽象类和隐藏继承的new下的类的重写都要用virtual来声明方法。继承举例：
```
<span style="font-size:18px;">class Basic
{     
     public virtual void basement()   
    {        
        console.wirteline("Basic");   
    }
}
class  Derived :Basic             //继承Basic
{    
      public override void basement()        //改写Basement方法
     {          
          console.writeline("show!")   
     }
}</span>
```
        （2）、接口的继承
        和类的继承一样，接口也可以继承。接口可以从一个或者多个接口中继承，而类只能从一个类中继承，如果从多个接口中继承，用“:”跟被继承接口名，被继承接口名之间用“,”隔开，如：
```
interface  A
{
      void  Paint();
}
interface  B:A               //B继承于A
{
      void  SetText(string text);
}
interface  C:A              //C继承于A
{
      void  SetItems(string() Items);
}
interface  D:B,C           //D继承于B，C
{
}
```
        接口D继承了A，B，C的三个接口的特性，可见接口的继承具有传递性。
# 二、多态
        多态是由继承来实现的，所以他体现了继承的性质，但是和继承略有不同，多态是同一操作作用于不同对象可以有不同的解释，产生不同的结果，在继承方面多态通过以自类复写父类中的虚函数型的方法来实现的。多态又分成两种，一种是编译时的多态，一种是运行是的多态。
（1）重载和复写的区别
        1）、重载是指类中定义方法的不同版本，举例：
         public  int  calculate (int x  ,int  y)
         public  double  calculate (double  x, double  y)
     在使用中可以视不同的情况而决定用哪个方法。
     两相同（名称，返回值（可以不同）），参数必须不同
```
public class Test                      //不同的方法
{
    public  int  calculate  (int x,int y)
   {
       return  x+y; 
    }
    public   double  calculate (double  x,double  y)
   {
       return  x+y;
    }
}
```
      2）、复写是子类为了满足自己的需求来对方法的重写
     要求：通过override来进行重写，只有虚方法和抽象对象才能重写，三相同（名称，参数，返回值）
demo:
```
public  class shape()
{
     public  const  double  pi=3.1415926;
     protected  double  x,y;
     public  shape()
     {
      }
      public  shape(double  x,double  y)
     {
           this.x=x;
           this.y=y;
     }
     public  virtual  double  getarea()
     {
           return  x*y;
     }
}
public  class circle:shape
{
      public  circle  (double  r) :base(r,0)
      {
      }
      public  override double  getarea()              //重写方法
     {
         return  pi*x*x;
     }
}
public  class  PloTest
{
     public  static  void Main()
     {
          double  r=6.0;
          circle  c=new  circle(r);
          console.writeline("圆的面积是：{0}",c.getarea);           //调用多态的方法
     }
}
```
# 三、总结
关于继承和多态就先总结这么多，其中的例子都是很好的例子，在此不做过多的陈述了。继承和多态总是要在一起用的，其中还包括了接口的实现，又有隐式方法的实现和隐式接口的实现，东西很多，重在理解和使用。
