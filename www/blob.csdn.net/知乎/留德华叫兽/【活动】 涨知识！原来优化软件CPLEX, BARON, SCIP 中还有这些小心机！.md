# 【活动】 涨知识！原来优化软件CPLEX, BARON, SCIP 中还有这些小心机！ - 知乎
# 

> **文章作者：**[@会飞的蜗牛](https://www.zhihu.com/people/hui-fei-de-gua-niu-20-25/activities)
**责任编辑：**[@会飞的蜗牛](https://www.zhihu.com/people/hui-fei-de-gua-niu-20-25/activities)
文章发表于**微信公众号【运筹OR帷幄】：**[【活动】 涨知识！原来优化软件CPLEX, BARON, SCIP 中还有这些小心机！](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s/OmYUQO0RodcvPQdYAtkjng)
*欢迎原链接转发，转载请私信*[@留德华叫兽](https://www.zhihu.com/people/961e8cc4f7512fda1ea6626ce9a05e8e)*获取信息，盗版必究。*
敬请关注和扩散本专栏及同名公众号，会邀请**全球知名学者**发布运筹学、人工智能中优化理论等相关干货、[知乎Live](https://www.zhihu.com/lives/users/961e8cc4f7512fda1ea6626ce9a05e8e)及行业动态：[『运筹OR帷幄』大数据人工智能时代的运筹学](https://zhuanlan.zhihu.com/operations-research)

**运筹学爱好者**快点看过来👀在这里一定会有你所感兴趣的话题，让你茅塞顿开。

 这里还有**大牛经典语录**以及**学界、业界前沿动态**☝️定会让你满载而归。

**『运筹OR帷幄』建立的微信学术群**共计10个，共计3500+人，群成员为全球华人Operations Research, Optimization, O.M., I.E., SCM，Data Science，Machine Learning， Computer Vision等方向的researchers，包括常青藤教授，各大Top互联网公司大佬，世界各地知名大学OR相关的硕博，**加群方式详见文章底部**。

** 【活动】版块急需一至两名志愿者，如果你对微信号运营有兴趣，或者想加入我们增长相关经验，欢迎加入我们。具体可以在微信公众号后台回复：“志愿者”关键词获取详细信息，申请时请注明【活动】版块**

　　 我们将开通OR/AI下面更细分领域的学术群，鼓励相近领域的运筹学爱好者深度交流。上期文章我们进行了新开微信群的投票活动，下面让我们一起来看一下投票结果吧！ 
![](https://pic3.zhimg.com/v2-58bb75e1e59b45403cb3b7f9f5bf0f56_b.jpg)

根据投票结果，我们将增加开通**数据科学｜挖掘** 和 **混合整数规划** 的微信学术群，如果你是运筹学|人工智能硕博或在读，请在**公众号后台**留言：“**加微信群**”。系统会进一步提示，邀请您进全球运筹或AI学者群（群内学界、业界大佬云集）。

同时，也欢迎大家继续在文末留言，说出你最想开设的领域的微信群~

 温馨提示：我们还有有【运筹|优化】【供应链|物流】【人工智能】【数据科学|分析】等运筹学|人工智能爱好者千人QQ群，请关注公众号点击“加入社区”按钮，获得入群传送门。

下面就来看看我们第六期是整理了哪些**话题**吧.

** 往期微信学术群群聊精华语录传送门**

**第一期  [遗传算法 & 动态规划](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzUxMTYwMzI0OQ%3D%3D%26mid%3D2247486158%26idx%3D1%26sn%3D2f1ef9e482481383d89013eccd865a11%26scene%3D21%23wechat_redirect)**

**第二期****[锥优化&运筹学入门教材](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzUxMTYwMzI0OQ%3D%3D%26mid%3D2247486209%26idx%3D1%26sn%3Dfbde365904858019bd31be55e28a2271%26scene%3D21%23wechat_redirect)**

**第三期****[航路路径优化算法 &线性规划](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzUxMTYwMzI0OQ%3D%3D%26mid%3D2247486251%26idx%3D1%26sn%3D0138eee67e6a0c0408932e5de7fbbc3b%26scene%3D21%23wechat_redirect)**

**第四期****[传统统计中的方法 VS](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzUxMTYwMzI0OQ%3D%3D%26mid%3D2247486328%26idx%3D1%26sn%3D6dad0f39029e702bc37942b8e5e9c157%26chksm%3Df9706c26ce07e5300d3baf858207361609ddde2ff42ec4e99c60889bfdd0bff378388e591825%26mpshare%3D1%26scene%3D21%26srcid%3D0906Rd1vOaddDD5n2qQ8xKv9%26pass_ticket%3D6gRHxs8VeIMSJQNyut2fCjD9ai69pXh9uCECc4YCXh0A1Oyd9c2J0Mtl/sIriVmK%23wechat_redirect)[机器学习](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzUxMTYwMzI0OQ%3D%3D%26mid%3D2247486328%26idx%3D1%26sn%3D6dad0f39029e702bc37942b8e5e9c157%26chksm%3Df9706c26ce07e5300d3baf858207361609ddde2ff42ec4e99c60889bfdd0bff378388e591825%26mpshare%3D1%26scene%3D21%26srcid%3D0906Rd1vOaddDD5n2qQ8xKv9%26pass_ticket%3D6gRHxs8VeIMSJQNyut2fCjD9ai69pXh9uCECc4YCXh0A1Oyd9c2J0Mtl/sIriVmK%23wechat_redirect)**

**第五期****[列生成算法，遗传算法，粒子](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzUxMTYwMzI0OQ%3D%3D%26mid%3D2247486413%26idx%3D1%26sn%3D482274946583b2854ee5390ce1e2a786%26scene%3D21%23wechat_redirect)[群](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzUxMTYwMzI0OQ%3D%3D%26mid%3D2247486413%26idx%3D1%26sn%3D482274946583b2854ee5390ce1e2a786%26scene%3D21%23wechat_redirect)[算法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzUxMTYwMzI0OQ%3D%3D%26mid%3D2247486413%26idx%3D1%26sn%3D482274946583b2854ee5390ce1e2a786%26scene%3D21%23wechat_redirect)**

## **2018年8月22日    Baron求解过大规模非线性问题误差**

**（出自微信群： 【3】Global O.R./OM/IE Community）**

**北-敬一-计算机系统生物学：**请问有谁用过Baron解过大规模非线性问题？我发现Baron得出来的解的误差明显大于设定的约束容差（feasibility tolerance)，我的是一个混合整数双线性问题，两三万个连续变量，十几个整数变量，十几个非线性约束。MATLAB/Baron界面，Baron版本均为最新。

**留-海德堡-组合优化AI：**tolerance指的是MIP Gap，还是某个约束的tolerance呢。Baron没用过，向您学习一下。

**北-敬一-计算机系统生物学：**不是，我指的是对于一个约束的容差，就是你把解代入一个约束，得出来的值和约束的lhs，rhs（约束不等式两边的常数）的差值。        
![](https://pic2.zhimg.com/v2-6338f2288fddbdd3dac181f5af761f69_b.jpg)
**北-敬一-计算机系统生物学：**其实我将这个问题问过Baron的开发者，但他也不知道为什么会这样。

**留-海德堡-组合优化AI：**试着改改这个tolerance，再跑跑。另外可以再试试其他minlp solver，比如Couenne。

**北-敬一-计算机系统生物学：**Tolerance 为10e-7或10e-6，有些约束的误差可以达到高于10e2，

目前只有Baron能求全局最优吧，还能怎么改容差差，我本来设定的容差就不高，对于非线性问题好像只有Baron能全局最优吧。

**留-海德堡-组合优化AI：**大家多读gurobi或者cplex关于分布式算法的文档吧，里面写的很清晰。minlp只是试图全局最优吧?nonconvex的时候，u never know。

## **2018年8月27日   　　　海德堡大学Panda**

**（出自微信群： Global O.R.optim PhD Community）**

**王-东北大学-动态优化：**Panda 是 Porta 的新版本 有用过Porta或者 Panda的小伙伴可以交流一下。

**留-海德堡-组合优化AI：**我在美国做硕士论文求convex hull所有facets的时候还用过Porta，去了海德堡才意识到原来是这里出品的。

**王-东北大学-动态优化：**最近有一篇论文 可以找到0-1混合整数规划凸包的一般描述，需要借助Porta软件来实现。不过貌似Porta的算法是暴力计算，决策变量维数超过30，计算时间就超长。所以我在考虑它的新版本Panda。

**留-海德堡-组合优化AI：**类似的还有polymake，还有一个Magdeburg大学出品的软件，忘了名字了。

**ZHOU-Univ of kentucky：**其他的软件还有parma polyhedra library, cddlib, lrslib, 以及新出的Normaliz。

**华-统计-交通优化：**panda是python的库？

**Yin-SMU-Power Engineering Phd：**这俩不是一个东西，py的库叫pandas。

## **2018年8月28日****IBM CPLEX 求解binary变量做post processing**

**（出自微信群： 【2】Global O.R./OM/IE Community）**

**Zelene-大连理工-优化：**求教各位前辈，我在用IBM ILOG CPLEX IDE求解一个混合整数规划模型时，把其中的一个二维决策变量定义为 float+ 类型，但在最后求得的结果里，该变量还是出现了负数值，请问这是为什么呢？如何可以修正？

**留-海德堡-组合优化AI：**决策变量不应该是binary么

**Zelene-大连理工-优化：** 我求解的是一个混合整数规划模型，里面有部分变量是连续型的，其中有一个要求是大于零的实数，所以我按照opl建模语言教程把它定义为float+。但不知为何求出来还是有小于零的值。

**李-南航大-机场交通优化：**那是近似解，有Gap，小于0的值应该很小，接近于0.binary是0-1变量类型的决策变量，但好多VRP问题会有binary类型变量和float+类变量。

**留-海德堡-组合优化AI：** 科学计算都有numerical error。可以做一下post processing～例如binary变量，解出来很有可能是+-0.0000000001，需要做一下post processing，加个判断语句，把这些变成0。

## **2018年8月29日  　优化软件 CPLEX & GUROBI &SCIP**

**（出自微信群： Global O.R./OM/IE Community）**

**李-西南交大-运输优化系统：**想问下我在用yalmip求解的时候，cplex和gurobi都能找到，可是为啥scip一直都是Solver not found，明明这三个软件我都装了~

**小千-东普-数据分析：**试试重新安装一下，可能是注册表信息损坏。

**李-西南交大-运输优化系统：**嗯嗯，就是重新装了两遍~，还把原来旧的5.0.1卸载了，装了新的6.0.0。然后我用path在matlab里面搜，确实没看到scip的path，cplex和gurobi的path都有，难道是scip没写matlab的接口？我确实在scip的安装文件夹里没找到跟matlab相关的，装完具体的solver软件还需要在matlab里面添加路径吗，我有点忘记了我之前装cplex和gurobi时候的了~
![](https://pic2.zhimg.com/v2-42352ef3410fdb10337b3ecf15810489_b.jpg)

**杨-西南交大-运筹优化：**需要添加路径。

**李-西南交大-运输优化系统：**可是SCIP安装文件夹里面没有m文件。

**杨-西南交大-运筹优化：**用yalmiptest 命令看看对应的solver是否起作用。

**李-西南交大-运输优化系统：**添加了一堆空文件夹，yalmiptest查过了，SCIP就是 solver not found。
![](https://pic3.zhimg.com/v2-be7629c842f209aa8b8ef0e73efdbec6_b.jpg)
**杨-西南交大-运筹优化：**没用过scip，之前折腾过没成功。

**李-西南交大-运输优化系统：**那yalmip官网上都是骗人的，说可以支持scip，我之前折腾scip也没成功，现在又要用还是不行~我去看看SCIP主页上有没有说相关情况。

**杨-西南交大-运筹优化：**可以发邮件问问Johan。

**李-西南交大-运输优化系统：**哈哈，有道理！~可以试试，说不定下个release他就修复这个bug了

**杨-西南交大-运筹优化：**Scip对mip模型的效率没有gurobi和cplex高。

**李-西南交大-运输优化系统：**我主要看重SCIP能解nonlinea。但其实我感觉应该SCIP的问题，它安装文件里没写个m文件接入matlab，我看gurobi和cplex都有。

**杨-西南交大-运筹优化：**我用这两个都没问题。

**李-西南交大-运输优化系统：**嗯嗯，我也是，cplex和gurobi都没问题，但是这俩好像解不了nonlinear。

**王-巴黎六大-运筹：**有接口告诉我哈 我两年前试过 没找到接口

**杨-西南交大-运筹优化：** 我之前scip没弄成功啊。

**伍-杉数科技：**[https://www.inverseproblem.co.nz/OPTI/](https://link.zhihu.com/?target=https%3A//www.inverseproblem.co.nz/OPTI/) 这个的学术版提供了来布接口。

**李-西南交大-运输优化系统：**

![](https://pic4.zhimg.com/v2-d201378941859e8662f38b03b21b825b_b.jpg)
**伍-杉数科技：**yalmip应该是依赖这个工具包。

**李-西南交大-运输优化系统：**我好像看到了SCIP官网上说接口要自己整，附了code。

**伍-杉数科技：**
![](https://pic1.zhimg.com/v2-977d831d25e12a62fd664ee990cf8d70_b.jpg)
**王-巴黎六大-运筹：**这个只能windows用，其他平台不能用的 小心哈。

**伍-杉数科技：**我不用来布，opti的作者应该是提供了接口的源。

**李-西南交大-运输优化系统：**我先看看SCIP的官网的，它好像附了code

**伍-杉数科技：**可以自行调整编译，确实是依赖这个库。
![](https://pic4.zhimg.com/v2-999d1fc05655a73c433e1726e9dc2b1b_b.jpg)
**王-巴黎六大-运筹：**Opti是windows接口 其他都不能用。

**伍-杉数科技：**[https://github.com/jonathancurrie/OPTI/tree/master/Solvers/Source/scip](https://link.zhihu.com/?target=https%3A//github.com/jonathancurrie/OPTI/tree/master/Solvers/Source/scip)，这是接口源码，我想有源码可以调整调整，自己编译一下，不是什么难事

**李-西南交大-运输优化系统：**恩，我就是在windows的~谢谢，我瞅瞅~SCIP官方在GitHub上的代码已经放弃matlab了。。。

**伍-杉数科技：**这是opti的作者写的。

**李-西南交大-运输优化系统：**没放matlab的，SCIP Interfaces · GitHub，[https://github.com/SCIP-Interfaces](https://link.zhihu.com/?target=https%3A//github.com/SCIP-Interfaces)。
![](https://pic2.zhimg.com/v2-c1b223fef0319aacbc8e3f0e6c521e79_b.jpg)
**王-巴黎六大-运筹：**没必要scip 好多替代的solver啊。

**李-西南交大-运输优化系统：**我刚刚看的scip官方的，除了lingo，其他解nonlinear的求解器我感觉我都用不了。scip是唯一开源的了。

**王-巴黎六大-运筹：**Ipopt

**李-西南交大-运输优化系统：**我的是minlp，就这几个里面选，Baron下来应该就到scip了。        
![](https://pic3.zhimg.com/v2-9b79cec03a2df52323113257b4672b32_b.jpg)
**伍-杉数科技：**couenne目前只有ampl接口，opti也支持，当然也可以pyomo调用。

**王-巴黎六大-运筹：**Jump不支持吗？

**伍-杉数科技：**支持，也是hook的nl那套。以ampl接口为原型。

**李-西南交大-运输优化系统：**不过你说yalmip也用的OPTI Toolbox，为啥yalmip识别不出SCIP。        
![](https://pic3.zhimg.com/v2-032eb879b46ae27c8c616d94dc1db056_b.jpg)
**伍-杉数科技：**不用来布，不用yalmip。

**李-西南交大-运输优化系统：**果然是要用它的code，不过不知道为什么只能识别linear，却识别不了nonlinear，应该还是它的bug，更新过一次patch的，
![](https://pic3.zhimg.com/v2-e1e2c99a27ba5c2cb7b23868f948a582_b.jpg)

> **文章来源申明：**本篇文章由『运筹OR帷幄』活动版块原创发起，未经授权，禁止转载。

可以在 **本公众号后台 **回复关键词：“**活动**”获取往届活动文章精选，如果觉得有用， 请勿吝啬你的留言和赞哦！~

**【活动】**版块急需一至两名志愿者，如果你对微信号运营有兴趣，或者想加入我们增长相关经验，欢迎加入我们。

**『运筹OR帷幄』**长期招收志愿者，如果你有一技之长，爱分享，欢迎加入我们。点**击下方图片**即可了解详情！（如对活动版块感兴趣，可以备注活动版块哦！）

扫二维码关注『运筹OR帷幄』公众号：
![](https://pic1.zhimg.com/v2-872dc2d2c5eaf08b38d693490549136c_b.jpg)
[点击查看『运筹OR帷幄』志愿者招募介绍及加入方式](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzUxMTYwMzI0OQ%3D%3D%26mid%3D2247486166%26idx%3D1%26sn%3Da598e1475ae4f91afb7c33114ee61a85%26chksm%3Df9706d88ce07e49ec590fc2589d251bbf2b7d605cd127b1a0bdff61b8bad985eddf42365d823%26scene%3D21%23wechat_redirect) ：
![](https://pic1.zhimg.com/v2-d1fbc7022dc62969dcf8a6afdacdfcb4_b.jpg)
[点击查看【骥寻伯乐】板块说明及投稿方式](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzUxMTYwMzI0OQ%3D%3D%26mid%3D2247486137%26idx%3D2%26sn%3D24e9487d15a8c590645a4d83950c8f20%26chksm%3Df9706de7ce07e4f1507edd01511920bd8f0a3a80384aef8ee8768b68931f872bc6cff5d3485f%26scene%3D21%23wechat_redirect) ：
![](https://pic3.zhimg.com/v2-88ba2272456e93bb2f6b9bf2d55ef1d2_b.jpg)



