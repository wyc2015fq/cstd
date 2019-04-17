# 二十四、ES(elasticsearch)搜索引擎安装和使用 - jiangjingxuan的博客 - CSDN博客





2017年01月25日 11:49:34[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：4386











![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_114_default_big.png)



大数据时代，搜索无处不在。搜索技术是全栈工程师必备技术之一，如今是开源时代，数不尽的资源供我们利用，如果要自己写一套搜索引擎无疑是浪费绳命。本节主要介绍搜索引擎开源项目elasticSearch的安装和使用

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 为什么需要搜索引擎



首先想一下：在一篇文章里找一个关键词怎么找？字符串匹配是最佳答案。

然后再想一下：找到100篇文章里包含某个关键词的文章列表怎么找？依然是关键词匹配

再继续想：找到100000000000（一千亿）篇文章里包含某个关键词的文章怎么找？如果用关键词匹配，以现在的电脑处理速度，从远古时代到人类灭绝这么长时间都处理不完，这时候搜索引擎要发挥作用了



## 搜索引擎技术有多么高深？



搜索引擎这种技术实际上从古代就有了。想象一个国家存储各类编撰资料的部门，有几个屋子的书，如果想找到某一本书的时候会怎么找呢？对了，有分类目录，先确定要找的书籍是哪个类别的，然后从目录里面找到想要找的书籍位于屋子的什么位置，然后再去拿。搜索引擎其实就是做了生成目录（也就是索引）的事情。那么如今的搜索引擎是怎么生成索引的呢？

要把互联网上的资料生成索引，拢共分三步：第一步，把资料编号；第二步，把每篇资料内容切成词；第三步，把词和资料编号的对应关系处理成“词=》编号列表”的形式

这时候你就可以迅速的找到一千亿篇文章里包含某个关键词的文章了，告诉我关键词是什么，我直接就从索引里找到了这个词对应的文章编号列表了，搞定！把需要数万年才能做完的工作用了不到一秒钟就搞定了，这就是搜索引擎的魅力！

当然，上面说的搜索引擎技术很简单，但百度数万工程师也不都是白吃饭的，如果想做好一个搜索引擎产品需要解决的问题就有很多了：收集网页时要考虑全、快、稳、新、优的问题，建索引时要考虑质量、效率、赋权、周期、时效性、资源消耗等问题，搜索的时候要考虑query分析、排序、筛选、展示、性能、广告、推荐、个性化、统计等问题，整体上要考虑地域性、容灾、国际化、当地法律、反作弊、垂直需求、移动互联网等诸多问题，所以百度大厦彻夜通明也是可以理解的。



## 开源搜索引擎



既然搜索引擎技术这么复杂，那么我们何必自寻烦恼了，开源社区为我们提供了很多资源，世界很美好。

说到开源搜索引擎一定要用的开源项目就是lucene，它不是搜索引擎产品，而是一个类库，而且是至今开源搜索引擎的最好的类库没有之一，因为只有它一个。lucene是用java语言开发，基本上涵盖了搜索引擎中索引和检索两个核心部分的全部功能，而且抽象的非常好，我后面会单独写数篇文章专门讲lucene的使用。最后强调一遍，它是一个类库，不是搜索引擎，你可以比较容易的基于lucene写一个搜索引擎。

然后要说的一个开源项目是solr，这是一个完整的搜索引擎产品，当然它底层一定是基于lucene的，毫无疑问，因为lucene是最好且唯一的搜索引擎类库。solr使用方法请看我的另一篇文章《[教你一步步搭建和运行完整的开源搜索引擎](http://www.shareditor.com/blogshow/6)》



最后要说的就是elasticSearch，这个开源项目也可以说是一个产品级别的开源项目，当然它底层一定是基于lucene的，毫无疑问，因为lucene是最好且唯一的搜索引擎类库，我承认我是唐僧。它是一种提供了RESTful api的服务，RESTful就是直接通过HTTP协议收发请求和响应，接口比较清晰简单，是一种架构规则。话不多说，下面我就说下安装方法和简单使用方法，这样更容易理解，之后我会单独讲解怎么让你的网站利用elasticSearch实现搜索功能



## elasticSearch安装



从github下载1.7版tag并编译（选择1.7版是因为当前我们的网站的symfony2版本还不支持2.x版本，但请放心的用，1.7版是经过无数人验证过的最稳定版本）
`wget https://codeload.github.com/elastic/elasticsearch/tar.gz/v1.7.5`
解压后进入目录执行

```java
mvn package -DskipTests
```

这会花费你很长一段时间，你可以去喝喝茶了



编译完成后会在target/releases中生成编译好的压缩包（类似于elasticsearch-1.7.5.zip这样的文件），把这个压缩包解压放到任意目录就安装好了



## 安装ik插件



ik是一个中文切词插件，elasticSearch自带的中文切词很不专业，ik对中文切词支持的比较好。

在[https://github.com/medcl/elasticsearch-analysis-ik](https://github.com/medcl/elasticsearch-analysis-ik)上找到我们elasticSearch对应的版本，1.7.5对应的ik版本是1.4.1，所以下载https://github.com/medcl/elasticsearch-analysis-ik/releases/tag/v1.4.1

解压出的目录是：

elasticsearch-analysis-ik-1.4.1

进入目录后执行

```java
mvn clean package
```

编译完后依然是在target/releases生成了类似于elasticsearch-analysis-ik-*.zip的压缩包，把里面的内容解压到elasticsearch安装目录的plugins/ik下

再把elasticsearch-analysis-ik-1.4.1/config/ik目录整体拷贝到elasticsearch安装目录的config下

修改elasticsearch安装目录下的config/elasticsearch.yml，添加：

```
index:
  analysis:
    analyzer:
      ik:
        alias: [ik_analyzer]
        type: org.elasticsearch.index.analysis.IkAnalyzerProvider
      ik_max_word:
        type: ik
        use_smart: false
      ik_smart:
        type: ik
        use_smart: true
```

这样ik就安装好了



## 启动并试用



直接进入elasticsearch安装目录，执行

```bash
./bin/elasticsearch -d
```

后台启动完成







elasticSearch是通过HTTP协议收发数据的，所以我们用curl命令来给它发命令，elasticSearch默认监听9200端口



写入一篇文章：

```
curl -XPUT 'http://localhost:9200/myappname/myblog/1?pretty' -d '
{
  "title": "我的标题",
  "content": "我的内容"
}'
```

会收到返回信息：

```
{
  "_index" : "myappname",
  "_type" : "myblog",
  "_id" : "1",
  "_version" : 1,
  "created" : true
}
```

这说明我们成功把一篇文章发给了elasticSearch，它底层会利用lucene自动帮我们建好搜索用的索引



再写一篇文章：

```
curl -XPUT 'http://localhost:9200/myappname/myblog/2?pretty' -d '
{
  "title": "这是第二篇标题",
  "content": "这是第二篇内容"
}'
```

会收到返回信息：

```
{
  "_index" : "myappname",
  "_type" : "myblog",
  "_id" : "2",
  "_version" : 1,
  "created" : true
}
```

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

这时我们找到elasticsearch安装目录的data目录下会生成这样的目录和文件：

```
ls data/nodes/0/indices/myappname/
0  1  2  3  4  _state
```

不同环境会稍有不同，但是都会生成myappname目录就对了



查看所有文章：

```
curl -XGET 'http://localhost:9200/myappname/myblog/_search?pretty=true' -d '
{
    "query" : {
        "match_all" : {}
    }
}'
```

这时会把我们刚才添加的两篇文章都列出来



搜索关键词“我的”：

```
curl -XGET 'http://localhost:9200/myappname/myblog/_search?pretty=true' -d '
{
    "query":{
        "query_string":{"query":"我的"}
    }
}'
```

会返回：

```
{
  "took" : 2,
  "timed_out" : false,
  "_shards" : {
    "total" : 5,
    "successful" : 5,
    "failed" : 0
  },
  "hits" : {
    "total" : 1,
    "max_score" : 0.191783,
    "hits" : [ {
      "_index" : "myappname",
      "_type" : "myblog",
      "_id" : "1",
      "_score" : 0.191783,
      "_source":
{
  "title": "我的标题",
  "content": "我的内容"
}
    } ]
  }
}
```

搜索关键词“第二篇”：

```
curl -XGET 'http://localhost:9200/myappname/myblog/_search?pretty=true' -d '
{
    "query":{
        "query_string":{"query":"第二篇"}
    }
}'
```

会返回：

```
{
  "took" : 2,
  "timed_out" : false,
  "_shards" : {
    "total" : 5,
    "successful" : 5,
    "failed" : 0
  },
  "hits" : {
    "total" : 1,
    "max_score" : 0.1879082,
    "hits" : [ {
      "_index" : "myappname",
      "_type" : "myblog",
      "_id" : "2",
      "_score" : 0.1879082,
      "_source":
{
  "title": "这是第二篇标题",
  "content": "这是第二篇内容"
}
    } ]
  }
}
```



如果想检查ik的切词效果，可以执行：

```
curl 'http://localhost:9200/myappname/_analyze?analyzer=ik_max_word&pretty=true' -d'
{
    "text":"中华人民共和国国歌"
}'
```

通过返回结果可以看出，ik_max_word切词把中华人民共和国国歌切成了“中华人民共和国”、“中华人民”、“中华”、“华人”、“人民共和国”、“人民”、“共和国”、“共和”、“国”、“国歌”

也就是说我们搜索这些词中的任意一个都能把这句话搜到，如果不安装ik插件的话，那只会切成：“中”、“华”、“人”、“民”、“共”、“和”、“国”、“国”、“歌”，不够智能，搜索也不好进行了



## 讲解一下

上面几条命令都是json形式，elasticSearch就是这么人性化，没治了。

这里面的myappname是你自己可以改成自己应用的名字，这在elasticSearch数据存储中是完全隔离的，而myblog是我们在同一个app中想要维护的不同的数据，就是你的不同数据，比如文章、用户、评论，他们最好都分开，这样搜索的时候也不会混

pretty参数就是让返回的json有换行和缩进，容易阅读，调试时可以加上，开发到程序里就可以去掉了

analyzer就是切词器，我们指定的ik_max_word在前面配置文件里遇到过，表示最大程度切词，各种切，360度切

返回结果里的hits就是“命中”，total是命中了几条，took是花了几毫秒，_score就是相关性程度，可以用来做排序的依据



## elasticSearch有什么用

上面都是json的接口，那么我们怎么用呢？其实你想怎么用就怎么用，煎着吃、炒着吃、炖着吃都行。比如我们的博客网站，当你创建一篇博客的时候可以发送“添加”的json命令，然后你开发一个搜索页面，当你输入关键词点搜索的时候，可以发送查询的命令，这样返回的结果就是你的搜索结果，只不过需要你自己润色一下，让展现更美观。感觉复杂吗？下一节告诉你怎么用symfony2的扩展来实现博客网站的搜索功能





