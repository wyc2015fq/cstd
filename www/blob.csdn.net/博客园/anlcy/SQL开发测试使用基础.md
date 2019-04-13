
# SQL开发测试使用基础 - anlcy - 博客园






# [SQL开发测试使用基础](https://www.cnblogs.com/camilla/p/7201467.html)

## 目录
一、客户端配置与使用
1、oracle（PLSQL Developer）
2、hive（hive cli）及命令
3、postgre（pgAdmin）
二、注意事项及基础
1、索引使用
2、dblink
3、Driving Table(驱动表)
4、查看解释计划
5、正则表达式
三、基础概念及函数
1、字符处理
2、数字处理
3、日期处理
4、case表达式
5、null值
6、In与Exists
7、开窗函数
8、decode函数
9、distinct
10、dual
四、常规用法
1、建表删表
2、增删改
3、select语句
4、over子句
5、联接查询
6、集合运算
7、游标
8、存储过程基本结构
9、时间切片
10、循环/定时调用
五、跨库导数
六、查询性能优化
七、相关链接

## 一、客户端配置与使用
### 1、oracle（PLSQL Developer）
1）TNS文件路径。TNS文件用于设置连接标识符、监听路径等。 查找路径如下：【帮助】-【支持信息】-【信息】。找到“TNS FIle”
![](http://wiki.gf.com.cn/download/attachments/45923351/image2017-7-18%2015%3A19%3A28.png?version=1&modificationDate=1500362539048&api=v2)
2）TNS信息
![](http://wiki.gf.com.cn/download/attachments/45923351/image2017-7-18%2015%3A19%3A37.png?version=1&modificationDate=1500362539062&api=v2)
3）终止会话。有时查询数据量大，程序会一直运行，此时想结束查询，可以终止会话。操作路径如下：【工具】-【会话】-【my sessions】-【Status】。找到Active状态会话，右键“关掉”。
![](http://wiki.gf.com.cn/download/attachments/45923351/image2017-7-18%2015%3A19%3A48.png?version=1&modificationDate=1500362539074&api=v2)
### 2、hive（hive cli）及命令
1）web访问http://10.2.85.84:8090/auth并登陆。
2）登陆后输入hive命令进入hive cli界面
![](http://wiki.gf.com.cn/download/attachments/45923351/image2017-7-18%2015%3A19%3A59.png?version=1&modificationDate=1500362539085&api=v2)
3）show databases 查看所有数据库
4）show tables    查看所有表
5）desc [tabname]   查看表信息
6）use [hegui_test]   进入hegui_test数据库
7）limit  [10]   查询语句后加  限制条数
### 3、postgre（pgAdmin） 略

## 二、注意事项及基础
### 1、索引使用
平时查询历史表等数据量大或者进行多表连接时，查询耗时较长。当前用户所在数据库提倡尽量使用索引。
1）查询表索引。（dblink查询的表由于权限问题暂时无法查看索引）
select * from user_ind_columns where table_name=upper('ENTRUST')   //ENTRUST是表名
### 2、dblink
1）由于业务原因，平时查询数据时涉及数据库较多，因此使用dblink技术打通各个库之间的数据。如平时用到的src_ucbs_entrust，其实是一个view。view通过dblink方式查询柜台数据库hsman.src_ucbs_entrust@hghs表。注意：跨数据库联接会引起大量io，从而导致查询速度慢。可在本地数据库建建临时表应对这种情况。
2）查询dblink信息
select * from all_db_links where db_link=upper('hghs')  //hghs是dblink
### 3、Driving Table(驱动表)
该表又称为外层表(OUTER TABLE)。这个概念用于嵌套与HASH连接中。如果该row source返回较多的行数据，则对所有的后续操作有负面影响。一般说来，是应用查询的限制条件后，返回较少行源的表作为驱动表，所以如果一个大表在WHERE条件有有限制条件(如等值限制)，则该大表作为驱动表也是合适的，所以并不是只有较小的表可以作为驱动表，正确说法应该为应用查询的限制条件后，返回较少行源的表作为驱动表。
### 4、查看解释计划。
plsql developer 编写完sql语句，按“F5” 。解释计划包含表对象，扫描方式，io数据量和cpu计算量。如果查询速度较慢，可以通过解释计划窗口分析原因，从而为调整查询语句提供参考信息。
![](http://wiki.gf.com.cn/download/attachments/45923351/image2017-7-18%2015%3A20%3A23.png?version=1&modificationDate=1500362539097&api=v2)
### 5、正则表达式
对字段提取部分特征信息时通常会用到正则表达式匹配。如从地址信息中提取mac或者电话号码。正则表达式相关知识请查阅：[http://wiki.gf.com.cn/pages/viewpage.action?pageId=45922377](http://wiki.gf.com.cn/pages/viewpage.action?pageId=45922377)

## 三、基础概念及函数
### 1、字符处理
#### 1）concat或|| 字符串串联运算
select  concat('hello','-world') from dual
select   'hello'||'-world' from dual
#### 2）substr(string,start_positon,[length])  字符串截取
解释 string：源字符串，start_position：截取开始位置，length：截取长度，length参数可选。
select substr('hello-world',5,4) from dual  //返回o-wo
#### 3）instr（string,subString,position,ocurrence）查找字符串位置
解释：string：源字符串  subString：要查找的子字符串 position：查找的开始位置 ocurrence：源字符串中第几次出现的子字符串
select instr('hello-world','o',3,2) from dual //返回8  即在字符串“hello-world”的第三个字符开始查找“o”字符，出现的第二次位置
#### 4）upper 根据当前字符集映射的所有字符更改为大写
select upper('hello-world') from dual  //返回HELLO-WORLD
#### 5）lower根据当前字符集映射的所有字符更改为小写
select upper('HELLO-WORLD') from dual  //返回hello-world
#### 6）length  字符串长度
select length('hello-world') from dual //返回11
#### 7）trim 删除字符串两边空格
select length('  hello-world  ') from dual //返回hello-world

#### 8）replace('string','s1','s2')
解释：string 希望被替换的字符或变量，s1 被替换的字符串，s2 要替换的字符串
select replace('hello-world','world','you') from dual //返回hello-you
#### 9）trunc按照指定的精度截取一个数
select trunc(123.1234,2) from dual
#### 10）to_char
select to_char(sysdate,'yyyy-mm-dd') from dual //返回当前系统日期 2017-07-17
#### 11）nvl(ex1,ex2)  ex1值为空则返回ex2,否则返回该值本身ex1
#### 12）regexp_like：与LIKE的功能相似
例：select * from src_ucbs_entrust where regexp_like(op_station,'^STORE.*') //查询地址信息以STORE开头的委托数据
#### 13）regexp_instr：与INSTR的功能相似
例：select * from src_ucbs_entrust where regexp_instr(op_station,'^STORE.*') >0  //查询地址信息以STORE开头的委托数据
#### 14）regexp_substr：与SUBSTR的功能相似
例：select op_station,regexp_substr(op_station,'[0-9]+') from src_ucbs_entrust // 截取地址信息第一个匹配的数字串
#### 15）regexp_replace：与REPLACE的功能相似
例：select op_station,regexp_replace(op_station,'^STORE.*','my entrust') from src_ucbs_entrust //将地址信息以STORE开头的地址信息替换成my entrust
### 2、数字处理
#### 1）abs 绝对值
select  abs(-123) from dual  //返回123
#### 2）to_number 将给出的字符转换为数字
select to_number('123') from dual  //返回123
#### 3）ceil向上取整
select ceil(123.4) from dual //返回124
#### 4）floor向下取整
select floor(123.4) from dual //返回123
#### 5）round四舍五入
select round(123.4) from dual //返回123
### 3、日期处理
#### 1）to_date(string,'format')  将字符串转化为ORACLE中的一个日期
select to_date('2017-01-01','yyyy-mm-dd') from dual
#### 2）sysdate 表示当前系统时间
select sysdate from dual
#### 3）前一天 sysdate-1
select sysdate-1 from dual //当前系统时间是2017-7-17 15:20:55  返回2017-7-16 15:20:55

#### 4）获取某年第一天，某月第一天
select trunc(sysdate,'year') from dual //返回2017-01-01
select trunc(sysdate,'month') from dual //返回2017-07-01
select trunc(sysdate,'day') from dual //返回2017-07-16
### 4、case表达式
case表达式包含简单表达式和搜索表达式。如果对字段内容进行替换可使用简单表达式。对有某一个特征数据汇总统计可使用搜索表达式。
#### 1）简单表达式
select case exchange_type when '1'  then '上海'when '2' then '深圳' end  from src_ucbs_entrust;
#### 2）搜索表达式
select client_id,sum(case when entrust_type<>'2' then 1 end )as  entrust_time from src_ucbs_entrust group by client_id
### 5、null值
null符号表示缺省值，它使用的是三值谓词逻辑。这意味着计算结果可以是true false unknown。
因此如果查询字段有可能是null，并且想排除null的情况，条件需加上  is not null
### 6、In与Exists
In是把外表和内表做Hash 连接，而exists 是对外表作loop 循环，每次loop循环再对内表进行查询。
当查询两个表的大小相当时，用In 和 exists差别不大。
如果两个表中一个表较小，一个表较大，那么子查询表大的用exists,子查询表小的用In，效率会高的。
也就是说 IN适合于外表大而内表小的情况；EXISTS适合于外表小而内表大的情况，这样效率会高的
### 7、开窗函数over
#### 1）按资金账号、股票代码、买卖方向汇总非废单委托金额
sum(case when entrust_type<>'2' then entrust_amount*entrust_price end) over(partition by fund_account,stock_code,entrust_bs)
#### 2）按定位串字段和时间进行分组排序
row_number() over(partition by position_str order by position_str, bb.RT_TIME desc)  as row_id
#### 3）按客户号、股东账号、股票代码、成交编号取最大的交易时间
max(business_time)over(partition by client_id,stock_account,stock_code,business_no) as business_time
#### 4）按行情批次时间和股票代码取最小的行情价格
min(rtb.RT_PRICE)over(partition by rta.exchange_time,sec_code) as min_price
#### 5）按股东账号和股票代码汇总数量
count(1) over(partition by stock_account_a,stock_account_b,stock_code) as bus_amount
### 8、decode函数
decode(条件,值1,翻译值1,值2,翻译值2,...值n,翻译值n,缺省值)的理解如下：
if （条件==值1）
then
return(翻译值1)
elsif （条件==值2）
then
return(翻译值2)
......
elsif （条件==值n）
then
return(翻译值n)
else
return(缺省值)
end if
例子：decode(sum(total_amount),0,0, sum(total_bal)/sum(total_amount) )
总金额sum(total_amount)=0时返回0，否则返回sum(total_bal)/sum(total_amount)
### 8、distinct  用于保证返回结果集去除重复记录
### 9、dual
1）用于计算  select 2-1 from dual
2）获取系统时间  select sysdate from dual
以上是oracle相关用法，hive和postgre用户用法和函数会有细微差异，如发现未能得到预期结果，请确认确认原因并做出相应调整。
---

## 四、常规用法
### 1、建表删表
1）基本建表create table
create table person(id number,first_name varchar2(25),last_name varchar2(25))
2）基于查询结果建表
create table my_entrust as select * from src_ucbs_entrust where 1=0;
3）删表
drop table person
### 2、增删改
1）新增
insert into my_entrust select * from src_ucbs_entrust where client_id='****';  //基于查询结果新增数据
insert into person(id,first_name,last_name) values(1,'kat','dennings')
2）删除。truncate和delete只删除数据不删除表结构定义。drop语句将删除表数据及结构。速度一般来说drop>truncate>delete
delete from person [where id=1] //where条件可选
truncate table person
drop table person
3）修改
update person set last_name='yyy' where id=1
### 3、select语句
select语句的目的是对表进行查询，应用一定的逻辑处理，并返回结果。查询过程涉及到多个处理阶段。以下是各个查询子句的逻辑处理顺序。
#### 1）from子句
from子句是在逻辑处理阶段第一个要处理的查询子句。它用于指定要查询的表名，以及对这些表进行操作的表运算符
#### 2）where子句
where子句可以指定一个谓语或逻辑表达式，从而过滤由from阶段返回的行。
where子句对性能有重要影响。与扫描整个表相比，通过使用索引，有时可以大幅减少获取请求的数据时付出的代价（io cpu等）
此阶段要时刻注意是否包括null值
#### 3）group by 子句
group by阶段可以将前面逻辑查询处理阶段返回的行按“组”进行组合。每个组由在group by子句中指定的各元素决定。
聚合函数只为每个组返回一个值。所以一个元素如果不在group by列表中出现，只能作为聚合函数（count、max、min、sum、avg）的输入。
所有聚合函数都会忽略null值，只有一个例外。count（*）
可以在聚合函数括号里使用dintinct排除掉重复值。
#### 4）having 子句
having子句用于指定对组进行过滤的谓语或者逻辑表达式。这与where阶段对单独的行进行过滤相对应。逻辑表达式计算结果是false或unknown的组将被过滤掉。
#### 5）select子句
select子句用于指定需要在查询返回的结果集中包含的属性（列）。可以使用as子句更改返回列名
#### 6）order by 子句
order by 子句用于展示数据时对输出结果中的行进行排序。如果不使用order by子句，查询结果是不保证有序的。
带有order by子句的查询会返回游标结果，从而保证数据是有序的。
asc关键字是升序，desc关键字是降序
### 4、over子句
over子句用于为行定义一个窗口，以便进行特定的计算。聚合函数（count、sum、avg、max、min）和排名函数（row_number、rank、dense_rank）都是可以支持over子句运算类型。
聚合函数传统上一直以group by查询作为操作的上下文。聚合开窗函数使用over子句提供窗口作为上下文，对窗口中的一组值进行操作。
注意：只有在select和order by 处理阶段才允许使用over子句。
如果想对行进行限制或者分区，则可以使用partition by子句。
如果row_number函数的order by 不能唯一确定行的顺序，查询结果就是不确定的。
### 5、联接查询
联接有三种类型：交叉联接、内联接、外联接。
交叉联接只有一个步骤：笛卡尔积
内联接有两个步骤：笛卡尔积、过滤
外联接有三个步骤：笛卡尔积、过滤、添加外部行
1）交叉联接
例：select aa.*,bb.* from src_ucbs_entrust aa,tctm_sec_quotation_rt bb
2）内联接
例：select aa.*,bb.* from src_ucbs_entrust aa inner join tctm_sec_quotation_rt bb  on aa.stock_code=bb.sec_code
3）外联接
例：select aa.*,bb.* from src_ucbs_entrust aa left join tctm_sec_quotation_rt bb  on aa.stock_code=bb.sec_code
### 6、集合运算
1）union
可以将两个输入查询的结果集组合成一个结果集。union会去掉重复记录。
例：查询普通委托和信用委托20170717日数据
select * from src_ucbs_his_entrust where init_date=20170717
union
select * from src_ucbs_his_crdtentrust where init_date=20170717
2）union all
功能和union类似，union all不会去掉重复记录。
3）intersect（交集）
对两个输入查询的结果集取其交集，只返回在两个查询结果集都出现的行。
4）except（差集）
对两个输入查询的结果集进行操作，返回出现抵押给结果集中，但不出现在第二个结果集中多有行。
例如：现在要找出两张表有差异的数据，需要用minus及uni[on](https://www.baidu.com/s?wd=on&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1YdujfkrjDkn1wWrAwWrHRs0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EPjbYPHTzP1fs)的方式查找出来，语句如下：
```python
select
```
```python
t1.*
```
```python
from
```
```python
(
```
```python
select
```
```python
*
```
```python
from
```
```python
test
```
```python
minus
```
```python
select
```
```python
*
```
```python
from
```
```python
test1) t1
```

```python
union
```

```python
select
```
```python
t2.*
```
```python
from
```
```python
(
```
```python
select
```
```python
*
```
```python
from
```
```python
test1
```
```python
minus
```
```python
select
```
```python
*
```
```python
from
```
```python
test) t2;
```

### 7、游标
游标是SQL的一个内存工作区，由系统或用户以变量的形式定义。游标的作用就是用于临时存储从数据库中提取的数据块。在某些情况下，需要把数据从存放在磁盘的表中调到计算机内存中进行处理，最后将处理结果显示出来或最终写回数据库。这样数据处理的速度才会提高，否则频繁的磁盘数据交换会降低效率。
以下列举游标的其中一种用法：包括定义和使用游标：
#### 1）定义游标
cursor real_cur is    select *    from zzh_9213_01 aa  where exchange_type = 2   and exchange_time >= p_start_time   and exchange_time <= p_end_time ;
#### 2）使用游标
for   cur in real_cur loop
insert into zzh_7105_report_new (init_date,begin_time,end_time, client_id, stock_account, stock_code, entrust_bs, total_bal,total_amount)
select tt.init_date,
cur.exchange_time-300 as begin_time,
cur.exchange_time as end_time,
tt.client_id,
tt.stock_account ,
tt.stock_code,
tt.entrust_bs,
sum(tt.business_balance) as total_bal ,
sum(tt.business_amount) as total_amount
from hsman.zzh_7105_realtime tt
where tt.BUSINESS_TIME >= cur.exchange_time-300
and tt.BUSINESS_TIME <= cur.exchange_time
group by tt.init_date,
tt.client_id,
tt.stock_account,
tt.stock_code,
tt.entrust_bs
having sum(tt.business_balance) >= 1000000;
end loop ;
### 8、存储过程基本结构
存储过程包含头部声明，变量声明（包括游标声明）、处理逻辑（包括异常捕获）、结束声明。
以下例子是一个完整的存储过程，逻辑是以三分钟为窗口进行时间切片，计算三分钟内交易金额大于100万的成交汇总数据。
//存储过程头部声明
create or replace function zzh_sp_exec_7105_new(v_start_time varchar,v_end_time varchar,v_date varchar,p_error_info out nvarchar2)
return number is
//变量定义
p_start_time  varchar(10) :=nvl(v_start_time,'93300');
p_end_time varchar(10) :=nvl(v_end_time,'150000');
p_today varchar(10):=nvl(v_date,to_char(sysdate,'yyyymmdd'));
//游标声明
cursor real_cur is   select *   from zzh_9213_01 aa    where exchange_type = 2    and exchange_time >= p_start_time   and exchange_time <= p_end_time ;
//存储过程处理开始
begin
//删除数据
execute immediate 'truncate table zzh_7105_realtime ';
execute immediate 'truncate table zzh_7105_report_new';
begin
//遍历游标
for   cur in real_cur loop
insert into zzh_7105_report_new (init_date,begin_time,end_time, client_id, stock_account, stock_code, entrust_bs, total_bal,total_amount)
select tt.init_date,
cur.exchange_time-300 as begin_time,
cur.exchange_time as end_time,
tt.client_id,
tt.stock_account ,
tt.stock_code,
tt.entrust_bs,
sum(tt.business_balance) as total_bal ,
sum(tt.business_amount) as total_amount
from hsman.zzh_7105_realtime tt
where tt.BUSINESS_TIME >= cur.exchange_time-300
and tt.BUSINESS_TIME <= cur.exchange_time
group by tt.init_date,
tt.client_id,
tt.stock_account,
tt.stock_code,
tt.entrust_bs
having sum(tt.business_balance) >= 1000000;
end loop ;
//异常捕获
commit;
exception
when others then
rollback;
p_error_info :=sqlerrm;
return 2;
end;
return 0;
end;//存储过程结束
### 9、时间切片
实时指标测试通常会涉及到时间切片进行数据统计。现测试方法是通过对日终（全量数据）进行切片，依据逻辑循环计算各时间切片是否符合条件。得到的计算结果与实时指标运行结果比对。以下是建立时间表和时间切片计算用法两个例子说明时间切片使用。
#### 1）建立切片基础表（时间表 )
create or replace function zzh_sp_exec_tradetime(p_error_info out nvarchar2)
return number is
v_start_time_str varchar2(30):='2017-01-01 090000';
v_end_time_str varchar2(30):='2017-01-01 151500';
v_start_time date;
v_end_time date;
v_time date;
begin
select to_date(v_start_time_str,'yyyy-mm-dd hh24miss') into v_start_time from dual;
select to_date(v_end_time_str,'yyyy-mm-dd hh24miss') into v_end_time from dual;
select to_date(v_start_time_str,'yyyy-mm-dd hh24miss') into v_time from dual;
execute immediate 'truncate  table zzh_date';
commit;
while  v_time<= v_end_time loop
--create table zzh_date (trade_time number)
insert into zzh_date(trade_time) values(to_number(to_char(v_time,'hh24miss')));
select v_time+1/(24*60) into v_time from dual;
commit;
end loop ;
return 9999;
end;

#### 2）切片函数结构
参考“存储过程基本结构 ”，该用例是以三分钟时间为窗口，进行滚动。即91500、91600、91700为一个窗口进行统计成交金额，91600、91700、91800为一个窗口进行统计金额。
### 10、循环/定时调用（行情存储）
由于oracle数据库行情表数据已不再更新数据。因此涉及到行情连接查询需要自己保存，即src_ucbs_price 每15（30）秒更新一次数据，隔一段时间保存src_ucbs_price数据到临时表。这样还会出现一个精度问题。需要评估15（30）秒行情是否满足测试需求。
脚本如下：
## 五、跨库导数
在数据比对过程中通常会涉及到结果在两个不同类型数据库。比如postgre数据库数据和oracle数据库数据进行比对。比对方式可以用excel或者将数据同步到同一个数据库。使用ETL工具可在postgre和oracle数据库之间进行数据转换。由于操作便利性和比对数据量不大，我们通常使用postgre数据导出+plsql文本导入器进行数据转换。
以下以隔日反向为例子。程序运行结果推送到postgre数据库，测试过程计算结果基于oracle数据源计算后存储在oracle数据库。比对方法是将postgre数据库结果导入到oracle数据库。
### 1、postgre数据库导数到oracle数据库
#### 1）数据导出（postgre）
在pg admin客户端执行查询程序运行结果sql语句。
select position_str,by2 as init_date,account_no as client_id,stock_code, case when ENTRUST_bs='买入' then 1 else 2 end as entrust_bs, business_price_buy as avg_price_b,
business_price_sell as avg_price_s,   business_amount_buy as total_amount_b , business_amount_sell as total_amount_s ,business_balance_buy as total_bal_b ,
business_balance_sell  as total_bal_s ,  case when by4='隔日' then '隔日反向' else '日内反向' end  as report_type
from t_tgf_dtl_7204 where warning_date='20170301'
#### 2）将查询结果复制到excel表格，并从excel表格复制到txt文本中。（plsql文本导入器不能识别xls、xlsx格式）
![](http://wiki.gf.com.cn/download/attachments/45923351/image2017-7-18%2015%3A20%3A50.png?version=1&modificationDate=1500362539111&api=v2)
![](http://wiki.gf.com.cn/download/attachments/45923351/image2017-7-18%2015%3A21%3A3.png?version=1&modificationDate=1500362539156&api=v2)
#### 3）建临时数据表（oracle）
基于测试计算结果表建立临时表。sql语句如下：
create table  zzh_7204_result_hz_pg as  select INIT_DATE,CLIENT_ID,stock_account,STOCK_CODE, AVG_PRICE_B,AVG_PRICE_S,TOTAL_AMOUNT_B,TOTAL_AMOUNT_S,TOTAL_BAL_B,TOTAL_BAL_S,REPORT_TYPEfrom zzh_7204_result_new where 1=0
#### 4）数据导入（oracle）
在plsql客户端，依次打开【工具】-【文本导入器...】-【打开】-【选择txt文件】
![](http://wiki.gf.com.cn/download/attachments/45923351/image2017-7-18%2015%3A21%3A14.png?version=1&modificationDate=1500362539127&api=v2)

切换【到oracle的数据 】标签栏，输入所有者和表名，客户端自动匹配导入的字段名，核对没有错误，点击【导入】按钮，完成导入数据。
![](http://wiki.gf.com.cn/download/attachments/45923351/image2017-7-18%2015%3A21%3A29.png?version=1&modificationDate=1500362539142&api=v2)
## 六、查询性能优化
1、由浅入深理解索引的实现[http://blog.csdn.net/idber/article/details/8109522](http://blog.csdn.net/idber/article/details/8109522)
2、Oracle性能优化-读懂执行计划[http://blog.csdn.net/lifetragedy/article/details/51320192](http://blog.csdn.net/lifetragedy/article/details/51320192)

## 七、相关链接
1、oracle官网：[http://docs.oracle.com/en/database/](http://docs.oracle.com/en/database/)
2、oracle官网书籍：[http://docs.oracle.com/database/122/nav/portal_booklist.htm](http://docs.oracle.com/database/122/nav/portal_booklist.htm)
3、postgre官网：[https://www.postgresql.org/](https://www.postgresql.org/)
4、postgre9.6文档[https://www.postgresql.org/docs/9.6/static/index.html](https://www.postgresql.org/docs/9.6/static/index.html)
5、hvie文档：[https://cwiki.apache.org/confluence/display/Hive/LanguageManual](https://cwiki.apache.org/confluence/display/Hive/LanguageManual)
6、易佰hive教程[http://www.yiibai.com/hive/](http://www.yiibai.com/hive/)（英文版本：[https://www.tutorialspoint.com/hive/hive_introduction.htm](https://www.tutorialspoint.com/hive/hive_introduction.htm)）
7、oracle 正则表达式匹配[http://www.cnblogs.com/qmfsun/p/4467904.html](http://www.cnblogs.com/qmfsun/p/4467904.html)





