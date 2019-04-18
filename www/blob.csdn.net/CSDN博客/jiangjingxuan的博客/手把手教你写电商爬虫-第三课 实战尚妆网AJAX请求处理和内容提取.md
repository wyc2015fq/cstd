# 手把手教你写电商爬虫-第三课 实战尚妆网AJAX请求处理和内容提取 - jiangjingxuan的博客 - CSDN博客





2017年02月27日 09:23:17[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：434








看完两篇，相信大家已经从开始的小菜鸟晋升为中级菜鸟了，好了，那我们就继续我们的爬虫课程。

上一课呢一定是因为对手太强，导致我们并没有完整的完成尚妆网的爬虫。



吭吭~，我们这一课继续，争取彻底搞定尚妆网，不留任何遗憾。

我们先回顾一下，上一课主要遗留了两个问题，两个问题都和ajax有关。

1、由于是ajax加载下一页，导致下一页url并不会被系统自动发现。

2、商品页面的价格是通过ajax加载的，我们直接从网页中获取不到信息本身。



好了，我们先解决第一个问题：

第一个问题实际上是一个爬虫中比较常见的问题，即url的发现，默认情况下，URL的发现是神箭手框架自动处理的，但是如果在ajax的情况下，框 架则无从发现url，这个时候就需要我们自己手动处理url的发现，这里，神箭手给我们提供了一个很方便的回调函数让我们来自己处理url的发现：

```
onProcessHelperUrl(url, content, site)
```

这个回调函数有两个参数，分别是当前处理的页面对象和整个爬取站的对象，我们可以通过获取页面对象的内容来分析是否有我们需要的新一页的url，通 过 site.addUrl()方法加入到url队列中既可。这里我们可以看到，当超出页数的时候，尚妆网会给我们返回一个这样的页面，我们就知道页数超过 了，不需要在加入新的页url：



![](https://img-blog.csdn.net/20160512162012512?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



这个页面我们很好判断，只需要看内容中是否有"无匹配商品"关键字既可。

这里我们需要一些基础的js能力，代码如下：

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

configs.onProcessHelperUrl = function(url, content, site){  
        if(!content.indexOf("无匹配商品")){  
            //如果没有到最后一页，则将页数加1  
            var currentPage = parseInt(url.substring(url.indexOf("&page=") + 6));  
            var page = currentPage + 1;  
            var nextUrl = url.replace("&page=" + currentPage, "&page=" + page);  
            site.addUrl(nextUrl);  
        }  
    }  

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```



原理很简单，如果内容中没有无匹配商品这个关键词的时候，则把当前页面的下一页加入的待爬队列中。
好了，ajax分页问题彻底解决，下面来看这个最棘手的ajax内容加载的问题，也就是如何获取到商品页面中的价格信息



首先，遇到这类问题，我们通常有两个思路：

**1、通过js引擎将整个页面渲染出来之后，在去做内容抽取，这个方案对于一些复杂js页面是唯一解决方案，用神箭手框架来处理也很简单，不过由于需要执行js，导致抓取速度很慢，不到不得已情况，我们先不使用这个核武器**

**2、通过刚刚处理分页的经验，我们可以预先分析ajax请求，然后将这一步多出来的请求和原来的页面请求做一个关联。这种方案适合比较简单的js页面中。**



OK，介绍完思路，根据经验，我们感觉尚妆网的ajax加载并没有十分复杂，所以我们选择方案二来处理这种ajax页面加载。

同样的，首页我们通过chrome开发者工具，抓取到这个ajax请求，这里教大家一个小窍门，开发者工具中，可以筛选请求对象未xhr，这种就是异步请求，我们就很容易发现我们的嫌疑url:

```
http://item.showjoy.com/product/getPrice?skuId=22912
```

我们在页面中找一下这个22912怎么提取最方便，我们很快就发现了一个标签：

```
<input type="hidden" value="22912" id="J_UItemId" />
```

这个标签很干净，获取的xpath也很简单：

```
//input[@id="J_UItemId"]/@value
```



这样就好办了，我们再看下这个页面请求的结果是什么：

```
{"count":0,"data":
{"discount":"6.2","discountMoney":"43.00","originalPrice":112,"price":"69.00","showjoyPrice":"69.00"},"isRedirect":0,"isSuccess":0,"login":0}
```



可以看出来，是一个典型的json对象，这个就好办了，神箭手框架中给我们提供了通过jsonpath提取内容的方式，可以很简单的提取到价格对象，即price对应的值。



那最后我们怎么才能关联这个请求呢？这里也是框架中提供的一个方案，叫做attachedUrl，专门用来解决关联请求的请求的问题，也就是某一个字段的值可以通过一个关联请求的内容中抽取出来。语法我就不介绍了，直接上代码吧：

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

 {  
      name: "skuid",  
      selector: "//input[@id='J_UItemId']/@value",  
    },  
    {  
      name: "price",  
      sourceType: SourceType.AttachedUrl,  
      attachedUrl: "http://item.showjoy.com/product/getPrice?skuId={skuid}",  
      selectorType: SelectorType.JsonPath,  
      selector: "$.data.price",  
    }  

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```



简单介绍一下attachedUrl的用法，首先我们要设置sourceType为attachedUrl，同时我们要设置一个attachedUrl，即 为关联请求的地址，其中由于有一个值是动态的，所以我们需要在这个抽取项之前先抽取一下这个动态的值，所以我们增加了一个抽取项的名字叫做skuid，在 attachedUrl中的调用方法为{skuid}，真实请求时，该项就会被自动替换成我们上一个skuid抽取项抽取到的值。接着，由于我们获取到的 是json返回，因此我们抽取的方式应该是通过jsonpath，最后，写一个抽取规则既可，jsonpath比xpath更加简单，相信大家一看就懂
 了。



好了，弄了这么多，完整的代码如下：

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

var configs = {  
        domains: ["www.showjoy.com","list.showjoy.com","item.showjoy.com"],  
        scanUrls: ["http://list.showjoy.com/search/?q=cateIds%3A1,cateName%3A%E9%9D%A2%E8%86%9C"],  
        contentUrlRegexes: ["http://item\\.showjoy\\.com/sku/\\d+\\.html"],  
        helperUrlRegexes: ["http://list\\.showjoy\\.com/search/\\?q=cateIds%3A1,cateName%3A%E9%9D%A2%E8%86%9C(\\&page=\\d+)?"],//可留空  
        fields: [  
            {  
                // 第一个抽取项  
                name: "title",  
                selector: "//h3[contains(@class,'choose-hd')]",//默认使用XPath  
                required: true //是否不能为空  
            },  
            {  
                // 第二个抽取项  
                name: "comment",  
                selector: "//div[contains(@class,'dtabs-hd')]/ul/li[2]",//使用正则的抽取规则  
                required: false //是否不能为空  
            },  
            {  
                // 第三个抽取项  
                name: "sales",  
                selector: "//div[contains(@class,'dtabs-hd')]/ul/li[3]",//使用正则的抽取规则  
                required: false //是否不能为空  
            },  
            {  
                name: "skuid",  
                selector: "//input[@id='J_UItemId']/@value",  
            },  
            {  
                name: "price",  
                sourceType: SourceType.AttachedUrl,  
                attachedUrl: "http://item.showjoy.com/product/getPrice?skuId={skuid}",  
                selectorType: SelectorType.JsonPath,  
                selector: "$.data.price",  
                  
            }  
        ]  
    };  
      
    configs.onProcessHelperUrl = function(url, content, site){  
        if(!content.indexOf("无匹配商品")){  
            //如果没有到最后一页，则将页数加1  
            var currentPage = parseInt(url.substring(url.indexOf("&page=") + 6));  
            var page = currentPage + 1;  
            var nextUrl = url.replace("&page=" + currentPage, "&page=" + page);  
            site.addUrl(nextUrl);  
        }  
        return true;  
    }  
    var crawler = new Crawler(configs);  
    crawler.start(); 

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```



终于搞定了，我们赶紧测试一下爬取的结果吧：

![](https://img-blog.csdn.net/20160512170135825?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



欣赏自己艰苦的劳动成果是不是很有成就感，不过现在的爬取结果依然有些美中不足，评论数和销售额拿到的都是一个完整的句子，而我们希望得到的是具体的数字，这个怎么操作呢？这个其实就是一个字段抽取到之后的进一步处理，框架中给我们提供了一个回调函数为：

```
afterExtractField(fieldName, data)
```



函数会将抽取名和抽取到的数据传进来，我们只需要通过js的字符串处理函数对数据进行进一步加工既可，直接上完整的修改过的代码：

```java

```

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

var configs = {  
    domains: ["www.showjoy.com","list.showjoy.com","item.showjoy.com"],  
    scanUrls: ["http://list.showjoy.com/search/?q=cateIds%3A1,cateName%3A%E9%9D%A2%E8%86%9C"],  
    contentUrlRegexes: ["http://item\\.showjoy\\.com/sku/\\d+\\.html"],  
    helperUrlRegexes: ["http://list\\.showjoy\\.com/search/\\?q=cateIds%3A1,cateName%3A%E9%9D%A2%E8%86%9C(\\&page=\\d+)?"],//可留空  
    fields: [  
        {  
            // 第一个抽取项  
            name: "title",  
            selector: "//h3[contains(@class,'choose-hd')]",//默认使用XPath  
            required: true //是否不能为空  
        },  
        {  
            // 第二个抽取项  
            name: "comment",  
            selector: "//div[contains(@class,'dtabs-hd')]/ul/li[2]",//使用正则的抽取规则  
            required: false //是否不能为空  
        },  
        {  
            // 第三个抽取项  
            name: "sales",  
            selector: "//div[contains(@class,'dtabs-hd')]/ul/li[3]",//使用正则的抽取规则  
            required: false //是否不能为空  
        },  
        {  
            name: "skuid",  
            selector: "//input[@id='J_UItemId']/@value",  
        },  
        {  
            name: "price",  
            sourceType: SourceType.AttachedUrl,  
            attachedUrl: "http://item.showjoy.com/product/getPrice?skuId={skuid}",  
            selectorType: SelectorType.JsonPath,  
            selector: "$.data.price",  
              
        }  
    ]  
};  
  
configs.onProcessHelperUrl = function(url, content, site){  
    if(!content.indexOf("无匹配商品")){  
        //如果没有到最后一页，则将页数加1  
        var currentPage = parseInt(url.substring(url.indexOf("&page=") + 6));  
        var page = currentPage + 1;  
        var nextUrl = url.replace("&page=" + currentPage, "&page=" + page);  
        site.addUrl(nextUrl);  
    }  
    return true;  
}  
configs.afterExtractField = function(fieldName, data){  
    if(fieldName == "comment" || fieldName == "sales"){  
        var regex = /.*（(\d+)）.*/;  
        return (data.match(regex))[1];  
    }  
    return data;  
}  
var crawler = new Crawler(configs);  
crawler.start(); 

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```


我们判断了如果是comment和sales抽取项时，通过正则直接匹配到括号里的数字，这里注意，网页上的括号本来是全角的括号，所以千万不要写错了。



这次终于可以开心的看着自己的爬虫数据结果了：

![](https://img-blog.csdn.net/20160512171749090?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





