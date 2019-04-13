
# Neo4j CQL 常用语句 - 追求卓越,做到专业 - CSDN博客


2018年11月29日 15:21:17[Waldenz](https://me.csdn.net/enter89)阅读数：93


**Node节点语法**
在cypher里面通过用一对小括号()表示一个节点，它在cypher里面查询形式如下：
1.  () 代表匹配任意一个节点
2.  (node1) 代表匹配任意一个节点，并给它起了一个别名
3.  (:Lable) 代表查询一个类型的数据
4.  (person:Lable) 代表查询一个类型的数据，并给它起了一个别名
5.  (person:Lable {name:"小王"}) 查询某个类型下，节点属性满足某个值的数据
6.  (person:Lable {name:"小王",age:23})　节点的属性可以同时存在多个，是一个AND的关系
**关系语法**
关系用一对-组成，关系分有方向的进和出，如果是无方向就是进和出都查询
1.  --> 指向一个节点
2.  -[role]-> 给关系加个别名
3.  -[:acted_in]-> 访问某一类关系
4.  -[role:acted_in]-> 访问某一类关系，并加了别名
5.  -[role:acted_in {roles:["neo","hadoop"]}]->   访问某一类关系下的某个属性的关系的数据
```python
//创建两个节点
Create(Yu:Coder{name:'鱼暖暖',sex:'男',hobby:'NBA'})
Create(Liang:Photographer{name:'凉暖暖',sex:'女',hobby:'aa'})
//查到节点1变量名：n, 查到节点2变量名：m ；关联关系- ；防止Math的节点不存在，关联连接的时候用merge,如果节点不存在创建；
//关系： [:关系data]    
//连接：(n)-[:关系]->(m)
Match(n:Coder{name:'鱼暖暖'})
Match(m:Photographer{name:'凉暖暖'})
Merge(n)-[:LOVE{years:'five years more'}]->(m)
```
![](https://img-blog.csdnimg.cn/20181129145207321.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
查询标签为Coder的节点以及所有从它出发的节点
`MATCH (e:Coder)-[rel]-(n) return e,n`![](https://img-blog.csdnimg.cn/20181129181507116.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
查询Tom Hanks参演的电影以及这些电影的导演：
`MATCH (n {name:"Tom Hanks"})-[:ACTED_IN]->(m)<-[:DIRECTED]-(d) RETURN n,m,d LIMIT 10`![](https://img-blog.csdnimg.cn/20181204191142365.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
参考：[https://blog.csdn.net/u010454030/article/details/53131229](https://blog.csdn.net/u010454030/article/details/53131229)

