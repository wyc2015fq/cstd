# D2R：将关系数据库映射到RDF - ChihkAnchor的博客 - CSDN博客





2019年02月26日 15:34:59[Chihk-Anchor](https://me.csdn.net/weixin_40871455)阅读数：226标签：[D2R																[rdf																[关系数据库](https://so.csdn.net/so/search/s.do?q=关系数据库&t=blog)
个人分类：[知识图谱																[问答系统																[数据库](https://blog.csdn.net/weixin_40871455/article/category/8667928)




关于rdf不懂的同学可以移步至[《语义网知识表示方法：RDF，RDFS与OWL》](https://blog.csdn.net/weixin_40871455/article/details/87912840)

我们这里主要介绍D2R，使用D2R将关系数据库中的数据映射到RDF中

D2R 主要包括 D2R Server， D2RQ Engine 以及 D2RQ Mapping 语言。
- D2R Server 是一个 HTTP Server，它的主要功能提供对 RDF 数据的查询访问接口，以供上层的 RDF 浏览器、SPARQL 查询客户端以及传统的 HTML 浏览器调用。
- D2RQ Engine 的主要功能是使用一个可定制的 D2RQ Mapping 文件将关系型数据库中的数据换成 RDF 格式。D2RQ engine 并没有将关系型数据库发布成真实的 RDF 数据，而是使用 D2RQ Mapping 文件将其映射成虚拟的 RDF 格式。该文件的作用是在访问关系型数据时将 RDF 数据的查询语言 SPARQL 转换为 RDB 数据的查询语言 SQL，并将 SQL 查询结果转换为 RDF 三元组或者 SPARQL 查询结果。D2RQ Engine 是建立在 [Jena](http://jena.sourceforge.net/ARQ/)（Jena 是一个创建 Semantic Web 应用的 Java 平台，它提供了基于 RDF，SPARQL 等的编程环境）的接口之上。
- D2RQ Mapping 语言的主要功能是定义将关系型数据转换成 RDF 格式的 Mapping 规则。

D2R 的主体构架

![](https://img-blog.csdnimg.cn/20190226101222644.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

## 如何使用 D2R

### 环境准备

在使用 D2R 之前，先要进行准备工作，即对 D2R 的环境进行下载和安装：
- [下载 D2R](http://d2rq.org/)的环境
- 下载与你目前使用的数据库版本相匹配的驱动（我这里是[清华镜像站的mysql驱动地址](https://mirrors.tuna.tsinghua.edu.cn/mysql/downloads/Connector-J/) ），将其 jar 文件放在 D2R Server 的 lib 目录下面。D2R 目前已经详细测试过 Oracle, MySQL, PostgreSQL, Microsoft SQL Server。因为D2R自带mysql驱动所以这一步可以省略

使用 D2R 可以对关系型数据库的数据进行两种方案的转化与访问。第一种方案是将关系型数据库的数据转换为虚拟的 RDF 数据进行访问，分为两个步骤，第一步是生成 Mapping 文件，第二步是使用 Mapping 文件对关系型数据进行转换与访问。我们可以通过 D2R server 访问关系型数据，或者通过在自己的 Java application 中调用 Jena/Seasame 的 API 去访问数据。第二种方案是直接将关系型数据库的数据包装成真实的 RDF 文件，以供一些可以访问 RDF Store 的接口访问。由于一般来讲，当对外提供服务，查询操作比较频繁的情况下，最好是将RDB的数据直接转为RDF，会节省很多SPARQL到SQL的转换时间；但是如果数据库的数据规模都比较大，且内容经常发生变化，转换为虚拟的 RDF 数据空间复杂度会更低，更新内容更加容易，因此第一种方案的应用更加广泛。我们在这里只介绍第一种方案，并分下述两个步骤进行介绍。

### 生成 Mappping file

D2R 提供了生成 Mapping 文件的执行脚本。我们需要在命令行中进入您所下载的 D2R 所在的路径，执行下面的命令：

```bash
generate-Mapping [-u username] [-p password] [-d driverclass] [-o outfile.n3] [-b base uri] jdbcURL
```

参数意义如下：

**jdbcURL**

JDBC 链接数据库的 URL
|123456|`MySQL: jdbc:mysql://servername/databasename ``PostgreSQL: jdbc:postgresql://servername/databasename ``Oracle: jdbc:oracle:thin:@servername:1521:databasename ``Microsoft SQL Server: jdbc:sqlserver://servername;databaseName=databasename ``（包含分号，使用时需要用引号引起来）``DB2：jdbc:db2:// servername:50000/ databasename`|
|----|----|

**-u username**

数据库用户登录名

**-p password**

数据库用户登录密码

**-d driverclass**

数据库驱动类名
|12345|`MySQL: com.mysql.jdbc.Driver ``PostgreSQL: org.postgresql.Driver ``Oracle: oracle.jdbc.OracleDriver ``Microsoft SQL Server: com.microsoft.sqlserver.jdbc.SQLServerDriver ``DB2：com.ibm.db2.jcc.DB2Driver`|
|----|----|

**-o outfile.n3**

Mapping file 的输出文件，后缀名可以是其他，如：ttl

**-b base uri**

Base uri 作为词汇的命名空间，默认为 `http://localhost:2020/``词汇的前缀按照``http://``baseURI``/vocab/resource/`*`模式生成。base uri 需要与 server 的 uri 相一致。`*

**例如** : 现在有一个名为 Academic 的mysql数据库，*执行下面命令便可以根据**Academic**的**Schema**生成一个**Mapping**文件存储在**Academic.n3**中**：*

```bash
generate-Mapping -o Academic.n3 -d com.mysql.jdbc.Driver -u test -p test jdbc:mysql://localhost/Academic
```

### **使用 Mapping 文件对数据进行转化与访问**

生成 Mapping 文件后，我们便希望通过 Mapping 文件对关系型数据库的数据进行转换和访问，这里有两种方式：

一种是通过 D2R Server 自动调用 D2RQ Engine 对数据进行转换访问，

另一种在自己的 Java application 中通过 Jena/Seasame 的 API 去使用 D2RQ Engine。

### 使用 D2R server 默认的数据处理和访问

该方法很简单，不需要自己写代码，只需要在命令行下面进入 D2R 所在文件路径，执行下面的命令启动 D2R Server：

```bash
d2r-server Academic.n3
```

其中 Academic.n3 即为上一节中生成 Mapping 文件。

![](https://img-blog.csdnimg.cn/20190226113545294.png)

然后在 Web 浏览器中访问 http://localhost:2020， 便可以默认的 HTML 浏览器、DF 浏览器以及 SPARQL 查询端对我们的数据进行访问：

![](https://img-blog.csdnimg.cn/20190226113807188.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190226115748500.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190226115828214.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

### 使用 Jena API 构建自己的应用

如果需要在 RDF 数据层上构建自己的应用，开发自己的客户端展示方法，就要用到第二种方式，即直接在自己的 Java 工程中通过 Jena/Seasame 的 API 去使用 D2RQ, 这里仅介绍 Jena 的使用方式，Seasame 的使用留给读者自己去实践。

首先需要在你的工程中引用 D2RQ 与 Jena 相关的 jar 包：
- 将你下载的 D2R server 目录下的 lib/d2rq-0.8.1.jar 加入你工程的 classpath；
- 下载 [ARQ2.8.1](http://jena.sourceforge.net/ARQ/)，将其 lib 目录下的所有 jar 包加入你工程的 classpath。

使用 Jena API 可以多种方式调用 D2RQ, 其中最简洁的方式为直接编写 SPARQL 查询语言，其余方式都需要调用 Jena 的封装类，如 Resource，Property， RDFNode 等，需要读者非常熟悉 Jena 的接口，并需要为自己的数据创建相应的 Jena 类对象，因此比较麻烦。本文在此介绍最容易上手也是最灵活的访问方式，SPARQL 查询。

假定我们现在需要构建一个应用，展示所有作者的 name, email 和 paper，因此在 server 端需要提供一个查询类，如下所示：

```java
package test; 
 
import com.hp.hpl.jena.query.Query; 
import com.hp.hpl.jena.query.QueryExecutionFactory; 
import com.hp.hpl.jena.query.QueryFactory; 
import com.hp.hpl.jena.query.QuerySolution; 
import com.hp.hpl.jena.query.ResultSet; 
 
import de.fuberlin.wiwiss.d2rq.ModelD2RQ; 
 
public class AcademicTest { 
 public void queryBySPARQL(){ 
 
     ModelD2RQ m = new ModelD2RQ("file:/D:/Academic.n3"); 
      
     String SPARQLPrefix = "PREFIX vocab: http://localhost:2020/vocab/resource/"; 
     String SPARQL =  SPARQLPrefix + 
       "SELECT  ?name ?email ?paper WHERE {" + 
       "    ?author vocab:Name ?name . " + 
       "    ?author vocab:Email ?email ." + 
           "    ?author vocab:Paper2Author ?paper . }"  ; 
     Query query = QueryFactory.create(SPARQL); 
     ResultSet rs = QueryExecutionFactory.create(query, m).execSelect(); 
     while (rs.hasNext()) { 
        QuerySolution row = rs.nextSolution(); 
        System.out.println("name:"+row.get("name")+",email:"+row.get("email")+",paper:"+row.get("paper")); 
     } 
  } 
}
```

用 SPARQL 查询数据的步骤如下：
- 创建 D2RQ model。该步骤需要注意的是，创建 model 时需要指定 Mapping 文件的路径，路径可以是绝对路径，也可以是基于工程根目录的相对路径，路径之前必须加上前缀“file:/”。
- 执行 SPARQL 语言。该 SPARQL 语句中 ?author 为主语，vocab:Name，vocab:Email 以及 vocab:Paper2Author 为谓语，查询结果 ?name，?email 以及 ?paper 为宾语。注意 SPARQL 语句需要有命名空间的前缀说明。如在程序的使用了 vocab:Name，则在前缀需要声明 vocab: http://localhost:2020/vocab/resource/，并且该声明必须与 Mapping 文件的前缀相一致。有关 SPARQL 的详细语法，在此不作详细介绍，读者可以参阅 [W3C 关于 SPARQL](http://www.w3.org/TR/rdf-sparql-query/)的详细说明。在执行 SPARQL 语句时，需要指明要执行的语句，以及所依附的 D2RQ model。
- 处理查询结果（e.g.，输出到 console）。关于 ResultSet 的处理，jena 提供了一些类似 SQL 的接口去获取每个查询结果项。

执行结果：

```
name:Jie Bao, email:baojie@cs.rpi.edu, paper:file:///D:/Academic2.n3#ZJ.PAPER/1 
name:Li Ding, email:dingl@cs.rpi.edu, paper:file:///D:/Academic2.n3#ZJ.PAPER/1 
name:Deborah McGuinness, email:dlm@cs.rpi.edu, paper:file:///D:/Academic2.n3#ZJ.PAPER/1 
name:James Hendler, email:hendler@cs.rpi.edu, paper:file:///D:/Academic2.n3#ZJ.PAPER/1 
name:James Hendler, email:hendler@cs.rpi.edu, paper:file:///D:/Academic2.n3#ZJ.PAPER/3 
name:James Hendler, email:hendler@cs.rpi.edu, paper:file:///D:/Academic2.n3#ZJ.PAPER/2 
name:Jennifer Golbeck, email:jgolbeck@umd.edu, paper:file:///D:/Academic2.n3#ZJ.PAPER/2 
name:Jennifer Golbeck, email:jgolbeck@umd.edu, paper:file:///D:/Academic2.n3#ZJ.PAPER/3
```

注意，结果中 name 与 email 的值都是一个文字型资源，而 paper 的值则是一个类资源，用一个路径表示（注意，笔者目前创建的是 Java 工程，因此可以用一个文件路径来标识一个资源，在 Web 工程中，可以曝露出 HTTP URI，例如图 4 中用 http://localhost:2020/resource/ZJ.paper/1 表示一篇 paper）。

如何读者希望查询某一具体实例资源的属性值，在写 SPARQL 语句的时候注意需要将路径用“<>”引起来，例如：
`"SELECT  ?title WHERE {  <file:///D:/Academic2.n3#ZJ.PAPER/1>  vocab:Title ?title }"`
到此为止，我们便介绍了如何使用 D2R 将关系型数据库中的数据转换为虚拟的 RDF 数据，并对其进行访问查询的最基本方法。接下来我们将介绍如何丰富自动生成的 Mapping 文件，使得更多的数据能够关联在一起。

## 定制自己的 Mapping 文件

本节首先简单介绍 Mapping 语言的一些基本语法，然后介绍如何利用 Mapping 语言对 Mapping 文件进行定制。

### Mapping 语言的基本语法

Mapping 语言中最重要的是两个概念，一个是 **d2rq:ClassMap**，另一个是 **d2rq:PropertyBridge**。 d2rq:ClassMap 代表 OWL Ontology 或者 RDFS Schema 中的一个或一组相似的 Class，它一般映射到关系数据库中的一个表。其中包括几个重要属性：
- **d2rq:Class**：表示该 ClassMap 所对应 Class，其取值可以来自现有的 OWL Ontology 或者 RDFS Schema，也可以根据自己的数据特征定义新的 Class；
- **d2rq:UriPattern**: 描述了一个 URI 模板，用来指导生成实例化资源的真实 URI。一般用“ZJ.AUTHOR/@@ZJ.AUTHOR.AUTHORID@@”来表示，“/”前面的部分为表名，后面“@@”之间的部分为表的列。

d2rq:PropertyBridge 代表 OWL Ontology 或者 RDFS Schema 中 Class 的属性，它一般映射到关系数据库中某个表的一列。其中包括几个重要属性：
- **d2rq:belongsToClassMap**: 表示该 propertyBridge 所属的 ClassMap；
- **d2rq:property**: 表示该 propertyBridge 所对应 property，其取值可以来自现有的 OWL Ontology 或者 RDFS Schema，也可以根据自己的数据特征定义新的 property；
- **d2rq:column**: 表示该 propertyBridge 关联的某个具体表中的列。
- **d2rq:refersToClassMap**: 表示该 propertyBridge 引用的其它 ClassMap，它的取值不是该 propertyBridge 所属 ClassMap 对应表中的取值，而是从引用的 ClassMap 对应表中取值。该属性一般会跟随几个 d2rq:join 来指明关联的条件，d2rq:join 类似 sql 语句中的 where 条件。

mapping文件示例：

```
@prefix map: <#> .
@prefix db: <> .
@prefix vocab: <vocab/> .
@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .
@prefix rdfs: <http://www.w3.org/2000/01/rdf-schema#> .
@prefix xsd: <http://www.w3.org/2001/XMLSchema#> .
@prefix d2rq: <http://www.wiwiss.fu-berlin.de/suhl/bizer/D2RQ/0.1#> .
@prefix jdbc: <http://d2rq.org/terms/jdbc/> .

map:database a d2rq:Database;
	d2rq:jdbcDriver "com.mysql.jdbc.Driver";
	d2rq:jdbcDSN "jdbc:mysql://localhost/kg_demo_movie";
	d2rq:username "root";
	d2rq:password "7231wyyg";
	jdbc:autoReconnect "true";
	jdbc:zeroDateTimeBehavior "convertToNull";
	.

# Table genre
map:genre a d2rq:ClassMap;
	d2rq:dataStorage map:database;
	d2rq:uriPattern "genre/@@genre.genre_id@@";
	d2rq:class vocab:genre;
	d2rq:classDefinitionLabel "genre";
	.
map:genre__label a d2rq:PropertyBridge;
	d2rq:belongsToClassMap map:genre;
	d2rq:property rdfs:label;
	d2rq:pattern "genre #@@genre.genre_id@@";
	.
map:genre_genre_id a d2rq:PropertyBridge;
	d2rq:belongsToClassMap map:genre;
	d2rq:property vocab:genre_genre_id;
	d2rq:propertyDefinitionLabel "genre genre_id";
	d2rq:column "genre.genre_id";
	d2rq:datatype xsd:integer;
	.
map:genre_genre_name a d2rq:PropertyBridge;
	d2rq:belongsToClassMap map:genre;
	d2rq:property vocab:genre_genre_name;
	d2rq:propertyDefinitionLabel "genre genre_name";
	d2rq:column "genre.genre_name";
	.

# Table movie
map:movie a d2rq:ClassMap;
	d2rq:dataStorage map:database;
	d2rq:uriPattern "movie/@@movie.movie_id@@";
	d2rq:class vocab:movie;
	d2rq:classDefinitionLabel "movie";
	.
map:movie__label a d2rq:PropertyBridge;
	d2rq:belongsToClassMap map:movie;
	d2rq:property rdfs:label;
	d2rq:pattern "movie #@@movie.movie_id@@";
	.
map:movie_movie_id a d2rq:PropertyBridge;
	d2rq:belongsToClassMap map:movie;
	d2rq:property vocab:movie_movie_id;
	d2rq:propertyDefinitionLabel "movie movie_id";
	d2rq:column "movie.movie_id";
	d2rq:datatype xsd:integer;
	.
map:movie_movie_title a d2rq:PropertyBridge;
	d2rq:belongsToClassMap map:movie;
	d2rq:property vocab:movie_movie_title;
	d2rq:propertyDefinitionLabel "movie movie_title";
	d2rq:column "movie.movie_title";
	.
map:movie_movie_introduction a d2rq:PropertyBridge;
	d2rq:belongsToClassMap map:movie;
	d2rq:property vocab:movie_movie_introduction;
	d2rq:propertyDefinitionLabel "movie movie_introduction";
	d2rq:column "movie.movie_introduction";
	.
map:movie_movie_rating a d2rq:PropertyBridge;
	d2rq:belongsToClassMap map:movie;
	d2rq:property vocab:movie_movie_rating;
	d2rq:propertyDefinitionLabel "movie movie_rating";
	d2rq:column "movie.movie_rating";
	d2rq:datatype xsd:double;
	.
map:movie_movie_release_date a d2rq:PropertyBridge;
	d2rq:belongsToClassMap map:movie;
	d2rq:property vocab:movie_movie_release_date;
	d2rq:propertyDefinitionLabel "movie movie_release_date";
	d2rq:column "movie.movie_release_date";
	.

# Table movie_to_genre (n:m)
map:movie_to_genre__link a d2rq:PropertyBridge;
	d2rq:belongsToClassMap map:movie;
	d2rq:property vocab:movie_to_genre;
	d2rq:refersToClassMap map:genre;
	d2rq:join "movie_to_genre.movie_id => movie.movie_id";
	d2rq:join "movie_to_genre.genre_id => genre.genre_id";
	.
```

### 定制 Mapping 文件

Linked data 最重要的特点便是资源和资源之间是互联的，从一个资源可以跳转到其它许多资源。如，从类资源movie 可以跳转到类资源 person，同样从类资源 person 也可以跳转到类资源 movie。这些关联生成的前提是原始的数据中存在 person_to_movie表，通过外键引用 person 表与 movie 表。

有了主外键关系，D2R 便可以自动为其创建 RDF 关联。具体做法为，如果一个表 A 被表 B 外键引用，则为 A 对应的 ClassMap 创建一个 PropertyBridge，其 d2rq:belongsToClassMap 为 A 所对应的 ClassMap，d2rq:refersToClassMap 为 B 所对应的 ClassMap。 这样，在浏览 A 的实例资源时，便可以通过该 PropertyBridge 跳转到 B 的实例资源。如果一个表的所有列都通过外键引用其它数据表，则不会为该表生成对应的 ClassMap，而是选取一个被引用的表，为其创建属性，属性取值来自其它被引用的表。例如本文所举例子中的 person_to_movie：

清单 3. 默认的 Mapping 文件生成的 person_to_movie 属性

```
# Table person_to_movie (n:m)
map:person_to_movie__link a d2rq:PropertyBridge;
	d2rq:belongsToClassMap map:movie;
	d2rq:property vocab:person_to_movie;
	d2rq:refersToClassMap map:person;
	d2rq:join "person_to_movie.movie_id => movie.movie_id";
	d2rq:join "person_to_movie.person_id => person.person_id";
```

实际情况中，很多数据之间虽然有联系，但数据库的创建者们并没有为之建立主外键的关系。在这种情况下，我们就需要根据一定的领域常识或经验，手动地修改 Mapping 文件，为之建立一些必要的关联。

在命令行中使用下面的命令将我们的数据转为RDF：
`dump-rdf.bat -o Academic.nt Academic.n3`
Academic.n3文件是我们上文通过 general-mapping命令 生成的文件。其支持导出的RDF格式有“TURTLE”, “RDF/XML”, “RDF/XML-ABBREV”, “N3”, 和“N-TRIPLE”。“N-TRIPLE”是默认的输出格式。

![](https://img-blog.csdnimg.cn/20190226152908419.png)

Academic.nt文件内容示例： 

```
<file:///F:/Java/d2rq-0.8.1/Academic.nt#movie/13> <file:///F:/Java/d2rq-0.8.1/vocab/person_to_movie> <file:///F:/Java/d2rq-0.8.1/Academic.nt#person/163441> .
<file:///F:/Java/d2rq-0.8.1/Academic.nt#movie/24> <file:///F:/Java/d2rq-0.8.1/vocab/person_to_movie> <file:///F:/Java/d2rq-0.8.1/Academic.nt#person/240171> .
<file:///F:/Java/d2rq-0.8.1/Academic.nt#movie/79> <file:///F:/Java/d2rq-0.8.1/vocab/person_to_movie> <file:///F:/Java/d2rq-0.8.1/Academic.nt#person/1336> .
<file:///F:/Java/d2rq-0.8.1/Academic.nt#movie/79> <file:///F:/Java/d2rq-0.8.1/vocab/person_to_movie> <file:///F:/Java/d2rq-0.8.1/Academic.nt#person/1337> .
<file:///F:/Java/d2rq-0.8.1/Academic.nt#movie/79> <file:///F:/Java/d2rq-0.8.1/vocab/person_to_movie> <file:///F:/Java/d2rq-0.8.1/Academic.nt#person/1338> .
<file:///F:/Java/d2rq-0.8.1/Academic.nt#movie/79> <file:///F:/Java/d2rq-0.8.1/vocab/person_to_movie> <file:///F:/Java/d2rq-0.8.1/Academic.nt#person/1339> .
<file:///F:/Java/d2rq-0.8.1/Academic.nt#movie/79> <file:///F:/Java/d2rq-0.8.1/vocab/person_to_movie> <file:///F:/Java/d2rq-0.8.1/Academic.nt#person/1340> .
<file:///F:/Java/d2rq-0.8.1/Academic.nt#movie/79> <file:///F:/Java/d2rq-0.8.1/vocab/person_to_movie> <file:///F:/Java/d2rq-0.8.1/Academic.nt#person/1341> .
<file:///F:/Java/d2rq-0.8.1/Academic.nt#movie/82> <file:///F:/Java/d2rq-0.8.1/vocab/person_to_movie> <file:///F:/Java/d2rq-0.8.1/Academic.nt#person/643> .
<file:///F:/Java/d2rq-0.8.1/Academic.nt#movie/87> <file:///F:/Java/d2rq-0.8.1/vocab/person_to_movie> <file:///F:/Java/d2rq-0.8.1/Academic.nt#person/695> .
<file:///F:/Java/d2rq-0.8.1/Academic.nt#movie/146> <file:///F:/Java/d2rq-0.8.1/vocab/person_to_movie> <file:///F:/Java/d2rq-0.8.1/Academic.nt#person/1339> .
<file:///F:/Java/d2rq-0.8.1/Academic.nt#movie/146> <file:///F:/Java/d2rq-0.8.1/vocab/person_to_movie> <file:///F:/Java/d2rq-0.8.1/Academic.nt#person/1619> .
<file:///F:/Java/d2rq-0.8.1/Academic.nt#movie/146> <file:///F:/Java/d2rq-0.8.1/vocab/person_to_movie> <file:///F:/Java/d2rq-0.8.1/Academic.nt#person/1624> .
<file:///F:/Java/d2rq-0.8.1/Academic.nt#movie/285> <file:///F:/Java/d2rq-0.8.1/vocab/person_to_movie> <file:///F:/Java/d2rq-0.8.1/Academic.nt#person/1619> .
<file:///F:/Java/d2rq-0.8.1/Academic.nt#movie/393> <file:///F:/Java/d2rq-0.8.1/vocab/person_to_movie> <file:///F:/Java/d2rq-0.8.1/Academic.nt#person/240171> .
<file:///F:/Java/d2rq-0.8.1/Academic.nt#movie/604> <file:///F:/Java/d2rq-0.8.1/vocab/person_to_movie> <file:///F:/Java/d2rq-0.8.1/Academic.nt#person/52908> .
<file:///F:/Java/d2rq-0.8.1/Academic.nt#movie/605> <file:///F:/Java/d2rq-0.8.1/vocab/person_to_movie> <file:///F:/Java/d2rq-0.8.1/Academic.nt#person/52908> .
<file:///F:/Java/d2rq-0.8.1/Academic.nt#movie/672> <file:///F:/Java/d2rq-0.8.1/vocab/person_to_movie> <file:///F:/Java/d2rq-0.8.1/Academic.nt#person/20999> .
<file:///F:/Java/d2rq-0.8.1/Academic.nt#movie/673> <file:///F:/Java/d2rq-0.8.1/vocab/person_to_movie> <file:///F:/Java/d2rq-0.8.1/Academic.nt#person/20999> .
<file:///F:/Java/d2rq-0.8.1/Academic.nt#movie/674> <file:///F:/Java/d2rq-0.8.1/vocab/person_to_movie> <file:///F:/Java/d2rq-0.8.1/Academic.nt#person/20999> .
<file:///F:/Java/d2rq-0.8.1/Academic.nt#movie/675> <file:///F:/Java/d2rq-0.8.1/vocab/person_to_movie> <file:///F:/Java/d2rq-0.8.1/Academic.nt#person/20999> .
<file:///F:/Java/d2rq-0.8.1/Academic.nt#movie/746> <file:///F:/Java/d2rq-0.8.1/vocab/person_to_movie> <file:///F:/Java/d2rq-0.8.1/Academic.nt#person/20640> .
<file:///F:/Java/d2rq-0.8.1/Academic.nt#movie/746> <file:///F:/Java/d2rq-0.8.1/vocab/person_to_movie> <file:///F:/Java/d2rq-0.8.1/Academic.nt#person/69033> .
<file:///F:/Java/d2rq-0.8.1/Academic.nt#movie/767> <file:///F:/Java/d2rq-0.8.1/vocab/person_to_movie> <file:///F:/Java/d2rq-0.8.1/Academic.nt#person/20999> .
<file:///F:/Java/d2rq-0.8.1/Academic.nt#movie/843> <file:///F:/Java/d2rq-0.8.1/vocab/person_to_movie> <file:///F:/Java/d2rq-0.8.1/Academic.nt#person/1337> .
<file:///F:/Java/d2rq-0.8.1/Academic.nt#movie/843> <file:///F:/Java/d2rq-0.8.1/vocab/person_to_movie> <file:///F:/Java/d2rq-0.8.1/Academic.nt#person/1338> .
<file:///F:/Java/d2rq-0.8.1/Academic.nt#movie/843> <file:///F:/Java/d2rq-0.8.1/vocab/person_to_movie> <file:///F:/Java/d2rq-0.8.1/Academic.nt#person/12466> .
<file:///F:/Java/d2rq-0.8.1/Academic.nt#movie/843> <file:///F:/Java/d2rq-0.8.1/vocab/person_to_movie> <file:///F:/Java/d2rq-0.8.1/Academic.nt#person/1002925> .
```

有了这些rdf文件或者说有了能从关系型数据库中查询rdf的工具之后，我们接下来就可以使用sparql在数据库中查询我们想要的内容，方便之后我们进一步将其封装成问答系统 。

参考：[https://zhuanlan.zhihu.com/p/32552993](https://zhuanlan.zhihu.com/p/32552993)

[https://www.ibm.com/developerworks/cn/web/1003_zhangjing_d2r/](https://www.ibm.com/developerworks/cn/web/1003_zhangjing_d2r/)](https://blog.csdn.net/weixin_40871455/article/category/8355978)](https://blog.csdn.net/weixin_40871455/article/category/8355979)](https://so.csdn.net/so/search/s.do?q=rdf&t=blog)](https://so.csdn.net/so/search/s.do?q=D2R&t=blog)




