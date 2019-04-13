
# Neo4j通过id匹配数据 - 追求卓越,做到专业 - CSDN博客


2018年11月29日 15:54:40[Waldenz](https://me.csdn.net/enter89)阅读数：463


根据属性查询数据。查询name为Tom Hanks的节点。
`match (n {name:'Tom Hanks'}) return n`但是通过id匹配数据，不能像其它普通属性一样直接match
`MATCH (n) WHERE id(n)=251 RETURN n`
![](https://img-blog.csdnimg.cn/20181129155116188.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20181129155135184.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)

