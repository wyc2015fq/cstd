
# Bash: sleep - 延迟指定时间 - Augusdi的专栏 - CSDN博客


2015年08月22日 10:40:57[Augusdi](https://me.csdn.net/Augusdi)阅读数：4433


﻿﻿
## 用途说明
sleep命令常用于在shell脚本中延迟时间。

## 常用方式
注意：以下用法中<n>可以为小数。
格式：sleep <n>
格式：sleep <n>s
延迟<n>秒。
格式：sleep <n>m
延迟<n>分钟。
格式：sleep <n>h
延迟<n>小时。
格式：sleep <n>d
延迟<n>天。

## 使用示例
### 示例一
[root@jfht ~]\#date; sleep 5; date
2011年 04月 17日 星期日 19:49:23 CST
2011年 04月 17日 星期日 19:49:28 CST
[root@jfht ~]\#

### 示例二
[root@jfht ~]\#date; sleep 1.5m; date
2011年 04月 17日 星期日 19:50:06 CST
2011年 04月 17日 星期日 19:51:36 CST
[root@jfht ~]\#

## 问题思考
## 相关资料
【1】中国IT实验室 linux sleep命令详解
http://linux.chinaitlab.com/command/808582.html
【2】bbs.ChinaUnix.net 用sleep命令代替crontab定期执行命令
http://bbs.chinaunix.net/thread-2193921-1-1.html


