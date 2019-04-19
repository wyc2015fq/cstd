# left join 和 left outer join 的区别 - sxf_123456的博客 - CSDN博客
2017年08月23日 16:13:52[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：786
个人分类：[mysql](https://blog.csdn.net/sxf_123456/article/category/7122238)
## [left join 和 left outer join 的区别](http://www.cnblogs.com/cy163/archive/2008/10/16/1312920.html)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)通俗的讲：  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  A   leftjoin   B   的连接的记录数与A表的记录数同  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  A   rightjoin   B   的连接的记录数与B表的记录数同    
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  A   leftjoin   B   等价B   rightjoin   A  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)table   A:  
Field_K,   Field_A  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**1**                       a  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**3**                       b  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**4**                       c  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)table   B:  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  Field_K,   Field_B  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**1**                       x  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**2**                       y  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**4**                       z  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)select   a.Field_K,   a.Field_A,   b.Field_K,   b.Field_B  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)from   a   leftjoin   b   on   a.Field_K=b.Field_K  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  Field_K         Field_A         Field_K         Field_B          
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)----------   ----------   ----------   ----------    
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**1**                     a                     **1**                     x                    
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**3**                     b                     NULLNULL
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**4**                     c                     **4**                     z                    
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)select   a.Field_K,   a.Field_A,   b.Field_K,   b.Field_B  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)from   a   rightjoin   b   on   a.Field_K=b.Field_K  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  Field_K         Field_A         Field_K         Field_B          
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)----------   ----------   ----------   ----------    
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**1**                     a                     **1**                     x                    
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)NULLNULL**2**                     y                    
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**4**                     c                     **4**                     z      
 --
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)举个例子：  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  假设ａ表和ｂ表的数据是这样的。  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  a                         b    
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  id     name　　id     stock　  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**1**　　a             **1****15**
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**2**         b             **2****50**
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**3**         c               　  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)select*from   a   innerjoin   b   on   a.id=b.id  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  这个语法是连接查询中的内连接，它产生的结果是  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  两个表相匹配的记录出现在结果列表中。  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  根据上面的表，出现的结果是这样的  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  a.id     name     b.id     stock  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**1**　　     a             **1****15**
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**2**             b             **2****50**
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)----------------------------  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)select*from   a,b   where   a.id=b.id  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  这个语法是内连接的另外一种写法，其执行结果与inner   join   一样  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)--------------------------------    
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)select*from   a   left/rightjoin   b   on   a.id=b.id  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  这个是外连接语法中的左外连接或右外连接  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  如果是左外连接的话，它将显示ａ表的所有记录，  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)select   a.*,b.*from   a   leftjoin   b   on   a.id=b.id  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  查询的结果是这样的：  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  a.id     name     b.id     stock  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**1**　　       a         **1****15**
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**2**               b         **2****50**
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**3**               c       nullnull
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)--------------------------------------------  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  如果是右外连接的话，它将显示ｂ表的所有记录，  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)select   a.*,b.*from   a   rightjoin   b   on   a.id=b.id  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  查询的结果是这样的：  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  a.id     name     b.id     stock  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**1**　　       a         **1****15**
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**2**               b         **2****50**
--
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)select   a.*,b.*from   a   leftjoin   b   on   a.k   =   b.k    
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)select   a.*,b.*from   a   leftouterjoin   b   on   a.k   =b.k  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)----------上面两种一样left   join是left   outer   join的简写  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)select   a.*,b.*from   a   leftinnerjoin   b   on   a.k   =   b.k    
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  没有这种写法，错误的语句.
--
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)在你要使用多个left   join的时候  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  比如说10个  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  我们把10个全都写成left   join的形式  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  然后再SQL让他自动运行一下，它会把最后一次出现的left   join变成left   outerjoin
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  所以依此推理，最后一个left   join会以left   outer   join的形式存在  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  当然，不管变不变对结果的显示没有任何影响  
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  希望我的实验能对你有所帮助   
--
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)使用关系代数合并数据
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**1** 关系代数
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)合并数据集合的理论基础是关系代数，它是由E.F.Codd于1970年提出的。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)在关系代数的形式化语言中：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        用表、或者数据集合表示关系或者实体。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        用行表示元组。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        用列表示属性。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)关系代数包含以下8个关系运算符
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        选取――返回满足指定条件的行。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        投影――从数据集合中返回指定的列。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        笛卡尔积――是关系的乘法，它将分别来自两个数据集合中的行以所有可能的方式进行组合。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        并――关系的加法和减法，它可以在行的方向上合并两个表中的数据，就像把一个表垒在另一个表之上一样。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        交――返回两个数据集合所共有的行。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        差――返回只属于一个数据集合的行。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        连接――在水平方向上合并两个表，其方法是：将两个表中在共同数据项上相互匹配的那些行合并起来。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        除――返回两个数据集之间的精确匹配。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)此外，作为一种实现现代关系代数运算的方法，SQL还提供了：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        子查询――类似于连接，但更灵活；在外部查询中，方式可以使用表达式、列表或者数据集合的地方都可以使用子查询的结果。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)本章将主要讲述多种类型的连接、简单的和相关的子查询、几种类型的并、关系除以及其他的内容。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**2** 使用连接
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**2.1** 连接类型
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)在关系代数中，连接运算是由一个笛卡尔积运算和一个选取运算构成的。首先用笛卡尔积完成对两个数据集合的乘运算，然后对生成的结果集合进行选取运算，确保只把分别来自两个数据集合并且具有重叠部分的行合并在一起。连接的全部意义在于在水平方向上合并两个数据集合（通常是表），并产生一个新的结果集合，其方法是将一个数据源中的行于另一个数据源中和它匹配的行组合成一个新元组。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)SQL提供了多种类型的连接方式，它们之间的区别在于：从相互交叠的不同数据集合中选择用于连接的行时所采用的方法不同。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)连接类型        定义
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)内连接        只连接匹配的行
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)左外连接        包含左边表的全部行（不管右边的表中是否存在与它们匹配的行），以及右边表中全部匹配的行
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)右外连接        包含右边表的全部行（不管左边的表中是否存在与它们匹配的行），以及左边表中全部匹配的行
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)全外连接        包含左、右两个表的全部行，不管另外一边的表中是否存在与它们匹配的行。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)(H)(theta)连接        使用等值以外的条件来匹配左、右两个表中的行
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)交叉连接        生成笛卡尔积－它不使用任何匹配或者选取条件，而是直接将一个数据源中的每个行与另一个数据源的每个行都一一匹配
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)在INFORMIX中连接表的查询
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)如果FROM子句指定了多于一个表引用，则查询会连接来自多个表的行。连接条件指定各列之间（每个表至少一列）进行连接的关系。因为正在比较连接条件中的列，所以它们必须具有一致的数据类型。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)SELECT语句的FROM子句可以指定以下几种类型的连接
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)FROM子句关键字        相应的结果集
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)CROSSJOIN        笛卡尔乘积（所有可能的行对）
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)INNERJOIN        仅对满足连接条件的CROSS中的列
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)LEFTOUTERJOIN        一个表满足条件的行，和另一个表的所有行
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)RIGHTOUTERJOIN        与LEFT相同，但两个表的角色互换
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)FULLOUTERJOINLEFTOUTER 和 RIGHT OUTER中所有行的超集
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**2.2** 内连接（InnerJoin）
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)内连接是最常见的一种连接，它页被称为普通连接，而E.FCodd最早称之为自然连接。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)下面是ANSI SQL－92标准
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)select*
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)from  t_institution i
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)innerjoin t_teller t
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)on i.inst_no = t.inst_no
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)where i.inst_no = "**5801**"
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)其中inner可以省略。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)等价于早期的连接语法
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)select*
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)from t_institution i, t_teller t
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)where i.inst_no = t.inst_no
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)and i.inst_no = "**5801**"
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**2.3** 外连接
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**2.3**.**1**        左外连接(LeftOuter Jion)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)select*
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)from  t_institution i
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)leftouterjoin t_teller t
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)on i.inst_no = t.inst_no
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)其中outer可以省略。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**2.3**.**2**        右外连接(Rigt Outer Jion)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)select*
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)from  t_institution i
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)rightouterjoin t_teller t
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)on i.inst_no = t.inst_no
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**2.3**.**3**        全外连接(FullOuter)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)全外连接返回参与连接的两个数据集合中的全部数据，无论它们是否具有与之相匹配的行。在功能上，它等价于对这两个数据集合分别进行左外连接和右外连接，然后再使用消去重复行的并操作将上述两个结果集合并为一个结果集。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)在现实生活中，参照完整性约束可以减少对于全外连接的使用，一般情况下左外连接就足够了。在数据库中没有利用清晰、规范的约束来防范错误数据情况下，全外连接就变得非常有用了，你可以使用它来清理数据库中的数据。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)select*
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)from  t_institution i
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)fullouterjoin t_teller t
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)on i.inst_no = t.inst_no
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**2.3**.**4**        外连接与条件配合使用
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)当在内连接查询中加入条件是，无论是将它加入到join子句，还是加入到where子句，其效果是完全一样的，但对于外连接情况就不同了。当把条件加入到 join子句时，SQL Server、Informix会返回外连接表的全部行，然后使用指定的条件返回第二个表的行。如果将条件放到where子句中，SQL Server将会首先进行连接操作，然后使用where子句对连接后的行进行筛选。下面的两个查询展示了条件放置位子对执行结果的影响：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)条件在join子句
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)select*
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)from  t_institution i
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)leftouterjoin t_teller t
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)on i.inst_no = t.inst_no
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)and i.inst_no = “**5801**”
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)结果是：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)inst_no    inst_name            inst_no    teller_no  teller_name
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**5801**       天河区               **5801****0001**       tom
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**5801**       天河区               **5801****0002**       david
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**5802**       越秀区
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**5803**       白云区
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)条件在where子句
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)select*
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)from  t_institution i
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)leftouterjoin t_teller t
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)on i.inst_no = t.inst_no
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)where i.inst_no = “**5801**”
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)结果是：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)inst_no    inst_name            inst_no    teller_no  teller_name
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**5801**       天河区               **5801****0001**       tom
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**5801**       天河区               **5801****0002**       david
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**2.4** 自身连接
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)自身连接是指同一个表自己与自己进行连接。这种一元连接通常用于从自反关系（也称作递归关系）中抽取数据。例如人力资源数据库中雇员与老板的关系。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)下面例子是在机构表中查找本机构和上级机构的信息。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)select s.inst_no superior_inst, s.inst_name sup_inst_name, i.inst_no, i.inst_name
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)from t_institution i
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)join t_institution s
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)on i.superior_inst = s.inst_no
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)结果是：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)superior_inst sup_inst_name        inst_no    inst_name
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**800**           广州市               **5801**       天河区
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**800**           广州市               **5802**       越秀区
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**800**           广州市               **5803**       白云区
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)**2.5** 交叉(无限制) 连接
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)交叉连接用于对两个源表进行纯关系代数的乘运算。它不使用连接条件来限制结果集合，而是将分别来自两个数据源中的行以所有可能的方式进行组合。数据集合中一的每个行都要与数据集合二中的每一个行分别组成一个新的行。例如，如果第一个数据源中有5个行，而第二个数据源中有4个行，那么在它们之间进行交叉连接就会产生20个行。人们将这种类型的结果集称为笛卡尔乘积。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)大多数交叉连接都是由于错误操作而造成的；但是它们却非常适合向数据库中填充例子数据，或者预先创建一些空行以便为程序执行期间所要填充的数据保留空间。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)select*
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)from  t_institution i
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)crossjoin t_teller t
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)在交叉连接中没有on条件子句
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
