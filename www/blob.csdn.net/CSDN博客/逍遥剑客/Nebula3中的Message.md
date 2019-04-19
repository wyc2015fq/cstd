# Nebula3中的Message - 逍遥剑客 - CSDN博客
2009年02月15日 20:15:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：1918标签：[引擎																[command																[application																[设计模式																[脚本																[多线程](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)](https://so.csdn.net/so/search/s.do?q=设计模式&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=command&t=blog)](https://so.csdn.net/so/search/s.do?q=引擎&t=blog)
个人分类：[引擎研究](https://blog.csdn.net/xoyojank/article/category/337622)
所属专栏：[Nebula引擎研究](https://blog.csdn.net/column/details/thenebuladevice.html)
最近心情不好, 一直没学习-_-
今天是近期最后一个双休, 玩够了就看看代码吧
本来是计划看模型部分的, 无奈现阶段N3中的模型还是用的N2的, 作者又说N3的新版本近期内会出来, 所以先放一放, 新的版本很可能包含N3的模型/动画部分, 到时再看也不迟
Message嘛, 我觉得是整个N3中最为核心的部分, 虽然没几个类, 但可以是说整个N3的精华所在. 为什么这么说呢? 看看它有多少多少类从Port派生的(这图不全):
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090215/N3_Messaging_Port.JPG)
可以说, 每个部分都与它息息相关.
那, Port是什么? 看下图:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090215/N3_Messaging_Dispatcher.JPG)
Message比较好理解, 它就是一个事件, 或是一个操作. 其实就是设计模式中的Command模型, 但N3中的Command成了脚本库和数据库的指令了, 所以只好改叫Message(xoyojank: 我瞎猜的). 
 有Message必须有Handler来进行处理, 那Port就好说了, 它就是Message的接收器, 包含了一堆的Handler来处理消息. 有一个异步的Port实现叫AsyncPort, 它是所有Fat线程子系统接口(XXXInterface)的基类, 有自己的线程而已. 功能上来说, 跟Port一样的.
还有个Dispatcher. 其实它也是一个Port, 但是包含了许多个别的Port. 存在的意义在于把复杂的消息处理机制隐藏掉, 给外界留一个简单的外表. 这个在逻辑部分(即N3的Application层)用的比较多, 能把逻辑部分抽像成这样的游戏我还真没怎么见过. (题外话: 我觉得这也是N3跟OGRE之流差别最大的一个地方: 它是一个游戏引擎, 不仅仅是图形引擎)
还有一点值得注意, 那就是Message预留了二进制序列化的接口, 这意味着可以把消息写入流进行转输. Sep2008版本中加入的Remote子系统就是它的应用, 可以远程操控应用程序, 很酷的一个特性. 
其实, 我感觉N3中所有的模块都是基于一个思想, 那就是任何的操作, 都有一些相关的消息, 和对应的处理者来执行. 可能有人会觉得这样不如直接编码一个函数调用来得简单, 但是扩展性和可复用性大大地增强了. 无论对于脚本化还是多线程化来说, 这都是非常方便的. 
期待N3未来的表现, 因为它不输于任何一个商业引擎! 商业引擎强大在配套的工具上, 但是架构上跟N3相比, 还是有些差距的(至少UE3和GB个人认为不如它) 
