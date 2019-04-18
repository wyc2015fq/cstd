# 【Python】提取列表内大于阈值提取位置 - YZXnuaa的博客 - CSDN博客
2018年03月30日 14:48:58[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：4016
probs = np.array([[0, 0, 0, 0, 0, 1, 2], [1, 2, 2, 0, 0, 1, 2]])
a = np.array(probs >= 1, dtype='bool')
c = np.nonzero(a)
b = probs[a]
print(probs, '\n',a, '\n', c, '\n', b,)
结果
[[0 0 0 0 0 1 2]
 [1 2 2 0 0 1 2]]
[[False False False False False  True  True]
 [ True  True  True False False  True  True]]
(array([0, 0, 1, 1, 1, 1, 1]), array([5, 6, 0, 1, 2, 5, 6]))
[1 2 1 2 2 1 2]
