# 聊聊 StackOverflow 的标签引擎（上） - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [赵荣](http://www.jobbole.com/members/q3612171732) 翻译，[黄利民](http://www.jobbole.com/members/huanglimin) 校稿。未经许可，禁止转载！
英文出处：[Matt Warren](http://mattwarren.github.io/2014/11/01/the-stack-overflow-tag-engine-part-1/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
我已经将一些[资源](http://mattwarren.github.io/resources/)和之前发表的[演讲](http://mattwarren.github.io/speaking/)的页面加到我的网站中了，如果你想要了解更多细节，可以看一下。
## Stack Overflow标签引擎
我第一次听说Stack Overflow的[标签引擎之殇](http://samsaffron.com/archive/2011/10/28/in-managed-code-we-trust-our-recent-battles-with-the-net-garbage-collector)是在我读到[他们和.NET垃圾收集器搏斗的故事](http://blog.marcgravell.com/2011/10/assault-by-gc.html)的时候。如果你从来没听过的话，我建议你先读一下前面链接中的文章，然后再看看这篇有意思的的技术债务案例分析。
但是只要你上过[Stack Overflow](http://www.stackoverflow.com/)，你一定用过这个功能，而且用的时候可能都没意识到它的存在。它的能力主要通过 stackoverflow.com/questions/tagged 这样的 url 体现出来的【译者注：比如要找带java标签的问题，url就是http://stackoverflow.com/questions/tagged/java】，当你找到所有带[.NET](http://stackoverflow.com/questions/tagged/.net)、[C#](http://stackoverflow.com/questions/tagged/c#)或者[Java](http://stackoverflow.com/questions/tagged/java)标签的问题时，你可以看到下面这样的页面（注意右边栏中，相关标签已经被选中了）。
![](http://ww4.sinaimg.cn/large/7cc829d3gw1f36tq6n0pjj20qa0cgwkf.jpg)
## 标签API
和简单的搜索一样，你可以很轻松的通过一些复杂的查询语句来找到你想要的结果（有些复杂查询你需要你先登录stackoverflow），比如你可以搜索：
- [带.NET标签但是不带jQuery标签的问题](http://stackoverflow.com/questions/tagged/.net+or+jquery-)
- [最受欢迎的C#问题（根据投票数）](http://stackoverflow.com/questions/tagged/c%23?order=desc&sort=votes)
- [前10页被提问频率最高的XML相关问题](http://stackoverflow.com/questions/tagged/xml?sort=frequent&page=10&pagesize=5)
- [最早的.NET问题](http://stackoverflow.com/questions/tagged/.net?page=197709&sort=newest&pagesize=1)
像上面这样的搜索一般和你个人偏好没有任何关系。但是如果你希望默认屏蔽带某些标签的问题时，可以在登录后到你的账户管理页面，点击 Preferences 菜单，就可以在页面底部看到忽略标签列表。有时候，站里的一些牛人设置了100多个忽略标签，像这种情况，要处理这些和个人设置相关的搜索时，要解决的问题就不是那么简单了。【译者注：现在stackoverflow登录后，点击头像进入账户管理页面，然后有个 Settings 按钮（找不到可以Ctrl+F一下），然后就可以看到Preference了】
## 获取公开的提问数据
正像我说的那样，如果要建立一套标签引擎，我想应该先看一下它会处理处理哪些数据【译者注：额，原作者之前真的没有说过，他给出的链接里也没有说过，大家看看就好】。幸运的是，我们可以从[这里](https://archive.org/details/stackexchange)下载到所有的Stack Exchange站点的数据【译者注：stackoverflow和stackexchange关系参考[这里](http://hao.jobbole.com/stack-exchange-network/)】。为了简化问题，只管第一次发布的内容（不包括编辑历史），我下载了[stackoverflow.com-Posts.7z](https://archive.org/download/stackexchange/stackoverflow.com-Posts.7z)（高能预警，这个文件有5.7GB），这个压缩包中最新数据是到2014年9月份的。简单描述一下这份数据长什么样子的吧，一个典型的问题长得就像下面这个xml文件的样子。对于标签引擎，我们只需要关系红色框框住的部分即可，因为它只需要提供一个索引，所以它只需要关注meta-data，不需要关心问题的具体内容是什么。
![](http://ww2.sinaimg.cn/large/7cc829d3gw1f36tq7nx59j20qi0iqthh.jpg)
下面是对下载的数据做了一些简单的的统计分析，可以看到，有超过7900万条问题，如果要把所有数据读取到List中，要消耗足足2GB的内存。


```
Took 00:00:31.623 to DE-serialise 7,990,787 Stack Overflow Questions,used 2136.50 MB
Took 00:01:14.229 (74,229 ms) to group all the tags, used 2799.32 MB
Took 00:00:34.148 (34,148 ms) to create all the "related" tags info, used 362.57 MB
Took 00:01:31.662 (91,662 ms) to sort the 191,025 arrays
After SETUP - Using 4536.21 MB of memory in total
```
一共花了31秒从磁盘上的文件反序列化（通过这个第三方库[protobuf-net](https://code.google.com/p/protobuf-net/)），然后又花了大约3.5分钟进行了处理和排序，最后我们大概用了4.5GB的内存空间，下面是一些统计信息：【译者注：原作者没提供预处理的源代码，这个效率相当给力啊】。


```
Max LastActivityDate 14/09/2014 03:07:29
Min LastActivityDate 18/08/2008 03:34:29
Max CreationDate 14/09/2014 03:06:45
Min CreationDate 31/07/2008 21:42:52
Max Score 8596 (Id 11227809)
Min Score -147
Max ViewCount 1917888 (Id 184618)
Min ViewCount 1
Max AnswerCount 518 (Id 184618)
Min AnswerCount 0
```
结果真的很让人吃惊，stackoverflow上被查看最多的一个[问题](http://stackoverflow.com/questions/184618/what-is-the-best-comment-in-source-code-you-have-ever-encountered)点击次数居然超过了190万次【译者注：这个问题现在点击次数已经达到234万次以上了】，这个问题已经被管理员锁定了，尽管该问题实际上没什么建设性，【译者注：因为stackoverflow的管理员认为这个问题具有历史意义，但是他们认为这个问题基本上没什么意义，所以也提醒不要提类似的问题】，该问题同样也是回答次数最多的问题，有518个回答。接下来，最让人惊叹的可能是获赞次数最多的问题，达到了惊人的8192次，该问题是“[为何处理排序的数组比未排序的要快？](http://stackoverflow.com/questions/11227809/why-is-processing-a-sorted-array-faster-than-an-unsorted-array)”【译者注：这个问题现在获赞次数超过了14000次，最高票回答接近2万次zan了】。
## 创建索引
所谓索引长什么样子？简单说来，就是一堆经过排序的列表(List)，列表中记录的是问题主表（List)中的偏移量，问题主表是持有所有Quesiton对象的。用下图可以说明索引：
![](http://ww4.sinaimg.cn/large/7cc829d3gw1f36tq9o3q5j20t307a77t.jpg)
说明：这种组织方式非常类似于[Lucene](http://lucene.apache.org/)的索引结构【译者注：扫盲一下，这就是咱们大Java世界的Luncene】 从下面的代码看，创建索引这件事情也不是多复杂的事情：


```
// start with a copy of the main array, with Id's in order, { 0, 1, 2, 3, 4, 5, ..... }
tagsByLastActivityDate = new Dictionary&amp;amp;amp;lt;string, int[]&amp;amp;amp;gt;(groupedTags.Count);var byLastActivityDate = tag.Value.Positions.ToArray();
Array.Sort(byLastActivityDate, comparer.LastActivityDate);
```
同时，比较器的代码也很简单（注意一下，这里是对byLastActivityDate数组进行排序，不过是使用的quesiton数组中的数据来决定该数组中元素的位置的）


```
public int LastActivityDate(int x, int y){
    if (questions[y].LastActivityDate == questions[x].LastActivityDate)
    return CompareId(x, y);
   // Compare LastActivityDate DESCENDING, i.e. most recent is first
    return questions[y].LastActivityDate.CompareTo(questions[x].LastActivityDate);}
```
好了，一旦我们建立好了这些排序后的列表，如上图表示的那样（编辑时间从近到远，分数从低到高），我们可以很容易的从这两个数组中获取他们在问题主表Quesitons列表中的数据。举个例子，遍历Score数组（1，2，…，7，8），获取所有的Id，可以得到这样的数组{8，4，3，5，6，1，2，7}，数组内容就是他们在主表Questions中的位置。下面的代码就是表述如何怎么查询的，同时考虑到了翻页的情况（pageSize和skip）。


```
var result = queryInfo[tag]
    .Skip(skip)
    .Take(pageSize)
    .Select(i =&amp;amp;amp;gt; questions[i])
    .ToList();
```
上面的工作搞定后，我就可以提供API，你可以通过浏览器来查询了。注意下图中的时间是在服务器处理的时间，事实上这个数字也是靠谱的，查询单个标签的速度非常非常快。
![](http://ww2.sinaimg.cn/large/7cc829d3gw1f36tqajgf8j20gy0gowhm.jpg)
## 下篇内容预告
现在，基础索引已经建立起来了，下次我们来看看下面的问题怎么处理：
- 复杂的布尔型查询 .net or jquery- and c#
- 那些选了100多个排除标签的情况
还有一些其他我想到的问题。
> 
**打赏支持我翻译更多好文章，谢谢！**
[打赏译者](#rewardbox)
#### 打赏支持我翻译更多好文章，谢谢！
任选一种支付方式
![](http://www.jobbole.com/wp-content/uploads/2016/04/ecc1aaae4d8261fe267c08e14ef6e064.jpg)![](http://www.jobbole.com/wp-content/uploads/2016/04/0dd5adad8022506f9d221188fe54f5bc.jpg)
