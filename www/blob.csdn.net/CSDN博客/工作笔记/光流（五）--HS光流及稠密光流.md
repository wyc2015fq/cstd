# 光流（五）--HS光流及稠密光流 - 工作笔记 - CSDN博客





2016年07月14日 22:48:18[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：10336
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









转自：

[http://baike.baidu.com/link?url=AXK--povXLrhU6bRTJPsz4lLzva_fNDP0tLglwKtzi9tu-T8gkIi4ffhHWSSTgQG-HLNB-NC6cgtgUK-TEKHzS7aFU92noxQXyt5xqVLAC7EVND6ICEwZ9MUsoP12Xez](http://baike.baidu.com/link?url=AXK--povXLrhU6bRTJPsz4lLzva_fNDP0tLglwKtzi9tu-T8gkIi4ffhHWSSTgQG-HLNB-NC6cgtgUK-TEKHzS7aFU92noxQXyt5xqVLAC7EVND6ICEwZ9MUsoP12Xez)﻿﻿

# 稠密光流

稠密光流是一种针对图像进行逐点匹配的图像配准方法，不同于稀疏光流只针对图像上若干个特征点，稠密光流计算图像上所有的点的偏移量，从而形成一个稠密的光流场。通过这个稠密的光流场，可以进行像素级别的图像配准，所以其配准后的效果也明显优于稀疏光流配准的效果。但是其副作用也是明显的，由于要计算每个点的偏移量，其计算量也明显大于稀疏光流[2]


## 稠密光流目标函数与求解


类似于稀疏光流，稠密光流的目标函数也是含有一个数据项：使得配准后的两张图像上每个对应点尽可能相同，用数学公式表示就是：


![](http://a.hiphotos.baidu.com/baike/s%3D320/sign=5638777ed7160924d825a419e406359b/b21bb051f8198618d6bfffac4ded2e738bd4e61a.jpg)


上式中

![](http://b.hiphotos.baidu.com/baike/s%3D47/sign=36b131c7aeec8a13101a56e7f60380e9/4ec2d5628535e5dd868cb0f771c6a7efce1b6271.jpg)

为参考图像（第一张图像），

![](http://g.hiphotos.baidu.com/baike/s%3D44/sign=790f1156221f95caa2f593b2c917124c/8601a18b87d6277f702a26102f381f30e924fcda.jpg)

为当前图像（第二张图像），也是待配准图像。偏移量

![](http://b.hiphotos.baidu.com/baike/s%3D99/sign=342d53934010b912bbc1faf7c3fd6208/4d086e061d950a7bfb345aec0dd162d9f2d3c991.jpg)

是图像上没一点的偏移量，

![](http://a.hiphotos.baidu.com/baike/s%3D34/sign=e0e453160bf3d7ca08f63972f31fe993/5fdf8db1cb13495471e5fc4c514e9258d1094a1a.jpg)

是一个误差函数，根据不同的需求可以取绝对值函数，二次函数等一些非负对称且正半轴单调递增的函数。

由于是稠密匹配，如果单一只有上面的数据项是不够的，因为对纯色区域的点显然是没法通过上式找到准确的偏移量的，因此还需要加入一个平滑项。平滑项的目的就是，对于特征比较弱的区域的点，其偏移量尽量向强特征点的偏移量靠近，或者说相邻两个特征点的偏移量相差不能太大，也就是偏移量的变化率不能太大，用数学语言描述就是：


![](http://e.hiphotos.baidu.com/baike/s%3D224/sign=5f4624577ff0f736dcfe4b033e55b382/6f061d950a7b02088fd62dc965d9f2d3572cc8ff.jpg)


上式中

![](http://a.hiphotos.baidu.com/baike/s%3D112/sign=4a3f7717def9d72a1364141ce62a282a/8c1001e93901213fe87d5df453e736d12f2e954f.jpg)

是梯度的模长，

![](http://f.hiphotos.baidu.com/baike/s%3D46/sign=c1be13e2e1cd7b89ed6c3b850e244687/730e0cf3d7ca7bcb1fffd407b9096b63f624a815.jpg)

是一个类似

![](http://a.hiphotos.baidu.com/baike/s%3D34/sign=e0e453160bf3d7ca08f63972f31fe993/5fdf8db1cb13495471e5fc4c514e9258d1094a1a.jpg)

的误差函数。

将上面两个公式合并起来就得到了最终的目标函数：


![](http://f.hiphotos.baidu.com/baike/s%3D482/sign=10f5e30b4e540923ae696276a058d1dc/c9fcc3cec3fdfc030f01fe90d33f8794a4c22698.jpg)


为了描述简单，上面的偏移量

![](http://b.hiphotos.baidu.com/baike/s%3D99/sign=342d53934010b912bbc1faf7c3fd6208/4d086e061d950a7bfb345aec0dd162d9f2d3c991.jpg)

就不写成显著的函数形式，

![](http://h.hiphotos.baidu.com/baike/s%3D12/sign=6c4ad3f5ac64034f0bcdc604aec355fb/2934349b033b5bb5ee5b3dd631d3d539b600bc0d.jpg)

是一个权重因子。目前主流的稠密光流算法中，主要根据误差函数取绝对值函数和二次函数分成两大类（其他函数由于复杂度较高，一般比较少讨论）：L1-光流（L1-范数）和L2-光流（L2-范数）。下面就两大类光流进行讨论：


## 稠密光流L2-光流


L2-光流是最早进行研究的稠密光流，由于其两个误差函数都是二次函数，因此被称为L2-光流。由于二次函数是所有误差函数中最容易进行极值求解的，因此这种方法在快速计算中经常被用到。L2-光流最早是Horn和Schunck在1981年提出并求解的[1]，因此也被称为H-S算法。

L2-光流的目标函数如下：


![](http://g.hiphotos.baidu.com/baike/s%3D518/sign=674252266009c93d03f20ef6a73df8bb/aa18972bd40735fa4898569199510fb30f2408a9.jpg)


为了方便计算，可以考虑用稀疏光流中近似方法，对数据项进行线性近似，从而得到新的目标函数：


![](http://d.hiphotos.baidu.com/baike/s%3D452/sign=72e41619263fb80e08d160d204d12ffb/b2de9c82d158ccbf1f8caf121ed8bc3eb13541e1.jpg)


原始论文中的标准解法是采用变分法，得到一个Euler-Lagrange方程，然后通过数值方法求解偏微分方程。考虑到一般人没学过泛函分析，此处我用初等微积分的知识进行求解。

将上面的定积分形式改写成离散的形式：


![](http://g.hiphotos.baidu.com/baike/s%3D301/sign=5b9405cbd239b60049ce09b7d8503526/4ec2d5628535e5dda94ba3f771c6a7efce1b62b6.jpg)



![](http://h.hiphotos.baidu.com/baike/s%3D212/sign=1370c6073d292df593c3ab148e305ce2/9e3df8dcd100baa14a8f44934010b912c8fc2e72.jpg)



![](http://g.hiphotos.baidu.com/baike/s%3D517/sign=9319d40997ef76c6d4d2fb2aaa17fdf6/bd3eb13533fa828bd0da9bcbfa1f4134970a5a18.jpg)


注意上面的

![](http://e.hiphotos.baidu.com/baike/s%3D25/sign=b207617ed7160924d825a51ed5074c47/472309f79052982276f43febd0ca7bcb0b46d4d6.jpg)

是

![](http://e.hiphotos.baidu.com/baike/s%3D45/sign=67d8b61a5dafa40f38c6cfd8ab645971/1b4c510fd9f9d72a74711135d32a2834349bbb84.jpg)

的简写形式，不是偏导数。由此

![](http://e.hiphotos.baidu.com/baike/s%3D49/sign=ad88e3326759252da7171c0d359bf9c3/18d8bc3eb13533faf6fa9206afd3fd1f41345b4d.jpg)

可以看成是所有

![](http://b.hiphotos.baidu.com/baike/s%3D57/sign=531bf909758b4710ca2ffdcbc2ce6384/0b7b02087bf40ad18b63c6cb502c11dfa9ecce17.jpg)

构成的多元函数，应用多元函数的极值求解不难得到：


![](http://d.hiphotos.baidu.com/baike/s%3D708/sign=939dab9862380cd7e21ea0ed9945ad14/e4dde71190ef76c6840caf039a16fdfaaf51673f.jpg)



![](http://d.hiphotos.baidu.com/baike/s%3D698/sign=d6359fe2a851f3dec7b2ba6daceef0ec/a71ea8d3fd1f4134dc295256221f95cad1c85eff.jpg)


平滑项求偏导数含有4项是因为

![](http://e.hiphotos.baidu.com/baike/s%3D205/sign=f15bdcc5e01190ef05fb95dffb1b9df7/a71ea8d3fd1f4134e3640956221f95cad1c85e42.jpg)

这三项中均含有

![](http://e.hiphotos.baidu.com/baike/s%3D25/sign=b207617ed7160924d825a51ed5074c47/472309f79052982276f43febd0ca7bcb0b46d4d6.jpg)

这一项，为了书写方便，舍去下标得到方程组：


![](http://a.hiphotos.baidu.com/baike/s%3D193/sign=c25f83116c63f624185d3d0ab444eb32/0df431adcbef76097b7691b729dda3cc7cd99e9c.jpg)



![](http://e.hiphotos.baidu.com/baike/s%3D191/sign=bef810ede7fe9925cf0c6d5905a95ee4/cc11728b4710b9124c2dfed6c4fdfc039245222d.jpg)


上式中：

![](http://d.hiphotos.baidu.com/baike/s%3D270/sign=17b1cc073d292df593c3ab128c315ce2/d000baa1cd11728bc44f850acffcc3cec3fd2cb3.jpg)

，

![](http://a.hiphotos.baidu.com/baike/s%3D264/sign=7222cecb502c11dfdad1b82557266255/6159252dd42a283424603f235cb5c9ea14cebfd7.jpg)

，

![](http://e.hiphotos.baidu.com/baike/s%3D46/sign=3e6fedba0bf431adb8d2423f4a36d8a7/9345d688d43f8794281a4006d51b0ef41bd53a34.jpg)


上面的方程组实际是一个大型稀疏线性方程组，一般采用迭代法求解，最容易编程实现的就是SOR（超松弛迭代法），解出上面的二元一次方程就可以得到相应的雅克比迭代公式：


![](http://g.hiphotos.baidu.com/baike/s%3D160/sign=38b619f63a6d55fbc1c672205d224f40/5243fbf2b2119313ae5f8a9862380cd791238dfd.jpg)



![](http://b.hiphotos.baidu.com/baike/s%3D159/sign=c6e04f3c45a7d933bba8e076944bd194/377adab44aed2e734287f3048001a18b87d6faa8.jpg)


Lluís Garrido提供了HS-算法的c语言实现[3]





## 稠密光流L1-光流


相对于L2-光流，L1-光流提出的时间就相对要晚很多了[4]，具体原因可能是其求解过程比较复杂。但是采用L1-范数有个好处是，误差函数增长较慢，这样对于大的偏移量的惩罚项也相对较小，从而可以计算偏移量较大的光流。其目标函数的数据项和平滑项由两个绝对值函数组成，因此被称为L1-光流。具体目标函数如下：


![](http://f.hiphotos.baidu.com/baike/s%3D461/sign=5cf47f19263fb80e08d160d107d12ffb/9d82d158ccbf6c81dd175fc0bb3eb13533fa40f1.jpg)


由于绝对值函数在0点处没有导数，因此一般不适合传统导数方法求极值。有一种近似做法是采用另一个处处可微的函数替代绝对值函数：


![](http://h.hiphotos.baidu.com/baike/s%3D92/sign=66117ec3b1fd5266a32b3016aa184ea1/279759ee3d6d55fb5521040e6a224f4a20a4dd37.jpg)


其中s是一个非常小的常数，这样就可以采用传统的梯度法和牛顿法对上面的函数进行极值求解了[4]。唯一的不足是，采用上面的近似后，导数的计算较为复杂，每次迭代的计算量灰常大，计算速度狠慢。后来有一个哥们发现数学上有个Total Variation（TV）可以用于二次式与绝对值和的求解，因此又整出了一套称为TV-L1的高大上的解法[5] ，这种解法相对上面的解法的优点就是快，另外也更精确。正因为如此，后面发论文的也基本都采用L1-范数的形式。

TV-L1的基本思想就是把目标函数拆分成两个：一个标准TV-L1的形式，另一个可以直接求解析解的形式。为了方便书写，将二元组

![](http://g.hiphotos.baidu.com/baike/s%3D33/sign=a4d9e9174cfbfbedd859307c78f0573c/d62a6059252dd42ad6427983043b5bb5c9eab84b.jpg)

用向量

![](http://d.hiphotos.baidu.com/baike/s%3D10/sign=aaf6f7167cec54e745ec1e1eb8382a8c/0824ab18972bd407be33a9d57c899e510fb30925.jpg)

表示，

![](http://e.hiphotos.baidu.com/baike/s%3D35/sign=4f05cf3ec8fc1e17f9bf8a344b90a9f4/3b87e950352ac65c1c941e5bfcf2b21193138a00.jpg)

用向量

![](http://b.hiphotos.baidu.com/baike/s%3D10/sign=919af9194b086e066ea83b4b02088b4a/203fb80e7bec54e71fd27f36be389b504fc26adf.jpg)

表示，引入辅助变量

![](http://h.hiphotos.baidu.com/baike/s%3D9/sign=ef904ddf983df8dca23d83a1cd3f83/29381f30e924b899365e3a1069061d950a7bf6bc.jpg)

，目标函数可以简写为：


![](http://c.hiphotos.baidu.com/baike/s%3D335/sign=814007c18a1001e94a3c120c8d0f7b06/0823dd54564e9258f385c8c69b82d158ccbf4e60.jpg)


其中：

![](http://e.hiphotos.baidu.com/baike/s%3D354/sign=10fdc0c78ab1cb133a693a16e95456da/738b4710b912c8fce79b5de5fb039245d68821a9.jpg)


然后将其拆分成标准的TV-L1形式：


![](http://d.hiphotos.baidu.com/baike/s%3D207/sign=523c748f5eee3d6d26c680cb74166d41/37d3d539b6003af33986bb48322ac65c1038b6dd.jpg)


和可直接求解析解项：


![](http://e.hiphotos.baidu.com/baike/s%3D225/sign=10334ab5ccef7609380b9e9d1bdca301/5d6034a85edf8db1d7d4324c0e23dd54564e741b.jpg)


其求解过程采用螺旋法。

稠密光流1. 求解TV-L1项：

首先固定

![](http://g.hiphotos.baidu.com/baike/s%3D9/sign=19389b0f783e6709ba0049ce3b3422/a08b87d6277f9e2fb9b67a201830e924b899f313.jpg)

，然后采用TV-L1求解第一项中的

![](http://h.hiphotos.baidu.com/baike/s%3D10/sign=253b30c1f5d3572c62e298dc8b1383b3/4ec2d5628535e5ddc8fbc2f771c6a7efcf1b62c6.jpg)

可以得到：


![](http://d.hiphotos.baidu.com/baike/s%3D227/sign=d9c3a79fee50352ab561220a6443fb1a/9a504fc2d562853506ed7a0997ef76c6a7ef638d.jpg)


其中

![](http://f.hiphotos.baidu.com/baike/s%3D98/sign=b05c6f0997ef76c6d4d2f7239c163fcc/d50735fae6cd7b89cbd087ab082442a7d9330e5d.jpg)

，

![](http://g.hiphotos.baidu.com/baike/s%3D94/sign=d0f78f205443fbf2c12caa27b17ec7f6/d0c8a786c9177f3e71a2562577cf3bc79f3d5607.jpg)

，

![](http://f.hiphotos.baidu.com/baike/s%3D128/sign=935783ef9752982201333dc1efca7b3b/faf2b2119313b07e1ff4ef200bd7912397dd8c49.jpg)

由下面的迭代式计算：


![](http://e.hiphotos.baidu.com/baike/s%3D227/sign=88fdd67833a85edffe8cf9217e5509d8/f9dcd100baa1cd114647cd08be12c8fcc3ce2d20.jpg)


其中

![](http://d.hiphotos.baidu.com/baike/s%3D52/sign=020f1a4a9f22720e7fcee2f87acbeb43/9825bc315c6034a8552707a9cc134954082376d4.jpg)

，初始值

![](http://g.hiphotos.baidu.com/baike/s%3D45/sign=f764ef81ae014c081d3b29a00a7b8c08/7dd98d1001e939013f7fef167cec54e736d1969c.jpg)

，迭代步长

![](http://h.hiphotos.baidu.com/baike/s%3D52/sign=cc9efa76dac451daf2f60ce9b6fd7210/279759ee3d6d55fb65b0140e6a224f4a20a4dda0.jpg)



### 稠密光流2. 解析解项：


上面求出

![](http://c.hiphotos.baidu.com/baike/s%3D10/sign=a57d912577cf3bc7ec00c9ecd1006a39/4d086e061d950a7bc07c23ec0dd162d9f2d3c949.jpg)

后就将其固定，解析解项是一个关于

![](http://h.hiphotos.baidu.com/baike/s%3D9/sign=969a854c514e9258a2348adf9c5d39/b219ebc4b74543a91d9f97e419178a82b9011415.jpg)

的分段二次函数，可以很容易求出其极值：


![](http://d.hiphotos.baidu.com/baike/s%3D280/sign=c50d828d0d7b020808c938e952d9f25f/d8f9d72a6059252d7f21b72c339b033b5bb5b9e0.jpg)


求出

![](http://e.hiphotos.baidu.com/baike/s%3D9/sign=15f486e1d02a60595610ed2b28a167/9d82d158ccbf6c81c4f146c0bb3eb13532fa40d7.jpg)

后将其代回1中继续求解

![](http://d.hiphotos.baidu.com/baike/s%3D10/sign=57270e1e1a950a7b71354ac40ad18976/7aec54e736d12f2ea217fb4848c2d56285356883.jpg)

，直到

![](http://b.hiphotos.baidu.com/baike/s%3D66/sign=132762107cf40ad111e4c4e5572c862b/203fb80e7bec54e72f8d4f36be389b504fc26ab0.jpg)

终止迭代。

opencv2.3里面有TV-L1的源代码实现[6] ，可以参考其源代码理解算法流程。

﻿﻿



