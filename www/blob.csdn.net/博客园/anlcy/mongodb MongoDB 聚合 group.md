
# mongodb MongoDB 聚合 group - anlcy - 博客园






# [mongodb MongoDB 聚合 group](https://www.cnblogs.com/camilla/p/7908027.html)
MongoDB中聚合(aggregate)主要用于处理数据(诸如统计平均值,求和等)，并返回计算后的数据结果。有点类似sql语句中的 count(*)。
基本语法为：db.collection.aggregate([<stage1>,<stage2>,...])
现在在mycol集合中有以下数据：
{ "_id" : 1, "name" : "tom", "sex" : "男", "score" : 100, "age" : 34 }
{ "_id" : 2, "name" : "jeke", "sex" : "男", "score" : 90, "age" : 24 }
{ "_id" : 3, "name" : "kite", "sex" : "女", "score" : 40, "age" : 36 }
{ "_id" : 4, "name" : "herry", "sex" : "男", "score" : 90, "age" : 56 }
{ "_id" : 5, "name" : "marry", "sex" : "女", "score" : 70, "age" : 18 }
{ "_id" : 6, "name" : "john", "sex" : "男", "score" : 100, "age" : 31 }
1、$sum 计算总和。
Sql:  select sex,count(*) from mycol group by sex
MongoDb:  db.mycol.aggregate([{
sex', personCount: {$sum: 1}}}])
![](https://images2015.cnblogs.com/blog/194720/201608/194720-20160811140754637-1064246062.png)
Sql:  select sex,sum(score) totalScore from mycol group by sex
MongoDb:  db.mycol.aggregate([{
sex', totalScore: {
score'}}}])
![](https://images2015.cnblogs.com/blog/194720/201608/194720-20160811140954184-1609618065.png)
2、$avg  计算平均值
Sql:  select sex,avg(score) avgScore from mycol group by sex
Mongodb: db.mycol.aggregate([{
sex', avgScore: {
score'}}}])
![](https://images2015.cnblogs.com/blog/194720/201608/194720-20160811141300887-1518398155.png)
3、$max 获取集合中所有文档对应值得最大值。
Sql:  select sex,max(score) maxScore from mycol group by sex
Mongodb: db.mycol.aggregate([{
sex', maxScore : {
score'}}}])
![](https://images2015.cnblogs.com/blog/194720/201608/194720-20160811141440027-1275411843.png)
4、$min 获取集合中所有文档对应值得最小值。
Sql:  select sex,min(score) minScore from mycol group by sex
Mongodb: db.mycol.aggregate([{
sex', minScore : {
score'}}}])
![](https://images2015.cnblogs.com/blog/194720/201608/194720-20160811141536481-907441449.png)
5、$push 把文档中某一列对应的所有数据插入值到一个数组中。
Mongodb: db.mycol.aggregate([{
sex', scores : {
score'}}}])
![](https://images2015.cnblogs.com/blog/194720/201608/194720-20160811141802215-1229143273.png)
6、$addToSet 把文档中某一列对应的所有数据插入值到一个数组中,去掉重复的
db.mycol.aggregate([{
sex', scores : {
score'}}}])
![](https://images2015.cnblogs.com/blog/194720/201608/194720-20160811141923590-1704142144.png)
7、 $first 根据资源文档的排序获取第一个文档数据。
db.mycol.aggregate([{
sex', firstPerson : {
name'}}}])
![](https://images2015.cnblogs.com/blog/194720/201608/194720-20160811142238277-1414770145.png)
8、 $last 根据资源文档的排序获取最后一个文档数据。
db.mycol.aggregate([{
sex', lastPerson : {
name'}}}])
![](https://images2015.cnblogs.com/blog/194720/201608/194720-20160811142332793-712437189.png)
9、全部统计 null
db.mycol.aggregate([{
push:'$score'}}}])
![](https://images2015.cnblogs.com/blog/194720/201608/194720-20160818165951328-769010829.png)
**例子**
现在在t2集合中有以下数据：
{ "country" : "china", "province" : "sh", "userid" : "a" }
{  "country" : "china", "province" : "sh", "userid" : "b" }
{  "country" : "china", "province" : "sh", "userid" : "a" }
{  "country" : "china", "province" : "sh", "userid" : "c" }
{  "country" : "china", "province" : "bj", "userid" : "da" }
{  "country" : "china", "province" : "bj", "userid" : "fa" }
需求是统计出每个country/province下的userid的数量（同一个userid只统计一次）
过程如下。
首先试着这样来统计：
db.t2.aggregate([ {
country", "prov": "province"} , "number":{
sum:1}} } ])
结果是错误的：
![](https://images2015.cnblogs.com/blog/194720/201705/194720-20170509105314285-808164818.png)
原因是，这样来统计不能区分userid相同的情况 （上面的数据中sh有两个 userid = a)
为了解决这个问题，首先执行一个group，其id 是 country, province, userid三个field：
db.t2.aggregate([ {
country", "province": "
userid" } } } ])
![](https://images2015.cnblogs.com/blog/194720/201705/194720-20170509105423004-1020811984.png)
可以看出，这步的目的是把相同的userid只剩下一个。
然后第二步，再第一步的结果之上再执行统计：
db.t2.aggregate([
{
country", "province": "userid" } } } ,
{group: {"_id": { "country" : "_id.country", "province": "_id.province"  }, count : {
sum : 1 }  } }
])
这回就对了
![](https://images2015.cnblogs.com/blog/194720/201705/194720-20170509105559926-721535226.png)
加入一个$project操作符，把_id去掉
db.t2.aggregate([ {
country", "province": "userid" } } } ,
{group: {"_id": { "country" : "_id.country", "province": "_id.province"  }, count: {sum : 1 }  } },
{project : {"_id": 0, "country" : "
_id.country", "province" : "$_id.province", "count" : 1}}
])
最终结果如下：
![](https://images2015.cnblogs.com/blog/194720/201705/194720-20170509105730801-1948014396.png)

## 管道的概念
管道在Unix和Linux中一般用于将当前命令的输出结果作为下一个命令的参数。
MongoDB的聚合管道将MongoDB文档在一个管道处理完毕后将结果传递给下一个管道处理。管道操作是可以重复的。
表达式：处理输入文档并输出。表达式是无状态的，只能用于计算当前聚合管道的文档，不能处理其它的文档。
这里我们介绍一下聚合框架中常用的几个操作：
$project：修改输入文档的结构。可以用来重命名、增加或删除域，也可以用于创建计算结果以及嵌套文档。
match使用MongoDB的标准查询操作。
$limit：用来限制MongoDB聚合管道返回的文档数。
$skip：在聚合管道中跳过指定数量的文档，并返回余下的文档。
$unwind：将文档中的某一个数组类型字段拆分成多条，每条包含数组中的一个值。
$group：将集合中的文档分组，可用于统计结果。
$sort：将输入文档排序后输出。
$geoNear：输出接近某一地理位置的有序文档。
1、$project实例
db.mycol.aggregate({$project:{name : 1, score : 1}})
![](https://images2015.cnblogs.com/blog/194720/201608/194720-20160811145707715-2050560306.png)
这样的话结果中就只还有_id,name和score三个字段了，默认情况下_id字段是被包含的，如果要想不包含_id话可以这样:
db.mycol.aggregate({$project:{_id : 0, name : 1, score : 1}})
![](https://images2015.cnblogs.com/blog/194720/201608/194720-20160811145640106-1338573698.png)
2、$match实例
group管道操作符进行处理
db.mycol.aggregate([{
gt: 30,lt: 100}}},{group:{_id:'sex',count:{
sum:1}}}])
![](https://images2015.cnblogs.com/blog/194720/201608/194720-20160811150107840-1022915497.png)





