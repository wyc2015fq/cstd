# F-Principle：初探深度学习在计算数学的应用 - Paper weekly - CSDN博客





2019年02月28日 08:33:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：113









![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)




作者丨许志钦、张耀宇

学校丨纽约大学阿布扎比分校博士后、纽约大学库朗研究所访问学者

研究方向丨计算神经科学、深度学习理论




在上一篇文章[F-Principle：初探理解深度学习不能做什么](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247494694&idx=1&sn=7020fb834ce8307f27ce9c072047d37d&chksm=96ea33a6a19dbab0a6585daa00d5b5c65501dd633fa677c80541fad0e170d92baffe379315c3&scene=21#wechat_redirect)中，我们介绍了如何用一个统一的框架来初步理解深度学习的优势与局限。在这篇文章中，我们将探索**如何利用我们对深度学习已有的理解来设计算法**。我们期待这些初步的探索能对解决真实问题起到引导作用。 




首先，我们回顾一下这个框架。我们的发现可以用一句话概括：深度学习倾向于优先使用低频来拟合目标函数。我们将这个机制称为 F-Principle（频率原则）。




我们先用一个简单的例子来理解 F-Principle。用一个深度神经网络（DNN）去拟合如下的红色实线函数。训练完后，该曲线能被深度网络（蓝色点）很好地拟合。




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmXQIJSH0MWGX0XGhK1edvkUp0X3ibzViaMviaQdoNSicdPoSecsySI6FwDJ3VlJPSTibgV2BzeAVmK9yw/640?wx_fmt=png)




我们感兴趣的是 DNN 的训练过程。如下图动画所示，红色为目标函数的 FT（傅里叶变换），蓝色为网络输出的 FT，每一帧表示一个训练步，横坐标是频率，纵坐标是振幅。




我们发现，在训练过程中，DNN 表示的函数的演化在频域空间有一个清晰的规律，即频率从低到高依次收敛。




![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgmXQIJSH0MWGX0XGhK1edvkNlmWkm4D6vcWm0nnQptn0cBRrOsNl9xOC3XsfPJkGjbRanSpBH7XTA/640?wx_fmt=gif)




部分读者觉得 F-Principle 非常直观，甚至非常显然（trivial）。部分原因可能是因为 F-Principle 并不是一个陌生的概念，而是我们日常生活中经常都在用的一个原则。想象一下，如果让一个人去记住一个没见过的东西，一般比较粗糙的轮廓信息会先被记住，然后再是很多细节。




没错，DNN 也正是使用了这样的一个学习过程。举一个例子，我们来训练一个 DNN 来记住一张图片。DNN 的输入是一个位置坐标 (x,y)，我们希望它输出这个位置对应的灰度值。




下图的一系列图展示了不同训练步数，DNN 学习到的图像，正如我们前面所猜测的从粗糙的轮廓到细节的学习过程。




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmXQIJSH0MWGX0XGhK1edvk3LicMibYeBNIWURw0gJRfp5C8b8YDPAcyXcusGDItiaRiaIxS2yhuGUAEA/640?wx_fmt=png)




然而！更多的人非常惊讶于 DNN 有这样简单的规则。特别地，在计算数学中，很多迭代格式都是先收敛高频，例如 Jacobi 方法和 Gauss–Seidel 方法。由于低频往往是比较重要的，因此高频优先的原则造成了很多苦恼。于是许多研究者手动设计很多算法来提升收敛低频的速度，比如多重网格法。




对于大部分 DNN，它并不需要任何显示的条件或者手动设计，它自然有低频优先的原则。举个例子，考虑如下 Poisson 方程：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmXQIJSH0MWGX0XGhK1edvkEhgp11MVY2onht7OfACyFpUCkI81EQYmDUIxyULH1hBSqApEEJ0yyA/640?wx_fmt=png)




其中，![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmXQIJSH0MWGX0XGhK1edvkeV4FKIH0e5wFdzvXgUTvKibDdWLooEL04sbpP3iaWltf1iaONGsbbx6Ig/640?wx_fmt=png)。它的真实解如下图曲线所示：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmXQIJSH0MWGX0XGhK1edvkS4gQkcwHTGqxAPdYR18dYKqicpoGNJGXRVGXugFhK4pwpweXIjpgpCA/640?wx_fmt=png)

**▲**黑色为Poisson方程的真解，插图里红色为真解的傅里叶变换




