# Nhibernate中One—to—One关系映射详解 - weixin_33985507的博客 - CSDN博客
2009年04月14日 17:42:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
最近对NHibernate的中持久化对象间关系初次涉及，发现第一次并不是很好理解其中关系分配，心里总是认为NHibernate框架关系分配不是很合理，但今天下午还就是拿了一个下午时间来研究One-to-One关系映射,相对其他one-to-many/many-to-one/many-to-mang关系而言，稍微简单理解一些，我发现这个One-to-one实质其实就是一个特殊的one-to-many关系映射,NHibernate中加以细分.不加以废话，切入正题.
持久化对象之间一对一的关联关系是通过 one-to-one元素定义的.在对持久化对象配置文件一个节点,为了方便了解后面的one-to-one中配置对该节点中涉及到属性加以罗列:
```
![ContractedBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedBlock.gif)![ExpandedBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif)Code
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)--one-to-one总体结构
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)<one-to-one
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)        name="PropertyName"                                (1)
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)        class="ClassName"                                  (2)
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)        cascade="all|none|save-update|delete"              (3)
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)        constrained="true|false"                           (4)
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)        fetch="join|select"                                (5)
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)        property-ref="PropertyNameFromAssociatedClass"     (6)
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)        access="field|property|nosetter|ClassName"         (7)
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)/>
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)--各个属性定义:
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)(1)name:属性名-必选属性
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)(2)Class:呗关联的类的名称(可选-默认是通过反射得到的属性类型)
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)(3)  cascade(级联) (可选) 表明操作是否从父对象级联到被关联的对象
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)(4)constrained(约束)表名该类对应的表对应的数据库数据库表，和被关联的对象所对应的数据库表之间，通过一个外键引用对主键进行约束。 这个选项影响Save()和Delete()在级联执行时的先后顺序以及 决定该关联能否被委托(也在schema export tool中被使用). 
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)(5)  fetch (可选 - 默认设置为select): 在外连接抓取或者序列选择抓取选择其一. 
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)(6)  property-ref: (可选) 指定关联类的属性名，这个属性将会和本类的主键相对应。如果没有指定，会使用对方关联类的主键。 
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif) 
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)(7)  access (可选 - 默认是 property): NHibernate用来访问属性的策略
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif) 
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)
```
在园子里看到一个大牛写的很不错，总结的恰到好处，简洁而切合实际应用,我就不加以啰嗦,收藏做日后备份
直接引用:[http://www.cnblogs.com/renrenqq/archive/2006/08/16/478198.html](http://www.cnblogs.com/renrenqq/archive/2006/08/16/478198.html)
