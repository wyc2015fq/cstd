# [知识图谱构建] 二.《Neo4j基础入门》基础学习之创建图数据库节点及关系 - 杨秀璋的专栏 - CSDN博客





2018年12月19日 18:01:17[Eastmount](https://me.csdn.net/Eastmount)阅读数：650
所属专栏：[知识图谱、web数据挖掘及NLP](https://blog.csdn.net/column/details/eastmount-kgdmnlp.html)









该系列文章主要讲解知识图谱及Neo4j图数据库的用法，本篇文章是作者学习《Neo4j基础入门》书籍的在线笔记，主要讲解Neo4j的基础知识及基本语法，希望大家喜欢。

前文：
[[知识图谱构建] 一.Neo4j图数据库安装初识及药材供应图谱实例](https://blog.csdn.net/Eastmount/article/details/83268410)
[neo4j 实战、实例、示例 创建电影关系图 -1](https://blog.csdn.net/sinstar1/article/details/82151392)
[https://www.w3cschool.cn/neo4j/](https://www.w3cschool.cn/neo4j/)
PS：2019年1~2月作者参加了CSDN2018年博客评选，希望您能投出宝贵的一票。我是59号，Eastmount，杨秀璋。投票地址：[https://bss.csdn.net/m/topic/blog_star2018/index](https://bss.csdn.net/m/topic/blog_star2018/index)
![](https://img-blog.csdnimg.cn/20190104155648543.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
五年来写了314篇博客，12个专栏，是真的热爱分享，热爱CSDN这个平台，也想帮助更多的人，专栏包括Python、数据挖掘、网络爬虫、图像处理、C#、Android等。现在也当了两年老师，更是觉得有义务教好每一个学生，让贵州学子好好写点代码，学点技术，"师者，传到授业解惑也"，提前祝大家新年快乐。2019我们携手共进，为爱而生。

# 一.基础知识

**1.节点（Node）**

节点是图数据库的基本元素，表示一个实体记录，相当于关系数据库中的一条记录。在Neo4j中节点可以包括多个属性（Property）和多个标签（Label）。下图中节点包括两个属性，id和name。

```
match(n{name:"格尔木源鑫堂"}) return n
```
![](https://img-blog.csdnimg.cn/20181219125809549.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**2.关系（Relationship）**

关系是图数据库中的基本元素，当数据库中存在节点后，需要将节点连接起来构成图。关系用来连接两个节点，也称为图论的边（Edge），起始端和结束端都必须是节点，关系不能从空发起，关系可以包含多个属性。

```
match(n) return n
```
![](https://img-blog.csdnimg.cn/20181219131939765.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**3.属性（Property）**

节点和关系都可以有多个属性。属性是由键值对组成的 <属性:属性值>。属性值可以是基本的数据类型或基本数据类型组成的数组。比如：boolean、byte、short、int、char、double、float、long、string。

**4.路径（Path）**

当创建图后，图中任意两个节点之间都是可能存在路径的，它是由节点和关系组成的，路径是有长度的。单独一个节点可以组成路径长度为0，两个节点之间的路径长度为1，依次类推。

```
match(n:Enterprise{name:"威门药业"})-[:买入]->(herb) return n,herb
```

输出“威门药业”买入的药品路径，如下图所示：
![](https://img-blog.csdnimg.cn/20181219133809399.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**5.遍历（Traversal）**

节点可以被关系串联或并联起来，由于关系可以有方向的，所以可在图中进行遍历操作。图遍历时可以指定有方向或无方向，因此在创建关系时不必为两个节点创建互相指向的关系，而是在遍历时不指定遍历方向即可。

遍历一张图就是按照一定的规则，根据它们之间的关系，依次访问所有相关联的节点的操作。Neo4j提供了遍历操作，包括广度优先和深度优先搜索，采用Cypher实现。

# 二.创建图数据库节点

## (一) 基础

点击“Add Graph”创建项目 “Graph1219”，如下图所示。
![](https://img-blog.csdnimg.cn/20181219152334121.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
点击“Start”按钮开始绘制图。
![](https://img-blog.csdnimg.cn/20181219152608659.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**1.单标签节点创建语法**

使用create关键字创建单个节点使用下面语句，其中表示节点名称，表示标签名称。

```
CREATE (<node-name>:<label-name>)
```

下面代码是创建一个节点为Book。

```
create(m:Book)
Added 1 label, created 1 node, completed after 140 ms.
match(n) return n
```

输出结果如下所示：
![](https://img-blog.csdnimg.cn/20181219160118724.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**2.多标签节点创建语法**

下面是使用create关键字创建一个节点多个标签的情况。

```
CREATE (<node-name>:<label-name1>:<label-name2>.....:<label-namen>)
```

实例如下：

```
create(m:Cinema:Film:Movie:Picture)
Added 4 labels, created 1 node, completed after 34 ms.
match(n) return n
```

输出结果如所示：
![](https://img-blog.csdnimg.cn/20181219160406664.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
## (二) 实例

接下来讲解具体的实例，老师、学生、课程之间的关系。

**1.使用Cypher语句创建学生节点**

使用Cypher中create指令创建六个Student节点，这些节点包括姓名、性别、学院和出生日期四个属性。

```
create(stu1:Student{ name:'王建平', Sex:'男', Sdept:'自动化', Sbirth:'1996-01-12' })
create(stu2:Student{ name:'刘华', Sex:'女', Sdept:'自动化', Sbirth:'1995-07-01' })
create(stu3:Student{ name:'范林军', Sex:'女', Sdept:'计算机', Sbirth:'1994-06-30' })
create(stu4:Student{ name:'李伟', Sex:'男', Sdept:'数学', Sbirth:'1995-05-01' })
create(stu5:Student{ name:'黄艳', Sex:'女', Sdept:'数学', Sbirth:'1996-04-01' })
create(stu6:Student{ name:'何纯', Sex:'男', Sdept:'数学', Sbirth:'1995-06-30' })
```

执行 “match(n) return n” 显示结果如下图所示：
![](https://img-blog.csdnimg.cn/20181219162241142.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**2.使用Cypher语句创建课程节点**

使用Cypher中create指令创建五个Course节点，表示课程，这些节点包括姓名、课时和编号三个属性。

```
create(Course1:Course{ name:'英语', Ctime:'64', Cno:'C01'})
create(Course2:Course{ name:'数据结构', Ctime:'48', Cno:'C02'})
create(Course3:Course{ name:'Python', Ctime:'64', Cno:'C03'})
create(Course4:Course{ name:'计算机网络', Ctime:'32', Cno:'C04'})
create(Course5:Course{ name:'数据库', Ctime:'48', Cno:'C05'})
```

设置不同类型节点的颜色，然后执行 “match(n) return n” 显示结果如下图所示：
![](https://img-blog.csdnimg.cn/20181219163716116.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**3.使用Cypher语句创建教师节点**

使用Cypher中create指令创建四个Teacher节点，表示教师，这些节点包括姓名、性别和学院三个属性。

```
create(Tea1:Teacher{ name:'杨秀璋', Sex:'男', Dept:'计算机'})
create(Tea2:Teacher{ name:'小民', Sex:'男', Dept:'计算机'})
create(Tea3:Teacher{ name:'小娜', Sex:'女', Dept:'英语'})
create(Tea4:Teacher{ name:'小王', Sex:'男', Dept:'计算机'})
```

设置不同类型节点的颜色，然后执行 “match(n) return n” 显示结果如下图所示：
![](https://img-blog.csdnimg.cn/2018121916435211.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**4.使用Cypher语句创建学院节点**

使用Cypher中create指令创建四个Department节点，表示学院节点，这些节点包括姓名和编号两个属性。

```
create(Dept1:Department{ name:'自动化学院', No:'001'})
create(Dept2:Department{ name:'计算机学院', No:'002'})
create(Dept3:Department{ name:'英语学院', No:'003'})
create(Dept4:Department{ name:'数学学院', No:'004'})
```

显示结果如下图所示：
![](https://img-blog.csdnimg.cn/20181219164845861.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
# 三.创建图数据库关系

接着创建学生、课程、教师和学院之间的关系，但遇到了一个问题。

**问题：**

在创建学生-课程之间选课关系过程中，遇到了一个问题。

```
create 
(stu1)-[:选课]->(Course1), (stu1)-[:选课]->(Course2), (stu1)-[:选课]->(Course3),
(stu2)-[:选课]->(Course1), (stu2)-[:选课]->(Course2), (stu2)-[:选课]->(Course3),
(stu3)-[:选课]->(Course1), (stu3)-[:选课]->(Course2), (stu3)-[:选课]->(Course3), 
(stu3)-[:选课]->(Course4), (stu3)-[:选课]->(Course5),
(stu4)-[:选课]->(Course1), (stu4)-[:选课]->(Course3), (stu4)-[:选课]->(Course5),
(stu5)-[:选课]->(Course1), (stu5)-[:选课]->(Course3), (stu5)-[:选课]->(Course5),
(stu6)-[:选课]->(Course1), (stu6)-[:选课]->(Course3), (stu6)-[:选课]->(Course5)
```

运行结果如下图所示，它没有把节点关联起来，而是重新创建了一组节点及关系。而书中直接能创建对应的关系，非常疑惑。
![](https://img-blog.csdnimg.cn/2018121917075056.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
而如果是使用下面这个语句，用match先找到两个节点，然后在给两个节点添加关系，但它并不是在stu1和Course1之间创建了一条关系，而是所有学生和课程之间均创建了关系。

```
MATCH (stu1:Student),(Course1:Course)
create (stu1)-[:选课]->(Course1)
```

显示如下所示：
![](https://img-blog.csdnimg.cn/20181219174238251.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
那怎么解决呢？

原来，在实际操作，在创建节点的同时创建关系，即创建节点和关系语句必须一次执行完，否则如上面所述，创建关系时括号（）中的节点被认为是新的节点。

比如执行下面这段书中电影代码，Keanu等作为节点的标识符可以被下面识别，分开执行后面语句是认为新的节点。

```
CREATE (TheMatrix:Movie {title:'The Matrix', released:1999, tagline:'Welcome to the Real World'})
CREATE (Keanu:Person {name:'Keanu Reeves', born:1964})
CREATE (Carrie:Person {name:'Carrie-Anne Moss', born:1967})
CREATE (Laurence:Person {name:'Laurence Fishburne', born:1961})
CREATE (Hugo:Person {name:'Hugo Weaving', born:1960})
CREATE (LillyW:Person {name:'Lilly Wachowski', born:1967})
CREATE (LanaW:Person {name:'Lana Wachowski', born:1965})
CREATE (JoelS:Person {name:'Joel Silver', born:1952})
CREATE
  (Keanu)-[:ACTED_IN {roles:['Neo']}]->(TheMatrix),
  (Carrie)-[:ACTED_IN {roles:['Trinity']}]->(TheMatrix),
  (Laurence)-[:ACTED_IN {roles:['Morpheus']}]->(TheMatrix),
  (Hugo)-[:ACTED_IN {roles:['Agent Smith']}]->(TheMatrix),
  (LillyW)-[:DIRECTED]->(TheMatrix),
  (LanaW)-[:DIRECTED]->(TheMatrix),
  (JoelS)-[:PRODUCED]->(TheMatrix)

CREATE (Emil:Person {name:"Emil Eifrem", born:1978})
CREATE (Emil)-[:ACTED_IN {roles:["Emil"]}]->(TheMatrix)
```

然后执行 “match(n) return n” 显示结果如下图所示：
![](https://img-blog.csdnimg.cn/201812191803193.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**完美解决方法**

1.首先需要把数据清空，重来插入节点数据。

```
删除关系
MATCH (n)-[r]-()
DELETE r

单纯删除所以节点：
match (n)
delete n
```

2.然后执行所有代码，包括插入节点及插入关系。关系包括：
**(1) 创建学生-课程之间关系 选课**
**(2) 创建教师-课程之间关系 授课**
**(3) 创建学生-学院之间关系 属于**
**(4) 创建教师-学院之间关系 隶属**
下面这段代码是创建学生和课程之间的关系，代码如下：

```
create(stu1:Student{ name:'王建平', Sex:'男', Sdept:'自动化', Sbirth:'1996-01-12' })
create(stu2:Student{ name:'刘华', Sex:'女', Sdept:'自动化', Sbirth:'1995-07-01' })
create(stu3:Student{ name:'范林军', Sex:'女', Sdept:'计算机', Sbirth:'1994-06-30' })
create(stu4:Student{ name:'李伟', Sex:'男', Sdept:'数学', Sbirth:'1995-05-01' })
create(stu5:Student{ name:'黄艳', Sex:'女', Sdept:'数学', Sbirth:'1996-04-01' })
create(stu6:Student{ name:'何纯', Sex:'男', Sdept:'数学', Sbirth:'1995-06-30' })
create(Course1:Course{ name:'英语', Ctime:'64', Cno:'C01'})
create(Course2:Course{ name:'数据结构', Ctime:'48', Cno:'C02'})
create(Course3:Course{ name:'Python', Ctime:'64', Cno:'C03'})
create(Course4:Course{ name:'计算机网络', Ctime:'32', Cno:'C04'})
create(Course5:Course{ name:'数据库', Ctime:'48', Cno:'C05'})
create(Tea1:Teacher{ name:'杨秀璋', Sex:'男', Dept:'计算机'})
create(Tea2:Teacher{ name:'小民', Sex:'男', Dept:'计算机'})
create(Tea3:Teacher{ name:'小娜', Sex:'女', Dept:'英语'})
create(Tea4:Teacher{ name:'小王', Sex:'男', Dept:'计算机'})
create(Dept1:Department{ name:'自动化学院', No:'001'})
create(Dept2:Department{ name:'计算机学院', No:'002'})
create(Dept3:Department{ name:'英语学院', No:'003'})
create(Dept4:Department{ name:'数学学院', No:'004'})
create 
(stu1)-[:选课]->(Course1), (stu1)-[:选课]->(Course2), (stu1)-[:选课]->(Course3),
(stu2)-[:选课]->(Course1), (stu2)-[:选课]->(Course2), (stu2)-[:选课]->(Course3),
(stu3)-[:选课]->(Course1), (stu3)-[:选课]->(Course2), (stu3)-[:选课]->(Course3), 
(stu3)-[:选课]->(Course4), (stu3)-[:选课]->(Course5),
(stu4)-[:选课]->(Course1), (stu4)-[:选课]->(Course3), (stu4)-[:选课]->(Course5),
(stu5)-[:选课]->(Course1), (stu5)-[:选课]->(Course3), (stu5)-[:选课]->(Course5),
(stu6)-[:选课]->(Course1), (stu6)-[:选课]->(Course3), (stu6)-[:选课]->(Course5)
```

输出结果如下图所示，可以看到选修数据库（Course5）的学生包括何纯、李伟、黄艳和范林军。也可以看到学生何纯（stu6）选修了课程数据库（Course5）、Python（Course3）、英语（Course1）。

(stu6)-[:选课]->(Course1), (stu6)-[:选课]->(Course3), (stu6)-[:选课]->(Course5)
![](https://img-blog.csdnimg.cn/20181219183411710.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
接着是最终的完整CQL语句，代码如下：

```
create(stu1:Student{ name:'王建平', Sex:'男', Sdept:'自动化', Sbirth:'1996-01-12' })
create(stu2:Student{ name:'刘华', Sex:'女', Sdept:'自动化', Sbirth:'1995-07-01' })
create(stu3:Student{ name:'范林军', Sex:'女', Sdept:'计算机', Sbirth:'1994-06-30' })
create(stu4:Student{ name:'李伟', Sex:'男', Sdept:'数学', Sbirth:'1995-05-01' })
create(stu5:Student{ name:'黄艳', Sex:'女', Sdept:'数学', Sbirth:'1996-04-01' })
create(stu6:Student{ name:'何纯', Sex:'男', Sdept:'数学', Sbirth:'1995-06-30' })
create(Course1:Course{ name:'英语', Ctime:'64', Cno:'C01'})
create(Course2:Course{ name:'数据结构', Ctime:'48', Cno:'C02'})
create(Course3:Course{ name:'Python', Ctime:'64', Cno:'C03'})
create(Course4:Course{ name:'计算机网络', Ctime:'32', Cno:'C04'})
create(Course5:Course{ name:'数据库', Ctime:'48', Cno:'C05'})
create(Tea1:Teacher{ name:'杨秀璋', Sex:'男', Dept:'计算机'})
create(Tea2:Teacher{ name:'小民', Sex:'男', Dept:'计算机'})
create(Tea3:Teacher{ name:'小娜', Sex:'女', Dept:'英语'})
create(Tea4:Teacher{ name:'小王', Sex:'男', Dept:'计算机'})
create(Dept1:Department{ name:'自动化学院', No:'001'})
create(Dept2:Department{ name:'计算机学院', No:'002'})
create(Dept3:Department{ name:'英语学院', No:'003'})
create(Dept4:Department{ name:'数学学院', No:'004'})
create 
(stu1)-[:选课]->(Course1), (stu1)-[:选课]->(Course2), (stu1)-[:选课]->(Course3),
(stu2)-[:选课]->(Course1), (stu2)-[:选课]->(Course2), (stu2)-[:选课]->(Course3),
(stu3)-[:选课]->(Course1), (stu3)-[:选课]->(Course2), (stu3)-[:选课]->(Course3), 
(stu3)-[:选课]->(Course4), (stu3)-[:选课]->(Course5),
(stu4)-[:选课]->(Course1), (stu4)-[:选课]->(Course3), (stu4)-[:选课]->(Course5),
(stu5)-[:选课]->(Course1), (stu5)-[:选课]->(Course3), (stu5)-[:选课]->(Course5),
(stu6)-[:选课]->(Course1), (stu6)-[:选课]->(Course3), (stu6)-[:选课]->(Course5)
create
(Tea1)-[:教课]->(Course3),(Tea1)-[:教课]->(Course5),
(Tea2)-[:教课]->(Course2),
(Tea3)-[:教课]->(Course1),
(Tea4)-[:教课]->(Course4),(Tea4)-[:教课]->(Course5)
create
(Tea1)-[:隶属{rela:'普通教师'}]->(Dept2),
(Tea2)-[:隶属{rela:'副院长'}]->(Dept2),
(Tea3)-[:隶属{rela:'副院长'}]->(Dept3),
(Tea4)-[:隶属{rela:'系主任'}]->(Dept1)
create
(stu1)-[:包括]->(Dept1),
(stu2)-[:包括]->(Dept1),
(stu3)-[:包括]->(Dept2),
(stu4)-[:包括]->(Dept4),
(stu5)-[:包括]->(Dept4),
(stu6)-[:包括]->(Dept4);
```

关系：

（1）学生-课程

选课：王建平选择课程1、2、3，刘华选择课程1、2、3，范林军选择课程1、2、3、4、5，李伟、黄艳、何纯同属于数学学院，选课为1、3、5。

（2）教师-课程

授课：杨秀璋教数据库、Python，小民教数据结构，小娜教英语，小王教数据库和计算机网络。

（3）教师-学院

隶属：杨秀璋隶属于计算机学院（属性：普通教师），小民隶属于计算机学院（属性：副院长），小娜隶属于英语学院（属性：副院长），小王隶属于自动化学院（属性：系主任）。

（4）学生-学院

包括：自动化学院包括学生王建平、刘华，计算机学院包括学生范林军，数学学院包括李伟、黄艳、何纯。
最终输出的关系如下图所示：
![](https://img-blog.csdnimg.cn/20181219185500824.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20181219185520820.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
写到这里这篇文章基本结束，我后续将继续深入学习找到更好地解决方法，MATCH中增加WHERE应该可以定位指定节点，实现定向的关系补充或修改。同时，如果数据量多我也建议使用CSV文件导入。总之，希望基础文章对您有所帮助，一起加油。

(By: Eastmount 2018-12-19 晚7点 [https://blog.csdn.net/Eastmount/](https://blog.csdn.net/Eastmount/) )






