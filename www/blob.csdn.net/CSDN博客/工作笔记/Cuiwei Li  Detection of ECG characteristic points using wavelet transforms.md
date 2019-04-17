# Cuiwei Li / Detection of ECG characteristic points using wavelet transforms - 工作笔记 - CSDN博客





2012年01月18日 14:53:04[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5402
个人分类：[信号处理](https://blog.csdn.net/App_12062011/article/category/1063270)








|Detection of ECG characteristic points using wavelet transformsCuiwei Li;   Chongxun Zheng;   Changfeng Tai;   Biomed. Eng. Inst., Xi'an Jiaotong Univ., China 二进样条小波变换方法 检测QRS，P,T 波峰值点和 起终点。根据波形功率谱特性 选择检测的特征尺度。QRS标记点用四个特征尺度，QRS起终点因为含有高频成分选择小尺度，一般P,T 波有低频特性选择较大的尺度进行检测，但因为基线漂移的原因又不能过大。多尺度检测QRS可以抑制高频噪声、P,T 波、基线漂移，去除孤立的模极大值点可以抑制伪迹。估计奇点的正则性，计算李普希兹指数。---------奇点在相邻尺度间小波变换幅值的变化。|
|----|




