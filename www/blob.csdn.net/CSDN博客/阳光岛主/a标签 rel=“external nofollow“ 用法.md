
# a标签 rel=“external nofollow“ 用法 - 阳光岛主 - CSDN博客

2018年12月06日 08:27:26[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：537


NoFollow 是 Google 曾经提出的一个a新标签，目的是减少垃圾留言（例如米扑博客的留言：[https://blog.mimvp.com/about\#comments](https://blog.mimvp.com/about#comments)），解释当前页面与此留言链接指向页面之间的关系。使用了此标签表明链接与网站作者无关，也就是说搜索引擎不会用这个链接计算网站的 PR 值。很多博客程序都会自动在评论链接中加上 nofollow 标签。现在主流的 Blog 程序，如 WordPress 和 MovableType 均默认为其留言与 trackback 中的链接自动添加 nofollow 属性。这样可以使 Spammer 意图通过这种方式提高网站流行度的目标落空。
米扑博客原文：[a标签 rel=“external nofollow“ 用法](https://blog.mimvp.com/article/26224.html)
rel 有很多的属性值，比如 next、previous、chapter、section 等，本文不做解说，自行百度科普。
比较常见的就是【rel="nofollow"】和【rel="external nofollow"】这两种属性值的应用了。
**网页源码里，a标签如下：**
<a ref="external nofollow" target="_blank" href="https://blog.mimvp.com" title="i love mimvp.com">米扑博客</a>
**解释说明：**
nofollow 、external nofollow：告诉搜索引擎，不要将链接计入权重中，他是告诉一些非本站的链接、不想传递权重，例如网页里的统计代码、备案号链接、供用户查询链接等
external ：告诉搜素引擎，这是非本站的链接，这个作用相当于target=“_blank”
external 和 nofollow 这两个属性的大致可以解释为 “这个链接非本站链接，不要爬取也不要传递权重”

**ref="external nofollow"**
ref="nofollow" 的作用，它是告诉搜索引擎，不要将该链接计入权重。
多数情况下，可以将一些不想传递权重的链接进行nofllow处理；
例如一些非本站的链接，不想传递权重，但是又需要加在页面中的像统计代码、备案号链接、供用户查询的链接等等。
除了 ref="nofollow" 还有两种链接属性形式，一种是 ref="external"，另一种是 ref="external nofollow"。这两种分别代表什么意思呢？又有何区别呢？
## rel="external"
此属性的意思是告诉搜索引擎，这个链接不是本站链接，其实作用相当于target=‘_blank’。
为什么要这样写呢？因为有些网站因为是采用严格的DOCTYPE声明的，如果你在网页源码中的第一行看到：在这种情况下target="_blank"会失效，因此采用rel="external"这个参数来替代。

## rel="external nofollow"
上面已经说过rel=’nofollow’ 和 rel=’external’ 两种属性的所代表的意思了。这个属性基本上是相当于将两种属性结合起来，大致可以解释为 “这个链接非本站链接，不要爬取也不要传递权重”。因此在SEO的角度来说，是一种绝对隔绝处理的方法，可以有效减少蜘蛛爬行的流失。

## rel="nofollow noopener noreferrer"
超链接 target="_blank" 要增加 rel="nofollow noopener noreferrer" 来堵住钓鱼安全漏洞。如果你在链接上使用 target="_blank"属性，并且不加上rel="noopener"属性，那么你就让用户暴露在一个非常简单的钓鱼攻击之下。
为了告知来自于不受保护的站点的用户，我们运行一个利用了这个缺陷的脚本。
|1
|2
|3
|if (window.opener) {
|window.opener.location = "https://mimvp.com/fishing?referrer="+document.referrer;
|}
|
我相信绝大多数站点都没有恰当地处理这个问题，为了限制 window.opener的访问行为，原始页面需要在每个使用了target="_blank"的链接中加上一个rel="noopener"属性。
然而，火狐不支持这个属性值，所以实际上你要用 rel="noopener noreferrer"来完整覆盖。
**target="_blank"属性特别提示**
当你浏览一个页面点击一个a标签连接 <a href="mimvp.com" target="_blank"> 跳转到另一个页面时，
在新打开的页面（mimvp）中可以通过 window.opener获取到源页面的部分控制权， 即使新打开的页面是跨域的也照样可以（例如 location 就不存在跨域问题）。
**rel=noopener 新特性**
<a href="mimvp.com" target="_blank" rel="noopener noreferrer"></a>
在chrome 49+，Opera 36+，打开添加了rel=noopener的链接， window.opener 会为null。
在老的浏览器中，可以使用 rel=noreferrer 禁用HTTP头部的Referer属性，使用下面JavaScript代替target='_blank' 的解决此问题：
|1
|2
|3
|var otherWindow = window.open('https://mimvp.com');
|otherWindow.opener = null;
|otherWindow.location = url;
|
使用 window.open 打开页面，手动剑opener设置为null。
更多资料，请参考：[超链接 target="_blank" 要增加 rel="nofollow me noopener noreferrer" 来堵住钓鱼安全漏洞](https://my.oschina.net/jsan/blog/741317)

