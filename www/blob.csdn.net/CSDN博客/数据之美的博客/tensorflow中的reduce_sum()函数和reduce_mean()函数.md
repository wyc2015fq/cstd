# tensorflow中的reduce_sum()函数和reduce_mean()函数 - 数据之美的博客 - CSDN博客
2019年01月23日 00:45:41[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：60
在计算损失时，通常会用到reduce_sum()函数来进行求和，但是在使用过程中常常会搞不清楚具体是怎样进行计算的，通过查阅资料，逐渐搞清楚了这个函数的用法，下面就来详细解释一下。
在TensorFlow官方文档中有这样的解释： 
![](https://img-blog.csdn.net/20171115220137017?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hlbmdzaHVoYW8xOTkx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
其实在reduce_sum()中，是从维度上去考虑的。其中的参数reduction_indices很容易搞蒙圈，上个图加深理解吧。 
![](https://img-blog.csdnimg.cn/20190123004414800.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29wcG82MjI1ODgwMQ==,size_16,color_FFFFFF,t_70)
调用reduce_sum(arg1, arg2)时，参数arg1即为要求和的数据，arg2有两个取值分别为0和1，通常用reduction_indices=[0]或reduction_indices=[1]来给arg2传递参数。从上图可以看出，当arg2 = 0时，是纵向对矩阵求和，原来矩阵有几列最后就得到几个值；相似地，当arg2 = 1时，是横向对矩阵求和，原来矩阵有几行最后就得到几个值；当省略arg2参数时，默认对矩阵所有元素进行求和，最后得到一个值。
看到这里，函数名的前缀为什么是reduce_其实也就很容易理解了，reduce就是“对矩阵降维”的含义，下划线后面的部分就是降维的方式，在reduce_sum()中就是按照求和的方式对矩阵降维。那么其他reduce前缀的函数也举一反三了，比如reduce_mean()就是按照某个维度求平均值，等等。
--------------------- 
作者：闪电小飞猪 
来源：CSDN 
原文：https://blog.csdn.net/chengshuhao1991/article/details/78545723 
版权声明：本文为博主原创文章，转载请附上博文链接！
