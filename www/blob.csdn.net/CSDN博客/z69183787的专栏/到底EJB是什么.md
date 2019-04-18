# 到底EJB是什么 - z69183787的专栏 - CSDN博客
2014年09月12日 09:54:58[OkidoGreen](https://me.csdn.net/z69183787)阅读数：663
个人分类：[Ejb](https://blog.csdn.net/z69183787/article/category/2186235)

到底EJB是什么？被口口相传的神神秘秘的，百度一番，总觉得没有讲清楚的，仍觉得一头雾水。百度了很久，也从网络的文章的只言片语中，渐渐有了头绪。
　　用通俗话说，EJB就是："把你编写的软件中那些需要执行制定的任务的类，不放到客户端软件上了，而是给他打成包放到一个服务器上了"。是的，没错！EJB 就是将那些"类"放到一个服务器上，用C/S 形式的软件客户端对服务器上的"类"进行调用。快崩溃了吧！这么几句就非常清楚的解释了EJB是什么（也是本人在高人的博文里看来的），而很多文章总是执着于解释EJB原理，EJB出现的意义。
　　再次将这篇费了很大力气找的文章地址贴出来，博文对EJB的原理和实质的解释非常清晰：[http://blog.csdn.net/jojo52013145/article/details/5783677](http://blog.csdn.net/jojo52013145/article/details/5783677)
　　另外，EJB的出现引起很大的争议，这里一片引用一篇，04年的，有点老了，但其对自己的看法做了详细的阐述：[http://cache.baidu.com/c?m=9f65cb4a8c8507ed4fece763105392230e54f7227796945468d4e419ce3b46111727feed7c63525483846b6776fc081efdf142226a5060e1c09e9f4babba993f2f8c7c62671df04019d41dabdf037881769f4d98ab0e93bbe74493b9a2d4c855239d0b55&p=9972c35d85cc45fe0ebd9b780864&user=baidu&fm=sc&query=%C6%F3%D2%B5EJB&qid=92abc8ea09204a86&p1=10](http://cache.baidu.com/c?m=9f65cb4a8c8507ed4fece763105392230e54f7227796945468d4e419ce3b46111727feed7c63525483846b6776fc081efdf142226a5060e1c09e9f4babba993f2f8c7c62671df04019d41dabdf037881769f4d98ab0e93bbe74493b9a2d4c855239d0b55&p=9972c35d85cc45fe0ebd9b780864&user=baidu&fm=sc&query=%C6%F3%D2%B5EJB&qid=92abc8ea09204a86&p1=10)
![](http://pic002.cnblogs.com/images/2012/433489/2012083120195629.png)
　　EJB在架构中所处的位置如上图，EJB在J2EE开发中的详细：
![](http://pic002.cnblogs.com/images/2012/433489/2012083120242021.png)
——朋友，转载请注明出自这里。 PS：本随笔属个人学习小结，文中内容有参考互联网上的相关文章。如果您博文的链接被我引用，我承诺不会参杂经济利益。其中如果发现文中有不正确的认知或遗漏的地方请评论告知，谢谢！
