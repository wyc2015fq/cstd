# X3DAudio中声道音量跳变的问题 - 逍遥剑客 - CSDN博客
2010年01月09日 01:08:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2297
对于3D音效, 左右耳的音量是不一样的. 当然, 眼下没有条件试验传说中的6.1声道是啥子效果, 所以不考虑这种情况. 正是因为左右有音量的差别, 才能根据耳朵去定位音源的方位. 
如果是只有两个声道, 比如耳机, 那么两只耳朵的音量变化有时候就很突兀. 为什么呢? 
以现实来说, 人的两耳所能感受的声音方向是不同的, 正好左右对称. 如下图. 横线上两耳的音量差别是最大的, 纵线上两耳的音量是相等的. 如果音源正好位于横线上, 那么在一定范围外, 有一只耳朵是听不到声音的, 这是由于声音的方向性和耳朵可接收声音角度共同决定的. 一旦两耳和音源的相对位置沿横线发生变化, 就会出现一只耳朵突然有声音或是突然没声音的奇怪现象. 
![](http://images.cnblogs.com/cnblogs_com/xoyojank/ear-sound.jpg)
为了解决这个问题, X3DAudio的emitter提供了两个属性, **InnerRadius** 和 **InnerRadiusAngle**. InnerRadius是为了在这个半径内, 声音可以平滑地进行过渡, 而InnerRadiusAngle则是为了让声音可以"扩散"到周围的扬声器上去. 简单地说, 就是牺牲3D空间感换取声音的渐变效果. 实际使用需要试验才能得出好的效果. (我这里暂时定了5米的半径和90度的角度.)
![](http://images.cnblogs.com/cnblogs_com/xoyojank/x3daudio_emitter_inner_radius.png)
