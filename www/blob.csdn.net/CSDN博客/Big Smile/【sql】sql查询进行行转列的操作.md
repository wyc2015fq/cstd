# 【sql】sql查询进行行转列的操作 - Big Smile - CSDN博客
2017年12月24日 22:26:54[王啸tr1912](https://me.csdn.net/tr1912)阅读数：9009
        最近遇到了一个很是动态化的查询需求，要求的列数是动态加载的，而不是固定的列数，并且列内容的由来都是通过动态联查得到的，所以很是棘手，需要用到行转列的这种sql查询方式。
# 一、行转列的简单用法理解
        行转列的用法有的时候体现在一个第三条件介入问题的关联，我们都知道，我们做查询的时候，实际上查询出来的是一张表，直观来看是一个二维的数据结构，可以包含两种关系在其中，分别对应两种条件关联产生，但是实际上我们做出的查询结果，在纵向上总是保持为是一个数据集的计数功能，也就是说我们的条件关系是建立在一个一维线性关联上面的，但是我们又可以让我们的表可以表达更多的东西是为什么呢？是因为我们在这个一维的关系上叠加了很多不同的，但是有关系（依赖）的字段关联，其中的每一项都或多或少和前面的字段是有关系才能出现在后面的，但是什么时候用到行转列呢？其中一种情况就是我们所要展示的列的数据，是以一种交叉关系存在于某一个表的一列中的复数项利用的关系的字段，这个时候我们才会用到行转列这个思想。
        举一个最最简单的例子，就是学生，科目，分数这个例子：
学生表：
|学号|姓名|性别|年龄|
|----|----|----|----|
|01|小明|男|12|
|02|小芳|女|13|
科目表：
|代号|名称|老师|
|----|----|----|
|01|语文|王老师|
|02|数学|李老师|
分数表：
|id|学号|科目代号|分数|
|----|----|----|----|
|01|01|01|98|
|02|01|02|87|
|03|02|01|76|
|04|02|02|88|
        这样的结构，当我想要知道任何一个人的各科成绩的时候，需要联查这几张表，这个时候如果每个学生可能对应的科目是有差异的时候，我们想要得到成绩的列数也是不同的了，这个时候我们需要把课程作为列添加到学生表后面作为联查的项来进行查询。
# 二、简单的实现
        假设，我们查询的所有学生上的课程都是一样的，而且不会变化，那么我们用如下的sql语句进行查询就可以得到结果：
```
Select st.stuid, st.stunm, 
    MAX(CASE c.coursenm WHEN '大学语文' THEN s.scores ELSE 0 END ) '大学语文',
    MAX(CASE c.coursenm WHEN '新视野英语' THEN ifnull(s.scores,0) ELSE 0 END ) '新视野英语', 
    MAX(CASE c.coursenm WHEN '离散数学' THEN ifnull(s.scores,0) ELSE 0 END ) '离散数学',
    MAX(CASE c.coursenm WHEN '概率论与数理统计' THEN ifnull(s.scores,0) ELSE 0 END ) '概率论与数理统计',
    MAX(CASE c.coursenm WHEN '线性代数' THEN ifnull(s.scores,0) ELSE 0 END ) '线性代数',
    MAX(CASE c.coursenm WHEN '高等数学(一)' THEN ifnull(s.scores,0) ELSE 0 END ) '高等数学(一)',
    MAX(CASE c.coursenm WHEN '高等数学(二)' THEN ifnull(s.scores,0) ELSE 0 END ) '高等数学(二)'
From Student  st
Left Join score s On st.stuid = s.stuid
Left Join courses c On c.courseno = s.courseno
Group by st.stuid
```
     但是如果科目是随着年级的变化而变化的话，写到sql中就不是很明智的选择了。这个时候就需要用到动态的查询了。
     我们可以看到，创建列的语句其实就是几个重复的语句，只要把他们变成动态的就可以了。
```
MAX(CASE c.coursenm WHEN '大学语文' THEN s.scores ELSE 0 END ) '大学语文',
    MAX(CASE c.coursenm WHEN '新视野英语' THEN ifnull(s.scores,0) ELSE 0 END ) '新视野英语', 
    MAX(CASE c.coursenm WHEN '离散数学' THEN ifnull(s.scores,0) ELSE 0 END ) '离散数学',
    MAX(CASE c.coursenm WHEN '概率论与数理统计' THEN ifnull(s.scores,0) ELSE 0 END ) '概率论与数理统计'
```
     于是有了下面的查询方式：
```
Select st.stuid, st.stunm, 
(
    SELECT
      GROUP_CONCAT(DISTINCT
        CONCAT(
          'MAX(IF(c.coursenm = ''',
          c.coursenm,
          ''', s.scores, NULL)) AS ',
          c.coursenm
        )
      )
    FROM courses c
)
From Student  st
Left Join score s On st.stuid = s.stuid
Left Join courses c On c.courseno = s.courseno
Group by st.stuid;
```
     但是这样查询是不行的，这样的话会把整个大括号中的内容算作是一列的内容，导致输出了sql语句的一部分，所以这里需要分开进行书写查询：
```
SET @sql = NULL;
SELECT
  GROUP_CONCAT(DISTINCT
    CONCAT(
      'MAX(IF(c.coursenm = ''',
      c.coursenm,
      ''', s.scores, 0)) AS ''',
      c.coursenm, ''''
    )
  ) INTO @sql
FROM courses c;
SET @sql = CONCAT('Select st.stuid, st.stunm, ', @sql, 
                        ' From Student  st 
                        Left Join score s On st.stuid = s.stuid
                        Left Join courses c On c.courseno = s.courseno
                        Group by st.stuid');
PREPARE stmt FROM @sql;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;
```
      这样写就可以了，通过我自己的实践，是可以查询出来的，一下为我的查询语句：
```
SET @sql = NULL;
SET @goods_id = '1';
SELECT
  GROUP_CONCAT(DISTINCT
    CONCAT(
      'MAX(IF(bgp.property_name = ''',
      bgp.property_name,
      ''', bgpi.item_name, 0)) AS ''',
      bgp.property_name, ''''
    )
  ) INTO @sql
FROM
	busi_goods_property as bgp;
SET @sql = CONCAT('Select bgs.sku_num,bgs.sku_price,
					bgs.sku_stock,
					bgs.sku_stock_warning, ', @sql, 
                        ' FROM
						busi_goods_sku AS bgs
					LEFT JOIN busi_goods_sku_property AS bgsp ON bgs.sku_id=bgsp.sku_id
					left JOIN busi_goods_property as bgp ON bgsp.property_id=bgp.property_id
					LEFT JOIN busi_goods_property_item as bgpi ON bgsp.item_id=bgpi.item_id
                        Where bgs.goods_id = ''', @goods_id, '''
                        group by bgs.sku_num');
PREPARE stmt FROM @sql;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;
```
    这里需要注意的是，在大查询的末尾，必须有那个group by的语句，否则查询只会查询出一条结果，并且数据是不准确的。这里主要是因为在查询数据的时候是以我们的最外层分组为依据来查询的里面动态列的内容，所以需要对外层进行分组之后才可以知道里面的数据对应关系。
