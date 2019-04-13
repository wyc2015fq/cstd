
# CNN卷积神经网络的三种基本模式（不懂的话还得多努力啊！） - 喜欢打酱油的老鸟 - CSDN博客


置顶2019年02月06日 21:06:58[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：166


[https://www.toutiao.com/i6652175494592594445/](https://www.toutiao.com/i6652175494592594445/)
2019-01-30 14:38:22
通常用外部api进行卷积的时候，会面临mode选择。
这里清晰展示三种模式的不同之处，其实这三种不同模式是对卷积核移动范围的不同限制。
设 image的大小是7x7，filter的大小是3x3
1，full mode
![CNN卷积神经网络的三种基本模式（不懂的话还得多努力啊！）](http://p1.pstatp.com/large/pgc-image/5c6e34a998c64f23ad1a1f05c8652111)
橙色部分为image, 蓝色部分为filter。full模式的意思是，从filter和image刚相交开始做卷积，白色部分为填0。filter的运动范围如图所示。
2，same mode
![CNN卷积神经网络的三种基本模式（不懂的话还得多努力啊！）](http://p1.pstatp.com/large/pgc-image/2c653ffb9ac445669a34cc9a82a05a50)
当filter的中心(K)与image的边角重合时，开始做卷积运算，可见filter的运动范围比full模式小了一圈。注意：这里的same还有一个意思，卷积之后输出的feature map尺寸保持不变(相对于输入图片)。当然，same模式不代表完全输入输出尺寸一样，也跟卷积核的步长有关系。same模式也是最常见的模式，因为这种模式可以在前向传播的过程中让特征图的大小保持不变，调参师不需要精准计算其尺寸变化(因为尺寸根本就没变化)。
3.valid
![CNN卷积神经网络的三种基本模式（不懂的话还得多努力啊！）](http://p1.pstatp.com/large/pgc-image/d9c22c4a57d94e19a9acc8dad2d540bd)
当filter全部在image里面的时候，进行卷积运算，可见filter的移动范围较same更小了。

