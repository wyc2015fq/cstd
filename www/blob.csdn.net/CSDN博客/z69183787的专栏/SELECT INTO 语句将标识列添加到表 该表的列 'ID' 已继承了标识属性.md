# SELECT INTO 语句将标识列添加到表 该表的列 'ID' 已继承了标识属性 - z69183787的专栏 - CSDN博客
2013年11月29日 16:19:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3732
from:[http://www.cnblogs.com/icewindq/](http://www.cnblogs.com/icewindq/)
SQL 2005标识列继承问题
      在点击查看列表时，出现了以下一个异常：
无法使用 SELECT INTO 语句将标识列添加到表 '#TMP_FB37F2DD_6103_414A_931B_6EB0D9664369'，该表的列 'ID' 已继承了标识属性。
这个主要我们在取数据时，用到了一个分页的存储过程，将一页的数据插入到临时表，代码片断如下： 
Code
--创建一个用于分页的空的临时表
SELECT IDENTITY(BIGINT,1,1) AS ROWID,
       * INTO #TMP_39F71656_CFF0_4FAA_BE24_9E62B12488E1
FROM   (
           SELECT TOP 0 *
           FROM   T_forum_speaker
           WHERE Forum_ID = 1169
             AND fallnum = 13
           ORDER BY
                  orderid
       ) AS T ; 
此时这个临时表#TMP_39F71656_CFF0_4FAA_BE24_9E62B12488E1可能会有两个标识列：ROWID和ID（表 T_forum_speaker的主键），根据微软的官方文档，这是不允许，如下所示：
将现有标识列选入新表时，新列将继承 IDENTITY 属性，除非下列条件中的一个成立：
·SELECT 语句包含联接、GROUP BY 子句或聚合函数。
·多个 SELECT 语句由 UNION 联接。
·标识列在选择列表内多次列出。
·标识列是表达式的一部分。
·标识列来自远程数据源。
请参阅[http://technet.microsoft.com/zh-cn/library/ms188029.aspx](http://technet.microsoft.com/zh-cn/library/ms188029.aspx)
