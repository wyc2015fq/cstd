# 10291 用Python忽略全部邀请 - 知乎
# 

.
　　昨晚写了一个 Python 程序，可以自动忽略知乎上的全部邀请。欢迎大 V 们试用！

　　链接：[https://github.com/MaigoAkisame/zhihu-ignore-invites](https://link.zhihu.com/?target=https%3A//github.com/MaigoAkisame/zhihu-ignore-invites)

　　下面是原理介绍。

## 一、忽略邀请时发生了什么

　　在“[邀请我回答的问题](https://www.zhihu.com/question/invited)”页面（下文简称“邀请页面”），点击一个邀请后的“忽略”按钮，可以发现这个邀请消失了，但页面并没有刷新。由此推测，点击“忽略”按钮时页面向服务器发送了一个 AJAX 请求。

　　在 Chrome 中按 F12 键调出开发者工具窗口，并切换至 Network → XHR 标签页。再点击一个邀请后的“忽略”按钮，发现果然出现了一个请求。

![](https://pic4.zhimg.com/dd2e81257c51c1f44d3a76bb46f82d8b_b.png)　　查看该请求的细节可以发现，这是一个 POST 请求，目标 URL 是 [https://www.zhihu.com/question/askpeople](https://www.zhihu.com/question/askpeople)，并带有如下的表单数据：

![](https://pic1.zhimg.com/97d031fa6d83160f128bbfefaf6f0c94_b.png)这里，op 表示操作， pass 应该就是“忽略”的意思。qid 是一个八位十进制数，代表问题的编号，你在浏览问题页面时，URL 最后的八位数字就是它。uid 是一个 32 位十六进制数，代表了用户的身份。_xsrf 是 cross-site request forgery（跨站请求伪造）的缩写，也是一个 32 位十六进制数，用来告诉服务器这确实是你本人在操作。

　　利用 Unix 或 Cygwin 中的 curl 工具，可以在命令行发送 POST 请求，如下。当然，这里的 qid 要换成下一个邀请的编号。![](https://pic2.zhimg.com/4827fd92ad4094b0c34b6423b8a6f34d_b.png)

结果返回一串乱码，刷新邀请页面，发现邀请并未消失。这是因为刚刚发送的 POST 请求不带 cookies，被服务器拒绝了。

　　在 Cookies 标签页下可以找到浏览器发送的 POST 请求所带有的 cookies，它们是各种意味不明的字符串。经试验，其中只有 _xsrf 和 z_c0 两项是必需的；_xsrf 的值与表单参数中的相同。把这两个 cookies 添加到用 curl 发送的 POST 请求中去，发现返回一个 json 对象；刷新邀请页面，第一个邀请果然就消失了。
![](https://pic4.zhimg.com/dae65febe5f84f0f0639191380569ecb_b.png)
## 二、批量忽略邀请

　　弄懂了忽略一个邀请是怎么操作的，就容易写出批量忽略邀请的脚本了。首先模拟登录，然后反复读取邀请页面（页面上会有最近的 20 个邀请），提取出页面上的问题编号，逐一发送 POST 请求忽略，直到不再有邀请为止。

　　利用 Python 里的 requests 库，可以发送 GET 或 POST 请求。与 curl 不同，它发送的请求必须带有 headers，不过我发现，必需的 headers 只有 User-Agent 这一项，而且值为空就行。 requests 库里有个 Session 类，其对象可以把 cookies 记忆起来，于是手动提取 _xsrf 和 z_c0 两项 cookies 的工作就可以省去了。

　　下面是各个步骤的做法，和它们的返回结果：
- **模拟登录**：需要向 [https://www.zhihu.com/login/email](https://www.zhihu.com/login/email) 页面发送一个 POST 请求，表单数据只需要 email（账号）和 password（密码），并不需要 captcha（验证码）。如果登录成功，会返回状态码 200（OK）和一个 json 对象，其中 msg（消息）项的值为字符串“登陆成功”（是的，有错别字）。
- **读取邀请页面**：只需向 [https://www.zhihu.com/question/invited](https://www.zhihu.com/question/invited) 发送一个 GET 请求，不需要任何表单数据。如果读取成功，会返回状态码 200（OK）和 HTML 格式的页面内容。由于我在手工打开邀请页面的时候，经常遇到 500 页面（“服务器提了一个问题”），所以我的脚本会重复读取直至成功。
- **忽略邀请**：需要向 [https://www.zhihu.com/question/askpeople](https://www.zhihu.com/question/askpeople) 发送一个 POST 请求，表单数据包括 op（值为 pass）、qid（问题编号）、uid（用户身份）、_xsrf。Cookies 由 Session 对象自动搞定。如果忽略成功，会返回状态码 200（OK）和一个 json 对象 {"r": 0, "msg": ""}。

　　忽略邀请时所需的 qid、uid、_xsrf 三项数据，都可以在邀请页面中找到。qid 就在邀请本身里面，在附近还可以找到邀请者的信息。uid 在页面中的一处脚本里，是“user_hash”后面的一串十六进制数；_xsrf 在页面底部的一个隐藏输入框里。
![](https://pic2.zhimg.com/b2320c8e08eabcd2a2387e933ab0405d_b.png)![](https://pic4.zhimg.com/665d4b8df6edcc1c9ad6d6f00c3b4007_b.png)
![](https://pic4.zhimg.com/f3b6bb96c202caf93053f220ea280737_b.png)　　使用 [BeautifulSoup](https://link.zhihu.com/?target=https%3A//www.crummy.com/software/BeautifulSoup/) 库，可以将 HTML 页面解析成树形结构，方便地提取上面所需的各项数据。

　　有了上面的准备，就可以写脚本了。忽略一个邀请大约用时 0.5 秒，效率还可以。不过，当忽略了几百个邀请后，我发现我的知乎账号因为访问太快被暂时冻结了╮(￣▽￣)╭，不过好在 1 个多小时后就恢复了正常。在现在的版本中，我只能每 5 秒忽略一个邀请，并在忽略 20 个页面（400 个邀请）后休息 1 分钟。

## 三、隐藏的“全部忽略”功能？

　　我在探索的过程中，意外地发现知乎可能有未公开的“全部忽略”功能。我是这么发现的：

　　首先，我试图定位忽略邀请的 POST 请求是在哪里发起的。邀请页面上的“忽略”按钮（其实是个超链接），其 href 值为“javascript:;”，也就是说发起 POST 请求的代码并不在此处。我依次打开邀请页面所加载的各个 JavaScript 文件，并搜索“askpeople”、“pass”等关键字，最终发现发起 POST 请求的代码位于 [https://www.zhihu.com/static/revved/-/js/closure/page-misc.13415dac.js](view-source:https://www.zhihu.com/static/revved/-/js/closure/page-misc.13415dac.js) 这个脚本中。将脚本美化后，我发现了这样几个函数：

![](https://pic2.zhimg.com/a918e6ff2b73a2477402e8533b98460d_b.png)　　其中 z.e.Zo 就是发起忽略邀请的 POST 请求的函数。但是，从 z.e.Pa 和 z.e.sQ 两个函数来看，[https://www.zhihu.com/question/askpeople](https://www.zhihu.com/question/askpeople) 这个地址还提供了“恢复邀请”和“忽略全部邀请”的功能，z.e.C 甚至表明页面上本应有一个“忽略全部邀请”的按钮！然而，当我试着用 curl 发送“忽略全部邀请”的 POST 请求（op = ignore_all，无 qid 参数）时，却得到了“服务器提了一个问题”的页面。

　　不知道知乎可不可以把这个“忽略全部邀请”功能公开出来呢？⊂(●∧●)⊃ [@知乎小管家](https://www.zhihu.com/people/3d198a56310c02c4a83efb9f4a4c027e)
## 四、参考文献

　　我在写脚本的过程中参考了下面两篇文章，在此向作者致谢。
- [怎样用 Python 设计一个爬虫模拟登陆知乎？ - Jean Green 的答案](https://www.zhihu.com/question/29925879/answer/54819870)

- [用 Python Requests 抓取知乎用户信息](https://link.zhihu.com/?target=http%3A//zihaolucky.github.io/using-python-to-build-zhihu-cralwer/)

