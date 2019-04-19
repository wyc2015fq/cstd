# SQL SERVER中查找某个字段位于哪些表 - 左直拳的马桶_日用桶 - CSDN博客
2010年11月26日 18:44:00[左直拳](https://me.csdn.net/leftfist)阅读数：2636标签：[sql server																[join																[table](https://so.csdn.net/so/search/s.do?q=table&t=blog)](https://so.csdn.net/so/search/s.do?q=join&t=blog)](https://so.csdn.net/so/search/s.do?q=sql server&t=blog)
个人分类：[sql server](https://blog.csdn.net/leftfist/article/category/136470)
select tab.name table_name, col.name column_name
  from sysobjects tab
  left join syscolumns col on tab.id = col.id and tab.xtype = 'U'
 where col.name like '%字段名%'
 order by 1,2
