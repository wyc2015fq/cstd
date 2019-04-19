# C#——委托和事件 - Big Smile - CSDN博客
2015年10月30日 21:01:06[王啸tr1912](https://me.csdn.net/tr1912)阅读数：914标签：[面向对象																[C#																[委托](https://so.csdn.net/so/search/s.do?q=委托&t=blog)](https://so.csdn.net/so/search/s.do?q=C#&t=blog)](https://so.csdn.net/so/search/s.do?q=面向对象&t=blog)
个人分类：[编码学习																[C#](https://blog.csdn.net/tr1912/article/category/5855387)](https://blog.csdn.net/tr1912/article/category/5787481)
所属专栏：[C#学习基础](https://blog.csdn.net/column/details/csharp-wx.html)
# 一、委托
        说道委托，很多的教材上都说是类似于C语言上的指针，但是以我理解的程度来看，更像是复制这个方法或函数的使用，因为委托可以看成是一个数据类型一样的东西，传递进去的是函数或方法。但是这在我看来更像是植物大战僵尸中的克隆者，因为使用的方法和原来的函数没有什么两样，再者说就是可以少写一个对象名，另一方面说道委托就是程序委托给这个函数去完成某个事件，是一个中间者的境地。（Ps：从另一方面更像是数据库中对表或者方法的重命名）
        接下来让我们来看一下委托的简单实现：
    由于委托是委托的方法，所以要先定义一个方法：
```
public class MathOpt
{
       public  int Add (int argument1,int argument2)     //一个相加的方法
      {
           rerturn  argument1+argument2; 
      }
}
```
    然后是定义委托 ：
`public delegate int MathOptDelegate(int value1,int value2);     //定义委托`  （注意委托的关键字是delegate）
    然后是使用：
```
static  void Main(string[] args)
{
       MathOptDelegate  oppDel;              //声明委托变量
       MathOpt  obj = new MathOpt();        //声明函数对象
       
       oppDel = obj.Add;                    //给委托变量附上对象的方法
       Console.WriteLine(oppDel(1,2));      //使用(传递的参数和被委托的方法一样)
}
```
    最后输出的是3，由此证明了传进委托对象的函数确实执行了对象obj的add方法。
（注意声明委托的类型要和方法返回的值的类型一致）
**多重委托：**
多重委托实际上也是通过委托和委托之间的叠加来实现的，但是要求是参数必须相同而且返回值为空。多重委托最常用的两个方法是Combine()和Remove()，实际使用的时候用"+、-"代替。
例子：
```
delegate void Dg(string str);      //定义委托
class A                            //定义类
{
     public  static void H(string str)
    {
          console.writeline("hello,{0}",str);
    }
    public  static  void G(string  str)
    {
          console.writeline("Goodbye,{0}",str);
    }
}
class  Myclass
{
    public static Main(string[] args)
    { Dg  a,b,c,d;                      //声明委托对象
      a=new  Dg(A.H);                   //委托a指向A的H方法
      b=new  Dg(A.G);                   //委托b指向A的G方法
      c=a+b;                            //创建多重委托
      d=c-a;                            //把a从多重委托中移除
      console.writeline("Invoke  delegate  a:");
      a("A");
      console.writeline("Invoke  delegate  b:");
      b("B");
      console.writeline("Invoke  delegate  c:");
      c("C");
      console.writeline("Invoke  delegate  d:");
      d("D");
    }
}
```
运行结果：
 Invoke  delegate  a:
  hello,A
Invoke  delegate  b:
  Goodbye,B
Invoke  delegate  c:
  hello,C
  Goodbye,C
Invoke  delegate  d:
  Goodbye,D
# 二、事件
        我们都知道在面向对象的程序设计中，消息的传递有的时候是程序出发的根本，所以有些功能的执行就需要消息去通知他们，在这里我们用事件的触发来传递消息实现功能的执行。
        事件的声明格式（举例）；
       public delegate void a;
       public event a Click;
其中event后面跟的必须是一个委托，委托后面是执行事件。所以有事件必有委托。
**事件的预定和撤销：**
        事件需要预定才能被触发，而已经预定了的事件也可以被撤销，预定和撤销的操作符分别是”+=“和”-=“。
使用：OkButton.Click=new EventHandler(OkButtonClick);
有的时候感觉以前vb中用的句柄函数可能也是通过这种方式实现的。（EventHandler感觉有句柄的意思）
        由于视频中没有提到多少事件的事情，所以也只能总结到此。
# 三、总结
        委托和事件在实际的应用上有很多的关联，所以要放在一起学习，有很多巧妙的例子都是用事件写成的。委托可以看作是C#中的指针，所以也需要重要的把控一下。
