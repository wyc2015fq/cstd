
# usleep - 延迟以微秒为单位的时间 - Augusdi的专栏 - CSDN博客


2015年08月22日 10:39:59[Augusdi](https://me.csdn.net/Augusdi)阅读数：2489


﻿﻿
## 用途说明
usleep命令用于延迟以微秒为单位的时间（sleep some number of microseconds），实际上达不到这个精度（Probably not accurate on many machines down to the microsecond.  Count on precision only to -4 or maybe -5.）。如果系统中没有usleep命令，可采用sleep命令来进行，sleep命令可以指定一个带小数的时间延迟。1秒=1000毫秒=1000,000微秒=1000,000,000纳秒。
## 常用方式
格式：usleep <us>
延迟<us>微秒。

## 使用示例
### 示例一 usleep命令精度测试
[root@jfht ~]\#date +%S.%N; usleep; date +%S.%N
04.194798315
04.199352948
[root@jfht ~]\#date +%S.%N; usleep; date +%S.%N
12.684509970
12.688986355
[root@jfht ~]\#date +%S.%N; usleep 1000; date +%S.%N
33.179766926
33.186719087
[root@jfht ~]\#date +%S.%N; usleep 10000; date +%S.%N
45.556317402
45.571467950
[root@jfht ~]\#date +%S.%N; usleep 100000; date +%S.%N
59.656176597
59.761001631
[root@jfht ~]\#date +%S.%N; usleep 1000000; date +%S.%N
56.700405298
57.704304279
[root@jfht ~]\#

### 示例二 sleep命令精度测试
[root@jfht ~]\#date +%S.%N; sleep 1; date +%S.%N
47.111204217
48.115898894
[root@jfht ~]\#date +%S.%N; sleep 0.1; date +%S.%N
57.760602228
57.865456080
[root@jfht ~]\#date +%S.%N; sleep 0.01; date +%S.%N
08.912279911
08.927840798
[root@jfht ~]\#date +%S.%N; sleep 0.001; date +%S.%N
18.171663617
18.176771668
[root@jfht ~]\#date +%S.%N; sleep 0.0001; date +%S.%N
27.290651737
27.295785240
[root@jfht ~]\#

## 问题思考
## 相关资料
【1】NIXway.net usleep (1) 命令参考
http://www.nixway.net/index.php?manitem&mid=2558
【2】LinuxSir.Org ubuntu中usleep命令不可用，要装哪个包呢？
[http://www.linuxsir.org/bbs/showthread.php?t=315109](http://www.linuxsir.org/bbs/showthread.php?t=315109)

[http://codingstandards.iteye.com/blog/1007783](http://codingstandards.iteye.com/blog/1007783)

