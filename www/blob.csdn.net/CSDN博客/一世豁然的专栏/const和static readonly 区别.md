# const和static readonly 区别 - 一世豁然的专栏 - CSDN博客





2015年04月14日 09:24:50[一世豁然](https://me.csdn.net/Explorer_day)阅读数：581








本博客转载于：[http://www.cnblogs.com/cuihongyu3503319/archive/2009/02/23/1396653.html](http://www.cnblogs.com/cuihongyu3503319/archive/2009/02/23/1396653.html)







我们都知道，const和static readonly的确很像：通过类名而不是对象名进行访问，在程序中只读等等。

在多数情况下可以混用。

二者本质的区别在于，const的值是在编译期间确定的，因此只能在声明时通过常量表达式指定其值。而

static readonly是在运行时计算出其值的，所以还可以通过静态构造函数来赋值。

明白了这个本质区别，我们就不难看出下面的语句中static readonly和const能否互换了：

1. static readonly MyClass myins = new MyClass();

2. static readonly MyClass myins = null;

3. static readonly A = B * 20;

   static readonly B = 10;

4. static readonly int [] constIntArray = new int[] {1, 2, 3};

5. void SomeFunction()

    {

      const int a = 10;

       ...

    }

1：不可以换成const。new操作符是需要执行构造函数的，所以无法在编译期间确定

2：可以换成const。我们也看到，Reference类型的常量（除了String）只能是Null。

3：可以换成const。我们可以在编译期间很明确的说，A等于200。

4：不可以换成const。道理和1是一样的，虽然看起来1,2,3的数组的确就是一个常量。

5：不可以换成readonly，readonly只能用来修饰类的field，不能修饰局部变量，也不能修饰property等

其他类成员。

因此，对于那些本质上应该是常量，但是却无法使用const来声明的地方，可以使用static readonly。例

如C#规范中给出的例子：



public class Color

{

    public static readonly Color Black = new Color(0, 0, 0);

    public static readonly Color White = new Color(255, 255, 255);

    public static readonly Color Red = new Color(255, 0, 0);

    public static readonly Color Green = new Color(0, 255, 0);

    public static readonly Color Blue = new Color(0, 0, 255);static readonly需要注意的一个问

题是，对于一个static readonly的Reference类型，只是被限定不能进行赋值（写）操作而已。而对其成

员的读写仍然是不受限制的。 

public static readonly MyClass myins = new MyClass();

…

myins.SomeProperty = 10;  //正常

myins = new MyClass();    //出错，该对象是只读的

但是，如果上例中的MyClass不是一个class而是一个struct，那么后面的两个语句就都会出错。 


    private byte red, green, blue;

    public Color(byte r, byte g, byte b)

     {

         red = r;

         green = g;

         blue = b;

     }

}



在通俗一点说,const类型赋值必须是脱离系统运行时才能初始化的值(const page p=null正确,const page p= new Page()错误,因为 new Page()需要运行时才初始化)可以使用static readonly(static readonly page p=
 new Page())




