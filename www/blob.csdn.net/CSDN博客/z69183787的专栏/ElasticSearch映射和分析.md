# ElasticSearch映射和分析 - z69183787的专栏 - CSDN博客
2017年09月29日 15:00:27[OkidoGreen](https://me.csdn.net/z69183787)阅读数：292
## 分析和分析器
分析(analysis)是这样一个过程：
- 首先，表征化一个文本块为适用于倒排索引单独的词(term)
- 然后标准化这些词为标准形式，提高它们的“可搜索性”或“查全率”
这个工作是分析器(analyzer)完成的。一个分析器(analyzer)只是一个包装用于将三个功能放到一个包里：
### 字符过滤器
首先字符串经过字符过滤器(character filter)，它们的工作是在表征化（译者注：这个词叫做断词更合适）前处理字符串。字符过滤器能够去除HTML标记，或者转换`"&"`为`"and"`。
### 分词器
下一步，分词器(tokenizer)被表征化（断词）为独立的词。一个简单的分词器(tokenizer)可以根据空格或逗号将单词分开（译者注：这个在中文中不适用）。
### 表征过滤
最后，每个词都通过所有表征过滤(token filters)，它可以修改词（例如将`"Quick"`转为小写），去掉词（例如停用词像`"a"`、`"and"``"the"`等等），或者增加词（例如同义词像`"jump"`和`"leap"`）
Elasticsearch提供很多开箱即用的字符过滤器，分词器和表征过滤器。这些可以组合来创建自定义的分析器以应对不同的需求。我们将在《自定义分析器》章节详细讨论。
### 内建的分析器
不过，Elasticsearch还附带了一些预装的分析器，你可以直接使用它们。下面我们列出了最重要的几个分析器，来演示这个字符串分词后的表现差异：
```
"Set the shape to semi-transparent by calling set_trans(5)"
```
### 标准分析器
标准分析器是Elasticsearch默认使用的分析器。对于文本分析，它对于任何语言都是最佳选择（译者注：就是没啥特殊需求，对于任何一个国家的语言，这个分析器就够用了）。它根据[Unicode Consortium](http://www.unicode.org/reports/tr29/)的定义的单词边界(word boundaries)来切分文本，然后去掉大部分标点符号。最后，把所有词转为小写。产生的结果为：
```
set, the, shape, to, semi, transparent, by, calling, set_trans, 5
```
### 简单分析器
简单分析器将非单个字母的文本切分，然后把每个词转为小写。产生的结果为：
```
set, the, shape, to, semi, transparent, by, calling, set, trans
```
### 空格分析器
空格分析器依据空格切分文本。它不转换小写。产生结果为：
```
Set, the, shape, to, semi-transparent, by, calling, set_trans(5)
```
### 语言分析器
特定语言分析器适用于很多语言。它们能够考虑到特定语言的特性。例如，`english`分析器自带一套英语停用词库——像`and`或`the`这些与语义无关的通用词。这些词被移除后，因为语法规则的存在，英语单词的主体含义依旧能被理解（译者注：`stem English words`这句不知道该如何翻译，查了字典，我理解的大概意思应该是将英语语句比作一株植物，去掉无用的枝叶，主干依旧存在，停用词好比枝叶，存在与否并不影响对这句话的理解。）。
`english`分析器将会产生以下结果：
```
set, shape, semi, transpar, call, set_tran, 5
```
注意`"transparent"`、`"calling"`和`"set_trans"`是如何转为词干的。
### 当分析器被使用
当我们索引(index)一个文档，全文字段会被分析为单独的词来创建倒排索引。不过，当我们在全文字段搜索(search)时，我们要让查询字符串经过同样的分析流程处理，以确保这些词在索引中存在。
全文查询我们将在稍后讨论，理解每个字段是如何定义的，这样才可以让它们做正确的事：
- 当你查询全文(full text)字段，查询将使用相同的分析器来分析查询字符串，以产生正确的词列表。
- 当你查询一个确切值(exact value)字段，查询将不分析查询字符串，但是你可以自己指定。
现在你可以明白为什么《映射和分析》的开头会产生那种结果：
- `date`字段包含一个确切值：单独的一个词`"2014-09-15"`。
- `_all`字段是一个全文字段，所以分析过程将日期转为三个词：`"2014"`、`"09"`和`"15"`。
当我们在`_all`字段查询`2014`，它一个匹配到12条推文，因为这些推文都包含词`2014`：
```
GET /_search?q=2014              # 12 results
```
当我们在`_all`字段中查询`2014-09-15`，首先分析查询字符串，产生匹配任一词`2014`、`09`或`15`的查询语句，它依旧匹配12个推文，因为它们都包含词`2014`。
```
GET /_search?q=2014-09-15        # 12 results !
```
当我们在`date`字段中查询`2014-09-15`，它查询一个确切的日期，然后只找到一条推文：
```
GET /_search?q=date:2014-09-15   # 1  result
```
当我们在`date`字段中查询`2014`，没有找到文档，因为没有文档包含那个确切的日期：
```
GET /_search?q=date:2014         # 0  results !
```
### 测试分析器
尤其当你是Elasticsearch新手时，对于如何分词以及存储到索引中理解起来比较困难。为了更好的理解如何进行，你可以使用`analyze` API来查看文本是如何被分析的。在查询字符串参数中指定要使用的分析器，被分析的文本做为请求体：
```java
GET /_analyze?analyzer=standard
Text to analyze
```
结果中每个节点在代表一个词：
```
{
   "tokens": [
      {
         "token":        "text",
         "start_offset": 0,
         "end_offset":   4,
         "type":         "<ALPHANUM>",
         "position":     1
      },
      {
         "token":        "to",
         "start_offset": 5,
         "end_offset":   7,
         "type":         "<ALPHANUM>",
         "position":     2
      },
      {
         "token":        "analyze",
         "start_offset": 8,
         "end_offset":   15,
         "type":         "<ALPHANUM>",
         "position":     3
      }
   ]
}
```
`token`是一个实际被存储在索引中的词。`position`指明词在原文本中是第几个出现的。`start_offset`和`end_offset`表示词在原文本中占据的位置。
`analyze` API 对于理解Elasticsearch索引的内在细节是个非常有用的工具，随着内容的推进，我们将继续讨论它。
### 指定分析器
当Elasticsearch在你的文档中探测到一个新的字符串字段，它将自动设置它为全文`string`字段并用`standard`分析器分析。
你不可能总是想要这样做。也许你想使用一个更适合这个数据的语言分析器。或者，你只想把字符串字段当作一个普通的字段——不做任何分析，只存储确切值，就像字符串类型的用户ID或者内部状态字段或者标签。
为了达到这种效果，我们必须通过映射(mapping)人工设置这些字段。
## 映射
我们知道索引中每个文档都有一个类型(type)。 每个类型拥有自己的映射(mapping)或者模式定义(schema definition)。一个映射定义了字段类型，每个字段的数据类型，以及字段被Elasticsearch处理的方式。映射还用于设置关联到类型上的元数据。
### 核心简单字段类型
Elasticsearch支持以下简单字段类型：
|类型|表示的数据类型|
|----|----|
|String|`string`|
|Whole number|`byte`, `short`, `integer`, `long`|
|Floating point|`float`, `double`|
|Boolean|`boolean`|
|Date|`date`|
当你索引一个包含新字段的文档——一个之前没有的字段——Elasticsearch将使用动态映射猜测字段类型，这类型来自于JSON的基本数据类型，使用以下规则：
|JSON type|Field type|
|----|----|
|Boolean: `true` or `false`|`"boolean"`|
|Whole number: `123`|`"long"`|
|Floating point: `123.45`|`"double"`|
|String, valid date: `"2014-09-15"`|`"date"`|
|String: `"foo bar"`|`"string"`|
> 
### 注意
这意味着，如果你索引一个带引号的数字——`"123"`，它将被映射为`"string"`类型，而不是`"long"`类型。然而，如果字段已经被映射为`"long"`类型，Elasticsearch将尝试转换字符串为long，并在转换失败时会抛出异常。
### 查看映射
我们可以使用`_mapping`后缀来查看Elasticsearch中的映射。在本章开始我们已经找到索引`gb`类型`tweet`中的映射：
```java
GET /gb/_mapping/tweet
```
这展示给了我们字段的映射（叫做属性(properties)），这些映射是Elasticsearch在创建索引时动态生成的：
```java
{
   "gb": {
      "mappings": {
         "tweet": {
            "properties": {
               "date": {
                  "type": "date",
                  "format": "dateOptionalTime"
               },
               "name": {
                  "type": "string"
               },
               "tweet": {
                  "type": "string"
               },
               "user_id": {
                  "type": "long"
               }
            }
         }
      }
   }
}
```
> 
### 小提示
错误的映射，例如把`age`字段映射为`string`类型而不是`integer`类型，会造成查询结果混乱。
要检查映射类型，而不是假设它是正确的！
### 自定义字段映射
映射中最重要的字段参数是`type`。除了`string`类型的字段，你可能很少需要映射其他的`type`：
```java
{
    "number_of_clicks": {
        "type": "integer"
    }
}
```
`string`类型的字段，默认的，考虑到包含全文本，它们的值在索引前要经过分析器分析，并且在全文搜索此字段前要把查询语句做分析处理。
对于`string`字段，两个最重要的映射参数是`index`和`analyer`。
### `index`
`index`参数控制字符串以何种方式被索引。它包含以下三个值当中的一个：
|值|解释|
|----|----|
|`analyzed`|首先分析这个字符串，然后索引。换言之，以全文形式索引此字段。|
|`not_analyzed`|索引这个字段，使之可以被搜索，但是索引内容和指定值一样。不分析此字段。|
|`no`|不索引这个字段。这个字段不能为搜索到。|
`string`类型字段默认值是`analyzed`。如果我们想映射字段为确切值，我们需要设置它为`not_analyzed`：
```java
{
    "tag": {
        "type":     "string",
        "index":    "not_analyzed"
    }
}
```
> 
其他简单类型——`long`、`double`、`date`等等——也接受`index`参数，但相应的值只能是`no`和`not_analyzed`，它们的值不能被分析。
### 分析
对于`analyzed`类型的字符串字段，使用`analyzer`参数来指定哪一种分析器将在搜索和索引的时候使用。默认的，Elasticsearch使用`standard`分析器，但是你可以通过指定一个内建的分析器来更改它，例如`whitespace`、`simple`或`english`。
```java
{
    "tweet": {
        "type":     "string",
        "analyzer": "english"
    }
}
```
在《自定义分析器》章节我们将告诉你如何定义和使用自定义的分析器。
### 更新映射
你可以在第一次创建索引的时候指定映射的类型。此外，你也可以晚些时候为新类型添加映射（或者为已有的类型更新映射）。
> 
### 重要
你可以向已有映射中增加字段，但你不能修改它。如果一个字段在映射中已经存在，这可能意味着那个字段的数据已经被索引。如果你改变了字段映射，那已经被索引的数据将错误并且不能被正确的搜索到。
我们可以更新一个映射来增加一个新字段，但是不能把已有字段的类型那个从`analyzed`改到`not_analyzed`。
为了演示两个指定的映射方法，让我们首先删除索引`gb`：
```
DELETE /gb
```
然后创建一个新索引，指定`tweet`字段的分析器为`english`：
```java
PUT /gb <1>
{
  "mappings": {
    "tweet" : {
      "properties" : {
        "tweet" : {
          "type" :    "string",
          "analyzer": "english"
        },
        "date" : {
          "type" :   "date"
        },
        "name" : {
          "type" :   "string"
        },
        "user_id" : {
          "type" :   "long"
        }
      }
    }
  }
}
```
`<1>` 这将创建包含`mappings`的索引，映射在请求体中指定。
再后来，我们决定在`tweet`的映射中增加一个新的`not_analyzed`类型的文本字段，叫做`tag`，使用`_mapping`后缀:
```java
PUT /gb/_mapping/tweet
{
  "properties" : {
    "tag" : {
      "type" :    "string",
      "index":    "not_analyzed"
    }
  }
}
```
注意到我们不再需要列出所有的已经存在的字段，因为我们没法修改他们。我们的新字段已经被合并至存在的那个映射中。
### 测试映射
你可以通过名字使用`analyze` API测试字符串字段的映射。对比这两个请求的输出：
```java
GET /gb/_analyze?field=tweet
Black-cats <1>
GET /gb/_analyze?field=tag
Black-cats <1>
```
`<1>` 我们想要分析的文本被放在请求体中。
`tweet`字段产生两个词，`"black"`和`"cat"`,`tag`字段产生单独的一个词`"Black-cats"`。换言之，我们的映射工作正常。
