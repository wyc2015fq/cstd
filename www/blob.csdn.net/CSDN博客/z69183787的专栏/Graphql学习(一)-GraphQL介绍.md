# Graphql学习(一)-GraphQL介绍 - z69183787的专栏 - CSDN博客
2019年02月03日 10:26:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：78
个人分类：[Graphql-X](https://blog.csdn.net/z69183787/article/category/7601603)
![](https://zh69183787.github.io/images/graphql.png)
这一篇简单介绍一下GraphQL的概念及可以帮助我们解决什么问题，并有什么优势
GraphQL：既是一种用于 API 的查询语言也是一个满足你数据查询的运行时。 GraphQL 对你的 API 中的数据提供了一套易于理解的完整描述，使得客户端能够准确地获得它需要的数据，而且没有任何冗余，也让 API 更容易地随着时间推移而演进，还能用于构建强大的开发者工具。
以上内容摘抄自官网。简单说一下我的理解：可以说是Web服务的一种新的架构和规范，并且在FaceBook、gitHub、Twitter中均已使用。对比现在业务使用的Restful来说，有下列优势：
- 单endpoint：一个请求获取所有数据，解决RestAPI多个请求获取资源的问题
- 清晰的数据模型，字段强类型
- 按需获取，减少网络请求，无冗余
- API迭代顺畅，无须版本化
- 协议而非存储，对服务端数据进行组装过滤
## Rest比较
- 数据获取：Rest缺乏扩展性，GraphQL获取时，payload可以扩展，按需获取
- API调用：Rest有多个endpoint，GraphQL在大多数情况下只有1个endpoint，只是body内容不同
- 复杂请求：Rest需要多次，GraphQL一次调用，减少网络开销
- 返回处理：Rest有多种httpCode及Status，GraphQL只有200响应，错误内容需要在结果集中特殊获取
- 版本号：Rest使用V1、V2，GraphQL可根据Schema自行扩展
可以看出，GraphQL对前端开发来说帮助巨大，减少了很多工作量。但为了配合使用，服务器端需要怎么协同，并有什么好处呢？简单看了几个例子后发现，后端如果使用Graphql重构，就相当于服务层做了一层使用GraphQL引擎对已获取的结果数据再做一次包装。
## 自己的理解
- 可以预见，服务端在改造时，数据获取层不会有太大的工作量，但由于只有一个endpoint，在业务逻辑层可能需要拼接原来多个接口返回的数据。比如个人主页的访问，原本需要a请求返回个人信息，b请求返回朋友关系。在GraphQL的架构中，需要将a、b请求的数据在服务端合并，并使用GaphQL引擎构建运行时，对外提供接口c。这样就意味着c接口需要承担a、b两个请求的查询量，性能方面是否会有瓶颈？
- 其次，对于数据模型的定义务必精确，哪些字段可以被查询，哪些需要有请求参数，还有分页等都需要考虑。
- 对于数据校验、用户权限及数据安全性来说解决方案也比较模糊。因为整体的请求返回需要GraphQL引擎来处理，所以原本一些过滤器拦截器的方案可能被替代，需要业务层来完整处理应对。
- GraphQL-Java的文档较少，而且官网上的一些demo也无法运行，前期调研成本较大
## 总结
通过对官网的浏览，对后端来说，并没有觉得有什么很明显的优势。但对前端或者整个项目来说，提升还是挺明显的，具体效果等实际应用了以后再看吧。后面几章会介绍一下GraphQL的语法，并使用GraphQL-java做一些服务器端的Demo及数据模型的创建。
# 参考
[Graphsql中文官网地址](http://graphql.cn/)
[Graphsql-Java文档](http://graphql-java.readthedocs.io/en/stable/getting_started.html)
[Graphsql-Java官方文档翻译](https://segmentfault.com/a/1190000012119468)
