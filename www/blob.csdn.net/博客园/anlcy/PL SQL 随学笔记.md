
# PL-SQL 随学笔记 - anlcy - 博客园






# [PL\SQL 随学笔记](https://www.cnblogs.com/camilla/p/8487473.html)
一.在PL\SQL语句块begin。。。end;中，不能直接使用select，必须与into结合查询。
例如：
declare aa：=22;
id2 integer;
begin
select * from tabname where id=aa ；
end;//提示错误该select语句中缺少into子句
正确：select count（*）into id2  from tabname where id=aa ；
当然这样只能得到一条数据，如果需要多数据查询可以使用游标；
二.oracle循环使用：在PL/SQL中可以使用LOOP语句对数据进行循环处理，利用该语句可以循环执行指定的语句序列。常用的LOOP循环语句包含3种形式：基本的LOOP、WHILE...LOOP和FOR...LOOP。
create or replace package body LOOP_TEST_DEMO IS
  --while循环
  procedure loop_while(start_value in number, end_value in number) is
    current_value number := start_value;
  begin
    while current_value <= end_value loop
      dbms_output.put_line('now number:' || current_value);
      current_value := current_value + 1;
    end loop;
  end loop_while;
  --数值型For循环
  procedure loop_num_for(lowest in number, highest in number) is
  begin
    FOR even_number in lowest .. highest
    --升序
     loop
      --dbms_output.put_line(even_number);
      --处理非平滑增长的索引
      if mod(even_number, 2) = 0 then
        dbms_output.put_line('now number:' || even_number);
      end if;
    end loop;
    --降序
    FOR even_number in reverse lowest .. highest loop
      dbms_output.put_line('now number:' || even_number);
    end loop;
  end loop_num_for;
  --游标型For循环
  procedure loop_cursor_for is
  begin
    declare
      cursor userinfo_cur is
        select * from greenet_user_info;
    begin
      FOR userinfo_rec in userinfo_cur loop
        dbms_output.put_line('username is:' || userinfo_rec.user_name);
      end loop;
    end;
  end loop_cursor_for;
end LOOP_TEST_DEMO;
--使用loop 循环：首先定义游标和变量
CURSOR C1 IS SELECT  eNAME,ejob FROM emp WHERE deptno=10;
v_NAME VARCHAR2(10);--变量
v_job VARCHAR2(10);--变量
第一种：使用loop 循环
open c1;
loop
fetch c1  into v_name,v_job ;   --一般放在一个循环体中，每执行一次，得到一行记录，相当于next记录指针，并赋给INTO所对应的变量。
exit when c1%notfound;   --没有记录的话执行其它操作
……
end loop
colse c1;
这里需要注意，exit when语句一定要紧跟在fetch之后。必避免多余的数据处理。
而且处理逻辑需要跟在exit when之后。
循环结束后要记得关闭游标。
存储过程游标循环示例：CREATE OR REPLACE PROCEDURE sp_sync_plan IS 
  CURSOR C_EMP IS --声明显式游标  
    SELECT * FROM dc_check_todo;  
  C_ROW C_EMP%ROWTYPE; --定义游标变量，该变量的类型为基于游标C_EMP的记录  
BEGIN 
  --For 循环  
  FOR C_ROW IN C_EMP LOOP  
    DBMS_OUTPUT.PUT_LINE(C_ROW.todo_id || '--' );  
  END LOOP;  
 
  --Fetch 循环  
  OPEN C_EMP;--必须要明确的打开和关闭游标  
  LOOP  
    FETCH C_EMP  
      INTO C_ROW;  
    EXIT WHEN C_EMP%NOTFOUND;  
    DBMS_OUTPUT.PUT_LINE(C_ROW.todo_id || '++' );  
  END LOOP;  
  CLOSE C_EMP;  
 
  --While 循环  
  OPEN C_EMP;--必须要明确的打开和关闭游标  
    FETCH C_EMP INTO C_ROW;  
    WHILE C_EMP%FOUND LOOP  
      DBMS_OUTPUT.PUT_LINE(C_ROW.todo_id || '**' );  
      FETCH C_EMP INTO C_ROW;  
    END LOOP;  
  CLOSE C_EMP;  
