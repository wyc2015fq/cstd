
# AI产品经理数据模型设计文档(简版) - 喜欢打酱油的老鸟 - CSDN博客


2019年04月03日 08:32:42[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：217


[http://www.woshipm.com/ai/2019675.html](http://www.woshipm.com/ai/2019675.html)

> 目前数据、算法、AI产品经理岗位十分热门，本文写给想要入行这方面的新人们，带大家一起了解一下什么是数据模型设计文档。
![](http://image.woshipm.com/wp-files/2019/03/g3Z9i2ZmJ4dhLZTzXbrs.png)
有一些产品童鞋不惜花很大的力量想要看看数据模型设计文档长什么样子，下面就列举一个数据模型文档的例子，本文尽量奔着通用型产品经理能看懂来写。
据笔者实践经历来看，目前数据、算法、AI等是热门产品经理岗位。但是不同的产品经理还是建议遵循自身的职业背景定位来做好对应的产品岗位。
AI产品经理有偏AI本身的算法类AI产品经理，例如：像打造Caffe深度学习框架的贾扬清，贾扬清是产品经理，但是不是所有的产品经理都能像他一样一手创造一个框架级产品。
AI产品经理有偏AI算法赋能数据的，这一类产品经理建议主要经理放在数据的业务逻辑上，而不是下面笔者列举的为数据建模上除非你的特点和爱好是数据模型。
AI产品经理有偏AI+的，就是利用比较成熟的AI框架、成熟的算法模型来赋能，来助力自己的产品和应用，例如：笔者就在新书《AI赋能-AI重新定义产品经理》一书中撰写了不同的产品经理如何入行不同的AI产品岗位助力产品落地。
下面我们讲一个基础数据模块模型设计例子，企业组织架构模型设计。
组织是企业实现目标的基础单元，所有业务功能、流程、事件等都以满足组织发展的需要而设定。通常情况下，企业的组织随着企业发展有时候需要改进、改变或创建声明角色。所以在创建组织架构模型时，充分考虑数据架构的灵活适应性以应对未来不可知的业务模型变化，减少对基础模型的调整和影响，这也是复合企业运行OKR的。
## 一、组织架构模型
包含的内容及关系见下图：
![](http://image.woshipm.com/wp-files/2019/03/iFYjTLHF00XdeVfcKtTs.png)
在基础模块部分，企业组织架构涉及的数据模型包含组织单元、组织人员、声明角色、权限控制【执行、访问功能】；相关模块还包括流程数据、产品数据。
另外组织单元的数据模型也可包括外部各方和业务伙伴组织。
## 二、组织单元数据模型
组织单元数据为树形数据结构；数据模型按照组织树进行存储。对于SaaS企业应用，租户【客户】的组织单元数据模型与租户【客户】信息表相关联「客户管理数据模型设计」。
## 三、组织单元概念数据模型
![](http://image.woshipm.com/wp-files/2019/03/TfbwcaLUFIkACRVxvgPJ.png)
## 四、组织单元实体属性
![](http://image.woshipm.com/wp-files/2019/03/eBiiblvP33ioKfEPYKtD.png)
![](http://image.woshipm.com/wp-files/2019/03/nyKwzcDioz1J1ZrHMZB6.png)
## 五、组织类型实体属性
![](http://image.woshipm.com/wp-files/2019/03/nXTa4aXTaddGgqmw5COi.png)
## 六、组织人员数据模型
在组织人员的数据结构设计上，采用了将人员账号和人员基本信息分离的模式；更灵活和开放的应用账号管理和人员基本信息访问的需求。
## 七、人员概念数据模型
![](http://image.woshipm.com/wp-files/2019/03/gyQpHqkppvDiNqPLv8Sd.png)
## 八、user实体属性
![](http://image.woshipm.com/wp-files/2019/03/qbOJXp8RuMO4bFJZOI2O.png)
![](http://image.woshipm.com/wp-files/2019/03/untdaVa3Tiv9RpzvA5Ly.png)
## 九、Person实体属性
![](http://image.woshipm.com/wp-files/2019/03/PvB0SakiC8MYSSTwd8Zo.png)
![](http://image.woshipm.com/wp-files/2019/03/JGWNNUq5QUfFSo4hS3sD.png)
若person实体应用于相关模型，可扩展人员联系实体。
## 十、声明角色模型
为定义灵活的添加和更改角色类型数据，以维护如权限、授权或适应不同类型角色的分类。在声明角色时，采用了当事人角色、当事人、以及角色类型三级声明模式。
## 十一、声明角色概念数据模型
![](http://image.woshipm.com/wp-files/2019/03/h1LGBZLq82gX81azIrhw.png)
## 十二、当事人实体属性
![](http://image.woshipm.com/wp-files/2019/03/e1xbx2PCuk63bHA104FA.png)
## 十三、当事人角色属性
![](http://image.woshipm.com/wp-files/2019/03/q0xaiVXnSCEFa2uGoOuS.png)
## 十四、角色类型属性
![](http://image.woshipm.com/wp-files/2019/03/q5BiQcYXlcJfjBnYzvGJ.png)
## 十五、权限控制模型
权限控制对应于执行、访问功能等。在上下文应用中，建议使用混合式数据模型，有流程自定义的项目角色，同步支持自定义的项目角色。与功能相关部分模型在后续的业务功能中做解释。
## 十六、权限概念模型
![](http://image.woshipm.com/wp-files/2019/03/JNSmRNsNo7GJe2RGym8z.png)
## 十七、权限管理实体属性
![](http://image.woshipm.com/wp-files/2019/03/qEIRt8dvGtSPlwoRZrf0.png)
## 十八、概念模型总图
![](http://image.woshipm.com/wp-files/2019/03/Kvc51SITbC8fA7YWW499.png)
## 十九、客户管理数据模型设计
![](http://image.woshipm.com/wp-files/2019/03/Vwo03dFWRunlJu6PlgJU.png)
## 二十、联系人管理数据模型设计
修改补充：
跨org的组织机构；
org-user-role关联关系；
user group；
夸机构的职能组织。
## 二十一、算法模型
聚类分析分析模型：聚类分析支持的数据项包含年龄、地域、地区、背景、兴趣、学校学历等，支持自定义聚类数；算法支持k-means等多种聚类方法，算法模型具体参见算法工程师提供，包括算法参数设置部分；
预测分析模型：预测支持新产研发将需要的团队人员费用、资源需求等及后期销售收入预测等，详情参见算法模型设计文档；
关联分析模型：维度和数据指标（度量值）均支持关联分析，详情见数据分析说明文档；
精准营销模型：复合模型，按照不同的阶段，不同的目的，在不同数据的支持下，详情参见算法模型详细设计，功能对接CRM/ERP等SaaS系统。
## 二十二、AI+数据分析产品经理
模型是无形的且没有固定的产出物，本文以上非常适合对深度数据分析，数据挖掘感兴趣的读者。
另外笔者从《AI赋能-AI重新定义产品经理》一书中讲解了AI助力数据、AI落地产品的方向。而在另外一本书《产品经理进化论-AI+时代产品经理的思维方法》中讲解了AI寻找需求的方向。
《AI+时代产品经理的思维方法》比较适合创业者或者老板或者高阶的产品。而《AI赋能-AI重新定义产品经理》比较适合落地AI助力产品。
笔者对算法的讲解方式是：例如线性SVM算法，会分成两个方面讲解。一个方面是建议对技术感兴趣的同学继续琢磨其中的凸优化、KKT与SMO对偶、minmax与maxmin等提升运算效率的算法。另外一个反面建议AI+产品型还是多研究SVM应用来对数据进行分类，那么图片也是数据，所以主要精力应该是寻找和设计出图片分类的应用。

