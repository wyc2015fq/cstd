# 【NLP】关键词提取之PositionRank详解 - zkq_1986的博客 - CSDN博客





2018年10月11日 11:53:22[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：181








PositionRank提取关键词与TextRank相似，都是基于PageRank的图关系计算词的得分。用S(V_i)表示词的重要性得分，其公式如下：

![](https://img-blog.csdn.net/20181011114716433)                                                     （1）

其中，![](https://img-blog.csdn.net/20181011115155384?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)表示词的初始得分归一化后的值。

![](https://img-blog.csdn.net/20181011143501674)                            （2）

词的初始得分跟词的在文本中的位置成反比，跟词频成正比。假定词v在文本的第2，第3，第8位置出现，p=1/2+1/3+1/8。

a(alpha)表示阻尼因子，一般设为0.75

w表示图的边的权重，即词之间的相似度。

O(v_j)表示词v_j所有出向边的权重和。











