# [asp.net]webbrowser循环处理之Application.DoEvents() - TaigaComplex求职中 - 博客园







# [[asp.net]webbrowser循环处理之Application.DoEvents()](https://www.cnblogs.com/TaigaCon/archive/2012/08/22/2650087.html)





最近被webbrowser搞昏了，就是上一篇文章中关于暴力破解谈到的webbrowser方法。

在webbrowser中，本人写的是模拟人手，多次改变密码，点击按钮实现的破解方法，不过出现了问题。

代码如下：

```
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;

namespace CrackBrowser
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        private void button1_Click(object sender, EventArgs e)
        {
            for (int i = 0; i <= 10 ; i++)
            {
                webBrowser1.Document.GetElementById("UID").SetAttribute("value", "admin");
                webBrowser1.Document.GetElementById("PW").SetAttribute("value", i.ToString());
                webBrowser1.Document.GetElementById("Button1").InvokeMember("click");   //模拟点击
                if (webBrowser1.Document.Body.InnerHtml.Contains("登陆成功"))
                {
                    MessageBox.Show("登陆成功");

                }
            }

        }

    }
}
```

原本很简单的想法，却无法实现。

本来是发送了11次请求，结果是只显示最后的一次响应数据。

然后想了下，从webbrowser发送数据到接收响应数据，需要一段时间，因此给代码加上了休眠代码：

```
Thread.sleep(2000);
```

结果还是不行，从Fiddler上看，服务器已近返回了全部数据，可是运行结果没有改变。

后来从网上找到解决方法，就是在Thread.Sleep(2000);后面加上一句

```
Application.DoEvents();
```

关于这句话的解释，MSDN解析为：**处理当前在消息队列中的所有 Windows 消息。**

这么一来，就能清晰知道webbrowser只显示最后一项响应的问题所在了。

 由于本程序采用的是单线程，所以在上面的代码执行期间，程序不会进行其他操作，因此也不会去执行服务器返回的响应，

当上面代码执行完毕后，for循环已经提交了最后一个请求，并且webbrowser显示了服务器的最后一个响应。

而前面的请求虽然已经提交，并且服务器也返回了，不过webbrowser并没有去重绘页面，因为for循环一直在占用这个线程，使得webbrowser一直没机会去重绘页面。

当加入了Application.DoEvents();这句代码后，如MSDN所描述，处理器会处理当前在消息队列中的所有Windows消息，当然也包括重绘webbrowser页面的消息。

最终代码如下：

```
1 using System;
 2 using System.Collections.Generic;
 3 using System.ComponentModel;
 4 using System.Data;
 5 using System.Drawing;
 6 using System.Linq;
 7 using System.Text;
 8 using System.Windows.Forms;
 9 using System.Threading;
10 
11 namespace CrackBrowser
12 {
13     public partial class Form1 : Form
14     {
15         public Form1()
16         {
17             InitializeComponent();
18         }
19        
20         private void button1_Click(object sender, EventArgs e)
21         {
22           
23             for (int i = 0; i <= 10 ; i++)
24             {
25                 webBrowser1.Document.GetElementById("UID").SetAttribute("value", "admin");
26                 webBrowser1.Document.GetElementById("PW").SetAttribute("value", i.ToString());
27                 webBrowser1.Document.GetElementById("Button1").InvokeMember("click");
28 
29                 Thread.Sleep(2000); Application.DoEvents();
30                 if (webBrowser1.Document.Body.InnerHtml.Contains("登陆成功"))
31                 {
32                     MessageBox.Show("登陆");
33 
34                 }
35             }
36 
37         }
38     }
39 }
```

另外，也可以用多线程处理以上问题，不过多线程需要解决的问题就是协调循环跟页面加载的同步问题

当然，以上只是演示性的代码，等待webbrowser加载HTML页已近耗费了相当长的时间，当暴力破解的每一次循环都花费超过两秒的话，暴力破解已经失去的他的意义












