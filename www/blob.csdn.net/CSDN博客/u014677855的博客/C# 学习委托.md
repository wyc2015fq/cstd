# C# 学习委托 - u014677855的博客 - CSDN博客
2018年08月29日 13:57:32[xiaobigben](https://me.csdn.net/u014677855)阅读数：100
## 目录
- - [目录](#目录)
- [一、什么是委托](#一什么是委托)
- [二、委托的声明](#二委托的声明)
- [三、委托的实例化](#三委托的实例化)
- [四、委托的用途](#四委托的用途)
- [五、委托的多播](#五委托的多播)
- [6 委托实现事件实例](#6-委托实现事件实例)
## 一、什么是委托
其实在做项目的过程中，应该也多多少少用过委托，但是当想清楚而流利地口述出委托到底是什么的时候，却又犯结巴，总觉得表达不到位。今天看了几篇博文和书本内容，干脆写成博客来帮助自己理解和记忆下委托。 
言归正传，什么是委托？
- 委托是一种引用类型，包含特定参数列表和方法。
- 委托类似于C或C++中的函数的指针，委托是存有对某个方法的引用的一种引用类型。
- 一旦声明一个委托，委托就可指向与其具有相同签名的方法。
- 当创建委托实例的时候，就会将方法作为参数传递给new语句，那么委托就持有了该方法。
- 调用委托的时候，就把传递给委托的参数传递给委托所指向的方法。
- 委托主要用于实现事件和回调方法。
## 二、委托的声明
委托声明决定了可由该委托引用的方法。委托可指向与其具有相同签名的方法。 
声明语法：
`delegate <return type> <delegate name> (parameter list)`
声明一个委托：
`public delegate int MyDelegate(string s);`
## 三、委托的实例化
一旦声明了委托类型，委托对象必须使用new关键字来创建，且与一个特定的方法相关。创建一个委托实例时，传递给new语句的参数就像方法调用一样书写，但是不带有参数。下面的实例演示了委托的声明、实例化和使用。
```
/// <summary>
    /// 声明一个委托，委托可指向与其具有相同签名的方法
    /// </summary>
    /// <param name="s"></param>
    public delegate void printString(string s);
    class Program
    {
        static void Main(string[] args)
        {
            //创建委托实例时，将方法传递给new语句，那么委托就持有了该方法
            printString ps1 = new printString(WriteToScreen);
            //创建委托实例，传递给new语句一个方法作为参数，委托指向了该方法
            printString ps2 = new printString(WriteToFile);
            ////调用委托时，把传递给委托的参数传递给了委托所指向的方法
            //ps1("i like u");
            //ps2("i like u");
            //将委托作为参数，使用委托回调方法
            sendString(ps1);
            sendString(ps2);
            Console.ReadKey();
        }
        // 该方法打印到控制台
        public static void WriteToScreen(string str)
        {
            Console.WriteLine("The String is: {0}", str);
        }
        // 该方法打印到文件
        public static void WriteToFile(string s)
        {
            FileStream fs;
            StreamWriter sw;
            fs = new FileStream("c:\\message.txt",
            FileMode.Append, FileAccess.Write);
            sw = new StreamWriter(fs);
            sw.WriteLine(s);
            sw.Flush();
            sw.Close();
            fs.Close();
        }
        /// <summary>
        /// 将委托作为参数，回调方法
        /// </summary>
        /// <param name="ps"></param>
        public static void sendString(printString ps)
        {
            ps("c# is so interesting.");
        }
    }
```
运行结果： 
![这里写图片描述](https://img-blog.csdn.net/20180829113527232?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180829113537783?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 四、委托的用途
实例1：
```
/// <summary>
    /// 声明一个委托，委托可指向与其具有相同签名的方法
    /// </summary>
    /// <param name="s"></param>
    public delegate int  NumberChanger(int n);
    class Program
    {
        static void Main(string[] args)
        {
            //创建委托实例，将方法传递给委托对象，委托持有该方法
            NumberChanger n1 = new NumberChanger(AddNum);
            NumberChanger n2 = new NumberChanger(MultNum);
            //把传递给委托实例的参数传递给委托指向的方法
            n1(25);
            Console.WriteLine("Vaule of num:{0}", getNum());
            n2(5);
            Console.WriteLine("Vaule of num:{0}", getNum());
            Console.ReadKey();
        }
        static int num = 10;
        public static int AddNum(int p)
        {
            num += p;
            return num;
        }
        public static int MultNum(int q)
        {
            num *= q;
            return num;
        }
        public static int getNum()
        {
            return num;
        }
    }
```
结果： 
![这里写图片描述](https://img-blog.csdn.net/20180829120029201?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 五、委托的多播
委托对象可使用+运算符进行合并。一个合并委托调用它所合并的两个委托。只有相同类型的委托可被合并。-运算符可用于从合并的委托种移除组件委托。 
使用委托的多播特点，可以创建一个委托被调用时要调用的方法的调用列表。这被称为委托的多播，也叫组播。 
下面实例演示了委托的多播：
```
/// <summary>
    /// 声明一个委托，委托可指向与其具有相同签名的方法
    /// </summary>
    /// <param name="s"></param>
    public delegate int  NumberChanger(int n);
    class Program
    {
        static void Main(string[] args)
        {
            NumberChanger n;
            //创建委托实例，将方法传递给委托对象，委托持有该方法
            NumberChanger n1 = new NumberChanger(AddNum);
            NumberChanger n2 = new NumberChanger(MultNum);
            n = n1;//创建委托的实例，n指向方法AddNum
            n += n2;//+运算符合并两个委托，n就是合并委托、一个合并委托调用它所合并的两个委托
            //那么调用合并委托n时，就可以创建委托被调用时要调用的方法的调用列表
            n(5);
            //调用多播，调用合并委托的方法调用列表
            Console.WriteLine("Vaule of num:{0}", getNum());
            Console.ReadKey();
        }
        static int num = 10;
        public static int AddNum(int p)
        {
            num += p;
            return num;
        }
        public static int MultNum(int q)
        {
            num *= q;
            return num;
        }
        public static int getNum()
        {
            return num;
        }
    }
```
运行结果为：`value of Num:75`
上面实例运行过程为：委托实例n合并了两个委托，那么也就创建了调用委托时要调用的方法的列表。合并委托n先指向n1指向的方法；再指向n2指向的方法。 
实例2：
```
public delegate void BuyTicket();
    class Program
    {
        static void Main(string[] args)
        {
            //将方法传递给new语句，创建委托的实例
            BuyTicket buy = new BuyTicket(MrZhang.BuyTicket);
            //合并了另一个委托实例
            buy += new BuyTicket(MrZhang.BuyMovieTicket);
            //此时创建了委托被调用时要调用的方法的调用列表：BuyTicket，BuyMovieTicket
            buy();
            Console.ReadKey();
        }
    }
    public class MrZhang
    {
        public static void BuyTicket()
        {
            Console.WriteLine("this is your ticket,please get it.");
        }
        public static void BuyMovieTicket()
        {
            Console.WriteLine("What's movie do you want to watch?");
        }
    }
```
运行结果： 
this is your ticket,please get it. 
What’s movie do you want to watch? 
上面的实例比较容易理解，主程序委托校长买张火车票，然后买完火车票后再买张电影票。就合并了两个委托，合并后的委托调用时调用两个委托所指向的方法。
## 6 委托实现事件实例
在项目中创建了一个自定义控件，用于显示界面中的顶部工具栏。工具栏中有一个按钮需要实现“关于系统”的功能。 
![这里写图片描述](https://img-blog.csdn.net/20180829134922843?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
将这个自定义控件放在不同的应用程序中要实现的功能自然是不一样的，那么就用到委托和事件了。 
定义一个委托：
`public delegate void lbAboutclickeventhander(object sender, EventArgs e);`
这个委托与双击“关于系统”按钮的事件相结合，来实现在不同的应用程序中，展示不同内容的功能。 
定义一个事件如下：
```
//定义一个按钮
public event lbAboutclickeventhander lbAboutclick;
//引发事件的条件：点击“关于系统”的按钮，就触发事件
private void lbAbout_Click(object sender, EventArgs e)
{
    //这个事件被触发
    if (lbAboutclick != null)
        lbAboutclick(this, e);//开始执行订阅类中的方法：
}
```
在主程序中调用的时候，注册这个事件：通过委托调用订阅器中的方法this.topNavi1_lbAboutclick
```
//注册事件
 this.topNavi1.lbAboutclick += new TopNavi.lbAboutclickeventhander(this.topNavi1_lbAboutclick);
```
一旦点击了自定义控件中“关于系统”的按钮，那么便触发了事件。开始执行注册事件时委托调用的方法，也就是主程序中的topNavi1_lbAboutclick（object sender,EventArgs e）.
```
private void topNavi1_lbAboutclick(object sender, EventArgs e)
{
     FrmAbout fb = new FrmAbout();
     fb.ShowDialog();
 }
```
执行结果便是主程序中给定的功能，显示当前系统中的“关于系统”内容。
