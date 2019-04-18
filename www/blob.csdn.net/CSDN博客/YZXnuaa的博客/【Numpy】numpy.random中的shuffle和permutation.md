# 【Numpy】numpy.random中的shuffle和permutation - YZXnuaa的博客 - CSDN博客
2018年03月13日 14:07:23[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：223

numpy.random.shuffle(x) and numpy.random.permutation(x)，这两个有什么不同，或者说有什么关系？
答: np.random.permutation与np.random.shuffle有两处不同：
如果传给permutation一个矩阵，它会返回一个洗牌后的矩阵副本；而shuffle只是对一个矩阵进行洗牌，无返回值。 如果传入一个整数，它会返回一个洗牌后的arange。
下面的源码可以看出来：
```
3280        def permutation(self, object x):
...
3307            if isinstance(x, (int, np.integer)):
3308                arr = np.arange(x)
3309            else:
3310                arr = np.array(x)
3311            self.shuffle(arr)
3312            return arr
```
