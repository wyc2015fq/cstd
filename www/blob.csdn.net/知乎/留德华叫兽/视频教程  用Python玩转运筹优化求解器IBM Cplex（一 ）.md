# 视频教程 | 用Python玩转运筹优化求解器IBM Cplex（一 ） - 知乎
# 

![](https://pic4.zhimg.com/v2-a79055a98d76a6ac612c7cc81d4abb73_b.jpg)
**编者按：**

优化求解器对于做运筹学应用的学生来说，意义重大。

然而直到今天，放眼望去，全网（包括墙外）几乎没有一个系统的Cplex中文求解器教程。

作为华人运筹学的最大的社区，『运筹OR帷幄』 责无旁贷，我们决定承担这“吃力不讨好”的责任。

Cplex视频教程由『运筹OR帷幄』【视频】版块荣誉出品，主编留德华叫兽策划和监制，【视频】版块责编唐博编程并录制，初步设置16节课，希望能为大家扫盲数学规划求解器的编程基础，并且用实例带大家入门如今最火的人工智能、计算机视觉（底层即优化问题）。

> **文章作者：﻿﻿**[@留德华叫兽](https://www.zhihu.com/people/961e8cc4f7512fda1ea6626ce9a05e8e)**，『运筹OR帷幄』主编**
**视频录制、剪辑：唐博 ，『运筹OR帷幄』报道、视频版块责编**
**视频策划、监制：﻿﻿﻿**[@留德华叫兽](https://www.zhihu.com/people/961e8cc4f7512fda1ea6626ce9a05e8e)
文章发表于**微信公众号【运筹OR帷幄】：**[视频教程 | 用Python玩转运筹优化求解器IBM Cplex（一 ）](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s/k85S-vUATQix3yzXeS43HQ)
*欢迎原链接转发，转载请私信*[@运筹OR帷幄](https://www.zhihu.com/people/06894b555d0cf621aa210fc8d70cd16d)*获取信息，盗版必究。*
敬请关注和扩散本专栏及同名公众号，会邀请**全球知名学者**发布运筹学、人工智能中优化理论等相关干货、[知乎Live](https://www.zhihu.com/lives/users/961e8cc4f7512fda1ea6626ce9a05e8e)及行业动态：[『运筹OR帷幄』大数据人工智能时代的运筹学](https://zhuanlan.zhihu.com/operations-research)

## **01优化求解器概述**

当我们费劲千辛万苦对一个实际问题用数学规划建模以后，写成如下优化问题：
![](https://pic3.zhimg.com/v2-406000a350f1f0742473471469a4c1b6_b.jpg)
我们知道，数学建模完成后要求解具体的实例，下一步便需要设计相应算法，然后导入实际数据求解。

如果问题是线性规划，因为有相应的多项式时间算法（例如内点算法），设计并实现一下算法可能还不是那么的困难。

但是，如果问题是一个整数规划，其求解的基本算法是分支定界法：

留德华叫兽：[【学界】混合整数规划/离散优化的精确算法--分支定界法及优化求解器](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzg2MTA0NzA0Mw%3D%3D%26mid%3D2247487105%26idx%3D1%26sn%3Df43041a1228fbc1761e54d1760ddd772%26scene%3D21%23wechat_redirect)

那么我们是否还要亲自手撸一遍指数级复杂度的分支定界法呢？

几十年前，当市场上还不存在优化求解器的时候，很不幸的告诉你：需要！

在海德堡大学读博期间，我与欧洲运筹协会主席、巴塞罗那理工Elena Fernandez教授（[https://www.eio.upc.edu/en/homepages/elena](https://link.zhihu.com/?target=https%3A//www.eio.upc.edu/en/homepages/elena)）闲聊的时候，她就跟我诉苦当年她读博时期手撸分支定界算法的“悲惨”经历。（当时作为一名运筹学研究精确算法博士生的毕业难度/代码能力可想而知。）

而今，正因为有了优化求解器的存在，我们只需将以上整数规划模型的系数矩阵输入到优化求解器中，它就能够给我们快速求出最优解或可行解。

并且除了最基础的分支定界法，求解器通常还集成了各种“花式“的启发式和割平面算法，使得求解效率大大提高！

因此，运筹学博士生的毕业难度大大降低！

还是一头雾水？优化求解器到底是什么呢？

从编程的角度，大家可以把它理解为一个专门求解优化/整数规划模型的算法包（library），你可以用任何编程语言（C/C++、Java、Python）去调用这个包里的方程，只要你把你要求解的整数规划模型目标方程和系数矩阵输入进去（告诉它你要求解的具体问题），它就会给你求解出结果。

例如在Python环境下只需如下四步：

**1**导入CPLEX的包

import cplex

**2**初始化一个优化模型

prob = cplex.Cplex() # 此处的prob可以是任意变量名

**3**输入模型的目标函数、约束条件及变量系数

prob.objective.set_sense(...) # 设置目标方程的取值方向：求最大值/最小值

prob.variables.add(...) # 添加变量

prob.linear_constraints.add(...) # 添加约束条件

**4**直接调用优化求解器的求解方法(method)，一键求解

prob.solve()

## **02商业整数规划求解器**

**1. IBM ILOG Cplex**

由于欧盟玛丽居里项目的缘故，我曾经在意大利Blogna Cplex的developer之一Andrea Tramontani手底下“实习”了半年

网址：[http://www.ibm.com/products/ilog-cplex-optimization-studio](https://link.zhihu.com/?target=http%3A//www.ibm.com/products/ilog-cplex-optimization-studio)

支持模型：混合整数（平方）规划、Constraint programming

支持语言：C/C++、Java、Python、Matlab等

特点：支持Benders分解模块（仅此一家）、速度Top2

当前版本：12.8

**2. Gurobi**

网址：[http://www.gurobi.com](https://link.zhihu.com/?target=http%3A//www.gurobi.com)

支持模型：混合整数（平方）规划、Constraint programming

支持语言：C/C++、Java、R、Python、Matlab等

特点：速度Top2、价格最高

当前版本：8.0

**3. FICO Xpress**

网址：[http://www.fico.com/en/products/fico-xpress-optimization](https://link.zhihu.com/?target=http%3A//www.fico.com/en/products/fico-xpress-optimization)

支持模型：混合整数（非线性）规划、Constraint programming

特点：速度Top3，支持鲁棒优化

当前版本：8.5

更多关于优化求解器的介绍，请参见【运筹OR帷幄】之前的一篇推文：[【学界】运筹学数学规划|离散优化求解器大搜罗](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzg2MTA0NzA0Mw%3D%3D%26mid%3D2247486981%26idx%3D1%26sn%3D976d756a10bdad2e67c349245538733a%26scene%3D21%23wechat_redirect)

## **03Cplex视频教程（Python API）**

因为本科和硕士都是数学背景的关系，我的编程基础很差。回想起当年我读博初期，从硕士研究优化理论到博士初次接触运筹学的应用，在建立好优化模型后，对于如何使用求解器是一头雾水，无从下手。

直到去到IBM Cplex实习，Cplex开发者之一的Dr. Andrea和IBM科学家Andrea Lodi教授，以及玛丽居里项目几个同事悉心地指导，才使我短短几个月便从Cplex最简单的应用到Cplex callback function到列生成法的实现逐一捋了一遍！

优化求解器对于做运筹学应用的学生来说，意义重大。

然而直到今天，放眼望去，全网（包括墙外）几乎没有一个系统的Cplex中文求解器教程。

作为华人运筹学的最大的社区，『运筹OR帷幄』 责无旁贷，我们决定承担这“吃力不讨好”的责任。

Cplex视频教程由『运筹OR帷幄』视频版块荣誉出品，主编留德华叫兽策划和监制，【视频】版块责编唐博编程并录制。

我们假设读者已有一定的Python编程基础，在传授如何用Python调用Cplex求解数学规划问题的同时，我们也试图讲解如何把一个实际问题（例如计算机视觉案例）数学建模成整数规划模型，以及一些常用的大规模优化问题的分解方法（割平面、列生成）等建模技巧。

希望这个系列视频能为大家扫盲数学规划求解器的编程基础，并且用实例带大家入门如今最火的人工智能、计算机视觉（底层都是优化问题）。

**总提纲（试行）：**
- CPLEX的下载和安装
- CPLEX Python API的安装和调用
- 数学规划模型lp、mps格式文件的读写
- 线性规划问题建模及求解
- 线性规划问题敏感性分析
- 线性规划问题优化器选择
- 混合整数规划问题建模及求解
- 混合整数规划问题求解策略设置
- 混合整数二次规划问题求解
- 模型修改和重新优化
- 进阶版--绝对值不等式转线性
- 进阶版--高次不等式转线性
- 进阶版--大M不等式及其伤害
- 进阶版--数学建模的艺术之一题多解
- 进阶版--割平面法之计算机视觉实例
- 进阶版--列生成法之计算机视觉实例

以上提纲仅为试行版本，随着视频录制的进行，可能会有修改或增减。

也希望优化求解器大牛们参与到我们的视频教程创作中来，或提供宝贵的建议，一起参与到运筹学的科普中～

**第一期视频内容：**
- 用CPLEX制作图像处理工具之展望
- CPLEX学术版申请
- CPLEX下载及安装
- Anaconda环境下安装CPLEX Python API
- CPLEX Python API的调用

据@唐博说，第一期视频虽然只有不到10分钟，但是前期策划和转变，加上反复录制和剪辑，前前后后花了至少有8个小时。

不光是录制视频，其实『运筹OR帷幄』创作推文也一样，每篇文章都会花费责编/作者数小时创作/编辑。

因此，各位读者如果看到您觉得不错的推文，希望可以点个“好看”和分享，让知识传播地更远，一起科普运筹学！

> 郑重声明：
所有Cplex Python API视频教程
免费
代码开源

## 好了，千呼万唤使出来，下面便是第一期的视频教程：

**嫌手机屏幕太小，看得不够过瘾！？**

没问题！欢迎关注『运筹OR帷幄』官方B站：运筹OR帷幄，Cplex Python API系列视频教程和视频直播都会首发于B站。

B站网址：[https://space.bilibili.com/403058474](https://link.zhihu.com/?target=https%3A//space.bilibili.com/403058474)

此外，欲获得视频中的源代码，请于『运筹OR帷幄』公众号后台回复关键词： Cplex教程

## **强烈建议大家下载源代码，跟着唐博一起手撸Cplex的Python源代码喔！～**

## **04运筹OR帷幄【视频】版块成立啦**

**「视频」版块上线初期，将涵盖三个模块：**

Part ONE：  

视频教程--即类似今天这样的技术视频教程

Part TWO：

科普快照--即科普类、趣味性视频，让学科传播地更远

Part THREE：

SundayLive--即留德华叫兽担任主播，并不定期邀请行业好友/大佬，与大家谈天说地

**欲知详情，请见：**

[号外！『运筹OR帷幄』开直播、出教程啦！「视频」版块正式开通！](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzg2MTA0NzA0Mw%3D%3D%26mid%3D2247488056%26idx%3D1%26sn%3Da0e6f7a87dd997462e826aa05c4eee59%26scene%3D21%23wechat_redirect)

**【视频】版块主编/责编火热招募中**【视频】版块现招募版块（副）主编，欢迎对运筹学/数据科学/AI科普有热情的小伙伴加盟，将学科知识传播给平台10W+的同行，共同学习和成长。

最后，如果对于【视频】版块或者Cplex教程的提纲有任何建议或想法，都欢迎在评论区指出！

**温馨提示**

欲获得视频中的源代码， 可以在 本公众号后台 回复关键词：“ **Cplex教程 **”，如果觉得有用， 请勿吝啬你的留言和赞哦！~

扫二维码关注『运筹OR帷幄』公众号：
![](https://pic1.zhimg.com/v2-872dc2d2c5eaf08b38d693490549136c_b.jpg)
[点击查看『运筹OR帷幄』志愿者招募介绍及加入方式](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s/4ganskchRA_iO1CiNtWmOA)：
![](https://pic1.zhimg.com/v2-d1fbc7022dc62969dcf8a6afdacdfcb4_b.jpg)

