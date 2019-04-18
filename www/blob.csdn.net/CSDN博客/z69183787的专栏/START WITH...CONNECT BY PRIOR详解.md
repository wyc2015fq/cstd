# START WITH...CONNECT BY PRIOR详解 - z69183787的专栏 - CSDN博客
2014年03月08日 11:18:08[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2136
**START WITH...CONNECT BY PRIOR详解**
    ORACLE中的SELECT语句可以用START WITH...CONNECT BY PRIOR子句实现递归查询，CONNECT BY 是结构化查询中用到的，其基本语法是：
SELECT columnName1[,columnName2,…]
FROM tableName1,tableName2,..
[START WITH columnName3] 
CONNECT BY {PRIOR col 1 = col 2 | col 1 = PRIOR col 2}
WHERE columnName4;
**上述语法主要用查询表中的树型结构关系**
columnName3是根结点的限定语句，当然可以放宽限定条件，以取得多个根结点，实际就是多棵树。
{PRIOR col 1 = col 2 | col 1 = PRIOR col 2}是连接条件
PRIOR表示强制从根节点到叶节点的顺序检索或从强制从叶节点到根节点的顺序检索
columnName4表示过滤条件，是对结果树进行过滤，注意是先有结果树再过滤，不是先过滤再得到结果树
**例1**：
SELECT * FROM EMP
START WITH EMPNO=7369  
CONNECT BY  PRIOR MGR = EMPNO;
上述查询语句，从根节点empno = 7369开始查询雇员编号和对应的领导编号的关系，PRIOR放在等号的前面表示从根节点到叶节点查询。如果不写明PRIOR表示默认从叶节点到根节点查询。
**例2**：
SELECT * FROM EMP
WHERE empno <>  7902
START WITH EMPNO=7369  
CONNECT BY  PRIOR MGR = EMPNO;
上述查询语句，从根节点empno = 7369开始查询雇员编号和对应的领导编号的关系，得到一棵树，WHERE子句是把这棵树中的EMPNO=7902的节点去掉。
**知识扩展1．树结构的描述**树结构的数据存放在表中，数据之间的层次关系即父子关系，通过表中的列与列间的关系来描述，如emp 表中的empno 和mgr 。empno 表示该雇员的编号，mgr 表示该雇员对应的领导的编号，即子节点的empno 值等于父节点的mgr 值。在表的每一行中都有一个表示父节点的mgr （除根节点外），通过每个节点的父节点，就可以确定整个树结构。
在SELECT 语句中使用START WITH 和CONNECT BY子句可以查询表中的树型结构关系。其语法如下：
SELECT语句
[START WITH]
CONNECT BY {PRIOR 列名1 = 列名2| 列名1 = PRIOR 列名2};
其中：CONNECT BY 子句说明每行数据将是按层次顺序检索，并规定将表中的数据连入树型结构的关系中。PRIORY 运算符必须放置在连接关系的两列中某一个的前面。对于节点间的父子关系，PRIOR 运算符所在的列表示该列是父节点，相应的另一列则表示子节点，从而确定查找树结构是的顺序是自顶向下还是自底向上。在连接关系中，除了可以使用列名外，还允许使用列表达式。**START WITH ****子句为可选项**，用来标识哪个节点作为查找树型结构的根节点。若该子句被省略，则表示所有满足查询条件的行作为根节点。
**START WITH选项**：不但可以指定一个根节点，还可以指定多个根节点。
**例3**：
SELECT * FROM EMP
START WITH EMPNO=7369 OR EMPNO = 7521 
CONNECT BY PRIOR MGR = EMPNO;
上述查询语句，从根节点empno = 7369或empno = 7521开始查询雇员编号和对应的领导编号的关系，得到两个根节点
**2．关于PRIOR**
运算符PRIOR 被放置于等号前后的位置，决定着查询时的检索顺序。
PRIOR 被置于CONNECT BY 子句中等号的前面时，则强制从根节点到叶节点的顺序检索，即由父节点向子节点方向通过树结构，我们称之为自顶向下的方式。如：
CONNECT BY PRIOR EMPNO=MGR 
PIROR 运算符被置于CONNECT BY 子句中等号的后面时，则强制从叶节点到根节点的顺序检索，即由子节点向父节点方向通过树结构，我们称之为自底向上的方式。例如：
CONNECT BY EMPNO=PRIOR MGR 
在这种方式中也应指定一个开始的节点。
**3．定义查找起始节点**
在自顶向下查询树结构时，不但可以从根节点开始，还可以定义任何节点为起始节点，以此开始向下查找。这样查找的结果就是以该节点为开始的结构树的一枝。
**4．使用LEVEL**在具有树结构的表中，每一行数据都是树结构中的一个节点，由于节点所处的层次位置不同，所以每行记录都可以有一个层号。层号根据节点与根节点的距离确定。不论从哪个节点开始，该起始根节点的层号始终为1 ，根节点的子节点为2 ，依此类推。
**5．节点和分支的裁剪**在对树结构进行查询时，可以去掉表中的某些行，也可以剪掉树中的一个分支，使用WHERE 子句来限定树型结构中的单个节点，以去掉树中的单个节点，但它却不影响其后代节点（自顶向下检索时）或前辈节点（自底向顶检索时），如上例2。
**6．排序显示**
如同在其它查询中一样，在树结构查询中也可以使用ORDER BY 子句，改变查询结果的显示顺序，而不必按照遍历树结构的顺序。
**例4**
SELECT * FROM EMP
START WITH EMPNO=7902  
CONNECT BY PRIOR MGR = EMPNO
ORDER BY empno ASC;
