# 零基础学习GitHub桌面版-8配置你的Hexo - 博客堂 - CSDN博客





2018年02月02日 20:26:13[最小森林](https://me.csdn.net/u012052268)阅读数：495
所属专栏：[零基础学习GitHub桌面版](https://blog.csdn.net/column/details/17109.html)











- [完善你的页面](#1-完善你的页面)- [1 标签页 Tags page](#11-标签页-tags-page)
- [2 分类页 Categories page](#12-分类页-categories-page)
- [3 社交媒体连接 Social Media Contact](#13-社交媒体连接-social-media-contact)- [31如何在contacts中增加新的选项](#131如何在contacts中增加新的选项)

- [4 Feed 链接 Feed link](#14-feed-链接-feed-link)

- [评论支持 Comment support](#2-评论支持-comment-support)
- [开通网站内搜索](#3开通网站内搜索)
- [开通站内统计](#4开通站内统计)
- [小技巧](#5小技巧)- [1 设置favicon](#51-设置favicon)
- [2 主页显示摘要](#52-主页显示摘要)
- [3 文章预览图片](#53-文章预览图片)
- [4支持数学公式](#54支持数学公式)





# 1. 完善你的页面

新站初见，还有一些网站页面是空的，比如tags还有分类页面、自我介绍页面，打开的时候显示404，下面挨个配置。

## 1.1 标签页 Tags page

添加一个标签页面，里面包含您网站中的所有标签。
- 使用hexo命令创建一个名为 tags 页面
`hexo new page "tags"`
此时生成了文件夹：INFO  Created: ~/Desktop/MyBlog/source/tags/index.md
- 编辑标签页 index.md, 设置页面类型为tags.

```
title: All tags
date: 2018-01-18 13:39:45
type: "tags"
layout: "tags"
```
- 添加 tags 到**主题**配置文件 _config.yml 里（注意是主题的配置文件）:

```
menu:
  home: 
  archives: archives
  tags: tags
```

然后执行：

```bash
hexo clean
hexo g -d
然后输入github账号密码
```

然后去访问自己的网站 [https://xqtbox.github.io](https://xqtbox.github.io) 吧，哈哈，漂亮

## 1.2 分类页 Categories page

添加一个分类页面，里面包含您网站中的所有分类
- 使用hexo命创建一个名为 categories 页面
`hexo new page "categories"`
这样就创建了：INFO  Created: ~/Desktop/MyBlog/source/categories/index.md
- 编辑分类页categories/index.md, 设置页面类型为 categories.

```
title: All categories
date: 2014-12-22 12:39:04
type: "categories"
layout: "categories"
```
- 添加 categories 到主题配置文件 _config.yml 里:

```
menu:
  home: 
  archives: archives
  categories: categories
```

然后执行：

```bash
hexo clean
hexo g -d
然后输入github账号密码
```

然后去访问自己的网站 [https://xqtbox.github.io](https://xqtbox.github.io) 吧，哈哈，漂亮

## 1.3 社交媒体连接 Social Media Contact

配置一下主题 可以自动添加链接到您的社交媒体帐户里:

```
social:
  GitHub: your-github-url
  Twitter: your-twitter-url
  Weibo: your-weibo-url
  DouBan: your-douban-url
  ZhiHu: your-zhihu-url
```

填写真实地址即可。

### 1.3.1如何在contacts中增加新的选项？

但是我们不一定有facebook和twitter，那么如何在contacts中增加中文的新的选项？如新浪微博、微信等，并且添加对应的图标？

答案是： 

访问 [http://fontawesome.io/icons/](http://fontawesome.io/icons/) ，找到自己需要的图标，然后将对应的icon名称直接填入主题的配置文件的cantacts中。 

例如：
```
contacts:
  github: http://github.com/xqtbox/
  twitter: 'https://twitter.com/hanangellove'
  weibo: "http://weibo.com/1619592223/"
  rss: atom.xml
```

这个网站提供了一些国内常见的contact比如 新浪微博、微信、百度等公司，但是这个网站不提供的就不行了。

## 1.4 Feed 链接 Feed link

显示 feed 链接。也就是RSS订阅：

在主题配置文件_config.yml里设置rss , 如下所示:
- rss: false 会禁用 feed 链接。
- 
rss: 使用站点 feed 链接。这是默认的选项。 

按照插件[hexo-generator-feed](https://github.com/hexojs/hexo-generator-feed)的README中的安装说明进行操作。在完成这个插件的配置后，Feed链接也生成好了

- 
rss: [http://your-feed-url](http://your-feed-url) 设置你的 feed 链接.


# 2. 评论支持 Comment support

博客是用来交流与分享的地方，如果不能评论，那就会少很多乐趣。下面我们配置Disqus 评论系统。

好消息是NexT 已经原生支持 Disqus 评论系统，我们只需要简单配置，过程思路如下：
- 去disqus官网申请个帐号，免费的。
- 注册的时候填写一个用户名（shortname）
- 复制这个用户名在博客配置文件里，就ok了

首先账号的注册过程见： [https://www.jianshu.com/p/c4f65ebe23ad](https://www.jianshu.com/p/c4f65ebe23ad)

注册好后，进入theme文件夹，再进入你的主题，打开你的**主题配置**文件 _config.yml 添加以下代码:
`disqus_shortname: your-disqus-shortname`
然后执行：

```
hexo clean
hexo generate
hexo deploy
```

然后去访问自己的网站 [https://xqtbox.github.io](https://xqtbox.github.io) 吧，哈哈，漂亮

> 
注意，这个评论系统部署在国外，不翻墙是看不到评论的…不过程序员肯定都翻墙，也没关系。


# 3.开通网站内搜索

![image](https://camo.githubusercontent.com/4e61568beed3ebd094281fc38bb3532bcbdc8b49/68747470733a2f2f6f6f6f2e306f302e6f6f6f2f323031362f30342f30372f353730363834363761643565612e706e67)

Insight Search是一个插件，用于搜索你的网站内的文字。下面是配置方法：
- install hexo-generator-json-content
`sudo npm install -S hexo-generator-json-content`- 更改配置文件：
`insight: true`
同时可以开启Baidu搜索，但是不推荐。 (Not Recommended)
`baidu: true`
# 4.开通站内统计

使用LeanCloud平台为Hexo博客添加文章浏览量统计组件：  
[http://crescentmoon.info/2014/12/11/popular-widget/](http://crescentmoon.info/2014/12/11/popular-widget/)

注册LeanCloud(Xu4…) 

1. 首先到『控制台』创建一个应用，名字随便取。 

2. 点击新建应用的『数据』选项，选择『创建Class』，取名为”Counter“。 

3. 点击新建应用右上角的齿轮，在『应用Key』选项里得到APP ID 和 APP Key，在后面会用到。
修改hexo页面生成文件 

1. 新建popular_posts.ejs

首先在theme/你的主题/layout/_widget目录下新建popular_posts.ejs文件,其内容为

```xml
<% if (site.posts.length){ %>
  <div class="widget-wrap">
    <h3 class="widget-title">浏览数目</h3>
    <div class="widget">
      <ul class="popularlist">
      </ul>
    </div>
  </div>
<% } %>
```
- 修改head.ejs

修改theme/你的主题/layout/_partial/head.ejs文件,在head标签的最后插入：

```xml
<script src="https://cdn1.lncld.net/static/js/av-min-1.2.1.js"></script>
<script>AV.initialize("你的APP ID", "你的APP Key");</script>
```
- 修改after-footer.ejs

修改theme/你的主题/layout/_partial/after-footer.ejs文件,在最后插入：

```xml
<!--page counter part-->
<script>
function addCount (Counter) {
        url=$('.article-date').attr('href').trim();
        title = $('.article-title').text().trim();
        var query=new AV.Query(Counter);
        //use url as unique idnetfication
        query.equalTo("url",url);
        query.find({
            success: function(results){
                if(results.length>0)
                {
                    var counter=results[0];
                    counter.fetchWhenSave(true); //get recent result
                    counter.increment("time");
                    counter.save();
                }
                else
                {
                    var newcounter=new Counter();
                    newcounter.set("title",title);
                    newcounter.set("url",url);
                    newcounter.set("time",1);
                    newcounter.save(null,{
                        success: function(newcounter){
                        //alert('New object created');
                        },
                        error: function(newcounter,error){
                        alert('Failed to create');
                        }
                        });
                }
            },
            error: function(error){
                //find null is not a error
                alert('Error:'+error.code+" "+error.message);
            }
        });
}
$(function(){
        var Counter=AV.Object.extend("Counter");
        //only increse visit counting when intering a page
        if ($('.article-title').length == 1)
           addCount(Counter);
        var query=new AV.Query(Counter);
        query.descending("time");
        // the sum of popular posts
        query.limit(10); 
        query.find({
            success: function(results){
                    for(var i=0;i<results.length;i++)    
                    {
                        var counter=results[i];
                        title=counter.get("title");
                        url=counter.get("url");
                        time=counter.get("time");
                        // add to the popularlist widget
                        showcontent=title+" ("+time+")";
                        //notice the "" in href
                        $('.popularlist').append('<li><a href="'+url+'">'+showcontent+'</a></li>');
                    }
                },
            error: function(error){
                alert("Error:"+error.code+" "+error.message);
            }
            }
        )
        });
</script>
```

这段代码的核心逻辑就是对Counter对象的增加和查询，每一篇文章都会有一个time字段来记录访问次数。这里查询的时候我用的是文章通过Hexo生成的URL作为主键的，所以post文件夹目录下的文件名一旦取好就不要轻易修改了，要不然访问次数会重新计算的:)。
- 修改config.yml

最后，修改theme/你的主题/config.yml文件,在widgets:选项找个位置下添加- popular_posts即可。

还可以参考：[https://notes.wanghao.work/2015-10-21-%E4%B8%BANexT%E4%B8%BB%E9%A2%98%E6%B7%BB%E5%8A%A0%E6%96%87%E7%AB%A0%E9%98%85%E8%AF%BB%E9%87%8F%E7%BB%9F%E8%AE%A1%E5%8A%9F%E8%83%BD.html#%E9%85%8D%E7%BD%AELeanCloud](https://notes.wanghao.work/2015-10-21-%E4%B8%BANexT%E4%B8%BB%E9%A2%98%E6%B7%BB%E5%8A%A0%E6%96%87%E7%AB%A0%E9%98%85%E8%AF%BB%E9%87%8F%E7%BB%9F%E8%AE%A1%E5%8A%9F%E8%83%BD.html#%E9%85%8D%E7%BD%AELeanCloud)

# 5.小技巧：

## 5.1 设置favicon：

favicon的全称Favorites Icon，即地址栏左侧的图标：

![image](http://i.imgur.com/ZNLKFAE.png)

有个在线工具可以上传自己的图片去生成指定规格的favicon.ico文件： [http://www.atool.org/ico.php](http://www.atool.org/ico.php) 。打开主题配置文件_config.yml可以看到favicon的配置信息：

```
# Put your favicon.ico into `hexo-site/source/` directory.
favicon: css/images/favicon.png
```

根据说明，我们将图标取名为favicon.ico然后放到当前工程的hexo\source目录下，重启博客即可生效。

## 5.2 主页显示摘要

要实现摘要，只需要在文章开头写好摘要后，另起一行键入

```xml
---
这是摘要
<!-- more -->
这是正文
```

也就是想显示多少就显示多少。

如果不想显示“Read More”而显示别的文字比如“阅读更多”，打开主题的配置文件，定位到如下位置：

_config.yml
`excerpt_link: Read More`
改为：
`excerpt_link: 阅读更多`
## 5.3 文章预览图片

如何设置preview 中的文章标题都是在图片下面。

在头文件的里面添加：
`banner: http://blog.zhangruipeng.me/hexo-theme-icarus/gallery/salt-lake.jpg`
这样文章又漂亮，在主页上还能显示小图片。

## 5.4支持数学公式

为了支持MathJax数学公式，只需要在主题的_config.yml文件中加上：plugins.mathjax = true

```
# Plugins
plugins:
    ...
    mathjax: true # options: true, false
```

同时还要设置/layout/plugin/scripts.ejs:：

```xml
<% if (theme.plugins.mathjax) { %>
    <!-- Edit here -->
    <script type="text/x-mathjax-config">
        MathJax.Hub.Config({ tex2jax: { inlineMath: [['$','$'], ['\\(','\\)']] } });
    </script>
    <%- js('https://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-MML-AM_CHTML') %>
<% } %>
```

发现第二步已经做好了，不用我们写，更新发布一下看效果。





