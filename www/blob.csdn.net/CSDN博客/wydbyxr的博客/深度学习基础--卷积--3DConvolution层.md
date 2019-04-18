# 深度学习基础--卷积--3DConvolution层 - wydbyxr的博客 - CSDN博客
2018年11月14日 10:00:39[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：184
# 3DConvolution层
  三维卷积对三维的输入进行滑动窗卷积，当使用该层作为第一层时，应提供input_shape参数。
  例如input_shape = (3,10,128,128)代表对10帧128*128的彩色RGB图像进行卷积 。
  参考资料：
[http://blog.csdn.net/zouxy09/article/details/9002508](http://blog.csdn.net/zouxy09/article/details/9002508)
[http://blog.csdn.net/auto1993/article/details/70948249](http://blog.csdn.net/auto1993/article/details/70948249)
[http://blog.csdn.net/lengxiaomo123/article/details/68926778](http://blog.csdn.net/lengxiaomo123/article/details/68926778)
## 例子1
```
#160*100*22
model.add(Convolution3D(
    10,
    kernel_dim1=9, # depth
    kernel_dim2=9, # rows
    kernel_dim3=9, # cols
    input_shape=(3,160,100,22),
    activation='relu'
))
#now 152*92*14
```
## 例子2
```
#34*19*8
model.add(Convolution3D(
    50,
    kernel_dim1=5, # depth
    kernel_dim2=9, # rows
    kernel_dim3=8, # cols
    activation='relu'
))
#now 26*12*4
```
