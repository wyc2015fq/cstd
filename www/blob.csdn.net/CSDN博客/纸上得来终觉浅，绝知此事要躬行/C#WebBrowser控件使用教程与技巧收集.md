# C#WebBrowser控件使用教程与技巧收集 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年09月09日 13:52:27[boonya](https://me.csdn.net/boonya)阅读数：1063标签：[WindowsCE																[webbrowser](https://so.csdn.net/so/search/s.do?q=webbrowser&t=blog)](https://so.csdn.net/so/search/s.do?q=WindowsCE&t=blog)
个人分类：[C#](https://blog.csdn.net/boonya/article/category/1776321)








**文章来自：[http://www.sufeinet.com/thread-3941-1-1.html](http://www.sufeinet.com/thread-3941-1-1.html)**

**先来看看常用的方法**


```
Navigate(string urlString):浏览urlString表示的网址
Navigate(System.Uri url)：浏览url表示的网址
Navigate(string urlString, string targetFrameName, byte[] postData, string additionalHeaders): 浏览urlString表示的网址，并发送postData中的消息
//（通常我们登录一个网站的时候就会把用户名和密码作为postData发送出去)
GoBack()：后退
GoForward()：前进
Refresh()：刷新
Stop()：停止
GoHome()：浏览主页
WebBrowser控件的常用属性：
Document：获取当前正在浏览的文档
DocumentTitle：获取当前正在浏览的网页标题
StatusText：获取当前状态栏的文本
Url：获取当前正在浏览的网址的Uri
ReadyState：获取浏览的状态
WebBrowser控件的常用事件：
DocumentTitleChanged,
CanGoBackChanged,
CanGoForwardChanged,
DocumentTitleChanged,
ProgressChanged,
ProgressChanged
DocumentCompleted 页面加载完成之后的事件
```


**1、获取非input控件的值：**


```
webBrowser1.Document.All["控件ID"].InnerText;
或webBrowser1.Document.GetElementById("控件ID").InnerText;
或webBrowser1.Document.GetElementById("控件ID").GetAttribute("value");
```

**2.获取input控件的值：**


```
webBrowser1.Document.All["控件ID"].GetAttribute("value");;
或webBrowser1.Document.GetElementById("控件ID").GetAttribute("value");
```

**3、给输入框赋值：**



```
//输入框
user.InnerText = "myname";
password.InnerText = "123456";
webBrowser1.Document.GetElementById("password").SetAttribute("value", "Welcome123");
```


**4、下拉、复选、多选：**



```
//下拉框：
secret.SetAttribute("value", "question1");  
//复选框
rememberme.SetAttribute("Checked", "True");
//多选框
cookietime.SetAttribute("checked", "checked");
```


**5、根据已知有ID的元素操作没有ID的元素：**


`HtmlElement btnDelete = webBrowser1.Document.GetElementById(passengerId).Parent.Parent.Parent.Parent.FirstChild.FirstChild.Children[1].FirstChild.FirstChild;`


根据Parent,FirstChild,Children[1]数组，多少层级的元素都能找到。
**6、获取Div或其他元素的样式：**

`webBrowser1.Document.GetElementById("addDiv").Style;`
**7、直接执行页面中的脚本函数，带动态参数或不带参数都行**：


```
Object[] objArray = new Object[1];
objArray[0] = (Object)this.labFlightNumber.Text;
webBrowser1.Document.InvokeScript("ticketbook", objArray);
webBrowser1.Document.InvokeScript("return false");
```

**8、自动点击、自动提交：**


```
HtmlElement btnAdd = doc.GetElementById("addDiv").FirstChild;
btnAdd.InvokeMember("Click");
```


**9、自动赋值，然后点击提交按钮的时候如果出现脚本错误或一直加载的问题，一般都是点击事件执行过快，这时需要借助Timer控件延迟执行提交按钮事件：**


```
this.timer1.Enabled = true;
this.timer1.Interval = 1000 * 2;
private void timer1_Tick(object sender, EventArgs e)
{
    this.timer1.Enabled = false;
    ClickBtn.InvokeMember("Click");//执行按扭操作
}
```

**10、屏蔽脚本错误：**

`将WebBrowser控件ScriptErrorsSuppressed设置为True即可`
**11、自动点击弹出提示框：**


```
private void webBrowser1_Navigated(object sender, WebBrowserNavigatedEventArgs e)
{
  //自动点击弹出确认或弹出提示
  IHTMLDocument2 vDocument = (IHTMLDocument2)webBrowser1.Document.DomDocument;
  vDocument.parentWindow.execScript("function confirm(str){return true;} ", "javascript"); //弹出确认
  vDocument.parentWindow.execScript("function alert(str){return true;} ", "javaScript");//弹出提示
}
```

**12.WebBrowser页面加载完毕之后，在页面中进行一些自动化操作的时候弹出框的自动点击(屏蔽)**



```
private void webBrowser1_DocumentCompleted(object sender, WebBrowserDocumentCompletedEventArgs e)
{
    //自动点击弹出确认或弹出提示
    IHTMLDocument2 vDocument = (IHTMLDocument2)webBrowser1.Document.DomDocument;
    vDocument.parentWindow.execScript("function confirm(str){return true;} ", "javascript"); //弹出确认
    vDocument.parentWindow.execScript("function alert(str){return true;} ", "javaScript");//弹出提示
    //下面是你的执行操作代码
}
```

**13、获取网页中的Iframe，并设置Iframe的src**


```
HtmlDocument docFrame = webBrowser1.Document.Window.Frames["mainFrame"].Document; 
或
HtmlDocument docFrame = webBrowser1.Document.All.Frames["mainFrame"].Document; 
docFrame.All["mainFrame"].SetAttribute("src", "http://www.sufeinet.com/");
```


网页中存在Iframe的时候webBrowser1.Url和webBrowser1_DocumentCompleted中的e.Url不一样，前者是主框架的Url,后者是当前活动框口的Url。
**14、让控件聚焦**


```
this.webBrowser1.Select();
this.webBrowser1.Focus();
doc.All["TPL_password_1"].Focus();
```

**15、打开本地网页文件**

`webBrowser1.Navigate(Application.StartupPath + @"\Test.html");`
**16、获取元素、表单**


```
//根据Name获取元素
public HtmlElement GetElement_Name(WebBrowser wb,string Name)
{
    HtmlElement e = wb.Document.All[Name];
    return e;
}
 
//根据Id获取元素
public HtmlElement GetElement_Id(WebBrowser wb, string id)
{
    HtmlElement e = wb.Document.GetElementById(id);
    return e;
}
 
//根据Index获取元素
public HtmlElement GetElement_Index(WebBrowser wb,int index)
{
    HtmlElement e = wb.Document.All[index];
    return e;
}
 
//获取form表单名name,返回表单
public HtmlElement GetElement_Form(WebBrowser wb,string form_name)
{
    HtmlElement e = wb.Document.Forms[form_name];
    return e;
}
 
 
//设置元素value属性的值
public void Write_value(HtmlElement e,string value)
{
    e.SetAttribute("value", value);
}
 
//执行元素的方法，如：click，submit(需Form表单名)等
public void Btn_click(HtmlElement e,string s)
{
 
    e.InvokeMember(s);
}
```

**17.获取Cookie**


```
[DllImport("wininet.dll", CharSet = CharSet.Auto, SetLastError = true)]
      static extern bool InternetGetCookieEx(string pchUrl, string pchCookieName, StringBuilder pchCookieData, ref System.UInt32 pcchCookieData, int dwFlags, IntPtr lpReserved);
      private static string GetCookieString(string url)
      {
          uint datasize = 1024;
          StringBuilder cookieData = new StringBuilder((int)datasize);
          if (!InternetGetCookieEx(url, null, cookieData, ref datasize, 0x2000, IntPtr.Zero))
          {
              if (datasize < 0)
                  return null;
              cookieData = new StringBuilder((int)datasize);
              if (!InternetGetCookieEx(url, null, cookieData, ref datasize, 0x00002000, IntPtr.Zero))
                  return null;
          }
          return cookieData.ToString();
      }
      private void webBrowser1_DocumentCompleted_1(object sender, WebBrowserDocumentCompletedEventArgs e)
      {
          richTextBox1.Text = string.Empty;
          if (cbcookie.Checked)
          {
              if (checkBox1.Checked)
              {
                  richTextBox1.Text = GetCookieString(textBox1.Text.Trim());
              }
              else
              {
                  richTextBox1.Text = webBrowser1.Document.Cookie;
              }
          }
      }
```

**18.怎么设置代理**
[http://www.sufeinet.com/thread-2242-1-1.html](http://www.sufeinet.com/thread-2242-1-1.html)
**19.怎么在加载完成某个页面之后执行代码**


```
//本事件是当每次加载完成当前页面后才会执行的
        private void webBrowser1_DocumentCompleted(object sender, WebBrowserDocumentCompletedEventArgs e)
        {
            //e.Url是当前加载的页面，
            if (e.Url.ToString().Contains("http://sufeinet.com"))
            {
                //执行操作1
            }
            else if (e.Url.ToString().Contains("http://baidu.com"))
            {
                //执行操作2
            }
        }
```

**20.怎么禁止在新窗口中打开网页**


```
private void webBrowser1_NewWindow(object sender, CancelEventArgs e)
{
        string url = ((System.Windows.Forms.WebBrowser)sender).StatusText;
        webBrowser1.Navigate(url);
        e.Cancel = true;
     
}
```


**21.怎么设置Cookie**


`<span style="font-size:10px;color:#000000;">webBrowser1.Document.Cookie=“你的Cookie值”;</span>`




