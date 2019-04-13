
# MongoDB aggregate聚合 - 追求卓越,做到专业 - CSDN博客


2016年11月11日 11:20:40[Waldenz](https://me.csdn.net/enter89)阅读数：348



$project：包含、排除、重命名和显示字段
$match：查询，需要同find()一样的参数
$limit：限制结果数量
$skip：忽略结果的数量
$sort：按照给定的字段排序结果
$group：按照给定表达式组合结果
$unwind：分割嵌入数组到自己顶层文件
文档：[MongoDB 官方](http://docs.mongodb.org/manual/reference/method/db.collection.aggregate/)[aggregate](http://docs.mongodb.org/manual/reference/method/db.collection.aggregate/)[说明](http://docs.mongodb.org/manual/reference/method/db.collection.aggregate/)。
相关使用：
db.collection.aggregate([array]);
array可是是任何一个或多个操作符。
group和match的用法，使用过sqlserver，group的用法很好理解，根据指定列进行分组统计，可以统计分组的数量，也能统计分组中的和或者平均值等。
group之前的match，是对源数据进行查询，group之后的match是对group之后的数据进行筛选；
同理，sort，skip，limit也是同样的原理；
1{_id:1,name:"a",status:1,num:1}2{_id:2,name:"a",status:0,num:2}3{_id:3,name:"b",status:1,num:3}4{_id:4,name:"c",status:1,num:4}5{_id:5,name:"d",status:1,num:5}
以下是示例：
应用一：统计name的数量和总数；
db.collection.aggregate([
{$group:{_id:"$name",count:{$sum:1},total:{$sum:"$num"}}
]);
应用二：统计status=1的name的数量；
db.collection.aggregate([
{$match:{status:1}},
{$group:{_id:"$name",count:{$sum:1}}}
]);
应用三：统计name的数量，并且数量为小于2的；
db.collection.aggregate([
{$group:{_id:"$name",count:{$sum:1}},
{$match:{count:{$lt:2}}}
]);
应用四：统计stauts=1的name的数量，并且数量为1的；
db.collection.aggregate([
{$match:{status:1}},
{$group:{_id:"$name",count:{$sum:1}}},
{$match:{count:1}}
]);
多列group，根据name和status进行多列
db.collection.aggregate([
{$group:{_id:{name:"$name",st:"$status"},count:{$sum:1}}}
]);
$project该操作符很简单，
db.collection.aggregate([
{$project:{name:1,status:1}}
]);
结果是，只有_id,name,status三个字段的表数据，相当于sql表达式 select _id,name,status from collection

$unwind
这个操作符可以将一个数组的文档拆分为多条文档，在特殊条件下有用，本人暂没有进行过多的研究。

以上基本就可以实现大部分统计了，group前条件，group后条件，是重点。


