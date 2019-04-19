# 从 PHP 转到 Node.js 的那些事 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [未济](http://www.jobbole.com/members/q529586910) 翻译，[艾凌风](http://www.jobbole.com/members/hanxiaomax) 校稿。未经许可，禁止转载！
英文出处：[Tim Whidden](http://beautifulcode.1stdibs.com/2015/10/16/from-apache-php-to-nodejs-expres/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
如果你们开发团队正在使用PHP，并考虑迁移到Node.js，这篇文章很适合你。本文并不探讨从PHP移植到Node.js的细节，以及Node.js的基础知识。而是涵盖：决策制定、着手点的描述、编写 Node.js 服务器的深层次注意事项、以及部署策略。
## 为什么迁移？
[1stdibs](https://www.1stdibs.com/) 决定从 Apache/PHP 迁移到 Node.js+Express 有五个理由：
- **代码更少**
- **全栈式JS**
- **开发人员幸福度更高**
- **投入回报率**
- **未来的优化**
### 代码更少
1stdibs基于面向服务体系架构（SAO），前端调用后台的JAVA服务。这意味着需要同时维护前端模型，以及服务端PHP和客户端JS模板。试想一下，如果可以摆脱PHP，就能够统一前端展现与后台模型于一种语言：JavaScript（同时可以合并一些模板）。从维护的角度来看，这么做代码更简洁，并且没有重复逻辑。
[同构JavaScript万岁](https://www.google.com/webhp?q=isomorphic%20javascript)!
### 全栈式JS（及其优点）
整个开发栈使用一种语言很简便。对开发者来说，较少的环境切换使他们开心和高效。额外的好处是工具使用更简单。相比之前使用[Composer](https://getcomposer.org/)和[npm](https://www.npmjs.com/)两个包管理器，现在只需要一个。尽管Composer很出色，由于nbp负责工具和客户端管理，nbp总是必要的。一旦去掉所有的PHP代码，nbp将成为仅有的包管理器。
### 开发人员乐意
我们要保证开发人员的技能集得到扩展、职业生涯不断发展，这一点很重要。对于JavaScript工程师而言，Node.js极具吸引力。能够在服务端使用与客户端相同的工具、风格和模式，是非常顺手和高效的。此外，Node.js相当流行，在企业级开发上[也得到了长足发展](http://www.infoworld.com/article/2907190/javascript/javascript-will-lead-a-massive-shift-in-enterprise-development.html)。Node.js是JavaScript工程师的必备技能。
### 投入回报率
我们在招聘优秀的JS工程师和培训初级JS工程师方面花了大价钱。由于客户端栈很复杂，我们需要高级JavaScript工程师。我们不再雇用PHP工程师，仅仅雇用了JavaScript工程师。我们的观点是，为什么不培养他们在服务端的技能呢？
### 未来的优化
长远而言，我们打算把两个庞大的应用分割成一系列独立部署的小应用。这很容易通过Node.js、Express和nbp实现。理论上，PHP（比如使用[Slim](http://www.slimframework.com/)）可以做同样的事。但我们非但得不到上述好处，还会搞得一团糟：在Apache/PHP上进行操作会更加复杂，基础设施也会变得有些奇怪。
## 选择框架
那个最终被我们用Node.js替换掉的PHP应用，主要有如下职责：
- 登录和授权
- 路由选择和服务端模板引擎（服务HTML）
- 引导前端应用
- 代理服务（为了回避CORS）
- 服务静态资源（js，css，images）
这些就是我们需要替换掉的基本功能。
我们尝试过不少框架，[Express](http://expressjs.com/)令人叹为观止（试一下我们评估过的[spreadsheet](https://docs.google.com/spreadsheets/d/1Dbo1WZ3qr7jmr6PvsANDWuaypFgUui62KMMr5RDhDgU/edit?usp=sharing)）。任何未基于Express 的框架看起来都不靠谱。Express通俗易懂，并有良好的文档。另外，可以招聘到正经培训过Express的人。
我们添加了一些[kraken](http://krakenjs.com/)的核心模块（[express-enrouten](https://github.com/krakenjs/express-enrouten)用于路由选择、[lusca](https://github.com/krakenjs/lusca)负责安全）；此外，[i18n-node](https://github.com/mashpie/i18n-node)提供国际化支持，模板引擎使用[Swig](https://github.com/paularmstrong/swig/)（我们后来放弃了Swig。呵呵，开源软件还是有风险的）。
我们考虑过全盘使用kraken，但是从原来的服务端php模板引擎[Twig](http://twig.sensiolabs.org/)切换到Swig直截了当，还很快捷。此外，kraken里面的Dust和i18n也不讨人喜欢。
## 编写服务器
选好了框架，下一步该写服务器了。
使用Apache+PHP时，你不需要再写一个服务器。Apache本身就是服务器，PHP是应用。如果使用Node.js，服务器和应用是同一个。从Apache/PHP转到PHP，你需要处理一些之前自然而然使用的功能，这一点很重要。使用Apache，你（或者系统管理员）配置服务器，在PHP应用里完全不用关心Apache为你处理的那些事。Node.js却以一种不同的方式来工作。
### 提供静态文件服务
毋庸置疑，提供静态文件服务是Apache的核心功能。Node.js与此不同，你要在应用中配置静态文件服务。幸运的是这很简单，有良好的文档说明，并且[是在Express中实现的](http://expressjs.com/starter/static-files.html)。
### 日志
很多基本的Apache配置为你提供访问日志和错误日志。使用Node.js时，估计你也猜到了，同样需要在应用中配置。所幸很多优秀的开源软件包使之变得很简单。[Morgan](https://github.com/expressjs/morgan)是一个基本的请求日志记录器，它配置简单，允许你把日志写到输出流（标准输出设备或文件）。如果你需要把日志写到数据库，或者有别的（更高级的）日志需求，那就试一下[winston](https://github.com/winstonjs/winston)吧。
### 代理
我们有一个基本需求：能够代理传输客户端ajax请求到后台服务。相比于处理CORS头，代理所有来自相同域的请求要简单得多。但你要想通过代理使用[webpack-dev-server](http://webpack.github.io/docs/webpack-dev-server.html)（正如我们所做的），就必须在Node.js应用中处理这一问题。[http-proxy](https://github.com/nodejitsu/node-http-proxy)是一个简单可靠的解决方案。
### 剩下的工作
除了上面提到的，还有一些列别的工作需要完成。我们从一个MVC应用谈起，该应用基于 [CodeIgniter](http://www.codeigniter.com/)(CI)框架，为一系列单页应用提供服务。大部分工作就是移植：
- CI控制器移植到Express路由选择器和中间件（包括登录和认证）
- Twig模板引擎移植到Swig（这一步比较琐碎）
- Service层数据访问（以便正常启动客户端单页应用）
上面并未列出CodeIgniter模型这一关键组件。事实上根本不用重写PHP模型！**太给力了！**我们的客户端应用使用Backbone模型。当然这要扩展Backbone.Model.sync，从而使之全局地工作在服务器和客户端。
## 部署
如果你的app规模较大，不应该一次性全部上线。可以通过渐进式部署的方式逐步上线。我们因此花费了好几周。
渐进式部署的优点：
- 最小化bug范围
- 每次在发布一部分路由及功能的同时，其他工程师可以正常进行开发
- 对正在进行的功能开发和改进影响最小 — 新功能可以继续发布（这可能导致重复的工作）
- 如果操作得当，可以快速回滚到之前的服务
### NGINX很不错
该如何逐步上线呢？我们在众多的服务器中挑选了[Nginx](https://en.wikipedia.org/wiki/Nginx)。


```
+----------+
 http        |          |---> Apache/PHP
 request---->|  Nginx   |
             |          |---> Node.js
             +----------+
```
Nginx允许你一次只“打开”一个路由（如果发现情况不妙就关掉 — 正如我们多次遇到的），这给了你很大的自由度。我们也发现打开路由的时候不用部署代码，这很有帮助。这在一周一次的发布计划里，为我们提供了一些回旋空间。
不过有一个缺点，你需要确保客户端代码同时接受旧的Apache/PHP服务器和新的Node.js服务器提供的服务。这并不可怕，不过你要把旧服务器上未优化的功能移植到新的服务器。屏住呼吸去做吧（记得写一个便利贴去清理你的技术债）。
## 总结
从头到尾，整个移植工作大概花费了一年。这听起来可能有点荒谬，不过这个时间表包括决策过程（比较匆忙）、基于Express写一个满足需求的核心框架、移植所有功能、逐步渐进式上线。此外，请记住，我们始终只有一两个开发者为之工作 — 并且是兼职。
如果你想尝试一下，请慎重考虑。你的团队能否受益？你的整个组织能否受益？如果你来自商业组织，请记住商业需要持续运转。你需要在商业目标和工程目标之间找到良好的平衡。
我们很高兴尝试过。
