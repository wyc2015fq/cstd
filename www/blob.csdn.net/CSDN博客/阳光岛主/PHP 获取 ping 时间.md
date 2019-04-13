
# PHP 获取 ping 时间 - 阳光岛主 - CSDN博客

2015年06月30日 15:58:17[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：5232



PHP 可以通过exec函数执行shell命令，来获取ping时间。
本文示例代码，请见**[米扑博客](http://blog.mimvp.com/)**：[PHP 获取ping时间](http://blog.mimvp.com/2015/06/php-gets-the-ping-time/)

**代码示例：**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|33
|34
|echo|'PHP_INT_MAX : '|. PHP_INT_MAX .|"<br><br>"|;
|$ip|=|'115.29.237.28'|;|// IP地址
|if|(PATH_SEPARATOR==|':'|)|// linux
|{
|echo|'I am linux'|.|"<br><br>"|;
|exec|(|"ping -c 3 -w 5 $ip"|,|$info|);
|echo|'linux info : '|.|"<br>"|;
|print_r(|$info|);
|$ping_time_line|=|end|(|$info|);
|echo|'<br><br>ping_time_line : '|.|$ping_time_line|.|"<br><br>"|;
|$ping_time|=|explode|(|"="|,|$ping_time_line|)[1];
|$ping_time_min|=|explode|(|"/"|,|$ping_time|)[0] / 1000.0;
|$ping_time_avg|=|explode|(|"/"|,|$ping_time|)[1] / 1000.0;
|$ping_time_max|=|explode|(|"/"|,|$ping_time|)[2] / 1000.0;
|echo|$ping_time_min|.|" "|.|$ping_time_avg|.|" "|.|$ping_time_max|.|"<br><br>"|;
|}
|else|// windows
|{
|echo|'I am windows'|.|"<br><br>"|;
|exec|(|"ping -c 3 -w 5 $ip"|,|$info|);
|echo|'windows info : '|.|"<br>"|;
|print_r(|$info|);
|$info_time|=|end|(|$info|);
|echo|'<br><br>info_time : '|.|$info_time|.|"<br><br>"|;
|}
|

**运行结果：**
PHP_INT_MAX : 9223372036854775807
I am linux
linux info :
Array ( [0] => PING 115.29.237.28 (115.29.237.28) 56(84) bytes of data. [1] => 64 bytes from 115.29.237.28: icmp_seq=1 ttl=52 time=26.1 ms [2] => 64 bytes from 115.29.237.28: icmp_seq=2 ttl=52 time=27.5 ms [3] => 64 bytes from 115.29.237.28: icmp_seq=3 ttl=52 time=25.2 ms [4] => [5] => — 115.29.237.28 ping statistics — [6] => 3 packets transmitted, 3 received, 0% packet loss, time 2002ms [7] => rtt min/avg/max/mdev = 25.280/26.339/27.590/0.970 ms )
ping_time_line : rtt min/avg/max/mdev = 25.280/26.339/27.590/0.970 ms
0.02528 0.026339 0.02759
**获取Ping时间封装成函数**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|function|ping_time(|$ip|) {
|$ping_cmd|=|"ping -c 3 -w 5 "|.|$ip|;
|exec|(|$ping_cmd|,|$info|);
|$ping_time_line|=|end|(|$info|);
|$ping_time|=|explode|(|"="|,|$ping_time_line|)[1];
|$ping_time_min|=|explode|(|"/"|,|$ping_time|)[0] / 1000.0;
|$ping_time_avg|=|explode|(|"/"|,|$ping_time|)[1] / 1000.0;
|$ping_time_max|=|explode|(|"/"|,|$ping_time|)[2] / 1000.0;
|$result|=|array|();
|$result|[|'ping_min'|] =|$ping_time_min|;
|$result|[|'ping_avg'|] =|$ping_time_avg|;
|$result|[|'ping_max'|] =|$ping_time_max|;
|print_r(|$result|);
|}
|ping_time(|'115.29.237.28'|);
|

**运行结果：**
Array ( [ping_min] => 0.025497 [ping_avg] => 0.025947 [ping_max] => 0.026753 )


