# Swift笔记（一）：Extended Grapheme Clusters - game3108的专栏 - CSDN博客
2016年10月28日 16:43:34[game3108](https://me.csdn.net/game3108)阅读数：619
## 前言
简书地址：[http://www.jianshu.com/p/ccde89ad9c1b](http://www.jianshu.com/p/ccde89ad9c1b)
最近在看苹果官方swift文档[《The Swift Programming Language》](https://developer.apple.com/library/prerelease/content/documentation/Swift/Conceptual/Swift_Programming_Language/index.html#//apple_ref/doc/uid/TP40014097-CH3-ID0)，记录一些笔记。
## Extended Grapheme Clusters
swift使用了Extended Grapheme Clusters作为Character的显示。内容如下：
> 
Every instance of Swift’s Character 
   type represents a single *extended grapheme cluster*. An extended grapheme cluster is a sequence of one or more Unicode scalars that (when combined) produce a single human-readable character.
具体的定义可以参考unicode的标准文档[Grapheme Cluster Boundaries](http://www.unicode.org/reports/tr29/#Grapheme_Cluster_Boundaries)：
Unicode标准提供了算法去定义grapheme cluster boundaries，其中包括两种变种：***legacy grapheme clusters*** and ***extended grapheme clusters***。
> 
A ***legacy grapheme cluster*** is defined as a base (such as A or カ) followed by zero or more continuing characters. One way to think of this is as a sequence of characters that form a “stack”.
An ***extended grapheme cluster*** is the same as a legacy grapheme cluster, with the addition of some other characters. The continuing characters are extended to include all spacing combining marks, such as the spacing (but dependent) vowel signs in Indic scripts.
具体的计算方式可以从文章中进行详细的了解。
## 举例
拿一个Apple文档里的例子：
```
let precomposed: Character = "\u{D55C}"                  // 한
let decomposed: Character = "\u{1112}\u{1161}\u{11AB}"   // ᄒ, ᅡ, ᆫ
// precomposed is 한, decomposed is 한
```
韩文的音节可以拆分和组合，上面的两个String就是相同的String。
**因为这种编码方式的问题，Swift想取一个String的字符个数，需要使用`"".characters.count`的方式，获取character，再获取chara的个数。**
Swift这边的String用的是21bit Unicode scalar字符编码方式（相当于UTF-32），而OC中的NSString用的是UTF-16字符编码方式。 
所以对同一个String，转化为NSString，可能获得的长度方式也不同:
```
var str = "Hello ��" // the square is an emoji
str.characters.count // returns 7
(str as NSString).length // returns 8
```
就是现在**所见非所得了**，所以在处理swift string与nsstring转化时，要注意一下unicode的编码和长度问题。
(这边如果你是csdn看的话，你会发现这个emoji表情再csdn这会转化为两个?号。这也就是csdn的页面编码方式不支持emoji)
## 参考资料
1.[The Swift Programming Language](https://developer.apple.com/library/prerelease/content/documentation/Swift/Conceptual/Swift_Programming_Language/index.html#//apple_ref/doc/uid/TP40014097-CH3-ID0)
2.[Why is Swift counting this Grapheme Cluster as two characters instead of one?](http://stackoverflow.com/questions/35823611/why-is-swift-counting-this-grapheme-cluster-as-two-characters-instead-of-one)
3.[Grapheme Cluster Boundaries](http://www.unicode.org/reports/tr29/#Grapheme_Cluster_Boundaries)
