# Web 前端优化最佳实践之 Cookie 篇

 

转载：<http://www.dbanotes.net/web/best_practices_for_speeding_up_your_web_site_server.html>

Web 前端优化最佳实践第三部分面向 Cookie 。目前只有 2 条实践规则。

#### 1. 缩小 Cookie (Reduce Cookie Size)

Cookie 是个很有趣的话题。根据 [RFC 2109](http://www.ietf.org/rfc/rfc2109.txt)  的描述，每个客户端最多保持 300 个 Cookie，针对每个域名最多 20 个 Cookie (实际上多数浏览器现在都比这个多，比如  Firefox 是 50 个) ，每个 Cookie 最多 4K，注意这里的 4K 根据不同的浏览器可能不是严格的 4096 。别扯远了，对于  Cookie 最重要的就是，尽量控制 Cookie 的大小，不要塞入一些无用的信息。

#### 2. 针对 Web 组件使用域名无关性的 Cookie (Use Cookie-free Domains for Components)

这个话题在此前针对 [Web 图片服务器](http://www.dbanotes.net/web/web_image_server.html)的讨论中曾经提及。这里说的 Web 组件(Component)，多指静态文件，比如图片 CSS 等，Yahoo! 的静态文件都在 yimg.com 上，客户端请求静态文件的时候，减少了 Cookie 的反复传输对主域名 (yahoo.com) 的影响。

从这篇 [When the Cookie Crumbles](http://yuiblog.com/blog/2007/03/01/performance-research-part-3/) 能看出，MySpace 和 eBay 的 Cookie 都不小的，想必是对用户行为比较关心。[eBay 前不久构造了 Personalization Platform](http://www.dbanotes.net/database/ebay_personalization_platform_mysql.html) ，就是从 Cookie 的限制中跳出来。 