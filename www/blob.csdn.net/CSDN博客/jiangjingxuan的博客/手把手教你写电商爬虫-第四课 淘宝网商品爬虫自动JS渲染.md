# 手把手教你写电商爬虫-第四课 淘宝网商品爬虫自动JS渲染 - jiangjingxuan的博客 - CSDN博客





2017年02月27日 09:23:53[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：347
个人分类：[手把手教你写爬虫](https://blog.csdn.net/jiangjingxuan/article/category/6749897)









老规矩，爬之前首先感谢淘宝公布出这么多有价值的数据，才让我们这些爬虫们有东西可以搜集啊，不过淘宝就不用我来安利了

广大剁手党相信睡觉的时候都能把网址打出来吧。

工欲善其事，必先利其器，先上工具：

**1、神箭手云爬虫，**

**2、Chrome浏览器 **

**3、Chrome的插件XpathHelper 不知道是干嘛的同学请移步第一课**



好了，咱们还是先打开淘宝网：

![](https://img-blog.csdn.net/20160512172656852?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

清新的画面，琳琅满目的商品，隐约感到的是前方是一场恶战：

淘宝这么多的商品，这么多的主题，到底从哪里开始呢？要不就女装了，别问我为什么，咱们先打开一个女装的列表页面：

```
https://s.taobao.com/list?q=%E5%A5%97%E8%A3%85%E5%A5%B3%E5%A4%8F
```

我们点击一下下一页看看连接：

```
https://s.taobao.com/list?q=%E5%A5%97%E8%A3%85%E5%A5%B3%E5%A4%8F&bcoffset=-4&s=60
```

看着好像不难，很简单，不过值得注意的是，我们把鼠标移到下一页可以看到连接本身并不是这个，这个是js处理的，这种情况类似前面我们遇到的尚妆网下一页，我们一会再说。

我们再看下详情页

```
https://item.taobao.com/item.htm?spm=a217f.7283053.1997524073.204.hEmtfc&id=527101625954&scm=1029.minilist-17.1.16&ppath=&sku=&ug=#detail

https://item.taobao.com/item.htm?spm=a217f.7283053.1997524073.209.hEmtfc&id=528697742170&scm=1029.minilist-17.1.16&ppath=&sku=&ug=#detail
```

看着比较简单，就直接提取成：

```
https://item\\.taobao\\.com/item\\.htm\\?.*
```

我比较懒，这个就先这样吧，然后我们来看看详情页我们需要抽取哪些信息，商品名称，价格自然不能少，这次再来一个新鲜的，就是缩略图列表，因为淘宝的商品不只有一个缩略图，因此需要爬取一个缩略图数组。

同样，用chrome开发者工具和xpathhelper对抽取项进行分析，看了一下结果，尝试过滤xhr，结果：

![](https://img-blog.csdn.net/20160512190743124?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



高手就是高手，在大量的请求中，找到响应的请求，相当的困难。不要紧，还记得我们上节课提到的核武器吗-JS渲染引擎，只要我们把JS渲染引擎打开，那么我们完全不用操心大量ajax请求的判断，当然他的问题是效率低一些，不过没关系，反正在云上跑嘛，睡一觉就好了。

在神箭手里调用JS渲染引擎的方式很简单，只需要一行代码设置既可：

```
configs.enableJS=true
```

好了，那我们现在可以无视ajax请求了，直接用chrome工具打开，直接提取：

```
//em[@id="J_PromoPriceNum"]
```
``简单暴啦，然后高兴没有1秒钟，瞬间蒙逼，淘宝上的价格不仅有这种促销价格，还有价格区间，还有的是普通的价格，我的天的，这不是要人命吗~
没办法，再难也要硬着头皮上，目前来看，一共有两种价格的抽取方式，当然不排除有其他多种的情况，我们先对这两种分别写一下抽取规则：

```
//em[@id="J_PromoPriceNum"]  
//em[contains(@class,"tb-rmb-num")]
```

我们可以通过 | 这个来连接，表达不同页面的选取可以共存

```
//em[@id="J_PromoPriceNum"] | //em[contains(@class,"tb-rmb-num")]
```

不容易。我们再看看商品名称，这个相对简单：

```
//h3[contains(@class,'tb-main-title')]/@data-title
```

最后，我们需要抽取图片的地址：

```
//ul[@id="J_UlThumb"]/li//img/@src
```



由于这一项存在多项，因此需要将该抽取规则的repeated字段设置成true

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

  var configs = {  
        domains: ["www.taobao.com","item.taobao.com"],  
        scanUrls: ["https://www.taobao.com/go/market/nvzhuang/citiao/taozhuangqun.php"],  
        contentUrlRegexes: ["https://item\\.taobao\\.com/item\\.htm\\?.*"],  
        helperUrlRegexes: ["https://www\\.taobao\\.com/go/market/nvzhuang/citiao/taozhuangqun.php"],//可留空  
        enableJS:true,  
        fields: [  
            {  
                // 第一个抽取项  
                name: "title",  
                selector: "//h3[contains(@class,'tb-main-title')]/@data-title",//默认使用XPath  
                required: true //是否不能为空  
            },  
            {  
                // 第二个抽取项  
                name: "price",  
                selector: "//em[@id='J_PromoPriceNum'] | //em[contains(@class,'tb-rmb-num')]",//默认使用XPath  
            },  
            {  
                // 第三个抽取项  
                name: "thumbs",  
                selector: "//ul[@id='J_UlThumb']/li//img/@src",//默认使用XPath  
            },  
        ]  
    };  
    var crawler = new Crawler(configs);  
    crawler.start();  

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```



好了，虽然淘宝比尚妆网难的多，但是在我们拿出核武器之后，一切迎刃而解，回头我们在来处理下一页的问题，这里虽然和尚妆网不一样，但是整体的原理大同小异，就不一一解释，我们直接上代码：

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

  var configs = {  
        domains: ["s.taobao.com","item.taobao.com"],  
        scanUrls: ["https://s.taobao.com/list?q=%E5%A5%97%E8%A3%85%E5%A5%B3%E5%A4%8F"],  
        contentUrlRegexes: ["https?://item\\.taobao\\.com/item\\.htm\\?.*"],  
        helperUrlRegexes: ["https?://s\\.taobao\\.com/list\\?q=%E5%A5%97%E8%A3%85%E5%A5%B3%E5%A4%8F.*"],//可留空  
        enableJS:true,  
        fields: [  
            {  
                // 第一个抽取项  
                name: "title",  
                selector: "//h3[contains(@class,'tb-main-title')]/@data-title",//默认使用XPath  
                required: true //是否不能为空  
            },  
            {  
                // 第二个抽取项  
                name: "price",  
                selector: "//em[@id='J_PromoPriceNum'] | //em[contains(@class,'tb-rmb-num')]",//默认使用XPath  
                required: true //是否不能为空  
            },  
            {  
                // 第三个抽取项  
                name: "thumbs",  
                selector: "//ul[@id='J_UlThumb']/li//img/@src",//默认使用XPath  
                repeated:true  
            },  
        ]  
    };  
    configs.onProcessHelperUrl = function(url, content, site){  
        if(!content.indexOf("未找到与")){  
            var currentStart = parseInt(url.substring(url.indexOf("&s=") + 3));  
            var start = currentStart + 60;  
            var nextUrl = url.replace("&s=" + currentStart, "&s=" + start);  
            site.addUrl(nextUrl);  
        }  
        return true;  
    };  
    var crawler = new Crawler(configs);  
    crawler.start(); 

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```



OK 大功告成，测试结果如下，由于开启的js渲染，所以爬取的速度比较慢，只能耐心等待了。

![](https://img-blog.csdn.net/20160513160950341?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





