# 科学网—Google的Go语言编程 - 王伟华的博文




# Google的Go语言编程                           

已有 7178 次阅读2012-5-13 18:56|个人分类:[计算机&大数据](http://blog.sciencenet.cn/home.php?mod=space&uid=81613&do=blog&classid=11842&view=me)|系统分类:[教学心得](http://blog.sciencenet.cn/home.php?mod=space&do=blog&view=all&uid=81613&catid=9)|关键词:Google,编程语言,,Go|[编程语言](misc.php?mod=tag&id=33007)



Go的定位是系统编程，比如Web服务器、存储系统和数据库等，但也欢迎在其它方面的应用。据Google官方的消息，该语言还有望被用于其它领域。现在Google终于在其[官方博客宣](http://blog.golang.org/2012/03/go-version-1-is-released.html)布Go语言首个稳定版本发布，这也标志着第一个原生支持Windows用户的自然语言发布。Google发布稳定版本的目的是给开发者的开发提供一个固定的努力目标。直到现在，该语言仍发生着定期改变，有些可能与现有代码完全不同。从现在开始，除非必要，Google工程师将确保新特性与已有特性兼容。Go语言的很大一部分灵感来源于C语言，同时对像Pascal、Newsqueak和Limbo这样的语言也有借鉴。Go语言定位于系统程序开发，具有简洁、快速、安全、并行、有趣、开源等特色。同时支持泛型编程，内存管理，数组安全，编译迅速。同时发布的包括 Go 编程语言、官方库和工具集的 1.0 稳定版本。同步开发的 GAE 团队也同步发布 1.6.4 版的 Google App Engine SDK for the Go runtime，提供对新版GO语言的支持。
闲话少说，还是先来段代码过过瘾吧，哈哈！
简单的Hello World程序：
package mainimport "fmt"  // Package implementing formatted I/O.
func main() 

{fmt.Printf("Hello, 世界n")} 


在这里我也给出一些学习Go语言的材料共各位需要了解Go语言的博友参考。（均来自网上，无版权，请勿商用！）
1. [go(Google)语言精要.pdf](http://blog.sciencenet.cn/home.php?mod=attachment&id=21195)

2.[《Google_Go：初级读本》.pdf](http://blog.sciencenet.cn/home.php?mod=attachment&id=21196)

3.[google的Go语言编程.pdf](http://blog.sciencenet.cn/home.php?mod=attachment&id=21197)

4.[Effective_Go.pdf](http://blog.sciencenet.cn/home.php?mod=attachment&id=21198)

（未完待续）



转载本文请联系原作者获取授权，同时请注明本文来自王伟华科学网博客。
链接地址：[http://blog.sciencenet.cn/blog-81613-570492.html](http://blog.sciencenet.cn/blog-81613-570492.html)

上一篇：[统计编程--R语言简介](blog-81613-570484.html)
下一篇：[奇妙的数学](blog-81613-571397.html)



