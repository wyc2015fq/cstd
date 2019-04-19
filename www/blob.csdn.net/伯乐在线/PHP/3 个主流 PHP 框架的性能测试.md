# 3 个主流 PHP 框架的性能测试 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [李虎头](http://www.jobbole.com/members/kphcdr) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
**主流PHP框架性能非权威测试**
作为一个PHP开发者，而且是初创企业团队的技术开发者，选择开发框架是个很艰难的事情。
用`ThinkPHP`的话，招聘一个刚从培训机构出来的开发者就可以上手了，但是性能和后期代码解耦是个让人头疼的事情。不过很多第三方功能不需要自己写，众多大牛已经给铺好路了。
用`Laravel`的话，传说写起来很爽扩展性也够，但是学习成本有点高，总不能给初级开发者半个月的时间去学习框架吧。而且据说江湖人士透漏，`Laravel`性能不怎么样，文档也并不是特别丰富。
用`Yii`的话，语法有点啰嗦，前后端代码分离有点小麻烦，不过性能是非常好的，而且已经有众多国内大公司在使用了，出现意外可以快速的找到大牛答疑解惑。
不如做个简单的性能评测，选个性能出色的总不会出错
# 性能测试
测试时间： 2016年05月06日
测试工具： `siege` (因为MAC系统的`ab`总是会出现 `pr_socket_recv: Connection reset by peer (54)` 的错误，所以选择了`siege`)
测试机器： MacPro Core i5 处理器/8GB内存/256GB SSD闪存
测试环境： Apache PHP5.6
框架版本： `ThinkPHP 3.2.3``Laravel 5.2``Yii2.0.5`
测试原则： 每次测试循环5次，取中等数据
补充说明： 所有项目都是让在子目录中
# 结果说明


```
Transactions:               2119 hits  处理请求总数量
Availability:              96.85 %  可用性
Elapsed time:               9.74 secs  运行时间
Data transferred:           1.31 MB 数据传输量
Response time:              0.60 secs 响应时间
Transaction rate:         217.56 trans/sec 每秒处理效率
Throughput:             0.13 MB/sec 每秒处理数据
Concurrency:              130.28 并发
Successful transactions:        2162 成功的请求
Failed transactions:              69 失败的请求
Longest transaction:            2.85 最长的单个请求
Shortest transaction:           0.01 最短的单个请求
```
# 开始测试
## 增加一个控制器，并在控制器中输入10000次HelloWorld
### 并发50循环10次
首先向我们走来的是ThinkPHP，看上去还行


```
Transactions:                500 hits
Availability:             100.00 %
Elapsed time:               2.81 secs
Data transferred:          52.45 MB
Response time:              0.26 secs
Transaction rate:         177.94 trans/sec
Throughput:            18.67 MB/sec
Concurrency:               47.10
Successful transactions:         500
Failed transactions:               0
Longest transaction:            0.48
Shortest transaction:           0.03
```
然后是Laravle，嗯，略微有些失望


```
Transactions:                500 hits
Availability:             100.00 %
Elapsed time:              13.33 secs
Data transferred:          52.45 MB
Response time:              1.27 secs
Transaction rate:          37.51 trans/sec
Throughput:             3.93 MB/sec
Concurrency:               47.55
Successful transactions:         500
Failed transactions:               0
Longest transaction:            3.64
Shortest transaction:           0.07
```
最后赛前比较看好的Yii


```
Transactions:                500 hits
Availability:             100.00 %
Elapsed time:               4.84 secs
Data transferred:          52.45 MB
Response time:              0.46 secs
Transaction rate:         103.31 trans/sec
Throughput:            10.84 MB/sec
Concurrency:               47.65
Successful transactions:         500
Failed transactions:               0
Longest transaction:            0.88
Shortest transaction:           0.04
```
### 并发200循环10次
首先是ThinkPHP


```
Transactions:               1977 hits
Availability:              98.85 %
Elapsed time:              10.03 secs
Data transferred:         207.40 MB
Response time:              0.95 secs
Transaction rate:         197.11 trans/sec
Throughput:            20.68 MB/sec
Concurrency:              187.68
Successful transactions:        1977
Failed transactions:              23
Longest transaction:            1.22
Shortest transaction:           0.02
```
然后是Laravel


