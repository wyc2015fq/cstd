
# Oracle的rownum和分页 - 李昆鹏的博客 - CSDN博客


2018年06月06日 21:16:44[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：1221


--------------------------------------------Oracle的rownum和分页--------------------------------------------------
**Rownum**
ROWNUM:表示行号，实际上此是一个列,但是这个列是一个伪列,此列可以在每张表中出现。
范例：查询emp表带有rownum列
selectrownum, t.*fromemp t
![](https://img-blog.csdn.net/20180606211551835?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
我们可以根据rownum来取结果集的前几行，比如前5行
![](https://img-blog.csdn.net/20180606211557884?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
但是我们不能取到中间几行，因为rownum不支持大于号，只支持小于号，如果想                 实现我们的需求怎么办呢？答案是使用子查询，也正是oracle分页的做法。
select*
from(selectrownumrm, a.*from(select*fromemp) awhererownum<11) bwhereb.rm >5
代码示例：
|select rownum, t.* from emp t; --|使用|rownum|来显示行号
|select rownum, t.* from emp t where rownum<6; --|使用|rownum|来限制查找的行数
|--rownum|是不支持大于号的，所以下面的这种分页方式是错误的
|select rownum ,t.* from emp t where rownum>5 and rownum < 11;
|--|分页步骤
|--|第一步查询全量的数据
|select * from emp;
|--|第二步以第一步的结果集作为一张表，限定条件是|rownum|小于结束行，结果列把|rownum|作为结果集
|select rownum rw, a.* from (select * from emp) a where rownum < 6;
|--|第三步以第二步的结果集作为一张表，限定条件是第二步的|rownum|列大于开始行号，结果是|*
|select *
|from
|(select rownum rw, a.* from
|(select * from emp) a --|其实第一步是可以省略的，但是在实际开发中会限制条件查询，使用这三步操作会更加方便
|where rownum < 11) b
|where b.rw > 5;
|/*
|pageNo:|当前的页码
|pageSize:|每页的记录数
|pageNo      pageSize      startNum        endNum
|1            5              1             6
|2            5              5             11
|3            5              10            16
|已知|pageNo|和|pageSize|数计算|startNum|和|endNum|的公式
|startNum = (pageNo - 1) *  pageSize
|endNum = (pageNo * pageSize) +  1
|*/
|


