# pcb差分线包地,地线打过孔的原因 - xqhrs232的专栏 - CSDN博客
2017年02月18日 22:48:44[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2857
原文地址::[http://bbs.eetop.cn/thread-162354-1-1.html](http://bbs.eetop.cn/thread-162354-1-1.html)
相关文章
1、大家好,在PADS里面,怎样做差分线的包地处理,多谢!----[https://zhidao.baidu.com/question/279086540.html](https://zhidao.baidu.com/question/279086540.html)
ground-signal-signal-ground（GSSG）：
Although differential signaling generally provides for all return path currents, the GSSG geometry can support common-mode signals with a well-controlled return current path.Any common-mode signals that get
coupled or generated can propagate through the via (rather than being scattered) and along the transmission line to the receiver where it is terminated.
GSSG 一般只在serdes（1Gbps以上）走线中使用，如XFP，XAUI，pci expres。
而一般的差分走线因为速率较低（1Gbps以下），密度大（如SFI4.2总线），不会使用GSSG过孔。
//============================================================================
备注::
1>在两根差分线中间先放上过孔，然后灌铜就可以了.
2>包地处理指在线的两侧++++走线的另外一层铺地铜
