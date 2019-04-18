# post 相比get 有很多优点，为什么现在的HTTP通信中大多数请求还是使用get？ - z69183787的专栏 - CSDN博客
2016年06月12日 20:11:10[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1197
好吧， 除了哲学方式的回答以外，下面是一个浏览器从业人员的看法
事实上GET 和 POST 在实践上面有非常大的区别，这两种方法在语义上面并非能互相取代。
**POST 是否比 GET 安全**
是的， POST要比GET安全一点点，注意，是一点点。。。
说这两者都是明文传送当然是没有错的了，但是这里有一个细节，就是GET的URL会被放在浏览器历史和WEB 服务器日志里面。
POST 发完基本就木有了。。
所以如果你把关键数据放在GET里面，被人偷窥了浏览器，或者WEB服务器被入侵日志被人倒去了，基本泄露可能性100%。而POST来说，日志没有记录，只要数据库服务器不被入侵，基本还是安全的。
当然如果被抓了包，这一切都没有什么卵用，所以，HTTPS该用还是得用。
**GET 相对 POST 的优势是什么**
最大的优势是， GET 的URL可以人肉手输啊。。。你在地址栏打个POST给我看看。本质上面， GET 的所有信息都在URL， 所以很方便的记录下来重复使用。
所以如果你希望
－ 请求中的URL可以被手动输入
－ 请求中的URL可以被存在书签里，或者历史里，或者快速拨号里面，或者分享给别人。
－ 请求中的URL是可以被搜索引擎收录的。
－ 带云压缩的浏览器，比如Opera mini/Turbo 2, 只有GET才能在服务器端被预取的。
－ 请求中的URL可以被缓存。
请使用GET. 
大家有没有注意到，其实这里面很多方面的要求是和网站的运营相关的，而不是技术相关的。任何的技术行为中，其实多多少少都能看到商业的影子。
反之，就用POST. 特别是有一些东西你是不想让人家可以在浏览器地址栏里面可以输入的。比如，如果你设计一个blog系统, 设计这样一个URL来删掉所有帖子。
`http://myblog.com/?action=delete_all`
我只能说很快你就知道什么叫不作死就不会死这个道理了，搜索引擎的爬虫分分钟教你做人。
另外一个准则是，可以重复的交互，比如取个数据，跳个页面， 用GET.
不可以重复的操作， 比如创建一个条目/修改一条记录， 用POST, 因为POST不能被缓存，所以浏览器不会多次提交。
WEB API 的设计相对于网页来说更加复杂，同时也有GET/POST的问题，目前主流接受的方法是RESTful, 参见这里
[Representational
 state transfer](http://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Representational_state_transfer%23cite_note-4)
这个是趋势：
![](http://image95.360doc.com/DownloadImg/2016/02/2611/66685411_2.jpg)
同时对于何时用GET, W3C 也有一篇文章专门解释：
[http://www.w3.org/2001/tag/doc/whenToUseGet.html](http://link.zhihu.com/?target=http%3A//www.w3.org/2001/tag/doc/whenToUseGet.html)
摘一点出来：
> 
1.3 Quick Checklist for Choosing HTTP GET or POST
- Use GET if:
- The interaction is more like a question (i.e., it is a safe operation such as a query, read operation, or lookup).
and
> - Use POST if:
- The interaction is more like an order, or
- The interaction changes the state of the resource in a way that the user would perceive (e.g., a subscription to a service), or o The user be held accountable for the results of the interaction.
However, before the final decision to use HTTP GET or POST, please also consider considerations for sensitive data and practical considerations.
