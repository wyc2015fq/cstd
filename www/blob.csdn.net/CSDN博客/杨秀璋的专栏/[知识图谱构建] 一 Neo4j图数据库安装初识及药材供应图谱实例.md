# [知识图谱构建] 一.Neo4j图数据库安装初识及药材供应图谱实例 - 杨秀璋的专栏 - CSDN博客





2018年10月22日 22:03:03[Eastmount](https://me.csdn.net/Eastmount)阅读数：1123
所属专栏：[知识图谱、web数据挖掘及NLP](https://blog.csdn.net/column/details/eastmount-kgdmnlp.html)









2012年5月，谷歌公司的知识图谱（Knowledge Graph）产品被正式提出，其旨在将人、地点、物等信息作为实体，将实体间的联系作为关系，并将实体和关系以图的形式进行存储。作为语义网的最新产物，知识图谱这种新型的搜索引擎改变了传统的文本搜索结果，而随后国内公司也开发出了相应产品，例如百度公司的“知心”和搜狗公司的“知立方”等。知识图谱本质上属于一种语义网。知识图谱中的每一条数据或事实一般会采用   <实体，属性，属性值> <实体, 关系, 实体> 的三元组形式来描述。基于知识图谱，开发人员可以开发语义相关应用，例如语义搜索、问答系统、实体推荐等应用。
![](https://img-blog.csdn.net/20181022101121609?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这系列文章将详细讲解知识图谱的一些应用及实例，前部分主要介绍Neo4j图数据库的基本用法，结合语句讲解一个药材供应链的案例，导入CSV数据实现图谱。

PSS：2019年1~2月作者参加了CSDN2018年博客评选，希望您能投出宝贵的一票。我是59号，Eastmount，杨秀璋。投票地址：[https://bss.csdn.net/m/topic/blog_star2018/index](https://bss.csdn.net/m/topic/blog_star2018/index)
![](https://img-blog.csdnimg.cn/20190104155648543.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
五年来写了314篇博客，12个专栏，是真的热爱分享，热爱CSDN这个平台，也想帮助更多的人，专栏包括Python、数据挖掘、网络爬虫、图像处理、C#、Android等。现在也当了两年老师，更是觉得有义务教好每一个学生，让贵州学子好好写点代码，学点技术，"师者，传到授业解惑也"，提前祝大家新年快乐。2019我们携手共进，为爱而生。

## 一.Neo4j安装

从官方网站下载Neo4j对应的版本，包括网页版、桌面版。作者下载的是 neo4j-desktop-offline-1.1.10-setup.exe。

官方地址： [https://neo4j.com/download/](https://neo4j.com/download/)

CSDN地址：[https://download.csdn.net/download/eastmount/10737631](https://download.csdn.net/download/eastmount/10737631)![](https://img-blog.csdn.net/20181022105212295?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
1.双击Neo4j桌面版软件，点击安装。
![](https://img-blog.csdn.net/2018102210594754?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2.选择安装位置，点击下一步成功安装。
![](https://img-blog.csdn.net/20181022110227726?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
3.运行软件并初始化，如下图所示。
![](https://img-blog.csdn.net/20181022110450941?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
4.成功运行后的软件如下图所示。
![](https://img-blog.csdn.net/20181022110553312?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 二.药材供应链图谱实现

**（一）导入数据创建实体**

1.点击“Project”=>“Add Graph”增加新的图形。
![](https://img-blog.csdn.net/20181022132916350?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2.在弹出的新界面中点击“Create a Local Graph”按钮，创建一个本地的图形。
![](https://img-blog.csdn.net/20181022133027364?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
3.输入创建图形的名字及密码，如“Graph1022”、“123456”，点击创建。
![](https://img-blog.csdn.net/20181022133136608?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
4.在“Graph1022”中点击“Start”按钮开启图数据库服务器。
![](https://img-blog.csdn.net/20181022133257861?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
5.点击“Manage”管理页面，接着点击“Open Browser”打开浏览器。
![](https://img-blog.csdn.net/20181022133747681?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://img-blog.csdn.net/20181022134023690?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
弹出如下所示界面，该界面中可以输入CQL语句编辑图数据库。
![](https://img-blog.csdn.net/20181022134241635?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**（二） 导入数据创建关系**

1.假设该药材的供应链如下图所示，现在需要建立对应的简单图谱。
![](https://img-blog.csdn.net/2018102213473410?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2.需要导入的实体数据主要有两类，企业类、药材类。如下所示：
**(1) 企业 entity-01.csv**

```
id	name	label
1	威门药业	企业
2	东莞焱兴实业	企业
3	银川塞外杞香	企业
4	安徽亳州药业	企业
5	抚松泓宇特产	企业
6	格尔木源鑫堂	企业
7	亳州一阳药业	企业
8	成都德音电商	企业
9	康美中药材	企业
10	亳州昌盛鑫参茸	企业
11	郑州伟丰生物	企业
12	山东大凯生物	企业
```

**(2) 药材 entity-02.csv**

```python
id	name	label
13	北沙参	药材
14	枸杞子	药材
15	麦冬	药材
16	川楝子	药材
17	当归	药材
18	地黄	药材
19	桂枝	药材
20	人参	药材
21	党参	药材
22	白芷	药材
23	羌活	药材
24	细辛	药材
25	防风	药材
26	荆芥	药材
27	薄荷	药材
28	甘草	药材
29	乳糖	药材
30	糊精	药材
```

3.接着将需要导入的csv文件放置在该数据库的import文件夹下，然后执行如下语句。

```python
LOAD CSV WITH HEADERS  FROM "file:///entity-01.csv" AS line
MERGE (p:Enterprise{id:line.id,name:line.name})
```

代码功能：载入CSV文件从import文件夹中，AS line表示按行读取文件内容，包括两个值：id 和 name，抽象的实体概念名字为Enterprise（企业）。
![](https://img-blog.csdn.net/20181022161337940?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
文件夹位置为：C:/Users/yxz/.Neo4jDesktop/neo4jDatabases/database-4e2113ef-fa69-444a-9470-14af5e553e63/installation-3.4.1/import/entity-01.csv
![](https://img-blog.csdn.net/2018102214563026?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
4.插入实体成功之后，执行下面语句显示所插入的实体。

```python
match(n) return n
```

点击运行之后可以看到插入的12个企业实体。
![](https://img-blog.csdn.net/20181022210329553?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
同时可以编辑节点的颜色、大小，显示指定的双属性，如下所示。
![](https://img-blog.csdn.net/20181022210534325?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
5.执行如下语句插入30个药材实体。

```python
LOAD CSV WITH HEADERS  FROM "file:///entity-02.csv" AS line
MERGE (p:Medicinal{id:line.id,name:line.name})
```

代码功能：载入CSV文件从import文件夹中，AS line表示按行读取文件内容，包括两个值：id 和 name，抽象的实体概念名字为Medicinal（药材）。

显示结果如下所示，不同种类的实体显示不同的颜色。
![](https://img-blog.csdn.net/20181022210859323?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
6.接着插入关系，包括三种：
**(1) rela-11.csv：企业与企业的关系，企业买卖交易。**

```python
from_id	pro1	pro2	pro3	pro4	pro5	pro6	pro7	to_id
1	交易	北沙参	威门药业股份有限公司	东莞市焱兴实业发展有限公司	936	10	9360	2
1	交易	枸杞子	威门药业股份有限公司	银川塞外杞香科贸有限公司	159	6	954	3
1	交易	麦冬	威门药业股份有限公司	安徽亳州药业集团	938	9	8442	4
1	交易	川楝子	威门药业股份有限公司	安徽亳州药业集团	767	4	3068	4
1	交易	当归	威门药业股份有限公司	安徽亳州药业集团	585	2	1170	4
1	交易	地黄	威门药业股份有限公司	安徽亳州药业集团	575	2	1150	4
1	交易	桂枝	威门药业股份有限公司	安徽亳州药业集团	787	7	5509	4
1	交易	人参	威门药业股份有限公司	抚松县泓宇特产有限公司	488	7	3416	5
1	交易	党参	威门药业股份有限公司	格尔木源鑫堂生物科技有限公司	861	1	861	6
1	交易	白芷	威门药业股份有限公司	亳州市一阳药业销售有限公司	282	3	846	7
1	交易	羌活	威门药业股份有限公司	亳州市一阳药业销售有限公司	853	1	853	7
1	交易	细辛	威门药业股份有限公司	亳州市一阳药业销售有限公司	560	3	1680	7
1	交易	防风	威门药业股份有限公司	成都德音电子商务有限公司	626	3	1878	8
1	交易	荆芥	威门药业股份有限公司	成都德音电子商务有限公司	62	3	186	8
1	交易	薄荷	威门药业股份有限公司	枣庄市台儿庄区康美中药材种植专业合作社	445	1	445	9
1	交易	甘草	威门药业股份有限公司	亳州市昌盛鑫参茸有限责任公司	426	5	2130	10
1	交易	乳糖	威门药业股份有限公司	郑州市伟丰生物科技有限公司	484	1	484	11
1	交易	糊精	威门药业股份有限公司	山东大凯生物科技有限公司	249	2	498	12
```

CQL代码如下，需要注意 from_id（关系起始） 和 to_id（关系介绍），包含的属性需要和CSV表格一致。

```python
LOAD CSV WITH HEADERS FROM "file:///rela-11.csv" AS line
match (from:Enterprise{id:line.from_id}),(to:Enterprise{id:line.to_id})
merge (from)-[r:rel{pro1:line.pro1,pro2:line.pro2,pro3:line.pro3,pro4:line.pro4,pro5:line.pro5,pro6:line.pro6,pro7:line.pro7}]->(to)
```

执行“match(n) return n”代码查看结果如下图所示，可以看到企业之间交易关系。
![](https://img-blog.csdn.net/2018102221224074?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**(2) rela-12.csv：企业与药品的关系，威门药业购入药品。**

```python
from_id	pro1	pro2	pro3	pro4	to_id
1	买入	药品	威门药业股份有限公司	东莞市焱兴实业发展有限公司	13
1	买入	药品	威门药业股份有限公司	银川塞外杞香科贸有限公司	14
1	买入	药品	威门药业股份有限公司	安徽亳州药业集团	15
1	买入	药品	威门药业股份有限公司	安徽亳州药业集团	16
1	买入	药品	威门药业股份有限公司	安徽亳州药业集团	17
1	买入	药品	威门药业股份有限公司	安徽亳州药业集团	18
1	买入	药品	威门药业股份有限公司	安徽亳州药业集团	19
1	买入	药品	威门药业股份有限公司	抚松县泓宇特产有限公司	20
1	买入	药品	威门药业股份有限公司	格尔木源鑫堂生物科技有限公司	21
1	买入	药品	威门药业股份有限公司	亳州市一阳药业销售有限公司	22
1	买入	药品	威门药业股份有限公司	亳州市一阳药业销售有限公司	23
1	买入	药品	威门药业股份有限公司	亳州市一阳药业销售有限公司	24
1	买入	药品	威门药业股份有限公司	成都德音电子商务有限公司	25
1	买入	药品	威门药业股份有限公司	成都德音电子商务有限公司	26
1	买入	药品	威门药业股份有限公司	枣庄市台儿庄区康美中药材种植专业合作社	27
1	买入	药品	威门药业股份有限公司	亳州市昌盛鑫参茸有限责任公司	28
1	买入	药品	威门药业股份有限公司	郑州市伟丰生物科技有限公司	29
1	买入	药品	威门药业股份有限公司	山东大凯生物科技有限公司	30
```

CQL代码如下：

```python
LOAD CSV WITH HEADERS FROM "file:///rela-12.csv" AS line
match (from:Enterprise{id:line.from_id}),(to:Medicinal{id:line.to_id})
merge (from)-[r:rel{pro1:line.pro1,pro2:line.pro2,pro3:line.pro3,pro4:line.pro4}]->(to)
```

执行“match(n) return n”代码查看结果如下图所示，可以看到企业药材之间关系。
![](https://img-blog.csdn.net/20181022213304156?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**(3) rela-21.csv：企业与药品的关系，from_id企业卖出to_id药品。**

```python
from_id	pro1	pro2	to_id
2	药品	卖出	13
3	药品	卖出	14
4	药品	卖出	15
4	药品	卖出	16
4	药品	卖出	17
4	药品	卖出	18
4	药品	卖出	19
5	药品	卖出	20
6	药品	卖出	21
7	药品	卖出	22
7	药品	卖出	23
7	药品	卖出	24
8	药品	卖出	25
8	药品	卖出	26
9	药品	卖出	27
10	药品	卖出	28
11	药品	卖出	29
12	药品	卖出	30
```

CQL代码如下：

```python
LOAD CSV WITH HEADERS FROM "file:///rela-21.csv" AS line
match (from:Enterprise{id:line.from_id}),(to:Medicinal{id:line.to_id})
merge (from)-[r:卖出{pro1:line.pro1,pro2:line.pro2}]->(to)
```

运行结果如下图所示：
![](https://img-blog.csdn.net/20181022215443329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
7.可以编辑节点、关系显示的内容及颜色大小属性。
![](https://img-blog.csdn.net/20181022214037671?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
8.获取实体关系的顶层概念关系。

```python
call db.schema
```

显示结果如下图所示：
![](https://img-blog.csdn.net/20181022214229534?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
注意1：如果在导入数据过程中出现错误，需要删除关系、节点，则使用下面代码。

```python
删除节点及关系
MATCH (n)-[r]-()
DELETE n,r
match(n) return n

删除关系
MATCH (n)-[r]-()
DELETE r

单纯删除所以节点：
match (n)
delete n
```

注意2：为了防止出现中文乱码，建议采用记事本打开CSV文件，将文件编码方式从ASC-II修改为UTF-8。
![](https://img-blog.csdn.net/20181022145848456?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**完整CQL代码如下:**

```python
载入实体
LOAD CSV WITH HEADERS  FROM "file:///entity-01.csv" AS line
MERGE (p:Enterprise{id:line.id,name:line.name})
LOAD CSV WITH HEADERS  FROM "file:///entity-02.csv" AS line
MERGE (p:Medicinal{id:line.id,name:line.name})

显示节点
match(n) return n
match(n) return n limit 9

删除节点及关系
MATCH (n)-[r]-()
DELETE n,r
match(n) return n

删除关系
MATCH (n)-[r]-()
DELETE r

单纯删除所以节点：
match (n)
delete n

查询指定节点
match (tom {id: "1"}) return tom

载入关系1
LOAD CSV WITH HEADERS FROM "file:///rela-11.csv" AS line
match (from:Enterprise{id:line.from_id}),(to:Enterprise{id:line.to_id})
merge (from)-[r:rel{pro1:line.pro1,pro2:line.pro2,pro3:line.pro3,pro4:line.pro4,pro5:line.pro5,pro6:line.pro6,pro7:line.pro7}]->(to)

match(n) return n

载入关系2
LOAD CSV WITH HEADERS FROM "file:///rela-12.csv" AS line
match (from:Enterprise{id:line.from_id}),(to:Medicinal{id:line.to_id})
merge (from)-[r:rel{pro1:line.pro1,pro2:line.pro2,pro3:line.pro3,pro4:line.pro4}]->(to)

match(n) return n

载入关系3
LOAD CSV WITH HEADERS FROM "file:///rela-21.csv" AS line
match (from:Enterprise{id:line.from_id}),(to:Medicinal{id:line.to_id})
merge (from)-[r:卖出{pro1:line.pro1,pro2:line.pro2}]->(to)

MATCH p=()-[r:`卖出`]->() RETURN p LIMIT 25
match(n) return n

显示顶层概念
call db.schema
```

最后希望文章对您有所帮助，如果有错误的地方还请海涵。推荐我的知识图谱基础文章。
[中文知识图谱研讨会的学习总结 (上) 图谱引入、百度知心、搜狗知立方](https://blog.csdn.net/Eastmount/article/details/49785639)
[搜索引擎和知识图谱那些事 (上).基础篇](https://blog.csdn.net/Eastmount/article/details/46874155)
[知识图谱相关会议之观后感分享与学习总结](https://blog.csdn.net/Eastmount/article/details/46672701)
[知识图谱基础（二）-知识图谱的知识表达系统](https://www.jianshu.com/p/941dc6d7e760)

(By: Eastmount 2018-10-22 晚10点 [https://blog.csdn.net/Eastmount/](https://blog.csdn.net/Eastmount/) )




