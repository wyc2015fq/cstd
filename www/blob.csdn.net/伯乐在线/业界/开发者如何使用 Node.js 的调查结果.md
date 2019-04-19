# 开发者如何使用 Node.js 的调查结果 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [古鲁伊](http://www.jobbole.com/members/guluyi) 翻译。未经许可，禁止转载！
英文出处：[Gergely Nemeth](https://blog.risingstack.com/node-js-developer-survey-results-2016/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
## 导读
为了解开发者如何使用 Node.js 以及偏爱的相关技术，2016年 RisingStack 进行了一项调查。本文对调查结果进行了总结。
结果表明，MongoDB、RabbitMQ、AWS、Jenkins、Docker 以及 Amazon Container Services 是开发、集成、装载 Node.js 应用的首选。
此项调查也让我们了解开发 Node.js 的方方面面以及异步控制流、调试、持续集成或寻找包的选择方案。结果显示，Node 开发者的主要痛点是调试。
这项调查从 2016 年 7 月 11 日至 8 月 15 日，共开放了 35 天。在此期间，1126 位 Node.js 开发者共同完成了这项调查。其中 55% 开发者拥有两年以上的 Node.js 经验，26% 使用 Node 一至两年。20% 在上市公司工作，7% 在《财富》500 强。
## Node.js 相关技术
### MongoDB 成为数据库首选
![](http://ww4.sinaimg.cn/mw690/63918611gw1far8d7jtolj21190u0tc5.jpg)
根据结果，MongoDB 显然是 Node.js 开发者的首选数据库。大约 2/3 的参与者表示他们在自己的 Node.js 应用中使用 MongoDB。值得注意的是，在阅历丰富的 Node 工程师当中，Redis 的人气剧增，PostgreSQL 和 ElasticSearch 也是如此。
![](http://ww2.sinaimg.cn/mw690/63918611gw1far8d802a1j21190pvjvf.jpg)
> 
“Redis 和 PostgreSQL 在富有经验的开发者中人气剧增”——@RisingStack #nodejs #survey
### Redis 在缓存解决方案上领先，但是许多开发者还没有做这方面的处理。
![](http://ww3.sinaimg.cn/mw690/63918611gw1far8d8gmtyj21190uan07.jpg)
一半参与者表示在用 Redis 进行缓存，但难以置信的是，有 45% 根本没有缓存方案。相互比对经验丰富的程序员的答案，我们可以发现，相比于 Node.js 经验少于 1 年的工程师，Redis 在 Node 长期使用者中的人气相当高。
![](http://ww2.sinaimg.cn/mw690/63918611gw1far8d8xjwlj21190pm0wb.jpg)
### 消息系统的流行度仍旧较低
根据调查，58% 的 Node.js 开发者没有使用任何的消息系统。这意味着开发者在自己的微服务系统中很少使用消息，而用了 REST API，或者他们的系统不够复杂。
![](http://ww3.sinaimg.cn/mw690/63918611gw1far8d9f0kxj21190u5n0f.jpg)
使用消息系统的开发者表示他们大多用 RabbitMQ（24% 的调查对象）。如果我们只研究使用消息系统的开发者的答案，那么 RabbitMQ 远胜于其它已有的解决方案。
![](http://ww4.sinaimg.cn/mw690/63918611gw1far8da3w2ej21190uajut.jpg)
### Node.js 应用大多在 AWS 上运行
根据调查，43% 的 Node.js 开发者用 AWS 来运行应用，但是自己运行数据中心也很普遍（34%），尤其是在企业间（几乎 50% 有自己的数据中心）。这也并不奇怪。
![](http://ww4.sinaimg.cn/mw690/63918611gw1far8dapx7jj21190uadjf.jpg)
> 
“43% 的 Node.js 开发者用 AWS 运行应用”——@RisingStack #nodejs #survey @awscloud
但有意思的是，Heroku 和 DigitalOcean 并驾齐驱成为 Node.js 第二大云平台。根据数据，DigitalOcean 在小公司（少于 50 人）间更流行，而 Heroku 作为企业解决方案也很强。
![](http://ww2.sinaimg.cn/mw690/63918611gw1far8dbcquhj21190p9gq3.jpg)
### Docker 在 Node 社区中独领风骚
目前，Docker container 是多数 Node.js 开发者的首选解决方案（47% 的调查对象表示在用——而 container 技术用户中则高达 73%）。Docker 似乎在所有规模的公司间都很流行，但资深程序员使用得更多（一年以上开发经验的）。
![](http://ww4.sinaimg.cn/mw690/63918611gw1far8dc034yj21190u6gp2.jpg)
> 
“Docker container 是多数 Node.js 开发者的首选方案。”——@RisingStack #nodejs #survey @docker
64% 的调查对象表示使用 container 技术——意味着 container 的流行度从 2016 年 1 月 [Node.js 调查](https://nodejs.org/en/blog/announcements/nodejs-foundation-survey/) 的 45% 显著提升了 20%。
![](http://ww2.sinaimg.cn/mw690/63918611gw1far8dcm34zj21190pktck.jpg)
### Amazon Container Service 是运行 container 的首选
![](http://ww1.sinaimg.cn/mw690/63918611gw1far8dd7hp6j21190u941n.jpg)
虽然 Amazon Container Service 是运行 Node.js container 的首选，但值得注意的是，根据我们的调查，Kubernetes 也占比 25%，并且似乎在企业 Node.js 开发者中尤其受欢迎。
![](http://ww4.sinaimg.cn/mw690/63918611gw1far8dddqddj21190qmdjs.jpg)
## Node.js 开发
### 配置文件比环境变量更加常用。
与证书相比，多数 Node 开发者（59% vs. 38%）更喜欢配置文件。只有 29 位调查对象（3%）表示两个都用。
![](http://ww3.sinaimg.cn/mw690/63918611gw1far8ddpp2dj21190pjtbk.jpg)
只用配置文件有潜在的安全问题，因为这表明证书存放在仓库中。如果你在 GitHub 的生产系统中有证书，那么很快就会有无赖程序员来找麻烦。
为了安全性，建议使用环境变量，但是总的来说仍然可以用配置文件。
### Promise 主导异步控制流
在 Node.js中，大部分核心库都涉及回调。结果表明 Node.js 用户目前都倾向使用 promise。
![](http://ww4.sinaimg.cn/mw690/63918611gw1far8de3pv5j21190um0we.jpg)
大概半年前，[Node.js 核心仓库的一条 pull-request](https://github.com/nodejs/node/issues/11) 要求异步函数返回原生 Promise。对此的回答是：“Promise API 目前对核心功能来说没有意义，因为基于 V8 的 promise 以及和其它 ES* 特性的关系还在发展初期。短期内 TC 内部在核心中探索 promise 的兴趣不大”。
也许是时候重新审视这个问题了，因为出现了需求。
### 开发者依赖于 console.log 调试
Console.log 比其它像是 Node Inspector、Built-in debugger 和 debug module 等调试方案更胜一筹。大概 1/4 的 Node 开发者在应用中使用 console.log 定位 error，即使也有更加精细的解决办法。
![](http://ww1.sinaimg.cn/mw690/63918611gw1far8defywuj21190qswi3.jpg)
> 
“大概 1/4 的 Node 开发者在应用中使用 console.log 定位 error。”——@RisingStack #nodejs #survey
进一步观察数据可以发现更有经验的开发者也倾向于 Node Inspector 和 Debug Module。
![](http://ww3.sinaimg.cn/mw690/63918611gw1far8deut0rj21190pln0z.jpg)
### APM 在 Node.js 社区中仍旧低迷
根据调查显示，只有 1/4 Node.js 开发者使用 APM——应用性能监测工具——来识别应用问题。但是，数据集的趋势表明 APM 使用率会随着公司规模和开发者经验而上升。
![](http://ww2.sinaimg.cn/mw690/63918611gw1far8df8l7pj21190znn2w.jpg)
### SaaS CI 在 Node.js 社区的市场占有率依然较低
根据调查对象的答案，使用 shell 脚本是将代码推到测试或生产环境最普遍的方式。但是 Jenkins 目前在持续交付和集成平台方面明显胜出，并且随着公司规模的扩大而更受欢迎。
![](http://ww4.sinaimg.cn/mw690/63918611gw1far8dfkl19j21190q5td2.jpg)
### Node.js 开发者极少更新依赖
Node.js 应用强烈推荐经常更新依赖，因为 [根据一项最近的调查](https://blog.risingstack.com/controlling-node-js-security-risk-npm-dependencies/)，约 15% 的 npm 包带有已知的漏洞，76% 的 Node 商店利用漏洞依赖。
![](http://ww2.sinaimg.cn/mw690/63918611gw1far8dfzbxtj21190qvadi.jpg)
每周至少要更新一次依赖，否则应用就会时刻暴露在严重的攻击下。根据调查，45% Node.js 开发者一个月以上更新一次依赖，27% 每月更新一次，只有 28% 回答至少每周更新一次依赖。
这些数字与公司规模和程序员经验都没有关系。
### Node.js 开发者用 Google 找包
根据调查，大多数开发者用 Google 找包，并 决定应该使用哪一个。虽然在调查对象中， npmjs.org/npms.io 搜索平台的流行度达到了 56%，但是数据显示在经验丰富（四年以上的 Node 开发）的程序员中流行度几乎高达 70%！此情况下偏好会随经验增加。
![](http://ww2.sinaimg.cn/mw690/63918611gw1far8dgbqsij21190n2wim.jpg)
### 刚接触 Node.js 的开发者不知道什么是语义版本管理
虽然 71% 的调查对象在发布/使用模块时会用到语义版本管理，但是在我们看来，这个数字应该更高。每个人都应该使用语义版本控制，因为 npm 就用到了 semver。不用语义版本管理更新包很容易破坏 Node.js 应用。
![](http://ww1.sinaimg.cn/mw690/63918611gw1far8dgqpkqj21190r1djj.jpg)
深挖数据集，我们可以发现大约有一半经验少于一年的 Node 开发者不知道 semver 是什么或者没有使用 semver，而资深开发者们 semver 的接受度更高。
### Node.js 团队快速引进新技术、工具
据调查，35% Node 开发者能够几天内就为公司引进新的技术/工具/产品，29% 需要几周的时间。
![](http://ww1.sinaimg.cn/mw690/63918611gw1far8dgx2s2j211910an3g.jpg)
> 
“35% Node 开发者能够几天内为公司引进新的技术/工具”——@RisingStack #nodejs #survey
深入研究数据，可以发现一种合理的模式——引进新技术/工具所需的时间会随着公司规模递增。
### 调试是开发 Node.js 的最大痛点
我们也询问了 Node 开发者，什么是他们开发的最大痛点。答案排名如下：
- 调试/性能分析/性能监测
- 回调和回调地狱
- 理解异步编程
- 依赖管理
- 缺少统一规定/最佳实践
- 结构化
- 糟糕的文档
- 寻找合适的包
> 
“开发 Node.js 最大的痛点是调试。”——@RisingStack #nodejs #survey
## 总结
开发 Node.js 仍是一项有意思、不断变化的体验。在此感谢抽时间回答问题的工程师们，希望本文的信息对整个 Node 社区是有价值的。
> 
**打赏支持我翻译更多好文章，谢谢！**
[打赏译者](#rewardbox)
#### 打赏支持我翻译更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/06/8b23458dab045a48228dfa537d5241ea.png)![](http://jbcdn2.b0.upaiyun.com/2016/06/d2beee507374a3f33067e97fc125e755.jpg)
