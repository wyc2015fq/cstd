# 能从远程获得乐趣的 Linux 命令 - 文章 - 伯乐在线
原文出处： [Jason Baker](https://opensource.com/article/18/12/linux-toy-remote)   译文出处：[Linux中国/geekpi](https://linux.cn/article-10437-1.html)
> 
使用这些工具从远程了解天气、阅读资料等。
![](http://jbcdn2.b0.upaiyun.com/2019/01/f83bd996007b0dc95b43d441fb3dfa3d.png)
我们即将结束为期 24 天的 Linux 命令行玩具日历。希望你有一直在看，如果没有，请回到[开始](https://opensource.com/article/18/12/linux-toy-boxes)，从头看过来。你会发现 Linux 终端有很多游戏、消遣和奇怪之处。
虽然你之前可能已经看过我们日历中的一些玩具，但我们希望每个人都遇见一个新事物。
今天的玩具（实际是玩具集合）有点不同。到目前为止，我主要是想把重点放在那些独立的玩具上，并且完全可在开源许可下使用。但是我从读者那里得到了一些很好的建议，利用开源工具远程访问一些开源或者不开源的东西。今天，我将介绍其中的一些。
第一个是经典之作：使用 Telnet 观看星球大战的 ASCII 演绎版本。你的系统可能已经安装了 Telnet，因此你只需运行：


```
$ telnet towel.blinkenlights.nl
```
我第一次看到它是十年之前，因此我对于它还存在有点惊奇。如果你还没看过，请留出一点时间看一下。你不会后悔的。
![](http://jbcdn2.b0.upaiyun.com/2019/01/e27c6bee59388cb6347efe50c4f97c8e.png)
接下来，Opensource.com 的撰稿人 [Manuel Dewald](https://opensource.com/users/ntlx) 提出了一种从终端获取当地天气的方法。它很简单，你只需安装 `curl`（或者，`wget`）。


```
$ curl wttr.in
```
![](http://jbcdn2.b0.upaiyun.com/2019/01/8fab6acc489e41247c24e29af466818d.png)
最后，在假期中虽然你可以从[命令行 Web 浏览器](https://opensource.com/article/16/12/web-browsers-linux-command-line)浏览你喜欢的网站（包括 Opensource.com），但有一些我最喜欢的网站可以通过专用客户端更轻松地浏览。其中两个是 Reddit 和 Hacker News，有人推荐给我一些它们的客户端，你可能也想尝试，它们都使用开源许可。我尝试过 [haxor-news](https://github.com/donnemartin/haxor-news) （Hacker News） 和 [rtv](https://github.com/michael-lazar/rtv) （Reddit），它们都还不错。
