# 谁需要 GUI？—— Linux 终端生存之道 - 文章 - 伯乐在线
原文出处： [Bryan Lunduke](http://www.networkworld.com/article/3091139/linux/who-needs-a-gui-how-to-live-in-a-linux-terminal.html#slide1)   译文出处：[GHLandy](https://linux.cn/article-7980-1.html)
> 
完全在 Linux 终端中生存并不容易，但这绝对是可行的。
### 处理常见功能的最佳 Linux shell 应用
你是否曾想像过完完全全在 Linux 终端里生存？没有图形桌面，没有现代的 GUI 软件，只有文本 —— 在 Linux shell 中，除了文本还是文本。这可能并不容易，但这是绝对可行的。我最近尝试完全在 Linux shell 中生存30天。下边提到的就是我最喜欢用的 shell 应用，可以用来处理大部分的常用电脑功能（网页浏览、文字处理等）。这些显然有些不足，因为纯文本操作实在是有些艰难。
### 在 Linux 终端里发邮件
![](http://jbcdn2.b0.upaiyun.com/2016/11/f82d487b92a75a4d8f3534c818f3c608.png)
要在终端里发邮件，选择有很多。很多人会推荐 mutt 和 notmuch，这两个软件都功能强大并且表现非凡，但是我却更喜欢 alpine。为何？不仅是因为它的高效性，还因为如果你习惯了像 Thunderbird 之类的 GUI 邮件客户端，你会发现 alpine 的界面与它们非常相似。
### 在 Linux 终端里浏览网页
![](http://jbcdn2.b0.upaiyun.com/2016/11/ded9abc327c6066ea5d5cfa28b8726aa.jpg)
我有一个词要告诉你：[w3m](https://en.wikipedia.org/wiki/W3m)。好吧，我承认这并不是一个真实的词。但 w3m 的确是我在 Linux 终端的 web 浏览器选择。它能够很好的呈现网页，并且它也足够强大，可以用来在像 Google+ 之类的网站上发布消息（尽管方法并不有趣）。 Lynx 可能是基于文本的 Web 浏览器的事实标准，但 w3m 还是我的最爱。
### 在 Linux 终端里编辑文本
![](http://jbcdn2.b0.upaiyun.com/2016/11/36e74122a4d085175b54d5178bfb3e01.jpg)
对于编辑简单的文本文件，有一个应用是我最的最爱。不！不！不是 emacs，同样，也绝对不是 vim。对于编辑文本文件或者简要记下笔记，我喜欢使用 nano。对！就是 nano。它非常简单，易于学习并且使用方便。当然还有更多的软件具有更多功能，但 nano 的使用是最令人愉快的。
### 在 Linux 终端里处理文字
![](http://jbcdn2.b0.upaiyun.com/2016/11/35df4c920b1518ba4c8cbac800021591.jpg)
在一个只有文本的 shell 之中，“文本编辑器” 和 “文字处理程序” 实在没有什么大的区别。但是像我这样需要大量写作的，有一个专门用于长期写作的软件是非常必要的。而我最爱的就是 wordgrinder。它由足够的工具让我愉快工作——一个菜单驱动的界面（使用快捷键控制）并且支持 OpenDocument、HTML 或其他等多种文件格式。
### 在 Linux 终端里听音乐
![](http://jbcdn2.b0.upaiyun.com/2016/11/a2a6f5715e689e202898390911f7b724.jpg)
当谈到在 shell 中播放音乐（比如 mp3，ogg 等），有一个软件绝对是卫冕之王：[cmus](https://en.wikipedia.org/wiki/Cmus)。它支持所有你想得到的文件格式。它的使用超级简单，运行速度超级快，并且只使用系统少量的资源。如此清洁，如此流畅。这才是一个好的音乐播放器的样子。
### 在 Linux 终端里发送即时消息
![](http://jbcdn2.b0.upaiyun.com/2016/11/24b8e59020f305a8c564ab50a532e1a2.jpg)
当我在想如果可以在终端里发送即时消息会是什么样子的时候，我的思绪瞬间爆发了。你可能知道 Pidgin——一个支持多种协议的 IM 客户端，它也有一个终端版，叫做“[finch](https://developer.pidgin.im/wiki/Using%20Finch)”，你可以使用它来同时链接多个网络、同时和几个人聊天。而且，它的界面也和 Pidgin 极为相似。多么令人惊叹啊！想要使用 Google 环聊（Google Hangouts）就试试 [hangups](https://github.com/tdryer/hangups)。它有一个非常漂亮的分页式界面，并且效果非常好。认真来说，除了一些可能需要的 emoji 表情和嵌入式图片外，在终端里发送即时消息真的是一个很好的体验。
### 在 Linux 终端里发布推文
![](http://jbcdn2.b0.upaiyun.com/2016/11/f730b7198ab7f7d3c7a0ebc43085eba1.jpg)
这不是开玩笑！由于 [rainbowstream](http://www.rainbowstream.org/) 的存在，我们已经可以在终端里发布推文了。尽管我时不时遇到一些 bug，但整体上，它工作得很好。虽然没有网页版 Twitter 或官方移动客户端那么好用，但这是一个终端版的 Twitter，来试一试吧。尽管它的功能还未完善，但是用起来还是很酷，不是吗？
### 在 Linux 终端里看 Reddit 新闻
![](http://jbcdn2.b0.upaiyun.com/2016/11/6ba739012dfbee5f25a9889ac65e894b.jpg)
不管如何，在命令行中享受 Reddit 新闻时间真的感觉很棒。使用 rtv 真是一个相当愉快的体验。不管是阅读、评论，还是投票表决，它都可以。其体验和在网页版有一定相似。
### 在 Linux 终端里管理进程
![](http://jbcdn2.b0.upaiyun.com/2016/11/0604a7b6f41ea55cfedc7d27595bb023.jpg)
可以使用 [htop](http://hisham.hm/htop/)。与 top 相似，但更好用、更美观。有时候，我打开 htop 之后就让它一直运行。没有原因，就是喜欢！从某方面说，它就像将音乐可视化——当然，这里显示的是 RAM 和 CPU 的使用情况。
### 在 Linux 终端里管理文件
![](http://jbcdn2.b0.upaiyun.com/2016/11/c69a1b1d386516b99b664157b8acbb06.png)
在一个纯文本终端里并不意味着你不能享受生活之美好。比方说一个出色的文件浏览和管理器。这方面，[Midnight Commander](https://en.wikipedia.org/wiki/Midnight_Commander) 是很好用的。
### 在 Linux 终端里管理终端窗口
![](http://jbcdn2.b0.upaiyun.com/2016/11/d201701be88252b9b725fc05dcab0ea1.png)
如果要在终端里工作很长时间，就需要一个多窗口终端了。它是这样一个软件 —— 可以将用户终端会话分割成一个自定义网格，从而可以同时使用和查看多个终端应用。对于 shell，它相当于一个平铺式窗口管理器。我最喜欢用的是 [tmux](https://tmux.github.io/)。不过 [GNU Screen](https://en.wikipedia.org/wiki/GNU_Screen) 也很好用。学习怎么使用它们可能要花点时间，但一旦会用，就会很方便。
### 在 Linux 终端里进行讲稿演示
![](http://jbcdn2.b0.upaiyun.com/2016/11/244bcdcdab03ae8ff86dac768b96d753.jpg)
这类软件有 LibreOffice、Google slides、gasp 或者 PowerPoint。我在讲稿演示软件花费很多时间，很高兴有一个终端版的软件。它称做“[文本演示程序（tpp）](http://www.ngolde.de/tpp.html)”。很显然，没有图片，只是一个使用简单标记语言将放在一起的幻灯片展示出来的简单程序。它不可能让你在其中插入猫的图片，但可以让你在终端里进行完整的演示。
