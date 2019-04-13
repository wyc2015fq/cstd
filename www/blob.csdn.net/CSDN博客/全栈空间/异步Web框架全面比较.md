
# 异步Web框架全面比较 - 全栈空间 - CSDN博客

2019年04月11日 09:38:39[流浪的狗和一坨屎](https://me.csdn.net/github_38885296)阅读数：45个人分类：[Web技术与标准](https://jimmy.blog.csdn.net/article/category/7147036)[服务器&数据库](https://jimmy.blog.csdn.net/article/category/7150811)[行业&资讯](https://jimmy.blog.csdn.net/article/category/7175806)[](https://jimmy.blog.csdn.net/article/category/7150811)
[](https://jimmy.blog.csdn.net/article/category/7147036)



Node.js是开发web后端的**最佳选择**。它用于构建快速，可伸缩的服务器端Web应用程序。事件驱动的非阻塞I / O模型使Node.js轻量且高效。这些功能促使很多开发人员采用Node.js. 今天，让我们看一下开发人员的9个最佳Node.js框架列表。
目前最强的web后端框架仍然是Express，但是 我很担心以后express会变成所谓的“经典的框架‘或者”传统web框架“，因为一旦被贴上这些标签，express就过时了。
除了express，还有许多有竞争力的对手，他们之间相互独立，功能强大。
## 11个流行web框架
### [Hapi.js](http://hapijs.com/)
Hapi是一个强大而强大的框架，用于开发API。它是由Eran Hammer 2011在沃尔玛首次推出，同时试图处理黑色星期五的交通。完善的插件系统和各种关键功能，如输入验证，基于配置的功能，实现缓存，错误处理，日志记录等，使Hapi成为最受欢迎的框架之一。它用于构建有用的应用程序，并通过PayPal，迪士尼等几个大型网站提供技术解决方案。
### [Express.js](http://expressjs.com/)
Express.js由核心Node项目团队成员之一TJ Holowaychuk建立。一个大型社区支持这个框架，因此它具有不断更新和改进所有核心功能的优势。这是一个用于构建大量移动应用程序和API的极简主义框架。
Express[在过去两年中](http://npm-stat.com/charts.html?package=express&author=&from=&to=)拥有[超过5300万的下载量](http://npm-stat.com/charts.html?package=express&author=&from=&to=)，已成为构建Web应用程序的关键工具包之一，其稳定性对于许多Node.js用户来说至关重要，特别是那些刚刚开始使用该平台的用户。Express还支持一些支持Node.js的最重要的项目，包括[kraken.js](http://krakenjs.com/)，这是一个安全且可扩展的层，可扩展Express并被企业大量使用。Kraken.js[于2014年由PayPal](https://www.paypal-engineering.com/2014/03/03/open-sourcing-kraken-js/)开源。它还支持[Sails.js](http://sailsjs.org/)，一个可以轻松构建自定义企业级Node.js应用程序的Web框架，以及Node.js API框架[Loopback](http://loopback.io/)。

### [Nest.js](https://nestjs.com/)
NestJS 是一个精心制作的服务器端（后端）应用程序框架，以支持开发人员的生产力并让他们的生活更加快乐。
NestJS 是一个完全用 TypeScript 编写的框架（它也支持 JS ），它很容易测试，并且带来了所有必要的东西，你总是但您不知道该选什么。那么，它不仅是一个框架，它还是进入领域驱动设计，事件采购，微服务架构等先进工程概念领域的推动者。 所有软件包都以简单轻量级的形式打包，以便您可以选择 - 无论您决定使用整个框架还是仅重新使用其组件。
### [Socket.io](http://socket.io/)
它用于构建实时Web应用程序。它是一个Javascript库，允许Web客户端和服务器之间的双向数据流。异步数据I / O，二进制流，即时消息是该框架的一些最重要的功能。
### [Koa.js](http://koajs.com/)
这个未来主义的框架于2013年推出，用于使编写Web应用程序和REST apis的过程更加愉快。它基本上是一个使用生成器的Node.js的富有表现力的中间件。回调的有限使用使得生成器成为更有效的错误缓解方法。
### [Meteor.js](https://www.meteor.com/)
Node.js世界中最常用的框架之一。在庞大的开发人员社区，教程，自定义包和文档的支持下，该框架仅用于为Javascript创建一些出色的Web和移动应用程序。
### [fastify](https://www.fastify.io/)
Fastify是一个高度专注于以最少开销和强大的插件架构，为开发人员提供最佳体验的Web框架。
它受到了 Hapi 和 Express 的启发，是目前最快的 Node 框架之一。
Fastify 独特的将 JSON Schema 应用到请求时的 validation 和响应时的 serialization，作者写的`fast-json-stringify`包更是达到了**2x faster than JSON.stringify()**的神奇效果。

### [Mojito](https://github.com/yahoo/mojito)
Yahoo！cocktail是一个移动开发平台。Mojito是一个基于雅虎鸡尾酒的框架。由于Yahoo！鸡尾酒完全是用javascript编写的，因此mojito既可以在服务器端运行，也可以在客户端运行。Mojito是一个MVC（模型 - 视图 - 控制器）框架，具有许多功能。

### [Derby](http://derbyjs.com/)
这是一个MVC框架，用于创建实时移动和Web应用程序。Derby的Racer是Node.js的实时数据同步引擎，允许跨客户端和服务器进行多站点，实时并发和数据同步。赛车优化冲突解决方案，并允许通过利用ShareJS实时编辑应用程序。
### [Sails.js](http://sailsjs.org/)
这是另一个受Node.js开发人员欢迎的MVC框架。该框架通过开发聊天应用程序，仪表板和多人游戏获得了动力。它最着名的是构建数据驱动的API。它使用水线进行对象相关的映射和数据库解决方案。该框架使用**Express.js**来处理HTTP请求，并基于Node.js构建。它与Grunt模块的兼容性，包括LESS，SASS，Stylus，CoffeeScript，Jade，Dust，使Sails.js成为基于浏览器的应用程序的理想候选者。
### [Total.js](https://www.totaljs.com/?language=en)
Total.js是支持MVC架构的现代模块化Node.js。Angular.js，Polymer，Backbone.js，Bootstrap和更多客户端框架与此框架完全兼容。该框架是完全可扩展和异步的。事实上，它不需要任何工具如Grunt进行压缩，因此易于使用。它还嵌入了NoSql，并支持数组和其他原型。
## 如何选择合适的框架
互联网时代是快速变化和发展的时代。每天开发的应用程序都变得越来越先进。此列表中的每个框架都具有不同的功能，适用于不同类型的应用程序。Node.js正在影响高级应用程序的开发，这个列表为您提供了开发人员使用的最流行框架的完整列表。
每种框架都有优略，份额最高的框架也未必是最棒的，反正我一千从没听说过除了expressjs之外还有这么多可爱的web框架。我选择一款最合适的框架主要从以下几点考量他：
> GitHub star的数量，侧面衡量框架的市场份额；

> 官网UI的风格，风格精美的界面也能代表开发者对项目投入的激情；

> 第三方评论和投票：看看其他用户的意见；

> GitHub的更新周期和issue反馈的质量；

> 评估下官网的网页性能（官网很可能就是用自生框架写的）；
除此之外，不同框架的理论支持，也就是首页会给出的特性和动机，然后项目的开发者团队也可以作为评估的手段，除此之外，大多数框架都是很相似的。

