# MySQL操作符



```
简要介绍MySQL操作符
　　常用：
　　　　算术运算符、比较操作符、逻辑操作符、位运算符……
```

------

```
一、算术运算符
　　+：加
　　-：减
　　*：乘
　　/：除，返回商
　　%，mod()：除，返回余数
mysql> select  3%2,mod(3,2);
+------+----------+
| 3%2  | mod(3,2) |
+------+----------+
|    1 |        1 |
+------+----------+
注意：除法运算中，若除数为0(无意义)，是非法除数，返回结果为null。
二、比较操作符
　　---利用比较操作符在DQL语句中实现按条件的检索

示例用表：球员信息表player、比赛信息表matches
1、获得联盟会员号码为7060的球员信息
mysql> select * from player where leagueno=’7060’;
 
2、获得没有联盟会员号码的球员信息
mysql> select * from player where leagueno=null;
error……报错
mysql> select * from player where leagueno <=> null;
或
mysql> select * from player where leagueno is null;
“<=>”安全的等于运算符：和“=”类似，在操作数相等时值为 1，不同之处在于即使
操作的值为 NULL 也可以正确比较。
 
3、找出获胜局数等于2且输掉局数等于3的比赛信息
条件中可以进行列值的成对比较
mysql> select * from matches where (won,lost)=(2,3);
注意：条件(2,4)>(1,3)并不等于(2>1) and (4>3)，而是等于(2>1) or (2=1 and 4>3)

说明：使用不同运算符的成对比较在转换为标量表达式比较时，都有各自的转换规则。
 
4、between的范围比较：使用BETWEEN …AND… 操作符可以测试列值是否在一个取值范围之内 
例：找出1962—1964年之间出生的球员的编号、出生日期
mysql> select playerno,birth_date from players where birth_date between '1962-01-01' and '1964-01-01';
 
5、in操作符：测试一个列值是否在一个值列表中(值列表中可以是字面量、列或标量子查询) 
例1：得到那些出生在1962、1963或1970的球员的编号、出生年份
mysql> select playerno,year(birth_date) from players where year(birth_date) in (1962,1963,1970);
例2：两局获胜或者两局输掉的所有比赛
mysql> select * from matches where 2 in (won,lost);
例3：比分为3比1和3比2的所有比赛（in的成对比较）
mysql> select * from matches where (won,lost) in ((3,1),(3,2));
说明：在MySQL内部，in转换成or操作，not in转换成and操作
 
6、 like操作符：实现模式匹配查询或者模糊查询
like “匹配模式字符串”测试一个列值是否匹配给出的模式
特殊含义的通配字符：
　　%：表示0个或者任意多个字符
　　_：只表示一个任意字符
例1：找出名字以大写字母B开头的球员
mysql> select * from players where name like ‘B%’;
例2：得到名字以小写字母r结尾的球员
mysql> select * from players where name like ‘%r’;
例3：得到名字的第二个字母为e的球员的名字和编号
mysql> select name,playerno from players where name like ‘_e%’;
例4：找出名字中包含一个下划线的球员
mysql> select playerno,name from players where name like ‘%/_%’ escape ‘/’;
注意：如果转义字符指定反斜杠\，必须是escape ‘\\’(反斜杠\在进行模式匹配时，会被剥离一次)
例如：
mysql> select * from table_name where col_name like ‘%\\\%’ escape ‘\\’; 
##反斜杠会被语法分析程序剥离一次，在进行模式匹配时，又会被剥离一次，最后会剩下一个反斜杠接受匹配.
7、regexp操作符(regular expression)
　　--比like功能更加强大---正则
例1：得到名字以ba开头的球员信息
mysql> select * from players where name regexp '^ba';

例2：得到名字包含了字母a、b或c的球员信息
mysql> select * from players where name regexp '[abc]';

例3：对于名字中有字母m、e、n中的一个，并且连续出现2次的球员信息
mysql> select * from players where name regexp '[men][men]';

三、逻辑操作符
　　---逻辑运算符又称为布尔运算符，用来确认表达式的真和假
　　如果在where中同时用到了and 和or运算符，先算and后算or，因为and优先级高；
　　可以使用小圆括号来改变运算的顺序。
例1： 得到那些住在Stratford或者出生于1963年的球员的编号、姓名、出生日期，但是不包括那些住在Stratford并且出生于1963年的球员
mysql> select playerno,name,birth_date from players where (town='Stratford' or year(birth_date)='1963') and not (town='Stratford' or year(birth_date)='1963');
 
例2：得到那些住在Stratford或者出生于1963年的球员的编号、姓名、出生日期，但是不包括那些住在Stratford并且出生于1963年的球员
mysql> select playerno,name,birth_date from players where (town='Stratford') xor (year(birth_date)='1963');

四、位运算符
    ---位运算是将给定的操作数转化为二进制后， 对各个操作数每一位都进行指定的逻辑运算，得到的二进制结果转换为十进制数后就是位运算的结果
mysql> SELECT 29 & 15;
+---------+
| 29 & 15 |
+---------+
|      13 |
+---------+
1 row in set (0.01 sec)

mysql> SELECT 1 << 2;
+--------+
| 1 << 2 |
+--------+
|      4 |
+--------+
1 row in set (0.00 sec)
位运算符其实也就是位函数，通过mysql> ? Bit Functions了解使用，用得不多，但是要有个印象，要用的时候查看帮助
```

@author：http://www.cnblogs.com/geaozhang/