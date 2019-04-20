# C# 学习多线程 - u014677855的博客 - CSDN博客
2018年08月31日 17:31:30[xiaobigben](https://me.csdn.net/u014677855)阅读数：227
## 目录
- - [目录](#目录)
- [一、线程相关概念介绍](#一线程相关概念介绍)
- [二、多线程](#二多线程)
- [三、线程操作之Thread类](#三线程操作之thread类)- [3.1创建线程](#31创建线程)
- [3.2线程的属性](#32线程的属性)- [3.2.1 线程的优先级别](#321-线程的优先级别)
- [3.2.2 线程的状态](#322-线程的状态)
- [3.3 线程的方法](#33-线程的方法)
- [3.4前台线程和后台线程](#34前台线程和后台线程)
- [四、线程同步](#四线程同步)
- [五、跨线程访问](#五跨线程访问)
- [六、终止线程](#六终止线程)
- [七、同步和异步](#七同步和异步)
- [七、同步和异步](#七同步和异步-1)
## 一、线程相关概念介绍
当启动一个应用程序的时候，系统就会为其分配所需的内存和资源。内存与资源你的物理隔离就叫做进程。分配给进程的内存与其他进程的内存被隔离，具有所属的线程才可以访问它。
一个正在运行的应用程序在操作系统中被视为一个进程。线程是操作系统分配处理器时间的基本单元，在进程中可以有多个线程同时执行代码。进程之间是相对独立的，一个进程无法访问另一个进程的数据，一个进程运行失败也不会影响另一个进程的运行，Windows操作系统就是利用进程把工作划分为多个独立区域的。
一个进程至少包括一个可执行的线程。创建一个线程首先要在指令中某点处启动线程。线程的执行顺序是由执行代码决定的，应用程序中最初启动的线程叫做主线程。在.Net 应用程序中，主线程是在Main函数中启动的，最先调用Application.Run().
```
static void Main()
{
    Application.EnableVisualStyles(); 
    Application.Run(new FrmMain());
    instance.ReleaseMutex();
    }
}
```
单线程和多线程是针对进程而言的，如果一个进程包含多个线程，那么叫做多线程进程；如果一个进程包含一个线程，那么叫做单线程进程。
## 二、多线程
线程其实是指程序执行路径，是进程中的基本执行单元。每一个线程都定义了一个独特的程序控制流。如果应用程序设计到复杂和耗时的操作的话，那么用多线程是有益的。不同的线程执行不同的工作。 
多线程的优点：可以同时完成多个任务，使程序的响应速度更快。
## 三、线程操作之Thread类
### 3.1创建线程
一个进程可以创建一个或多个线程以执行与该进程关联的部分程序代码。在C#中，所有与线程相关的操作都在System.Threading命名空间中。线程是使用Thread类处理的。使用Thread类创建线程时，只需要提供线程入口，线程入口告诉程序让这个线程做什么。通过实例化一个Thread类的对象就可以创建一个线程。创建新的Thread类的实例时，将创建新的托管线程。简单来说，创建一个线程的步骤为：
- 编写线程所要执行的方法method()，方法可以是静态方法，也可以是实例方法。
- 创建一个ThreadStart委托的实例，该委托实例封装了启动线程时由新线程调用的方法method。
`ThreadStart ts = new ThreadStart(method);`- 然后将委托实例作为参数，构造Thread对象。
`Thread t = new Thread(ts);`- 创建好线程后， 启动线程
`t.Start();`
上面的线程创建步骤实例化了一个Thread对象，并指明了即将要调用的方法method()，然后启动线程。委托实例中作为参数的方法不需要参数，也没有返回值。
一个简单实例帮助理解线程创建：
```
static void Main(string[] args)
  {
       int i = 0;
       ThreadStart ts = new ThreadStart(method);
       ThreadStart ts1 = delegate ()
       {
           Console.WriteLine("我是通过匿名委托创建的线程");
       };
       ThreadStart ts3 = () =>
       {
           Console.WriteLine("我是通过Lambda表达式创建的线程");
       };
       Thread t = new Thread(ts);
       Thread t1 = new Thread(ts1);
       Thread t2 = new Thread(ts3);
       t.Start();
       t1.Start();
       t2.Start();
       Console.WriteLine("线程开始启动");
       Console.ReadLine();
   }
   public static void method()
   {
       Console.WriteLine("我是通过创建委托实例创建的线程");
   }
}
```
运行结果： 
![这里写图片描述](https://img-blog.csdn.net/20180831105609501?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 3.2线程的属性
线程常用属性有：
|属性名称|说明|
|----|----|
|CurrentContext|获取线程正在其中执行的当前上下文|
|CurrentThread|获取当前正在运行的线程|
|IsAlive|获取一个值，指示当前线程的执行状态|
|IsBackground|获取一个值，指示某个线程是否为后台线程|
|IsThreadPoolThread|指示线程是否属于托管线程池|
|Name|获取或设置线程的名称|
|Priority|获取或设置一个值，指示线程的调度优先级|
|ThreadState|获取一个值，该值包含当前线程的状态|
|ManagedThreadId|线程的唯一标识符|
#### 3.2.1 线程的优先级别
CPU按照线程的优先级给予服务。高优先级的线程可以完全阻止低优先级的线程执行。.NET为线程设置了Priority属性来定义线程执行的优先级别，里面包含5个选项，其中Normal是默认值。除非系统有特殊要求，否则不应该随便设置线程的优先级别。
#### 3.2.2 线程的状态
通过ThreadState可以检测线程是处于Unstarted、Sleeping、Running 等等状态，它比 IsAlive 属性能提供更多的特定信息。
前面说过，一个应用程序域中可能包括多个上下文，而通过CurrentContext可以获取线程当前的上下文。
CurrentThread是最常用的一个属性，它是用于获取当前运行的线程。
### 3.3 线程的方法
下面的实例帮助理解线程的方法和属性。
```
static void Main(string[] args)
 {
      //获取当前正在运行的线程
      Thread thread = Thread.CurrentThread;
      //设置当前线程的名字
      thread.Name = "主线程";
      //获取当前线程的唯一标识符
      int id = thread.ManagedThreadId;
      //获取当前线程的状态
      string state = thread.ThreadState.ToString();
      //获取当前线程的优先级
      ThreadPriority proiority = thread.Priority;
      string message = String.Format("Thread Id:{0}\n"+"Thread Name:{1}\n"+"Thread State:{2}\n"+"Thread Priority:{3}", id, thread.Name, state, proiority);
      Console.WriteLine(message);
      Console.ReadLine();
  }
```
运行结果： 
![这里写图片描述](https://img-blog.csdn.net/20180831112335893?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 3.4前台线程和后台线程
- 
前台线程
只有所有的前台线程都结束，应用程序才能结束。默认情况下创建的线程都是前台线程。
- 
后台线程
只要所有的前台线程结束，后台线程自动结束。通过IsBackground属性设置后台线程。必须在调用Start()方法之前设置线程的类型，否则一旦线程启动，将无法改变其类型。 
通过BeginInvoke方法运行的线程都是后台线程。 
下面的实例来说明前台线程与后台线程的区别：
```
class Program
 {
      static void Main(string[] args)
      {
          //后台类的实例
          BackgroundTest back = new BackgroundTest(10);
          ThreadStart ts = new ThreadStart(back.RunLoop);
          //创建前台线程：默认创建的线程都是前台线程。前台线程都结束后，应用程序结束。
          Thread t = new Thread(ts);
          //设置前台线程名称
          t.Name = "前台线程";
          BackgroundTest back1 = new BackgroundTest(20);
          //创建后台线程：要在线程启动前设置该线程为后台线程
          Thread t2 = new Thread(new ThreadStart(back1.RunLoop));
          t2.Name = "后台线程";
          //设置该线程t2为后台线程：前台线程结束后，后台线程自动结束
          t2.IsBackground = true;
          //启动线程
          t.Start();
          t2.Start();
          Console.ReadLine();
      }
      public static void method( )
      {
          Console.WriteLine();
      }
  }
  class BackgroundTest
  {
      private int Count;
      public BackgroundTest(int count)
      {
          this.Count = count;
      }
      public void RunLoop()
      {
          string threadName = Thread.CurrentThread.Name;
          for (int i = 0; i < Count; i++)
          {
              Console.WriteLine("{0}计数：{1}", threadName, i.ToString());
              Thread.Sleep(500);
          }
          Console.WriteLine("{0}完成计数", threadName);
      }
    }
 }
```
运行结果为： 
![这里写图片描述](https://img-blog.csdn.net/20180831133625193?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
 上述运行结果说明，当前台线程结束后，后台线程未执行完，但自动结束了。 
 在上述实例中，如果不设置IsBackground属性为true，那么就默认两个线程都为前台线程。会等前台线程执行完毕后关闭应用程序。 
 后台线程一般用于处理不重要的事情，应用程序结束后，后台线程是否执行完对整个应用程序没有影响。
## 四、线程同步
所谓同步，是指在某一时刻只有一个线程可以访问变量。 
如果不能确保对变量的访问是同步的，就会产生错误。
C#为同步访问变量提供了一个非常简单的方式，就是使用C#关键字Lock,它可以把一段代码定义为互斥段，互斥段在一个时刻内，只允许一个线程进入执行，而其他线程必须等待。关键字Lock语句定义如下：
```
Lock(expression)
{
    statement_block//代码块
}
```
expression代表你希望跟踪的对象： 
如果你想保护一个类的实例，一般地，可以使用this。 
如果想保护一个静态变量，一般使用类名就可以了。Lock中的代码块在一个时刻内只可能被一个线程执行。 
实例演示Lock关键字的用法，使得某个变量在某一时刻只能被一个线程执行。
```
class Program
{
    static void Main(string[] args)
    {
        BookShop book = new BookShop();
        Thread t1 = new Thread(new ThreadStart(book.Sale));
        Thread t2 = new Thread(new ThreadStart(book.Sale));
        //启动线程
        t1.Start();
        t2.Start();
        Console.ReadKey();
    }
    public static void method( )
    {
        Console.WriteLine();
    }
}
class BookShop
{
    public int num = 1;
    public void Sale()
    {
        lock(this)
        {
            int tmp = num;
            if (tmp > 0)
            {
                Thread.Sleep(100);
                num -= 1;
                Console.WriteLine("售出一本书，还剩余{0}本", num);
            }
            else
            {
                Console.WriteLine("没有了");
            }
        }
    }
}
```
上面的实例就说明了用lock关键字解决线程同步的问题。 
一旦用lock关键字将这段代码设置为互斥段的时候，当前时间只允许一个线程进入执行代码。
## 五、跨线程访问
新建了一个窗体项目，当点击“测试”按钮的时候，给文本框赋值1—10000。 
 下面的实例，当点击实例的时候，就创建了一个线程，去执行给文本框赋值的方法。
```
private void btnTest_Click(object sender, EventArgs e)
{
    //创建一个线程，去执行将1到10000写在文本框上的方法
    Thread thread = new Thread(new ThreadStart(Test));
    //后台线程
    thread.IsBackground = true;
    thread.Start();
}
private void Test()
{
    for(int i=0;i<10000;i++)
    {
        this.textBox1.Text = i.ToString();
    }
}
```
运行程序的时候，发生错误： 
![这里写图片描述](https://img-blog.csdn.net/20180831155814278?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
分析发生错误的原因：控件txtBox1是由主线程创建的，thread线程是另外创建的一个线程，在.Net上执行的是托管代码，C#强制要求这些代码是线程安全的，即不允许跨线程访问Windows窗体的控件。
解决方案： 
 1、屏蔽掉c#编译器对跨线程调用的检查。具体做法为：在加载窗体的时候，将C#内置控件Control类的CheckForIllegalCrossThreadCalls属性设置为false。
```
private void Form1_Load(object sender, EventArgs e)
 {
      //屏蔽掉编译器对跨线程调用的检查
      Control.CheckForIllegalCrossThreadCalls = false;
  }
```
再次运行时，程序正常运行并实现所需的功能。但在实际的开发中，这样设置是不符合.Net安全规范的。产品开发过程中，是不允许这样设置的。那么如果要实现从一个线程成功地访问另一个线程创建的控件，要使用C#的方法回调机制。 
2、使用回调函数 
C#的方法回调机制，也是建立在委托的基础上的。下面给出回调方法的实现过程。 
（1）定义、声明回调
```
//定义回调
private delegate void DoSomeCallBack(Type para);
//声明回调
DoSomeCallBack doSomeCallBack;
```
可以看出，这里的回调实际上就是一个委托。 
（2）初始化回调方法
```
doSomeCallBack = new DoSomeCallBack(DoSomeMethod);
```初始化回调方法，实际上就是实例化了刚刚定义的委托。这里作为参数的方法被称为回调方法。它封装了对另一个线程中窗体控件或其他类的操作。
（3）触发对象动作
定义，声明，初始化了回调方法后，就需要触发回调方法要操作的对象。
<div class="se-preview-section-delimiter"></div>
```
Opt obj.Invoke(doSomeCallBack,arg);
```
Opt obj为目标操作对象，比如说其他线程中的某个空间，调用其Invoke()方法。
下面的实例帮助理解跨线程调用时使用回调方法。
<div class="se-preview-section-delimiter"></div>
```
public partial class Form1 : Form 
 { 
     public Form1() 
     { 
         InitializeComponent(); 
     } 
     private delegate void setTextValueCallBack(int value); 
     private setTextValueCallBack setCallBack;
```
private void button1_Click(object sender, EventArgs e)
 {
     //初始化回调函数
     setCallBack = new setTextValueCallBack(SetValue);
     //创建一个线程去执行这个回调函数要操作的方法
     Thread t = new Thread(new ThreadStart(Test));
     t.IsBackground = true;
     t.Start();
 }
 private void Test()
 {
     for(int i=0;i<10001;i++)
     {
         //在控件上执行回调方法，触发操作
         textBox1.Invoke(setCallBack,i);
     }
 }
 /// <summary>
 /// 定义回调使用的方法
 /// </summary>
 /// <param name="value"></param>
 private void SetValue(int value)
 {
     textBox1.Text = value.ToString();
 }
```
} 
“` 
上面的代码实现了我们所需要的功能。
## 六、终止线程
Abort()方法终止线程
## 七、同步和异步
同步和异步是对方法执行顺序的描述。 
同步：等待上一行完成计算后，才会进入下一行。 
例如：请同事吃饭，同事说很忙，然后就等着同事忙完，然后一起去吃饭。 
异步：不会等待上一行方法完成，会直接进入下一行。 
例如：请同事吃饭，同事说很忙，那自己去吃饭，同事去忙。 
同步方法和异步方法的区别如下：
- 同步方法主线程由于要忙于计算，所以会卡住界面。异步方法不会卡住界面。
- 同步方法由于只有一个线程在计算，所以执行速度慢。异步线程由多个线程并发运算，所以执行速度快。
n 
n 
n 
n 
n 
n 
n 
n 
n 
n 
n 
n 
n 
n 
n 
n 
n 
n 
n 
n 
n 
n 
n 
n 
n 
n 
n 
n 
n 
n 
n 
n 
n 
n 
n 
n
## 七、同步和异步
同步和异步是对方法执行顺序的描述。 
同步：等待上一行完成计算后，才会进入下一行。 
例如：请同事吃饭，同事说很忙，然后就等着同事忙完，然后一起去吃饭。 
异步：不会等待上一行方法完成，会直接进入下一行。 
例如：请同事吃饭，同事说很忙，那自己去吃饭，同事去忙。 
同步方法和异步方法的区别如下：
- 同步方法主线程由于要忙于计算，所以会卡住界面。异步方法不会卡住界面。
- 同步方法由于只有一个线程在计算，所以执行速度慢。异步线程由多个线程并发运算，所以执行速度快。
