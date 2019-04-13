
# neo4j︱与python结合的py2neo使用教程（四） - 素质云笔记-Recorder... - CSDN博客

2018年04月11日 18:55:21[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：5598


图数据库常规的有：neo4j（支持超多语言）、JanusGraph/Titan（分布式）、Orientdb，google也开源了图数据库Cayley（Go语言构成）、PostgreSQL存储RDF格式数据。
—- 目前的几篇相关：—–
[neo4j︱图数据库基本概念、操作罗列与整理（一）](https://blog.csdn.net/sinat_26917383/article/details/79883503)
[neo4j︱Cypher 查询语言简单案例（二）](https://blog.csdn.net/sinat_26917383/article/details/79850412)
[neo4j︱Cypher完整案例csv导入、关系联通、高级查询（三）](https://blog.csdn.net/sinat_26917383/article/details/79852596)
相关内容可参考：[The Py2neo v4 Handbook](http://py2neo.org/v4/index.html#)，还有中文文档：[neo4j 0.1.0a documentation](http://neo4j.com.cn/public/docs/index.html)、[Neo4j社区](http://neo4j.com.cn/)
---[一、创建与基本使用、属性查询](#一创建与基本使用属性查询)

---[1.1 创建节点与关系](#11-创建节点与关系)

---[1.2 子图Subgraphs](#12-子图subgraphs)

---[1.3 Walkable Types](#13-walkable-types)

---[1.4 连接已有图数据库 - .Graph()](#14-连接已有图数据库-graph)

---[1.5 其他应用](#15-其他应用)


---[二、查询方式](#二查询方式)

---[2.1 结果查询-.run/.data/.match](#21-结果查询-rundatamatch)

---[2.2 查找节点-.find/.find_one](#22-查找节点-findfindone)

---[2.3 更灵活的查询 - NodeMatcher](#23-更灵活的查询-nodematcher)

---[2.3  match() 或 match_one() 查找Relationship](#23-match-或-matchone-查找relationship)

---[2.4 类似set的重设 - push、setdefault、update](#24-类似set的重设-pushsetdefaultupdate)

---[2.5 删除 - .delete()/.delete_all()](#25-删除-deletedeleteall)


---[三、OGM - Object Graph Mapping](#三ogm-object-graph-mapping)

---[同时参考文献：](#同时参考文献)




---一、创建与基本使用、属性查询

---1.1 创建节点与关系

---举个简单的例子：

---from py2neo import Node, Relationship
a = Node("Person", name="Alice")
b = Node("Person", name="Bob")
ab = Relationship(a, "KNOWS", b)
>>> ab
>>> (alice)-[:KNOWS]->(bob)

---新建两个节点a、b，分别具有一个name属性值，还新建a与b之间有向关系ab，ab的label为KNOWS。

---其中：

---class Node(*labels, **properties)

---class Relationship(start_node, type, end_node, **properties)

---Node 和 Relationship 都继承了 PropertyDict 类，它可以赋值很多属性，类似于字典的形式，例如可以通过如下方式对 Node 或 Relationship 进行属性赋值，接着上面的代码，实例如下:

---a

---[

---'age'

---] =

---20

---b[

---'age'

---] =

---21

---r[

---'time'

---] =

---'2017/08/31'

---print(

---a

---, b, r)

---运行结果：

---(

---alice

---:Person

---{

---age

---:

---20

---,name:

---"Alice"

---}) (

---bob

---:Person

---{

---age

---:

---21

---,name:

---"Bob"

---}) (

---alice

---)

----

---[:KNOWS {time:"2017/08/31"}]

----

--->(

---bob

---)

---可见通过类似字典的操作方法就可以成功实现属性赋值。

---另外还可以通过 setdefault() 方法赋值默认属性，例如：

---a.setdefault(

---'location'

---,

---'北京'

---)
print(a)

--->>>

---(alice:Person {age:

---20

---,location:

---"北京"

---,name:

---"Alice"

---})

---另外也可以使用 update() 方法对属性批量更新，接着上面的例子实例如下：

---data

---=

---{
    '

---name'

---: '

---Amy

---',
    '

---age'

---: 21
}

---a

---.update(

---data

---)

---print

---(a)

---其中包含的节点属性有：

---hash(node) 返回node的ID的哈希值

---node[key] 返回node的属性值，没有此属性就返回None

---node[key] = value 设定node的属性值

---del node[key] 删除属性值，如果不存在此属性报KeyError

---len(node) 返回node属性的数量

---dict(node) 返回node所有的属性

---walk(node) 返回一个生成器且只包含一个node

---labels() 返回node的标签的集合

---has_label(label) node是否有这个标签

---add_label(label) 给node添加标签

---remove_label(label) 删除node的标签

---clear_labels() 清楚node的所有标签

---update_labels(labels) 添加多个标签，注labels为可迭代的

---其中连接的属性有：

---hash(relationship) 返回一个关系的hash值

---relationship[key] 返回关系的属性值

---relationship[key] = value 设定关系的属性值

---del relationship[key] 删除关系的属性值

---len(relationship) 返回关系的属性值数目

---dict(relationship) 以字典的形式返回关系的所有属性

---walk(relationship) 返回一个生成器包含起始node、关系本身、终止node

---type() 返回关系type

---1.2 子图Subgraphs

---class Subgraph(nodes, relationships) 子图是节点和关系不可变的集合。

---from

---py2neo import Node, Relationship

---a

---= Node(

---'Person'

---, name=

---'Alice'

---)
b = Node(

---'Person'

---, name=

---'Bob'

---)
r = Relationship(

---a

---,

---'KNOWS'

---, b)
s =

---a

---| b | r
print(s)
>>> ({(alice:Person {name:

---"Alice"

---}), (bob:Person {name:

---"Bob"

---})}, {(alice)-[:KNOWS]->(bob)})

---还可以通过 nodes() 和 relationships() 方法获取所有的 Node 和 Relationship，实例如下：

---print(s.

---nodes()

---)

---print(s.

---relationships()

---)

---另外还可以利用 & 取 Subgraph 的交集，例如：

---s1 = a | b | r
s2 = a | b

---print

---(s1 & s2)

--->>>

---(

---{(alice:

---Person

---{name:

---"Alice"

---}

---), (bob:

---Person

---{name:

---"Bob"

---}

---)}

---,

---{}

---)

---还可以进行一些额外操作：

---from py2neo import

---Node

---,

---Relationship

---, size, order
s = a | b | r

---print

---(s.

---keys

---())

---print

---(s.

---labels

---())

---print

---(s.

---nodes

---())

---print

---(s.

---relationships

---())

---print

---(s.

---types

---())

---print

---(

---order

---(s))

---print

---(

---size

---(s))

--->>>

---frozenset

---(

---{'name'}

---)

--->>>

---frozenset

---(

---{'

---Person

---'}

---)

--->>>

---frozenset

---(

---{(alice:

---Person

---{name:

---"Alice"

---}

---), (bob:

---Person

--->>> >>> >>>

---{name:

---"Bob"

---}

---)}

---)

--->>>

---frozenset

---(

---{(alice)-[:

---KNOWS

---]->(bob)}

---)

--->>>

---frozenset

---(

---{'

---KNOWS

---'}

---)

--->>> 2
>>> 1

---其中子图拥有的属性内容：

---subgraph | other | … 子图的并

---subgraph & other & … 子图的交

---subgraph - other - … 子图的差

---subgraph ^ other ^ … 子图对称差

---subgraph.keys() 返回子图节点和关系所有属性的集合

---subgraph.labels() 返回节点label的集合

---subgraph.nodes() 返回所有节点的集合

---subgraph.relationships() 返回所有关系的集合

---subgraph.types() 返回所有关系的type的集合

---order(subgraph) 返回子图节点的数目

---size(subgraph) 返回子图关系的数目

---1.3 Walkable Types

---Walkable Types是一个拥有遍历功能的子图。最简单的构造就是把一些子图合并起来:

---from

---py2neo import Node, Relationship

---a

---= Node(

---'Person'

---, name=

---'Alice'

---)
b = Node(

---'Person'

---, name=

---'Bob'

---)
c = Node(

---'Person'

---, name=

---'Mike'

---)
ab = Relationship(

---a

---,

---"KNOWS"

---, b)
ac = Relationship(

---a

---,

---"KNOWS"

---, c)
w = ab + Relationship(b,

---"LIKES"

---, c) + ac
print(w)
>>> (alice)-[:KNOWS]->(bob)-[:LIKES]->(mike)<-[:KNOWS]-(alice)

---另外我们可以调用 walk() 方法实现遍历，实例如下：

---from

---py2neo

---import

---walk

---for

---item

---in

---walk

---(

---w

---):

---print

---(

---item

---)
>>> 
(

---alice

---:Person

---{

---name

---:

---"Alice"

---})
(

---alice

---)

----

---[:KNOWS]

----

--->(

---bob

---)
(

---bob

---:Person

---{

---name

---:

---"Bob"

---})
(

---bob

---)

----

---[:LIKES]

----

--->(

---mike

---)
(

---mike

---:Person

---{

---name

---:

---"Mike"

---})
(

---alice

---)

----

---[:KNOWS]

----

--->(

---mike

---)
(

---alice

---:Person

---{

---name

---:

---"Alice"

---})

---可以看到它从 a 这个 Node 开始遍历，然后到 b，再到 c，最后重新回到 a。

---另外还可以利用 start_node()、end_node()、nodes()、relationships() 方法来获取起始 Node、终止 Node、所有 Node 和 Relationship，例如：

---print

---(

---w

---.start_node

---())

---print

---(

---w

---.end_node

---())

---print

---(

---w

---.nodes

---())

---print

---(

---w

---.relationships

---())
>>> (

---alice

---:Person

---{

---name

---:

---"Alice"

---})
>>> (

---alice

---:Person

---{

---name

---:

---"Alice"

---})
>>> ((

---alice

---:Person

---{

---name

---:

---"Alice"

---}), (

---bob

---:Person

---{

---name

---:

---"Bob"

---}), (

---mike

---:Person

---{

---name

---:

---"Mike"

---}), (

---alice

---:Person

---{

---name

---:

---"Alice"

---}))
>>> ((

---alice

---)

----

---[:KNOWS]

----

--->(

---bob

---), (

---bob

---)

----

---[:LIKES]

----

--->(

---mike

---), (

---alice

---)

----

---[:KNOWS]

----

--->(

---mike

---))

---相关属性：

---walk(walkable) 转为一个生成器包含节点和关系

---start_node() 返回walk()的起始节点

---end_node() 返回walk()的最后节点

---nodes() 返回walk()所有节点的元组

---relationships() 返回walk()所有关系的元组

---1.4 连接已有图数据库 - .Graph()

---在 database 模块中包含了和 Neo4j 数据交互的 API，最重要的当属 Graph，它代表了 Neo4j 的图数据库

---test_graph = Graph(
    "http://localhost:7474", 
    username="neo4j", 
    password="xxxx"
)

---test_graph

---,就连接上了电脑中默认的图数据库，就可以进行查询了。

---还可以利用 create() 方法传入 Subgraph 对象来将关系图添加到数据库中，实例如下：

---from

---py2neo import Node, Relationship, Graph

---a

---= Node(

---'Person'

---, name=

---'Alice'

---)
b = Node(

---'Person'

---, name=

---'Bob'

---)
r = Relationship(

---a

---,

---'KNOWS'

---, b)
s =

---a

---| b | r
graph = Graph(password=

---'123456'

---)
graph.

---create

---(s)

---另外我们也可以单独添加单个 Node 或 Relationship，实例如下：

---from

---py2neo import Graph, Node, Relationship
graph = Graph(password=

---'123456'

---)

---a

---= Node(

---'Person'

---, name=

---'Alice'

---)
graph.

---create

---(

---a

---)
b = Node(

---'Person'

---, name=

---'Bob'

---)
ab = Relationship(

---a

---,

---'KNOWS'

---, b)
graph.

---create

---(ab)

---1.5 其他应用

---查找是否存在节点 - exists(subgraph)

---print(test_graph.exists(node3))

---节点的度数

---test_graph.degree(node3)

---.

---二、查询方式

---2.1 结果查询-.run/.data/.match

---比较传统的方式：通过nodes的ID进行检索

---graph = Graph()

---\# 其中的数字对应的是节点，ID

---\# 这个ID不按顺序来的，要注意

---graph.nodes[

---1234

---]
graph.nodes.

---get

---(

---1234

---)

---还有一种方式，

---match

---的方式：

---\# .run/.data查询
test_graph.data("MATCH (a:Person {name:'You'}) RETURN a")
>>> [{'a': (c7d1cb9:Person {name:"You"})}]
list(test_graph.run("MATCH (a:Person {name:'You'}) RETURN a"))
>>>[('a': (c7d1cb9:Person {name:"You"}))]
test_graph.run("MATCH (a:Person {name:'You'}) RETURN a").data()
>>>[{'a': (c7d1cb9:Person {name:"You"})}]
\# 查询关系
test_graph.run("MATCH (a:Person {name:'You'})-[b:FRIEND]->(c:Person {name:'Johan'}  )   RETURN a,b,c")

---graph.run()，之中填写的是查询语句。查询的结果也可以转换为dataframe的格式：

---pd.DataFrame(test_graph.data("MATCH (a:Person {name:'Anna'}) RETURN a"))
                  a
0  {'name': 'Anna'}
1  {'name': 'Anna'}
2  {'name': 'Anna'}
3  {'name': 'Anna'}

---其中需要注意的是，查询出来的结果是dict/list格式的，并不是graph型，于是不能进行后续查询。

---查询出来的结果，可以标准化成一些表格的格式：

---\# graph查询

---graph

---.run

---(

---"MATCH (n:leafCategory) RETURN n LIMIT 25"

---)

---.data

---()

---\# list型

---graph

---.run

---(

---"MATCH (n:leafCategory) RETURN n LIMIT 25"

---)

---.to

---_data_frame()

---\# dataframe型

---graph

---.run

---(

---"MATCH (n:leafCategory) RETURN n LIMIT 25"

---)

---.to

---_table()

---\# table

---2.2 查找节点-.find/.find_one

---查找节点的个数：

---\# 节点个数

---len(graph

---.nodes

---)
len(graph

---.nodes

---.match

---(

---"leafCategory"

---))

---\# 某类别的节点个数

---通过find进行节点查询

---另外的可以通过

---find

---的方式进行查找：

---- .find,查找全部，需要传入的不定参数label、property_key、property_value、limit，返回符合筛选条件节点的生成器

---- .find_one,只查找单节点，需要传入的不定参数label、property_key、property_value，返回符合筛选条件一个节点，即使多个都满足，也会返回唯一节点

---\# 查找全部

---graph=test_graph.find(label=

---'Person'

---)

---for

---node

---in

---graph:
    print(node)
>>>(b54ad74:Person {age:

---18

---,name:

---"Johan"

---})
(b1d7b9d:Person {name:

---"Rajesh"

---})
(cf7fe65:Person {name:

---"Anna"

---})
(d780197:Person {name:

---"Julia"

---})

---\# 查找单节点

---test_graph.find_one(label=

---'Person'

---,property_key=

---'name'

---,property_value=

---'You'

---)

--->>>

---(c7d1cb9:Person {name:

---"You"

---})

---此时返回的是可复用的图类型，就可以去衡量相关属性。

---节点是否存在的判断

---\# 该节点是否存在

---test_graph

---.exists

---(graph

---.nodes

---[

---1234

---])

---2.3 更灵活的查询 - NodeMatcher

---py2neoV3有这个函数，py2neoV4没有该函数了，各位注意！！变成这个函数了：

---class py2neo.matching.NodeMatcher(graph)

---参考

---[v4 Handbook](https://py2neo.org/v4/matching.html#py2neo.matching.NodeMatcher)

---NodeMatcher是为更好的查询节点，支持更多的查询条件，比graph更友好

---selector = NodeMatcher(test_graph)
\#selector = NodeSelector(test_graph)
list(selector.select("Person", name="Anna"))
list(selector.select("Person").where("_.name =~ 'J.*'", "1960 <= _.born < 1970"))

---在这里我们用 NodeSelector 来筛选 age 为 21 的 Person Node，实例如下：

---from

---py2neo import Graph, NodeSelector
graph = Graph(password=

---'123456'

---)

---selector

---= NodeMatcher(graph)
\#

---selector

---= NodeSelector(graph)
persons =

---selector

---.

---select

---(

---'Person'

---, age=

---21

---)
print(list(persons))

---另外也可以使用 where() 进行更复杂的查询，例如查找 name 是 A 开头的 Person Node，实例如下：

---from

---py2neo import Graph, NodeSelector
graph = Graph(password=

---'123456'

---)

---selector

---= NodeMatcher(graph)
persons =

---selector

---.

---select

---(

---'Person'

---).

---where

---(

---'_.name =~ "A.*"'

---)
print(list(persons))

---另外也可以使用 order_by() 进行排序：

---from

---py2neo import Graph, NodeSelector
graph = Graph(password=

---'123456'

---)

---selector

---= NodeMatcher(graph)
persons =

---selector

---.

---select

---(

---'Person'

---).order_by(

---'_.age'

---)
print(list(persons))

---还包括：

---first()返回单个节点

---limit(amount)返回底部节点的限值条数

---skip(amount)返回顶部节点的限值条数

---order_by(*fields)排序

---where(*conditions, **properties)筛选条件

---2.3  match() 或 match_one() 查找Relationship

---.match 匹配关系

---.match_one，匹配并返回所有满足条件的一条关系

---// 此时start_node为节点

---for

---rel

---in

---test_graph.match(start_node=node3, rel_

---type

---=

---"FRIEND"

---):
    print(rel.end_node()[

---"name"

---])
>>>Johan
Julia
Andrew

---\# match_one

---test_graph.match_one(start_node=node3, rel_

---type

---=

---"FRIEND"

---)
>>> (c7d1cb9)-[:FRIEND]->(b54ad74)

---2.4 类似set的重设 - push、setdefault、update

---push 跟set一样：更新、添加，push(subgraph) 更新节点、关系或子图

---push

---node = test_graph.find_one(label=

---'Person'

---)
node[

---'age'

---] =

---18

---test_graph.push(node)
print(test_graph.find_one(label=

---'Person'

---))

--->>>

---(b54ad74:Person {age:

---18

---,name:

---"Johan"

---})

---PropertyDict 类属性

---a

---= Node(

---'Person'

---, name=

---'Alice'

---)

---a

---[

---'age'

---] =

---20

---因为a集成了PropertyDict 类属性，所以可以像dict一样进行简单赋值或添加。

---setdefault() 方法

---a.setdefault(

---'location'

---,

---'北京'

---)
print(a)

--->>>

---(alice:Person {age:

---20

---,location:

---"北京"

---,name:

---"Alice"

---})

---但如果赋值了 location 属性，则它会覆盖默认属性

---update() 方法对属性批量更新

---data

---=

---{
    '

---name'

---: '

---Amy

---',
    '

---age'

---: 21
}

---a

---.update(

---data

---)

---print

---(a)

---2.5 删除 - .delete()/.delete_all()

---delete(subgraph) 删除节点、关系或子图

---delete_all() 删除数据库所有的节点和关系

---from

---py2neo import Graph
graph = Graph(password=

---'123456'

---)
node = graph.find_one(label=

---'Person'

---)
relationship = graph.match_one(rel_type=

---'KNOWS'

---)
graph.

---delete

---(relationship)
graph.

---delete

---(node)

---在删除 Node 时必须先删除其对应的 Relationship，否则无法删除 Node。

---三、OGM - Object Graph Mapping

---参考：

---[https://cuiqingcai.com/4778.html](https://cuiqingcai.com/4778.html)

---可以实现一个对象和 Node 的关联，例如：

---from

---py2neo.ogm

---import

---GraphObject, Property, RelatedTo, RelatedFrom

---class

---Movie

---(

---GraphObject

---)

---:
    __primarykey__ = 'title'
    title =

---Property

---()

---released =

---Property

---()

---actors =

---RelatedFrom

---('

---Person

---', '

---ACTED_IN

---')

---directors =

---RelatedFrom

---('

---Person

---', '

---DIRECTED

---')

---producers =

---RelatedFrom

---('

---Person

---', '

---PRODUCED

---')

---class

---Person

---(

---GraphObject

---)

---:
    __primarykey__ = 'name'
    name =

---Property

---()

---born =

---Property

---()

---acted_in =

---RelatedTo

---('

---Movie

---')

---directed =

---RelatedTo

---('

---Movie

---')

---produced =

---RelatedTo

---('

---Movie

---')

---我们可以用它来结合 Graph 查询，例如：

---from

---py2neo

---import

---Graph

---from

---py2neo.ogm

---import

---GraphObject, Property
graph = Graph(password=

---'123456'

---)

---class

---Person

---(GraphObject)

---:

---__primarykey__ =

---'name'

---name = Property()
    age = Property()
    location = Property()
person = Person.select(graph).where(age=

---21

---).first()
print(person)
print(person.name)
print(person.age)
>>><Person name=

---'Alice'

--->
>>>Alice
>>>

---21

---这样我们就成功实现了对象和 Node 的映射。

---我们可以用它动态改变 Node 的属性，例如修改某个 Node 的 age 属性，实例如下：

---person = Person

---.select

---(graph)

---.where

---(age=

---21

---)

---.first

---()
print(person.__ogm__

---.node

---)
person

---.age

---=

---22

---print(person.__ogm__

---.node

---)
graph

---.push

---(person)
>>>(ccf5640:Person {age:

---21

---,location:

---"北京"

---,name:

---"Mike"

---})
>>>(ccf5640:Person {age:

---22

---,location:

---"北京"

---,name:

---"Mike"

---})

---另外我们也可以通过映射关系进行 Relationship 的调整，例如通过 Relationship 添加一个关联 Node，实例如下：

---from

---py2neo import Graph

---from

---py2neo.ogm import GraphObject,

---Property

---, RelatedTo
graph = Graph(password=

---'123456'

---)

---class

---Person(GraphObject):
    __primarykey__ =

---'name'

---name =

---Property

---()
    age =

---Property

---()
    location =

---Property

---()
    knows = RelatedTo(

---'Person'

---,

---'KNOWS'

---)
person = Person.

---select

---(graph).

---where

---(age=

---21

---).first()
print(list(person.knows))
new_person = Person()
new_person.name =

---'Durant'

---new_person.age =

---28

---person.knows.

---add

---(new_person)
print(list(person.knows))

---运行结果：

---[

---<

---Person

---name

---=

---'Bob'

--->

---]
[

---<

---Person

---name

---=

---'Bob'

--->

---,

---<

---Person

---name

---=

---'Durant'

--->

---]

---这样我们就完成了 Node 和 Relationship 的添加，同时由于设置了 primarykey 为 name，所以不会重复添加。

---但是注意此时数据库并没有更新，只是对象更新了，如果要更新到数据库中还需要调用 Graph 对象的 push() 或 pull() 方法，添加如下代码即可：

---graph.

---push

---(person)

---也可以通过 remove() 方法移除某个关联 Node，实例如下：

---person = Person.

---select

---(graph).

---where

---(name=

---'Alice'

---).

---first

---()
target = Person.

---select

---(graph).

---where

---(name=

---'Durant'

---).

---first

---()
person.knows.remove(target)
graph.push(person)
graph.

---delete

---(target)

---这里 target 是 name 为 Durant 的 Node，代码运行完毕后即可删除关联 Relationship 和删除 Node。

---以上便是 OGM 的用法，查询修改非常方便，推荐使用此方法进行 Node 和 Relationship 的修改。

---更多内容可以查看：

---[http://py2neo.org/v3/ogm.html\#module-py2neo.ogm](http://py2neo.org/v3/ogm.html#module-py2neo.ogm)

---。

---同时参考文献：

---[[Neo4j系列四]Neo4j的python操作库py2neo之一](http://mp.weixin.qq.com/s?__biz=MzU2NzQ2OTAwMw==&mid=2247483678&idx=1&sn=49328cb13d20f51bfe01891beea76fee&chksm=fc9df7bacbea7eacfad5e24719f254ba0f77f6b9b3634f936648fee1c2c75aae23ba3e2a4e8f&mpshare=1&scene=1&srcid=0410in11eSC9aUg7NAQirslR#rd)

---[[Neo4j系列五]Neo4j的python操作库py2neo之二](http://mp.weixin.qq.com/s?__biz=MzU2NzQ2OTAwMw==&mid=2247483683&idx=1&sn=8c1594f13cb1a830e06e246ba44f2b0b&chksm=fc9df787cbea7e9152ea6bd2f5a988ee5f9aab23b0c04d2e45a73f8586037e93ff11f6fda743&mpshare=1&scene=1&srcid=0410VdUea35Ra6lVRoEyE4dz#rd)

---[[Neo4j系列六]Neo4j的python操作库py2neo之三](http://mp.weixin.qq.com/s?__biz=MzU2NzQ2OTAwMw==&mid=2247483686&idx=1&sn=979a48b626588c4549c246d919bb9cdc&chksm=fc9df782cbea7e9429daefc3f053e45509cd45e6873a445b57d7090d729521d184dad93ecc57&mpshare=1&scene=1&srcid=0410V76rvZCUMi9XWQZrSqix#rd)

---[Neo4j简介及Py2Neo的用法](https://cuiqingcai.com/4778.html)


