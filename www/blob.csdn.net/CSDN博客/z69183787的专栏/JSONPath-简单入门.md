# JSONPath-简单入门 - z69183787的专栏 - CSDN博客
2019年03月26日 17:42:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：29
原文来自：[http://goessner.net/articles/JsonPath/](http://goessner.net/articles/JsonPath/)
- JSONPath - 是xpath在json的应用。
xml最大的优点就有大量的工具可以分析，转换，和选择性的提取文档中的数据。XPath是这些最强大的工具之一。
如果可以使用xpath来解析json，以下的问题可以被解决：
1，数据不使用特殊的脚本，可以在客户端交互的发现并取并获取。
2，客户机请求的JSON数据可以减少到服务器上的相关部分，这样可以最大限度地减少服务器响应的带宽使用率。
如果我们愿意，这个可以解析json数据的工具会变得有意义。随之而来的问题是它如何工作，jsonpath的表达式看起来怎么样。
事实上，json是由c系统编程语言表示自然数据，有特定语言的特定语法来访问json数据。
xpath的表达式：
/store/book[1]/title
我们可以看作是：
x.store.book[0].title
或
x['store']['book'][0]['title']
在Javascript, Python 和 PHP 中一个变量x表示json数据。经过观察，特定的语言里有内置xpath来解析数据。
JSONPath工具的问题
-依赖某种特定的语言
- 需要依赖XPath 1.0
- 减少代码量和内存的消耗
- 在运行时
- JSONPath 表达式
JSONPath 是参照，xpath表达式来解析xml文档的方式，json数据结构通常是匿名的并且不一定需要有根元素。JSONPaht 用一个抽象的名字$来表示最外层对象。
JOSNPath 表达式可以使用.  符号如下：
`$.store.book[0].title`
或者使用[] 符号
`$['store']['book'][0]['title']`
从输入路径来看。内部或者输出的路径都会转化成-符号。
JSONPath 允许使用通配符 * 表示所以的子元素名和数组索引。还允许使用 '..' 从E4X参照过来的和数组切分语法`[start:end:step]是从ECMASCRIPT 4 参照过来的。`
表达式在下面的脚本语言中可以使用显示的名称或者索引：
`$.store.book[(@.length-1)].title`
使用'@'符号表示当前的对象，?(<判断表达式>) 使用逻辑表达式来过滤。
`$.store.book[?(@.price < 10)].title`
这里有个表格，说明JSONPath语法元素和对应XPath元素的对比。
|**XPath**|**JSONPath**|**Description**|
|----|----|----|
|/|$|表示根元素|
|.|@|当前元素|
|/|. or []|子元素|
|..|n/a|父元素|
|//|..|递归下降，JSONPath是从E4X借鉴的。|
|*|*|通配符，表示所有的元素|
|@|n/a|属性访问字符|
|[]|[]|子元素操作符|
|||[,]|连接操作符在XPath 结果合并其它结点集合。JSONP允许name或者数组索引。|
|n/a|[start:end:step]|数组分割操作从ES4借鉴。|
|[]|?()|应用过滤表示式|
|n/a|()|脚本表达式，使用在脚本引擎下面。|
|()|n/a|Xpath分组|
XPath还有很多的语法（本地路径，操作符，和函数）没有列在这里。只要知道xpath和jsonpath脚本之中的不同点就行了。
- []在xpath表达式总是从前面的路径来操作数组，索引是从1开始。
- 使用JOSNPath的[]操作符操作一个对象或者数组，索引是从0开始。
- JSONPath 例子
接下我们看jsonpath表示的例子。下面是一个简单的json数据结构代表一个书店（原始的xml文件是）
```java
{ "store": {
    "book": [ 
      { "category": "reference",
        "author": "Nigel Rees",
        "title": "Sayings of the Century",
        "price": 8.95
      },
      { "category": "fiction",
        "author": "Evelyn Waugh",
        "title": "Sword of Honour",
        "price": 12.99
      },
      { "category": "fiction",
        "author": "Herman Melville",
        "title": "Moby Dick",
        "isbn": "0-553-21311-3",
        "price": 8.99
      },
      { "category": "fiction",
        "author": "J. R. R. Tolkien",
        "title": "The Lord of the Rings",
        "isbn": "0-395-19395-8",
        "price": 22.99
      }
    ],
    "bicycle": {
      "color": "red",
      "price": 19.95
    }
  }
}
```
|**XPath**|**JSONPath**|**结果**|
|----|----|----|
|`/store/book/author`|`$.store.book[*].author`|书点所有书的作者|
|`//author`|`$..author`|所有的作者|
|`/store/*`|`$.store.*`|store的所有元素。所有的bookst和bicycle|
|`/store//price`|`$.store..price`|store里面所有东西的price|
|`//book[3]`|`$..book[2]`|第三个书|
|`//book[last()]`|`$..book[(@.length-1)]`|最后一本书|
|`//book[position()<3]`|`$..book[0,1]``$..book[:2]`|前面的两本书。|
|`//book[isbn]`|`$..book[?(@.isbn)]`|过滤出所有的包含isbn的书。|
|`//book[price<10]`|`$..book[?(@.price<10)]`|过滤出价格低于10的书。|
|`//*`|`$..*`|所有元素。|
