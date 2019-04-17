# 搜索和推荐中的精度和召回(recall)分别是什么意思？ - 知乎
# 



●今日面试题分享●




搜索和推荐中的精度和召回(recall)分别是什么意思？




解析：




精度/精确率，和召回率是广泛用于信息检索和统计学分类领域的两个度量值，用来评价结果的质量。




其中精度是检索出相关文档数与检索出的文档总数的比率，衡量的是检索系统的查准率；

召回率是指检索出的相关文档数和文档库中所有的相关文档数的比率，衡量的是检索系统的查全率。




一般来说，Precision就是检索出来的条目（比如：文档、网页等）有多少是准确的，Recall就是所有准确的条目有多少被检索出来了。




正确率、召回率和 F 值是在鱼龙混杂的环境中，选出目标的重要评价指标。不妨看看这些指标的定义先：




    1. 精确率 = 提取出的正确信息条数 /  提取出的信息条数     

    2. 召回率 = 提取出的正确信息条数 /  样本中的信息条数    




顺便说一句，如果两者取值在0和1之间，数值越接近1，查准率或查全率就越高。比如定义：F值  = 正确率 * 召回率 * 2 / (正确率 + 召回率) （F 值即为正确率和召回率的调和平均值）










这就好比推荐系统根据你的喜好，推荐了10个商品，其中真正相关的是5个商品。在所有商品当中，相关的商品一共有20个，那么

k精度 = 5 / 10

k召回 = 5 / 20




咱们再看下先第二个例子。比如搜：北京大学，有三个网页被搜索到了：

a. 北京大学保安考上研究生

b. 北京互联网工作招聘

c. 大学生活是什么样的




其中只有a是被正确搜索到的，其他两个其实是和用户搜索词无关，而事实上数据库里还有这种网页：

d. 北大开学季

e. 未名湖的景色




这两个没被搜索到，但d、e和“北京大学”的相关度是超过b、c的，也就是应该被搜索（被召回）到的却没有显示在结果里，即：

精确率 =   (a)  /  (a + b + c)

召回率 =   (a)  /  (a + d + e)




不妨再看第三个例子：某池塘有1400条鲤鱼，300只虾，300只鳖。现在以捕鲤鱼为目的。撒一大网，逮着了700条鲤鱼，200只虾，100只鳖。那么，这些指标分别如下：




代表查准率的正确率 = 700 / (700 + 200 + 100) = 70%

代表查全率的召回率 = 700 / 1400 = 50%

F值 = 70% * 50% * 2 / (70% + 50%) = 58.3%




不妨看看如果把池子里的所有的鲤鱼、虾和鳖都一网打尽，这些指标又有何变化：




正确率 = 1400 / (1400 + 300 + 300) = 70%

召回率 = 1400 / 1400 = 100%

F值 = 70% * 100% * 2 / (70% + 100%) = 82.35%        




由此可见，正确率是评估捕获的成果中目标成果所占得比例；召回率，顾名思义，就是从关注领域中，召回目标类别的比例；而F值，则是综合这二者指标的评估指标，用于综合反映整体的指标。




当然希望检索结果Precision越高越好，同时Recall也越高越好，但事实上这两者在某些情况下有矛盾的。比如极端情况下，我们只搜索出了一个结果，且是准确的，那么Precision就是100%，但是Recall就很低；而如果我们把所有结果都返回，那么比如Recall是100%，但是Precision就会很低。因此在不同的场合中需要自己判断希望Precision比较高或是Recall比较高。如果是做实验研究，可以绘制Precision-Recall曲线来帮助分析。







**题目来源**：七月在线官网（[http://www.julyedu.com](https://link.zhihu.com/?target=http%3A//www.julyedu.com)）——面试题库——面试大题——推荐系统




●[金融风控面试十二问](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzI4MTQ2NjU5NA%3D%3D%26mid%3D2247489435%26idx%3D1%26sn%3D72d79ceb0c77757d9f665133ee983db6%26chksm%3Deba99416dcde1d0052f3515829ee99c7961515393ebb0ad381f8a70f60f4c8dd56a3a7b64ccd%26token%3D2024341789%26lang%3Dzh_CN%26scene%3D21%23wechat_redirect)


●[一次面试让你知道数据结构与算法对前端的重要性](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzI4MTQ2NjU5NA%3D%3D%26mid%3D2247489395%26idx%3D1%26sn%3D47a697484d6944ca208f8acacf141750%26chksm%3Deba994fedcde1de86a0e991fb7af7a2d37ed5d6de999813a80a2f518070980e553ae13ce28fd%26token%3D2024341789%26lang%3Dzh_CN%26scene%3D21%23wechat_redirect)

●[3句话告诉你薪资高到没朋友的数据科学职位到底是干啥的？](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzI4MTQ2NjU5NA%3D%3D%26mid%3D2247489392%26idx%3D1%26sn%3D67da7e6375d429ba4a07faad5f96ba59%26chksm%3Deba994fddcde1deb55725c5730d161e825097eca2c40f6e54dbe02833bb41fe80a7bf8c1e3d5%26token%3D2024341789%26lang%3Dzh_CN%26scene%3D21%23wechat_redirect)

●[一文详解：什么是B树？](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzI4MTQ2NjU5NA%3D%3D%26mid%3D2247489249%26idx%3D1%26sn%3D20bce3a19894f6afa6ca3e59b862e790%26scene%3D21%23wechat_redirect)

●[机器学习中的数学基础（微积分和概率统计）](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzI4MTQ2NjU5NA%3D%3D%26mid%3D2247489243%26idx%3D1%26sn%3Df46ad9a173125b1ad264cd6da92720fa%26scene%3D21%23wechat_redirect)

●[34个最优秀好用的Python开源框架](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzI4MTQ2NjU5NA%3D%3D%26mid%3D2247489145%26idx%3D1%26sn%3D02f71efed8bcf5257184f3514fe98d4e%26scene%3D21%23wechat_redirect)

●[【实战分享】电影推荐系统项目实战应用](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzI4MTQ2NjU5NA%3D%3D%26mid%3D2247489159%26idx%3D1%26sn%3D49853f5aed37e84f8006f8ab48ca4d8e%26scene%3D21%23wechat_redirect)

[●Python打牢基础，从19个语法开始！](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzI4MTQ2NjU5NA%3D%3D%26mid%3D2247489256%26idx%3D1%26sn%3Dc8fbeea42d3ff2e7a29fffa028375b4e%26chksm%3Deba99565dcde1c732f7ee31aba4f4832f44741d369ef9aa4a81fa141763d10ea854fe7278a02%26scene%3D21%23wechat_redirect)




关注V💗公众号：**七月在线实验室 **




后台回复：**100**   免费领取**【机器学习面试100题】**

后台回复：**干货 **免费领取**【全体系人工智能学习资料】**

**后台回复： 领资料****免费领取全套【NLP工程师必备干货资料】**