真解中有三个比较显著的频率峰值。如果我们用 Jacobi 迭代去解这个方程，如下图，高频先收敛。




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmXQIJSH0MWGX0XGhK1edvk3cwQlgWq3mRqGSw8nTnJMjdiaFEXuUvQbE6ByLXReX1ZZXg8KviaO2Lw/640?wx_fmt=png)



**▲**Jacobi迭代：三个重要频率点处的相对误差与训练步数的关系，红色表示相对误差小，高频先收敛




如果我们用 DNN 来解这个方程，我们会得到完全不一样的结果。DNN 的输入为 x，我们用 DNN 的输出去逼近真解 u(x)。损失函数采用 Poisson 方程的能量泛函。具体解的过程可以参见论文。如下图所示，F-Principle 非常明显，既低频比高频收敛得快很多。




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmXQIJSH0MWGX0XGhK1edvkeoyThBnKFZzqhDwMibaeZAqicZYrnxEgOey2G1XPVIhyWbXmLkRI5bTA/640?wx_fmt=png)

**▲**宽度为1-4000-500-400-1的DNN：三个重要频率点处的相对误差与训练步数的关系，红色表示相对误差小，低频先收敛




一个自然的想法是，为了更快地解 Poisson 方程，我们可以先用 DNN 解出低频部分。等低频收敛好以后，将 DNN 的输出作为 Jacobi 迭代的初值来继续解 Poisson 方程。




在下图中，我们展示了结合两种方法的效果。下图展示的是数值解和真实解之差的最大绝对值随计算机运行时间的变化。




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmXQIJSH0MWGX0XGhK1edvkazLtBX9w1vxJ4D65vhpOfnG5FCib2erKFBlVuXWCCDeElaxl1jSJYyw/640?wx_fmt=png)

**▲**结合DNN与Jacobi来解Poisson方程。横坐标：计算机运行时间。纵坐标：数值解和真实解之差的最大绝对值。绿色星号：DNN的结果。虚线：Jacobi方法继续运算的结果，不同虚线表示选择不同DNN训练时刻的输出作为初值




如果单纯使用 DNN，绿色星号表明在一段时间后，DNN 的输出振荡很厉害。三条虚线的结果是在不同时刻使用 Jacobi 继续运算的结果。最左边的蓝色虚线选择的时刻过早，导致 Jacobi 方法需要花大时时间来收敛低频，因此误差下降很慢。最右边的绿色虚线选择的时刻过晚，导致大量时间花在 DNN 收敛高频上。中间的橙色虚线刚好利用了 DNN 收敛好的低频，并且马上使用 Jacobi 方法快速收敛高频，所以误差在短时间内能下降最快。




在偏微分方程的数值问题中，有很多问题对于传统算法是很困难的。很多研究工作已经开始探索如何使用 DNN 来解这些问题。




这个例子只是一个非常简单地情况，对于用 F-Principle 的理解来帮助 DNN 应用在更复杂、更高维的问题中，仍然有非常多值得探讨的问题。这个例子展示了在理解 DNN 的基础上去使用 DNN，我们可以既利用 DNN 的强项，又能避免 DNN 的弱点。




# 参考文献




Xu, Zhi-Qin John; Zhang, Yaoyu; Luo, Tao; Xiao, Yanyang & Ma, Zheng (2019), ‘Frequency principle: Fourier analysis sheds light on deep neural networks’, arXiv preprint arXiv:1901.06523.




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)







**点击以下标题查看更多往期内容：**




