# 成功抓取douban 所有电影 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年12月01日 10:56:29[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：183
个人分类：[scrapy](https://blog.csdn.net/elesos/article/category/7108617)
之前爬了250，想爬所有的电影
Rule(LinkExtractor(allow=(r'https://movie.douban.com/subject/\d+')),
callback="parse_item"),
修改为
start_urls = ["https://movie.douban.com/subject/26611090/"]
rules = [
Rule(LinkExtractor(allow=(r'https://movie.douban.com/subject/\d+'),
restrict_xpaths='//*[@id="recommendations"]/div'),
callback="parse_item",
follow=True),
]
提取
喜欢这部电影的人也喜欢  推荐的10部电影，这样应该不会出现太多电视剧。
问题：
[https://movie.douban.com/subject/2074813](https://movie.douban.com/subject/2074813) 这种页面没有评分
[https://movie.douban.com/subject/1292945](https://movie.douban.com/subject/1292945)
解决，插入库之前判断。如果数据不符合格式，可传默认值0分等。或丢弃
1，如果存在了，就更新下分数和投票数据
最后爬到 [https://movie.douban.com/subject/2127013](https://movie.douban.com/subject/2127013)共 41924 行
2017-11-15 11:30:56 ERROR:Item 404:https://movie.douban.com/subject/2970103/?from=subject-page
