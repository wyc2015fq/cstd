# [Hive]用concat_w实现将多行记录合并成一行 - z69183787的专栏 - CSDN博客
2018年12月27日 18:41:59[OkidoGreen](https://me.csdn.net/z69183787)阅读数：258
[https://blog.csdn.net/yeweiouyang/article/details/41286469](https://blog.csdn.net/yeweiouyang/article/details/41286469)
[https://blog.csdn.net/waiwai3/article/details/79071544](https://blog.csdn.net/waiwai3/article/details/79071544)
需求：对用户的订单进行分析，将不同订单类型分别多少单展示出来，每个用户一行
原数据：
user    order_type    order_number
user1    delivered    10
user2    returned    1
user1    returned    3
user2    delivered    20
目标：
user    order
user1    delivered(10),returned(3)
user2    delivered(20),returned(1)
1.使用concat（）函数将order_type和order_number连接起来
concat（order_type,'(',order_number,')'）
user    order
user1    delivered(10)
user2    returned(1)
user1    returned(3)
user2    delivered(20)
2.使用concat_ws（）和collect_set（）进行合并行
将上面列表中一个user可能会占用多行转换为每个user占一行的目标表格式，实际是“列转行”
select user,concat_ws(',',collect_set(concat（order_type,'(',order_number,')'）))  order from table group by user
order是别名
collect_set的作用：
（1）去重，对group by后面的user进行去重
（2）对group by以后属于同一user的形成一个集合，结合concat_ws对集合中元素使用，进行分隔形成字符串
建表如下：
# 创建商品与促销活动的映射表
hive -e "set mapred.job.queue.name=pms;
set hive.exec.reducers.max=32;
set mapred.reduce.tasks=32;
drop table if exists product_promotion;
create table product_promotion(product_id bigint, promotion_id String);
insert into table product_promotion 
select p2.product_id, p2.promotion_id 
from pms.promotionv2 p1 inner join pms.promotionv2_main_product_sku p2 
on (p1.id=p2.promotion_id)
where from_unixtime(unix_timestamp(),'yyyy-MM-dd HH:mm:ss') between p1.start_date and p1.end_date;"
数据表的记录如下：
5112 960024
5112 960025
5112 960026
5112 960027
5112 960028
5113 960043
5113 960044
5113 960045
5113 960046
对promotion_id进行合并：
select product_id, concat_ws('_',collect_set(promotion_id)) as promotion_ids from product_promotion group by product_id
执行结果：
hive > select product_id, concat_ws('_',collect_set(promotion_id)) as promotion_ids from product_promotion group by product_id;
OK
5112 960024_960025_960026_960027_960028
5113 960043_960044_960045_960046
Time taken: 3.116 seconds
