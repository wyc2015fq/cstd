# 屏蔽微软的SignalR - 左直拳的马桶_日用桶 - CSDN博客
2016年05月24日 20:16:48[左直拳](https://me.csdn.net/leftfist)阅读数：8292
去年采用ASP.NET MVC开发项目，在谷歌浏览器里调试页面的时候，发现项目在不停地请求数据，链接很奇怪：
`http://localhost:63004/654c2dd725bb4401b8fc0c0a7d1312d2/browserLinkSignalR/poll?transport=longPolling&connectionToken=AQAAANCMnd8BFdERjHoAwE%2FCl%2BsBAAAABU73n5jBU0qT4THvITRjcQAAAAACAAAAAAAQZgAAAAEAACAAAABOZWYVL4mAw4MexRxGrAMANR1UsxDwcIjHIrZk3zDc3wAAAAAOgAAAAAIAACAAAADXGKc0LhwbHbjt21xezNlCKvcYewc8i2m5VK6%2BTaWLpTAAAACxsYt%2B%2FX9v7QCyFdjXzz8dz2r%2FlxWEqug6fXibToUbTgvataCyshym0QlmNVLEBl5AAAAA5EPQ4izyyIHASYi5ZCTOmtgN9WbS00h5jTuU6C8EwtHcxQ%2BBlNYfQA3kAVJjemk78q%2FrYn%2BDZNuHge7mxF8Epg%3D%3D&messageId=d-83DA592F-A%2C0%7CB%2C14%7CC%2C0&requestUrl=http%3A%2F%2Flocalhost%2FHNHY.Web%2FZHZS%2FZhanShi&browserName=Chrome&userAgent=Mozilla%2F5.0+(Windows+NT+6.1%3B+WOW64)+AppleWebKit%2F537.36+(KHTML%2C+like+Gecko)+Chrome%2F50.0.2661.102+Safari%2F537.36&tid=8&_=1464090722216`
好长的一串东西，几秒就请求一次。因为我们采用了bootstrap的框架，当时我就以为是bootstrap的问题，时间紧，且懒，所以就没有去研究。
后来因为项目在IE下运行，常常会卡死，不得不优化一下，就想看看这个请求到底是咋回事。
一查才知道是微软搞的鬼。名叫 “SignalR”。
这东西干啥用的呢？
据说是我们用Visual Studio开发的时候，常常视图的源代码改了，而浏览器那边不知道，就要重新刷新一次页面，才能看到最新结果。如果我们同时打开了多种浏览器，比如谷歌啦，火狐啦，IE啦，那就好麻烦好麻烦（有多麻烦？），于是微软就贴心地在VS，自动为我们提供了这个东东，让我们能自动看到修改结果（有吗？不用刷新就能看到？）
> 
Signal 是微软支持的一个运行在 Dot NET 平台上的 html websocket 
  框架。它出现的主要目的是实现服务器主动推送（Push）消息到客户端页面，这样客户端就不必重新发送请求或使用轮询技术来获取消息。
可访问其官方网站：[https://github.com/SignalR/](https://github.com/SignalR/) 获取更多资讯。
我靠！微软的毛病向来就是不分使用对象，将开发者与普通用户混淆，将原本需要开发人员干预的东西掩盖起来，简单易用的外表下面是开发人员最需要理解的原理。这是造成.NET程序员入门快，但提升难，素质偏低的主要原因。
但是这个SignalR，可恶之处原不止于此。它在开发者毫不知情的情况下，强塞给他们，造成许多困扰，且毫无必要。
这个SignalR的请求是怎么来的呢？只有在打开VS情况下才会有，部署在服务器的IIS上，是没有这些奇怪的请求的。那么它的原理是怎么样的呢？
代码里是找不到痕迹的，我搜索了许多遍。
也不是IIS的问题，部署在服务器上是正常的，没有SignalR请求；在我开发机上，项目也在IIS上跑，但不开VS就没事。
其实要禁掉SignalR很容易，在VS上将 “启用浏览器链接”的打勾去掉就行了。
据说，这些自动发出去的请求，是通过module等模块实现的，在输出页面的时候，自动加上了一些JS脚本。然后这些脚本就不停地请求。
那么这些module是怎么来的？我猜测，.netframework是有一些东西可以调用控制的，VS能够调用。
我真不知道怎么形容。微软开发者的脑袋里一定是塞满了大便。“噢，快停下，你们这帮愚蠢的土拨鼠。”
