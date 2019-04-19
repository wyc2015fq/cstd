# 【高并发简单解决方案】redis缓存队列+mysql 批量入库+php离线整合 - 文章 - 伯乐在线
原文出处： [崔小拽](http://cuihuan.net/article/%E3%80%90%E9%AB%98%E5%B9%B6%E5%8F%91%E7%AE%80%E5%8D%95%E8%A7%A3%E5%86%B3%E6%96%B9%E6%A1%88%E3%80%91redis%E7%BC%93%E5%AD%98%E9%98%9F%E5%88%97-mysql-%E6%89%B9%E9%87%8F%E5%85%A5%E5%BA%93-php%E7%A6%BB.html)
> 
需求背景：有个调用统计日志存储和统计需求，要求存储到mysql中；存储数据高峰能达到日均千万，瓶颈在于直接入库并发太高，可能会把mysql干垮。
## 问题分析
思考：应用网站架构的衍化过程中，应用最新的框架和工具技术固然是最优选择；但是，如果能在现有的框架的基础上提出简单可依赖的解决方案，未尝不是一种提升自我的尝试。
解决：
- 问题一：要求日志最好入库；但是，直接入库mysql确实扛不住，批量入库没有问题，done。【批量入库和直接入库性能差异参考文章】
- 问题二：批量入库就需要有高并发的消息队列，决定采用redis list 仿真实现，而且方便回滚。
- 问题三：日志量毕竟大，保存最近30条足矣，决定用php写个离线统计和清理脚本。
done，下面是小拽的简单实现过程
## 一：设计数据库表和存储
- 考虑到log系统对数据库的性能更多一些，稳定性和安全性没有那么高，`存储引擎自然是只支持select insert 没有索引的archive`。如果确实有update需求，也可以采用myISAM。
- 考虑到log是实时记录的所有数据，数量可能巨大，`主键采用bigint，自增即可`。
- 考虑到log系统`以写为主，统计采用离线计算，字段均不要出现索引`，因为一方面可能会影响插入数据效率，另外读时候会造成死锁，影响写数据。
## 二：redis存储数据形成消息队列
由于高并发，尽可能简单，直接，上代码。

MySQL
```
connect('xx', 6379);
$redis->auth("password");
// 加上时间戳存入队列
$now_time = date("Y-m-d H:i:s");
$redis->rPush("call_log", $interface_info . "%" . $now_time);
$redis->close();
/* vim: set ts=4 sw=4 sts=4 tw=100 */
?>
```
## 三：数据定时批量入库。
定时读取redis消息队列里面的数据，批量入库。

MySQL
```
connect('ip', port);
$redis_xx->auth("password");
// 获取现有消息队列的长度
$count = 0;
$max = $redis_xx->lLen("call_log");
// 获取消息队列的内容，拼接sql
$insert_sql = "insert into fb_call_log (`interface_name`, `createtime`) values ";
// 回滚数组
$roll_back_arr = array();
while ($count lPop("call_log");
    $roll_back_arr = $log_info;
    if ($log_info == 'nil' || !isset($log_info)) {
        $insert_sql .= ";";
        break;
    }
    // 切割出时间和info
    $log_info_arr = explode("%",$log_info);
    $insert_sql .= " ('".$log_info_arr[0]."','".$log_info_arr[1]."'),";
    $count++;
}
// 判定存在数据，批量入库
if ($count != 0) {
    $link_2004 = mysql_connect('ip:port', 'user', 'password');
    if (!$link_2004) {
        die("Could not connect:" . mysql_error());
    }
    $crowd_db = mysql_select_db('fb_log', $link_2004);
    $insert_sql = rtrim($insert_sql,",").";";
    $res = mysql_query($insert_sql);
    // 输出入库log和入库结果;
    echo date("Y-m-d H:i:s")."insert ".$count." log info result:";
    echo json_encode($res);
    echo "n";
    // 数据库插入失败回滚
    if(!$res){
       foreach($roll_back_arr as $k){
           $redis_xx->rPush("call_log", $k);
       }
    }
    // 释放连接
    mysql_free_result($res);
    mysql_close($link_2004);
}
// 释放redis
$redis_cq01->close();
?>
```
## 四：离线天级统计和清理数据脚本

MySQL
```
?php
/**
* static log ：每天离线统计代码日志和删除五天前的日志
*
* @Author:cuihuan@baidu.com
* 2015-11-06
* */
// 离线统计
$link_2004 = mysql_connect('ip:port', 'user', 'pwd');
if (!$link_2004) {
    die("Could not connect:" . mysql_error());
}
$crowd_db = mysql_select_db('fb_log', $link_2004);
// 统计昨天的数据
$day_time = date("Y-m-d", time() - 60 * 60 * 24 * 1);
$static_sql = "get sql";
$res = mysql_query($static_sql, $link_2004);
// 获取结果入库略
// 清理15天之前的数据
$before_15_day = date("Y-m-d", time() - 60 * 60 * 24 * 15);
$delete_sql = "delete from xxx where createtime
```
## 五：代码部署
主要是部署，批量入库脚本的调用和天级统计脚本，crontab例行运行。

MySQL
```
# 批量入库脚本
*/2 * * * * /home/cuihuan/xxx/lamp/php5/bin/php /home/cuihuan/xxx/batchLog.php >>/home/cuihuan/xxx/batchlog.log
# 天级统计脚本
0 5 * * * /home/cuihuan/xxx/php5/bin/php /home/cuihuan/xxx/staticLog.php >>/home/cuihuan/xxx/staticLog.log
```
> 
总结：相对于其他复杂的方式处理高并发，这个解决方案简单有效：通过redis缓存抗压，mysql批量入库解决数据库瓶颈，离线计算解决统计数据，通过定期清理保证库的大小。
