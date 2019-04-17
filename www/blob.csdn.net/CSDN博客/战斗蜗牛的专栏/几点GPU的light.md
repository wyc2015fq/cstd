# 几点GPU的light - 战斗蜗牛的专栏 - CSDN博客





2011年12月14日 16:02:55[vbskj](https://me.csdn.net/vbskj)阅读数：580标签：[fft](https://so.csdn.net/so/search/s.do?q=fft&t=blog)
个人分类：[CUDA](https://blog.csdn.net/vbskj/article/category/643641)









1. FFT可以自己定制，根据不同的规模，自己改写。下一代的cufft可能可以从device中fetch data

2. sqrt也可以自己写，应该要自己写。这个太费时了。

3. 累加的问题解决：原子操作是最差的。  1）利用share memory进行两两归并（比如都加到第一个，或者4:1,5:2.。。），然后多个block间再进行归并，间接实现累加。 缺点：必须知道累加的次数； 2）利用register的快速计算进行累加，比如一个warp内利用一个register进行3-5次累加，隐藏fetch data的延迟；3）利用fermi中的内存映射，隐藏延迟。





