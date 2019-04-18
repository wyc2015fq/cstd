# 博客园、CSDN博客过滤器（附源码） - weixin_33985507的博客 - CSDN博客
2012年10月07日 09:02:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
**一、写文章的目的**
         我在工作之余总喜欢刷刷博客园和CSDN的文章，甚至有时漫无目的，影响到了工作。所以有时会想，每天这么多文章，有哪些是自己想要的，哪些是自己用不上的，与其一篇篇看，不如做个过滤器，将两个网站的文章整合在一起，并添加些过滤条件，且不方便？
**二、功能分析**
        1、整合博客园首页（[http://www.cnblogs.com/](http://www.cnblogs.com/)）和CSDN热门博客（[http://blog.csdn.net/hot.html](http://blog.csdn.net/hot.html)）
        2、添加过滤条件，如：标题、内容、推荐数范围、评论数范围、阅读数范围、源博客页数（如搜索博客园前几页的数据）等
        3、博客显示样式模仿博客园，每篇博客包括图像、推荐数（CSDN并无推荐数功能，故此查询条件对其无效）、标题、内容、评论数、阅读数等信息
        4、文章具体内容还是需要通过点击标题url链接到具体页面查看
        5、以下是截图说明
![](https://images.cnblogs.com/cnblogs_com/gossip/201210/201210070902105853.png)
**三、源码解析**
        1、本项目采用MVC3+VS2010开发
        2、本项目使用HtmlAgilityPack.dll开源类库读取Html数据(项目地址：[http://htmlagilitypack.codeplex.com/](http://htmlagilitypack.codeplex.com/)）
        3、IndexController.cs：用于读取博客园和Csdn数据
              a)item.SelectSingleNode("div[2]/h3/a").InnerText  通过HtmlAgilityPack+XPath读取html中数据
        4、WebInfoModels.cs： 包含查询条件、博客显示项目等信息相关Model
        5、本项目代码不多，其它代码在源码中均有注释
**四、总结及展望**
        1、本项目美工欠缺，所以界面不怎么美观，呵呵。欢迎有美工背景的同学提供帮助
        2、博客园和CSDN的文章整合在一起（博客园在前，CSDN在后），不怎么方便浏览，下一版本会考虑分栏显示
        3、未提供博客园首页的【48小时阅读排行榜】、【10天内推荐排行】等功能，预计在下一版本新增。
        4、欢迎各位同学提出意见和建议
[源码下载](http://files.cnblogs.com/gossip/CnBlogsCsdnArticles.zip)
注意：运行此项目需要安装MVC3，下载地址：[http://www.asp.net/mvc/mvc3](http://www.asp.net/mvc/mvc3)
