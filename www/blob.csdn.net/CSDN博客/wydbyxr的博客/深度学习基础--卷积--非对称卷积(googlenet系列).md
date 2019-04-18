# 深度学习基础--卷积--非对称卷积(googlenet系列) - wydbyxr的博客 - CSDN博客
2018年11月14日 09:57:08[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：201
# 非对称卷积（googlenet v3）
  1）先进行 n×1 卷积再进行 1×n 卷积，与直接进行 n×n 卷积的结果是等价的。
  2）非对称卷积能够降低运算量，这个很好理解吧，原来是 n×n 次乘法，改了以后，变成了 2×n 次乘法了，n越大，运算量减少的越多。
  3）虽然可以降低运算量，但这种方法不是哪儿都适用的，非对称卷积在图片大小介于12×12到20×20大小之间的时候，效果比较好，具体原因未知。
  原文如下：
  In practice, we have found that employing this factorization does not work well on early layers, but it gives very good results on medium grid-sizes (On m×m feature maps, where m ranges between 12 and 20).
