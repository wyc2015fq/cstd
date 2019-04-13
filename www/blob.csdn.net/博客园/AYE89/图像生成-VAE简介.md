
# 图像生成-VAE简介 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[图像生成-VAE简介](https://www.cnblogs.com/eniac1946/p/8126794.html)
|Posted on|2017-12-27 17:20|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8126794)|[收藏](#)
|VAE(Variational Autoencoder)   生成式模型
|理论：
|基于贝叶斯公式、KL散度的推导
|1. 自动编码器的一般结构
![](https://images2017.cnblogs.com/blog/1181483/201712/1181483-20171227172523956-902758180.png)
|2. 产生一幅新图像
|输入的数据经过神经网络降维到一个编码(code)，接着又通过另外一个神经网络去解码得到一个与输入原数据一模一样的生成数据，然后通过去比较这两个数据，最小化他们之间的差异来训练这个网络中编码器和解码器的参数。当这个过程训练完之后，我们可以拿出这个解码器，随机传入一个编码(code)，希望通过解码器能够生成一个和原数据差不多的数据，上面这种图这个例子就是希望能够生成一张差不多的图片。
![](https://images2017.cnblogs.com/blog/1181483/201712/1181483-20171227172648628-333951104.png)
|3. VAE
![](https://images2017.cnblogs.com/blog/1181483/201712/1181483-20171227172841988-1197899293.png)

|实现：
|[on Caffe:  Github](https://github.com/cjyanyi/vae_tutorial)
|介绍文章 知乎：
|[VAE（4）——实现 - 知乎专栏](https://www.google.com.hk/url?sa=t&rct=j&q=&esrc=s&source=web&cd=1&ved=0ahUKEwiVucOm7anYAhUHEpQKHXnqCN0QFggmMAA&url=https%3A%2F%2Fzhuanlan.zhihu.com%2Fp%2F22684931&usg=AOvVaw3Eps8WajBxre-BAVRfFXGy)




|结构：Encoder 和 Decoder
|三个主要部分：
|Encoder的Loss计算：KL散度；
|z的重采样生成；
|Decoder的Loss计算：最大似然；
![](https://images2017.cnblogs.com/blog/1181483/201712/1181483-20171227172324550-1787791534.png)







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
