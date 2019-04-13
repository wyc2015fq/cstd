
# Neo4j  set 为已存在的节点创建标签 - 追求卓越,做到专业 - CSDN博客


2018年11月29日 19:36:48[Waldenz](https://me.csdn.net/enter89)阅读数：1273


w3cshool中关于set子句解释如下：
`Neo4j CQL已提供SET子句来执行以下操作。
向现有节点或关系添加新属性
添加或更新属性值`殊不知，set子句还可用来**为已存在的节点创建标签**
如下图1，三个节点name都为鱼暖暖，其中一个节点没有label，其余两个为Coder
添加标签语句： 为name为鱼暖暖的节点都添加标签Coder1，如图2
`MATCH (e {name:"鱼暖暖"}) set e:Coder1 return e`或者 根据id查询出某个节点，然后创建标签。如图3
`MATCH (e {name:"鱼暖暖"}) where id(e)=360 set e:Coder2 return e`
![](https://img-blog.csdnimg.cn/20181129193317192.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20181129194046568.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20181129194129389.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)

