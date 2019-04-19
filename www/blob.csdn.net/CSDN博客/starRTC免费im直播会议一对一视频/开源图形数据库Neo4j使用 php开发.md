# 开源图形数据库Neo4j使用 php开发 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年08月07日 15:22:39[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：227
先看看它的示例数据
打开 Neo4j Browser
:play movie graph
写代码，然后点play执行
Cypher, the graph query language.Neo4j提供了Cypher查询语言，它类似于关系型数据库中的SQL语句。
(1) 创建一个节点：create (n: Person {name: "Dennis"}) return n
插入一个Person类别的节点，且这个节点有一个属性name，属性值为Andres
CREATE (n:Person { name : 'Andres'});
插入边。插入一条a到b的有向边，且边的类别为Follow
MATCH (a:Person),(b:Person) WHERE a.name = 'Node A' AND b.name = 'Node B' CREATE (a)-[r:Follow]->(b);
(2) 查询所有的Person节点：match (n: Person) return n
更新节点。更新一个Person类别的节点，设置新的name。
MATCH (n:Person { name: 'Andres' }) SET n.name = 'Taylor';
1.Create: insert movie data into the graph
create语句用于Insert graph data
2.Find: retrieve individual movies and actors
如：Find the actor named "Tom Hanks".
MATCH (tom {name: "Tom Hanks"}) RETURN tom
查询名为"Dennis"的Person节点：match (n: Person) where n.name="Dennis" return n
Find 10 people:
MATCH (people:Person) RETURN people.name LIMIT 10
List all Tom Hanks movies
MATCH (tom:Person {name: "Tom Hanks"})-[:ACTED_IN]->(tomHanksMovies) RETURN tom,tomHanksMovies
Tom Hanks' co-actors
MATCH (tom:Person {name:"Tom Hanks"})-[:ACTED_IN]->(m)<-[:ACTED_IN]-(coActors) RETURN coActors.name
How people are related to "Cloud Atlas"
MATCH (people:Person)-[aaa]-(:Movie {title: "Cloud Atlas"}) RETURN people.name, Type(aaa)
3.Query: discover related actors and directors
MATCH p=shortestPath( (bacon:Person {name:"Kevin Bacon"})-[*]-(meg:Person {name:"Meg Ryan"}) ) RETURN p
![](http://note.youdao.com/yws/res/100965/WEBRESOURCE2a8ef0336cbf204b794b52c6a8986a96)
MATCH (ms:Person { name:'Andres' }),(cs:Person { name:'Taylor' }), p = shortestPath((ms)-[r:Follow]-(cs)) RETURN p;
Delete all Movie and Person nodes, and their relationships：
MATCH (n) DETACH DELETE n
删除节点和与其相连的边。
MATCH (n:Person { name:'Andres' }) DETACH DELETE n;
删除边。
MATCH (a:Person)-[r:Follow]->(b:Person) WHERE a.name = 'Andres' AND b.name = 'Taylor' DELETE r;
:server connect命令用于连接
- 查询两个节点之间的关系。
MATCH (a:Person { name:'Andres' })-[r]->(b:Person { name:'Taylor' }) RETURN type(r);
- 查询一个节点的所有Follower。
MATCH (:Person { name:'Taylor' })-[r:Follow]->(Person) RETURN Person.name;
- 查看所有的节点数和边数
MATCH (n) RETURN count(n); MATCH ()-->() RETURN count(*);
适合存储”修改较少，查询较多，没有超大节点（常见于大V）“的图数据。Arangodb也许是一个不错的考虑对象，根据其官网的说明，Arangodb不仅具有一般图形数据库的优点，而且在各种操作的速度上领先于Neo4j
用php开发
[https://neo4j.com/developer/language-guides/](https://neo4j.com/developer/language-guides/)
[https://neo4j.com/developer/php/](https://neo4j.com/developer/php/)
[https://neo4j.com/developer/example-project/](https://neo4j.com/developer/example-project/)
[https://github.com/neo4j-examples?q=movies](https://github.com/neo4j-examples?q=movies)
dbms.security.auth_enabled=false
关系也可以有属性，而且有type类型。
用的这个
[https://github.com/elesos/neo4jphp](https://github.com/elesos/neo4jphp)
[labels] => Array ( [0] => Person )
有没有知道如何多数据库切换？
Active database: graph.db
