# 写完代码后，一般人检查自己有没有bug，他是检查编译器有没有bug - 知乎
# 



> Root 编译自 Wired
量子位 出品 | 公众号 QbitAI


这个人，只能是谷歌新任的AI掌门人Jeff Dean了。
![](https://pic4.zhimg.com/v2-e7589f2efa7a91f979ab052cc4ae026f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='810' height='72'></svg>)
4月初，[谷歌宣布分拆搜索和AI部门](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247496450%26idx%3D1%26sn%3Dc285080f681880d91d8985db8e404730%26chksm%3De8d04470dfa7cd66b187c3b79aecbed1eea5c9e6b0d5df01f59879794ecdfa9f487326fe5bba%26scene%3D21%23wechat_redirect)，并让谷歌的传奇大神Jeff Dean担任AI部门的负责人。为了进一步了解谷歌在AI上的新动作和野心，《连线》专门了采访Dean。

> 谷歌在AI方面的研究，大部分用于改进现有的产品，比如说搜索和[智能手机上的软件](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247491563%26idx%3D2%26sn%3D2ede16aae98c9fe61d4d43fc2a32cbdb%26chksm%3De8d3a899dfa4218fa6954d0b29ac6e4f703aaa16d26642fae205809e879e4a477cb4370d5cd4%26scene%3D21%23wechat_redirect)。

众所周知，谷歌已经发力的相关AI业务线还有[解决AI人才短缺的自动化训练AI的AutoML项目](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247493350%26idx%3D1%26sn%3D76db3f988ae1c373c50ebe6814a276ea%26chksm%3De8d05194dfa7d88257ae1dc188aa68ace7fc8f9dc90ab3da93152a973d4509441eccff1a368d%26scene%3D21%23wechat_redirect)；
![](https://pic4.zhimg.com/v2-6c8cdbd3ec5a7c560e69a05c54b75517_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='595'></svg>)
以及攻克无人驾驶技术的同母姐妹公司[Waymo](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247495475%26idx%3D5%26sn%3D300fbc1487fc9d5e97499bbf18012f80%26chksm%3De8d05841dfa7d157840863e82d9e7697490687bb02e50e621fa75d9c0ad5ba49392b2015a57a%26scene%3D21%23wechat_redirect)。Waymo也运用了AutoML技术提升无人车的视觉识别系统。

“但以AI技术研究这个发展速度，完全可以拓展谷歌从来没有涉足的领域，**开辟新的业务线**。”Dean给连线记者透露说，“关注人类健康，就是谷歌密切留意的一个大方向。”

具体细节，Dean没有透露。

但是仔细翻翻谷歌研究博客（Google Research Blog），可以大概看出他们**强势跨界医疗**的野心。

比如说，早在16年，谷歌就在印度测试了一款医用软件，用深度学习算法扫描眼睛得到视网膜图像，从而判断糖尿病患者的视网膜是否发生了病变。
![](https://pic3.zhimg.com/v2-f974a01927258701ef8d73661a09edfa_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='463' height='342'></svg>)
再比如说，昨天才发布的[AR显微镜](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247497110%26idx%3D5%26sn%3D1273f5577e5ead4c0afb155c696db34d%26chksm%3De8d042e4dfa7cbf21da549bf113f1a7057481958d1ca4aa7b4e4e19d99e835917220fc73eaed%26scene%3D21%23wechat_redirect)，检测出活检切片中的乳腺癌细胞，用时只需0.0758秒。 
![](https://pic1.zhimg.com/v2-4258f924134d435dbb3346ac0bde5128_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='635' height='393'></svg>)
除了加速AI技术落地医疗方面的应用，Dean还提到了谷歌更长远的计划，是通过自动化AI技术让机器人拥有尽可能多的技能。

最理想的通用机器人，应该是能做到在完全陌生的场景下，也能麻利儿地分析解决问题。

不过，成长速度快到惊人的AI技术超出了人们认知边界。AI能力越强，人们越恐惧。

谷歌作为科技巨头公司，肩负着大众对规范AI技术的期望，也被置于一个非常敏感的道德边缘。

本月初，就有[3千多名谷歌的员工联名上书](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247496583%26idx%3D1%26sn%3D8fb11bc2e3e67c7786ce24aeda8220c3%26chksm%3De8d044f5dfa7cde3de596367c32e0a86f58274357ca184110e2e05f5452f8396121452ce00c3%26scene%3D21%23wechat_redirect)谷歌CEO劈柴，表明强烈反对谷歌参与五角大楼的战争技术研发项目。即使是分析无人机的视频片段也不行。
![](https://pic3.zhimg.com/v2-e35f829b0f1cdcf832799c1f9faea0b6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='312'></svg>)
Dean拒绝置评此事。不过他认为，此事更大的意义在于，谷歌收到了一个信号，明白了人们非常在意AI未来的道德约束问题。但这个军事Maven项目只是整合了现有的开源技术，本身非常平平无奇。

> “借这次事件，谷歌全体员工也开始重新思考公司的定位和责任。”

最后，传送一个彩蛋：

Quora上的「谷歌大神Jeff Dean有多牛」合辑。

[https://www.quora.com/What-are-all-the-Jeff-Dean-facts](https://link.zhihu.com/?target=https%3A//www.quora.com/What-are-all-the-Jeff-Dean-facts)

估计只有coder才能全get到点。Happy hunting~


**你可能感兴趣**

[谷歌大脑2017总结（Jeff Dean执笔，干货满满，值得收藏）](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247493165%26idx%3D1%26sn%3Db2716aacc19d7b16f57ebf427c67a732%26chksm%3De8d0515fdfa7d849af1db2a84e13873b6eba4a1b73ac19002310f431a703b4ae0cacd68cd5e7%26scene%3D21%23wechat_redirect)

[第二弹！谷歌大脑2017总结下篇：Jeff Dean梳理6大领域研究](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247493195%26idx%3D1%26sn%3D37560b99915e66c709544faec5c5331c%26chksm%3De8d05139dfa7d82f6a9486bb0e307ac326f866f43288278bc2a6f8a73f188c46cbd3b875e3ad%26scene%3D21%23wechat_redirect)

[Google传奇Jeff Dean最新演讲：如何构建未来的机器学习芯片](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247491955%26idx%3D2%26sn%3Dc85b2af9fc0e84050c1df725fb4a89f7%26chksm%3De8d05601dfa7df17281c059a0173308d205b07314e511c1966bbb439f6f3151599371ebe95ec%26scene%3D21%23wechat_redirect)




