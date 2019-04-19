# C#基础温习: 理解委托和事件 - 文章 - 伯乐在线
原文出处： [gao-yang](http://www.cnblogs.com/gao-yang/p/5493028.html)
# 1. 委托
> 
　　委托类似于C++中的函数指针（一个指向内存位置的指针）。委托是C#中类型安全的，可以订阅一个或多个具有相同签名方法的函数指针。简单理解，委托是一种可以把函数当做参数传递的类型。很多情况下，某个函数需要动态地去调用某一类函数，这时候我们就在参数列表放一个委托当做函数的占位符。在某些场景下，使用委托来调用方法能达到减少代码量，实现某种功能的用途。
## 1.1 自定义委托
声明和执行一个自定义委托，大致可以通过如下步骤完成：
-  利用关键字delegate声明一个委托类型，它必须具有和你想要传递的方法具有相同的参数和返回值类型；
-  创建委托对象，并且将你想要传递的方法作为参数传递给委托对象；
-  通过上面创建的委托对象来实现该委托绑定方法的调用。
下面一段代码，完成了一次应用委托的演示：


C#
```
//step01：使用delegate关键字声明委托
    public delegate int CalcSumDelegate(int a, int b);class Program
    {
        static void Main(string[] args)
        {
            //step03：实例化这个委托，并引用方法
            CalcSumDelegate del = new CalcSumDelegate(CalcSum);
            //step04：调用委托
            int result = del(5, 5);
            Console.WriteLine("5+5=" + result);
        }//step02：声明一个方法和委托类型对应
        public static int CalcSum(int a, int b)
        {
            return a + b;
        }
    }
```
通过上面4个步骤，完成了委托从声明到调用的过程。接着，咱也学着大神用ILSpy反编译上面的代码生成的程序集。截图如下：
![](http://jbcdn2.b0.upaiyun.com/2016/05/ae74c30238042ec59e5caebfd4937b911.png)
（1）自定义委托继承关系是：**System.MulticastDelegate —> System.Delegate —>System.Object**。
（2）委托类型自动生成3个方法：**BeginInvoke**、**EndInvoke**、**Invoke**。查资料得知，委托正是通过这3个方法在内部实现调用的。
Invoke 方法，允许委托同步调用。上面调用委托的代码del(5, 5)执行时，编译器会自动调用 del.Invoke(5,5)；
BeginInvoke 方法，以允许委托的异步调用。假如上述委托以异步的方式执行，则要显示调用dal.BeginInvoke(5,5)。
> 
 注意：BeginInvoke 和 EndInvoke 是.Net中**使用异步方式调用同步方法**的两个重要方法，具体用法详见微软[官方示例](https://msdn.microsoft.com/zh-cn/library/2e08f6yc(v=vs.100).aspx)。
## 1.2 多播委托
一个委托可以引用多个方法，包含多个方法的委托就叫多播委托。下面通过一个示例来了解什么是多播委托：


C#
```
//step01：声明委托类型
    public delegate void PrintDelegate();
    public class Program
    {
        public static void Main(string[] args)
        {
            //step03：实例化委托，并绑定第1个方法
            PrintDelegate del = Func1;
            //绑定第2个方法
            del += Func2;
            //绑定第3个方法
            del += Func3;
            //step04：调用委托
            del();//控制台输出结果：
            //调用第1个方法!
            //调用第2个方法! 
            //调用第3个方法!
        }
        //step02：声明和委托对应签名的3个方法
        public static void Func1()
        {
            Console.WriteLine("调用第1个方法!");
        }
        public static void Func2()
        {
            Console.WriteLine("调用第2个方法!");
        }
        public static void Func3()
        {
            Console.WriteLine("调用第3个方法!");
        }
    }
```
可以看出，多播委托的声明过程是和自定义委托一样的，可以理解为，多播委托就是自定义委托在实例化时通过 “+=” 符号多绑定了两个方法。
**（1）为什么能给委托绑定多个方法呢？**
自定义委托的基类就是多播委托**MulticastDelegate **，这就要看看微软是如何对System.MulticastDelegate定义的：
***MulticastDelegate **拥有一个带有链接的委托列表，该列表称为调用列表，它包含一个或多个元素。在调用多路广播委托时，将按照调用列表中的委托出现的顺序来同步调用这些委托。如果在该列表的执行过程中发生错误，则会引发异常。（摘自MSDN）*
**（2）为什么使用“+=”号就能实现绑定呢？**
先来看上述程序集反编译后的调用委托的代码：
![](http://jbcdn2.b0.upaiyun.com/2016/05/0ed38f015225c210e71665e12496a0d01.png)
“+=”的本质是调用了Delegate.Combine方法，该方法将两个委托连接在一起，并返回合并后的委托对象。
**（3）多播委托能引用多个具有返回值的方法嘛？**
答案是，当然能。委托的方法可以是无返回值的，也可以是有返回值的。不过，对于有返回值的方法需要我们从委托列表上手动调用。否则，就只能得到委托调用的最后一个方法的结果。下面通过两段代码验证下：

C#
```
public delegate string GetStrDelegate();
    public class Program
    {
        public static void Main(string[] args)
        {
            GetStrDelegate del = Func1;
            del += Func2;
            del += Func3;
            string result = del();
            Console.WriteLine(result);
            
            //控制台输出结果：
        //You called me from Func3
        }
        public static string Func1()
        {
            return "You called me from Func1!";
        }
        public static string Func2()
        {
            return "You called me from Func2!";
        }
        public static string Func3()
        {
            return "You called me from Func3!";
        }
    }
```
正确做法：利用GetInvocationList获得委托列表上所有方法，循环依次执行委托，并处理委托返回值。

C#
```
public delegate string GetStrDelegate();
    public class Program
    {
        public static void Main(string[] args)
        {
            GetStrDelegate del = Func1;
            del += Func2;
            del += Func3;
            //获取委托链上所有方法
            Delegate[] delList = del.GetInvocationList();
            //遍历，分别处理每个方法的返回值
            foreach (GetStrDelegate item in delList)
            {
                //执行当前委托
                string result = item();
                Console.WriteLine(result);
                //控制台输出结果：
                //You called me from Func1
                //You called me from Func2
                //You called me from Func3
            }
            Console.ReadKey();
        }
        public static string Func1()
        {
            return "You called me from Func1";
        }
        public static string Func2()
        {
            return "You called me from Func2";
        }
        public static string Func3()
        {
            return "You called me from Func3";
        }
    }
```
## 1.3 匿名方法
匿名方法是C#2.0版本引入的一个新特性，用来简化委托的声明。假如委托引用的方法只使用一次，那么就没有必要声明这个方法，这时用匿名方法表示即可。

C#
```
//step01：定义委托类型
    public delegate string ProStrDelegate(string str);
    public class Program
    {
        public static void Main(string[] args)
        {
            //step02：将匿名方法指定给委托对象
            ProStrDelegate del = delegate(string str) { return str.ToUpper(); };
            string result = del("KaSlFkaDhkjHe");
            Console.WriteLine(result);
            Console.ReadKey();
            //输出：KASLFKAFHKJHE
        }
    }
```
（1）匿名方法只是C#提供的一个语法糖，方便开发人员使用。在性能上与命名方法几乎无异。
（2）匿名方法通常在下面情况下使用：
1.委托需要指定一个临时方法，该方法使用次数极少；
2.这个方法的代码很短，甚至可能比方法声明都短的情况下使用。
## 1.4 Lambda表达式
Lambda表达式是C#3.0版本引入的一个新特性，它提供了完成和匿名方法相同目标的更加简洁的格式。下面示例用Lambda表达式简化上述匿名方法的例子：


C#
```
public delegate string ProStrDelegate(string str);
    public class Program
    {
        public static void Main(string[] args)
        {
            //匿名委托
            //ProStrDelegate del = delegate(string str) { return str.ToUpper(); };
            //简化1
            //ProStrDelegate del1 = (string str) =>{ return str.ToUpper(); };
            //简化2
            //ProStrDelegate del2 = (str) =>{ return str.ToUpper(); };
            //简化3
            ProStrDelegate del3 = str => str.ToUpper();
            string result = del3("KaSlFkaDhkjHe");
            Console.WriteLine(result);
            Console.ReadKey();
            //输出：KASLFKAFHKJHE
        }
    }
```
- 简化1：去掉delegate关键字，用”=>”符号表示参数列表和方法体之间的关系；
- 简化2：去掉方法的参数类型；假如只有一个参数，参数列表小括号()也可省略；
- 简化3：如果方法体中的代码块只有一行，可以去掉return，去掉方法体的大括号{}。
## 1.5 内置委托
上述几种委托的使用，都没能离开定义委托类型这一步骤。微软干脆直接把定义委托这一步骤封装好，形成三个泛型类：Action、Func和Predicate，这样就省去了定义的步骤，推荐使用。


C#
```
public class Program
    {
        public static void Main(string[] args)
        {
            //Action
            Actionstring> action = delegate(string str) { Console.WriteLine("你好!" + str); };
            action("GG");
//Func
            Funcint, int, int> func = delegate(int x, int y) { return x + y; };
            Console.WriteLine("计算结果：" + func(5, 6));
//Predicate
            Predicatebool> per = delegate(bool isTrue) { return isTrue == true; };
            Console.WriteLine(per(true));
        }
    }
```
它们的区别如下：
- Action委托：允许封装的方法有多个参数，不能有返回值；
- Func委托：允许封装的方法有多个参数，必须有返回值；
- Predicate委托：允许封装的方法有一个参数，返回值必须为bool类型。
# 2. 事件
> 
　　委托是一种类型，事件依赖于委托，故事件可以理解为是委托的一种特殊实例。它和普通的委托实例有什么区别呢？委托可以在任意位置定义和调用，但是事件只能定义在类的内部，只允许在当前类中调用。所以说，事件是一种类型安全的委托。
## 2.1 定义事件
通过一个简单的场景来演示下事件的使用：

C#
```
/// 
    /// 音乐播放器
    /// 
    public class MusicPlayer
    {
        //step01：定义 音乐播放结束 事件
        public event EventHandler PlayOverEvent;
        public string Name { get; set; }
        public MusicPlayer(string name)
        {
            this.Name = name;
        }
        //step02：定义一个触发事件的方法
        public void PlaySong()
        {
            //模拟播放
            Console.WriteLine("正在播放歌曲：" + this.Name);
            for (int i = 0; i 20; i++)
            {
                Console.Write(".");
                Thread.Sleep(100);
            }
            //播放结束，则触发PlayOverEvent事件
            if (PlayOverEvent != null)
            {
                PlayOverEvent(this, null);
            }
        }
    }
    public class Program
    {
        static void Main(string[] args)
        {
            //创建音乐播放器对象
            MusicPlayer player = new MusicPlayer("自由飞翔");
            //step03：注册事件
            player.PlayOverEvent += player_PlayOverEvent;
            //播放歌曲，结束后触发事件
            player.PlaySong();
            Console.ReadKey();
        }
        static void player_PlayOverEvent(object sender,EventArgs e)
        {
            MusicPlayer player = sender as MusicPlayer;
            Console.WriteLine("rn{0}播完了！", player.Name);
        }
    }
```
程序运行结果：
![](http://jbcdn2.b0.upaiyun.com/2016/05/020c6c2b71e6ffc52ff7a670eb8ad9101.gif)
（1）总结上面事件使用的几个步骤：
    step01：用event关键字定义事件，事件必须要依赖一个委托类型；
    step02：在类内部定义触发事件的方法；
    step03：在类外部注册事件并引发事件。
（2）public event EventHandler PlayOverEvent
这句代码在MusicPlayer类定义了一个事件成员PlayOverEvent，我们说事件依赖于委托、是委托的特殊实例，所以EventHandler肯定是一个委托类型。下面我们来验证一下：
![](http://jbcdn2.b0.upaiyun.com/2016/05/028de144b09481aa0669b57dce25dafe1.png)
EventHandler是微软封装好的事件委托，该委托没有返回值类型，两个参数：sender事件源一般指的是事件所在类的实例；TEventArgs事件参数，如果有需要创建，要显示继承System.EventArgs。
## 2.2 事件的本质

C#
```
MusicPlayer player = new MusicPlayer("自由飞翔");
            //注册事件
            player.PlayOverEvent += player_PlayOverEvent;
            player.PlaySong();
```
从上面代码我们观察到，事件要通过”+=”符号来注册。我们猜想，事件是不是像多播委托一样通过Delegate.Combine方法可以绑定多个方法？还是通过反编译工具查看下：
![](http://jbcdn2.b0.upaiyun.com/2016/05/6ac1830c5bb22c38c10aed1978f224fd1.png)
我们看到PlayOverEvent事件内部生成了两个方法：add_ PlayOverEvent和remove_ PlayOverEvent。add方法内部调用Delegate.Combine把事件处理方法绑定到委托列表；remove方法内部调用Delegate.Remove从委托列表上移除指定方法。其实，事件本质上就是一个多播委托。
# 3. 参考文章
[1] Edison Chou，[http://www.cnblogs.com/edisonchou/p/4827578.html](http://www.cnblogs.com/edisonchou/p/4827578.html)
[2] jackson0714，[http://www.cnblogs.com/jackson0714/p/5111347.html](http://www.cnblogs.com/jackson0714/p/5111347.html)
[3] Sam Xiao， [http://www.cnblogs.com/xcj26/p/3536082.html](http://www.cnblogs.com/xcj26/p/3536082.html)
