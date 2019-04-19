# XACT与X3DAudio整合的问题 - 逍遥剑客 - CSDN博客
2008年11月29日 14:44:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：1594
XACT本身是不带3D音效的, 相关的功能是由X3DAudio来完成. 
刚刚封装完毕的时候, 没有注意到哪里有调节距离衰减的参数
参考了一下SDK的文档, 发现X3DAUDIO_EMITTER的pVolumeCurve, CurveDistanceScaler这两个参数是用来做衰减用的.
不过SDK的文档写错了(March2008, 最新的没问题), 搞得我郁闷了半天.
其实pVolumeCurve为NULL时CurveDistanceScaler仍然有效, 只不过是你需要在XACT的工具里进行设置:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081129/XACT_DistanceAttenuation.JPG)
当然也可以创建更加复杂的效果, 不过一般用不上. 下面这个是引擎声音音调随速度变化的曲线^_^
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081129/XACT_RPC.JPG)
实际用的时候发现3D音量衰减在刚开始播放时是没有起作用的, 而这个时间差正好近似等于IXACT3Engine的更新(DoWork)间隔. SDK推荐的更新间隔是30~100ms, 没想到这到引起问题了. 因为我的Cue的XACT3DApply都是跟DoWork一起调用的, 所以没有及时更新变换矩阵的信息, 导致下一次整个AudioEngine进行Update的时候才计算出正确的3D音效. 
解决办法: Cue Play开始后立即Update(XACT3DCalculate, XACT3DApply)一下
