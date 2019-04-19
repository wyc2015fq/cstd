# 聊聊 StackOverflow 的标签引擎（中） - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [赵荣](http://www.jobbole.com/members/q3612171732) 翻译，[sunshinebuel](http://www.jobbole.com/members/sunshinebuel) 校稿。未经许可，禁止转载！
英文出处：[Matt Warren](http://mattwarren.github.io/2015/08/19/the-stack-overflow-tag-engine-part-2/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
我已经将相关 [资源](http://mattwarren.github.io/resources/) 和之前发表的 [演讲](http://mattwarren.github.io/speaking/) 的页面加到我的网站中了，如果你想了解更多，可以去看看。其中还有一篇我2014年我在伦敦NDC会议上发表的演讲视频：“[性能也是一种特性](http://mattwarren.github.io/speaking/#NDCLondon2014)”
## 要点回顾
这是介绍Stack Overflow标签引擎系列文章中的第二篇，[如果之前没看过第一篇，建议您先阅读](http://blog.jobbole.com/99910/)。
上一篇文章发布后，Stack Overflow公开了一个份非常漂亮的性能报告，里面有更多标签引擎的统计数据。从下图中，你可以看到他们把标签引擎部署在几台性能比较强悍的机器上*（译者注：这个估计是以当年的标准算强悍）*，但实际上，CPU使用峰值只有10%，这意味着这套系统还有很大潜力，所以访问需求激增或者某天业务特别繁忙时，它依然hold的住。
![](http://jbcdn2.b0.upaiyun.com/2016/01/28c8edde3d61a0411511d3b1866f06363.png)
## 设置忽略标签
[在第一篇文章中](http://blog.jobbole.com/99910/)，我只做了一些最简单的事情，比如查询带有某个特定标签的问题，还有一些多条件排序（根据分数，点击量等等）。但是事实上，标签引擎需要处理更多更复杂的事情，比如下面这位说的：
![](http://jbcdn2.b0.upaiyun.com/2016/01/665f644e43731ff9db3d341da5c827e12.png)
上图中这位想表达什么呢？他的重点是说我们做标签引擎时，任何时候要记住每个人都是可定制从搜索结果中排除哪些结果的，而且排除哪些是可以设置的，你可以在“忽略标签列表”中设置，比如你设置了某个忽略标签，那么当你搜索时，带这些标签的问题都不会出现在你的眼前。
**注意：**如果能让你知道有哪些问题因为你的偏好设置被隐藏了，这样用户体验会更好。一旦有搜索结果因为你的偏好设置被隐藏时，你可以看到如下的信息：（当然，也可以设置不要把问题隐藏起来，而将命中的问题标记成灰色）：
![](http://jbcdn2.b0.upaiyun.com/2016/01/38026ed22fc1a91d92b5d2ef93540f202.png)
正常情况下，绝大部分人应该只有很少的忽略标签，最多不会超过10个。但是这个世界总从不缺乏例外，和我接触过的一位Stack Overflow的深度用户，[@leppia](http://stackoverflow.com/users/15541/leppie)，分享了一下他设置的标签列表*（译者注：我把这篇文章贴下来翻译时，看到足足14页长度，深深的以为被黄利民推荐的这篇文章给坑了，但是发现这个标签列表占了10页，oh，yeah…）*：


```
vb6 vba homework grails coldfusion flash iphone air sifr ms-access db2 vbscript perl sap jpa gql java-ee magento ipad qt weblogic blackberry gwt pentaho wordpress mac corba intellij-idea lucene
```
点击[**这里**](https://github.com/login/oauth/authorize?client_id=7e0a3cd836d3e544dbd9&redirect_uri=https%3A%2F%2Fgist.github.com%2Fauth%2Fgithub%2Fcallback%3Freturn_to%3Dhttps%253A%252F%252Fgist.github.com%252Fleppie%252F4d9b84abd8c2d06d6ef4&response_type=code#file-gistfile1-txt)下载完整的标签列表。
你可能要花很大功夫才能看完整个标签列表，为了节省时间，下面列出了一些统计数据：
- 这份清单有3753个标签，其中210个带有通配符（比如cocoa*, *hibernate*这样的）
- 这些标签和通配符展开以后，总共会生成7677个标签（整个stack overflow大概有30529个标签）*（译者注：这是写这篇文章时的数字）*
- 有6428251个问题至少命中这7677个标签中的一个！
## 通配符
如果你想感受一下使用通配符过滤，你可以访问下面的url：
- [*java*](http://stackoverflow.com/questions/tagged/*java*?sort=votes)
- [facebook-javascript-sdk] [java] [java.util.scanner] [java-7] [java-8] [javabeans] [javac] [javadoc] [java-ee] [java-ee-6] [javafx] [javafx-2] [javafx-8] [java-io] [javamail] [java-me] [javascript] [javascript-events] [javascript-objects] [java-web-start]
- [.net*](http://stackoverflow.com/questions/tagged/.net*?sort=votes)
- [.net] [.net-1.0] [.net-1.1] [.net-2.0] [.net-3.0] [.net-3.5] [.net-4.0] [.net-4.5] [.net-4.5.2] [.net-4.6] [.net-assembly] [.net-cf-3.5] [.net-client-profile] [.net-core] [.net-framework-version] [.net-micro-framework] [.net-reflector] [.net-remoting] [.net-security] [.nettiers]
一种简单的处理方法就像下面代码中描述的那样，具体说来就是遍历所有的通配符标签，然后将每个通配符标签和所有标签比对，看看是否匹配。（Is ActualMatch(…)方法是个简单的字符串匹配函数，其实[StartsWith](https://msdn.microsoft.com/en-us/library/baketfxw(v=vs.110).aspx)，[EndsWith](https://msdn.microsoft.com/en-us/library/2333wewz(v=vs.110).aspx)或者[Contains](https://msdn.microsoft.com/en-us/library/dy85x1sa(v=vs.110).aspx)更加合适一些。*（译者注：原作者是用VB.net写的，所以这里给的是三个方法是MSDN上的方法介绍，用来判断一个字符串是不是以某个字符串开头、结尾，或者包含某个字符串）*。


```
var expandedTags = new HashSet();
foreach (var wildcard in wildcardsToExpand){ 
    if (IsWildCard(tagToExpand)) { 
         var rawTagPattern = tagToExpand.Replace("*", ""); 
                foreach (var tag in allTags) {
                    if (IsActualMatch(tag, tagToExpand, rawTagPattern)) expandedTags.Add(tag);
                } 
     } else if (allTags.ContainsKey(tagToExpand)) {
            expandedTags.Add(tagToExpand); 
    }
}
```
上面的代码在带有通配符标签较少的时候工作的很好，但是效率上不是很尽如人意。其实32000个标签，相对来说并不是很多，但即使这样，当wildcardsToExpand中含有210个元素时，都需要处理1秒钟了。在Twitter上和几个Stack overflow的员工聊过以后，他们认为，如果标签引擎的一次查询要超过500毫秒的话，就太慢了，所以，如果要花1秒钟来处理带有通配符的标签的话，是一件不能忍的事情。*（译者注：上面代码其实是做了一个双重循环，将所有匹配通配符的标签都拎出来，效率应该是O(mn)，当m和n都不是很小的时候，显然有必要优化）*
## Trigram Index
*（译者注：我不知道该如何准确、专业地翻译Trigram Index这个专业术语。。。）* 那我们可以做的更好么？其实正好有一项技术可以完美的解决这个问题，“[使用Trigram Index进行正则表达式匹配](https://swtch.com/~rsc/regexp/regexp4.html)”，这个技术也被用在[Google Code的搜索功能中](https://code.google.com/p/chromium/codesearch)。我不会在这里详细的解释此技术，想要了解更多，可以看一下前面链接中的文章，那里面已经写得非常详尽了。简单说起来，就是你需要为标签建立一个倒排索引，然后再来对这个索引进行搜索。这样带有通配符的标签数量就不会造成多大的影响，因为你已经不需要对整个标签列表做遍历了。
所谓Trigram，就是先标签将被分成3个字母一组的片段，比如javascript这个标签，将被划分为如下片段：（’_’用来表示开头和结尾）


```
_ja, jav, ava, vas, asc, scr, cri, rip, ipt, pt_
```
接下来，把所有标签都分成这样的3个字母的片段后，再对这样的片段建立索引，同时标记它们在标签中出现的位置，这样后面可以回溯：
- _ja -> { 0, 5, 6 }
- jav -> { 0, 5, 12 }
- ava -> { 0, 5, 6 }
- va_ -> { 0, 5, 11, 13 }
- _ne -> { 1, 10, 12 }
- net -> { 1, 10, 12, 15 }
- …
举个例子，如果你想要匹配所有包含java的标签，比如*java*，你可以找到jav和ava的索引值，假设从上面列出的片段列表中，会检索出2个匹配的元素：
- jav -> { 0, 5, 12 }
- ava -> { 0, 5, 6 }
现在你可以知道只有所有带有0和5索引有可能匹配的，因为它们都包含jav和ava，（索引中有6和12的不行，因为它们并不同时包含这两个片段）
> 
译者注：原作者的描述的确让人非常难以理解，很显然这种翻译类型文章也不适合我来写大量篇幅的介绍性文字，尽量用3句话说明：Trigram Index就是先将所有tag打个编号，比如0=java，1=xxx， 2=yyy，…，5=javascript, 6=jaxb, …12=javseen, 这样0和5都包含_ja, jav, ava，所以_ja，jav和ava索引中都带有{0， 5}；
接下来，*java*是想要找到所有带有java的标签，*表示非空字符的话，这样的标签必须同时包含jav和ava两个片段，因为分3个连续字母片段时已经在jav和ava的索引中记录下来包含这样标签的索引了，所以直接先找jav的索引，有0，5，12号标签，然后再找到ava索引中有0，5，6号，很显然，只有0号和5号标签可能包含java，6号和12号不可能包含java；这样一下子将搜索范围缩减到了2个，最后还需要做一下简单的模式匹配即可，因为有可能字符串这样的：javbcdeava
## 结论
下面是该算法在我的笔记本电脑上的运算结果，其中Contains方法是上面那个最原始的双重循环算法，Regex方法是希望优化速度，把正则表达式先编译一下（事实上，这样效率更差）


```
Expanded to 7,677 tags (Contains), took 721.51 ms
Expanded to 7,677 tags (Regex), took 1,218.69 ms
Expanded to 7,677 tags (Trigrams), took  54.21 ms
```
正如你所看到的这样，基于Trigram的倒排索引完胜。如果你感兴趣的话，请到Github上查看[源代码](https://github.com/mattwarren/StackOverflowTagServer/blob/master/TagServer/WildcardProcessor.cs)。
本文主要介绍了一种可以被标签引擎用来实现通配符查询的方法，因为我不在Stack Overflow工作，所以他们是不是采用同样的方法，我不得而知，但至少我这个方法效率还是相当不错的。
## 下一步讨论内容
我们取得了阶段性成果，但是想要做好标签引擎，还有很多事情需要考虑：
- [复杂的布尔查询](http://stackoverflow.com/questions/tagged/.net+or+jquery-)，比如带有这样的标签：“c# OR .NET”, “.net AND (NOT jquery)”，不仅要能实现，效率还要很高才行
- [针对标签服务的DDOS攻击](http://stackstatus.net/post/107352821074/outage-postmortem-january-6th-2015)
在十月份，我们收到大量的精心构造的请求，导致我们的标签引擎服务器资源使用率飙升，不过这正好是我们将高效的标签引擎发挥用途的地方。*（译者注：这是引用stack exchange网站中的一段话）*
> 
**打赏支持我翻译更多好文章，谢谢！**
[打赏译者](#rewardbox)
#### 打赏支持我翻译更多好文章，谢谢！
任选一种支付方式
![](http://www.jobbole.com/wp-content/uploads/2016/04/ecc1aaae4d8261fe267c08e14ef6e064.jpg)![](http://www.jobbole.com/wp-content/uploads/2016/04/0dd5adad8022506f9d221188fe54f5bc.jpg)
