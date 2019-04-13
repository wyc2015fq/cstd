
# 汇编：用e命令为啥改不了内存的值？ - 迂者-贺利坚的专栏 - CSDN博客

2017年03月10日 16:01:44[迂者-贺利坚](https://me.csdn.net/sxhelijian)阅读数：1374


题记：一次为学生的答疑，美妙的过程。
【学生】
老师 在不
【学生】
![这里写图片描述](https://img-blog.csdn.net/20170310155410506?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
【学生】
![这里写图片描述](https://img-blog.csdn.net/20170310155525558?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170310155525558?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
咋总是这样呢？不明白哪里错了，为啥用e改不了
[
](https://img-blog.csdn.net/20170310155525558?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)【学生】
好气哦
【迂者】
你的电脑得砸了，不能用
【学生】
。。。。。。很悲剧
【迂者】
是改了，但又变了
【迂者】
你想想，想不出来我再告你
【迂者】
你改的是一块特殊的区域
【学生】
咦 我想想
【学生】
昨天倪畅说是改的ROM区域  那里写不进去
【迂者】
那里是ROM
这儿不是ROM
但也不一般
【学生】
![这里写图片描述](https://img-blog.csdn.net/20170310155634743?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170310155634743?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
题意中是让往这里写啊。。
[
](https://img-blog.csdn.net/20170310155634743?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)【迂者】
你按题目要求做的时候屏幕上观察到了什么？
【学生】
得出的数据与我所期望的不同
【迂者】
是写入了
【学生】
那为啥会再次变化
奇怪
【迂者】
输入01 01 02 02 03 03 04 04后，不要用D命令看，就看屏幕上
【迂者】
你喊我“老师”的时侯，我45.2786岁，你再看了我一眼，我已经45.2792岁，我变了
【学生】
我试一下
![这里写图片描述](https://img-blog.csdn.net/20170310155732322?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
【学生】
咦
我知道了  发生变化了
![这里写图片描述](https://img-blog.csdn.net/20170310155835839?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170310155835839?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
【学生】
![这里写图片描述](https://img-blog.csdn.net/20170310155924044?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
【迂者】
这个弟子可教也！
把这个题目后的提示看一看，看看相关资料，你就清楚了。
【学生】
![这里写图片描述](https://img-blog.csdn.net/20170310160015716?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170310160015716?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
上面这个图标为啥会变  是因为输入的内容不一样嘛
[
](https://img-blog.csdn.net/20170310160015716?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)【学生】
嗯嗯  看来是这样了 懂了 我再去看看
【迂者】
看明白了，你把结论在这儿写写，我们的对话完美收场。
【学生】
这个题 你写入后 通过-d命令查看，不会得到自己想要的结果，他会时刻发生变化，不同的内容，会显示出不同气泡
【迂者】
因为你改的是显示存储区，改了，所以出现了彩色字符，但运行其他命令时，屏幕上的内容变了，显存区当然也就变了。
【学生】
受教了
【迂者】
很高兴带着弟子又经历了个探究的过程
【学生】
哈哈  突然发现就是一道小题中 也存着好多东西  小瞧了这些题
【迂者】
可见死记硬背式的学习，失去了多少乐趣，失去了多少获得能力的机会。
【学生】
嗯呢  确实
仅仅是一道小题，微不足道，对照课本，教程似乎三两下就做完了，殊不知里面含有了更多东西
【迂者】
为你不放过问题点赞！下周一，我想带大家突破一下，这种研讨，我们能在群中一起做就好了。我知道，在习惯中，同学们有些放不开，在思维模式中有些怪物在拦着你们。
【学生】
嗯

