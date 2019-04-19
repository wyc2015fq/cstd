# css hack - 左直拳的马桶_日用桶 - CSDN博客
2011年05月19日 12:39:00[左直拳](https://me.csdn.net/leftfist)阅读数：1482标签：[css																[浏览器																[ie																[float																[互联网																[工具](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=互联网&t=blog)](https://so.csdn.net/so/search/s.do?q=float&t=blog)](https://so.csdn.net/so/search/s.do?q=ie&t=blog)](https://so.csdn.net/so/search/s.do?q=浏览器&t=blog)](https://so.csdn.net/so/search/s.do?q=css&t=blog)
个人分类：[Web](https://blog.csdn.net/leftfist/article/category/674421)
css hack! hack! hack! hack! hack! hack! hack!……
我以为是一种工具软件，没想到，针对不同浏览器，写相应的CSS代码，就叫 CSS Hack!
hack! hack! hack! hack! hack! hack!……
号称搞互联网这么久了，连这个都不知道，你真是孤陋寡闻得可以！什么玩意你这是！
IE6问题多多，好多样式它都不兼容。令人气愤的是，根据统计，互联网上49%是IE6，狗日的。
当然，IE系列都是烂。比如IE8，有些样式，IE6/7没问题，其他浏览器也没问题，但它就有。你不是越做越靠近主流的吗，做不好你就不要再做浏览器了，害人不浅。我操你姥姥。
.fuckIE span {float:right;width:120px;*margin-top:10px;*margin-top:0px !important;
}
这句CSS怎么理解？
1、据说IE浏览器可以识别“*”，非IE浏览器不行，所以后面2句针对IE浏览器。
2、!important 
IE7及以上，其他浏览器可以识别，因此，最后一句是给IE7以上浏览器用的。
3、因为这2句都是定义margin-top的，IE7及以上浏览器两句都可以识别，以最后一个定义为准，相当于将margin-top的定义抹去
结论：**这2句就是为IE6准备的。只有IE6，才有margin-top:10px的效果。**
我操你，微软，我操你姥姥。
---------------------------------------------------------------------------------------------------------------------------------------------------
笑屎了！
上面这句代码，最后一句，IE6也可以解释，所以，还是行不通。要改成：
.fuckIE span {float:right;width:120px;_margin-top:10px;
 }
---------------------------------------------------------------------------------------------------------------------------------------------------
margin-right:10px;margin-right:15px/9;*margin-right:10px;_margin-right:5px;
主流：margin-right:10px
IE8：margin-right:15px
IE7：margin-right:10px
IE6：margin-right:5px
|**浏览器/hack**|**_**|*****|**/9**|
|----|----|----|----|
|**FF**|**NO**|**NO**|**NO**|
|**IE8**|**NO**|**NO**|**YES**|
|**IE7**|**NO**|**YES**|**YES**|
|**IE6**|**YES**|**YES**|**YES**|
