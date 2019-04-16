# 数据库设计(Database Design) - 彩虹糖的博客 - CSDN博客





2018年06月18日 10:57:47[彩虹糖梦](https://me.csdn.net/caozixuan98724)阅读数：2317








# 数据库设计（DB Design）

## 提纲
- DB设计 
- 特点
- 目的
- 方法 
- ER模型
- Mapping
- 规范化


## 数据库设计综述
- 信息系统（Information System）：可以通过组织收集管理控制和传播的信息资源。数据库就是信息系统的基本组成。
- 应对软件危机所产生的结构化开发方法：Information Systems Lifecycle(ISLC) 
- collect requirements
- design system 
- implement system
- maintain system

- 数据库应用系统开发包括 
- 数据库设计（Database Design） 
- **数据库设计的目的**： 
- **帮助理解数据的含义**
- **促进关于信息交流的沟通**

- 数据库设计的三个阶段 
- 数据库概念上的设计(conceptual database design)
- 数据库逻辑上的设计(logical database design)
- 数据库物理上的设计(physcal database design)

- 关系图![](https://i.imgur.com/785Y4Mq.png)
- 数据库管理系统选择(DBMS Selection) 
- define terms of reference of study
- shortlist two or three products
- evaluate products
- recommend selection and produce report

- 应用设计(Application Design) 
- 包括的主要两个方面 
- 用户接口设计
- 事务设计(transaction design)

- 事务 
- 概念：一个用户或一个应用程序所执行的访问或修改数据库的一个或一系列动作。
- 类型：retrieval,update,mixed


## E-R 模型
- 使用E-R模型的原因：数据库设计者，程序员和终端用户看待数据的方式不同
- 定义：E-R model is a model for communication that is **non-technical** and **free of ambiguities**
- 
E-R模型中的基本概念
- 实体类型(Entity Types) 
- 概念： a group of objects with the same properties identified by an enterprise as having an independent existence.
- 实例和表示：
- ![](https://i.imgur.com/7tjEEen.png)![](https://i.imgur.com/KM6L90N.png)- 关系类型（Relationship Types） 
- 概念： a set of meaningful associations among entity types
- 表示：![](https://i.imgur.com/2JXRtHY.png)
- 关系的度(degree):number of participating entities in a relationship
- complex relationships:度大于2的关系
- recursive relationship:![](https://i.imgur.com/V1bxnwf.png)
- 两个实体之间可以有多个关系：![](https://i.imgur.com/WzrwNLX.png)
- 属性(Attributes) 
- attribute:a property of an entity or a relationship type.
- attribute domain: the set of allowable values for one or more attributes.
- simple attribute: an attribute composed of a single component with an independent existence.
- composite attribute: an attribute composed of multiple components, each with an independent existence.
- single-valued attribute
- multi-valued attribute（像数组）
- derived attribute
- Keys: candidate key, promary key, conposite key
- 例子：![](https://i.imgur.com/1ncKOeY.png)
- **实体类型**
- 强实体：不依赖于其他实体的实体。
- 弱实体：依赖于其他实体的实体。(**a characteristic of a weak entity is that each entity occurrence cannot be uniquely identified using only the attributes associated with taht entity type**)
- 实例：![](https://i.imgur.com/p3YETu6.png)
- Structure Constraints 
- Multipliity（数量上的对应关系）注意含义
- ![](https://i.imgur.com/twm9Auj.png)
- Multiplicity is made up of two types of restrictions on relationships: **cardinality** (实体数量上的限制：最大值)and **participation**（是否参与的限制：最小值）
- ![](https://i.imgur.com/XDyH4Fa.png)
- E-R模型的问题 
- 扇形陷阱(fan trap):当一个实体与其他实体之间存在两个或更多的一对多联系时，就有可能存在扇形陷阱。
- ![](https://i.imgur.com/hdzTYKN.png)
- 显然我们无法回答上图的问题，为了避免这种问题，我们将E-R图改为这种形式：
- ![](https://i.imgur.com/aCXQga6.png)
- 深坑陷阱(chasm trap)：在关联实体的通路上存在一个或多个多样性最小为0的联系时，就可能产生深坑陷阱。
- ![](https://i.imgur.com/TIsDjgI.png)
- 改进方法：添加联系以标识遗漏实体的联系。
- ![](https://i.imgur.com/i2T1xsg.png)

## 增强E-R模型
- 
这一部分还后面映射九步算法中的8,9两步息息相关。

- 增强E-R模型可以理解为在原有模型的基础上增加了一下OOP的东西
- mandatory和optional，disjoint和nondisjoint 2*2可以形成四种组合
- 对应如下的图片：
- ![](https://i.imgur.com/GXzwnMg.jpg)

## 由E-R模型向关系模型映射
- E-R模型和关系模型的主要区别 
- 关系模型用主键和外键隐形的表示参与的实体
- 关系模型的列不存在多值

- 对比图![](https://i.imgur.com/zsICoi7.png)
- 模型转化的九步算法： 
- 转化强实体： 
- 创建新表
- 表的列包括所有单值实体属性，并把复合属性拆开放进去
- 找到主键和候选键（找不到手工加）
- ![](https://i.imgur.com/MfLSTy9.png)
- 转化一对一弱实体： 
- 找到这个实体的依赖方
- 把这个实体的单值属性和复合属性拆开加入到依赖方
- ![](https://i.imgur.com/ukjbiRg.png)
- 转化一对多或多对多弱实体： 
- 创建新表
- 表的列包括所有单值实体属性，并把复合属性拆开放进去
- 找主键：对于一对多关系，主键包括两部分本实体的部分键和依赖实体的主键。对于多对多关系，创建一个新列来做主键，然后参考步骤6
- ![](https://i.imgur.com/krnI6SJ.png)
- ![](https://i.imgur.com/RPHQCfc.png)
- 转化一对一关系： 
- 确定参与一对一关系的两张表
- 选取一张表（通常是完全参与的表）
- 这张完全参与的表加一个外键与另一张表的主键相关联
- （其他地方解释不清楚）
- ![](https://i.imgur.com/wFBJa5H.png)
- 转化一对多关系： 
- 假定S是“多”那一方，T是“一”那一方
- S添加外键关联T的主键
- ![](https://i.imgur.com/6l6Vcjr.png)
- 转化多对多关系 
- 创建一张新表
- 这张表包含T的种种值
- 外键是原先两张表的主键
- 把外键集合作为主键
- ![](https://i.imgur.com/A8bjP4Z.png)
- ![](https://i.imgur.com/AH2xXSO.png)
- 对于多值（类似数组）的处理 
- 创建一张新表
- 这张表只有一个值
- 然后当一对多去处理
- ![](https://i.imgur.com/7Gvdjvt.png)
- {Manadatory, Or} 
- 继续看上面关于E-R模型的那张图，这其实是最好处理的一种情况。
- 我们发现，所有子类就可以替代父类，因此我们没有必要创建父类表，直接用子类表就可以了。
- ![](https://i.imgur.com/hT37VEN.png)
- 可以看到，staff这张表不存在了
- 对于其他三种情况：在保留父类的情况下使用子类，在父类和子类间通过foreignkey来继承其相关属性。 

*![](https://i.imgur.com/THX89bs.png)
## 规范化(Normalization)
- 什么是规范化？生成一组既具有所期望的特性又能瞒住企业数据需求的关系的技术
- 性质： 
- 属性的个数最少
- 具有紧密逻辑联系的属性均在同一个关系中
- 最少的冗余，即每个属性只出现一次

- 优点： 
- 易于用户访问
- 数据易于维护
- 计算机存储空间小

- 缺点 ： 
- 性能相对降低

- 三种异常 
- 插入异常
- 删除异常
- 修改异常

- 函数依赖（functional dependency） 
- 在一个关系中，一个A只对应一个B记做 A->B(一个A不能对应多个B)
- 举例：![](https://i.imgur.com/bjjkCvL.png)
- 完全函数依赖
- 部分函数依赖
- 举个例子就可以：
- ![](https://i.imgur.com/KvmxQ2q.png)
- 传递依赖（transitive dependency）
- A->B, B->C 得到A->C
- 第一范式(1NF) 
- 每一行和每一列相交的位置有且仅有一个值
- 可以理解成为了避免ER模型中的组合属性啥的，拆成单值（都应该符合吧）

- 第二范式 
- 满足第一范式的要求，并且每个非主关键字属性完全依赖于主关键字
- 例子：
- staffNo,companyNo→staffName,staffAge,staffBirthDay,companyName,companyAge 

拆成staffNo→staffName,staffAge,staffBirthDay,companyNo 

companyNo→companyName,companyAge- 这样可以避免多个无关的实体混合在一张表中，并使它们的关系通过外键连接
- 第三范式 
- 满足第一范式和第二范式的要求并且所有非主关键字属性都不传播依赖于主属性的关系
- 也就是说任何非主属性不依赖于其它非主属性（在2NF基础上消除传递依赖）
- 这个比较难理解啊
- 这个[博客](https://blog.csdn.net/h330531987/article/details/71194540)有例子讲的还可以





























