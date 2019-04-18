# 【MIS】一家公司到底有多少种管理系统？——从包罗万象到术业专攻 - 知乎
# 

> **文章作者：**Aaron
**责任编辑：**黑铁师
文章发表于**微信公众号【运筹OR帷幄】：**[【MIS】一家公司到底有多少种管理系统？——从包罗万象到术业专攻](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s/EPWRwOBT8_616R5RWbdaTQ)
*欢迎原链接转发，转载请私信*[@留德华叫兽](https://www.zhihu.com/people/961e8cc4f7512fda1ea6626ce9a05e8e)*获取信息，盗版必究。*
敬请关注和扩散本专栏及同名公众号，会邀请**全球知名学者**发布运筹学、人工智能中优化理论等相关干货、[知乎Live](https://www.zhihu.com/lives/users/961e8cc4f7512fda1ea6626ce9a05e8e)及行业动态：[『运筹OR帷幄』大数据人工智能时代的运筹学](https://zhuanlan.zhihu.com/operations-research)

面对市场上琳琅满目的仅仅用几个英文字母代表的信息系统，每位刚刚接触的同学内心都是崩溃的。MES是什么鬼？他跟MIS是表兄弟吗？他跟WMS又是什么关系呢？下面我们就分两期来扒一扒这些企业中的信息系统，这是第一期。

先来一张全家福。
![](https://pic2.zhimg.com/v2-1ae7f51da9603fbf06eb56e9361f4401_b.jpg)

## **MIS**

MIS的全名叫管理信息系统（Management Information System），他算是最早期的信息系统了。一般来说，如果一个企业内部开发了一套单项的信息化管理系统，我们就可以称之为管理信息系统（MIS）。这类系统的构思多数是原有手工业务的翻版，往往不能实现全局的信息集成。不过，有哪个高大上的系统不是从初级版本发展起来的呢？

## **MRP/MRPII**

MRP（Material Requirement Planning），他的本质是个计算器，根据产品物料清单（Bill Of Material, BOM）、库存现有量和提前期等基础数据，计算出应该在什么时间按什么数量提供什么物料。

MRP II（Manufacturing Resources Planning）是从MRP发展而来的，主要是在MRP的基础上增加了产能计划以及车间作业、采购作业、销售作业、仓储作业、成本核算、财务账务处理等功能，并把第一层的供应商和客户档案纳入系统管理以控制收付款。可以看到，MRP II已经基本囊括了一家公司内部的所有作业。
![](https://pic2.zhimg.com/v2-cc5a45a65b2cfffc11ec5c75ef283015_b.jpg)

## **ERP**

ERP的全称是企业资源计划（Enterprise Resources Planning），他的产生源于Gartner公司在20世纪90年代的一个设想：能适应离散、流程和分销配送等 不同生产模式，并能实现两个集成（内部集成指实现产品研发、核心业务和数据采集的集成；外部集成指实现企业与供应链上所有合作伙伴的集成）的管理系统。

当时受限于网络等技术条件，这个设想还不可能完全实现。但市场上一些 MRP II 软件厂商在采纳了Gartner公司最初对ERP提出的技术要求（如4GL，RDBM，GUI，C/S，CASE等当时算是领先的技术），或解决了跨世纪的千年虫（Y2K）问题，或增加了Gartner最初提出的一些外扩功能（如EDI接口、DRP、运输、仓库、设备、质量、实验室及项目管理、现场服务及维修管理以及人力资源管理等）之后，就为了适应时代潮流和商业目的需要，把已有的MRP II产品易名为ERP。这种易名实质上还没有也不可能实现Gartner对ERP系统的基本定义和设想——“管理整个供应链”，软件的基本内容还是停留在原有MRP II的水平。然而这种易名的做法却模糊了MRP II同ERP的界限，使得一些从来没有接触过MRP II成熟产品的人，自然而然地把从MRP II易名而来的ERP看成是“面向企业内部管理”的系统。

其实后来出现的一连串管理系统，包括：SCM，SRM，WMS，MES，APS，EAM，CRM，PDM，PLM等等，他们的实质都是实现 Gartner 最初对广义 ERP 定义的补充。
目前市场上的ERP厂商包括：ORACLE、SAP、Infor、用友、金蝶等。

## **SCM**

接下来要介绍的这个SCM，可不是大名鼎鼎的彼得.德鲁克老先生提出的供应链管理理念，他只是一个软件——供应链管理信息系统（Supply Chain Management information system）。

SCM与ERP最大的区别在于，SCM把更多层次的供应商、企业合作伙伴（敏捷制造中的虚拟企业）以及各个节点之间的运输、仓储和配送业务纳入进来，强调上下游企业间的信息协同管理，是面向整个供应链的管理系统。

SCM系统的主要功能包括：供应链绩效评价与智能决策、供应链计划与优化、销售订单管理、分销与配送、运输管理、仓储管理、采购管理、供应商关系管理等。
目前市场上的SCM厂商包括：Infor、Covisint、E2open、明基逐鹿、元工国际、科箭软件。
![](https://pic4.zhimg.com/v2-047f7fbe04045b1ef9f01c4dbb9224fb_b.jpg)

## **MES**

MES的全称是制造执行系统（Manufacturing Execution System），他主要面向企业的生产制造业务单元，用于对产线工单的执行情况进行管理，主要功能模块包括：生产工单下达、工单配料、工单排程、线边库管理、车间作业、产能及设备综合效率（OEE）、设备维护、品质管理、工艺管理、作业员排班等。

MES与SCM类似，他们都是从ERP中衍生出来的专业系统。为什么会是这样？主要基于以下考虑，当面对某一细分领域内需要优化的需求时，需要权衡是在原系统上继续增加或优化功能模块还是独立开发一个专业系统来应对专业的需求。当原系统中模块较少时，增加一个模块对其影响还可接受，但随着原系统规模越来越大，增加或优化一个模块的复杂度和成本将会越来越高。为保障原系统的可靠性及运维复杂度，于是就独立出了各个细分领域的专业系统。MES是从ERP中衍生出来的，后来他又在设备维护与工单排程两个领域衍生出了EAM与APS两个系统。

目前市场上的MES厂商包括：西门子、GE、Rockwell、上海宝信等。
![](https://pic3.zhimg.com/v2-ff6e4c8f27337975f2d37fb7fe931366_b.jpg)

## **EAM**

EAM是企业资产管理系统（Enterprise Asset Management）的缩写，他主要面向资产密集型企业，提倡在不明显增加运行费用的前提下，通过信息技术降低停机时间、增加生产产量。主要功能包括：设备台账、备品备件库存管理、预防性维护计划、维修工单管理、质量管理、成本核算等。

目前市场上的EAM厂商包括：IBM、Infor等。
![](https://pic1.zhimg.com/v2-d670ae29f257ea65a472dd46edbfe6e4_b.jpg)

## **APS**

APS（Advanced Planning and Scheduling，高级计划排程）的核心功能包括两大部分：高级计划与高级排程。高级计划主要是对任务和资源进行指派，明确某项任务在什么时间分配哪个资源进行处理；高级排程则主要是对任务清单进行排序调度，以使某项或某几项目标（如最大完工时间、交期延误数量等）得到优化。

根据应用场景不同，APS又可分为供应链级APS与工厂级APS。供应链级APS侧重于供应链计划的优化，包括网络配置计划、需求计划、库存计划、多工厂计划、供应计划等的优化。工厂级APS则侧重于交期承诺、计划与排产、加工顺序调度、物料准时配送等的优化。

目前APS所用到的优化算法主要分为四种：
- 基于约束理论（Theory of Constraints，TOC）的有限产能算法；
- 启发式规则算法、专家系统、线性规划等；
- 遗传算法、模拟退火算法、蚁群/粒子群算法、神经网络等智能算法；
- 智能算法融合人工智能动态调整算法，以智能算法进行静态排程，以多Agent代理协商进行动态调整。
市场上的APS厂商包括：Asprova、坤帝科（Quintiq），另外国外知名的ERP厂商也都通过收购或研发拥有了自己的APS模块，如SAP APO，ORACLE VCP、Infor LN等。
![](https://pic2.zhimg.com/v2-c1cbdf2d92409b6f1363d4f47b0aa4d9_b.jpg)
本期先到这里，下期我们将继续介绍WMS、TMS、DRP、CRM、PDM等管理系统，敬请期待。

## **参考文献：**

(1) 陈启申. ERP—从内部集成起步[ M]. 电子工业出版社， 2005.

(2) 易拓必盛 (E2biz). 供应链管理中十大系统的协同——SCM和CRM、APS、MES. [ EB/ OL]. “E2biz服务笔记”微信公众号. 2017.

(3) e-works Research. 制造业供应链管理信息系统（SCM）选型与实施指南. [ EB/ OL]. www.e-works.net.cn. 2015.

(4) 蔡颖. APS走向实践. [ M]. 广东经济出版社. 2007.

> **文章来源申明：**本篇文章由『运筹OR帷幄』原创发布
如需转载请在公众号后台获取转载须知

【活动】版块长期招收志愿者，如果你有线上/线下活动策划经验，社群活动策划经验， 或者想加入我们增长相关经验，欢迎加入我们。具体可以在微信公众号后台回复：“志愿者”关键词获取详细信息，申请时请注明【活动】版块  

**『运筹OR帷幄』**长期招收志愿者，如果你有一技之长，爱分享，欢迎加入我们。点**击下方图片**即可了解详情！（如果对MIS版块感兴趣，欢迎备注【MIS】哦！）

扫二维码关注『运筹OR帷幄』公众号：
![](https://pic1.zhimg.com/v2-872dc2d2c5eaf08b38d693490549136c_b.jpg)
[点击查看『运筹OR帷幄』志愿者招募介绍及加入方式](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzUxMTYwMzI0OQ%3D%3D%26mid%3D2247486166%26idx%3D1%26sn%3Da598e1475ae4f91afb7c33114ee61a85%26chksm%3Df9706d88ce07e49ec590fc2589d251bbf2b7d605cd127b1a0bdff61b8bad985eddf42365d823%26scene%3D21%23wechat_redirect) ：
![](https://pic1.zhimg.com/v2-d1fbc7022dc62969dcf8a6afdacdfcb4_b.jpg)
[点击查看【骥寻伯乐】板块说明及投稿方式](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzUxMTYwMzI0OQ%3D%3D%26mid%3D2247486137%26idx%3D2%26sn%3D24e9487d15a8c590645a4d83950c8f20%26chksm%3Df9706de7ce07e4f1507edd01511920bd8f0a3a80384aef8ee8768b68931f872bc6cff5d3485f%26scene%3D21%23wechat_redirect) ：
![](https://pic3.zhimg.com/v2-88ba2272456e93bb2f6b9bf2d55ef1d2_b.jpg)

