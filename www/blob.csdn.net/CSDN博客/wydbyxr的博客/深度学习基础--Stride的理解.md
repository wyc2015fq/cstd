# 深度学习基础--Stride的理解 - wydbyxr的博客 - CSDN博客
2018年11月07日 11:02:03[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：278
# Stride的理解
  stride其实是图像处理中常用的概念。
## 解释
  一行有 11 个像素(Width = 11), 对一个 32 位(每个像素 4 字节)的图像, Stride = 11 * 4 = 44.
  但还有个字节对齐的问题, 譬如:
  一行有 11 个像素(Width = 11), 对一个 24 位(每个像素 3 字节)的图像, Stride = 11 * 3 + 3 = 36.
  为什么不是 Stride = 33? 因为它是按 4 字节对齐的.
  根据上面道理, 我们可以手动计算 Stride 的值:
  1）Stride = 每像素占用的字节数(也就是像素位数/8) * Width;
  2）如果 Stride 不是 4 的倍数, 那么 Stride = Stride + (4 - Stride mod 4);
