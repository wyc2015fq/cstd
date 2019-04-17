# C#系列  ---5 函数参数 optional , ref, out, params 和 参数值传递和引用传递问题 - 墨流觞的博客 - CSDN博客





2018年10月31日 22:32:20[墨氲](https://me.csdn.net/dss_dssssd)阅读数：93








## variables and parameters

variable 代表的是一个内存地址，该地址包含一个可变的值。可以是`local variable, parameter (value, ref, or out), field (instance or static), or array element`

#### 栈和堆（stack and heap）

栈（stack）：存储局部变量和参数（local variables and parameters）的内存块，当函数压栈或出栈时，栈在逻辑上会增加或缩减。**典型的递归函数的分析**

堆（heap）:
- 存储着对象(object),例如引用类型对象的实例。每当新的对象    创建，将分配到堆上，并返回对象的引用。运行时（runtime）    会有一个垃圾收集器，周期性的从堆上删除不再使用的对象，     从而保证程序不会因为内存不够而崩溃。
- 存储static field（静态字段），不会被垃圾收集器收集，直到程序结束或崩溃，静态字段的变量才会消失。**静态变量的概念在C++中也有**

## Definite Assignment 显示赋值

C#强制显示赋值。这样将无法使用未初始化的变量，避免程序错误

主要表现在：
- 局部变量在使用前一定要初始化
- 函数在被调用时，函数参数必须全部传入（除了可选参数）
- 其他的变量（比如字段和数组元素（fields and array elements））由运行时（runtime）自动初始化

**A field is a variable of any type that is declared directly in a class or struct.**

在类或结构体里直接声明的变量成员，除此之外也包括静态变量

```
public class CalendarEntry
{
    // private field
    private DateTime date;

    // public field (Generally not recommended.)
    public string day;

    // Public property exposes date field safely.
    public DateTime Date 
    {
        get 
        {
            return date;
        }
        set 
        {
            // Set some reasonable boundaries for likely birth dates.
            if (value.Year > 1900 && value.Year <= DateTime.Today.Year)
            {
                date = value;
            }
            else
                throw new ArgumentOutOfRangeException();
        }

    }

    // Public method also exposes date field safely.
    // Example call: birthday.SetDate("1975, 6, 30");
    public void SetDate(string dateString)
    {
        DateTime dt = Convert.ToDateTime(dateString);

        // Set some reasonable boundaries for likely birth dates.
        if (dt.Year > 1900 && dt.Year <= DateTime.Today.Year)
        {
            date = dt;
        }
        else
            throw new ArgumentOutOfRangeException();
    }

    
}
```

代码实例：

```
//使用未初始化的局部变量，出错
static void Main()
{
int x;
Console.WriteLine (x); // Compile-time error
}
//数组元素
static void Main()
{
int[] ints = new int[2];
Console.WriteLine (ints[0]); // 0
}
//静态变量， 字段
class Test
{
static int x;
static void Main() { Console.WriteLine (x); } // 0
}
```

#### 默认值 Default Values

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181031222812681.png)

### 参数 parameters

#### 参数传递
- 值传递： 在C#中，参数默认按值传递，即当传给函数时，传入的是参数的拷贝值

```
class Test
{
static void Foo (int p)
{
p = p + 1; // Increment p by 1
Console.WriteLine (p); // Write p to screen
}
static void Main()
{
int x = 8;
Foo (x); // Make a copy of x
Console.WriteLine (x); // x will still be 8
}
}
```

对于引用类型，将引用赋值，而非引用指向的对象。

