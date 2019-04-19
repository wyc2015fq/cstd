# Redis中的集合 - LC900730的博客 - CSDN博客
2017年05月19日 16:26:07[lc900730](https://me.csdn.net/LC900730)阅读数：180
Redis中的有序集合zset 
在实际应用中，需要存储一个person的综合考察评分score，因此redis的zset是一个很好的存储方式；
```php
//将人和对应的分数加入到某个zset中
public function set_userset_byjob($jobtypeid,$score,$personid){
    $key=$jobtypeid.'users';
    $redis->zadd($key,$score,$personid)
}
//取得集合中某些元素
//默认起始值是0，结束值是-1,实际项目中需要加限制
public function get_userset_byjob($jobtypeid,$start=0,$stop=-1){
    $key=$jobtypeid.'users';
    $redis->zadd($key,$score,$personid)
}
```
### set
$redis->sMembers($key) 
实际使用：
|向zset中添加|$rds->zadd($key,$score,$personid)(如果值已经存在，那么score会覆盖)|
|----|----|
|查看zset中成员个数|$rds->zCard($key)|
|查看zset中是否存在某个成员|$rds->exists($key)|
|排序，从高到低排序输出指定范围的数据|$rds->zRevRange($key,0,1):输出下标0-1的数据，倒序之后的下标也反过来了|
|排序，从高到低排序输出指定范围的数据|$rds->zRevRange($key,0,1,’withscore:BOOL’):输出下标0-1的数据,键值对是personid=>score的形式|
|获取zset集合中指定分数之间存在的成员个数|$rds->zcount($key,100,10000):输出分数>=100与|
