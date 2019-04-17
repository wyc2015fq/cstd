# 029_AUTOSAR VFB学习笔记-2 - 小灰笔记 - CSDN博客





2018年12月10日 22:36:54[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：57








先简单理解一下下面的这个配置系统的示意图。

![](https://img-blog.csdnimg.cn/20181210223635993.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         从上面的图中能够看懂的部分：
- SW-C之间的通信是通过VFB的，三角形的端口可能是表示信号的流向。
- 圆形或者弧形以及其他的形状含义暂时不是很明确，形状是否着色的含义具体也不是很清楚。
- VFB看起来不是针对一个ECU的，二是针对一个ECU网络的。这样就带来一个疑问：SW-C难道不是绑定到专门的ECU吗？不过，看到图中ECU的内部依然有SW-C，这就更加让人不理解了。那么，ECU外面的SW-C是什么含义呢？
- 图中ECU大致的组成与昨天看得那份文档有点相似。大致还是可以理解的。
- 不同ECU之间通过一定的通信方式相互联系，这个也是好理解的。不过，还是有一个疑问，VFB的配置文件中会含有多个ECU的信息？

![](https://img-blog.csdnimg.cn/20181210223635916.png)

         算是终于知道SW-C是什么含义了，不过关于端口的定义还是有些模糊。交互点？如何算是一个交互点呢？是一个变量还是一个接口函数？

![](https://img-blog.csdnimg.cn/20181210223635985.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         上面是一个带有8个端口的组件，黑块输入三角应该是表述状态输入、圆圈表示设置输入、黑色输出三角代表输出、白色三角是标定、带点的三角是EEPROM。这几个可能是跟软件设计的基础元素可以直接对上号的，剩下几个是一些具体化的功能信息。总体来说，这个描述其实跟软件建模还是很像的，不同的是端口未必是常用的变量类型。

![](https://img-blog.csdnimg.cn/2018121022363698.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         几个之前没有意识到的要点：
- 不能的组件可能使用到相同的代码，但是每个依然会有独立的存储区；
- 组件的端口有可能是在配置阶段可以增删的，不是确定的；

![](https://img-blog.csdnimg.cn/20181210223635989.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         这个可能能够回答之前我的一部分疑问：端口如何发挥作用，以怎样的代码形式呈现？从上面的描述看，应该每个端口还有一个接口形式的属性或实现。

![](https://img-blog.csdnimg.cn/20181210223636550.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         看到这里，似乎又有一个理解产生：端口与接口或许 本来就是一回事儿，端口是一种特定的接口实现。

![](https://img-blog.csdnimg.cn/20181210223635964.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         结合手中其他的资料小结一下：端口要么是PPort要么是RPort，PPort提供一个AUTOSAR的接口，而RPort请求一个AUTOSAR的接口。

![](https://img-blog.csdnimg.cn/2018121022363671.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         没有阴影的应该是简单的数据，而带有阴影的应该是服务。着重强调的是AUTOSAR？





