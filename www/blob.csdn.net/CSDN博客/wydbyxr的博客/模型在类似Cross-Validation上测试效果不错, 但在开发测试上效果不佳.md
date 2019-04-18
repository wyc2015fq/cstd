# 模型在类似Cross-Validation上测试效果不错, 但在开发测试上效果不佳? - wydbyxr的博客 - CSDN博客
2018年11月30日 09:41:42[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：32
# 模型在类似Cross-Validation上测试效果不错, 但在开发测试上效果不佳?
- 选取的训练数据的覆盖度不够, 即数据集不具有代表性(不过完备), 不能体现真实数据的分布。
- 模型迁移 (model drift), 随着时间的转移, 特征数据也会跟着变化。比如3个月前的模型对现在的特征可能不会有好的结果。