```
class Test
{
static void Foo (StringBuilder fooSB)
{
    fooSB.Append ("test");
    fooSB = null;
}
static void Main()
{
    StringBuilder sb = new StringBuilder();
    Foo (sb);
    Console.WriteLine (sb.ToString()); // test
}
}
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181031222827877.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

结合上图将不难理解，传入的fooSb其实相当于ref2，将ref1进行拷贝，也就是ref2和ref1指向同一位置，修改ref2指向对象的内容同样修改ref1的内容，但是将ref2重新指向null，并不会改变ref1的指向。

要是不想传入拷贝值，而是传入原始值呢？

使用引用传递，使用关键字：**`ref`**

```
class Test
{
static void Foo (ref int p)
{
    p = p + 1; // Increment p by 1
    Console.WriteLine (p); // Write p to screen
}
static void Main()
{
    int x = 8;
    Foo (ref x); // Ask Foo to deal directly with x
    Console.WriteLine (x); // x is now 9
}
}
```

使用ref实现交换函数：

```
class Test
{
static void Swap (ref string a, ref string b)
{
    string temp = a;
    a = b;
    b = temp;
}
static void Main()
{
    string x = "Penn";
    string y = "Teller";
    Swap (ref x, ref y);
    Console.WriteLine (x); // Teller
    Console.WriteLine (y); // Penn
}
}
```

### 关键字 out

也是用在参数前面，用来承接多个返回值， 与ref一样也是引用传递

但是，
- 在传入函数时不需要初始化
- 在函数退出时，必须完成赋值

结合例子理解一下：

```
class Test
{
static void Split (string name, out string firstNames,
out string lastName)
{
int i = name.LastIndexOf (' ');
firstNames = name.Substring (0, i);
lastName = name.Substring (i + 1);
}
static void Main()
{
string a, b;
Split ("Stevie Ray Vaughan", out a, out b);
Console.WriteLine (a); // Stevie Ray
Console.WriteLine (b); // Vaughan
}
}
```

#### C# 7新增的
- 动态声明out的变量类型

```
static void Main()
{
Split ("Stevie Ray Vaughan", out string a, out string b);
Console.WriteLine (a); // Stevie Ray
Console.WriteLine (b); // Vaughan
}
```
- 使用`out _`表示舍弃该变量

```
static void Main()
{
Split ("Stevie Ray Vaughan", out string a, out _);// Discard the 2nd param
Console.WriteLine (a); 

}
```

但是，为了向后兼容性，

以下的语法将报错

```
string _;
Split ("Stevie Ray Vaughan", out string a, _); // Will not compile
```

# 引用传递的含义

通过引用传递参数，简单的理解是传入原始值，而非传入拷贝值，这意味着两个完全相同。代表完全同一个实例。

比如下面代码中x和y代表同一个实例

```
class Test
{
static int x;
static void Main() { Foo (out x); }
static void Foo (out int y)
{
Console.WriteLine (x); // x is 0
y = 1; // Mutate y
Console.WriteLine (x); // x is 1
}
}
```

#### 关键字 params

params可以在函数的最后一个参数上指定，可以接受特定类型的任意数量的参数，

**参数类型必须声明为数组类型**

```
class Test
{
static int Sum (params int[] ints)
{
int sum = 0;
for (int i = 0; i < ints.Length; i++)
    sum += ints[i]; // Increase sum by ints[i]
return sum;
}
static void Main()
{
int total = Sum (1, 2, 3, 4);
Console.WriteLine (total); // 10
}
}
```

也可以将params参数看做一个普通的数组，此时，传入一个数组即可，比如：

下面的代码与`Main`中的第一行代码等效：

`int total = Sum (new int[] { 1, 2, 3, 4 } );`

### optional parameters 可选参数

### named parameters

可以再传递参数时使用名字来指定特定的参数

```
void Foo (int x, int y) { Console.WriteLine (x + ", " + y); }
void Test()
{
Foo (x:1, y:2); // 1, 2
}
```

但是，在传递参数时，位置参数一定要在命名的参数前面

`Foo (x:1, 2); // Compile-time error`

可选参数（optional parameters）和命名参数（named parameters）结合在一起使用，很有效果。

比如：

```
void Bar (int a = 0, int b = 0, int c = 0, int d = 0) { ... }

Bar(d:3) //仅仅改变d的值，而其他的参数值不用改变
```

### var  Implicitly Typed Local Variables 隐式的局部类型变量

如果编译器能够从初始化语句中推断出变量的类型，可以使用关键字`var`

```
var x = "hello";
var y = new System.Text.StringBuilder();
var z = (float)Math.PI;

//等效于
string x = "hello";
System.Text.StringBuilder y = new System.Text.StringBuilder();
float z = (float)Math.PI;
```

但是,使用`var`是在静态编译时就确定变量的具体类型了，所以以下代码会报错的：

```
var x = 5;
x = "hello"; // Compile-time error; x is of type int
```

**我觉得吧，var严重降低了代码的可读性**

### Expressions and Operators

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181031222846625.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181031222902424.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181031222914311.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181031222929967.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
### null operators

```
string s1 = null;
string s2 = s1 ?? "nothing"; // s2 evaluates to "nothing"
```

如果s1不是null,赋值给s2，否则将默认值’nothong’赋值给s2




