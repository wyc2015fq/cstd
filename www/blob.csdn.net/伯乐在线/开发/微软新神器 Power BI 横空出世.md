# 微软新神器 Power BI 横空出世 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [数据之巅](http://www.jobbole.com/members/asxinyu) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
在当前互联网，由于大数据研究热潮，以及数据挖掘，机器学习等技术的改进，各种数据可视化图表层出不穷，如何让大数据生动呈现，也成了一个具有挑战性的可能，随之也出现了大量的商业化软件。今天就给大家介绍一款逆天Power BI可视化工具。逆天的不仅仅是工具，还有其使用门槛和价格。
Power BI系列文章地址：http://www.cnblogs.com/asxinyu/p/Power_BI_Introduce.html
![](http://jbcdn2.b0.upaiyun.com/2016/06/d8c255453414edc401014995eb963802.jpg)
# 1.前言
半年来，一直在做数据统计的后台工作，虽然出来了数据，但考虑到前端可视化的问题，非常晕头，包括领导也在考虑这方面的事情，也不断接触了很多BI工具，传统BI架构中，数据和使用，以及报表都集成在一起，随着业务的复杂，开发人员工作越来越大，虽然现在使用前端工具做一个图表很容易，但考虑到纬度和灵活性，对复杂的业务来说，已经严重跟不上脚步。经过最近一段时间的学习了解和学习，体会也越来越深。
今天就给大家推荐一款BI神器Power BI，目前个人只是会简单的使用，主要精力集中在各项功能以及整个可视化系统实现的细节，如数据源刷新，云端展示，仪表盘设计，业务系统集成，协作共享等方面。虽然不是很深，但我相信，随着目前技术和各个厂家的不断投入和推广，这个工具也会越来越强大。
# 2.微软Power BI技术演变
虽然本次发布的Power BI独立产品眼球一亮，比较逆天，但其实微软在BI领域一直都很强，原因很简单，就是因为逆天的Excel，微软Excel很早就支持了数据透视表，并基于Excel开发了相关BI插件，如Power Query，PowerPrivot,Power View和Power Map等。这些插件让Excel如同装上了装逼的翅膀，瞬间高大上。由于Excel的普及和可操作性简单，加上数据透视表技术已经深入人心，所以全新的Power BI呼之欲出，相比Tableau等产品，有着无可比拟的天然优势。由于微软大船的原因，可能在这方面发力比较晚，让Tableau等专业产品有了发扬光大的机会。Tableau产品的体验课和售前演示都听过，目前不打算学习。看看目前在BI领域的魔力象限分布图：
![](http://jbcdn2.b0.upaiyun.com/2016/06/44d96e75ef5ee8ced6574d11cf6c3445.png)
很显然，微软与Tableau处于行业领先位置，但就目前的行业占有率来说，与Tableau还有差距。
# 3.全新的Power BI隆重登场
很显然Power BI是整合了Power Query，PowerPrivot,Power View和Power Map等一系列工具的成果，所以以前使用Excel做报表和BI分析的，可以快速使用，甚至直接使用以前的模型，同时新版本的Excel 2016也提供了Power BI插件。
## 3.1 Any data,Any where,Any time
看看这个口号，一起来了解Power BI。
### 3.1.1 Any Data
Power BI已经支持各种数据源，包括文件(如Excel，CSV，XML，Json，文本等，还支持文件夹)，数据库(常见的关系型数据库如Access，MSSQL，Oracle，DB2，Mysql等等)，还有各种微软云数据库，其他外部数据源（如R脚本，Hadoop文件，Web等等）；
![](http://jbcdn2.b0.upaiyun.com/2016/06/4028b9a5a40110d40561f80ad127406f.png)
### 3.1.2 Any Where
意味着我们可以在任何地方进行编辑和修改报表，不仅仅是Power Desktop可以进行编辑和发布报表，微软还有在线版编辑工具，功能也一点不差，在tableau的体验课上没见过有web开发环境。通过模型的发布，对组合发布的报表，我们可以使用分享功能，发送到邮箱，或者嵌入到业务系统中，非常方便。
![](http://jbcdn2.b0.upaiyun.com/2016/06/85d397002c2728f93e339a34e2ff6b58.png)
### 3.1.3  Any time
意味着不管你是开发者，还是领导，都可以随时通过互联网进行数据分析和决策。
![](http://jbcdn2.b0.upaiyun.com/2016/06/286fb4efcebec509420bf22fc964aca1.png)
![](http://jbcdn2.b0.upaiyun.com/2016/06/833b26db2f0a4544bff1ffe23b833dd3.png)
## 3.2 Power BI Desktop 与 Power BI Mobile
Power BI Desktop 是一个桌面版开发环境，同时也可以进行在线的开发和编辑，非常强大。如下图：
![](http://jbcdn2.b0.upaiyun.com/2016/06/410614e08dab7b6bcce86e8a1c255c17.png)
同时，也有针对手机端的Power BI Mobile，涵盖WP，Android和苹果三大平台。手机版的功能暂时没有体验，这里不过多说明，有兴趣的可以进行下载。
链接如下：[https://powerbi.microsoft.com/en-us/mobile/](https://powerbi.microsoft.com/en-us/mobile/)
![](http://jbcdn2.b0.upaiyun.com/2016/06/c8629143838260b908c8711ca0cf60c9.png)
## 3.3 其他功能亮点
由于功能比较多，个人也在学习使用阶段，暂时只把自己掌握和了解的部分功能写出来。
- 与Excel PowerPrivot集成，模型文件都可以通用；
- 与Excel集成，可以通过Excel发布上传模型；以前使用Excel做数据分析的，也非常容易上手
- 可以自定义视觉图像对象，可以在线下载别人制作的自定义图表；效果可以为所欲为，爽不爽。。。
- 智能问答，根据你的简单输入或者规定语法，自动获取数据和展示；爽呆了。。。
- 还可以根据你提供的数据源，自动进行各个纬度的分析和展示，然后挑选你任务有价值的图表直接放到仪表盘，爽呆了。
- 提供了REST API可以使用你喜欢的编程语言来推送数据到报表或者数据集中，更加灵活。![](http://jbcdn2.b0.upaiyun.com/2016/06/35688c5172c5914c3d092171a3528e15.png)
# 4.Power BI的授权和费用问题
很显然大家肯定关注Power BI的授权和使用费用问题。根据我个人的了解，行业领头羊Tableau的产品是价格不菲，15天试用期，听说是6K美刀一个用户起步，用户数有最低数，几个就不清楚了)，一年免费的新版本升级，一年后20%的升级费用。Soga,是不是很坑，那么大家肯定比较关注Power BI的价格。我把我了解的信息和大家分享一下：
1.有免费版。Desktop版本无功能限制，这一点最令人激动，需要使用企业邮箱注册。免费的存储在线存储服务是1G空间(已经可以满足很多很多人的需求了)。
2.有专业版，9.99美刀每用户每个月(10G空间)，就算10个用户（对大部分企业来说，10用户已经很强大，100G的，有多少公司的统计数据可以到？也意味着BI团队有10多人，注意不是开发），每个月也就几百块，一年几千块，可以说超值。专业版不仅在存储容量有增加，另外一个功能就是数据的在线刷新频率和刷新量大大增加。免费用户每天一次，1万行数据，；而专业版是每小时1百万数据行。其次就是共享协作的差别，免费版只能分享，但不能协作。对很多企业来说，1个用户创建已经OK了。
下面网址列出了具体的功能差别：[https://powerbi.microsoft.com/en-us/pricing/#](https://powerbi.microsoft.com/en-us/pricing/)
# 5.Power BI资源
1.Power BI下载：
官方网站：[https://powerbi.microsoft.com](https://powerbi.microsoft.com/en-us/mobile/)
注意Power BI目前没有中文主页，只有英文和台湾版本的主页，建议通过下列途径下载安装简体中文版。
Microsoft Power BI Desktop中文最新版：[下载地址](https://www.microsoft.com/zh-cn/download/details.aspx?id=45331)
Power BI Gateway – Enterprise：[下载地址](https://www.microsoft.com/zh-cn/download/details.aspx?id=50033)
2.文章：[微软又一逆天可视化神器Power BI](http://card.weibo.com/article/h5/s?from=timeline&isappinstalled=0#cid=1001603959151645616083&from=&wm=&ip=172.16.36.176)
3.个人在新生命论坛建立了Power BI板块，大家可以在论坛进行交流和提问，相关资源也将同步发布至网站。
论坛网址：[http://www.newlifex.com/](http://www.newlifex.com/)
4.个人建立的Power BI技术群，目前正在学习阶段，有兴趣的朋友可以一起学习讨论。
E-mail: **admin****@****bi365.vip****、1287263703@qq.com**
QQ：**1287263703**
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/12/5df37084d6a98660e69e2f5a721488d9.png)![](http://jbcdn2.b0.upaiyun.com/2016/12/167e39c87c330dd009e0aaf181874cbd.jpg)
