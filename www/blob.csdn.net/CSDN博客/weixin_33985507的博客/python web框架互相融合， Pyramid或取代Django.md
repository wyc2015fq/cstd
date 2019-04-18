# python web框架互相融合， Pyramid或取代Django - weixin_33985507的博客 - CSDN博客
2012年08月29日 14:25:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
Django用久了，难免会感觉到自由度不够。不可否认，这种自由度的牺牲能够带来一些好处。比如，一体化的框架（full-stack framework)能够提供全面的solution，你不需要再为cache,session,feed,orm,auth,template等问题困扰，完全按照Django的设计哲学来使用就可以了。使用Django能够快速开发应用，而且技术的统一对于团队开发来说无疑是一件好事，团队成员无需在技术选型上进行过多的争论——Django已经帮我们定好了，just DO it。
但是在提供全面的solution的同时，Django将自己打造成了一个封闭的框架。由于所有的组件都靠自己实现，可能在每一个方面都做得不够好（尽管够用）。同时为了方便使用，Django的各个组件之间耦合比较紧密，你很难将某个组件替换成你自己喜欢的库。比如Django最为人诟病的ORM和Template，如果你想替换成SQLAlchemy和Mako，几乎是不可能的事情。即使勉强使用，也会让你非常别扭。
话说天下大事，分久必合。在python web framework 的战国时代，Django几乎做到了一家独大。但如果其他的框架合纵联合，未必不能与Django一争高下。
现在这种融合的趋势已经显现，而Django如果依然固步自封，即使再强大，也会在历史的尘埃中销声匿迹。如果你不相信，我们可以追索一下事实真相，不难看出这种融合恰恰是从更加强大的Zope的分裂开始的。
在[《戏说WSGI》](http://www.cnblogs.com/holbrook/archive/2012/02/25/2357337.html)一文中，曾经列举了几个python web框架。其中Zope是一个另类，因为Zope非常厚重，适合组织大规模的开发，构建复杂的应用。为了能够面对更高的复杂度，Zope提供了一个组件框架ZCA (Zope Component Architure)，引入了接口的概念，使得复杂系统可以基于接口开发。ZCA提供了适配器组件，用于装配整个应用的各个组成部分，还进一步提供了可重用的工具组件，事件组件等。可以说，ZCA是Zope中最有价值的产品。
尽管ZCA能够将Zope提供的认证、对象发布、事务管理、授权等功能组装起来，但是这些功能在Zope之外还是很难使用。于是有了Repoze项目。Repoze的目标是将Zope的功能组件拆解出来，使得其他的python web应用能够使用。说到这里，不禁又要感慨一下WSGI的重要意义：WSGI定义了Web服务器和Web应用以及Web中间件之间的交互协议，只要支持WSGI，各种web服务器、web应用和中间件就能相互对接。而Repoze正是将Zope组件按照WSGI进行封装，从而遵循WSGI的python web 框架可以很容易使用这些组件。
最初，Repoze推出了自己的一个非常轻量级的web框架：repoze.bfg。在repoze.bfg中利用了ZCA的特性。于是python web framework世界中又多了一方势力。但恰恰是这个不起眼的小势力推动了历史的进程。repoze.bfg和Pylons类似, 几乎都是完全的策略自由（policy-free），你可以随意的指定数据持久层，模板系统等外围部分，而其核心就是约定了URL如何映射到 view 代码以及如何调用view，在这个过程中完全遵循WSGI规范。后来repoze.cfg的开发者决定将其合并到Pylons。于是拉开了Python web框架合并的序幕。这个合并的框架叫做Pyramid。无独有偶，原本基于Pylons的TurboGears也宣布开发基于Pyramid的分支。
至此，混乱的江湖看到了统一的希望，相信越来越多的python web框架会融合，或至少能够互通。或许，随着更加开放的框架的出现，Django会慢慢淡出大家的视野，让我们拭目以待。
