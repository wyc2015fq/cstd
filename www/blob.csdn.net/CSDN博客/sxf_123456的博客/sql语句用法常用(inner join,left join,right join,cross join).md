# sql语句用法常用(inner join,left join,right join,cross join) - sxf_123456的博客 - CSDN博客
2017年07月22日 09:45:46[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：159标签：[sql](https://so.csdn.net/so/search/s.do?q=sql&t=blog)
个人分类：[sql](https://blog.csdn.net/sxf_123456/article/category/7045491)
例子：   
  a表       id   name     b表     id     job   parent_id   
              1   张3                   1     23     1   
              2   李四                  2     34     2   
              3   王武                  3     34     4   
1、内连接(inner join)
select a.id,a.name,b.id,b.job,b.parent_id from a inner join b on a.id=b.parent_id;
  结果是     
  1   张3                   1     23     1   
  2   李四                  2     34     2   
2、外连接(left join 或 left outer join)
select a.id,a.name,b.id,b.job,b.parent_id from a left join b on a.id=b.parent_id;
  结果是     
  1   张3                    1     23     1   
  2   李四                  2     34     2   
  3   王武                  null   
3、右连接(right join或 right outer join)
select a.id,a.name,b.id,b.job,b.parent_id from a right join b on a.id=b.parent_id;
  结果是     
  1   张3                   1     23     1   
  2   李四                 2     34     2   
  null                       3     34     4 
4、全连接(full join)
select a.id,a.name,b.id,b.job,b.parent_id from a full join b on a.id=b.parent_id;
结果是     
  1   张3                   1     23     1   
  2   李四                 2     34     2   
  null                 3     34     4   
  3   王武                 null
5、交叉连接(cross join)
select * from a cross join b;
结果是
1   张3                   1     23     1  
1   张3                   2     34     2 
1   张3                   3     34     4 
2   李四                  1     23     1
2   李四                  2     34     2   
2   李四                  3     34     4 
3   王武                1     23     1
3   王武                  2     34     2   
3   王武                  3     34     4 
