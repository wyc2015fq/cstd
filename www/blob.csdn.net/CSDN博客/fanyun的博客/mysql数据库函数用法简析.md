# mysql数据库函数用法简析 - fanyun的博客 - CSDN博客
2016年03月23日 20:04:21[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：623
所属专栏：[数据库](https://blog.csdn.net/column/details/database-01.html)
**MySQL数据库提供了很多函数包括：数学函数；字符串函数；日期和时间函数；条件判断函数；系统信息函数；加密函数；格式化函数；**
**以下是MySQL数据库函数用法简析：**
语法：
**新建：**
Create function function_name(参数列表)returns返回值类型
函数体
函数名，应该合法的标识符，并且不应该与已有的关键字冲突。
一个函数应该属于某个[数据库](http://www.2cto.com/database/)，可以使用db_name.funciton_name的形式执行当前函数所属数据库，否则为当前数据库。
参数部分，由参数名和参数类型组成。
返回值类类型
函数体由多条可用的[mysql](http://www.2cto.com/database/MySQL/)语句，流程控制，变量声明等语句构成。
多条语句应该使用begin end语句块包含。
注意，一定要有return返回值语句。
**删除：**
Dropfunction if existsfunction_name;
**查看：**
Show function status like ‘partten’
Show create functionfunction_name;
**修改：**
Alter functionfunction_name函数选项。
例子：
Hello world!
**IF语句**
IF *search_condition*THEN
*statement_list*
[ELSEIF *search_condition*THEN*statement_list*]
...
[ELSE *statement_list*]ENDIF;
**CASE语句**
CASE *case_value*
WHEN *when_value*THEN*statement_list*
[WHEN *when_value* THEN*statement_list*]
...
[ELSE *statement_list*]
END CASE; 
**循环：**
**While**
[*begin_label*:]WHILE*search_condition*DO
*statement_list*
END WHILE [*end_label*];
如果需要在循环内提前终止 while循环，则需要使用标签；标签需要成对出现。
**退出循环**
退出整个循环leave 相当于break
退出当前循环iterate 相当于 continue
通过退出的标签决定退出哪个循环。
变量声明：
语法：
DECLARE var_name[,...] type [DEFAULT value]
这个语句被用来声明局部变量。要给变量提供一个默认值，请包含一个DEFAULT子句。值可以被指定为一个表达式，不需要为一个常数。如果没有DEFAULT子句，初始值为NULL。
使用
语序使用 set 和 select into语句为变量赋值。
注意在函数内是可以使用全局变量（用户自定义的变量的）@XXX 全局变量不用声明 可以直接@XXX使用。
> 
例子：获取当前班级内，最大的学号。 
> 
参考学生表 
create table join_student( 
stu_id int not null auto_increment, 
stu_no char(10), 
class_id int not null, 
stu_name varchar(10), 
stu_info text, 
primary key (stu_id) 
); 
计算新增学号 
drop function if existssno;
 delimiter $$ #在包含有语句块时 可以更换语句结束符“；” 为“$$” 
create function sno(c_id int)returns char(10) 
begin 
declare last_no char(10); #声明一个局部变量 用来保存当前最大的学号， 如果没有就为null
declare class_name char(10); 
select stu_no from join_student where class_id=c_id order by stu_no desc limit 1 into last_no; 
if last_no is null then #如果为空代表当前班级没有学生 从1开始，获得班级名字 
return concat ((select c_name from join_class where id=c_id into class_name),'001'); #concat() 函数的作用是连接字符串。 
else 
return concat(left(last_no,7),lpad(right(last_no,3) + 1, 3, '0')); 
end if;
 #return @last_no; 
end 
$$
delimiter ; 
随机获得学生名字。 
drop function if exists sname; 
delimiter $$ 
create function sname() returns char(2) 
begin 
declare first_name char(16) default '赵钱孙李周吴郑王冯陈褚卫蒋沈韩杨'; 
declare last_name char(10) default '甲乙丙丁戊己庚辛壬癸'; 
declare full_name char(2); 
set full_name=concat(substring(first_name,floor(rand()*16+1), 1), substring(last_name,floor(rand()*10+1), 1)); 
return full_name; 
end 
$$ 
delimiter ;
========================================================================================
**mysql常用内置函数**
数值函数
Abs(X)，绝对值abs(-10.9) = 10
Format(X，D)，格式化千分位数值format(1234567.456, 2) =1,234,567.46
Ceil(X),向上取整ceil(10.1) = 11
Floor(X),向下取整floor (10.1) = 10
Round(X),四舍五入去整
Mod(M,N) M%N M MOD N 求余 10%3=1
Pi(),获得圆周率
Pow(M,N) M^N
Sqrt(X)，算术平方根
Rand(),随机数
TRUNCATE(X,D) 截取D位小数
时间日期函数
Now(),current_timestamp(); 当前日期时间
Current_date();当前日期
current_time();当前时间
Date(‘yyyy-mm-dd HH;ii:ss’);获取日期部分
Time(‘yyyy-mm-dd HH;ii:ss’);获取时间部分
Date_format(‘yyyy-mm-dd HH;ii:ss’,’%D %y %a %d %m %b %j');
Unix_timestamp();获得unix时间戳
From_unixtime();//从时间戳获得时间
字符串函数
LENGTH(string ) //string长度，字节
CHAR_LENGTH(string) //string的字符个数
SUBSTRING(str ,position [,length ]) //从str的position开始,取length个字符
REPLACE(str ,search_str ,replace_str) //在str中用replace_str替换search_str
INSTR(string ,substring ) //返回substring首次在string中出现的位置
CONCAT(string [,... ]) //连接字串
CHARSET(str) //返回字串字符集
LCASE(string ) //转换成小写
LEFT(string ,length ) //从string2中的左边起取length个字符
LOAD_FILE(file_name) //从文件读取内容
LOCATE(substring , string [,start_position ]) //同INSTR,但可指定开始位置
LPAD(string ,length ,pad ) //重复用pad加在string开头,直到字串长度为length
LTRIM(string ) //去除前端空格
REPEAT(string ,count ) //重复count次
RPAD(string ,length ,pad) //在str后用pad补充,直到长度为length
RTRIM(string ) //去除后端空格
STRCMP(string1 ,string2 ) //逐字符比较两字串大小
流程函数：
CASE WHEN [condition]THEN result[WHEN [condition]THEN result ...][ELSE result]END 多分支
IF(expr1,expr2,expr3) 双分支。
聚合函数
Count()
Sum();
Max();
Min();
Avg();
Group_concat()
其他常用函数
Md5();
Default();
