# 怎么保存Protege 5.x中HermiT推理机reasoner的推理结果？ - 博客堂 - CSDN博客





2019年03月22日 11:35:57[最小森林](https://me.csdn.net/u012052268)阅读数：62标签：[protege																[reasoner																[hermit																[保存推理结果																[保存](https://so.csdn.net/so/search/s.do?q=保存&t=blog)
个人分类：[知识图谱																[自然语言处理](https://blog.csdn.net/u012052268/article/category/6830884)](https://blog.csdn.net/u012052268/article/category/8694645)





How save the result of reasoner HermiT in Protege

# 怎么保存Protege 5.x中HermiT推理机reasoner的推理结果？

我们在使用Protégé5.5，我想将推理器的结果保存到本体文件中。当我停止推理时，所有结果都消失了。如何保存结果？

## 1现象

当我们使用推理机推理后，protege推理出来的结果会用黄色标出。

但是当我们想要保存推理后的结果时候，点击file中的save project或者save as project，再打开该项目发现，还是原来的结构，并不是推理机推理后的结构数据。

并且点击stop reasoner之后，推理的结果也消失了。

## 2 解决办法

点击 File → Export inferred axioms as ontology…

![](http://ww1.sinaimg.cn/large/006qDjsOgy1g1beyn5g5fj30iw0ezabo.jpg)

选择所有选项：
![](http://ww1.sinaimg.cn/large/006qDjsOgy1g1bf03d2drj30p30jt77a.jpg)
然后选一个 你想保存的本体文件地址。

保存的文件没有后缀。所以我们把保存的文件的后缀 改为.owl 即可。

> 
参考大神：[https://stackoverflow.com/questions/23079459/how-save-the-result-of-reasoner-fact-or-hermit-in-protege](https://stackoverflow.com/questions/23079459/how-save-the-result-of-reasoner-fact-or-hermit-in-protege)](https://so.csdn.net/so/search/s.do?q=保存推理结果&t=blog)](https://so.csdn.net/so/search/s.do?q=hermit&t=blog)](https://so.csdn.net/so/search/s.do?q=reasoner&t=blog)](https://so.csdn.net/so/search/s.do?q=protege&t=blog)




