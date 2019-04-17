# neo4j︱Cypher 查询语言简单案例（二） - 素质云笔记/Recorder... - CSDN博客





2018年04月08日 12:54:51[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：3401








图数据库常规的有：neo4j（支持超多语言）、JanusGraph/Titan（分布式）、Orientdb，google也开源了图数据库Cayley（Go语言构成）、PostgreSQL存储RDF格式数据。

—- 目前的几篇相关：—– 
[neo4j︱图数据库基本概念、操作罗列与整理（一）](https://blog.csdn.net/sinat_26917383/article/details/79883503)
[neo4j︱Cypher 查询语言简单案例（二）](https://blog.csdn.net/sinat_26917383/article/details/79850412)
[neo4j︱Cypher完整案例csv导入、关系联通、高级查询（三）](https://blog.csdn.net/sinat_26917383/article/details/79852596)
neo4j系列第二篇，一个简单的demo案例，从构建-基本内容查询。 

![这里写图片描述](https://img-blog.csdn.net/20180408123410424?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


# 一、三要素:节点、关系、模式

### 1.1 节点

**基本用法：**

```
MATCH (node:Label) RETURN node.property

MATCH (node1:Label1)-->(node2:Label2)
WHERE node1.propertyA = {value}
RETURN node2.propertyA, node2.propertyB
```

我们通常用圆括号来包含节点标示符（当然也可以不用）。里面的内容也叫变量，变量不是单指某一个节点，而是服从条件的节点组合。如(p: Person)，如果想添加属性：(p: Person {name: “yinglish”})。 注意：节点标签、关系类型和属性名称区分大小写。 

其中节点之间用:’–>’，包含关系的可以用’->’

### 1.2 关系

有以下几种关系型写法：

```
-[rel:KNOWS]-> //单关系，关系的变量名为rel，类型是 KNOWS
(a)-[:KNOWS|:LIKE]->(b) //查询a到b的两类关系
-[{since:2010}]-> //附加属性
-[:KNOWS*..4]-> example: a -[rel: type*minHop..maxHop]-> b  
//从a节点到b节点最少可经过minHop、最大可经过maxHop个节点
```

关系表达,cypher语法: 
![此处输入图片的描述](https://s3.amazonaws.com/dev.assets.neo4j.com/wp-content/uploads/cypher_pattern_simple.png)
举例：

```
MATCH (n1:Label1)-[rel:TYPE]->(n2:Label2)
WHERE rel.property > {value}
RETURN rel.property, type(rel)
```

### 1.3 模式 Patterns

其功能为:`Patterns can be used to MATCH and CREATE data, but also (evaluating to a list of paths) in expressions, predicates and results.`

patterns 模式由一个或多个路径组成，路径间用逗号分隔，一条路径就是一个节点和关系的序列，这个序列指定开始和结束节点，如：(a) –> (b)，这条路径开始于a，通过outgoing关系指向b。如果你不在乎具体是哪个节点，不用指定它，只需要用空括号来表示就可以了，如：a –>()–>b。模式是cypher中的关键部分，灵活书写模式能帮我们正确获取、实现推测计算等等各种任务。
- friend-of-a-friend (user)-[:KNOWS]-(friend)-[:KNOWS]-(foaf)
- shortest path: path = shortestPath( (user)-[:KNOWS*..5]-(other) )
- collaborative filtering 

(user)-[:PURCHASED]->(product)<-[:PURCHASED]-()-[:PURCHASED]->(otherProduct)
- tree navigation 

(root)<-[:PARENT*]-(leaf:Category)-[:ITEM]->(data:Product)

比较费解…

.

# 二、使用案例demo

从0开始构建一个非常小的关系网络结构，来简单应用cypher语言。 

来源：[GraphGist: First Steps with Cypher by Michael Hunger](https://s3.amazonaws.com/dev.assets.neo4j.com/wp-content/uploads/cypher_pattern_simple.png)

### 1、创建自己’You’的节点

```
CREATE (you:Person {name:"You"})
RETURN you
```

![这里写图片描述](https://img-blog.csdn.net/20180408123634562?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 2、创建与自己关系的节点

```
MATCH (you:Person {name:"You"})
CREATE (you)-[like:LIKE]->(neo:Database {name:"Neo4j" })
RETURN you,like,neo
```

![这里写图片描述](https://img-blog.csdn.net/20180408123643214?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

match到’You’节点之后，新建一条关系，是’You’-‘like’-‘neo4j’。 

当create之后，return之后，就会返回三个实体，2个节点+1条关系。 

match中，变量-变量，可以用’–>’，关系型的指向用’->’
### 3、批量创建与自己有关的朋友

```
MATCH (you:Person {name:"You"})
FOREACH (name in ["Johan","Rajesh","Anna","Julia","Andrew"] |
 CREATE (you)-[:FRIEND]->(:Person {name:name}))
```

![这里写图片描述](https://img-blog.csdn.net/20180408123650832?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其中，foreach表示循环，反复create。再来看一个foreach的案例（from:[3.3.15. FOREACH](https://neo4j.com/docs/developer-manual/current/cypher/clauses/foreach/)）：

```
MATCH p =(begin)-[*]->(END )
WHERE begin.name = 'A' AND END .name = 'D'
FOREACH (n IN nodes(p)| SET n.marked = TRUE )
```

### 4、查询你的朋友圈

```
match (you {name:'You'})-[:FRIEND]->(yf)
return you,yf
```

![这里写图片描述](https://img-blog.csdn.net/20180408123657806?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

通过关系查询’You’的朋友圈

### 5、新建朋友圈之外的二度关系

```
match (neo:Database {name : 'Neo4j'})
create (anna:Person {name:'Anna'})-[:FRIEND]->(ama:Person:Expert {name:'Amanda'})-[:WORKED_WITH]->(neo)
return neo,anna,ama
```

![这里写图片描述](https://img-blog.csdn.net/20180408123706287?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

创建了’Anna’-‘Amanda’的朋友关系，创建了’Amanda’-‘neo’的工作关系。 

同时，定义了’Amanda’的’Expert’身份。
### 6、查询：我通过朋友圈的谁来学习neo4j

```
match (you:Person {name:"You"}) 
match (expert)-[:WORKED_WITH]->(neo:Database {name:'Neo4j'})
match path = shortestPath((you)-[:FRIEND*..5]-(expert))
return path,you,expert,neo
```

![这里写图片描述](https://img-blog.csdn.net/20180408123713959?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

先定位自己you,然后去寻找朋友中谁懂Neo4j：person->neo，然后寻找最短路径的：you-person即可达到目的。 

不能用you直接关联neo，而是通过朋友，二度跳转到neo4j。 

此时的`(expert)->(neo)`值得新手去理解一下，这里expert不是单独指定到某个人，而是指代所有WORKED_WITH neo的人。
## 参考文献：

[Intro to Cypher](https://neo4j.com/developer/cypher-query-language/#_create_a_record_for_yourself)
[GraphGist: First Steps with Cypher](https://neo4j.com/developer/graphgist/first-steps-with-cypher#_find_someone_in_your_network_who_can_help_you_learn_neo4j)

## [我的neo4j学习笔记](https://blog.csdn.net/yinglish_/article/details/53224133)


**公众号“素质云笔记”定期更新博客内容：**
![这里写图片描述](https://img-blog.csdn.net/20180226155348545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)









