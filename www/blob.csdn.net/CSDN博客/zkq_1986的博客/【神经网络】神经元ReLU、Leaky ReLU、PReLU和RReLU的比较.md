# 【神经网络】神经元ReLU、Leaky ReLU、PReLU和RReLU的比较 - zkq_1986的博客 - CSDN博客





2018年12月12日 11:51:18[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：123








下图是ReLU、Leaky ReLU、PReLU和RReLU的比较：

![](http://p0.ifengimg.com/pmop/2017/0701/C56E5C6FCBB36E70BA5EBC90CBD142BA320B3DF6_size19_w740_h217.jpeg)



    ReLU，对小于0部分，直接置为0；

    Leaky ReLU，对小于0部分，进行这样的转换：y_i=a_i * x_i ，它的a_i是固定的；

    PReLU中的a_i 根据数据变化而变化；

    RReLU中的a_i是一个在一个给定的范围内随机抽取的值，这个值在测试环节就会固定下来。



