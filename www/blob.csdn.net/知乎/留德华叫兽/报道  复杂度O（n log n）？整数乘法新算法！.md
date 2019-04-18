# 报道 | 复杂度O（n log n）？整数乘法新算法！ - 知乎
# 

**编者按：**

在小学时，大家就学会如何用列竖式的方法计算乘法。程序设计竞赛的选手们对于如何进行大数乘法计算想必也不陌生——这个利用快速傅里叶变换的Schönhage–Strassen算法早在上个世纪就已经被发明。如今，David Harvey和Joris van der Hoeven宣称他们发现了复杂度更低的整数乘法算法。

> **文章作者：﻿﻿运筹OR帷幄整理**
**责任编辑：Gloria Q，**唐博
文章发表于**微信公众号【运筹OR帷幄】：**[报道 | 复杂度O（n log n）？整数乘法新算法！](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s/7nvFqf04g45VqYgbUOOJsA)
*欢迎原链接转发，转载请私信*[@运筹OR帷幄](https://www.zhihu.com/people/06894b555d0cf621aa210fc8d70cd16d)*获取信息，盗版必究。*
敬请关注和扩散本专栏及同名公众号，会邀请**全球知名学者**发布运筹学、人工智能中优化理论等相关干货、[知乎Live](https://www.zhihu.com/lives/users/961e8cc4f7512fda1ea6626ce9a05e8e)及行业动态：[『运筹OR帷幄』大数据人工智能时代的运筹学](https://zhuanlan.zhihu.com/operations-research)

在计算机进行乘法计算的时候，传统的计算方法采用了类似竖式计算的方法——正如我们在小学课堂里面所学到的那样，因此时间复杂度是O(n ^ 2)。在进行大数（比如说数字位数超过2048）乘法计算的时候，目前的做法是Schönhage–Strassen算法，该算法利用快速傅里叶变换，将算法的复杂度降到O(n log n log log n)。

最近，Harvey 和Van Der Hoeven 在HAL贴出了两人合著的新论文“Integer multiplication in time O(n log n)”，宣称已经发现时间复杂度为O(n log n)的整数乘法算法。

论文摘要如下：
![](https://pic4.zhimg.com/v2-e136544547d5baed9405efbd4290841f_b.jpg)
（欲获取完整论文，请于公众号后台回复“整数乘法算法”获取论文链接）

David Harvey和Joris van der Hoeven两位大佬可以说是在计算数学领域深耕已久，之前就在整数乘法领域进行了多次合作，还写过一篇论文发现了整数乘法算法当时的最低复杂度。
![](https://pic3.zhimg.com/v2-2caaa7befaed164b50a0af3a1aafcc6a_b.jpg)
O(n log n)的整数乘法算法论文，也在知名社交新闻站点[http://Reddit.com](https://link.zhihu.com/?target=http%3A//Reddit.com)上引来了网友的热烈讨论：
![](https://pic3.zhimg.com/v2-c3d44fc7bcf2f74c6dfff036147295c2_b.jpg)
这里网友提到，当问题规模n超过2 ^ d ^ 12，d  > 1729（因此这个数字大于2 ^ 4096）时，这个算法才能优于O(n ^ 2)的竖式计算，而整个可观察宇宙中的原子数量仅仅是2 ^ 273。

2 ^ 4096 这个数字足以使整个银河系黯然失色，因此小编认为这个算法虽然在科研上意义重大，但是在工业界暂时是无法落地的。
![](https://pic3.zhimg.com/v2-6d8880c2c7de4cc42a6233e1db0de962_b.jpg)
关于这篇论文的数学价值，有网友提到在论文里的算法大量使用了傅里叶分析的定理和工具，这些定理能很好的解决卷积问题，而论文的创新之处在于可以把整数乘法问题变换成2个序列卷积，给出了一个被验证的数学工具。
![](https://pic1.zhimg.com/v2-cacde374f539a3ab4858260e919a6c38_b.jpg)
也有网友指出，尽管这个算法恐怕不能比现有的整数算法更快，也是极其具有现实意义，在讨论整数乘法以及基于整数乘法的其他算法的复杂度时，过去往往要引入很多复杂的数学概念，现在则可以直接在论文里使用O（n log n）而不必再去纠结这些细节。
![](https://pic2.zhimg.com/v2-dc988d3173e413441a1f7be825ea40dd_b.jpg)
有程序设计竞赛背景的网友从浮点数和计算精度的角度比较了竞赛中常用的Cooley-Tukey算法（即蝶形算法）和新的算法。
![](https://pic1.zhimg.com/v2-d1ed0eda83b0f5b0422499bb77705194_b.jpg)
有网友表示尽管自己并非专家，也还没读完这篇论文，但是同样的作者发现了之前被人熟知的算法。因此，虽然这篇论文还没有得到同行的审阅，但应该可以得到学界认可。
![](https://pic2.zhimg.com/v2-2f323eb3e482fb56800ec5eebf80d479_b.jpg)
也有网友询问该论文是否对现在寻找最大已知质数的那个项目 （GIMPS / PrimeGrid）有用，下面有人回答大概是不能。

这篇论文能否被证明正确？这篇论文的发表又将会在之后带来哪些新的成果？让我们一起拭目以待。

也欢迎『运筹OR帷幄』关注者中间的算法专家们，在文末留言回复，发表你们的见解～

参考链接：

[https://www.reddit.com/r/math/comments/b5aiz1/harvey_and_van_der_hoeven_claim_to_have_found_an/?utm_medium=android_app&utm_source=share](https://link.zhihu.com/?target=https%3A//www.reddit.com/r/math/comments/b5aiz1/harvey_and_van_der_hoeven_claim_to_have_found_an/%3Futm_medium%3Dandroid_app%26utm_source%3Dshare)

**温馨提示**

可以在 **本公众号后台 **回复关键词：“ **整数乘法算法 **”获取完整论文，如果觉得有用， 请勿吝啬你的留言和赞哦！~

## **相关文章推荐**

内点法是一种处理带约束优化问题的方法，其在线性规划，二次规划，非线性规划等问题上都有着很好的表现。在线性规划的问题上，相对于鼎鼎大名的单纯形法，内点法是多项式算法，而单纯形法并非多项式算法。从实际应用的效果来说，内点法也达到了足以和单纯形法分庭抗衡的地步，尤其针对大规模的线性规划问题内点法有着更大的发展潜力。

点击蓝字标题，即可阅读《[凸优化算法 I: 内点法(interior point method)求解线性规划问题](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzg2MTA0NzA0Mw%3D%3D%26mid%3D2247487081%26idx%3D1%26sn%3D594deb7f3c796b689e45eb232437384f%26scene%3D21%23wechat_redirect)》

**其他**

[0-1背包问题的动态规划算法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzg2MTA0NzA0Mw%3D%3D%26mid%3D2247487041%26idx%3D1%26sn%3D7f4bcbeeae70bb760e82f61f55f7f255%26scene%3D21%23wechat_redirect)

[整数规划经典方法--割平面法（Cutting Plane Method）](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzg2MTA0NzA0Mw%3D%3D%26mid%3D2247487102%26idx%3D1%26sn%3Dbb579b65df0fe64586bce84f52ff045c%26scene%3D21%23wechat_redirect)

[整数规划精确算法/近似算法/(元)启发算法/神经网络反向传播等算法的区别与关联](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzg2MTA0NzA0Mw%3D%3D%26mid%3D2247487098%26idx%3D1%26sn%3D4d6704390dafc5f6d65170264f105106%26scene%3D21%23wechat_redirect)

## **板块招聘信息**

【报道】板块：招募1~2名责编（兼职），需要文笔好，有热情和责任心，英语水平过硬，运筹学或相关专业硕士及以上学历，每周保证投入2~3小时。

关于我们：[『运筹OR帷幄』团队掠影](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzg2MTA0NzA0Mw%3D%3D%26mid%3D2247486861%26idx%3D1%26sn%3Daf8bc2792660d0a17c8f6364ce370d4a%26scene%3D21%23wechat_redirect)

请将简历发送至：operations_r@163.com

欢迎加入我们这个大家庭！

## **关于本文责编**
![](https://pic3.zhimg.com/v2-24de3ba2195935aaa8985656991b41a2_b.jpg)
扫二维码关注『运筹OR帷幄』公众号：
![](https://pic1.zhimg.com/v2-872dc2d2c5eaf08b38d693490549136c_b.jpg)
[点击查看『运筹OR帷幄』志愿者招募介绍及加入方式](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s/4ganskchRA_iO1CiNtWmOA)：
![](https://pic1.zhimg.com/v2-d1fbc7022dc62969dcf8a6afdacdfcb4_b.jpg)