```
Transactions:               1890 hits
Availability:              94.50 %
Elapsed time:              51.85 secs
Data transferred:         198.27 MB
Response time:              4.88 secs
Transaction rate:          36.45 trans/sec
Throughput:             3.82 MB/sec
Concurrency:              178.00
Successful transactions:        1890
Failed transactions:             110
Longest transaction:           26.01
Shortest transaction:           0.07
```
最后是Yii


```
Transactions:               1996 hits
Availability:              99.80 %
Elapsed time:              18.95 secs
Data transferred:         209.39 MB
Response time:              1.79 secs
Transaction rate:         105.33 trans/sec
Throughput:            11.05 MB/sec
Concurrency:              188.57
Successful transactions:        1996
Failed transactions:               4
Longest transaction:            3.29
Shortest transaction:           0.10
```
### 没想到在没有优化的情况下，ThinkPHP的速度是最快的，Yii略微多一些，Laravel步履蹒跚的跑完了测试。结果是有些出人意料的，不过上述的测试只代表了开发环境，下面会对框架进行优化，模拟线上环境。
## 优化框架
ThinkPHP：


```
APP_DEBUG改为false
```
Laravel：


```
APP_DEBUG改为false
php artisan route:cache
php artisan optimize
php artisan config:cache
composer dumpautoload -o
```
Yii：


```
YII_DEBUG改为false
composer dumpautoload -o
```
### 并发200循环10次
ThinkPHP


```
Transactions:               1655 hits
Availability:              82.75 %
Elapsed time:               8.21 secs
Data transferred:         173.62 MB
Response time:              0.69 secs
Transaction rate:         201.58 trans/sec
Throughput:            21.15 MB/sec
Concurrency:              139.29
Successful transactions:        1655
Failed transactions:             345
Longest transaction:            7.83
Shortest transaction:           0.00
```
Laravel:


```
Transactions:               1520 hits
Availability:              76.00 %
Elapsed time:              34.95 secs
Data transferred:         159.45 MB
Response time:              3.15 secs
Transaction rate:          43.49 trans/sec
Throughput:             4.56 MB/sec
Concurrency:              136.84
Successful transactions:        1520
Failed transactions:             480
Longest transaction:           19.18
Shortest transaction:           0.00
```
Yii:


```
Transactions:               1704 hits
Availability:              85.20 %
Elapsed time:              15.16 secs
Data transferred:         178.76 MB
Response time:              1.46 secs
Transaction rate:         112.40 trans/sec
Throughput:            11.79 MB/sec
Concurrency:              164.21
Successful transactions:        1704
Failed transactions:             296
Longest transaction:            9.04
Shortest transaction:           0.00
```
### 比较奇怪的一点，当框架关掉调试模式后都出现了较多失败的情况。不过明显的看到，Laravel进行简单的优化后，性能大大的提升了，但还是低于其他两个框架。
# 结论
在测试的过程中，其他我的心情是沮丧的，感觉自己的三观世界已经崩塌了，崩塌至渣。
ThinkPHP的性能比Yii高出一倍，比Laravel高出近四倍 Yii的性能比较中庸，但是在测试中会明显发现请求失败的情况比其他两个框架要少 Laravel依旧优雅，不过性能堪忧，用网友的话形容比较好 `学之者生用之者死`
# 最后
我的项目最后使用了自建框架进行开发，得益于composer的强大，开发的过程非常顺利，最后附上自有框架在200并发10次循环的数据，框架开源但是更新比较忙，欢迎在github上搜索PPPHP，和我一起搭建框架。


```
Transactions:               1672 hits
Availability:              83.60 %
Elapsed time:               6.18 secs
Data transferred:         175.40 MB
Response time:              0.57 secs
Transaction rate:         270.55 trans/sec
Throughput:            28.38 MB/sec
Concurrency:              153.16
Successful transactions:        1672
Failed transactions:             328
Longest transaction:            4.57
Shortest transaction:           0.01
```
