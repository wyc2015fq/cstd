# 如何在Elasticsearch中安装中文分词器(IK+pinyin) - 零度的博客专栏 - CSDN博客
2018年05月21日 17:39:52[零度anngle](https://me.csdn.net/zmx729618)阅读数：110标签：[elasticsearch																[ik																[pinyin](https://so.csdn.net/so/search/s.do?q=pinyin&t=blog)](https://so.csdn.net/so/search/s.do?q=ik&t=blog)](https://so.csdn.net/so/search/s.do?q=elasticsearch&t=blog)
个人分类：[ElasticSearch](https://blog.csdn.net/zmx729618/article/category/7665883)
如果直接使用Elasticsearch的朋友在处理中文内容的搜索时，肯定会遇到很尴尬的问题——中文词语被分成了一个一个的汉字，当用Kibana作图的时候，按照term来分组，结果一个汉字被分成了一组。
这是因为使用了Elasticsearch中默认的标准分词器，这个分词器在处理中文的时候会把中文单词切分成一个一个的汉字，因此引入中文的分词器就能解决这个问题。
本篇文章按照下面的内容进行描述：
- 分词器的作用
- 安装IK
- 简单的测试
- 模拟测试
- 安装elasticsearch-analysis-pinyin
- 简单的测试
- 模拟测试
## 分词器的作用
分词顾名思义，就是把一句话分成一个一个的词。这个概念在搜索中很重要，比如 `This is a banana.` 如果按照普通的空格来分词，分成`this`,`is`,`a`,`banana`，的出来的`a`其实对我们并没有什么用处。因此需要注意下面的问题：
- 1 区分停顿词（`a`,`or`,`and`这种都属于停顿词）
- 2 大小写转换(`Banana`与`banana`)
- 3 时态的转换....
具体的算法可以参考[http://tartarus.org/~martin/PorterStemmer/](http://tartarus.org/~martin/PorterStemmer/)，对照的词语可以参考这里[http://snowball.tartarus.org/algorithms/porter/diffs.txt](http://snowball.tartarus.org/algorithms/porter/diffs.txt)
相比中文，就复杂的度了。因为中文不能单纯的依靠空格，标点这种进行分词。就比如`中华人民共和国国民`，不能简单的分成一个词，也不能粗暴的分成`中华人民共和国`和`国民`，`人民`、`中华`这些也都算一个词！
因此常见的分词算法就是拿一个标准的词典，关键词都在这个词典里面。然后按照几种规则去查找有没有关键词，比如:
- 正向最大匹配(从左到右)
- 逆向最大匹配(从右到左)
- 最少切分
- 双向匹配（从左扫描一次，从右扫描一次）
[IK，elasticsearch-analysis-ik](https://github.com/medcl/elasticsearch-analysis-ik)提供了两种方式,`ik_smart`就是最少切分，`ik_max_word`则为细粒度的切分（可能是双向，没看过源码）
了解了分词器的背景后，就可以看一下如何在Elasticsearch重安装分词器了。
## 安装IK
在[github中下载相应的代码](https://github.com/medcl/elasticsearch-analysis-ik)，比如我的最新版本2.4.0就没有对应的ik版本，不用担心，只需要修改pom.xml就可以了：
```xml
<properties>
        <!-- 这里的版本号，修改成你对应的版本就行了。
        不过最好不要跨度太大，相近的版本可能没有问题，但是跨度太大的版本，这样做就不保证好使了-->
        <elasticsearch.version>2.4.0</elasticsearch.version>
        <maven.compiler.target>1.7</maven.compiler.target>
        <elasticsearch.assembly.descriptor>${project.basedir}/src/main/assemblies/plugin.xml</elasticsearch.assembly.descriptor>
        <elasticsearch.plugin.name>analysis-ik</elasticsearch.plugin.name>
        <elasticsearch.plugin.classname>org.elasticsearch.plugin.analysis.ik.AnalysisIkPlugin</elasticsearch.plugin.classname>
        <elasticsearch.plugin.jvm>true</elasticsearch.plugin.jvm>
        <tests.rest.load_packaged>false</tests.rest.load_packaged>
        <skip.unit.tests>true</skip.unit.tests>
        <gpg.keyname>4E899B30</gpg.keyname>
        <gpg.useagent>true</gpg.useagent> 
    </properties>
```
下载后，执行`mvn package`，进行打包：
```
├─config
├─src
└─target
    ├─archive-tmp
    ├─classes
    ├─generated-sources
    ├─maven-archiver
    ├─maven-status
    ├─releases
    │  └─elasticsearch-analysis-ik-1.9.5.zip
    └─surefire
```
编译完成后，可以在target/releases目录下找到对应的zip包。
解压zip包，复制到elasticsearch-root-path/plugins/ik下即可。
```
[root@hadoop-master ik]# ll
total 1428
-rw-r--r-- 1 root root 263965 Sep 26 15:03 commons-codec-1.9.jar
-rw-r--r-- 1 root root  61829 Sep 26 15:03 commons-logging-1.2.jar
drwxr-xr-x 3 root root   4096 Sep 26 16:11 config
-rw-r--r-- 1 root root  56023 Sep 26 15:03 elasticsearch-analysis-ik-1.9.5.jar
-rw-r--r-- 1 root root 736658 Sep 26 15:03 httpclient-4.5.2.jar
-rw-r--r-- 1 root root 326724 Sep 26 15:03 httpcore-4.4.4.jar
-rw-r--r-- 1 root root   2666 Sep 26 15:03 plugin-descriptor.properties
[root@hadoop-master ik]# pwd
/usr/elk/elasticsearch-2.4.0/plugins/ik
```
拷贝后，重启elasticsearch就可以使用分词器了。
## 最简单的测试
这里使用_analyze api对中文段落进行分词，测试一下：
```
GET _analyze
{
  "analyzer":"ik_max_word",
  "text":"中华人民共和国国歌"
}
```
可以看到ik尽可能多的切分的单词:
```
{
  "tokens": [
    {
      "token": "中华人民共和国",
      "start_offset": 0,
      "end_offset": 7,
      "type": "CN_WORD",
      "position": 0
    },
    {
      "token": "中华人民",
      "start_offset": 0,
      "end_offset": 4,
      "type": "CN_WORD",
      "position": 1
    },
    {
      "token": "中华",
      "start_offset": 0,
      "end_offset": 2,
      "type": "CN_WORD",
      "position": 2
    },
    {
      "token": "华人",
      "start_offset": 1,
      "end_offset": 3,
      "type": "CN_WORD",
      "position": 3
    },
    {
      "token": "人民共和国",
      "start_offset": 2,
      "end_offset": 7,
      "type": "CN_WORD",
      "position": 4
    },
    {
      "token": "人民",
      "start_offset": 2,
      "end_offset": 4,
      "type": "CN_WORD",
      "position": 5
    },
    {
      "token": "共和国",
      "start_offset": 4,
      "end_offset": 7,
      "type": "CN_WORD",
      "position": 6
    },
    {
      "token": "共和",
      "start_offset": 4,
      "end_offset": 6,
      "type": "CN_WORD",
      "position": 7
    },
    {
      "token": "国",
      "start_offset": 6,
      "end_offset": 7,
      "type": "CN_CHAR",
      "position": 8
    },
    {
      "token": "国歌",
      "start_offset": 7,
      "end_offset": 9,
      "type": "CN_WORD",
      "position": 9
    }
  ]
}
```
如果使用ik_smart,则会尽可能少的返回词语:
```
{
  "tokens": [
    {
      "token": "中华人民共和国",
      "start_offset": 0,
      "end_offset": 7,
      "type": "CN_WORD",
      "position": 0
    },
    {
      "token": "国歌",
      "start_offset": 7,
      "end_offset": 9,
      "type": "CN_WORD",
      "position": 1
    }
  ]
}
```
## 模拟测试
我这里直接在elastic Sense中进行测试的（强烈推荐这个插件，非常好用，不过输入中文的时候，有点BUG）
#### 第一步，创建一个空的索引
```
PUT test 
{
  
}
```
如果你用的是curl，可以执行`curl -XPUT localhost:9200/test`
#### 第二步，设置映射类型
```bash
POST test/test/_mapping
{
    "test": {
        "_all": {
            "analyzer": "ik_max_word",
            "search_analyzer": "ik_max_word",
            "term_vector": "no",
            "store": "false"
        },
        "properties": {
            "content": {
                "type": "string",
                "store": "no",
                "term_vector": "with_positions_offsets",
                "analyzer": "ik_max_word",
                "search_analyzer": "ik_max_word",
                "include_in_all": "true",
                "boost": 8
            }
        }
    }
}
```
上面的命令，是定义test索引下test类型的映射。其中定义了_all字段的分析方法，以及content属性的分析方法。
这里介绍下[什么是_all字段](http://blog.csdn.net/jiao_fuyou/article/details/49800969),其实_all字段是为了在不知道搜索哪个字段时，使用的。es会把所有的字段（除非你手动设置成false），都放在_all中，然后通过分词器去解析。当你使用query_string的时候，默认就在这个_all字段上去做查询，而不需要挨个字段遍历，节省了时间。
`properties`中定义了特定字段的分析方式。在上面的例子中，仅仅设置了content的分析方法。
- type，字段的类型为string，只有string类型才涉及到分词，像是数字之类的是不需要分词的。
- store，定义字段的存储方式，no代表不单独存储，查询的时候会从_source中解析。当你频繁的针对某个字段查询时，可以考虑设置成true。
- term_vector，定义了词的存储方式，with_position_offsets，意思是存储词语的偏移位置，在结果高亮的时候有用。
- analyzer，定义了索引时的分词方法
- search_analyzer，定义了搜索时的分词方法
- include_in_all，定义了是否包含在_all字段中
- boost，是跟计算分值相关的。
#### 设置完成后，添加一个文档
```bash
POST test/test/1
{
  "test":"美国留给伊拉克的是个烂摊子吗"
}
POST test/test/2
{
  "content":"公安部：各地校车将享最高路权吗"
}
POST test/test/3
{
  "content":"中韩渔警冲突调查：韩警平均每天扣1艘中国渔船"
}
POST test/test/4
{
  "content":"中国驻洛杉矶领事馆遭亚裔男子枪击 嫌犯已自首"
}
```
#### 最后，执行查询进行测试
```
GET test/_search
{
  "query" : { "term" : { "content" : "中国" }},
  "highlight" : {
        "pre_tags" : ["<tag1>", "<tag2>"],
        "post_tags" : ["</tag1>", "</tag2>"],
        "fields" : {
            "content" : {}
        }
    }
}
```
得到返回结果：
```
{
  "took": 4,
  "timed_out": false,
  "_shards": {
    "total": 5,
    "successful": 5,
    "failed": 0
  },
  "hits": {
    "total": 2,
    "max_score": 1.5,
    "hits": [
      {
        "_index": "test",
        "_type": "test",
        "_id": "4",
        "_score": 1.5,
        "_source": {
          "content": "中国驻洛杉矶领事馆遭亚裔男子枪击 嫌犯已自首"
        },
        "highlight": {
          "content": [
            "<tag1>中国</tag1>驻洛杉矶领事馆遭亚裔男子枪击 嫌犯已自首"
          ]
        }
      },
      {
        "_index": "test",
        "_type": "test",
        "_id": "3",
        "_score": 0.53699243,
        "_source": {
          "content": "中韩渔警冲突调查：韩警平均每天扣1艘中国渔船"
        },
        "highlight": {
          "content": [
            "中韩渔警冲突调查：韩警平均每天扣1艘<tag1>中国</tag1>渔船"
          ]
        }
      }
    ]
  }
}
```
## 安装elasticsearch-analysis-pinyin分词器
pinyin分词器可以让用户输入拼音，就能查找到相关的关键词。比如在某个商城搜索中，输入`shuihu`，就能匹配到`水壶`。这样的体验还是非常好的。
pinyin分词器的安装与IK是一样的，这里就省略掉了。下载的地址[参考github](https://github.com/medcl/elasticsearch-analysis-pinyin).
这个分词器在1.8版本中，提供了两种分词规则：
- `pinyin`,就是普通的把汉字转换成拼音；
- `pinyin_first_letter`，提取汉字的拼音首字母
## 简单的测试
首先创建索引，并创建分词器:
```
PUT medcl
{
  "index" : {
        "analysis" : {
            "analyzer" : {
                "pinyin_analyzer" : {
                    "tokenizer" : "my_pinyin",
                    "filter" : "word_delimiter"
                    }
            },
            "tokenizer" : {
                "my_pinyin" : {
                    "type" : "pinyin",
                    "first_letter" : "none",
                    "padding_char" : " "
                }
            }
        }
    }
}
```
然后使用analyze api，进行测试
```
GET medcl/_analyze
{
  "text":"刘德华",
  "analyzer":"pinyin_analyzer"
}
```
可以得到结果：
```
{
  "tokens": [
    {
      "token": "liu",
      "start_offset": 0,
      "end_offset": 3,
      "type": "word",
      "position": 0
    },
    {
      "token": "de",
      "start_offset": 0,
      "end_offset": 3,
      "type": "word",
      "position": 1
    },
    {
      "token": "hua",
      "start_offset": 0,
      "end_offset": 3,
      "type": "word",
      "position": 2
    }
  ]
}
```
如果分词器设置为pinyin_first_letter，则分析的结果为：
```
{
  "tokens": [
    {
      "token": "ldh",
      "start_offset": 0,
      "end_offset": 3,
      "type": "word",
      "position": 0
    }
  ]
}
```
## 模拟测试
#### 如果索引已经存在，需要先关闭索引
```
POST medcl/_close
{
  
}
```
#### 然后设置分词器配置
```
PUT medcl/_settings 
{
  "index" : {
        "analysis" : {
            "analyzer" : {
                "pinyin_analyzer" : {
                    "tokenizer" : "my_pinyin",
                    "filter" : ["word_delimiter","nGram"]
                }
            },
            "tokenizer" : {
                "my_pinyin" : {
                    "type" : "pinyin",
                    "first_letter" : "prefix",
                    "padding_char" : " "
                }
            }
        }
    }
}
```
#### 打开索引
```
POST medcl/_open
{
  
}
```
#### 定义映射类型
```
POST medcl/folks/_mapping
{
  "folks": {
        "properties": {
            "name": {
                "type": "multi_field",
                "fields": {
                    "name": {
                        "type": "string",
                        "store": "no",
                        "term_vector": "with_positions_offsets",
                        "analyzer": "pinyin_analyzer",
                        "boost": 10
                    },
                    "primitive": {
                        "type": "string",
                        "store": "yes",
                        "analyzer": "keyword"
                    }
                }
            }
        }
    }
}
```
#### 提交样例数据
```
POST medcl/folks/1
{
  "name":"刘德华"
}
```
#### 执行查询
```
GET medcl/folks/_search
{
 "query": {"match": {
   "name": "l d hua"
 }}
}
```
这里搜`liu de hua`,`ldh`,`l de hua`都能匹配到，还是很强大滴。
#### 得到结果
```
{
  "took": 7,
  "timed_out": false,
  "_shards": {
    "total": 5,
    "successful": 5,
    "failed": 0
  },
  "hits": {
    "total": 1,
    "max_score": 7.408082,
    "hits": [
      {
        "_index": "medcl",
        "_type": "folks",
        "_id": "1",
        "_score": 7.408082,
        "_source": {
          "name": "刘德华"
        }
      }
    ]
  }
}
```
