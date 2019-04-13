
# Redis数据基本操作 - 尹成的技术博客 - CSDN博客

2018年11月16日 09:56:01[尹成](https://me.csdn.net/yincheng01)阅读数：14个人分类：[区块链](https://blog.csdn.net/yincheng01/article/category/7618299)[GO语言](https://blog.csdn.net/yincheng01/article/category/7679307)[](https://blog.csdn.net/yincheng01/article/category/7618299)



**@概述**
这里列举了键值对和其它数据类型的一些常用操作；
更多操作命令和释义可以参考：[http://redis.cn/commands.html](http://redis.cn/commands.html)
**@字符串操作举例**
`set name bill //设置键值
setex name 20 bill //键值20秒后过期
mset age 60 gender male //一次性存储多个键值
get name //不存在则返回null
mget name age //一次性拿取多个键值
incr/decr age //将age加减1
incrby/decrby age 20 //将age加减20
append name gates //向原有value追加内容
strlen key //获取值长度`**@key键操作举例**
`keys * //显示所有键，慎用，数据量大时导致卡死
keys a* // 显示所有以a开头的键
exists name age //判断name和age这两个键存在几个
type name //查看值的类型
del name age //删除键
expire name 30 //name键值30秒过期
ttl name //查看键值的过期时间
persist name // 取消键的过期时间
rename name nickname // 重命名键（这样做不安全，容易覆盖已有数据）
renamenx name nickname // 确保不会覆盖其它键才重命名（nx=not exists,x后缀=exists）`**@哈希hash操作举例**
hash是将多个字段和值存储在一个键中，类似对象；
`hset p1 name bill // 设置p1对象的name为bill
hset p1 age 18 // 设置p1对象的age为18
hmset p2 name jobs age -1 //设置p2对象的name为jobs，age为-1
hget p1 name // 获取p1的name属性值
hmget p1 name age // 同时获取p1的name和age
hgetall p1 // 获取p1的全部字段和值
hkeys p1 // 获取p1的全部字段
hvals p1 // 获取p1的全部字段值
hlen p1 // 获取p1中所有字段数
hexists p1 name // 判断p1中是否有name字段
hdel p1 name // 删除p1中的name字段
hdel p2 name age // 删除p2中的name和age字段
hstrlen p1 age // p1中的age的长度`**@列表list操作举例**
在redis中也称队列，可按下标获取元素，也可从头尾弹出或放入；
`lpush mlist 2 // 从左侧向列表mlist追加元素2
rpush mlist 3 // 从右侧向列表mlist追加元素3
linsert mlist after 3 4 // 在列表mlist中3的后边追加元素4
lset mlist 1 200 // 设置列表mlist中的第1个元素为200（下标从0开始）
lrange mlist 0 3 // 查看列表mlist中下标[0到3]的元素
lrange mlist 0 -1 //从头看到尾
lpop mlist // 从列表左侧弹出一个值
rpop mlist // 从列表右侧弹出一个值
ltrim mlist 0 1 // 修剪掉mlist下标[0,1]以外的元素
llen mlist // 列表元素个数
lindex mlist 0 // 获取mlist中下标0的元素`**@集合set操作举例**
无序元素集，经常用于求交集、并集、差集等；
`sadd mset 1 2 3 4 // 向集合mset中添加元素1,2,3,4
smembers mset // 查看集合mset中的元素
scard mset // 查询集合元素个数
sinter mset mset2 //求mset和mset2的交集
sdiff mset mset2 //求mset相比mset2的差集（mset有而mset2无的部分）
sunion mset mset2 // 求mset和mset2的并集
sismember mset 3 //判断3是否是mset中的元素`**@有序集合zset**
可按权重对元素进行排列；
`zadd mzset 10 bill 9 jobs 6 jackma // 添加多个值及相应权重
zrange mzset 0 -1 // 返回第一个到最后一个之间所有的元素，按权重升序排列
zcard mzset // 元素个数
zcount mzset 6 10 // 权重6-10之间的元素个数
zscore mzset jackma // 返回 jackma在mzset中的权重`学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/344443?tuin=3d17195d)
扫码获取海量视频及源码   QQ群：721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116092700978.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

