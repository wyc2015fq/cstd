# 022_Excel空白值批量填充默认值 - 小灰笔记 - CSDN博客





2018年12月02日 19:33:07[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：400








         今天为了刷存在感学了一期Excel的技巧教程，学到了一个我觉得可能会比较有用的技巧：把空值批量填充上默认值。

         假设有如下Excel数据：

![](https://img-blog.csdnimg.cn/20181202193245969.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         如果我想把默认空白区填充成0，如果在我不知道这个技巧之前，我可能会手动填充或者填充几个之后鼠标下拉。或者全都改成0之后，再改回来等。如果数量多，我会直接考虑写一个Python的脚本。

         现在看来，有一个很简单的技巧。如上图，在选中的状态下按F5：

![](https://img-blog.csdnimg.cn/20181202193246118.png)

         出来上述界面后，选择定位条件：

![](https://img-blog.csdnimg.cn/20181202193246311.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

选择空值之后，确定，呈现空值选中状态如下：

![](https://img-blog.csdnimg.cn/20181202193246125.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         在下面的位置输入0。

![](https://img-blog.csdnimg.cn/20181202193246245.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         之后，CTRL+ENTER。接着就是魔法呈现的时刻：

![](https://img-blog.csdnimg.cn/20181202193246140.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         这个小技巧，很赞！



