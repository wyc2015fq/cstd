# 无人零售场景、技术全解读：伪需求or真风口？ - 人工智能学家 - CSDN博客
2017年10月09日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：371
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUdJ7WcPdZebaL3xHRDOFcoRX2z4WAenqqTQE3Mu4icnreUgicJLYciciaPLiaIOcpeL1FIhzIWjVvicR7g/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
*来源：智东西*
*概要：无人零售是不是伪需求，相关技术成熟度怎么样，行业竞争的本质是在拼什么。*
无人零售，作为新零售的热点概念，采用计算机视觉、生物识别、智能算法等技术，覆盖机器人制造、支付设备制造、高端装备制造等产业，颠覆了传统零售业的服务和供应链模式，用更为信息化、智能化的方式降低人力成本，提高运营和管理效率。
## **伪需求 or 真风口**
从市场渗透率来看，最近一年，在线下商超消费过的用户占比高达93.4%，通过线上零售渠道进行过消费的用户占比79.5%，而通过无人零售（自动售货机、无人零售店/便利店、开放货架等）进行过消费的用户仅占36.5%。
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUdJ7WcPdZebaL3xHRDOFco55HQCY6Ng1FyrR0EGwBImFgicNMMMSKykXCqkmcnbCqJVUZWQoLsfZw/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
**2017年中国零售用户消费场景分布*
从数据上来看，无人零售在零售业整体市场中的渗透率比较低，细分项的表现来看：
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUdJ7WcPdZebaL3xHRDOFcoXMwwE3vO2nPcibkECgEuiauRa4Urvu51P573SWYEFdBV8Ase24eicz1ag/0?wx_fmt=png)
**2017年中国自动售货机使用情况及分布场所*
1、自动售货机（饮料售货机、综合机、便利柜、自动咖啡机、智能售饭机）是无人零售中布局最早，也是用户占比最多（29.5%）的，但部分品类投放数量还不多，用户习惯还待养成；
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUdJ7WcPdZebaL3xHRDOFcoMoibYadkVrpnyG2qjPyVqiaDBAel6dhGLqjriajHiaXuJP2gce5LOpLUzA/0?wx_fmt=png)
**2017年无人店基本情况**
2、无人零售店/便利店（16.5%）概念比较火，最早因亚马逊的Amazon Go无人商店引起关注，今年7月的第二届淘宝造物节上，马云推出了占地达200平的线下实体店“淘咖啡”，集商品购物、餐饮于一身。目前，受技术、空间等因素影响，无人店很难售卖全品类商品（只能以零食饮料等标品及便利性应急商品为主，部分无人店还提供鲜食、生鲜食品、餐饮等），最为适合的场景为社区便利店。
*![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUdJ7WcPdZebaL3xHRDOFcoF9vbqTaKaJ7TtpfUezd9UoLUlhAibVBTvsySd3Jc1oOpapZOeo0yib5w/0?wx_fmt=png)*
**2017年中国开放货架获投企业一览表*
3、开放货架（使用率16.4%）多定位办公室休闲食品消费场景，目前全国覆盖率尚不高，技术壁垒低，丢损率相对较高，线下拓展严重依赖地推，需要烧钱培养用户习惯抢占市场，运营难度较大，但被认为是最接近消费者的一种模式：中商产业研究院数据显示，截止到9月末，已经有至少16家无人货架获得投资，最高达到3.3亿元，融资总额超过25亿元。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUdJ7WcPdZebaL3xHRDOFcotCP74iaxjkEvjzabQQV6QxAl67QAD9gCUGJk0ia2gO2xTgkBrLW6UpJw/0?wx_fmt=png)
**2017年中国无人零售业态分类*
从消费动机来看，用户第一次选择在无人店购物的原因各不相同，35.0%的用户是由于无人店能提供24小时服务而选择无人店进行消费。高达83.6%的客户多次选择在无人零售店消费的原因是由于无人店方便快捷、能够节省购物时间，方便快捷是用户选择无人店消费的最重要原因。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUdJ7WcPdZebaL3xHRDOFcosdfxWQKePBhXtnxWnThlOTrD3U0JAj3Oa1ZqbHmfaAWOezm7NZDJRA/0?wx_fmt=png)
**无人店消费动机*
方便快捷既是用户选择无人店的原因，也是无人店商商业模式易于扩展的主要因素之一：就拿瑞典公司Wheelys的环保主题“移动咖啡馆”连锁品牌来说，最初只有3个雇员，却被Y Combinator看中，在6个月不到的时间里建立起了全球（50多个国家和地区）销售网络。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUdJ7WcPdZebaL3xHRDOFco6gbyB5b7NZJOwic0p62NtUIKiatEjybVZxoAgzlVIenygxhBnYFSrNlg/0?wx_fmt=png)
**2017年中国无人店用户购买和希望购买的商品品类分布*
从商品种类来看，目前消费者在无人店购买最多的商品品类是饮料与零食，早餐与零食是无人店用户消费最主要的场景，未来可能会需要突发的个人物品和家庭物品补给的需求。
在无人店消费过的用户中，70.9%的用户表示当前无人店已满足自己的购物需求，未满足的原因主要包括商品种类少、品牌少，商品有破损或质量不合格。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUdJ7WcPdZebaL3xHRDOFcoU87UV2KezHtVHaM0GaFFGQWncGiaOaW1Hx09Gc0iccg01q6UsJm4YRog/0?wx_fmt=png)
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUdJ7WcPdZebaL3xHRDOFcoyiaXyAqn0rNs7WsZGzw4cLxUoA4EdsibwAMeyO68jAdo4PIXXibLgw4BQ/0?wx_fmt=png)
## **三大技术解读**
### **身份认证与顾客追踪**
身份认证或者说顾客追踪，是指对进入无人店的消费者进行识别和追踪，有利于提取相关信息，预测消费行为，降低商品损坏、丢失率。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUdJ7WcPdZebaL3xHRDOFcoMibhn3BusUicvWibsJhNl5FLh8ibOm7540vOShv8T9jaHZYBiaxnetexW2w/0?wx_fmt=png)
**Amazon Go顾客追踪系统*
亚马逊Amazon Go的技术方案是利用监控系统、麦克风捕捉、GPS 以及WIFI信号定位。当消费者扫码进入超市后，监控系统就会认出“他”是谁并一路“跟踪”，店内麦克风会根据周围环境声音判断消费者所处的位置。此外，用户手机的 GPS 以及 WIFI 信号亦能协助定位的实现。当“他”站在货架前准备购物时，货架上的相机系统便会启动，拍下“他”拿取了什么商品和离开货架时手中有什么商品。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUdJ7WcPdZebaL3xHRDOFcoJfoSA5sFt5tesKicav0QrRic5lB8NRBcU1eQrglqibMCZh7yGUs6QicuMQ/0?wx_fmt=png)
**淘咖啡身份确认技术*
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUdJ7WcPdZebaL3xHRDOFcolLgfgDRtptaWsNIXAC2F8KCyNcJd7TuzHZ7snzf4ic33Bo486oyZllQ/0?wx_fmt=png)
**淘咖啡顾客追踪技术*
阿里“淘咖啡”的技术方案是基于生物特征自主感知和学习系统的身份确认（识别率是0.02%）和基于目标检测与追踪系统的顾客追踪。生物特征自主感知和学习系统，用于解决在开放空间里对消费者身份的识别问题，将顾客生物特征与淘宝 ID 进行绑定，以实现对顾客的身份确认。目标检测与追踪系统主要是追踪消费者在店内的行为及运动轨迹，该功能主要依赖多路监控摄像头。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUdJ7WcPdZebaL3xHRDOFcoreVZwxUBdwC3e3AfaDVNjQQVZO9DlDDIRiaW9386xWiavOhNuFcZeevw/0?wx_fmt=png)
**Take Go身份确认技术*
深兰科技Take Go的技术方案是生物识别技术（静脉识别器）。顾客进入Take Go无人店需要手掌按在生物识别读写器上，这个识别器不是掌纹或者指纹识别器，应该是静脉识别器，静脉识别技术要
比指纹识别精确很多，也更大程度地避免了被冒用的可能性，很好地确保了用户资金的安全性。Take Go还在顾客追踪方面引入了卷积神经网络技术，用于对整个无人零售店内物品的监测、识别与跟踪。
缤果盒子也是通过手机扫码来确认身份，与便利蜂相同，顾客进入商店时需要扫描二维码（目前只支持实名认证的微信扫描）。当顾客进入商店后无需追踪，但内置了全视角视频监控，可有效震慑顾客在店内的作弊行为（比如破坏商品、撕毁 RFID 标签等）。
### **商品识别**
商品识别主要涉及计算机视觉，是对货架上商品信息变更的识别。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUdJ7WcPdZebaL3xHRDOFcoQO2icNPLV7sKeCLj8zg5ia7VgMBoh50HH6WdUf9GtnMAqEddG9X5ibWWw/0?wx_fmt=png)
**Amazon Go商品识别技术*
亚马逊Amazon Go的技术方案是利用感测融合技术，通过手势识别、红外传感器、压力感应装置、荷载传感器来判断用户取走了哪些商品以及放回了多少商品。同时，这些数据会实时传输给 Amazon Go 商店的信息中枢，每位顾客都不会有延迟。当放回的商品与原本位置不一致时（通过图像识别该位置与现有商品不一致时）Amazon Go会提醒工作人员将商品放回正确的位置。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUdJ7WcPdZebaL3xHRDOFcoZWSvrrFHZKJNW70Bdic8dq64WEt99iaasaR19O8RiaNUbGuktSN07qFJw/0?wx_fmt=png)
**淘咖啡商品识别技术*
阿里“淘咖啡”的技术方案采用了结算意图识别和交易系统。顾客需经过两道结算门，对商品的识别过程就是在这两道门之间完成（误别率0.1%）。有分析认为这套系统利用了RFID技术（一说机器视觉）。据工程师内测，把商品放进书包里、塞进裤兜里，多人拥挤在一个货柜前抢爆款，戴墨镜；戴墨镜＋戴帽子等行为下，系统基本都能识别，并自动扣款。
深兰科技Take Go的技术方案是通过机器学习，将该商品的一些特征数据信息记录到数据库中。当顾客走进Take Go 无人零售门店并拿起商品时，不管商品的位置是在顾客手上、怀中、口袋还是背包内，系统都能监测与识别。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUdJ7WcPdZebaL3xHRDOFco22kksKRSJj2zcQA6KxjHn6vdIGIiciaPficSUQsDgKd6LKpr2U010sfvg/0?wx_fmt=png)
**“小范 FAN AI”示意*
缤果盒子一开始采用了 RFID技术（店内商品包装上皆贴有 RFID 标签，内置的全视角视频监控），收银台检测区可对 RFID 标签进行识别，近期推出AI解决方案“小范 FAN AI”，图像识别技术取代了此前广泛使用的RFID电子标签，可以节省商品贴标签的人工和成本，并且改造成本极低。
### **支付技术**
约四成的无人店用户希望无人店付款时能够拿了就走，系统自动付款结算。三成用户希望通过自助扫码线上结算的方式来付款，仅有8.5%的用户还是希望有店员的协助。目前，“拿了就走”对技术水平要求较高。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUdJ7WcPdZebaL3xHRDOFcoPhVgZygTE5Iqb5KS9ic22p6GH6nFYxl3oiaOiaMiakhoQfyOibJQummkhvg/0?wx_fmt=png)
**2017年中国用户对无人店付款流程的期待*
亚马逊Amazon Go 所采用的支付手段为自动更新清单、自动结算。用户手机里的系统（该系统与 Amazon Go 商店的信息中枢无延迟地同步进行更新）会自动更新清单，在离店时，系统会自动在消费者的账户上结算金额。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUdJ7WcPdZebaL3xHRDOFcoteuy4H3z973SxiczFAZgAd3uwMwqPic66QLrpAUpUXbNg3kcpc5XLoQQ/0?wx_fmt=png)
**Amazon Go支付技术*
阿里淘咖啡采用的支付手段也是自动结算。当感应到顾客即将离店的信息时，结算门将会自动开启。当走到第二道门时，屏幕会显示“商品正在识别中”，并马上再显示“商品正在支付中”，自动扣款，结算完成后，会有语音提示顾客此次购物所花金额，随即第二道门便会自动打开，顾客离店。
同样，深蓝科技Take Go的顾客离开商店时会收到对应的账单，并被系统自动扣款。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUdJ7WcPdZebaL3xHRDOFcovTa0YARkSKamvrGzAG534OnZy3ZWFDq7iaEpYsDBMS4icVwkrxoyUnsw/0?wx_fmt=png)
**缤果盒子支付技术*
而缤果盒子采用的支付手段则为扫码支付，用户在商店将商品整齐放置于收银台检测区（新收银台通过图像识别、超声波、传感器等多重交叉验证，准确率超过99%），然后扫描显示屏上的收费二维码。便利蜂的支付手段也是在线支付+扫描支付凭证。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUdJ7WcPdZebaL3xHRDOFco2TicibSgUGVojMQ1orxCuy1eibjzvC7smmHibtNzZ2kfIxIBiabjQfTVNng/0?wx_fmt=png)
**各类无人零售流程比较*
综上，无人零售的技术方案主要分为人工智能、物联网和互联网三个流派。
其中，人工智能流派以Amazon Go、阿里淘咖啡、Take Go、缤果盒子为代表，技术主要包括机器视觉、深度学习算法、传感器融合技术、卷积神经网络、生物识别等，具备购物体验较好、能实现即拿即走、更有助于了解消费者需求等优势，但技术壁垒较高，且较多的顾客拥挤在一个区域时会对系统 GPU 形成高负荷，且定位精度、多商品种类识别率限制了实用性；
物联网流派的主要代表为缤果盒子、7-11 、罗森日本无人店，技术主要是 RFID 标签技术，在对货物的识别与防盗上更具优势，技术上也较为成熟，但RFID标签费用较高、且受气温影响较大。
互联网的主要代表为便利蜂、小e微店，主要是利用二维码来完成对货物的识别。从技术上来讲难度较低，成本也较为低廉，但购物体验较差，过程较为繁琐。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUdJ7WcPdZebaL3xHRDOFcoMjknDp1rW9Ra0qqQfNic7V8z0j4z1QlIqvo26OtvCAL9mtza0QdP8xw/0?wx_fmt=png)
## **供应链竞争和资本游戏**
除了技术壁垒，对于无人零售玩家而言，供应链和资本是两个主要实力。
除了前端营销能力，无人零售的核心还包括后端供应链效率的提升，即利用数字化链接场景、用户和商家（零售商与供货商），考验的是业对信息流、物流、资金流的控制。亚马逊、阿里自不用说，Take Go的背后（合作）有娃哈哈和京东新通路，缤果盒子的背后是（投资方）欧尚中国与大润发背后母公司高鑫零售。
资本方面，VC青睐于无人货架，而传统零售企业以及电商则更注重无人店的战略布局。成熟的供应链、较强的产品研发能力和相对充足的资金储备，推动着他们视无人零售新的增长点和数字化转型机遇。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUdJ7WcPdZebaL3xHRDOFcoibAsvnroHRicyjFFicjrKibuZGyQ2D4mdRCGDKd0lA44zbA8ohfErVYObQ/0?wx_fmt=png)
**无人零售融资情况*
作者认为，无人店的快捷方便、品牌市场的空白状态、线下零售的巨大诱惑和人力资本痛点等，是企业和资本进入该领域的主要原因。对于亚马逊、阿里巴巴来说，无人零售除了是业务拓展尝试之外，也是其先进技术实力的体验/展示平台。但无人零售面临着识别、定位、商品易丢损等问题，以及RFID标签、系统智能化升级所需的成本考量，以及来自供应链端的信息流、物流、资金流竞争，使得无人零售增添了一丝丝资本博弈的味道，这就导致最终它是否能实现方便快捷的同时带来高性价比，成为未知。
