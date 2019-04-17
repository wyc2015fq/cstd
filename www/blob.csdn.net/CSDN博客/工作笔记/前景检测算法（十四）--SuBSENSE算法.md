# 前景检测算法（十四）--SuBSENSE算法 - 工作笔记 - CSDN博客





2016年07月10日 20:27:39[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：6796
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









﻿﻿

# 原文：

[http://blog.csdn.net/u010598445/article/details/49021385](http://blog.csdn.net/u010598445/article/details/49021385)

# paper
- Self-Balanced SENsitivity SEgmenter
- 2015 St-Charles, Pierre-Luc Bilodeau, Guillaume-Alexandre Bergevin, Robert
- SuBSENSE: a universal change detection method with local adaptive sensitivity.

# code
- [https://bitbucket.org/pierre_luc_st_charles](https://bitbucket.org/pierre_luc_st_charles)

# system
- 系统全景 
![这里写图片描述](https://img-blog.csdn.net/20151010101713587)
- 选择Color-LBSP的原因 
![这里写图片描述](https://img-blog.csdn.net/20151010110520803)
- 选择N的值 
![这里写图片描述](https://img-blog.csdn.net/20151010110649515)
- 系统参数的物理意义 
![这里写图片描述](https://img-blog.csdn.net/20151010110700012)

# 说明
- 基于类似VIBE算法的样本一致性(sample consensus)
- 使用与PAWCS算法类似的更新方式及特征：Color-LBSP
- 在理解本文时可先参考VIBE算法中对样本一致性性质的利用。
- 本文可以说只对VIBE算法中的距离公式进行了改变，其它系统参数都是为了选择最合适的距离阈值。

# 系统参数

![这里写图片描述](https://img-blog.csdn.net/20151010134056732)
![这里写图片描述](https://img-blog.csdn.net/20151010134110495)
![这里写图片描述](https://img-blog.csdn.net/20151010134125409)



