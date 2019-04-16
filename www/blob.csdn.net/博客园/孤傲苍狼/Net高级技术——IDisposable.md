# .Net高级技术——IDisposable - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [.Net高级技术——IDisposable](https://www.cnblogs.com/xdp-gacl/p/3541086.html)



## IDisposable概述

　　GC(垃圾收集器)只能回收**托管（****Managed****）**内存资源，对于数据库连接、文件句柄、Socket连接等这些资源（非托管资源，UnManaged）就无能为例，必须程序员自己控制资源的回收。非托管资源：SqlConnection、FileStream等。

**对于使用非内存资源的类**，完全可以自己定义一个销毁资源的方法来供程序员回收，**.net****推荐实现****IDisposable****接口（为了规范非托管资源的回收），在****Dispose****方法中进行回收。**

**实现了****IDisposable****接口的对象都可以使用****using****进行资源管理。**using就是帮我们做了：

```
1 FileStream fs = new F.....
2 try {  
3 fs.read()....
4 }
5 finally {
6 fs.Dispose();
7 }
```

　　实现了IDisposable接口的对象都要进行回收，否则很可能有内存泄露。

**非托管内存资源才需要实现****IDisposable**

## IDisposable例子

```
1 namespace 资源回收
 2 {
 3     class Program
 4     {
 5         static void Main(string[] args)
 6         {
 7             //使用using对实现IDisposable的类了进行资源管理
 8 /*拿到一个对象的时候，首先判断这个对象是否实现了IDisposable接口，如果实现了，最好就用using包裹住这个对象，保证这个对象用完之后被释放掉，否则很可能出现资源泄露的问题
 9 */
10             using (Telphone t1 = new Telphone())
11             {
12                 t1.Open();
13                 t1.Speak("hello");
14                 t1.Bomb();
15                 //t1.Dispose();//如果在这里调用了Dispose()方法释放资源，那么在执行t1.Open()方法就出错，电话线已经被剪断了，无法再打电话了
16                 t1.Open();
17                 t1.Speak("I am back!");
18             }//代码执行到这里后，就会调用Dispose方法来进行资源回收
19             Console.ReadKey();
20         }
21     }
22     /// <summary>
23     /// Telphone类实现了IDisposable接口
24     /// </summary>
25     class Telphone : IDisposable
26     {
27         /// <summary>
28         /// 电话状态
29         /// </summary>
30         private TelphoneState state;
31         /// <summary>
32         /// 打电话
33         /// </summary>
34         public void Open()
35         {
36             if (state == TelphoneState.Disposed)
37             {
38                 throw new Exception("电话线已经被剪断，无法打开！");
39             }
40             state = TelphoneState.Open;
41             Console.WriteLine("拿起电话");
42         }
43         /// <summary>
44         /// 说话
45         /// </summary>
46         /// <param name="s">说话内容</param>
47         public void Speak(string s)
48         {
49             if (state != TelphoneState.Open)
50             {
51                 throw new Exception("没有连接");
52             }
53             Console.WriteLine(s);
54         }
55         /// <summary>
56         /// 挂掉电话
57         /// </summary>
58         public void Bomb()
59         {
60             state = TelphoneState.Close;
61             Console.WriteLine("挂掉电话");
62         }
63         #region IDisposable 成员
64         /// <summary>
65         /// 实现IDisposable接口中的Dispose()方法来释放非托管资源
66         /// 如何释放非托管资源由程序自己定
67         /// </summary>
68         public void Dispose()
69         {
70             if (state == TelphoneState.Open)
71             {
72                 Bomb();//挂掉电话
73             }
74             state = TelphoneState.Disposed;
75             Console.WriteLine("剪断电话线");
76         }
77         #endregion
78     }
79     /// <summary>
80     /// 电话状态枚举
81     /// </summary>
82     enum TelphoneState
83     {
84         Open, Close, Disposed
85     }
86 }
```

程序运行结果:

![](https://images0.cnblogs.com/blog/289233/201402/082158346122959.png)

**Dispose中通常会顺便调用Close，这只是实现类自己的行为，没人强求**， IDisposable中的 Dispose方法根本不知道Close方法的存在，**甚至可以没有****close方法。（Close与Dispose关系 常考）**。没有规定Dispose一定会调用Close方法，但是很多的实现了IDisposable接口的方法中，通常都是代为调用Close之类的方法。一般的**close只是关闭，也许关闭后还能打开，但是Dispose一般就表示资源被回收了，再也没法用了。**有的类可能没有close方法。

Close和Dispose的区别：

1、**一个类如果实现****IDisposable接口，则肯定有Dispose这个方法。但是一个类可以没有Close方法，也可以不叫Close**。

2、即使像SqlConnection、FileStream这种有Close也有Dispose方法的，IDisposable接口也没有约定Dispose必须帮助Close，只不过**一般的****Dispose的实现都判断是否Close，如果没有Close则帮助Close，但是这是好心、不是义务**。









