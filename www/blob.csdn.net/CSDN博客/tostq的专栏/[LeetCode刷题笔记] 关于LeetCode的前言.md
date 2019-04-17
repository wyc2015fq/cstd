# [LeetCode刷题笔记] 关于LeetCode的前言 - tostq的专栏 - CSDN博客





2016年07月22日 21:32:55[tostq](https://me.csdn.net/tostq)阅读数：9780








**原创文章**


转载请注册来源[http://blog.csdn.net/tostq](http://blog.csdn.net/tostq)




       又到了一年毕业就业季了，三年前的校招季我逃避了，可这一次终于还是要轮到我了=_=||。
作为要准备踏入码农行业的人来说，要准备校招，怎么能不去刷刷LeetCode呢？
LeetCode收录了许多互联网公司的算法题目，被称为刷题神器，我虽然早有耳闻，不过却一直没有上面玩过。即使这一年多来，做的编程还是挺多的，不过毕竟不是计算机专业的科班出身，在编程过程中土路子实在太多了，有时不仅写得煎熬，而且书写很多时候都非常不规范。所以就决定在找工作前夕，花点时间好好刷刷LeetCode。
主要的目的：
1、熟悉各互联网公司的算法题目，为找工作做准备。
2、复习以前学过的编程语言，LeetCode支持几乎所有主流编程语言，大家可以用不同语言来做题。
3、熟悉常见的算法和数据结构，LeetCode提供了交流平台，一些大神会将自己的解法贴出来共享，有些巧妙的解法实在令人叫绝，虽然几乎都是英文，但上面的国人也特别多（中文拼音名字>_<）。
4、学习别人的编程思维，加快编程的速度，避免常见的BUG。
LeetCode的题目并不多，目前大概有358道，不过有些题是加锁的，好像有付费才能使用，能做的题应该有150多道吧，这也是完全足够了。
另外LeetCode的题型都非常简单明了，并不需要的复杂的理解，一般都在50行以内就可以解决了，如果你写了上百行代码，就肯定说明你想太多了或太复杂，虽然都能用很短的代码就能解决，但并不意味着LeetCode的题目非常简单，实际上LeetCode基本上涉及到了所有常规的算法类型。
**关于LeetCode的刷题时间：**个人认为大概是要一个月左右，如果你是大神的话，也许大概能在两到三个星期间刷 完，不过做为新手，除了埋头做题，更重要的是去讨论区看看别人的代码或思路。像我的话，基本上每道做完后都会去看看别人的解法，虽然速度慢了点，不过会学到了许多。
**关于本系列博客：**LeetCode我现在虽然还没刷完，但也刷了很多，写这个博客的目的主要是记录总结刷题的过程，防止过段时间又忘了曾经自己所写，整个系列的博客应该会有20~30篇左右，应该会在今年9月份结束。另外博客主要是以专题的形式分类（比如数学类、数组类、链表类）记录题目，并不会单独把一个题拿出讲。
好了，现在就让我们开始LeetCode吧！
首先我们需要在leetcode上注册一个账号，然后登录，开始我们的编程吧！


**一、选择题目类型**
最上面标签栏Problems，给出了三个分类：Algorithms、Database、Shell，分别表示算法题、数据库题、Shell脚本题，第一个就是我们所需要的算法题。


![](https://img-blog.csdn.net/20160722212325278?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





**二、选择算法题**
点开Algorithms后，我们可以看到一列题目的列表，每个题目都有独一无二序号，后面的接受率（Acceptance）表示提交的正确率，Difficulty表示难易程度。
LeetCode按难易程度分成了：Hard、Medium、Easy三个级别。
Easy级别一般并不需要太多思考就可以想到算法，甚至可以通过直接的方式，特别适合新手去熟悉编程语言。
Medium级别就会有些难度，一般都会涉及到经典的算法，需要一定的思考。
Hard级别是最难的，有些时候是算法本身的难度，有些时候特别需要你考虑到各种细节。
每个题目前面的小箭头表示该题已经完成。题目列表最上方有一个Choose one filter，可以将已完成的题目从列表中去掉。

![](https://img-blog.csdn.net/20160722212419322?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



**三、筛选某一类型的题**
如果我们只想要找某一类型的题，可以通过Tags或Company来筛选
在题库的右边栏可以看到如下，这里列出许多公司的题目

![](https://img-blog.csdn.net/20160722212510885?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
另外，如果我们只想做关于字符串、数组或链表相关题，可以通过Tags

![](https://img-blog.csdn.net/20160722212530483?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
如果我们在做某一题时，觉得还想再做一个类似的，巩固一下，可以通过该题下面的Show Similar Problems和Tags来找到相似的问题

![](https://img-blog.csdn.net/20160722212546369?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**四、如何和别人讨论**
每个题目都有各自的Discuss按钮，点击进入后，就能看到了讨论区。

![](https://img-blog.csdn.net/20160722212614916?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
在这里，许多人都把自己的代码放到了上面，就像BBS一样，你可以发贴提问，也可以回复别人。

![](https://img-blog.csdn.net/20160722212632213?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**五、关于代码编写、测试与提交**
点开我们选择的题目后，就可以进行代码编写了，LeetCode一般都会直接提供一个函数式接口，我们只需要编写函数内部就可以了，而需要考虑到库文件，另外，在上面选择栏中，可以切换选择自己需要的编程语言。

![](https://img-blog.csdn.net/20160722212655703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

程序编写完了之后，不要急着提交（Submit Solution 按钮），先可以测试运行下（Run Code）

![](https://img-blog.csdn.net/20160722212742292?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们还可以点开Custom TestCase旁边的小框，点开后，可以在里面输入我们自己设定的输入值。
一般情况，数组的输入形式是[a1,a2,a3,a4……]
当然我们测试完整无误后，再选择提交Submit Solution。
如果出现错误，会有提示。
如果正确无误，会有如下提示：


![](https://img-blog.csdn.net/20160722212854636?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们可以点开More Details查看详细结果说明
或者点开Next challenges 旁边的题继续做题。

**六、查看自己提交的题目**

![](https://img-blog.csdn.net/20160722212926972?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
在最上面标签栏找到自己，选择：
**My Submissions：**可以找到自己提交的题目（包括了正确提交和错误提交）提交的代码也是都是可以看到的
**Manage Sessions：**主要是管理自己的提交情况，错误率和正确率，总完成率之类。

![](https://img-blog.csdn.net/20160722213003308?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
每道题旁边的My Submissions可以找到自己的对于该题的提交情况

![](https://img-blog.csdn.net/20160722213025574?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
点开后，就可以找到自己过去所有的提交，
点Accepted 或 Wrong Answer就可以查看自己过去提交的代码情况，当然还有得分。

![](https://img-blog.csdn.net/20160722213059715?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





