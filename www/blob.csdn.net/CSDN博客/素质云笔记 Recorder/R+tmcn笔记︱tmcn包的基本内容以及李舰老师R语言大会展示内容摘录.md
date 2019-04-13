
# R+tmcn笔记︱tmcn包的基本内容以及李舰老师R语言大会展示内容摘录 - 素质云笔记-Recorder... - CSDN博客

2016年09月07日 16:50:55[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：6059所属专栏：[R语言与自然语言处理](https://blog.csdn.net/column/details/13670.html)




**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)
———————————————————————————
tmcn包目前托管在在[R-forge](https://r-forge.r-project.org/R/?group_id=1571)上开发和发布。下载方式以及[Rforge相关链接](https://r-forge.r-project.org/R/?group_id=1571)：

`install.packages("tmcn", repos = "http://R-Forge.R-project.org")`

tmcn 包是一个进行中文文本挖掘的R包。包含了中文编码处理、文字操作、 文本挖掘模型和算法的一些函数。主要针对目前可用的 R 资源的不足而开发的。
目前最常用的文本挖掘包是 tm 包，该包已经成了事实上的分析标准， 几乎所有的其他 R 包都是基于这个框架的。但是 tm 包具有一些缺陷， 在R中进行分析的时候不是很方便。

最明显的问题是中文支持得不够好， 其函数的设计并没有考虑到国际化的需求和 UTF-8 的支持，很多函数操作中文时不方便。 此外，tm 包的开发大量使用了 S3 的面向对象方法，其最大的价值是为后续的开发者提供了接口， 但是这些对象对于使用者来说并没有什么便利，增加了学习的复杂度， 而且由于 S3 封装性上天然的缺陷，初学者容易出错而且提示不清楚。另外， tm 包及相关体系完全基于文档词条矩阵的数据结构，在大量数据的工程化实现方面非常便利， 但是所有的这些包包括 tm 在内并没有简单的能够高性能运算的机制， 该设计的优势在R中完全没有被体现。
基于目前 R 中可用的文本挖掘资源的不足，tmcn 试图去解决这些问题， 先从中文支持开始，然后逐渐更新去解决各种问题，但是也会考虑到 tm 的框架， 在框架之外进行一些有益的补充。
（摘录自tmcn官方主页：http://jianl.org/cn/R/tmcn.html）

**后续的李舰老师还基于tmcn模块开发了，CRF模块以及word2vec模块**。
CRF模块可见李舰老师R语言大会分享内容；
word2vec模块，可见笔者博客：[重磅︱文本挖掘深度学习之word2vec的R语言实现](http://blog.csdn.net/sinat_26917383/article/details/51319312)
# NLP︱R语言实现word2vec（词向量）经验总结（消除歧义、词向量的可加性）


————————————————————————————————————————


# 一、字符编码UTF-8 GBK unicode

GB2312(CP936)+改进=GBK--→unicode--→UTF-8

## 1、GBK
1993年，[Unicode](https://zh.wikipedia.org/wiki/Unicode)1.1版本推出，收录[中国大陆](https://zh.wikipedia.org/wiki/%E4%B8%AD%E5%9B%BD%E5%A4%A7%E9%99%86)、[台湾](https://zh.wikipedia.org/wiki/%E5%8F%B0%E6%B9%BE)、[日本](https://zh.wikipedia.org/wiki/%E6%97%A5%E6%9C%AC)及[韩国](https://zh.wikipedia.org/wiki/%E9%9F%A9%E5%9B%BD)通用[字符集](https://zh.wikipedia.org/wiki/%E5%AD%97%E7%AC%A6%E9%9B%86)的[汉字](https://zh.wikipedia.org/wiki/%E6%B1%89%E5%AD%97)，总共有20,902个。
中国大陆订定了等同于Unicode 1.1版本的“[GB 13000.1-93](https://zh.wikipedia.org/wiki/GB_13000)”“信息技术通用多八位编码字符集（UCS）第一部分：体系结构与基本多文种平面”。
由于[GB 2312-80](https://zh.wikipedia.org/wiki/GB_2312)只收录6763个汉字，有不少汉字，如部分在GB 2312-80推出以后才简化的汉字（如“啰”），部分人名用字（如中国前总理[朱镕基](https://zh.wikipedia.org/wiki/%E6%9C%B1%E9%95%95%E5%9F%BA)的“镕”字），台湾及香港使用的[繁体字](https://zh.wikipedia.org/wiki/%E7%B9%81%E4%BD%93%E4%B8%AD%E6%96%87)，[日语](https://zh.wikipedia.org/wiki/%E6%97%A5%E8%AF%AD)及[朝鲜语](https://zh.wikipedia.org/wiki/%E9%9F%93%E8%AA%9E)汉字等，并未有收录在内。于是厂商微软利用GB 2312-80未使用的编码空间，收录GB 13000.1-93全部字符制定了GBK编码。
根据[微软](https://zh.wikipedia.org/wiki/%E5%BE%AE%E8%BD%AF)资料，GBK是对GB2312-80的扩展，也就是*[CP936](https://zh.wikipedia.org/wiki/CP936)**字码表（Code Page 936）*的扩展（之前CP936和GB 2312-80一模一样），最早实现于[Windows 95](https://zh.wikipedia.org/wiki/Windows_95)简体中文版。虽然GBK收录[GB 13000.1-93](https://zh.wikipedia.org/wiki/GB_13000)的全部字符，但编码方式并不相同；因为GBK向下兼容GB2312，而GB 13000.1-93等同于Unicode 1.1，二者的编码方式完全不兼容。
汉字内码扩展规范，称GBK，全名为《汉字内码扩展规范(GBK)》
（来自维基百科）

列举几个GBK的编码：
`81 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
4 丂  丄  丅  丆  丏  丒  丗  丟  丠  両  丣  並  丩  丮  丯  丱  
5 丳  丵  丷  丼  乀  乁  乂  乄  乆  乊  乑  乕  乗  乚  乛  乢  
6 乣  乤  乥  乧  乨  乪  乫  乬  乭  乮  乯  乲  乴  乵  乶  乷  
7 乸  乹  乺  乻  乼  乽  乿  亀  亁  亂  亃  亄  亅  亇  亊  
8 亐  亖  亗  亙  亜  亝  亞  亣  亪  亯  亰  亱  亴  亶  亷  亸  
9 亹  亼  亽  亾  仈  仌  仏  仐  仒  仚  仛  仜  仠  仢  仦  仧  
A 仩  仭  仮  仯  仱  仴  仸  仹  仺  仼  仾  伀  伂  伃  伄  伅  
B 伆  伇  伈  伋  伌  伒  伓  伔  伕  伖  伜  伝  伡  伣  伨  伩  
C 伬  伭  伮  伱  伳  伵  伷  伹  伻  伾  伿  佀  佁  佂  佄  佅  
D 佇  佈  佉  佊  佋  佌  佒  佔  佖  佡  佢  佦  佨  佪  佫  佭  
E 佮  佱  佲  併  佷  佸  佹  佺  佽  侀  侁  侂  侅  來  侇  侊  
F 侌  侎  侐  侒  侓  侕  侖  侘  侙  侚  侜  侞  侟  価  侢`


## 2、UTF-8

互联网的普及，强烈要求出现一种统一的编码方式。UTF-8就是在互联网上使用最广的一种Unicode的实现方式。其他实现方式还包括UTF-16（字符用两个字节或四个字节表示）和UTF-32（字符用四个字节表示），不过在互联网上基本不用。重复一遍，这里的关系是，UTF-8是Unicode的实现方式之一。
GBK转化为UTF-8一定要经过unicode的过程。

————————————————————————————————————————


# 二、tmcn包的内容

tmcn内容大致包括，格式转化、字符操作

## 1、GBK字符集

`#GBK字符集
data(GBK)
head(GBK)``GBK py0        py Radical Stroke_Num_Radical                   Stroke_Order Structure   Freq
1  吖   a      ā yā      口                  3                   丨フ一丶ノ丨      左右     26
2  阿   a     ā ɑ ē      阝                  2                 フ丨一丨フ一丨      左右 526031
3  啊   a ɑ á à ǎ ā      口                  3           丨フ一フ丨一丨フ一丨    左中右  53936
4  锕   a         ā      钅                  5       ノ一一一フフ丨一丨フ一丨    左中右      3
5  錒   a         ā      釒                  8 ノ丶一一丨丶ノ一フ丨一丨フ一丨      左右      0
6  嗄   a     á shà      口                  3     丨フ一一ノ丨フ一一一ノフ丶      左右     11`

## 2、格式转化

检验编码类型用的是：Encoding（）函数，还有isUTF8(txt1)  isGBK(txt2)   isGBK(txt3)

`isUTF8(txt1)
isGBK(txt2)
isGBK(txt3)`
常规的字符格式转化用iconv

`txt1 <- c("\u4E2D\u56FDR\u8BED\u8A00\u4F1A\u8BAE")  #UTF-8编码
txt2 <- iconv(txt1, "UTF-8", "GBK")                 #icov把txt1字符串从utf8转化为GBK`也可以通过Encoding来表示：
`txt3 <- txt1
Encoding(txt3) <- "GBK"                             #改变编码，encoding是检验编码类型的`
tmcn中比较好的格式转化的函数：
`toUTF8(txt1)        #其他格式（GBK UTF-8）输出中文
catUTF8(txt1)       #中文以及其他格式输出UTF8
revUTF8("<U+4E2D><U+56FD>R<U+4F1A><U+8BAE>")  #把UTF8变成中文`
toUTF8()比较棒！可以多加留意！

## 3、繁简体以及拼音生成

`#繁体与拼音改写
toTrad(txt1)                        #繁体字
toTrad("中國R語言會議", rev = TRUE) #rev=TRUE代表由繁到简，默认为FALSE为由简到繁
toPinyin(txt1, capitalize = TRUE)   #由中文变成拼音，capitalize默认为FALSE，代表首字符小写`

## 4、字符操作

字符操作有根据正则表达式匹配，以及修缮语料的前后空格。但是不能去掉中间的空格。
`#字符操作
txt2 <- c("  Ben Franklin and Jefferson Davis", "\tMillard Fillmore")
#根据正则表达式匹配
strextract(txt2, "(?<first>[[:upper:]][[:lower:]]+)", perl = TRUE)   #根据正则表达式匹配内容
#修缮语料两边（去掉文章前后的空格，换行符）
strstrip(c("\taaaa ", " bbbb    ","  有这么坑爹的吗？","真、    的这么坑爹吗？"))`

————————————————————————————————————————

# 三、李舰老师在R语言大会展示内容的摘录

李健老师在2013年的R语言大会就已经把R语言的NLP相关的包都罗列并翻译了一遍，真是厉害。原官方网站链接可见：https://cran.r-project.org/web/views/NaturalLanguageProcessing.html

## 分析框架部分

1、openNLP

Apá
3
T
ù
ƒ
:
̨
?
1
g
m
u
"
•
©
|
±
ÿ
¥
È–
可以单句识别、句分解、句结构分析，构建语法树等；相对比较底层的一般文本挖掘任务，该包基础上进行二次开发比较好。
而且该包对中文的支持不是特别好。
笔者做过测试，可见博客：
# R+openNLP︱openNLP的六大可实现功能及其在R语言中的应用


2、qdap
一个综合了定量分析以及定性分析的大杂烩，包含了一些自然语言的函数。

3、koRpus
综合文本分析的包，词频分析较多；可读性分析以及语种识别比较有特色。

## 词分析

4、RKEA
提供了KEA的接口可以用来进行关键词提取。

5、wordcloud
画词云，现在已经升级了，可见我的博客：[R语言︱文本挖掘——词云wordcloud2包](http://blog.csdn.net/sinat_26917383/article/details/51620019)

6、zipfR
提供了一些关于词频分布的统计模型，尤其是词频分布中最常用的个zipf定律。

7、wordnet包
提供了英文文本数据库的接口

8、koNLP
一个韩文的自然语言处理的包

9、snowball snowballC Rstem
词干提取的包

## 语义分析

10、topicmodels lda
topicmodels提供了c接口使用LDA和相关的主题模型。lda也是主题建模。

11、RTextTools
自动文本分类

12、skmeans
几种模糊KMeans算法。

13、textcat
可以进行基于n-gram短语的文本聚类

14、movMF
提供了基于概率模型，基于vMF分布的文本聚类方法

15、lsa
潜语义分析，对文档词条矩阵进行奇异值分解来降维，然后计算相似度。进行文本相似性分析。

16、kernlab
提供了一些核机器学习的方式进行文本分类、聚类、新颖性检测降维等。

17、textir
提供了一些函数进行文本和语义挖掘。

————————————————————————————————————————


参考文献：tmcn官方主页：http://jianl.org/cn/R/tmcn.html
第六届中国R语言大会李舰老师分享内容：chrome-extension://ikhdkkncnoglghljlkmcimlnlhkeamad/pdf-viewer/web/viewer.html?file=http%3A%2F%2Fcos.name%2Fwp-content%2Fuploads%2F2013%2F11%2FChinaR2013SH_Nov03_07_LiJian.pdf



**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)
———————————————————————————


