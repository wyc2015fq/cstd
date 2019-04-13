
# Neo4j入门 - 追求卓越,做到专业 - CSDN博客


2018年11月29日 14:36:12[Waldenz](https://me.csdn.net/enter89)阅读数：56


**图形数据库 Graph Database**
面向网络的数据库，将结构化数据存储在网络上而不是表中；
节点集 和连接节点的关系。（有时也称泡泡和箭头）；
官方英文释义：
a database that uses graph structures for semantic queries with nodes, edges and properties to represent and store data – independent of the way the data is stored internally. It’s really the model and the implemented algorithms that matter.
图数据的特点：
1. 包含节点和边；
2. 节点上有属性（键值对）；
3. 边有名字和方向，并总是有一个开始节点和结束节点；
4. 边也可以有属性；
![](https://img-blog.csdnimg.cn/20181129140545886.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
图数据库和关系型数据库的概念区别
![](https://img-blog.csdnimg.cn/20181204190548648.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
Neo4j安装
1. Neo4j 官网：[https://neo4j.com/](https://neo4j.com/)windows下载桌面安装包后安装注册账号登录
2. 创建新数据库,选择Create a Local Graph，输入名称TestDBGraph，选择默认数据库版本。
![](https://img-blog.csdnimg.cn/2018112914193954.png)![](https://img-blog.csdnimg.cn/20181129141950441.png)
3. 用自带样例编码
![](https://img-blog.csdnimg.cn/20181129142544658.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20181129142556577.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20181129142621364.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20181129142702610.png)
4. 查询  CQL代表Cypher查询语言
找到名为 “Tom Hanks” 的演员..。
```python
MATCH (tom {name: "Tom Hanks"}) RETURN tom
```
![](https://img-blog.csdnimg.cn/20181129142858289.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)


