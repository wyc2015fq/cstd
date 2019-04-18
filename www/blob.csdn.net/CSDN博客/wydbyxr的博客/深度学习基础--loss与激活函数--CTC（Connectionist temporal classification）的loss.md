# 深度学习基础--loss与激活函数--CTC（Connectionist temporal classification）的loss - wydbyxr的博客 - CSDN博客
2018年11月16日 09:52:44[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：87
# CTC（Connectionist temporal classification）的loss
  用在online sequence。由于需要在分类结果中添加一个{no gesture}的类别，如果用在segmented video的分类时，需要去掉这类（因为视频总属于某个类）。
  CTC常用在NLP当中，解决的问题是连续训练的时候不好标数据的问题。比如一段”This is a new world”的语音辨识训练样本，往往不能或者代价太大去辨识出具体哪个字母或者音素在什么位置出现或者终结，CTC提供了一种统计方法，只需要给整体进行标注，并不需要对具体的字母进行标注。