END sp_sync_plan;
三、日期操作：
1.查询当前日期的上一年日期：
select to_char(add_months(trunc(sysdate,'year'),-12),'yyyymmdd'), to_char(last_day(add_months(trunc(sysdate,'year'),-1)),'yyyymmdd') from dual;
2.日期加减：
select sysdate,add_months(sysdate,（-）12) from dual;        --（减）加1年
select sysdate,add_months(sysdate,（-）1) from dual;        --（减）加1月
select sysdate,to_char(sysdate+（-）7,'yyyy-mm-dd HH24:MI:SS') from dual;  --加（减）1星期
select sysdate,to_char(sysdate+（-）1,'yyyy-mm-dd HH24:MI:SS') from dual;  --加（减）1天
select sysdate,to_char(sysdate+（-）1/24,'yyyy-mm-dd HH24:MI:SS') from dual;  --加（减）1小时
select sysdate,to_char(sysdate+（-）1/24/60,'yyyy-mm-dd HH24:MI:SS') from dual;  --加（减）1分钟
select sysdate,to_char(sysdate+（-）1/24/60/60,'yyyy-mm-dd HH24:MI:SS') from dual;  --加（减）1秒四、oracle数值函数 abs()、 ceil()、 cos()、 cosh()精解:
1、格式：abs(number)   即   abs(数值）
返回数值的绝对值。
例：abs(5)   返回  5  || abs(-5)   返回  5
2、格式：ceil(number)   即   ceil(数值）
根据输入值返回一个数值，输入参数可以是非整数，但返回结果则是大于等于输入参数的最小整数。
例：ceil(5.1)   返回  6 || abs(-5.2)   返回  -5
3、格式：cos(number)   即   cos(数值）
返回角度的余弦值。
例：cos(0)   返回  1 || cos(90)  返回  -0.44807362
4、格式：cosh(number)   即   cosh(数值）
返回输入值的反余弦值。
例：   cosh(0)   返回  1 || cosh(90*3.14/180)   返回  2.507
#### 四、关于oracle中in和exists的区别:
一般来说，这两个是用来做两张（或更多）表联合查询用的，in是把外表和内表作hash 连接，而exists 是对外表作loop 循环，假设有A、B两个表，使用时是这样的：
1、select * from A where id in (select id from B)--使用in
2、select * from A where exists(select B.id from B where B.id=A.id)--使用exists
也可以完全不使用in和exists：
3、select A.* from A,B where A.id=B.id--不使用in和exists
具体使用时到底选择哪一个，主要考虑查询效率问题：
第一条语句使用了**A表的索引**；
第二条语句使用了**B表的索引**；
第三条语句同时使用了**A表、B表的索引**；
如果A、B表的数据量不大，那么这三个语句执行效率几乎无差别；
**如果A表大，B表小，显然第一条语句效率更高，反之，则第二条语句效率更高；**
第三条语句尽管同时使用了A表、B表的索引，单扫描次数是笛卡尔乘积，效率最差。
**五、SQL优化：**
1、先执行From ->Where ->Group By->Order By
2、执行From 字句是从右往左进行执行。因此必须选择记录条数最少的表放在右边。这是为什么呢？
3、对于Where字句其执行顺序是从后向前执行、因此可以过滤最大数量记录的条件必须写在Where子句的末尾，而对于多表之间的连接，则写在之前。
因为这样进行连接时，可以去掉大多不重复的项。
4. SELECT子句中避免使用(*)ORACLE在解析的过程中, 会将’*’ 依次转换成所有的列名, 这个工作是通过查询数据字典完成的, 这意味着将耗费更多的时间
5、索引失效的情况:
① Not Null/Null 如果某列建立索引,当进行Select * from emp where depto is not null/is null。 则会是索引失效。
② 索引列上不要使用函数,SELECT Col FROM tbl WHERE substr(name ,1 ,3 ) = 'ABC'
或者SELECT Col FROM tbl WHERE name LIKE '%ABC%' 而SELECT Col FROM tbl WHERE name LIKE 'ABC%' 会使用索引。
③ 索引列上不能进行计算SELECT Col FROM tbl WHERE col / 10 > 10 则会使索引失效，应该改成
SELECT Col FROM tbl WHERE col > 10 * 10
④ 索引列上不要使用NOT （ != 、 <> ）如:SELECT Col FROM tbl WHERE col ! = 10
应该 改成：SELECT Col FROM tbl WHERE col > 10 OR col < 10 。
6、用UNION替换OR(适用于索引列)
union:是将两个查询的结果集进行追加在一起，它不会引起列的变化。 由于是追加操作，需要两个结果集的列数应该是相关的，
并且相应列的数据类型也应该相当的。union 返回两个结果集，同时将两个结果集重复的项进行消除。 如果不进行消除，用UNOIN ALL.
通常情况下, 用UNION替换WHERE子句中的OR将会起到较好的效果. 对索引列使用OR将造成全表扫描. 注意, 以上规则只针对多个索引列有效.
如果有column没有被索引, 查询效率可能会因为你没有选择OR而降低. 在下面的例子中, LOC_ID 和REGION上都建有索引.
高效:
SELECT LOC_ID , LOC_DESC , REGION
FROM LOCATION
WHERE LOC_ID = 10
UNION
SELECT LOC_ID , LOC_DESC , REGION
FROM LOCATION
WHERE REGION = “MELBOURNE”
低效:
SELECT LOC_ID , LOC_DESC , REGION
FROM LOCATION
WHERE LOC_ID = 10 OR REGION = “MELBOURNE”
如果你坚持要用OR, 那就需要返回记录最少的索引列写在最前面.
7. 用EXISTS替代IN、用NOT EXISTS替代NOT IN
在许多基于基础表的查询中, 为了满足一个条件, 往往需要对另一个表进行联接. 在这种情况下, 使用EXISTS(或NOT EXISTS)通常将提高查询的效率.
在子查询中, NOT IN子句将执行一个内部的排序和合并. 无论在哪种情况下, NOT IN都是最低效的(因为它对子查询中的表执行了一个全表遍历).
为了避免使用NOT IN, 我们可以把它改写成外连接(Outer Joins)或NOT EXISTS.
例子：
高效: SELECT * FROM EMP (基础表) WHERE EMPNO > 0 AND EXISTS (SELECT ‘X’ FROM DEPT WHERE DEPT.DEPTNO = EMP.DEPTNO AND LOC = ‘MELB’)
低效: SELECT * FROM EMP (基础表) WHERE EMPNO > 0 AND DEPTNO IN(SELECT DEPTNO FROM DEPT WHERE LOC = ‘MELB’)
**六、Rownum和row_number()over()的使用区别：**
在oracle中如何查询表中的top10条记录：select * from test where rownum<=10;
Rownum和row_number() over()的使用
ROWNUM是Oracle从8开始提供的一个伪列，是把SQL出来的结果进行编号，始终从1开始，常见的用途就是用来分页输出.
比如
SELECT *  FROM torderdetail a WHERE ROWNUM <= 10
这条语句就是输出前10条纪录，在这里用途上类似于sql sever的top，不过rownum对于指定编号区间的输出应该说更强大
SELECT *  FROM (SELECT a.*, ROWNUM rn   FROM torderdetail a) WHERE rn >= 10 AND rn <= 20
这条语句即是输出第10到第20条纪录，这里之所以用rownum rn，是把rownum转成实例，因为rownum本身只能用 <=的比较方式，只有转成实列，这样就可做 >=的比较了。
在实际用途中，常常会要求取最近的几条纪录，这就需要先对纪录进行排序后再取rownum <=
一般常见的
SELECT * FROM (SELECT  a.*  FROM torderdetail a   ORDER BY order_date DESC) WHERE ROWNUM <= 10
而在CSDN曾经发生过讨论，关于取近的10条纪录，有人给出这样的语句
SELECT  a.*  FROM torderdetail a  WHERE ROWNUM <= 10 ORDER BY order_date DESC
之所以会出现这样的语句，主要是从效率上的考虑，前面条语句，是要进行全表扫描后再排序，然后再取10条纪录，后一条语句则不会全表扫描，只会取出10条纪录，很明显后条语句的效率会高许多。
那为什么会有争议呢，那就在于在执行顺序上争议，是先执行排序取10条纪录，还是取10条纪录，再排序呢？两种顺序取出来的结果是截然相反的，先排序再取10条，就是取最近的10条，而先取10条，再排序，则取出的最早的10条纪录。对于此语句，普遍的认为执行顺序是先取10条纪录再排序的。所以此语句应该是错误。但实际上并非如此，此语句的执行顺序和order by的字段有关系，如果你order by 的字段是pk，则是先排序，再取10条（速度比第一种语句快），而排序字段不是PK 时，是先取10条再排序，此时结果就与要求不一样了，所以第二种写法一定要在排序字段是主键的情况下才能保证结果正确。
Row_number() over()这个分析函数是从9I开始提供的，一般的用途和rownum差不多。
一般写法row_number() over( order by order_date desc) 生成的顺序和rownum的语句一样，效率也一样（对于同样有order by 的rownum语句来说），所以在这种情况下两种用法是一样的。
而对于分组后取最近的10条纪录，则是rownum无法实现的，这时只有row_number可以实现，row_number() over(partition by 分组字段 order by 排序字段)就能实现分组后编号，比如说要取近一个月的每天最后10个订单纪录
SELECT *   FROM (SELECT a.*,
ROW_NUMBER () OVER (PARTITION BY TRUNC (order_date) ORDER BY order_date DESC) rn
FROM torderdetail a) WHERE rn <= 10
Rownum的另类用法，有时候我们会遇到这种需求，要求输出当月的所有天数，许多人会烦恼，数据库里又没有这样的表，怎么输出一个月的所有天数呢？用rownum就能解决：
SELECT    TRUNC (SYSDATE, 'MM') + ROWNUM - 1   FROM DUAL
CONNECT BY ROWNUM <= TO_NUMBER (TO_CHAR (LAST_DAY (SYSDATE), 'dd'))





