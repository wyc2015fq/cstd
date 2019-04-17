# Flask入门 - miner_zhu的博客 - CSDN博客





2018年07月18日 21:30:59[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：83








## Python轻量级框架Flask入门

flask内核内置了两个最重要的组件，所有其它的组件都是通过易扩展的插件系统集成进来的。这两个内置的组件分别是werkzeug和jinja2。

werkzeug是一个用于编写Python WSGI程序的工具包。

jinja2是一个功能极为强大的模板系统，它完美支持unicode中文，每个模板都运行在安全的沙箱环境中，使用jinja2编写的模板代码非常优美。

# Flask 框架

为了理解 Flask 框架是如何抽象出Web开发中的共同部分，我们先来看看Web应用程序的一般流程。对于Web应用来说，当客户端想要获取**动态资源**时，就会发起一个HTTP请求（比如用浏览器访问一个 URL），Web应用程序会在后台进行相应的业务处理，（从数据库或者进行一些计算操作等）取出用户需要的数据，生成相应的HTTP响应（当然，如果访问静态资源，则直接返回资源即可，不需要进行业务处理）。整个处理过程如下图所示：

![](http://xuelangzf-github.qiniudn.com/20161030_forum_design_flask_2.png)

实际应用中，**不同的请求可能会调用相同的处理逻辑**。这里有着相同业务处理逻辑的 HTTP 请求可以用一类 URL 来标识。比如论坛站点中，对于所有的获取Topic内容的请求而言，可以用 `topic/&lt;topic_id&gt;/` 这类URL来表示，这里的 topic_id 用以区分不同的topic。接着在后台定义一个 `get_topic(topic_id)` 的函数，用来获取topic相应的数据，此外还需要建立URL和函数之间的一一对应关系。这就是Web开发中所谓的**路由分发**，如下图所示：

![](http://xuelangzf-github.qiniudn.com/20161030_forum_design_flask_3.png)

Flask底层使用[werkzeug](https://github.com/pallets/werkzeug)来做路由分发，代码写起来十分简单，如下：


|1234|@app.route('/topic/<int:topic_id>/')def get_topic(topic_id):# Do some cal or read from database# Get the data we need.|
|----|----|

通过业务逻辑函数拿到数据后，接下来需要根据这些数据生成HTTP响应（对于Web应用来说，HTTP响应一般是一个HTML文件）。Web开发中的一般做法是提供一个HTML模板文件，然后将数据传入模板，经过渲染后得到最终需要的HTML响应文件。

一种比较常见的场景是，**请求虽然不同，但响应中数据的展示方式是相同的**。仍以论坛为例，对不同topic而言，其具体topic content虽然不同，但页面展示的方式是一样的，都有标题拦，内容栏等。也就是说，对于 topic 来说，我们只需提供一个HTML模板，然后传入不同topic数据，即得到不同的HTTP响应。这就是所谓的**模板渲染**，如下图所示：

![](http://xuelangzf-github.qiniudn.com/20161030_forum_design_flask_4.png)

Flask 使用 [Jinja2](https://github.com/pallets/jinja) 模板渲染引擎来做模板渲染，代码如下：


|12345|@app.route('/topic/<int:topic_id>/')def get_topic(topic_id):# Do some cal or read from database# Get the data we need.return render_template('path/to/template.html', data_needed)|
|----|----|

总结一下，Flask处理一个请求的流程就是，首先根据 URL 决定由那个函数来处理，然后在函数中进行操作，取得所需的数据。再将数据传给相应的模板文件中，由Jinja2 负责渲染得到 HTTP 响应内容，然后由Flask返回响应内容。

# Flask 入门

关于 Flask 框架的学习，不建议直接读[官网文档](http://flask.pocoo.org/docs/0.11/)，虽然这是一手的权威资料，但并不适合初学者入手。这里推荐几个学习资料，可以帮助新手很快的入门：

汇智网[flask框架](http://www.hubwiz.com/course/562427361bc20c980538e26f/)教程：一个非常适合入门的精简教程，主要分为七部分：
- 快速入门
	- 路由：URL 规则与视图函数
- 请求、应答与会话
- 上下文对象：Flask 核心机制
- 模版：分离数据与视图
- 访问数据库：SQLAlchemy简介
- 蓝图：Flask应用组件化


教程简练地总结了 Flask 最核心的内容，并且还提供了一个简单的在线练习环境，方便一边学习理论一边动手实践。

此外，麦子学院也有一个 Flask入门 视频教程，一共8小时的视频教程，涵盖flask web 开发的方方面面，包括环境的搭建，flask 语法介绍，项目结构的组织，flask 全球化，单元测试等内容。视频作者有 17 年软件开发经验，曾任微软深圳技术经理及多家海外机构担任技术顾问，够牛！视频讲的也确实不错。

如果上面两个不能满足你，那么还可以看《[Flask Web开发:基于Python的Web应用开发实战](http://union.click.jd.com/jdc?e=&p=AyIHZR5aEQISA1AYUyUCEgNdHlocASJDCkMFSjJLQhBaUAscSkIBR0ROVw1VC0dFFQIWD1AaUhYdS0IJRmsXamtUL38JUmdaUw1FMkELS0QcR1hDDh43Uh5cEwsUA1YrWxIBEAdlKzpnMkBpja3tzaejG4Gx1MCKhTdQH18WAxMEUB1rFQcUAVMZWRQDGwZWG2sSMkRDBUQeS1RWVworaw%3D%3D&t=W1dCFBBFC1pXUwkEAEAdQFkJBVsVBhoCVBJYCltXWwg%3D)》这本有着 8.6 评分的书，相信没看完就跃跃欲试想写点什么了。这么优秀的框架，Github 上当然也有 [awesome-flask](https://github.com/humiaozuzu/awesome-flask)了，想深入学习flask的话，这里不失为一个好的资源帖。

[本篇](http://selfboot.cn/2016/10/30/forum_design_flask/)大概谈了下 Flask 的路由分发和模版渲染，下篇我们会继续讲Flask使用中的一些问题。



文章来源：[http://python.jobbole.com/86707/](http://python.jobbole.com/86707/)



