# neo4j-rest-client使用摘要 - =朝晖= - 博客园
# [neo4j-rest-client使用摘要](https://www.cnblogs.com/dhcn/p/7124901.html)
1、使用它的原因，与django搭配的最好的neomodel目前只支持到v2.2，我已给官方发了issue，官方也回复了，马上修改并发布（老外对开源项目的负责态度让人感动)
2、这个库的文档中大概描述下面几种元素：元素、关系、标签、索引、查询、过滤、遍历、扩展、事务和批处理，其中前五个应该用的比较多，第六个看查询具体条件。
3、标签可以算是一种非常方便的节点访问途径，不过请注意标签的get和model里面的get不一样，返回的是一个可遍历对象。再就是查询的返回方式类似SQL，返回的是一个列表的列表。
4、用查询肯定要对Cypher比较熟悉:文档地址：http://neo4j.com/docs/cypher-refcard/current/，说实话，能用RefCard这种模式的文档，这个东西应该不算复杂，当然了，可能我用到的比较简单
5、一个gdb单例实现：
- gdb = None  
- def get_gdb():  
- global gdb  
- if gdb is None:  
-         gdb = GraphDatabase("http://neouser:neopassword@127.0.01:7474/db/data/")  
- return gdb  
- 
