# Stata: 图示交互效应\调节效应 - stata连享会 - CSDN博客





2019年03月15日 15:45:36[arlionn](https://me.csdn.net/arlionn)阅读数：430
个人分类：[stata绘图																[回归分析](https://blog.csdn.net/arlionn/article/category/7157127)](https://blog.csdn.net/arlionn/article/category/7157152)








![空间计量专题-海报.jpg](https://upload-images.jianshu.io/upload_images/7692714-9562ebfb358ccf28.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

> 
作者：邓浩然 (上海财经大学)

Stata 连享会： [知乎](https://zhuanlan.zhihu.com/arlion) | [简书](http://www.jianshu.com/u/69a30474ef33) | [码云](https://gitee.com/arlionn)



- Stata连享会 [精彩推文1](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文2](https://github.com/arlionn/stata/blob/master/README.md)

![](https://upload-images.jianshu.io/upload_images/7692714-8b1fb0b5068487af.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)




### 文章目录
- [1. 期刊论文中的调节效应图](#1__18)
- [2. 如何绘调节效应图 ？](#2___35)
- [2.1 在excel中绘出交互图](#21_excel_39)
- [2.2 在 Stata 中绘出交互图](#22__Stata__49)
- [基本原理](#_53)
- [Stata 实现](#Stata__66)
- [进一步美化图形](#_97)
- [3. 呈现调节作用的其他方式](#3__131)
- [4. 参考资料](#4__138)
- [关于我们](#_148)
- [联系我们](#_155)
- [往期精彩推文](#_162)



实证分析中，交乘项的使用非常的普遍。关于调节作用的讲解已经较为详实，具体可以点击以下链接进行了解，本文主要说明如何通过 Stata 绘图呈现调节作用。
[Stata：交乘项该如何使用？](https://www.jianshu.com/p/f7222672fe89)


## 1. 期刊论文中的调节效应图

一般我们在检验调节（交互）作用的时候，得到回归结果之后往往需要在文章中附上调节作用图，能够直观的从图形上观察到调节效应。

> 
**示例 1：** Li, J., & Tang, Y. (2010). CEO Hubris and Firm Risk Taking in China: The Moderating Role of Managerial Discretion. Academy of Management Journal, 53(1), 45–68. [[PDF]](http://www.bm.ust.hk/mgmt/files/staff/papers/JT/AMJ_v53p45_CEO%20hubris.pdf)


**解读**：横轴表示 CEO 的自负程度，纵轴表示企业风险偏好程度。可以非常明显的看出，在行业丰腴较高的情况下，自负 CEO 对于公司风险偏好的影响强于在行业丰腴程度较低的情况下，自负 CEO 对于公司风险偏好的影响。

![图1 CEO自负与企业风险偏好](https://upload-images.jianshu.io/upload_images/16406216-08f988cd972ff39f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

> 
**示例 2：** 李绍龙, 龙立荣, 贺伟. 高管团队薪酬差异与企业绩效关系研究:行业特征的跨层调节作用[J]. 南开管理评论, 2012, 15(4):55-65. [[PDF]](http://search.cnki.net/down/default.aspx?filename=LKGP201204007&dbcode=CJFD&year=2012&dflag=pdfdown)


**解读**：横轴表示高管垂直薪酬差异，纵轴表示企业绩效。从图中可以看出，在低水平薪酬差异的情况下，垂直薪酬差异对于企业绩效的影响强于在高水平薪酬差异的情况下，垂直薪酬差异对于企业绩效的影响。

![图2 薪酬差异与企业绩效](https://upload-images.jianshu.io/upload_images/16406216-47189c458f9d9b4b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## 2. 如何绘调节效应图 ？

下面将介绍两种方法绘出调节（交互）作用图，分别以 Excel 和 Stata 作为实现工具。

### 2.1 在excel中绘出交互图

在 Courser 有一门关于数据分析的课程叫做：[Understanding Your Data: Analytical Tools](https://www.coursera.org/learn/uva-darden-understanding-data-tools)。这门课程中有讲到如何 excel 做出调节作用的图像，可直接点击 [[Interpreting interaction effects]](http://www.jeremydawson.co.uk/slopes.htm) 下载作者已经制作好的 excel 模板，制作出的效果如图 3 所示。

知乎上网友详细说明了使用方法，参见
[「用EXCEL画交互效应图」](https://zhuanlan.zhihu.com/p/36785799)。文中提供的资料已经陈诉的十分详实，也因本文主要讲解在Stata中绘制调节效应图，此处便不过多赘述。

![图3 用EXCEL画交互效应图](https://upload-images.jianshu.io/upload_images/16406216-44237408b9f28f77.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

图3 用EXCEL画交互效应图
### 2.2 在 Stata 中绘出交互图

在 Stata 中可以很方便地实现同样的绘图效果。好处在于能够保证结果的可复制性，也能够少去复制到 excel 进行操作的麻烦。

#### 基本原理

Stata 中绘出同样的图像，我们需要先明白绘图的原理。
- 图三中High IV(自变量) 与Low IV(自变量)是分别指在自变量均值上加减一个标准差
- High Moderator(调节变量）与Low Moderator(调节变量)也是分别指调节变量在均值上加减一个标准差
- 用计算所得的数值进行两两组合，组合结果如下表
||High IV|Low IV|
|----|----|----|
|**High Moderator**|A|B|
|**Low Moderator**|C|D|
- 计算组合结果（A、B、C、D点）在回归方程上的预测值

#### Stata 实现
- 首先,我们选用 `auto.dta` 数据集，进行一个调节效应的回归，并将结果保存下来。

```
sysuse auto, clear
regress price c.length##c.mpg   //调节效应回归
est sto regression   //保存结果
```
- 其次，分别求自变量以及调节变量在均值上加减一个标准差的值

```
//分别求自变量与调节变量加减一个标准差的值
foreach v of var length mpg {
  su `v' if e(sample)
  local low_`v'=r(mean)-r(sd)
  local high_`v'=r(mean)+r(sd)
}
```

在调回保存的回归结果，再利用 `margins` 命令求取预测值，并用 `marginsplot` 绘制图形：

```
est restore regression //调取保存的回归结果
margins , at(mpg=(`low_mpg' `high_mpg') ///
         length = (`low_length' `high_length'))  //计算边际效应
marginsplot , xlabel(`low_mpg' "Low IV" `high_mpg' "High IV")  ///
              ytitle("Dependent variable")       ///
              ylabel(, angle(horizontal) nogrid) ///
              legend(position(3) col(1) stack)   ///
              title("") noci
```

运行完上述命令后，得到如下图形。

![图4 Stata绘制的交互作用图](https://upload-images.jianshu.io/upload_images/16406216-4710a0f17c9b2849.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

#### 进一步美化图形

对图形的调整主要是对图形选项进行调整，具体选项可以在 stata 中输入 `help marginsplot` 查看官方的帮助命令。下面列举一些常用的设定：
- **控制图形四周留白：** 比如我们希望图形 y 轴包含的范围更广，让直线不要完全充满整个图形，可以通过调整 `ylabel` 选项以及 `xlabel` 选项调整显示的宽度与高度。
- **坐标轴刻度：** 图 4 的 y 轴显示范围采用的是 stata 的默认设定：取值范围为 3000 至 8000，间隔为 1000。我们可以通过在 `ylabel` 选项中加入 `2000(1000)9000`，表明 y 轴显示范围为 2000 至 9000，间隔为 1000。
- **横轴的显示效果：** 图 4 的 x 轴仅显示两点（“Low IV”  “High IV”），为了调整直线所处的左右距离，可以修改 `xlabel` 选项，调整直线所处的距离。

下面是修改后的代码：

```
//分别求自变量与调节变量加减一个标准差的值
foreach v of var length mpg {
   su `v' if e(sample)
   local low_`v'=r(mean)-r(sd)
   local high_`v'=r(mean)+r(sd)
}

. est restore regression //在调取保存的回归结果
*-y轴的范围从2000至10000，间隔为1000
. margins , at(mpg=(`low_mpg' `high_mpg') ///
            length = (`low_length' `high_length')) //计算边际效应
. marginsplot,  ///
   xlabel(13 " " `low_mpg' "Low IV" `high_mpg' "High IV" 30 " ") ///
   ytitle("Dependent variable")            ///
   ylabel(2000(1000)9000, angle(0) nogrid) ///
   legend(position(3) col(1) stack)        ///
   title("") noci
```

运行以上代码，得到如下图像

![图5 修改后Stata绘制的图像](https://upload-images.jianshu.io/upload_images/16406216-b1380100f7fd0f0e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

### 3. 呈现调节作用的其他方式

当然还有很多方法呈现调节作用的效果，根据论文需要选择合适的方式。详情参见 Stata 连享会往期推文：
- [Stata：用 bytwoway 实现快速分组绘图](https://www.jianshu.com/p/1471cf87f25f)
- [Stata：边际效应分析](https://www.jianshu.com/p/012d8a6159cf)
- [Stata：图示连续变量的边际效应(交乘项)](https://www.jianshu.com/p/7af58033dc24)

### 4. 参考资料

[Statalist - Interaction Plot](https://www.statalist.org/forums/forum/general-stata-discussion/general/1373634-interaction-plot)





> 
#### 关于我们

- 【**Stata 连享会(公众号：StataChina)**】由中山大学连玉君老师团队创办，旨在定期与大家分享 Stata 应用的各种经验和技巧。
- 公众号推文同步发布于 [CSDN-Stata连享会](https://blog.csdn.net/arlionn) 、[简书-Stata连享会](http://www.jianshu.com/u/69a30474ef33) 和 [知乎-连玉君Stata专栏](https://www.zhihu.com/people/arlionn)。可以在上述网站中搜索关键词`Stata`或`Stata连享会`后关注我们。
- 点击推文底部【阅读原文】可以查看推文中的链接并下载相关资料。
- Stata连享会 [精彩推文1](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文2](https://github.com/arlionn/stata/blob/master/README.md)

> 
#### 联系我们

- **欢迎赐稿：** 欢迎将您的文章或笔记投稿至`Stata连享会(公众号: StataChina)`，我们会保留您的署名；录用稿件达`五篇`以上，即可**免费**获得 Stata 现场培训 (初级或高级选其一) 资格。
- **意见和资料：** 欢迎您的宝贵意见，您也可以来信索取推文中提及的程序和数据。
- **招募英才：** 欢迎加入我们的团队，一起学习 Stata。合作编辑或撰写稿件五篇以上，即可**免费**获得 Stata 现场培训 (初级或高级选其一) 资格。
- **联系邮件：**[StataChina@163.com](mailto:StataChina@163.com)

> 
#### 往期精彩推文

- [Stata连享会推文列表1](https://www.jianshu.com/p/de82fdc2c18a)
- [Stata连享会推文列表2](https://gitee.com/arlionn/jianshu/blob/master/README.md)
- Stata连享会 [精彩推文1](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文2](https://github.com/arlionn/stata/blob/master/README.md)

![](https://upload-images.jianshu.io/upload_images/7692714-8b1fb0b5068487af.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![欢迎加入Stata连享会(公众号: StataChina)](http://upload-images.jianshu.io/upload_images/7692714-8f8547b20ec61ca0.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)






