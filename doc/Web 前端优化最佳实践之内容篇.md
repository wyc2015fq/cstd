# Web 前端优化最佳实践之内容篇

 



转载 ：<http://www.dbanotes.net/web/best_practices_for_speeding_up_your_web_site_content.html>

Yahoo! 的 Exceptional Performance team 在 Web 前端方面作出了卓越的贡献。广为人知的[优化规则](http://developer.yahoo.com/performance/rules.html)也由 13 条到 14 条，再到 20 条，乃至现在的 [34 条](http://developer.yahoo.com/performance/)--真是与时俱进啊。最新的 34 条也针对不同的角度做了分类。

面向内容的优化规则目前有 10 条。

#### 1. 尽量减少 HTTP 请求 (Make Fewer HTTP Requests) 

作为第一条，可能也是最重要的一条。根据 Yahoo! 研究团队的数据分析，有很大一部分用户访问会因为这一条而取得最大受益。有几种常见的方法能切实减少 HTTP 请求：

- 1) **合并文件**，比如把多个 CSS 文件合成一个； 
- 2) **CSS Sprites** 利用 CSS background 相关元素进行背景图**绝对**定位；参见：[CSS Sprites: Image Slicing's Kiss of Death](http://alistapart.com/articles/sprites) 
- 3) **图像地图** 
- 4) **内联图象** 使用 [data: URL scheme](http://tools.ietf.org/html/rfc2397) 在实际的页面嵌入图像数据. 

#### 2. 减少 DNS 查找 (Reduce DNS Lookups)

必须明确的一点，DNS 查找的开销是很大的。另外，我倒是觉得这是 Yahoo! 所有站点的通病，Yahoo！主站点可能还不够明显，一些分站点，存在明显的类似问题。对于国内站点来说，如果过多的使用了站外的 Widget ，也很容易引起过多的 DNS 查找问题。

#### 3. 避免重定向 (Avoid Redirects)

不是绝对的避免，尽量减少。另外，应该注意一些不必要的重定向。比如对 Web 站点子目录的后面添加个 / (Slash)  ，就能有效避免一次重定向。http://www.dbanotes.net/arch 与 http://www.dbanotes.net/arch**/** 二者之间是有差异的。如果是 Apache 服务器，通过配置 Alias 或mod_rewrite 或是 DirectorySlash 能够消除这个问题。

#### 4. 使得 Ajax 可缓存 (Make Ajax Cacheable)

响应时间对 Ajax 来说至关重要，否则用户体验绝对好不到哪里去。提高响应时间的有效手段就是 Cache 。其它的一些优化规则对这一条也是有效的。 

#### 5. 延迟载入组件 (Post-load Components)

#### 6. 预载入组件 (Preload Components)

上面两条严格说来，都是属于**异步**这个思想灵活运用的事儿。

#### 7. 减少 DOM 元素数量 (Reduce the Number of DOM Elements)

#### 8. 切分组件到多个域 (Split Components Across Domains)

主要的目的是提高页面组件并行下载能力。但不要跨太多域名，否则就和第二条有些冲突了。

#### 9. 最小化 iframe 的数量 (Minimize the Number of iframes)

熟悉 SEO 的朋友知道 iframe 是 SEO 的大忌。针对前端优化来说 iframe 有其好处，也有其弊端，一分为二看问题吧。

#### 10. 杜绝 http 404 错误 (No 404s)

对页面链接的充分测试加上对 Web 服务器 error 日志的不断跟踪能有效减少 404 错误，亦能提升用户体验。值得一提的是，CSS 与 Java Script 引起的 404 错误因为定位稍稍"难"一点而往往容易被忽略。

这是内容篇的 10 条。应该说具体引导性的内容还不够详细。逐渐会根据自己的理解补充上来。

--EOF--