# 一张图搞定OAuth2.0    在Office应用中打开WPF窗体并且让子窗体显示在Office应用上  彻底关闭Excle进程的几个方法    （七）Net Core项目使用Controller之... - weixin_33985507的博客 - CSDN博客
2018年05月04日 13:53:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：81
# [一张图搞定OAuth2.0](http://www.cnblogs.com/flashsun/p/7424071.html)
**目录**
- [1、引言](https://www.cnblogs.com/flashsun/p/7424071.html#_label0)
- [2、OAuth2.0是什么](https://www.cnblogs.com/flashsun/p/7424071.html#_label1)
- [3、OAuth2.0怎么写](https://www.cnblogs.com/flashsun/p/7424071.html#_label2)
[回到顶部](https://www.cnblogs.com/flashsun/p/7424071.html#_labelTop)
### 1、引言
本篇文章是介绍OAuth2.0中最经典最常用的一种授权模式：**授权码模式**
非常简单的一件事情，网上一堆神乎其神的讲解，让我不得不写一篇文章来终结它们。
一项新的技术，无非就是了解它**是什么**，**为什么**，**怎么用**。至于为什么，本篇文章不做重点探讨，网上会有各种文章举各种什么丢钥匙、发船票的例子供你去阅读，个人认为还是有些哗众取宠，没有聊到本质。
那我们就重点聊聊OAuth2.0**是什么**，**怎么用**。但首先在读本文之前，你要先对OAuth2.0有一定的了解，建议先读一下阮一峰的oauth2.0文章，直接看**“授权码模式”**即可，带着疑问再来读本文效果更好。
http://www.ruanyifeng.com/blog/2014/05/oauth_2_0.html
[回到顶部](https://www.cnblogs.com/flashsun/p/7424071.html#_labelTop)
### 2、OAuth2.0是什么
#### OAuth2.0是什么——豆瓣和QQ的故事
OAuth简单说就是一种授权的**协议**，只要授权方和被授权方遵守这个协议去写代码提供服务，那双方就是实现了OAuth模式。
举个例子，你想登录豆瓣去看看电影评论，但你丫的从来没注册过豆瓣账号，又不想新注册一个再使用豆瓣，怎么办呢？不用担心，豆瓣已经为你这种懒人做了准备，用你的qq号可以授权给豆瓣进行登录，请看。
**第一步：在豆瓣官网点击用qq登录**
**![](https://images2017.cnblogs.com/blog/1096103/201708/1096103-20170824150221230-104373567.png)**
**第二步：跳转到qq登录页面输入用户名密码，然后点授权并登录**
![](https://images2017.cnblogs.com/blog/1096103/201708/1096103-20170824151117089-994331290.png)
**第三步：跳回到豆瓣页面，成功登录**
![](https://images2017.cnblogs.com/blog/1096103/201708/1096103-20170824151220418-2113396046.png)
 这几秒钟之内发生的事情，在**无知的用户视角**看来，就是在豆瓣官网上输了个qq号和密码就登录成功了。在一些**细心的用户视角**看来，页面经历了从豆瓣到qq，再从qq到豆瓣的两次页面跳转。但作为一群专业的程序员，我们还应该从**上帝视角**来看这个过程。
#### OAuth2.0是什么——上帝视角
　　简单来说，上述例子中的豆瓣就是**客户端**，QQ就是**认证服务器**，OAuth2.0就是客户端和认证服务器之间由于相互**不信任**而产生的一个**授权协议**。呵呵，要是相互信任那QQ直接把自己数据库给豆瓣好了，你直接在豆瓣输入qq账号密码查下数据库验证就登陆呗，还跳来跳去的多麻烦。
　　先上一张图，该图描绘了只几秒钟发生的所有事情用**上帝视角**来看的流程
![](https://images2017.cnblogs.com/blog/1096103/201708/1096103-20170824142737402-1297004164.png)
 就这这张图，来说一下上述例子中的三个步骤在图中的表现。所用到的请求路径名称都是虚构的，所附带的请求参数忽略了一些非重点的。
如想了解每次的请求和响应的标准齐全的参数，还是去读那篇阮一峰的文章。http://www.ruanyifeng.com/blog/2014/05/oauth_2_0.html
**第一步：在豆瓣官网点击用qq登录**
　　当你点击用qq登录的小图标时，实际上是向豆瓣的服务器发起了一个 `http://www.douban.com/leadToAuthorize 的请求，豆瓣服务器会响应一个**重定向地址**，指向qq授权登录`
　　浏览器接到重定向地址 `http://www.qq.com/authorize?callback=www.douban.com/callback ，再次访问。并注意到这次访问带了一个参数是callback，以便qq那边授权成功再次让浏览器发起这个callback请求。不然qq怎么知道你让我授权后要返回那个页面啊，每天让我授权的像豆瓣这样的网站这么多。`
　　至于访问这个地址之后，qq那边做出怎样的回应，就是第二步的事情了。总之第一步即对应了图中的这些部分。
![](https://images2017.cnblogs.com/blog/1096103/201708/1096103-20170824155817777-2073704717.png)
**第二步：跳转到qq登录页面输入用户名密码，然后点授权并登录**
　　上一步中浏览器接到重定向地址并访问 
```
http://www.qq.com/authorize?callback=www.douban.com/callback
```
　　qq的服务器接受到了豆瓣访问的authorize，在次例中所给出的回应是跳转到qq的登录页面，用户输入账号密码点击授权并登录按钮后，一定还会访问qq服务器中校验用户名密码的方法，若校验成功，该方法会响应浏览器一个重定向地址，并附上一个**code（授权码）**。由于豆瓣只关心像qq发起authorize请求后会返回一个code，并不关心qq是如何校验用户的，并且这个过程每个授权服务器可能会做些个性化的处理，只要最终的结果是返回给浏览器一个重定向并附上code即可，所以这个过程在图中并没有详细展开。现把展开图画给大家。
![](https://images2017.cnblogs.com/blog/1096103/201708/1096103-20170824161339668-1419889465.png)
**第三步：跳回到豆瓣页面，成功登录**
 这一步背后的过程其实是最繁琐的，但对于用户来说是完全感知不到的。用户在QQ登录页面点击授权登陆后，就直接跳转到豆瓣首页了，但其实经历了很多隐藏的过程。
首先接上一步，QQ服务器在判断登录成功后，使页面重定向到之前豆瓣发来的callback并附上code授权码，即 `callback=www.douban.com/callback `
页面接到重定向，发起 `http://www.douban.com/callback 请求`
豆瓣服务器收到请求后，做了两件再次与QQ沟通的事，即模拟浏览器发起了两次请求。一个是用拿到的code去换token，另一个就是用拿到的token换取用户信息。最后将用户信息储存起来，返回给浏览器其首页的视图。到此OAuth2.0授权结束。
![](https://images2017.cnblogs.com/blog/1096103/201708/1096103-20170824162606793-113527222.png)
[回到顶部](https://www.cnblogs.com/flashsun/p/7424071.html#_labelTop)
### 3、OAuth2.0怎么写
了解了上述过程后，代码自然就不难写了，起码框架是可以写出来的。我在github上分享了一个我自己模拟的简单的不能再简单的oauth2.0，大家可以参考一下，仅仅用于了解oauth的过程，可别用于公司哦，不然老板得开除你。
github地址：https://github.com/sunym1993/dataU-OAuth.git/
如果无法下载，可以加我单独发。
项目结构非常简单，只有两个模块，分别是豆瓣和QQ，分别启动即可。
![](https://images2017.cnblogs.com/blog/1096103/201708/1096103-20170824170114308-69112592.png)
最终效果也非常简单清晰，下面请忍受low逼的显示效果
第一步
![](https://images2017.cnblogs.com/blog/1096103/201708/1096103-20170824170259480-1143143487.png)
第二步
![](https://images2017.cnblogs.com/blog/1096103/201708/1096103-20170824170344011-582269854.png)
第三步
![](https://images2017.cnblogs.com/blog/1096103/201708/1096103-20170824170425652-1206143903.png)
QQ授权服务器产生的token会被豆瓣写入到用户浏览器的cookie里面吗？
如果写入的话，那么该cookie的域是豆瓣还是QQ授权服务器的？
如果写入的话，那么是在那一步写入的，是最后一步吗？
麻烦给解释下以上三个问题。
您的问题已经超出了oauth本身，oauth只是一个协议，最终传回用户数据时协议的全过程已经结束，它的任务已经完成。至于服务怎么用这些数据，是存入cookie还是什么的，那不同的服务方会有不同的选择，因人而异了。
当然如果非要描述的话，如果豆瓣选择存入cookie，那域当然是豆瓣的域了，此时已与QQ无关。
我上传到码云了 [https://gitee.com/sunym1993/datauoauthqq_bean.git](https://gitee.com/sunym1993/datauoauthqq_bean.git)
# 理解OAuth 2.0
## [在Office应用中打开WPF窗体并且让子窗体显示在Office应用上](http://www.cnblogs.com/bluedoctor/p/8946215.html)
在.NET主程序中，我们可以通过创建 ExcelApplication 对象来打开一个Excel应用程序，如果我们想在Excle里面再打开WPF窗口，问题就不那么简单了。
我们可以简单的实例化一个WPF窗体对象然后在Office应用程序的窗体上打开这个新的WPF窗体，此时Office应用的窗体就是WPF的宿主窗体。然后宿主窗体跟Office应用并不是在一个UI线程上，子窗体很可能会在宿主窗体后面看不到。这个时候需要调用Win32函数，将Office应用的窗体设置为WPF子窗体的父窗体，这个函数的形式定义如下：
```
[DllImport("user32.dll", SetLastError = true)]
private static extern IntPtr SetParent(IntPtr hWndChild, IntPtr hWndNewParent);
```
由于Office应用程序是非托管程序，WPF窗体是托管程序，.NET提供了一个 WindowInteropHelper 包装类，它可以将一个托管程序窗体包装得到一个窗口句柄，之后，就可以调用上面的Win32函数 SetParent 设置窗口的父子关系了。
下面方法是一个完整的方法，可以通过反射实例化一个WPF窗体对象，然后设置此WPF窗体对象为Office应用程序的子窗体，并正常显示在Office应用程序上。
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
   /// <summary>
        /// 在Excle窗口上显示WPF窗体
        /// </summary>
        /// <param name="assemplyName">窗体对象所在程序集</param>
        /// <param name="paramClassFullName">窗体对象全名称</param>
        public static void ExcelShowWPFWindow(string assemplyName, string paramClassFullName)
        {
            Application.Current.Dispatcher.Invoke(new Action(() => {
                try
                {
                    Assembly assembly = Assembly.Load(assemplyName);
                    Type classType = assembly.GetType(paramClassFullName);
                    object[] constuctParms = new object[] { };
                    dynamic view = Activator.CreateInstance(classType, constuctParms);
                    Window winBox = view as Window;
                    var winBoxIntreop = new WindowInteropHelper(winBox);
                    winBoxIntreop.EnsureHandle();
                    //将Excel句柄指定为当前窗体的父窗体的句柄，参考 https://blog.csdn.net/pengcwl/article/details/7817111
                    //ExcelApp 是一个Excle应用程序对象
                    var excelHwnd = new IntPtr(OfficeApp.ExcelApp.Hwnd);
                    winBoxIntreop.Owner = excelHwnd;
                    SetParent(winBoxIntreop.Handle, excelHwnd);
                    winBox.ShowDialog();
                }
                catch (Exception ex)
                {
                    MessageBox.Show("打开窗口错误："+ex.Message);
                }
            }));
        }
    }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
 下面是打开的效果图：
![](https://images2018.cnblogs.com/blog/114517/201804/114517-20180426102701311-659766666.png)
不过，既然是的打开了一个模态窗口，我们当然是想获得窗口的返回值。在WinForms比较简单，但是在WPF就需要做下设置。
首先看到上图的WPF窗体的XAML定义：
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
<Window x:Class="MyWPF.View.Test"
        xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
        xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
        xmlns:d="http://schemas.microsoft.com/expression/blend/2008"
        xmlns:mc="http://schemas.openxmlformats.org/markup-compatibility/2006"
        xmlns:local="clr-namespace:MyWPF.View"
         DataContext="{Binding TestViewModel, Source={StaticResource MyViewModelLocator}}"
        mc:Ignorable="d"
        Title="Test" Height="300" Width="300">
    <Grid>
        <Grid.RowDefinitions>
            <RowDefinition/>
            <RowDefinition Height="80"/>
        </Grid.RowDefinitions>
        <TextBox Text="{Binding TestVale1}"/>
        <Button Content="sure" Grid.Row="1" HorizontalAlignment="Center" VerticalAlignment="Center" x:Name="TestBtn" Click="testBtn_Click"/>
    </Grid>
</Window>
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
窗体绑定了一个 TestViewModel1的ViewModel:
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
public class TestViewModel : EntityBase,IWindowReturnValue<string>
{
        public TestViewModel()
        {
        }
       
        public string TestValue1
        {
            get { return getProperty<string>("TestValue1"); }
            set {
                setProperty("TestValue1",value,1000);
                ReturnValue = value;
            }
        }
     
        public string ReturnValue { get; set; }
        public string BackTest()
        {
           return TestValue1;
        }
    }
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
TestViewModel 继承了SOD框架的实体类基类，它可以方便的实现MVVM的依赖属性，参考[SOD的MVVM实现原理](http://www.cnblogs.com/bluedoctor/p/6060278.html)。本文重点看IWindowReturnValue<T>接口的定义：
```
public interface IWindowReturnValue<T>
    {
        T ReturnValue { get; set; }
    }
```
接口很简单，就是定义一个返回值属性，这个属性在ViewModel 里面适当的时候给它赋值即可。
最后，我们改写下前面的Excle打开窗体的函数就可以了，代码如下：
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
 public static T ExcelShowWPFWindow<T>(string assemplyName, string paramClassFullName)
        {
            T result = default(T);
            Application.Current.Dispatcher.Invoke(new Action(() =>
            {
                try
                {
                    Assembly assembly = Assembly.Load(assemplyName);
                    Type classType = assembly.GetType(paramClassFullName);
                    object[] constuctParms = new object[] { };
                    dynamic view = Activator.CreateInstance(classType, constuctParms);
                    Window winBox = view as Window;
                    var winBoxIntreop = new WindowInteropHelper(winBox);
                    winBoxIntreop.EnsureHandle();
//将Excel句柄指定为当前窗体的父窗体的句柄，参考 https://blog.csdn.net/pengcwl/article/details/7817111
                    var excelHwnd = new IntPtr(OfficeApp.ExcelApp.Hwnd);
                    winBoxIntreop.Owner = excelHwnd;
                    SetParent(winBoxIntreop.Handle, excelHwnd);
                    var dataModel = winBox.DataContext as IWindowReturnValue<T>;
                    winBox.ShowDialog();
                    result = dataModel.ReturnValue;
                }
                catch (Exception ex)
                {
                    MessageBox.Show("打开窗口错误：" + ex.Message);
                }
            }));
            return result;
        }
    }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
最后运行此示例，测试通过。
## [彻底关闭Excle进程的几个方法](http://www.cnblogs.com/bluedoctor/p/8777208.html)
之前研究过的问题，最近有朋友问，这里再总结下做一个笔记。
我们在应用程序里面通过创建Excle应用对象打开Excle的情况下，如果不注意几个问题，可能无法彻底关闭Excle进程，来考察下面的几种情况：
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
        public static void startexcel()
        {
            var  excel = new Microsoft.Office.Interop.Excel.Application();
            excel.Visible = true;
            var book=  excel.Application.Workbooks.Open("D:\\Book1.xlsx");
        }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
上面的代码打开了一个工作簿，Excel启动了一个独立进程并且呈现界面给用户，不会再犯方法结束后关闭Excel。这种情况下本意是为了让用户决定何时关闭工作簿。
结果，当用户手工关闭工作簿后，Excle进程没有关闭，这是因为我们的.NET 托管代码打开的Excle的非托管代码，.NET运行时没有释放相关的句柄，需要加上下面几行代码来释放：
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
        public static void startexcel()
        {
            var  excel = new Microsoft.Office.Interop.Excel.Application();
            excel.Visible = true;
            var book=  excel.Application.Workbooks.Open("D:\\Book1.xlsx");
            System.Runtime.InteropServices.Marshal.ReleaseComObject(excel);
            excel = null;
            GC.Collect();
        }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
上面的代码，Marshal.ReleaseComObject 会释放COM组件对象，这里是excel，然后，代码设置 excel=null,这样紧接着执行垃圾回收才有效，否则，无法回收excel句柄。
注意，执行上面的代码并不会关闭了Excel进程，它只是释放了Excle进程句柄与.NET运行时的关系。
当用户在外面手工关闭Excle窗体后，Excle进程才会真正从任务管理器消失。
有朋友可能说，我没有加上面三行代码也能够关闭Excle进程啊。
没错，上面的代码只不过是立即释放了Excle这种非托管资源。注意到我们的 excle对象是一个局部对象，所以当方法结束后，excle对象已经在方法堆栈上被清空了，只需要在外面合适的时候调用下垃圾回收，即可实现彻底关闭Excle进程的效果：
```
startexcel();
GC.Collect();
Console.WriteLine("excel close ok.");
```
如果我们的Excel进程不是由用户关闭而是要程序自动关闭怎么办？
这个时候只需要调用Excle应用程序对象的关闭方法即可。
完整的代码如下，并且下面的代码演示了Excle进程打开一个宏文件，然后再打开工作簿，处理事件，最后关闭Excle窗体，关闭进程清理资源的功能。
Excle的工作簿保存和关闭事件有时候比较有用，比如保存工作簿的时候就上传一份工作簿副本到服务器。
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
 public static void startexcel()
        {
            var  excel = new Microsoft.Office.Interop.Excel.Application();
            excel.Visible = true;
            excel.Workbooks.Open("C:\\A1000.xla");
            var book=  excel.Application.Workbooks.Open("D:\\Book1.xlsx");
            excel.WorkbookBeforeSave += Excel_WorkbookBeforeSave;
            excel.WorkbookBeforeClose += Excel_WorkbookBeforeClose;
            book.Close();
            excel.Quit();
            System.Runtime.InteropServices.Marshal.ReleaseComObject(excel);
            excel = null;
            GC.Collect();
        }
        private static void Excel_WorkbookBeforeClose(Workbook Wb, ref bool Cancel)
        {
            Console.WriteLine("Excel 关闭，title：" + Wb.Title);
        }
        private static void Excel_WorkbookBeforeSave(Workbook Wb, bool SaveAsUI, ref bool Cancel)
        {
            Console.WriteLine("Excel保存，title："+Wb.Title);
        }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
注：
本文的做法，也适用于关闭Word等其它Office程序。
# [（七）Net Core项目使用Controller之二](http://www.cnblogs.com/chenyinxin/p/8990063.html)
**一、简介**
 1、说明Post,Get定义的区别。
2、说明如何路由定义。
**二、Get、Post定义**
1、api不定义访问方式时，同时支持get 和 post。如果定义某种方式，则仅支持某种方式。具体看代码及运行效果。
这里有个知识点，什么时候使用get,什么时候使用post，个人习惯能get则get,不能get则post，至于put,delete，从来不用。
api代码
```
![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
![复制代码](https://common.cnblogs.com/images/copycode.gif)
    public class OneController : Controller
    {
        [HttpGet]
        public string GetString(string id)
        {
            return "get:" + id;
        }
        [HttpPost]
        public string PostString(string id)
        {
            return "post:" + id;
        }
        public string NullString(string id)
        {
            return "null:" + id;
        }
    }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
html测试代码
```
![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
![复制代码](https://common.cnblogs.com/images/copycode.gif)
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8" />
    <title>示例代码</title>
    <script src="http://libs.baidu.com/jquery/2.0.0/jquery.min.js"></script>
    <script>
        $(function () {
            $.get("one/getstring", { id: "001" }, function (result) { console.log(result) });
            $.post("one/getstring", { id: "001" }, function (result) { console.log(result) });
            $.get("one/poststring", { id: "001" }, function (result) { console.log(result) });
            $.post("one/poststring", { id: "001" }, function (result) { console.log(result) });
            $.get("one/nullstring", { id: "001" }, function (result) { console.log(result) });
            $.post("one/nullstring", { id: "001" }, function (result) { console.log(result) });
        });
    </script>
</head><body></body>
</html>
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
结果
![](https://images2018.cnblogs.com/blog/1006345/201805/1006345-20180504115224573-1468946903.png)
定义了httpget,则post返回404，定义了httppost则get返回404,不定义则都能访问。
** 三、整个Controller定义路由**
 1、直接上代码，一看就懂。仔细观察访问路径。
整个Controller定义路由：api代码
```
![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
![复制代码](https://common.cnblogs.com/images/copycode.gif)
1     [Route("api/one")]
2     public class OneController : Controller
3     {
4         [Route("GetString")]
5         public string GetString(string id)
6         {
7             return "get:" + id;
8         }
9     }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
整个Controller定义路由：html代码
```
![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
![复制代码](https://common.cnblogs.com/images/copycode.gif)
 1 <!DOCTYPE html>
 2 <html>
 3 <head>
 4     <meta charset="utf-8" />
 5     <title>示例代码</title>
 6     <script src="http://libs.baidu.com/jquery/2.0.0/jquery.min.js"></script>
 7     <script>
 8         $(function () {
 9             $.get("api/one/getstring", { id: "001" }, function (result) { console.log(result) });
10             $.post("api/one/getstring", { id: "001" }, function (result) { console.log(result) });
11         });
12     </script>
13 </head><body></body>
14 </html>
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
整个Controller定义路由：效果
![](https://images2018.cnblogs.com/blog/1006345/201805/1006345-20180504120711042-536054489.png)
![](https://images2018.cnblogs.com/blog/1006345/201805/1006345-20180504120715124-592378299.png)
**四、单个方法定义路由。**
 1、直接上代码，一看就懂。仔细观察访问路径。
api代码
```
![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
![复制代码](https://common.cnblogs.com/images/copycode.gif)
1     public class OneController : Controller
2     {
3         [Route("api2/one/getstring")]
4         public string GetString(string id)
5         {
6             return "get:" + id;
7         }
8     }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
html代码，路径差别
```
![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
![复制代码](https://common.cnblogs.com/images/copycode.gif)
 1 <!DOCTYPE html>
 2 <html>
 3 <head>
 4     <meta charset="utf-8" />
 5     <title>示例代码</title>
 6     <script src="http://libs.baidu.com/jquery/2.0.0/jquery.min.js"></script>
 7     <script>
 8         $(function () {
 9             $.get("api2/one/getstring", { id: "001" }, function (result) { console.log(result) });
10             $.post("api2/one/getstring", { id: "001" }, function (result) { console.log(result) });
11         });
12     </script>
13 </head><body></body>
14 </html>
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
运行效果
![](https://images2018.cnblogs.com/blog/1006345/201805/1006345-20180504121325095-353907366.png)
![](https://images2018.cnblogs.com/blog/1006345/201805/1006345-20180504121330046-1883065817.png)
**五、既定义路由又定义访问方式**
1、直接上代码，一看就懂。仔细观察访问路径。
api代码
```
![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
![复制代码](https://common.cnblogs.com/images/copycode.gif)
1     public class OneController : Controller
2     {
3         [HttpGet("api3/one/getstring")]
4         public string GetString(string id)
5         {
6             return "get:" + id;
7         }
8     }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
html 代码
```
![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
![复制代码](https://common.cnblogs.com/images/copycode.gif)
 1 <!DOCTYPE html>
 2 <html>
 3 <head>
 4     <meta charset="utf-8" />
 5     <title>示例代码</title>
 6     <script src="http://libs.baidu.com/jquery/2.0.0/jquery.min.js"></script>
 7     <script>
 8         $(function () {
 9             $.get("api3/one/getstring", { id: "001" }, function (result) { console.log(result) });
10             $.post("api3/one/getstring", { id: "001" }, function (result) { console.log(result) });
11         });
12     </script>
13 </head><body></body>
14 </html>
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
运行效果
![](https://images2018.cnblogs.com/blog/1006345/201805/1006345-20180504121635337-1941502096.png)
![](https://images2018.cnblogs.com/blog/1006345/201805/1006345-20180504121640810-313543531.png)
**六、结论**
 1、通过以上对比，可以知道，路由的定义有几种方式，并且可以自由组合。
2、另一种是在Startup中定义，见下图。
3、在生产过程中，以上方法几乎不会用到，一般都是用Startup默认路由，因为我们只要一个可以访问的唯一地址而已。
![](https://images2018.cnblogs.com/blog/1006345/201805/1006345-20180504121942338-788329635.png)
BitAdminCore框架作者。 框架演示：http://bit.bitdao.cn 框架使用：https://github.com/chenyinxin/cookiecutter-bitadmin-core 框架交流：QQ群202426919
