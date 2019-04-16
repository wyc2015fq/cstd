# koa-bodyparser返回413状态码的问题 - ggabcda的博客 - CSDN博客





2017年08月21日 15:33:10[ggabcda](https://me.csdn.net/ggabcda)阅读数：897








> 
413 Request Entity Too Large（请求实体太大）


数日前，我用 node.js 写的一个日志服务抛出了这个状态码……

自己写的服务抛出了一个自己都不认识的状态码，这是最气的！(╯‵□′)╯︵┻━┻

检查之下发现是 koa-bodyparser 这个中间件的配置不太对；

因为这个中间件实在是太好用，装上后几乎什么都不用管，以至于我连她需要什么配置都不清楚，出了错之后才赶紧回去翻文档，

虽然写的很简略，但官方文档中还是提到了

> 
formLimit: limit of the urlencoded body. If the body ends up being larger than this limit, a 413 error code is returned. Default is 56kb.

jsonLimit: limit of the json body. Default is 1mb.

textLimit: limit of the text body. Default is 1mb.


这三个配置项就是对请求体进行限制的，一般情况下默认的限制就够用，但如果涉及到使用 form-data 方式进行富文本上传，可能就会出错了，因为 form-data 的默认最大上传限制只有 56kb ；

只需要改成：


```java
```java
app.use(bodyParser({
      formLimit: '1mb'
    }));
```
```


至于这三个 limit 选项可以支持的单位，官方文档上没有写，但是查了一下原代码可以发现：
`koa-bodyparser` 的底层是 `co-body`， `co-body` 的底层又是
`raw-body`， `raw-body` 则使用 `bytes` 库的 `bytes.parse` 来格式化 limit 参数。

so，我们可以使用`bytes` 库支持的所有单位来设置 limit 配置项，具体来讲，文档上写的是：
- b for bytes 
- kb for kilobytes 
- mb for megabytes 
- gb for gigabytes 
- tb for terabytes 

如果不写单位，默认就是 'b' ，更细节的东西可以查看 `bytes` 库的文档



