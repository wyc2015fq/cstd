# Python分析《我不是药神》豆瓣评论 - zhusongziye的博客 - CSDN博客





2018年10月27日 21:56:08[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：88标签：[Python分析《我不是药神》豆瓣评论](https://so.csdn.net/so/search/s.do?q=Python分析《我不是药神》豆瓣评论&t=blog)
个人分类：[Python爬虫](https://blog.csdn.net/zhusongziye/article/category/7611456)









> 
来源：一别丶经年

my.oschina.net/zhanglikun/blog/1845888




**读取 Mongo 中的短评数据，进行中文分词**



![](https://img-blog.csdnimg.cn/20181027215435132.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_27,color_FFFFFF,t_70)



**对分词结果取 Top50 生成词云**



![](https://img-blog.csdnimg.cn/20181027215446357.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_27,color_FFFFFF,t_70)



生成词云效果



![](https://img-blog.csdnimg.cn/201810272155030.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_27,color_FFFFFF,t_70)



看来网上关于 我不是药神 vs 达拉斯 的争论很热啊。关于词频统计就这些，代码中也会完成一些其它的分析任务（其实都是一些很简单的任务，纯练手用），后续会持续更新。



## **其它分析任务**



**点赞数 Top10**



![](https://img-blog.csdnimg.cn/20181027215522206.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_27,color_FFFFFF,t_70)


- 
author = 忻钰坤, date = 2018-07-04 00:00:00, vote = 28129, comment = “你敢保证你一辈子不得病？”纯粹、直接、有力！常常感叹：电影只能是电影。但每看到这样的佳作，又感慨：电影不只是电影！由衷的希望这部电影大卖！成为话题！成为榜样！成为国产电影最该有的可能。

- 
author = 沐子荒, date = 2018-07-03 00:00:00, vote = 27237, comment = 王传君所有不被外人理解的坚持，都在这一刻得到了完美释放。他不是关谷神奇，他是王传君。 你看，即使依旧烂片如云，只要还有哪怕极少的人坚持，中国影视也终于还是从中生出了茁壮的根。 我不是药神，治不好这世界。但能改变一点，总归是会好的。

- 
author = 凌睿, date = 2018-06-30 00:00:00, vote = 18304, comment = 别说这是“中国版《达拉斯买家俱乐部》”了，这是中国的真实事件改编的中国电影，是属于我们自己的电影。不知道就去百度一下“陆勇”，他卖印度抗癌药的时候《达拉斯买家俱乐部》还没上映呢。所以别提《达拉斯买家俱乐部》了，只会显得你无知。（别私信我了，我800年前就知道《达拉斯》也是真事改编）

- 
author = 徐若风, date = 2018-06-06 00:00:00, vote = 16426, comment = 放豆瓣语境下，是部时至今日终于拍出来的国产“高分韩国电影”——拿现实题材拍商业类型片，社会性意义摆在那，群戏也处理得相当不错。对我们国家而言，这样的电影多一部是一部，走一步是一步。

- 
author = 桃桃淘电影, date = 2018-06-19 00:00:00, vote = 13337, comment = 最大的病，其实是穷病。真的被感动了，整体都很成熟，也有些许韩片的影子。几个演员表演都非常出色。可看性和内在的表达都不错。这个世界最荒诞在于，越贴近真实，真实越荒诞。人这一生，太不易了。最后，王传君，加油哦！

- 
author = 远世祖, date = 2018-06-30 00:00:00, vote = 9102, comment = 文牧野眼睛太毒了，观众的笑点、泪点、痛点被他牢牢抓住，徐峥现在不拼演技开始掏心炸肺放脱自我了，药物在中国绝对是个“不可说”，但这个电影说了它能说的，也不显山不漏水的说了它所不能说的，讲的是现实，但看过电影之后才会明白其实是超现实，2018最佳!

- 
author = 影志, date = 2018-06-19 00:00:00, vote = 7076, comment = “今后都会越来越好吧，希望这一天早点来”口罩成为符号，不是雾霾，而是人性的仪式，结尾竟然看到《辛德勒名单》一样的救赎。通俗感人，上海电影节首映哭倒一片，基于真实事件改编的社会意义加分，或许《我不是药神》之于中国，就像《摔跤吧爸爸》之于印度吧…能看到就不错。“其实只有一种病：穷病”

- 
author = Noodles, date = 2018-07-03 00:00:00, vote = 6926, comment = 人生建议：别买零食，吃不下的。

- 
author = 哪吒男, date = 2018-06-25 00:00:00, vote = 6211, comment = 最喜欢王传君的表演啊，几乎所有泪点都给他了！！而他曾经的同伴们，下月继续拿《爱情公寓》电影版面对观众。这个圈子里还是有不爱赚快钱的年轻演员，真好。

- 
author = 开开kergelen, date = 2018-07-04 00:00:00, vote = 5549, comment = 小时候路过一家药店，门口的对联写着“只愿世间无疾病，何愁架上药染尘”




**15 天评论量分布、走势**



![](https://img-blog.csdnimg.cn/20181027215533817.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_27,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181027215541779.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_27,color_FFFFFF,t_70)



**15天评论量分布图**



![](https://img-blog.csdnimg.cn/20181027215549312.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_27,color_FFFFFF,t_70)



**15天评论量走势图**



![](https://img-blog.csdnimg.cn/2018102721555946.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_27,color_FFFFFF,t_70)



