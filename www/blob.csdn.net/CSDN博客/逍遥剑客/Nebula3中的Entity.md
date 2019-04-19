# Nebula3中的Entity - 逍遥剑客 - CSDN博客
2011年01月26日 22:46:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：3149
所属专栏：[Nebula引擎研究](https://blog.csdn.net/column/details/thenebuladevice.html)
N3架构上是分了三层的, foundation, render, application.
一直以来看的都是foundation和render, 上层的一直没怎么看
今天大体上浏览了下, 感觉东西还是满多的
很多引擎都没这一层的, 如果没有实际项目的积累, 也没法抽象出复用的部分来
这里说的Entity不是GraphicsEntity, 而是Game::Entity, 代表游戏中的一个对象, 比如人, 道具什么的
一个Entity由以下几部分组成:
- ID : 这个没啥好说的
- AttributeTable: 属性表, 跟数据库绑定, 数据驱动的前提
- Properties: 或许叫Component更合适, 组件模型, 现在大家都这么干了, 除了国内的人-_-
跟其他组件模型有点不一样的是, 如果想调用Entity中某个组件的功能, 直接发个消息给这个Entity就可以. 它自己找到处理这个消息的Property进行处理. 从这一点来说, Entity也是一个消息分配器(Dispatcher):
                                    +------+
                                +-->| Port |
                               /    +------+
                +------------+/     +------+
    --- Msg --->| Dispatcher |----->| Port |
                +------------+/     +------+
                               /    +------+
                                +-->| Port |
                                    +------+
实际上Property是从Messaging::Port继承的, 然后Port再把消息交给Handler去处理. 这样一来, 扩展功能只需要增加Message和对应的Handler就可以了.
想想N3中其实很多地方都用了消息, 比如多线程. 虽然这种方式解决了同步之类的烦恼, 但是也造就了Message满天飞的现象, 以至于他们写了个idlc来自动生成Message头文件...
在看WPF线程模型的时候, 突然想到, 是不是可以参考它的跨线程调用方法(Dispatcher.**BeginInvoke**)呢? 其实就是把调用函数变成一个delegate, 再加上参数, 一起装在一起加入队列. 本质上还是消息的形式, 只不过不用定义消息结构和写处理分支了. 坏处就是, 把调用细节暴露出来, 耦合度太大......
嗯, 跑题了
那个Property派生出来GraphicsProperty, TransformableProperty, CameraProperty, InputProperty, StateProperty等等. 想要显示在场景里就用GraphicsProperty, 想响应输入就加一个InputProperty, 诸如此类. 要调用其它Property的方法也不用从Entity中找到再cast什么的, 直接一个消息扔进去就完了...
另外属性表的应用和实现都比较复杂, 下回分解
