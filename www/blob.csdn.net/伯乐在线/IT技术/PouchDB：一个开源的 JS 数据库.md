# PouchDB：一个开源的 JS 数据库 - 文章 - 伯乐在线
原文出处： [pouchdb](https://pouchdb.com/)   译文出处：[ITEYE/赖信涛](http://www.iteye.com/news/31968)
PouchDB 是一个受 [Apache CouchDB](http://couchdb.apache.org/) 启发的开源JavaScript数据库，可以完美运行在浏览器中。设计初衷是，帮助Web开发者构建能在本地离线运行的App，一如在线时运行的一样。
它能让应用程序离线时存储数据，并在回到线上时与服务器兼容，同步数据，保持用户数据的同步。
**什么是PouchDB？**
PouchDB是[CouchDB](http://couchdb.com/)的JavaScript实现，目标是实现可以和CouchDB相媲美的API，可以在浏览器和Node.js运行。
**什么是CouchDB？**
CouchDB是由Damien Katz于2015年写的NoSQL数据库，现在由Apache软件基金会负责维护。如果你是一个JavaScript开发者，你可能每天都在用CouchDB，因为它的核心技术驱动着[npm](https://www.npmjs.org/)。
**Couchbase、CouchDB等都是什么？**
现在，有两个数据库的血缘可以追溯到CouchDB：Couchbase，和Cloudant。他们都是从CouchDB分离出来的产品。
然而，这三种数据库都是使用了相同的CouchDBsync协议。这意味着PouchDB可与这三个数据库配合使用。
换句话说，这些数据库像是电话运营商，而CouchDB同步协议是底层的基础技术。
**CouchDB的优势：HTTP和sync**
现在已经有这么多SQL、NoSQL数据库了：MongoDB、PostgreSQL、MySQL等。那么为什么我们选择实现CouchDB，而不是别的呢？
CouchDB的优势有两点：HTTP和sync。
**HTTP：小而简单的协议**
当我们使用数据库时，经常需要在数据库和客户端的应用之间写类似转换层的东西，将数据库的查询语句转化成RESTful的HTTP调用。这些工作在编写的每一个App中被一次次开发重复着。
CouchDB将我们拯救了出来！通过我们的客户端App，可以直接和数据库通讯。它使用HTTP作为主要的通讯方式，没有特殊的协议，没有特殊的驱动：只有REST和HTTP。你可以通过你的浏览器，curl或像Postman这样的REST客户端直接和CouchDB连接。
换句话说，CouchDB是真正“为Web而生的数据库”。
**Sync：CouchDB的杀手级特性**
CouchDB另一个特性是，它从底向上都被设计成完全同步友好的数据库。
举个例子，如果你担心客户端应用的潜在危险，你可以直接在欧洲设一个CouchDB，再在北美搞一个，亚洲搞一个。在这些数据库之间开启双向复制之后，客户端就可以使用其中任何一个。
PouchDb使这一切都可以运行在你的浏览器里。
**讨论**
**PaulMest：**我已经使用PouchDB作为React Native应用的数据库6个月了，用SQLite数据库作为后端的存储，这样我们就可以使用设备更多的存储空间，而不只是50M。它工作得非常好，在一个离线缓存中维护数据，然后再同步到Digtal Ocean上的CouchDB 2.0。 让它在React native里工作非常有挑战性。让我们的系统跟上PouchDB的更新也非常有挑战性。实际上我们使用PouchDB 5.4.5，因为6.x版本中有breaking change，我还没有时间去搞明白哪里不兼容。PouchDB社区（尤其是Nolan lawson)非常活跃，给出示例、听取反馈回答问题等都非常积极。
**ojanik：**我们已经在产品环境中使用PouchDB大约15个月了。当初选择它是因为这是一个绿色软件，它给了我们两个优势：简单的离线支持、实时同步功能，配合后端的程序很方便。因为这个东西实际上就是一个带有本地缓存的Web App，部署新版本非常简单。
在架构方面，我们有大约有250个用户，每个都有独立的Couch数据库。我们还没有评估Couch 2.0，使用的还是Couch 1.6.
然而，我们在使用的过程中，还是遇到了一些问题：
- 载入时间问题。当你从0开始载入一个数据库时，非常耗时。初始化一个20-30M的数据库大约会花15-30分钟的时间，我们必须周期性地存储dump的文件，这样节省了一些时间。不过我觉得Couch2.0的同步协议已经解决了这个问题；
- 浏览器限制。当我们超过了浏览器的容量显示（通常是iOS的Safari，50M），我们就不得不采取一些具有创造性的做法。现在我们的方案是，为每一个客户运行两个数据库，一个是完整版本的数据库，另一个只存在7-8天。Pouch向后者同步数据。我们在完整数据库和临时数据库之间周期性地进行过滤复制。在客户端，如果一个客户需要7天之前的数据，就将Pouch切换到在线模式，作为一个库从在线数据库中取得数据；
- 处理冲突。因为CouchDB/PouchDB是分布式的，所以当用户基于相同的数据版本做了更新时，得当心冲突；
- 自定义后端逻辑。因为Pouch直接与Couch连接，所以不能在需要时完自定义后端逻辑。我们必须引入一个REST后台通道，以确保我们的后端在需要时运行额外的逻辑。
