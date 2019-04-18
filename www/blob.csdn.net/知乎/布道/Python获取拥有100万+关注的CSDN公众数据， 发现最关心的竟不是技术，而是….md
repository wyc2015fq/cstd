# Python获取拥有100万+关注的CSDN公众数据， 发现最关心的竟不是技术，而是… - 知乎
# 



从工业时代进入到信息服务时代，互联网行业逐渐兴起，传统行业逐渐没落。现如今，很多人混迹于互联网+的时代，人人都要学习编程已经逐渐被重视起来。

作为一个入行五年的老鸟，回首看看这五年的经历，恍若隔世。

从PC互联网时代到移动互联网时代的转变，也就是前几年的事情。IT界的快节奏已经成为常态。小步快跑 ，产品的快速的迭代，移动互联网有了飞速的发展。

现在，更加快捷方便的生活体验，都是由一群怀揣着“改变世界”的梦想的人，一群不辞辛劳的程序员实现的。

“程序员”在大众心里，有很多刻板印象的标签：死宅 、呆板、加班、智商很高情商却很感人，极客、一年四季的格子衫等，更有那句著名的“钱多话少死得早”。

这些刻板印象，对也不全对，它们只是程序员众多特性中的一个方面。

这样的一群人，他们对电脑的性能要求极高，对机械键盘非常热衷，对耳机追求到极致，除了我们常见的一些，他们还关心、喜欢的还有啥。

笔者将从CSDN程序人生 近几年的发文数据中，从数据的角度，来分析一下这群极客的人，脑子里面装的究竟都有些什么。

## **数据获取**

本次数据来源为CSDN程序人生公众号发文，第一步需要从获取公众号近几年的发文数据。公众号内容因发布在微信官方平台，所以只能通过微信，才能获取到对应的数据。

• **抓包**

**抓包是将网络传输发送与接收的数据包进行截获、重发、编辑等操作。**在这里，我们需要使用我们自己的个人电脑，安装相应的抓包工具进行抓包（Mac推荐使用Charles，Windows推荐使用Fiddler）。

**• HTTPS**

HTTPS是安全为目标的HTTP通道，HTTP下加入SSL层，HTTPS的安全基础是SSL，因此加密的详细内容就需要SSL。HTTPS的出现增加了抓包的难度，但并不是不可解的。

为了解决这个问题，抓包工具会作为一个中间代理人，手机端与抓包工具进行通信，抓包工具在与服务端进行通信。

手机端与抓包工具建立HTTPS请求使用的公钥是抓包工具下发给他的。所以，为了能正常通信，需要安装抓包工具生成的根证书并信任该证书。

从抓取到的数据包中，可以分析出公众号文章列表对应的请求接口，只需要分页发送所有数据请求，就可以拿到整个文章列表。

但点赞（好看）数与阅读数量就比较难获取。这两项数据只在微信客户端上有，经过分析（猜测），此请求是微信客户端触发并更新到页面中的，要获取到此数据，只能使用笨办法，让微信打开文章详情页面，发送请求，我们将请求返回的数据存储起来，在与文章标题进行关联，数据就可以使用了。

自动化才能解放双手，所以此处使用AnyProxy+ADB Shell 。

AnyProxy是一个基于Node.js的、可供插件配置的HTTP/HTTPS代理服务器。和上面提到的Charles 、 Fiddler类似，但更加适合开发者使用。

ADB是Android开发SDK中的一个工具，它可以使用屏幕点击、输入、滑动等功能，由此实现自动化点击屏幕。

启动AnyProxy，设置指定插件JS的路径，并运行ADB脚本，自动化打开页面，使得插件能够自动地将微信发送的请求中的Request与Response数据存储在数据库文件中，插件核心代码如下：


```
var url = require("url")
module.exports = {
   *beforeSendResponse(requestDetail, responseDetail)
  {
       try {
           var pathName = url.parse(requestDetail.url).pathname
           if(pathName == "/mp/getappmsgext") {
               saveReadCount(requestDetail, responseDetail)
          }
      } catch(err) {
           console.log("err")
      }
  }
};
```


## **数据清洗与分析**

经过一段时间的数据抓取，并且对数据进行了关联，本地一共获取到的数据有2630条，与动辄成千上万的电影评论数据相比，虽然量不是很多，但也足够我们从中分析出一些关键信息。

点赞是对网络上的某个内容(比如一条贴子、一篇文章或者一条微博等)表示赞同、喜爱。所以我们先来看一下程序人生公众号推文的点赞TOP10：

因为数据是存储在数据库中，所以只需要使用简单的SQL查询就可以拿到我们想要的数据，SQL代码如下：

select title as标题、author as作者、CAST(likeCount as int) as点赞数。from messages order by点赞数DESC limit 10。

执行上述SQL，拿到的结果如下：
![](https://pic4.zhimg.com/v2-18506112279ff93d404dc30cd9ce76b7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='509'></svg>)
从上图可以看到，“长春长生‘’最受关注，点赞数量远高于其它文章，可以看出我们程序员小哥哥们，也是时实关注社会重大事件，忧国忧民。

当然，点赞只是其中的一个指标，不能表示全部。毕竟有好多程序员连赞都懒得点。

除了点赞数量，我们来看看阅读数量的排行，此处使用的是控制台打印输出的数据，为了更好地格式化结构，使用了PrettyTable， 代码如下：


```
def getArticInfos(min, max):
   conn = sqlite3.connect('wechat.db')
   conn.text_factory = str
   cursor = conn.cursor()
   cursor.execute("select title, author, datetime, CAST(readCount as int) as read, CAST(likeCount as int) as like from messages where datetime > '2018-01-01 00:00:00' order by read desc")
   values = cursor.fetchall()
   table = PrettyTable(["Title", "Author", "Time", "Read Count", "Like Count"])
   table.align["Title"] = "l" 
   table.align["Author"] = "l" 
   table.padding_width = 1
   totalCount = 0
   for item in values:
       readCount = item[3]
       if  readCount >= min and readCount < max:
           table.add_row([str(item[0]), str(item[1]), str(item[2]), str(item[3]), str(item[4])])
           totalCount += 1
   print table
   print "Total Count:" + str(totalCount)
   conn.close()
```


此处列出了从7W+到10W+的文章列表：
![](https://pic1.zhimg.com/v2-d0affb656a84c13036bf32ebd234ef10_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='666'></svg>)



从图中我们可以看到，张小龙微信第一版需求启发地的龙泉寺备受关注，程序员的负面新闻也是众多程序员关注的焦点。行业新闻及与他们相关的事情，更能够吸引他们的注意。

最后，我们再整体地来看一下，这些文章中，标题里面都会出现哪些高频的词语。特此，使用Jieba分词来进行分词，并使用Matplotlib生成词云图，如下：
![](https://pic3.zhimg.com/v2-2f9b6f147b45b676518b7b5221b29e16_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='720'></svg>)



从图中，我们可以看到，“学习”可以说是相当高频的一个词语，当然，在各行各业，学习都是一个很重要的话题以及技能。

尤其是在 IT 行业，更新迭代更是异常迅捷，甚至快到前两天刚学的知识，过两天就过时了。

所以，众多的程序员都很关注自己的个人成长，学习自然必不可少。架构、框架、指南是一个吸引程序员比较好的词语。

最后最后，我想说的是，很想不通，作为一个程序员，你没有女朋友，你new一个对象不就完了吗？居然还要去相亲？

> 福利时间：关注知乎专栏，在微信公众号AiDevOps 回复获取“获取微信公众号爬虫代码”获取Python爬取微信公众号源码！


