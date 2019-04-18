# 在Oracle中使用rank()over()排名的问题 - z69183787的专栏 - CSDN博客
2014年04月15日 12:24:24[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1014
排序：
---rank()over(order by 列名 排序)的结果是不连续的，如果有4个人，其中有3个是并列第1名，那么最后的排序结果结果如：1 1 1 4
select scoreid, studentid,COURSENAME,totalexamscore ,
rank()over(order by TOTALEXAMSCORE desc)orderbyNum
from SCORECOURSE a ,COURSESCORE b
where a.SCORECOURSEID = b.SCORECOURSEID
---dense_rank()over(order by 列名 排序)的结果是连续的，如果有4个人，其中有3个是并列第1名， 那么最后的排序结果如：1 1 1 2
select scoreid, studentid,COURSENAME,totalexamscore ,
dense_rank()over(order by TOTALEXAMSCORE desc)orderbyNum
from SCORECOURSE a ,COURSESCORE b
where a.SCORECOURSEID = b.SCORECOURSEID
----rank () OVER (PARTITION BY 列名 ORDER BY 列名 排序)使用分区方式获取每门课程的最高分
SELECT *
  FROM (SELECT scoreid, studentid, coursename, TOTALEXAMSCORE,
               rank () OVER (PARTITION BY coursename ORDER BY TOTALEXAMSCORE DESC)orderbynum
          FROM scorecourse a, coursescore b
         WHERE a.scorecourseid = b.scorecourseid and studentID = 'xxxxx')
 WHERE orderbynum < 2
-----使用over实现成绩求和
-----SUM (totalexamscore) OVER (ORDER BY studentid) sum1  实现的是连续求和，如第一个学生的总评成绩是30，则sum1就展示为30，到第二个学生成绩出现的时候，则会依次累加
-----SUM (totalexamscore) OVER () sum2 就相当于是单纯的求和，和直接使用sum是一致的
SELECT scoreid, studentid, totalexamscore,
       SUM (totalexamscore) OVER (ORDER BY studentid) sum1,
       SUM (totalexamscore) OVER () sum2
  FROM coursescore
注：这个案例想了很久也没想到很直观的描述，还请大家亲自去测试一下吧！！
语法：
rank() over (order by 排序字段 顺序)
rank() over (partition by 分组字段 order by 排序字段 顺序)
1.顺序：asc|desc  名次与业务相关：
  示例：求优秀学员，成绩：降序  迟到次数：升序
2.分区字段：根据什么字段进行分区。
问题：分区与分组有什么区别?
•分区只是将原始数据进行名次排列(记录数不变),
•分组是对原始数据进行聚合统计(记录数变少,每组返回一条)。
注意：使用rank()over(order by 排序字段 顺序)排序的时候，空值是最大的
(如果排序字段为null，可能造成在排序时将null字段排在最前面，影响排序的正确性。
所以建议将dense_rank()over(order by 列名 排序)改为dense_rank()over(order by 列名 排序 nulls last)
这样只要排序字段为null,就会放在最后，而不会影响排序结果).
