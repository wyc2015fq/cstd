
# 多模态(RGB-D)——MMSS - 小花生的博客 - CSDN博客


2018年10月04日 17:30:07[Peanut_范](https://me.csdn.net/u013841196)阅读数：175


《MMSS: Multi-modal Sharable and Specific Feature Learning for RGB-D Object Recognition》
ICCV2015, Anran Wang, Jianfei Cai et al. MMSS
1. 摘要：
对于RGB-D目标识别，更多采用的方法是：
（1）RGB和D单独地学习特征
（2）简单的把RGB-D无差别的作为4通道的数据
（3）最后一层FC进行融合（concat或Eltwise）
都不能充分地利用不同模态间的关系
![在这里插入图片描述](https://img-blog.csdn.net/20181004172050874?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
依据不同模态即含有共同的模式，也含有特定的模式的动机，作者提出了多模态的特征学习框架。
[
](https://img-blog.csdn.net/20181004172050874?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)2. Multimodal Learning Structure
![在这里插入图片描述](https://img-blog.csdn.net/20181004172152649?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
我们的目的是去学习一个新的特征表示T，包含两部分特性：
1）两个模态的共同特性
2）单独模态的特有特性
让T1和T2表示RGB和D单独模态学习到的特征，我们迫使T1和T2共享一部分共同的部分Tc，和特有模态的部分T1s和T2s。
![在这里插入图片描述](https://img-blog.csdn.net/20181004172301816?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![在这里插入图片描述](https://img-blog.csdn.net/20181004172336912?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
训练流程：
![在这里插入图片描述](https://img-blog.csdn.net/20181004172427959?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
测试阶段：
![在这里插入图片描述](https://img-blog.csdn.net/20181004172516189?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
小结：
分析并学习共有的特征和独有的特征是一个比较新的话题
---
注：博众家之所长，集群英之荟萃。

[
](https://img-blog.csdn.net/20181004172516189?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
