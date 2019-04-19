# 可空类型Nullable到底是什么鬼 - 文章 - 伯乐在线
原文出处： [农码一生](http://www.cnblogs.com/zhaopei/p/What_Is_Nullable.html )
# 值类型为什么不可以为空
首先我们都知道引用类型默认值都是null，而值类型的默认值都有非null。
![](http://jbcdn2.b0.upaiyun.com/2016/06/a5eac46e185119b4ca3a197d4b4d750f.png)
为什么引用类型可以为空？因为引用类型变量都是保存一个对象的地址引用（就像一个url对应一个页面）,而引用类型值为null的时候是变量值指向了一个空引用（如同一个空的url）
![](http://jbcdn2.b0.upaiyun.com/2016/06/373382d08fac6e56aa47f50c2589fee8.png)
那为什么值不能有空值呢？其实很简单，因为如int值范围是-2147483648到2147483647。其中根本就没有给null值留那么一个位置。
![](http://jbcdn2.b0.upaiyun.com/2016/06/ea33a316eac352775c12ff0362ea65f2.png)
# 我们为什么需要用到可空类型
举个栗子吧，我们定义一个人（Person），它有三个属性出生日期（BeginTime）、死亡日期（EndTime）、年龄（Age）。
如果这个人还健在人世，请问怎么给死亡日期赋值？有人很聪明说“为空啊”。是的，这就是我们的需求。
微软在C#2.0的时候就为我们引入了可null值类型（ System.Nullable ），那么下面来定义Person类。


C#
```
public class Person
{
    /// <summary>
    /// 出生日期
    /// </summary>
    public DateTime BeginTime { get; set; }
    /// <summary>
    /// 死亡日期
    /// </summary>
    public System.Nullable<DateTime> EndTiem { get; set; }
    public int Age
    {
        get
        {
            if (EndTiem.HasValue)//如果挂了(如果有值，证明死了)
            {
                return (EndTiem.Value - BeginTime).Days;
            }
            else//还没挂
            {
                return (DateTime.Now - BeginTime).Days;
            }
        }
    }
}
```
这样，我们就可以很容易获得一个人的年龄了。


C#
```
static void Main(string[] args)
{
    Person p1 = new Person()
    {
        BeginTime = DateTime.Parse("1990-07-19")
    };
    Person p2 = new Person()
    {
        BeginTime = DateTime.Parse("1893-12-26"),
        EndTiem = DateTime.Parse("1976-09-09")
    };
    Console.WriteLine("我今年" + p1.Age + "岁。");
    Console.WriteLine("毛爷爷活了" + p2.Age + "岁。");
    Console.ReadKey();
}
```
# 可空类型的实现
我们前面用到了 System.Nullable 来表示可空时间类型，其实平时我们用得更多的是 DateTime? 直接在类型T后面加一个问号，这两种是等效的。多亏了微软的语法糖。
我们来看看 System.Nullable 到底是何物。
![](http://jbcdn2.b0.upaiyun.com/2016/06/6b24f425b2fce7c852471a3e83d6b79f.png)
搜噶，原来是一个结构。还看到了我们属性的 HasValue和Value属性。原来竟这般简单。一个结构两个属性，一个存值，一个存是否有值。那么下面我们也来试试吧。
![](http://jbcdn2.b0.upaiyun.com/2016/06/fc9e03d4b73817127fb342db25e55d00.png)
不好意思，让大家失望了。前面我们就说过了，值类型是不可以赋值null的(结构也是值类型)。
怎么办！怎么办！不对啊，微软自己也是定义的结构，它怎么可以直接赋值null呢。（奇怪，奇怪，毕竟是人家微软自己搞得，可能得到了特殊的待遇吧）
可是，这样就让我们止步了吗？NO!我们都知道，看微软的IL（中间语言）的时候，就像脱了它的衣服一样，很多时候不明白的地方都可以看个究竟，下面我们就去脱衣服。
首先，我们用几种不同的方式给可空类型赋值。


C#
```
static void Main(string[] args)
{
    System.Nullableint> number1 = null;
    System.Nullableint> number2 = new System.Nullableint>();
    System.Nullableint> number3 = 23;
    System.Nullableint> number4 = new System.Nullableint>(88);
    Console.ReadKey();
}
```
然后用reflector看编译后的IL。
![](http://jbcdn2.b0.upaiyun.com/2016/06/527532b27a2a0de9dcbe8c8f731d6f71.png)
原来如此，可空类型的赋值直接等效于构造实例。赋null时其实就是调用空构造函数，有值时就就把值传入带参数的构造函数。(柳暗花明又一村。如此，我们是否可以接着上面截图中的 MyNullable 继续模拟可空类型呢？且继续往下看。)


C#
```
public struct MyNullablewhere T : struct
{
    //错误    1    结构不能包含显式的无参数构造函数 
    //还好 bool默认值就是false,所以这里不显示为 this._hasValue = false也不会有影响
    //public MyNullable()
    //{
    //    this._hasValue = false;
    //}
    public MyNullable(T value)//有参构造函数
    {
        this._hasValue = true;
        this._value = value;
    }
    private bool _hasValue;
    public bool HasValue//是否不为空
    {
        get { return _hasValue; }
    }
    private T _value;
    public T Value//值
    {
        get
        {
            if (!this._hasValue)//如没有值，还访问就抛出异常
            {
                throw new Exception(" 可为空的对象必须具有一个值");
            }
            return _value;
        }
    }
}
```
哟西，基本上已经模拟出了可空类型出来的。（但是我们还是不能直接赋值，只能通过构造函数的方式来使用自定义的可空类型）。
全部代码如下：

C#
```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
namespace 可空类型
{
    public class Person
    {
        /// 
        /// 出生日期
        /// 
        public DateTime BeginTime { get; set; }
        /// 
        /// 死亡日期
        /// 
        public MyNullable EndTiem { get; set; } //这里改用MyNullable
        /// 
        /// 年龄
        /// 
        public double Age
        {
            get
            {
                if (EndTiem.HasValue)//如果挂了(如果有值，证明死了)
                {
                    return (EndTiem.Value - BeginTime).Days / 365;
                }
                else//还没挂
                {
                    return (DateTime.Now - BeginTime).Days / 365;
                }
            }
        }
    }
    public struct MyNullablewhere T : struct
    {
        //错误    1    结构不能包含显式的无参数构造函数 
        //还好 bool默认值就是false,所以这里不显示为 this._hasValue = false也不会有影响
        //public MyNullable()
        //{
        //    this._hasValue = false;
        //}
        public MyNullable(T value)//有参构造函数
        {
            this._hasValue = true;
            this._value = value;
        }
        private bool _hasValue;
        public bool HasValue//是否不为空
        {
            get { return _hasValue; }
        }
        private T _value;
        public T Value//值
        {
            get
            {
                if (!this._hasValue)//如没有值，还访问就抛出异常
                {
                    throw new Exception(" 可为空的对象必须具有一个值");
                }
                return _value;
            }
        }
    }
    class Program
    {
        static void Main(string[] args)
        {
            Person p1 = new Person()
            {
                BeginTime = DateTime.Parse("1990-07-19")
            };
            Person p2 = new Person()
            {
                BeginTime = DateTime.Parse("1893-12-26"),
                EndTiem = new MyNullable(DateTime.Parse("1976-09-09"))//这里使用MyNullable的有参构造函数
            };
            Console.WriteLine("我今年" + p1.Age + "岁。");
            Console.WriteLine("毛爷爷活了" + p2.Age + "岁。");
            Console.ReadKey();
        }
    }
}
```
和系统的可空类型得出了相同的结果。
![](http://jbcdn2.b0.upaiyun.com/2016/06/d4303c4e073073c58fa20c5487542796.png)
# 总结
- 可空类型是结构（也就是值类型）
- 所以可空类型的null值和引用类型的null是不一样的。（可空类型的并不是引用类型的null，而是用结构的另一种表示方式来表示null）
![](http://jbcdn2.b0.upaiyun.com/2016/06/e3a88d6d43e0017d8a79eeeaad67c1d8.png)
> 
有同学问，怎么样才可以做到直接赋值呢？这个我也没有什么好的办法，或许需要编译器的支持。
以上内容都是胡说八道。希望能对您有那么一点点用处，感谢阅读。
============== 2016-06-05更新==============
上面我们提出了疑问“**怎么样才可以做到直接赋值呢**”，本来我是没有好的解决办法。这里要感谢我们的园友@冲杀给我提供了好的解决方案。
implicit（关键字用于声明隐式的用户定义类型转换运算符。）


C#
```
public static implicit operator MyNullable(T value)
{
       return new MyNullable(value);
}
```
只需要在 struct MyNullable 中添加以上代码，就可以直接赋值了。（作用等效于是直接重写了“=”赋值符号）
![](http://jbcdn2.b0.upaiyun.com/2016/06/e78623f665efc5daf70e783f705d5d5e.png)
![](http://jbcdn2.b0.upaiyun.com/2016/06/e3aa9cf5c3f1df055e7ddc6f1c9ab11c.png)
完整代码如下：

C#
```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
namespace test
{
    public class Person
    {
        /// 
        /// 出生日期
        /// 
        public DateTime BeginTime { get; set; }
        /// 
        /// 死亡日期
        /// 
        public MyNullable EndTiem { get; set; } //这里改用MyNullable
        /// 
        /// 年龄
        /// 
        public double Age
        {
            get
            {
                if (EndTiem.HasValue)//如果挂了(如果有值，证明死了)
                {
                    return (EndTiem.Value - BeginTime).Days / 365;
                }
                else//还没挂
                {
                    return (DateTime.Now - BeginTime).Days / 365;
                }
            }
        }
    }
    public struct MyNullablewhere T : struct
    {
        //错误    1    结构不能包含显式的无参数构造函数 
        //还好 bool默认值就是false,所以这里不显示为 this._hasValue = false也不会有影响
        //public MyNullable()
        //{
        //    this._hasValue = false;
        //} 
        public MyNullable(T value)//有参构造函数
        {
            this._hasValue = true;
            this._value = value;
        }
        private bool _hasValue;
        public bool HasValue//是否不为空
        {
            get { return _hasValue; }
        }
        private T _value;
        public T Value//值
        {
            get
            {
                if (!this._hasValue)//如没有值，还访问就抛出异常
                {
                    throw new InvalidOperationException(" 可为空的对象必须具有一个值");
                }
                return _value;
            }
        }
        public static implicit operator MyNullable(T value)
        {
            return new MyNullable(value);
        } 
    }
    class Program
    {
        static void Main(string[] args)
        {
            Person p1 = new Person()
            {
                BeginTime = DateTime.Parse("1990-07-19")
            };
            Person p2 = new Person()
            {
                BeginTime = DateTime.Parse("1893-12-26"),
                EndTiem = DateTime.Parse("1976-09-09") 
                //new MyNullable(DateTime.Parse("1976-09-09"))
                //这里使用MyNullable的有参构造函数
            };
            Console.WriteLine("我今年" + p1.Age + "岁。");
            Console.WriteLine("毛爷爷活了" + p2.Age + "岁。"); 
            Console.ReadKey();
        }
    }
}
```
如此，我们已经完成了自定义可空类型的直接赋值。但只是部分，如果想要赋值null呢？
![](http://jbcdn2.b0.upaiyun.com/2016/06/09ddb47eb14d77c696809b14d8b1726f.png)
同样还是出现了最开始的编译错误。我们想到既然上面的值赋值可以重新（隐式转换）,那null应该也可以啊（null是引用类型的一个特定值）。
再加一个重载：


C#
```
//隐式转换
public static implicit operator MyNullable(string value)
{
    if (value == null)
        return new MyNullable();
    throw new Exception("赋值右边不能为字符串");
    //这里不知道是否可以在编译期间抛出错误(或者怎样限制只能传null)
}
```
如此可以满足我们的需求了（并无异常）。
![](http://jbcdn2.b0.upaiyun.com/2016/06/d6eb1ba5eaf6f0f1dece38875b812cdf.png)
可惜美中不足，如果给 p2.EndTiem 赋值一个非空字符串时，要运行时才会报错（而系统的可空类型会在编译期就报错）。不知道大神们可有解！！
虽然如此，能做到直接赋值还是让我小小激动了一把。为此，特意查了下关键字 implicit operator ，又是让我小小激动了一把，我们不仅可以“重写”赋值，我们还可以“重写”+ – * / % & | ^ > == != > =
下面我们先来“重写”下自定义可空类型的比较（==）运算符。


C#
```
//"重写"比较运算符
public static bool operator ==(MyNullable operand, MyNullable operand2)
{
    if (!operand.HasValue & !operand2.HasValue)
    {
        return true;
    }
    else if (operand.HasValue & operand2.HasValue)
    {
        if (operand2.Value.Equals(operand.Value))
        {
            return true;
        }
    }
    return false;
}
//"重写"比较运算符
public static bool operator !=(MyNullable operand, MyNullable operand2)
{
    return !(operand == operand2);
}
```


C#
```
Console.WriteLine("p1.EndTiem == null," + (p1.EndTiem == null).ToString());
Console.WriteLine("p2.EndTiem == null," + (p2.EndTiem == null).ToString());
Console.WriteLine("p1.EndTiem == DateTime.Parse(1976-09-09)," + (p1.EndTiem == DateTime.Parse("1976-09-09")).ToString());
Console.WriteLine("p2.EndTiem == DateTime.Parse(1976-09-09)," + (p2.EndTiem == DateTime.Parse("1976-09-09")).ToString());
p1.EndTiem = DateTime.Parse("2016-06-06");
p2.EndTiem = null;
Console.WriteLine();
Console.WriteLine("赋值 p1.EndTiem = DateTime.Parse(2016-06-06)  p2.EndTiem = null 后：");
Console.WriteLine("p1.EndTiem == null," + (p1.EndTiem == null).ToString());
Console.WriteLine("p2.EndTiem == null," + (p2.EndTiem == null).ToString());
Console.WriteLine("p1.EndTiem == DateTime.Parse(2016-06-06)," + (p1.EndTiem == DateTime.Parse("2016-06-06")).ToString());
Console.WriteLine("p2.EndTiem == DateTime.Parse(2016-06-06)," + (p2.EndTiem == DateTime.Parse("2016-06-06")).ToString());
```
![](http://jbcdn2.b0.upaiyun.com/2016/06/4b0b9b527074cb9e5aaff6ca186836b6.png)
结果完全符合！
完整代码如下：

C#
```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
namespace test
{
    public class Person
    {
        /// 
        /// 出生日期
        /// 
        public DateTime BeginTime { get; set; }
        /// 
        /// 死亡日期
        /// 
        public MyNullable EndTiem { get; set; } //这里改用MyNullable
        /// 
        /// 年龄
        /// 
        public double Age
        {
            get
            {
                if (EndTiem.HasValue)//如果挂了(如果有值，证明死了)
                {
                    return (EndTiem.Value - BeginTime).Days / 365;
                }
                else//还没挂
                {
                    return (DateTime.Now - BeginTime).Days / 365;
                }
            }
        }
    }
    public struct MyNullablewhere T : struct
    {
        //错误    1    结构不能包含显式的无参数构造函数 
        //还好 bool默认值就是false,所以这里不显示为 this._hasValue = false也不会有影响
        //public MyNullable()
        //{
        //    this._hasValue = false;
        //} 
        public MyNullable(T value)//有参构造函数
        {
            this._hasValue = true;
            this._value = value;
        }
        private bool _hasValue;
        public bool HasValue//是否不为空
        {
            get { return _hasValue; }
        }
        private T _value;
        public T Value//值
        {
            get
            {
                if (!this._hasValue)//如没有值，还访问就抛出异常
                {
                    throw new InvalidOperationException(" 可为空的对象必须具有一个值");
                }
                return _value;
            }
        }
        //隐式转换
        public static implicit operator MyNullable(T value)
        {
            return new MyNullable(value);
        }
        //隐式转换
        public static implicit operator MyNullable(string value)
        {
            if (value == null)
                return new MyNullable();
            throw new Exception("赋值右边不能为字符串");
            //这里不知道是否可以在编译期间抛出错误(或者怎样限制只能传null)
        }
        //"重写"比较运算符
        public static bool operator ==(MyNullable operand, MyNullable operand2)
        {
            if (!operand.HasValue & !operand2.HasValue)
            {
                return true;
            }
            else if (operand.HasValue & operand2.HasValue)
            {
                if (operand2.Value.Equals(operand.Value))
                {
                    return true;
                }
            }
            return false;
        }
        //"重写"比较运算符
        public static bool operator !=(MyNullable operand, MyNullable operand2)
        {
            return !(operand == operand2);
        }
    }
    class Program
    {
        static void Main(string[] args)
        {
            Person p1 = new Person()
            {
                BeginTime = DateTime.Parse("1990-07-19")
            };
            Person p2 = new Person()
            {
                BeginTime = DateTime.Parse("1893-12-26"),
                EndTiem = DateTime.Parse("1976-09-09")
                //new MyNullable(DateTime.Parse("1976-09-09"))
                //这里使用MyNullable的有参构造函数
            };
            Console.WriteLine("我今年" + p1.Age + "岁。");
            Console.WriteLine("毛爷爷活了" + p2.Age + "岁。");
            Console.WriteLine();
            Console.WriteLine("p1.EndTiem == null," + (p1.EndTiem == null).ToString());
            Console.WriteLine("p2.EndTiem == null," + (p2.EndTiem == null).ToString());
            Console.WriteLine("p1.EndTiem == DateTime.Parse(1976-09-09)," + (p1.EndTiem == DateTime.Parse("1976-09-09")).ToString());
            Console.WriteLine("p2.EndTiem == DateTime.Parse(1976-09-09)," + (p2.EndTiem == DateTime.Parse("1976-09-09")).ToString());
            p1.EndTiem = DateTime.Parse("2016-06-06");
            p2.EndTiem = null;
            Console.WriteLine();
            Console.WriteLine("赋值 p1.EndTiem = DateTime.Parse(2016-06-06)  p2.EndTiem = null 后：");
            Console.WriteLine("p1.EndTiem == null," + (p1.EndTiem == null).ToString());
            Console.WriteLine("p2.EndTiem == null," + (p2.EndTiem == null).ToString());
            Console.WriteLine("p1.EndTiem == DateTime.Parse(2016-06-06)," + (p1.EndTiem == DateTime.Parse("2016-06-06")).ToString());
            Console.WriteLine("p2.EndTiem == DateTime.Parse(2016-06-06)," + (p2.EndTiem == DateTime.Parse("2016-06-06")).ToString());     
            Console.ReadKey();
        }
    }
}
```
转换关键字：operator、explicit与implicit解析资料：[http://www.cnblogs.com/hunts/archive/2007/01/17/operator_explicit_implicit.html](http://www.cnblogs.com/hunts/archive/2007/01/17/operator_explicit_implicit.html)
大家还可以玩出更多的花样！！！
