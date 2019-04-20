# spark streaming性能分析经验 - Spark高级玩法 - CSDN博客
2018年12月08日 00:29:12[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：146

晚上健身回来，在和一个同学讨论高中和初中的数学题，结果一个都没做出来，很残酷。结果文章晚了会儿。
最近裁员闹的人心惶惶，听说有朋友整个部门被干掉，这种就跟自己关系不是太大，而有些只裁掉个别人，这时候就跟自己平时的表现有关了，要进行反思，能力达标否，得罪老板否……
做好自己能控制的事情，自己不能控制的事情减少些损失，这对我们来说就够了。
坚持每天学习是不会有坏处的。
最近星球里问spark streaming性能分析的人比较多，浪尖整理了一下很早以前星球里分享的内容。
在这里再给大家分享一下。
性能分析的概览：
1，队列是否积累。
2，executor是否倾斜。
3，task数据是否倾斜。
4，gc是否严重。
.....
这些貌似公众号都有文章输出了。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWSw07AP7oNYVCmbZyEpcb1W90TVL6wzFuO7ZPczBneQY1KyRf4MrAZ3fltaVFK3mKZgTmg37sbwg/640?wx_fmt=jpeg)性能分析
有兴趣加微信群的，可以加浪尖微信，拉你入群。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWSw07AP7oNYVCmbZyEpcb1XFSSicYJdriakvQZx3cvFpn7keQn86UKiahwJNNTVp2NzX4GUibJKHynIQ/640?wx_fmt=jpeg)浪尖微信
有兴趣关注浪尖头条号的，可以扫二维码
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWSw07AP7oNYVCmbZyEpcb12EiaUkj9wxQpEicZZ7ffhI24PRjMxtuRriaxlahIB9rSl4bZx5x7G3RAA/640?wx_fmt=jpeg)头条号
