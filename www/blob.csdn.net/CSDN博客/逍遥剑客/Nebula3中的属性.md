# Nebula3中的属性 - 逍遥剑客 - CSDN博客
2009年01月11日 00:56:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：1610
所属专栏：[Nebula引擎研究](https://blog.csdn.net/column/details/thenebuladevice.html)
Attr命名空间下实现了Mangalore里的动态属性. 属性是编译期间类型安全的key/value对, 这是数据库(database)子系统的基础. 
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090111/attribute.jpg)
如图, AttrId相于属性类型, 而AttributeDefinition是属性类型的定义, 其中包含了属性名, 属性的四字节码, 值类型和访问类型(读/写). 也就是说, AttrId是AttributeDefinition的实例.
实际使用时我们关心的是Attribute类. 
那么使用Attribute有什么好处呢? 我觉得最大的好处是可以把某个对象的属性进行抽象化, 不用每个属性都设置一个成员变量. 对于序列化来说, 有了Attribute我们可以不用关心具体的成员变量而对所有的Attriubte进行统一的处理. 仅仅是增加一个属性成员的话, 是不用对类的代码进行更改的. 我想每个游戏的Character类做需求做到最后, 成员变量的数目变得很吓人吧? 为了保证访问控制还要写上一大堆的get/set方法, 代码行数到最后根本控制不住. 而有了Attribute, 我们只需要知道属性名就可以动态的增加或减少属性了, 这对于脚本化也提供了及大的便利性, 呵呵.
