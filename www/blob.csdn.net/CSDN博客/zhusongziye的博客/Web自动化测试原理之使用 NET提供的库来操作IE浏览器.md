# Web自动化测试原理之使用.NET提供的库来操作IE浏览器 - zhusongziye的博客 - CSDN博客





2018年11月20日 20:41:09[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：93








### 目前市面上有很多Web UI自动化测试框架，比如WatiN, Selinimu,WebDriver，还有VS2010中的Coded UI等等。这些框架都可以操作Web中的控件，模拟用户输入，点击等操作，实现Web自动化测试。其实这些工具的原理都一样，都是通过调用IE COM接口和HTML DOM 对IE浏览器以及WEB测试对象的操作。



**本文介绍脱离这些自动化测试框架。  直接使用.NET提供的shdocvm.dll库来操作IE浏览器，使用mshtml.dll库来操作IE中的 HTML对象。**

阅读目录
- 
优点

- 
添加引用

- 
操作IE

- 
使用IE Develop Toolbar 查看HTML DOM

- 
操作IE中的HTML元素


# 优点

通过直接操作IE COM来实现Web自动化，能让你在几分钟之内快速建立一个轻量型的自动化测试程序。 大大的提高了测试效率。 也有助于你理解WatiN这些自动化测试框架的运行原理.

# 添加引用

shdocvm.dll和mshtml.dll这两个库的COM组件名字和他们的dll名字不一样。所以比较难找。

shdocvm.dll 的COM 组件名字叫"Microsoft Internet Controls". 添加引用如下Add References->Com Tab-> Microsoft Internet Controls

![](https://img-blog.csdnimg.cn/20181120203713846.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

mshtml.dll的COM组件名字叫"Microsoft.mshtml", 添加引用如下Add References-> .NET Tab->Microsoft.mshtml

![](https://img-blog.csdnimg.cn/20181120203724452.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

添加完引用后，就可以引用命名空间了



using mshtml;
using SHDocVw;


# 操作IE

通过shdocvm.dll中的InternetExplorer对象的属性和方法， 比如Height,Width。我们能够操作IE，以便模拟一些用户的操作，比如调整浏览器的大小，刷新页面等。



```
static void Main(string[] args)
{      
    InternetExplorer IE = new InternetExplorer();
    IE.Visible = true;
    object nil = new object();
    string CnblogUrl = "http://www.cnblogs.com";
    // 打开IE并且打开博客园主页
    IE.Navigate(CnblogUrl, ref nil, ref nil, ref nil, ref nil);
    Thread.Sleep(3000);

    // 设置IE左上角的位置
    IE.Top = 10;
    IE.Left = 10;
    // 设置IE的高度和宽度
    IE.Height = 800;
    IE.Width = 1000;
    // 导航到百度主页
    Thread.Sleep(3000);
    string BaiduUrl = "http://www.baidu.com";
    IE.Navigate(BaiduUrl, ref nil, ref nil, ref nil, ref nil);

    // 点击后退按钮
    Thread.Sleep(3000);
    IE.GoBack();

    // 刷新IE
    Thread.Sleep(3000);
    IE.Refresh();

    // 关闭IE
    Thread.Sleep(3000);
    IE.Quit();
}
```





# 使用IE develop toolbar 查看HTML DOM

下节中我们要操作HTML中的元素， 我们先需要知道这些控件的id,  使用IE Develop Toolbar或者firebug可以方便查看控件的id.

按F12启动IE Develop Toolbar, 或者点击IE工具栏中的Tools->Develop Tools

![](https://img-blog.csdnimg.cn/20181120203848626.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



 从图中可以看到博客园中的搜索textbox的id是"q"



# 操作IE中的HTML元素

模拟一个在博客园首页进行搜索的场景。 模拟在文本框中输入数据， 然后点击"找找看按钮"。

 我们通过mshtml.dll库里的getElementById()方法获得你要操作的HTML元素的引用。 然后你可以操作这个对象，比如输入值和点击它

```
static void Main(string[] args)
{      
    InternetExplorer IE = new InternetExplorer();
    IE.Visible = true;
    object nil = new object();
    string CnblogUrl = "http://www.cnblogs.com";
    // 打开IE并且打开博客园主页
    IE.Navigate(CnblogUrl, ref nil, ref nil, ref nil, ref nil);
    Thread.Sleep(3000);

    // 获取DOM对象
    HTMLDocument doc = (HTMLDocument)IE.Document;

    // 博客园主页上的搜索Textbox的id是 "q"
    HTMLInputElement SearchTextBox = (HTMLInputElement)doc.getElementById("q");
    SearchTextBox.value = "小坦克";

    // 博客园主页上的"找找看"按钮的id是"btnBloggerSearch"
    HTMLInputElement SearchButton = (HTMLInputElement)doc.getElementById("btnBloggerSearch");
    SearchButton.click();
}
```



运行效果如下:

![](https://img-blog.csdnimg.cn/20181120203938547.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)





