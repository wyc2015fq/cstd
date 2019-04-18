# 015_Arduino上实现一个简单的番茄时钟 - 小灰笔记 - CSDN博客





2018年11月18日 00:26:29[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：213








         最近的emacs番茄时钟一直报错，有时候不稳定有时候则崩溃，还有时候声音播放失败。我觉得在Windows上使用emacs有时候真是得有一份耐得住折腾的心。

         番茄时钟使用多了，突然间没了不习惯。本来打算买个实体闹钟，后来丑了一眼桌子上冷落的Arduino，心想还是简单做个LED灯显示版本出来吧！

         其实，这就是一个计数器，实现起来很简单！代码如下：

![](https://img-blog.csdnimg.cn/20181118002609779.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         计时20min以内，led慢闪，然后5min的led快闪，最后led常量。第二次开启，按一下复位键也就好了！



