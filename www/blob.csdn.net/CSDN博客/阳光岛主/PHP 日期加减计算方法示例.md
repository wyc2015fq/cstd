
# PHP 日期加减计算方法示例 - 阳光岛主 - CSDN博客

2018年02月28日 13:00:17[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：7420


PHP 标准的日期格式
date("Y-m-d H:i:s");
**PHP 简单的日期加减计算**
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
|<?php
|date_default_timezone_set(|'PRC'|);|//默认时区
|echo|"今天:"|,|date|(|"Y-m-d"|,time()),|"\n"|;
|echo|"今天:"|,|date|(|"Y-m-d"|,|strtotime|(|"18 june 2008"|)),|"\n"|;
|echo|"昨天:"|,|date|(|"Y-m-d"|,|strtotime|(|"-1 day"|)),|"\n"|;
|echo|"明天:"|,|date|(|"Y-m-d"|,|strtotime|(|"+1 day"|)),|"\n"|;
|echo|"一周后:"|,|date|(|"Y-m-d"|,|strtotime|(|"+1 week"|)),|"\n"|;
|echo|"一周零两天四小时两秒后:"|,|date|(|"Y-m-d G:H:s"|,|strtotime|(|"+1 week 2 days 4 hours 2 seconds"|)),|"\n"|;
|echo|"下个星期四:"|,|date|(|"Y-m-d"|,|strtotime|(|"next Thursday"|)),|"\n"|;
|echo|"上个周一:"|.|date|(|"Y-m-d"|,|strtotime|(|"last Monday"|)).|"\n"|;
|echo|"一个月前:"|.|date|(|"Y-m-d"|,|strtotime|(|"last month"|)).|"\n"|;
|echo|"一个月后:"|.|date|(|"Y-m-d"|,|strtotime|(|"+1 month"|)).|"\n"|;
|echo|"十年后:"|.|date|(|"Y-m-d"|,|strtotime|(|"+10 year"|)).|"\n"|;
|?>
|

**运行结果：**
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
|今天:2017-11-06
|今天:2008-06-18
|昨天:2017-11-05
|明天:2017-11-07
|一周后:2017-11-13
|一周零两天四小时两秒后:2017-11-15 22:22:42
|下个星期四:2017-11-09
|上个周一:2017-10-30
|一个月前:2017-10-06
|一个月后:2017-12-06
|十年后:2027-11-06
|

**PHP 进阶的日期加减计算**
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
|<?php
|// 默认时区
|date_default_timezone_set(|'PRC'|);
|echo|"今天: "|,|date|(|'Y-m-d H:i:s'|),|"\n"|;|// 输出当前时间
|echo|"明天: "|,|date|(|'Y-m-d H:i:s'|,|strtotime|(|'+1 day'|)),|"\n"|;|// 输出明天时间
|//strtotime 可以接受第二个参数，类型timestamp为指定日期
|echo|date|(|'Y-m-d H:i:s'|,|strtotime|(|"+1 day"|,|strtotime|(|'2017-11-11'|))),|"\n"|;

|// 这里+1 day 可以修改参数1为任何想需要的数
|// day也可以改成year（年）month（月）hour（小时）minute（分）second（秒）
|echo|date|(|'Y-m-d H:i:s'|,|strtotime|(|"+1 day +1 hour +1 minute"|)),|"\n"|;
|?>
|

运行结果：
今天: 2017-11-06 18:58:04
明天: 2017-11-07 18:58:04
2017-11-12 00:00:00
2017-11-07 19:59:04
**PHP 高级的计算日期时间差**
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
|35
|<?php
|// 默认时区
|date_default_timezone_set(|'PRC'|);
|echo|date|(|"Y-m-d H:i:s"|),|"\n"|;
|// 日期天数相加函数
|echo|date|(|'Y-m-d'|,|strtotime|(|'+1 day'|,|strtotime|(|'2016-09-12'|))),|"\n\n"|;
|echo|strtotime|(|'2016-09-12'|),|"\n"|;
|echo|date|(|"Y-m-d"|,|'1473609600'|),|"\n"|;
|echo|date|(|"Y-m-d"|,|'1573609600'|),|"\n"|;
|// 日期天数相加函数
|$d|=|"2016-09-12 10:12:20"|;
|echo|date|(|"Y-m-d"|,|strtotime|(|"$d   +1   day"|)),|"\n\n"|;
|// 把日期转换成时间截
|function|date2time(|$d|){
|$year|=((int)|substr|(|"$d"|,0,4));|//取得年份
|$month|=((int)|substr|(|"$d"|,5,2));|//取得月份
|$day|=((int)|substr|(|"$d"|,8,2));|//取得几号
|return|mktime|(0,0,0,|$month|,|$day|,|$year|);
|}
|echo|date2time(|"2016-09-12"|),|"\n\n"|;
|$date_1|=|"2066-09-12"|;
|$date_2|=|"2016-09-12"|;
|$Date_List_a1|=|explode|(|"-"|,|$date_1|);
|$Date_List_a2|=|explode|(|"-"|,|$date_2|);
|$d1|=|mktime|(0,0,0,|$Date_List_a1|[1],|$Date_List_a1|[2],|$Date_List_a1|[0]);
|$d2|=|mktime|(0,0,0,|$Date_List_a2|[1],|$Date_List_a2|[2],|$Date_List_a2|[0]);
|$Days|=|round|((|$d1|-|$d2|)/3600/24);
|echo|"$date_2 -> $date_1 相差: $Days 天"|,|"\n"|;
|?>
|

运行结果：
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
|2017-11-06 19:13:02
|2016-09-13
|1473609600
|2016-09-12
|2019-11-13
|2016-09-13
|1473609600
|2066-09-12 -> 2016-09-12 相差: 18262 天
|




