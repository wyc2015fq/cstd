# [asp.net]网站数据安全之验证码 - TaigaComplex求职中 - 博客园







# [[asp.net]网站数据安全之验证码](https://www.cnblogs.com/TaigaCon/archive/2012/08/20/2646941.html)





数据安全是网站实现必不可少的其中一环，其中最基本的就有防止暴力破解这一类的机器人攻击。

机器人攻击，顾名思义，单纯地由程序算法计算出用户名对应的密码，达到破解账户的功能。

机器人攻击的原理如下：

![](https://pic002.cnblogs.com/images/2012/421096/2012081922532359.jpg)

网页与服务器是通过http协议进行通信的，网页发出请求（request），经由服务器处理过后再把响应信息（response）返回给网页。

如果服务器不进行任何安全设置的话，机器人就可以通过循环，不断发送请求数据给服务器，比对服务器端的密码信息，就能达到暴力破解的效果。



为了伪造网页请求，首先需要做的就是知道网页发送了什么东西给服务器，因此需要截获网页发送给服务器的http报文。

以下是一个简单的asp.net登陆网页：

```
1 <body>
2     <form id="form1" runat="server">
3     <div>
4         <asp:TextBox ID="UID" runat="server" name="UID"></asp:TextBox><br />
5         <asp:TextBox ID="PW" runat="server" name="PW"></asp:TextBox>
6     </div>
7     <asp:Button ID="Button1" runat="server" Text="登录" onclick="Button1_Click" />
8     </form>
9 </body>
```



处理程序：

```
1 using System;
 2 using System.Collections.Generic;
 3 using System.Linq;
 4 using System.Web;
 5 using System.Web.UI;
 6 using System.Web.UI.WebControls;
 7 
 8 public partial class Login : System.Web.UI.Page
 9 {
10     protected void Page_Load(object sender, EventArgs e)
11     {
12 
13     }
14     protected void Button1_Click(object sender, EventArgs e)
15     {
16         if (UID.Text == "admin" && PW.Text == "123")
17         {
18             Response.Write("登陆成功");
19         }
20         else
21         {
22             Response.Write("登陆失败");
23         }
24     }
25 }
```

网页如下，输入用户名为admin，密码为111：

![](https://pic002.cnblogs.com/images/2012/421096/2012081923194382.jpg)

然后立即用监视工具（Fiddler）查看网页发送给服务器的请求报文：

![](https://pic002.cnblogs.com/images/2012/421096/2012081923250974.jpg)

从报文中，可以找到PW=111的字样，这就是我们发送过去的密码，通过循环地修改这个值，就能完成暴力破解。



机器人破解程序如下：

```
1 using System;
 2 using System.Collections.Generic;
 3 using System.Linq;
 4 using System.Text;
 5 using System.Net;
 6 
 7 namespace ConsoleApplication1
 8 {
 9     class Program
10     {
11         static void Main(string[] args)
12         {
13             WebClient wc = new WebClient();
14             wc.Encoding = Encoding.UTF8;
15             for (int i = 0; i < 5000; i++)
16             {
17                 string recv = wc.DownloadString("http://localhost:34581/Login/Login.aspx?__VIEWSTATE=%2FwEPDwULLTEzNjkxMzkwNjRkZItmCBZEBtaljaITi%2BP9tOSzd0a1hsw6qmnZTsRWpzng&__EVENTVALIDATION=%2FwEdAATO0aG3gZnScLq%2B3YhzJsDQVbQGLX9gL7AHTDMv6bbPFxoYRGJFkVe8AgQLV57tKfLN%2BDvxnwFeFeJ9MIBWR693X4PRaFD34N5nly1a8ZxUF2WtsMvHQaZ3G0IQUv6PL2A%3D&UID=admin&PW=" + i + "&Button1=%E7%99%BB%E5%BD%95");
18                 if (recv.Contains("登陆成功"))
19                 {
20                     Console.WriteLine("密码：" + i);
21                     break;
22                 }
23                 
24             }
25         }
26     }
27 }
```



**另外，其实也可以通过C#中的webBrowser控件（  getElementById().SetAttribute()  ）来实现机器人**



验证码就是一个防止暴力破解的很好方法。

验证码实现如下图：

![](https://pic002.cnblogs.com/images/2012/421096/2012082005164644.jpg)



分为以下几个步骤：

1、用户加载网页，页面发出生成新验证码的请求

2、一般处理程序受到请求后生成验证码，其中包含两部分：图片和相对应的字串（存于session）。

3、一般处理程序返回响应，网页加载该验证码图片。

4、填写验证码，网页发出请求，要求服务器的处理程序处理。

5、处理程序从session获取字串，与网页请求的验证码进行对比。



网页代码如下：

```
1 <body>
 2     <form id="form1" runat="server">
 3     <div>
 4     <img src="Handler.ashx" />
 5     </div>
 6     <asp:TextBox ID="TextBox1" runat="server"></asp:TextBox>
 7     <br />
 8     <asp:Button ID="Button1" runat="server" Text="登陆" onclick="Button1_Click" />
 9     </form>
10 </body>
```

<img src="Handler.ashx"/>这一句就是加载验证码图片。当加载网页的时候，会请求Handler.ashx处理程序处理，要求其提供验证码图片。



生成验证码的一般处理程序代码如下：

```
1 <%@ WebHandler Language="C#" Class="Handler" %>
 2 
 3 using System;
 4 using System.Web;
 5 
 6 public class Handler : IHttpHandler, **System.Web.SessionState.IRequiresSessionState**{//必须加上System.Web.SessionState.IRequiresSessionState
 7     public void ProcessRequest (HttpContext context) {
 8         context.Response.ContentType = "image/JPEG";        //说明响应的格式是JPEG
 9         using (System.Drawing.Bitmap bitmap = new System.Drawing.Bitmap(100, 50))            //生成位图
10         {
11             using (System.Drawing.Graphics graph = System.Drawing.Graphics.FromImage(bitmap))      //生成画布
12             {
13                   //以下这段为生成随机数，并把其存到session内
14                 Random rand = new Random();
15                 int code=rand.Next(1000, 9999);
16                 string strCode=code.ToString();
17                 HttpContext.Current.Session["code"] = strCode;
18 
19                      //下面这段是生成验证码图片
20                 graph.DrawString(strCode, new System.Drawing.Font("宋体", 30), System.Drawing.Brushes.Green, new System.Drawing.PointF(0, 0));
21                 bitmap.Save(**context.Response.OutputStream**, System.Drawing.Imaging.ImageFormat.Jpeg);   //位图保存在响应的输出流中
22             }
23         }
24     }
25  
26     public bool IsReusable {
27         get {
28             return false;
29         }
30     }
31 
32 }
```

一般处理程序生成验证码的bitmap，并存到响应（response）的输出流中，网页获得响应信息后把bitmap显示在网页上



为了实现点击验证码图片后，自动更换验证码，可以作如下修改

```
<img src="Handler.ashx" onclick="this.src='Handler.ashx?aaa='+new Date()"/>
```

点击图片后，浏览器会判断当前的URL与目标URL是否一样，如果一样则不会做任何响应，否则会向目标URL发出请求。

由于new Date()产生的信息一直变化，因此能保证不同的两秒内会产生不同的URL，从而达到点击更换的效果。












