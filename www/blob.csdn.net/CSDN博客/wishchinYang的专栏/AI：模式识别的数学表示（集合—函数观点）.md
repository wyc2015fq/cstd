# AI：模式识别的数学表示（集合—函数观点） - wishchinYang的专栏 - CSDN博客
2017年07月07日 18:24:10[wishchin](https://me.csdn.net/wishchin)阅读数：571
**前言**：
          模式识别的定义，参考：[模式识别两种方法：知识和数据](http://blog.csdn.net/wishchin/article/details/22654357) 。百科定义：模式识别（英语：Pattern Recognition），就是通过[计算机](http://baike.baidu.com/item/%E8%AE%A1%E7%AE%97%E6%9C%BA)用[数学](http://baike.baidu.com/item/%E6%95%B0%E5%AD%A6)技术方法来研究模式的自动处理和判读。我们把[环境](http://baike.baidu.com/item/%E7%8E%AF%E5%A2%83)与[客体](http://baike.baidu.com/item/%E5%AE%A2%E4%BD%93)统称为“模式”，此处我们称环境与客体的关系为模式。
         然而危机百科。[wiki百科](https://en.wikipedia.org/wiki/Pattern_recognition)。模式识别怎么能归到机器学习里面呢？人也可以进行模式识别的，语法系统-专家系统也可以进行模式识别的。
**一、函数映射**：[单射、双射与满射](http://www.cnblogs.com/wanghetao/archive/2012/03/16/2400619.html)
         参考书籍：《计算机科学中的离散结构》 188页
[数学](http://zh.wikipedia.org/wiki/%E6%95%B0%E5%AD%A6)上，**单射**、**满射**和**双射**指根据其[定义域](http://zh.wikipedia.org/wiki/%E5%AE%9A%E4%B9%89%E5%9F%9F)和[陪域](http://zh.wikipedia.org/wiki/%E9%99%AA%E5%9F%9F)的关联方式所区分的三类[函数](http://zh.wikipedia.org/wiki/%E5%87%BD%E6%95%B0)。
- **单射**：指将不同的变量[映射](http://zh.wikipedia.org/wiki/%E6%98%A0%E5%B0%84)到不同的值的函数。
- **满射**：指[陪域](http://zh.wikipedia.org/wiki/%E9%99%AA%E5%9F%9F)等于[值域](http://zh.wikipedia.org/wiki/%E5%80%BC%E5%9F%9F)的函数。即：对陪域中任意元素，都存在至少一个定义域中的元素与之对应。
- **双射**（也称**一一对应**）：既是[单射](http://zh.wikipedia.org/wiki/%E5%8D%95%E5%B0%84)又是[满射](http://zh.wikipedia.org/wiki/%E6%BB%A1%E5%B0%84)的函数。直观地说，一个双射函数形成一个对应，并且每一个输入值都有正好一个输出值以及每一个输出值都有正好一个输入值。
 （在一些参考书中，“一一”用来指**双射**，但是这里不用这个较老的用法。）
下图对比了四种不同的情况：
- 
双射（单射与满射） 
- 
单射但非满射
- 
满射但非单射
- 
非满射非单射
## 单射（one to one或injective）
        单射复合:第二个函数不必是单射。
       一个函数称为**单射**(**一对一**)如果每个可能的像最多只有一个变量映射其上。等价的有，一个函数是单射如果它把不同值映射到不同像。一个单射函数简称**单射**。形式化的定义如下。
函数 是[单射](http://zh.wikipedia.org/wiki/%E5%8D%95%E5%B0%84)[当且仅当](http://zh.wikipedia.org/wiki/%E5%BD%93%E4%B8%94%E4%BB%85%E5%BD%93)对于所有, 我们有- 一个函数*f* : *A* → *B*是单射当且仅当*A*是空的或*f*是左可逆的，也就是说，存在一个函数*g*: *B* → *A* 使得*g* o *f* = *A*上的恒等函数.
- 因为每个函数都是满射当它的[陪域](http://zh.wikipedia.org/wiki/%E9%99%AA%E5%9F%9F)限制为它的[值域](http://zh.wikipedia.org/wiki/%E5%80%BC%E5%9F%9F)时，每个单射导出一个到它的值域的[双射](http://zh.wikipedia.org/wiki/%E5%8F%8C%E5%B0%84)。更精确的讲，每个单射*f* : *A* → *B*可以分解为一个双射接着一个如下的包含映射。令*f**R* : *A* → *f*(*A*)为把陪域限制到像的*f*，令*i* : *f*(*A*)
 → *B*为从*f*(*A*)到*B*中的包含映射.则*f* = *i* o *f**R*. 一个对偶的分解会对满射成立。
- 两个单射的复合也是单射，但若*g* o *f*是单射，只能得出*f*是单射的结论。参看右图。
## 满射（onto）
满射复合：第一个函数不必为满射
一个函数称为**满射**如果每个可能的像至少有一个变量映射其上，或者说陪域任何元素都有至少有一个变量与之对应。形式化的定义如下：
函数为[满射](http://zh.wikipedia.org/wiki/%E6%BB%A1%E5%B0%84)，[当且仅当](http://zh.wikipedia.org/wiki/%E5%BD%93%E4%B8%94%E4%BB%85%E5%BD%93)对任意，存在满足。- 函数为一个满射，当且仅当存在一个函数满足等于上的[单位函数](http://zh.wikipedia.org/w/index.php?title=%E5%8D%95%E4%BD%8D%E5%87%BD%E6%95%B0&action=edit&redlink=1)。（这个陈述等同于[选择公理](http://zh.wikipedia.org/wiki/%E9%80%89%E6%8B%A9%E5%85%AC%E7%90%86)。）
- 将一个满射的[陪域](http://zh.wikipedia.org/wiki/%E9%99%AA%E5%9F%9F)中每个元素的[原像](http://zh.wikipedia.org/wiki/%E5%87%BD%E6%95%B0#.E5.83.8F.E5.92.8C.E5.8E.9F.E8.B1.A1)集看作一个等价类，我们可以得到以该等价类组成的集合（原定义域的[商集](http://zh.wikipedia.org/wiki/%E5%95%86%E9%9B%86)）为定义域的一个[双射](http://zh.wikipedia.org/wiki/%E5%8F%8C%E5%B0%84)。
- 如果和皆为满射，则为满射。如果是满射，则仅能得出是满射。参见右图。
## 双射（bijective）
双射复合：第一个函数不必为满射、第二个函数不必为单射
既是单射又是满射的函数称为**双射**. 函数为双射[当且仅当](http://zh.wikipedia.org/wiki/%E5%BD%93%E4%B8%94%E4%BB%85%E5%BD%93)每个可能的像有且仅有一个变量与之对应。
函数为[双射](http://zh.wikipedia.org/wiki/%E5%8F%8C%E5%B0%84)[当且仅当](http://zh.wikipedia.org/wiki/%E5%BD%93%E4%B8%94%E4%BB%85%E5%BD%93)对任意存在唯一满足。- 函数*f* : *A* → *B*为双射当且仅当其可逆，即，存在函数*g*: *B* → *A*满足*g* o *f* = *A*上的[恒等函数](http://zh.wikipedia.org/wiki/%E6%81%92%E7%AD%89%E5%87%BD%E6%95%B0)，且*f* o *g*为*B*上的恒等函数。
- 两个双射的[复合](http://zh.wikipedia.org/wiki/%E5%A4%8D%E5%90%88%E5%87%BD%E6%95%B0)也是双射。如*g* o *f*为双射，则仅能得出*f*为单射且*g*为满射。见右图。
- 同一集合上的双射构成一个[对称群](http://zh.wikipedia.org/wiki/%E5%B0%8D%E7%A8%B1%E7%BE%A4)。
- 如果皆为[实数](http://zh.wikipedia.org/wiki/%E5%AE%9E%E6%95%B0)，则双射函数可以被视觉化为两根任意的水平直线只相交正好一次。（这是水平线测试的一个特例。）
### 函数的势
        双射函数经常被用于表明集合*X*和*Y*是等[势](http://zh.wikipedia.org/wiki/%E5%8A%BF)的，即有一样的[基数](http://zh.wikipedia.org/wiki/%E5%9F%BA%E6%95%B0)。如果在两个[集合](http://zh.wikipedia.org/wiki/%E9%9B%86%E5%90%88)之间可以建立一个[一一对应](http://zh.wikipedia.org/wiki/%E4%B8%80%E4%B8%80%E5%AF%B9%E5%BA%94)，则说这两个集合等势。
        如果皆为[有限集合](http://zh.wikipedia.org/wiki/%E6%9C%89%E9%99%90%E9%9B%86%E5%90%88)，则这两个集合中之间存在一个双射，[当且仅当](http://zh.wikipedia.org/wiki/%E5%BD%93%E4%B8%94%E4%BB%85%E5%BD%93)X和Y的[元素](http://zh.wikipedia.org/wiki/%E5%85%83%E7%B4%A0)数相等。其实，在[公理集合论](http://zh.wikipedia.org/wiki/%E5%85%AC%E7%90%86%E9%9B%86%E5%90%88%E8%AB%96)中，*元素数相同*的定义被认为是个特例，一般化这个定义到[无限集合](http://zh.wikipedia.org/wiki/%E6%97%A0%E9%99%90%E9%9B%86%E5%90%88)需要导入[基数](http://zh.wikipedia.org/wiki/%E5%9F%BA%E6%95%B0)的概念，这是一个区别各类不同大小的[无限集合](http://zh.wikipedia.org/wiki/%E6%97%A0%E9%99%90%E9%9B%86%E5%90%88)的方法。
## 举例
对于每个函数给定[定义域](http://zh.wikipedia.org/wiki/%E5%AE%9A%E4%B9%89%E5%9F%9F)和[陪域](http://zh.wikipedia.org/wiki/%E9%99%AA%E5%9F%9F)很重要，因为改变这些就能改变函数属于什么*射*。
### 双射
- 任意集合上的[恒等函数](http://zh.wikipedia.org/wiki/%E6%81%92%E7%AD%89%E5%87%BD%E6%95%B0)id为一双射。
- 考虑函数，定义为。这个函数是双射，因为给定任意一个实数，我们都能解，得到唯一的实数解。
- [指数函数](http://zh.wikipedia.org/wiki/%E6%8C%87%E6%95%B0%E5%87%BD%E6%95%B0) 及其逆函数[自然对数](http://zh.wikipedia.org/wiki/%E8%87%AA%E7%84%B6%E5%AF%B9%E6%95%B0) 。
### 单射、但非满射
- 指数函数
### 满射、但非单射
- 指数函数的反向映射![大笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/laugh.gif)
### 即非单射也非满射
**二、PR的一个栗子**：
         举个栗子：已通常的图像处理为例，若是二分类问题，比如识别出带皮的栗子和不带皮的栗子两种。
**  问题描述**：识别栗子是带皮的还是不带皮的。这是个二分类问题。
![](https://img-blog.csdn.net/20170707180118433?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
         当然，我们人眼一眼就看出图中那些栗子是带皮的，哪些是不带皮的。
**定义域X**：所有带栗子的图片
        这样的：
![](https://img-blog.csdn.net/20170707181044936?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170707181100505?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170707181108849?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170707181113377?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170707181119074?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170707181127173?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
还有这样的：
![](https://img-blog.csdn.net/20170707182824292?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170707182831801?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170707182837101?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170707182851048?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170707182901955?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170707182908820?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
当然，也有这样的：
![](https://img-blog.csdn.net/20170707183318083?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170707183342790?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170707183351421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170707184120248?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170707184130014?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170707184140872?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170707184148699?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
还有这样的：
![](https://img-blog.csdn.net/20170707184405370?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170707184359276?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170707184352655?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
仔细看看，上面四张图片没有栗子，![再见](http://static.blog.csdn.net/xheditor/xheditor_emot/default/bye.gif)，超出定义域了，已经成为另外一个问题，不在讨论的范围，因此舍弃掉。
         所以定义域是所有带栗子的图片。但是图片中有没有栗子，和栗子是带皮的还是不带皮的，这是两个问题。我们假设已经检测到栗子，且把栗子的框全部标记下来，相当于已经抠图。
**  值域Y**：bool表达式 {Yes-带皮的栗子，No-不带皮的栗子}。一般形式化为{1，-1} 或{1,0}。
![](https://img-blog.csdn.net/20170707185316635?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        栗子的是否带皮的模式识别问题：
 f( x ) = { X——>Y }|{  X = 所有栗子的图片，Y={ 1-带皮, 0-不带皮 }  }
函数 f(x)即栗子图片二分类问题的模式。f(x)是一个单射，一个可能的定义 Xi 映射且映射到到一个像 Yi 上。
**PR的一般形式**：
       PR问题是一个根据模式分析的定义X，和模式的种类Y，来确定函数
 f( x ) = { X——>Y }，其中f（x）是一个单射。
       即一般要求：栗子不能同时是有皮的，但同时又是无皮的，模式的种类保持排他性，即模式为划分而不是覆盖，且一般为原子模式。
       疑问：遇到包裹了一半皮的栗子怎样？![疑问](http://static.blog.csdn.net/xheditor/xheditor_emot/default/doubt.gif)这好像超出了栗子图片的定义域。如果希望解决，可以扩充模式的值域使Y={
 1-带皮, 0-带部分皮 ， -1-不带皮} 
 ，这已经与原来的问题不是一个问题。
**三、模式函数映射**
模式函数是一个从问题定义域到模式值域的一个单射。
        从简单的贝叶斯方法，到只能支持二分类的原始支持向量机，到十几个类的分类上最好用的随机森林方法，到可以支持ImageNet上海量1860个类且分类精度极高的InceptionV4（参考：CNN[http://blog.csdn.net/wishchin/article/details/45286805](http://blog.csdn.net/wishchin/article/details/45286805)），其模式函数为
            f( x ) = { X——>Y }|{  X = ImageNet的图片，Y={ 1860个类的标记 }  }
          是一个单射函数。 普遍情况下，定义域小于值域，模式函数成为一个**hash函数**。
**图像处理IP的模式识别**
         以ImageNet为例，模式识别的定义域为{ X = ImageNet的图片  }，不管有多少张图片，这些图像分布在 2维欧式空间（也称为2维张量空间）的 L1*L2（L1维向量空间和L2维向量空间乘积，但两个向量空间不再一个欧式空间内）的长宽的图片集，而值域Y={
 1860个类的标记 } 为0维欧式空间甚至向量空间中的离散个点上，即把定义域中浩瀚的二维图片使用模式函数  f( x ) 压缩到 0维向量空间中的离散个点上，此过程主要功能为降维，也称为**压缩hash函数**。
         所以图像处理的模式识别问题，是一个数学形式为**降维hash**的问题。参考：[Reducing the Dimensionality of Data with Neural Networks：神经网络用于降维](http://blog.csdn.net/wishchin/article/details/45287125)
**语音识别NLP的模式识别**
         自然语言处理是语音识别的一个部分，并可以独立出来，也可以与图像处理结合，成为书面识别。书面图像识别是一个图像处理和模式识别问题，识别之后进行语义分析是NLP问题。
         语音识别加语义识别为引入了时序分析问题。暂且抛开时序分析，单列NLP。
         自然语言从底向上依次为 字母表-单词-句子-语义，其中链接关系为 词法-语法-语义。
         其中词法分析即字母识别，对应了语音识别中的字母语音识别和图像处理中的图像字母识别。模式函数的最终值域为自然语言存在的N个字母。IP中的字母PR问题是以一般的是一个数学形式为**降维hash**的问题，生成有字母字符串。语音识别中的字母分析问题包含了音素分析，定义域为所有可能的语音音素组合，模式函数的值域仍为自然语言存在的N个字母，或者单音节字母组合；然后再解析为所有字母字符串。
         语法分析和语义分析涉及到更复杂的模式分析，传统语法分析方法使用约定文法，现在流行的语法分析和语义分析是基于统计方法的语料分析，并取得了极高的分类识别精度。
         对单词和字母编码，把字母和单词映射到整数集合上，也称为单词嵌入。参考文章：[NLP:单词嵌入Word Embeddings](http://blog.csdn.net/wishchin/article/details/73770837)。原文链接：[http://blog.jobbole.com/77709/](http://blog.jobbole.com/77709/)。把定义从字母和单词分布空间转化到整数集合，即1维张量空间中的N维向量空间，完成编码。
        NLP数据集合：[最好用的 AI 开源数据集(涵盖计算机视觉、NLP、语音等 6 大类）](https://zhuanlan.zhihu.com/p/25887325?utm_source=tuicool&utm_medium=referral)
        对字母和单词编码后，开始了NLP的语法和语义模式识别问题。与图像处理与模式识别的方法将有很大区别，扯远了，到此为止。
