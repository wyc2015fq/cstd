# mysql必知必会--学习笔记2 - zhuzhuxia183的博客 - CSDN博客





2018年07月30日 14:59:04[喵小姐的邻居小小朱](https://me.csdn.net/zhuzhuxia183)阅读数：39
个人分类：[数据库](https://blog.csdn.net/zhuzhuxia183/article/category/7879767)








- 
mysql的基本操作中，还包含对于数据的检索排序，之前使用select 

关键词直接得到的结果为不是一个有序的，如何使用命令进行相关操作，主要是使用order by;

> 
select column from xxx order by id DESC  #这个表示的降序 

  select column from xxx order by id ASC   #这个表示的是升序


当然mysql默认情况是直接是ASC，所以大家看到ASC一般比较少
- 
除了对于mysql的数据进行排序之外，我们还需要对于数据进行简单的筛选，这里便要使用到基本的where 关键字，常用的事例为：

> 
select id from ad where id <> 1;


常见的where子句操作符包含 =、<>、！=、<、<=、>、>= 、between and 等等

- 
注意有的时候进行空值检查，需要注意null一般无法做比较，所以一般无法返回如

> 
select * from test where name<>’zhuzhuxia2’;


！！这里我们需要注意，当name字段为null的记录并不会被筛选出来！！

- 
对于数据过滤，最简单的就是在where中需要加入相关的一些约束条件，当然为了使我们约束条件变复杂，我们可以多个子句之间利用操作符来进行一个复杂化，当然主要是有AND 和 OR,对于这两个本身的意思而言，和数学上的含义一致，当然我们要注意，这些操作符之间的优先级使 AND先执行，还有 

1、IN   后面加一个集合，功能可以等价于OR

> 
select id from test  where id in(1,2)


2、NOT这里表示的意思使取反

> 
select id from test where id not IN(1,2)


- 
使用通配符来进行数据的过滤，常见的操作符为LIKE操作符，为了构造一个搜索模式，我们需要两个部分，一个是自面值，一个部分是通配符，比较常见的通配符有

1、%   %表示任意字符出现任意次数 

 2、_    _表示只匹配单个字符，不能多也不能少 

 3、使用这些通配符的技巧：- 不能过度的使用通配符
- 尽量不要再开头使用
- 注意通配符的使用规则