- 
[Airbnb实时搜索排序中的Embedding技巧](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247494488&idx=1&sn=452ac80c593a9b31252031eac38d0e01&chksm=96ea34d8a19dbdce940ed25bb93507aa6c4d118f84dd0bb965b060f232fe5d41894bbc9edcb6&scene=21#wechat_redirect)

- 
[图神经网络综述：模型与应用](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493906&idx=1&sn=15c9f18a1ce6baa15dc85ecb52e799f6&chksm=96ea3692a19dbf847c1711e6e194ad60d80d11138daf0938f90489a054d77cfd523bee2dc1d2&scene=21#wechat_redirect)

- 
[近期值得读的10篇GAN进展论文](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493987&idx=1&sn=ce1bcdce28e78f4a307743e389f42b10&chksm=96ea36e3a19dbff5cff7f4f1c9d9fc482bb2144d80566319b3d26bce4d9ab80689d38ab2e427&scene=21#wechat_redirect)


- 
[自然语言处理中的语言模型预训练方法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492317&idx=1&sn=e823a75d9463257ed9ea7b3e4677c1ae&chksm=96ea3d5da19db44be0872ff4e29043aa72c7a624a116196bfeeca092a15f9209d7cf8ce46eb5&scene=21#wechat_redirect)

- 
[从傅里叶分析角度解读深度学习的泛化能力](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491082&idx=1&sn=d7c1cb39c3be43154c658ca5a791eb4c&chksm=96e9c18aa19e489c32fe36671e4208ce42bf200e3a7adeda200fa2785462d16f85c58bb455b4&scene=21#wechat_redirect)

- 
[深度思考 | 从BERT看大规模数据的无监督利用](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247494978&idx=1&sn=529b8f661b13c7b6b729e9a926b0737a&chksm=96ea32c2a19dbbd43456086a2186480fa548fa982d536564e167739ef46282eaf6fca7316ced&scene=21#wechat_redirect)


- 
[AI Challenger 2018 机器翻译参赛总结](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247494103&idx=1&sn=fc372862e0369b1f6a943bf997f6fc1b&chksm=96ea3657a19dbf4108bbc4179e779aa04ef05fe84f0013fa6425b0cd7e761e9880917361c4c1&scene=21#wechat_redirect)

- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247494507&idx=1&sn=3c3cafef0fb51a7e40d9b9bbab53fd5f&chksm=96ea34eba19dbdfd31eaa760bb7cfd5e18f2e967c83c6ea6693ad9a062c55b3009211d824ca3&scene=21#wechat_redirect)[小米拍照黑科技：基于NAS的图像超分辨率算法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247495166&idx=1&sn=a158e603651bc4f26836151a9113e856&chksm=96ea327ea19dbb68b8987aca041bb21579a35b1c679e91fd2368c7f2fb7acd58508cd531bdfe&scene=21#wechat_redirect)

- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493520&idx=1&sn=2b04c009ef75291ef3d19e8fe673aa36&chksm=96ea3810a19db10621e7a661974c796e8adeffc31625a769f8db1d87ba803cd58a30d40ad7ce&scene=21#wechat_redirect)[异构信息网络表示学习论文解读](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247495219&idx=1&sn=b3a29b833fe8438e12b600650ec0245a&chksm=96ea31b3a19db8a5a335cc445f04eb13d5b1ee5451d688544240cce27dbc953993da4e842ab6&scene=21#wechat_redirect)

- 
[不懂Photoshop如何P图？交给深度学习吧](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247495276&idx=1&sn=99a84e81ba9f9a90b323c21c8c905765&chksm=96ea31eca19db8faca5cdd6447136e34a61a8a5a2d8e01489657f791841d30e0c78901a87400&scene=21#wechat_redirect)











**![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)****#****投 稿 通 道#**

** 让你的论文被更多人看到 **





如何才能让更多的优质内容以更短路径到达读者群体，缩短读者寻找优质内容的成本呢？ **答案就是：你不认识的人。**




总有一些你不认识的人，知道你想知道的东西。PaperWeekly 或许可以成为一座桥梁，促使不同背景、不同方向的学者和学术灵感相互碰撞，迸发出更多的可能性。 




PaperWeekly 鼓励高校实验室或个人，在我们的平台上分享各类优质内容，可以是**最新论文解读**，也可以是**学习心得**或**技术干货**。我们的目的只有一个，让知识真正流动起来。




📝 **来稿标准：**

• 稿件确系个人**原创作品**，来稿需注明作者个人信息（姓名+学校/工作单位+学历/职位+研究方向） 

• 如果文章并非首发，请在投稿时提醒并附上所有已发布链接 

• PaperWeekly 默认每篇文章都是首发，均会添加“原创”标志




**📬 投稿邮箱：**

• 投稿邮箱：hr@paperweekly.site

• 所有文章配图，请单独在附件中发送 

• 请留下即时联系方式（微信或手机），以便我们在编辑发布时和作者沟通










🔍




现在，在**「知乎」**也能找到我们了

进入知乎首页搜索**「PaperWeekly」**

点击**「关注」**订阅我们的专栏吧







**关于PaperWeekly**





PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/640?)

▽ 点击 | 阅读原文| 获取最新论文推荐




