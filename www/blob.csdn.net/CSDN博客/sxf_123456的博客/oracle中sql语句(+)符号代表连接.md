# oracle中sql语句(+)符号代表连接 - sxf_123456的博客 - CSDN博客
2017年12月06日 17:10:03[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：316
                
oracle中sql语句(+)符号代表连接
(+)在=前边为右连接
(+)在=后边为左连接
SELECT a.*, b.* from a(+) = b就是一个右连接，等同于select a.*, b.* from a right join b
SELECT a.*, b.* from a = b(+)就是一个左连接，等同于select a.*, b.* from a left join b
内连接
常用的连接运算符=、<、>
使用比较运算符根据每个表共有的列的值匹配两个表中的行
外连接
左连接
LEFT JOIN 或LEFT OUTER JOIN
列出左表中的所有行，若左表中某行在右表中没有匹配的行，则左表该行输出，右表该行为空
右连接
RIGHT JOIN 或RIGHT OUTER JOIN
列出右表中的所有行，如右表中某行在左表中没有匹配的行，则右表该行输出，左表该行为空
全连接
FULL JOIN 或FULL OUTER JOIN
对两表中的所有数据
当左表中的某行在右中没有匹配的行，左表该行输出，右表该行为空
当右表中的某行在左表中没有匹配的行，右表该行输出，左表该行为空
例子：   
-------------------------------------------------
  a表     id   name     b表     id   job   parent_id   
              1   张3           1     23     1   
              2   李四          2     34     2   
              3   王武          3     34     4       
  a.id同parent_id   存在关系   
--------------------------------------------------  
内连接
select a.*,b.* from a inner join  b on a.id=b.parent_id
 1   张3           1     23     1   
 2   李四          2     34     2
左连接
select a.*,b.* from a left join b on a.id=b.parent_id
 1   张3           1     23     1   
 2   李四          2     34     2
 3   王武          null     
右连接
select a.*,b.* from a right join b on a.id=b.parent_id
 1   张3           1     23     1   
 2   李四          2     34     2
 null              3     34     4  
全连接
select a.*,b.* from a full join b on a.id=b.parent_id
 1   张3           1     23     1   
 2   李四          2     34     2
 null              3     34     4 
 3   王武          null
