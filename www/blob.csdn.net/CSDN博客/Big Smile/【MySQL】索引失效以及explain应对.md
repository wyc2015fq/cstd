# 【MySQL】索引失效以及explain应对 - Big Smile - CSDN博客
2018年08月01日 00:31:40[王啸tr1912](https://me.csdn.net/tr1912)阅读数：953
所属专栏：[mysql系列](https://blog.csdn.net/column/details/26482.html)
        我们都知道在数据表中建立索引是可以增加查询的效率的，但是不知道同志们有没有注意过索引失效的情况，就是你明明建立了索引，但是却和没有建立索引的时候查询的时间是一样的。
# 前提
** 索引是什么？**
        MySQL官方对索引的定义为：索引（Index）是帮助MySQL高效获取数据的数据结构。可以得到索引的本质：索引是数据结构。
  使用Navicat建立索引的时候会发现可以选择两种数据结构：
![](https://img-blog.csdn.net/20180731200024810?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
hash和Btree都是我们非常熟悉的数据结构，再盗一张图：
![](https://img-blog.csdn.net/20180731200102643?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
图中是以二叉搜索树为数据结构存储的数据，我们可以很轻易的就查询到想要查询的数据。
**注意：**索引可以是B+树，也就是N叉树。聚集索引，次要索引，覆盖索引，复合索引，前缀索引，唯一索引默认都是使用B+树索引，统称索引。
**什么情况下使用索引？**
- 主键自动建立唯一索引
- 频繁作为查询条件的字段应该创建索引
- 查询中与其它表关联的字段，外键关系建立索引
- 频繁更新的字段不适合创建索引
- Where条件里用不到的字段不创建索引
- 单键/组合索引的选择问题，who？(在高并发下倾向创建组合索引)
- 查询中排序的字段，排序字段若通过索引去访问将大大提高排序速度
- 查询中统计或者分组字段
**什么情况下不需要建立索引：**
-  表记录太少
- 经常增删改的表
- 数据重复且分布平均的表字段，因此应该只为最经常查询和最经常排序的数据列建立索引。注意，如果某个数据列包含许多重复的内容，为它建立索引就没有太大的实际效果。
        好了，知道了索引是什么，以及使用索引的优缺点了之后，我们接下来看看索引是怎么生效的，又是怎么失效的，首先我们需要了解一下mysql自带的索引分析工具Explain。
# 一、Explain
** explain是什么呢？**
      使用EXPLAIN关键字可以模拟优化器执行SQL查询语句，从而知道MySQL是如何处理你的SQL语句的。分析你的查询语句或是表结构的性能瓶颈。
** explain能做什么呢？**
- 查看表的读取顺序
- 数据读取操作的操作类型
- 哪些索引可以使用
- 哪些索引被实际使用
- 表之间的引用
- 张表有多少行被优化器查询
        由于是mysql内置的，所以这个关键字的执行效率，还有活的信息的准确性和专业性，都是值得信赖的。先让我们看一下这个explain要怎么用吧：
**Explain + SQL语句**
对，就是这么简单，只是需要这样就可以使用这个关键字了，我们看一下使用这个关键字的查询结果都是什么：
![](https://img-blog.csdn.net/20180731203129594?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
下面来解释一下这些字段的意思：
**1、id：**select查询的序列号,包含一组数字，表示查询中执行select子句或操作表的顺序，他分为三种情况：
- id相同，执行顺序由上至下
- id不同，如果是子查询，id的序号会递增，id值越大优先级越高，越先被执行
- id相同不同，同时存在
**2、select_type：**查询的类型，主要是用于区别普通查询、联合查询、子查询等的复杂查询，包括如下：
- SIMPLE：简单的 select 查询,查询中不包含子查询或者UNION
- PRIMARY：查询中若包含任何复杂的子部分，最外层查询则被标记为
- SUBQUERY：在SELECT或WHERE列表中包含了子查询
- DERIVED：在FROM列表中包含的子查询被标记为DERIVED(衍生)MySQL会递归执行这些子查询, 把结果放在临时表里。
- UNION：若第二个SELECT出现在UNION之后，则被标记为UNION；若UNION包含在FROM子句的子查询中,外层SELECT将被标记为：DERIVED
- UNION RESULT：从UNION表获取结果的SELECT
**3、table：**显示这一行的数据是关于哪张表的
**4、type：**显示查询使用了何种类型，从最好到最差依次是：system>const>eq_ref>ref>range>index>ALL
- system：表只有一行记录（等于系统表），这是const类型的特列，平时不会出现，这个也可以忽略不计
- const：表示通过索引一次就找到了,const用于比较primary key或者unique索引。因为只匹配一行数据，所以很快如将主键置于where列表中，MySQL就能将该查询转换为一个常量
- eq_ref：唯一性索引扫描，对于每个索引键，表中只有一条记录与之匹配。常见于主键或唯一索引扫描。
- ref：非唯一性索引扫描，返回匹配某个单独值的所有行。本质上也是一种索引访问，它返回所有匹配某个单独值的行，然而，它可能会找到多个符合条件的行，所以他应该属于查找和扫描的混合体
- range：只检索给定范围的行,使用一个索引来选择行。key 列显示使用了哪个索引一般就是在你的where语句中出现了between、<、>、in等的查询这种范围扫描索引扫描比全表扫描要好，因为它只需要开始于索引的某一点，而结束语另一点，不用扫描全部索引。
- index：Full Index Scan，index与ALL区别为index类型只遍历索引树。这通常比ALL快，因为索引文件通常比数据文件小。（也就是说虽然all和Index都是读全表，但index是从索引中读取的，而all是从硬盘中读的）
- all：Full Table Scan，将遍历全表以找到匹配的行
- 一般来说，得保证查询至少达到range级别，最好能达到ref。
**5、possible_keys**：显示可能应用在这张表中的索引，一个或多个。查询涉及到的字段上若存在索引，则该索引将被列出，**但不一定被查询实际使用**
**6、key：** 实际使用的索引。如果为NULL，则没有使用索引，查询中若使用了覆盖索引，则该索引仅出现在key列表中。
**7、key_len：**表示索引中使用的字节数，可通过该列计算查询中使用的索引的长度。在不损失精确性的情况下，长度越短越好。
**8、ref：**显示索引的哪一列被使用了，如果可能的话，是一个常数。哪些列或常量被用于查找索引列上的值
**9、rows：**根据表统计信息及索引选用情况，大致估算出找到所需的记录所需要读取的行数
**10、Extra：**包含不适合在其他列中显示但十分重要的额外信息
- Using filesort：说明mysql会对数据使用一个外部的索引排序，而不是按照表内的索引顺序进行读取。MySQL中无法利用索引完成的排序操作称为“文件排序”。
- Using temporary ：使了用临时表保存中间结果,MySQL在对查询结果排序时使用临时表。常见于排序 order by 和分组查询 group by。
- USING index：表示相应的select操作中使用了覆盖索引(Covering Index)，避免访问了表的数据行，效率不错！如果同时出现using where，表明索引被用来执行索引键值的查找，如果没有同时出现using where，表明索引用来读取数据而非执行查找动作。
- Using where：表明使用了where过滤
- using join buffer：使用了连接缓存
- impossible where：where子句的值总是false，不能用来获取任何元组
- select tables optimized away：在没有GROUPBY子句的情况下，基于索引优化MIN/MAX操作或者对于MyISAM存储引擎优化COUNT(*)操作，不必等到执行阶段再进行计算，查询执行计划生成的阶段即完成优化。
- distinct：优化distinct操作，在找到第一匹配的元组后即停止找同样值的动作
 由于有这么强大的explain关键字，所以我们分析索引也变的好多了。
# 二、索引有效性分析
        首先我们有个staffs表，里面的字段如图所示：
![](https://img-blog.csdn.net/20180731234543819?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
然后，我们给name，age，pos建立一个联合索引   idx_staffs_nameAgePos  来进行下面的实验：
```
EXPLAIN SELECT * FROM staffs WHERE NAME = 'July';
EXPLAIN SELECT * FROM staffs WHERE NAME = 'July' AND age = 25;
EXPLAIN SELECT * FROM staffs WHERE NAME = 'July' AND age = 25 AND pos = 'dev';
```
我们执行如上述三条命令，然后我们来看一下结果：
![](https://img-blog.csdn.net/20180731234909377?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
        这个时候我们可以发现我们建立的索引在查询中全部起作用了，因为我们的type全都达到了最好的ref，而且key字段也是有值的，ref中有使用了哪个字段的因素。
由此可以得出：
where子句查询的是我们建立索引的字段的时候，索引的查询会全部生效的。
```
EXPLAIN SELECT * FROM staffs WHERE age = 25 AND pos = 'dev'; 
EXPLAIN SELECT * FROM staffs WHERE pos = 'dev';
```
![](https://img-blog.csdn.net/20180801000745975?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
从这里我们也可以看出，如果和建立的索引不同的话，只要有name还是会使用到索引的，但是没有name，有其他的不行。
综上我们可以得出第一条：
**1、按照建立索引的字段顺序进行从左到右的条件查询的话，我们的索引是会全部命中的，其他的情况少了name，或者是中间缺少了age，都是不行了，必须按照name，age和pos的顺序来**
`EXPLAIN SELECT * FROM staffs WHERE left(NAME,4) = 'July';`
![](https://img-blog.csdn.net/20180801001713810?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**2、如果加入了运算符的话，我们的索引查询也会失效。**
![](https://img-blog.csdn.net/20180801001829599?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
通过看第四个查询，我们可以看出，只有两个索引起作用了。
**3、范围条件右边的索引不生效**
![](https://img-blog.csdn.net/20180801002113477?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**4、使用“!=”和“<>”都会使索引失效**
![](https://img-blog.csdn.net/20180801002205543?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**5、使用null判断也无法使索引起效**
![](https://img-blog.csdn.net/20180801002338350?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**6、模糊查询的通配符%也会使索引失效。**
![](https://img-blog.csdn.net/20180801002435205?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**7、字符串类型不加‘’，虽然也能查询出来，但是索引也会失效**
![](https://img-blog.csdn.net/20180801002531837?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**8、使用OR查询的时候，也会使索引失效**
**9、使用order by和group by中的条件也会遵循第一条的结论，并且回接续where来使用条件顺序**
比如有索引index（a,b,c）
select * from table where a='1' order by b group by c
也会生效的，如果顺序不对，则不会生效。
# 三、建议
-     对于单键索引，尽量选择针对当前query过滤性更好的索引
-     在选择组合索引的时候，当前Query中过滤性最好的字段在索引字段顺序中，位置越靠前越好。
-    在选择组合索引的时候，尽量选择可以能够包含当前query中的where字句中更多字段的索引
-    尽可能通过分析统计信息和调整query的写法来达到选择合适索引的目的
-    少用Hint强制索引
