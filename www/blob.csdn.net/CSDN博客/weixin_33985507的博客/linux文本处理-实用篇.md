# linux文本处理-实用篇 - weixin_33985507的博客 - CSDN博客
2017年11月13日 18:00:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
之前梳理了一篇[linux文本处理常用命令--初级](https://www.jianshu.com/p/6a3fdf4af5c9)，本文列举的是日常中使用的文本处理命令。这些命令都是由前文介绍的命令组合而成，从而产生巨大的功用。
### 统计nginx一段访问日志的状态码
```
/var/log/nginx$ sudo cat access.log
127.0.0.1 - - [29/Aug/2017:13:52:24 +0000] "HEAD / HTTP/1.1" 200 0 "-" "curl/7.35.0"
127.0.0.1 - - [29/Aug/2017:14:05:53 +0000] "GET / HTTP/1.1" 200 612 "-" "curl/7.35.0"
192.168.33.1 - - [29/Aug/2017:14:14:54 +0000] "GET / HTTP/1.1" 200 396 "-" "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.101 Safari/537.36"
192.168.33.1 - - [29/Aug/2017:14:14:55 +0000] "GET /favicon.ico HTTP/1.1" 404 208 "http://192.168.33.10/" "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.101 Safari/537.36"
/var/log/nginx$ sudo cat access.log | cut -f 6-100 -d ' ' | cut -f 4 -d ' '|sort | uniq -c
      3 200
      1 404
```
### 查找所有含有'curl'字符串的ini文件
```
$ sudo find / -type f -name '*.ini' | xargs grep 'curl'  # -type f 只查文件类型
/usr/share/php5/curl/curl.ini:extension=curl.so
/var/lib/ucf/cache/:etc:php5:mods-available:curl.ini:extension=curl.so
/etc/php5/fpm/php.ini:[curl]
/etc/php5/fpm/php.ini:;curl.cainfo =
/etc/php5/mods-available/curl.ini:extension=curl.so
/etc/php5/cgi/php.ini:[curl]
/etc/php5/cgi/php.ini:;curl.cainfo =
/etc/php5/cli/php.ini:[curl]
/etc/php5/cli/php.ini:;curl.cainfo =
```
### 切割所需的行
```
$ cat t.txt
序号 心得ID 商品ID 正确答案 选错的订正说明
1 82074 3742295 有效心得 恭喜您答对啦，该心得为有效心得哦~
2 78279 3475124 无效心得 该心得文案描述混乱，且图片不是实拍噢！
3 78265 3808979 无效心得 该心得图片为原商品截图，不是实拍图哦！
4 82095 481963 有效心得 恭喜您答对啦，该心得为有效心得哦~
5 78259 3552015 无效心得 该心得首图不清楚是什么东东噢！
6 78246 2045471 无效心得 该心得图文不符噢！
7 82088 2580621 有效心得 恭喜您答对啦，该心得为有效心得哦~
8 78230 3272946 无效心得 该心得图片为原商品主图，不是实拍图哦！
9 78223 2305965 无效心得 该心得没有关联实物商品噢！
10 78209 2197827 无效心得 该心得文案描述混乱，不知所云噢！
11 82058 4249396 有效心得 恭喜您答对啦，该心得为有效心得哦~
12 78194 2533394 无效心得 该心得图片与描述文案不符噢！
13 78207 3888364 无效心得 该心得图片为原商品主图，不是实拍图哦！
14 82010 2534743 有效心得 恭喜您答对啦，该心得为有效心得哦~
15 78188 2780369 无效心得 该心得图片方向不是正的噢 ！
16 146113 3216467 无效心得 该心得实拍图看不出来实物是什么东东噢！
17 146095 3474798 无效心得 该心得实拍图片与关联的商品不符噢！
18 146090 1978822 无效心得 该心得实拍图片与关联的商品不符噢！
$  grep ' 有效心得' t.txt   | cut -d ' ' -f 2 | paste -s -d ',' -
82074,82095,82088,82058,82010
$  grep ' 有效心得' t.txt   | cut -d ' ' -f 2 | paste -s -d ',' - | pbcopy #复制到剪切板
```
