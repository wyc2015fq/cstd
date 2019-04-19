# logstash 从数据库获取数据到将数据导入到es步骤 - sxf_123456的博客 - CSDN博客
2017年08月03日 21:08:49[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：495
                
1、logstash配置文件存放路径
/data/app/etc/logstash/conf.d/
2、sql脚本存放路径
/usr/local/xuanfeng/etl/scripts
3、获取数据的oracle命令
/usr/local/xuanfeng/etl/bin
4、oracle命令执行生成日志存放路径
/usr/local/xuanfeng/etl/log
5、执行logstash生成日志存放路径
/data/app/var/log/logstash/
6、采集数据转换为json格式的配置文件路径
/usr/local/xuanfeng/etl/etc
7、执行采集命令
./oracle2file -c 018_areazw -f 018 -n 001 -o -t 20170720000000
请输入正确的参数：调度执行时间（ -t ）
用法: oracle2file [-options] name [args...]
  -c string
调度执行配置编号
  -f string
调度流程编号
  -n string
调度流程节点编号
  -o启用详细输出<选项>
默认：false
  -t string
调度执行时间
格式：[YYYYMMDDHH24MISS]
8、生成csv文件存放路径
/data/gatherfiles/datafiles/device
9、文件读取纪录值保留路径
/data/elk/sincedb
10、将数据通过logstash导入es命令
cd /data/app/etc/logstash/conf.d/
logstash -f 配置
            
