# Logstash将日志产生时间替换@timestamp - 零度的博客专栏 - CSDN博客
2018年07月02日 19:13:21[零度anngle](https://me.csdn.net/zmx729618)阅读数：3600
## 一、跟着官网学习一下date插件
      日期过滤器用于从字段中解析日期，然后使用该日期或时间戳作为事件的logstash时间戳。例如，syslog事件通常具有这样的时间戳：
Bash
```bash
"Apr 17 09:32:01"
```
       你可以使用日期格式MMM dd HH：mm：ss来解析这个。日期过滤器对于排序事件和回填旧数据尤为重要。 如果您在活动中没有找到正确的日期，那么稍后搜索它们可能会排序错乱。在没有这个过滤器的情况下，logstash会根据第一次看到事件（在输入时间），如果时间戳还没有在事件中设置，选择一个时间戳。 例如，在文件输入的情况下，时间戳被设置为每次读取的时间。
### 1.1 Date过滤器配置选项
       该插件支持以下配置选项和稍后介绍的通用选项。
#### locale：
      值类型是字符串,这个设置没有默认值。使用IETF-BCP47或POSIX语言标记指定用于日期分析的区域设置。 简单的例子是en，美国的BCP47或者en_US的POSIX。大多数情况下需要设置语言环境来解析月份名称（MMM模式）和星期几名称（EEE模式）。如果未指定，则将使用平台默认值，但对于非英文平台默认情况下，英文解析器也将用作回退机制。
#### match：
      值类型是数组,默认值是[]。首先有字段名称的数组，格式模式如下，[field，formats ...]如果你的时间字段有多种可能的格式，你可以这样做：
Bash
```bash
match => [ "logdate", "MMM dd yyyy HH:mm:ss",
          "MMM  d yyyy HH:mm:ss", "ISO8601" ]
```
       #以上将匹配一个系统日志（rfc3164）或iso8601时间戳。
       有一些特殊的例外。 以下格式文字可帮助您节省时间并确保日期解析的正确性。
Bash
```bash
ISO8601 - 应解析任何有效的ISO8601时间戳，如2011-04-19T03:44:01.103Z
UNIX - 将解析float或int值，表示自1346149001.132以及1326149001.132以来的秒数（以秒为单位）
UNIX_MS - 将分析int值表示unix时间（以毫秒为单位），如1366125117000
TAI64N - 将解析tai64n时间值
```
        例如，如果您有一个字段logdate，值类似于Aug 13 2010 00:03:44，您可以使用此配置：
Bash
```bash
filter {
  date {
    match => [ "logdate", "MMM dd yyyy HH:mm:ss" ]
  }}
```
      如果您的字段嵌套在结构中，则可以使用嵌套语法[foo] [bar]来匹配其值。 有关更多信息，请参阅字段参考[https://www.elastic.co/guide/en/logstash/current/event-dependent-configuration.html#logstash-config-field-references](https://www.elastic.co/guide/en/logstash/current/event-dependent-configuration.html#logstash-config-field-references)
有关语法的更多细节 :
       用于解析日期和时间文本的语法使用字母来指示时间值（月，分等）的类型，以及重复的字母来表示该值的形式（2位月份，全月份名称等）。以下是可用于解析日期和时间的内容：
y year：
Bash
```bash
yyyy  #全年号码。 例如：2015。
yy    #两位数年份。 例如：2015年的15。
```
M month of the year :
Bash
```bash
M     #最小数字月份。 例如：1 for January and 12 for December.。
MM    #两位数月份。 如果需要，填充零。 例如：01 for January  and 12 for Decembe
MMM   #缩短的月份文本。 例如： Jan for January。 注意：使用的语言取决于您的语言环境。 请参阅区域设置以了解如何更改语言。
MMMM  #全月文本，例如：January。 注意：使用的语言取决于您的语言环境。
```
d day of the month :
Bash
```bash
d   #最少数字的一天。 例如：1月份的第一天1。
dd  #两位数的日子，如果需要的话可以填零.例如：01 for the 1st of the month。
```
H  hour of the day (24-hour clock):
Bash
```bash
H   #最小数字小时。 例如:0表示午夜。
HH  #两位数小时，如果需要填零。 例如：午夜00。
```
m minutes of the hour (60 minutes per hour)：
Bash
```bash
m   #最小的数字分钟。 例如：0。
mm  #两位数分钟，如果需要填零。 例如：00。
```
s seconds of the minute (60 seconds per minute) :
Bash
```bash
s    #最小数字秒。 例如：0。
ss   #两位数字，如果需要填零。 例如：00。
```
S 秒的小数部分最大精度是毫秒（SSS）。 除此之外，零附加。
Bash
```bash
S    #十分之一秒。例如：0为亚秒值012
SS   #百分之一秒 例如：01为亚秒值01
SSS  #千分之一秒 例如：012为亚秒值012
```
Z 时区偏移或身份
Bash
```bash
Z   #时区偏移，结构为HHmm（Zulu/UTC的小时和分钟偏移量）。例如：-0700。
ZZ  #时区偏移结构为HH:mm（小时偏移和分钟偏移之间的冒号）。 例如：-07：00。
ZZZ  #时区身份。 例如：America/Los_Angeles。 注意：有效的ID在列表中列出http://joda-time.sourceforge.net/timezones.html
```
z 时区名称。 时区名称（z）不能被分析。
w  一年中的一周
Bash
```bash
w  #最小数字周。 例如：1。
ww  #两位数周，如果需要填零。例如：01。
```
D 一年中的一天
e 星期几（数量）
E 一周中的天(文本)
Bash
```bash
E,EE,EEE  #星期几缩写。 例如：Mon, Tue, Wed, Thu, Fri, Sat, Sun。 注意：这个的实际语言将取决于您的语言环境。
EEEE   #一周中的全文。 例如：Monday, Tuesday，...注意：这个的实际语言将取决于您的语言环境。
```
#对于非格式化语法，您需要在值的周围放置单引号字符。 例如，如果你解析ISO8601时间，“2015-01-01T01:12:23”这个小小的“T”不是一个有效的时间格式，而你想说“从字面上看，一个T”，你的格式将是 这个：“yyyy-MM-dd’T'HH:mm:ss”
#其他不太常见的日期单位，如时代（G），世纪（C），上午/下午（一），和更多，可以了解joda-time文档:[http://www.joda.org/joda-time/key_format.html](http://www.joda.org/joda-time/key_format.html)
#### tag_on_failure :
值类型是数组，默认值是[“_dateparsefailure”]。没有成功匹配时，将值追加到标签字段
#### target :
值类型是字符串，默认值是“@timestamp”。将匹配的时间戳存储到给定的目标字段中。 如果未提供，则默认更新事件的@timestamp字段。
#### timezone：
值类型是字符串,这个设置没有默认值。指定要用于日期分析的时区标准ID。有效的ID列在Joda.org可用时区页面上。如果时区无法从数值中提取，那么这非常有用，并且不是平台默认值。 如果没有指定，将使用平台默认值。 Canonical ID是很好的，因为它为您节省了夏令时。例如，America/Los_Angeles或Europe/Paris是有效的ID。该字段可以是动态的，并使用％{field}语法包含事件的一部分
### 2.2 常规选项
![图片.png](http://www.51niux.com/zb_users/upload/2017/12/201712201513738926878315.png)
#前面翻译好多次了这里就不翻译了。
博文来自：www.51niux.com
## 二、来走一下date的小例子
### 2.1 先不加date过滤插件查看一下logstash的输出：
#这里搞了个三月份的旧日志来测试一下
#下面是stdout { codec => rubydebug}的输出：
Bash
```bash
"@timestamp" => 2017-12-20T08:39:07.982Z,
                    "request_time" => "0.040",
             "nginx.access.method" => "GET",
               "nginx.access.time" => "14/Mar/2017:00:00:02 +0800",
```
#如果不用 date 插件，那么 Logstash 将处理时间作为时间戳。时间戳字段是 Logstash 自己添加的内置字段 @timestamp，也就是说如果你没有设置date过滤器，时间是因为这个时间是UTC时间，那么timestamp时间呢就是比北京时间采集时间少8个小时，也就是说如果我们的采集时间是16:39:07秒，那么timestamp的时间就是08:39:07。
#下面是es上面的查看：
![图片.png](http://www.51niux.com/zb_users/upload/2017/12/201712201513759266636121.png)
![图片.png](http://www.51niux.com/zb_users/upload/2017/12/201712201513759322986229.png)
![图片.png](http://www.51niux.com/zb_users/upload/2017/12/201712201513762870474303.png)
#可以看到Kibana显示的信息跟ES里面记录的时间是不一致的，因为Logstash采集到数据之后插入到ES中，所以ES中保留的时间也是UTC的时间,创建索引也是按照Logstash里面的时间创建的。但是Kibana呢是根据你当前浏览器的时区然后再在timestamp基础上加减时间的，所以kibana显示的时间是我们以为正确的。
博文来自：www.51niux.com
### 3.2 加上date插件之后再来试一试
#[14/Mar/2017:06:57:12 +0800]  #怎么在logstash里面把这个转换成"2017-11-20 12:00:00"，直接这种格式我是真不会......。在nginx里面还好直接指定日志格式'{"timestamp":"$time_iso8601",'，然后logstash里面通过match => ["timestamp", "ISO8601"]匹配到。
#我这里将日志传送到ES集群里面也不是主要靠filebeat采集数据到Logstash，然后Logstash再进行grok转换成json格式。如果数据很大的话grok转换成json格式挺耗资源的，所以都是事先将日志通过程序转换成json格式。
Bash
```bash
{"agent":"Windows 7","client_time":"2017-11-20 12:00:00","client_ip":"123.10.91.106"}
```
#类似于上面这种格式当然里面的内容要很多啊，我这里特意把日志时间往前调了一个月方便对比哈。
#然后logstash直接通过file的codec => "json"格式采集就可以了，信息都是以json格式存储在logstash本地的。
$ vim /home/elk/logstash/conf/date-test.conf 
Bash
```bash
input {
    file {
         path => "/usr/local/nginx/logs/access.*"
         codec => "json"
    }
}
filter {
    date {
        match => ["client_time", "yyyy-MM-dd HH:mm:ss"]   #这里是如果client_time跟后面的格式匹配上了就会去替换，替换什么呢？target默认指的就是@timestamp,所以就是以client_time的时间更新@timestamp的时间
    }
}
output {
    stdout { codec => rubydebug }
            elasticsearch {
                hosts => ["192.168.14.60","192.168.14.61","192.168.14.62","192.168.14.63","192.168.14.64"]
                index => "logstash-nginx-access-test-51niux-com-%{+YYYY.MM}"
            }
}
```
$ /home/elk/logstash/bin/logstash -f /home/elk/logstash/conf/date-test.conf    #启动logstash，然后插入一条测试数据类似于我上面展示的那种json格式到被采集文件中去
#logstash打印到屏幕的输出查看:
Bash
```bash
{
           "path" => "/usr/local/nginx/logs/access.log",
          "agent" => "Windows 7",
     "@timestamp" => 2017-11-20T04:00:00.000Z,   #从屏幕端的数据可以看到timestamp时间已经不是采集时间而是采集数据里面的日志时间了，只不过将时间转变成了UTC时间。
    "client_time" => "2017-11-20 12:00:00",
       "@version" => "1",
           "host" => "localhost.localdomain",
      "client_ip" => "123.10.91.106"
}
```
#Kibana上面的查看：
![图片.png](http://www.51niux.com/zb_users/upload/2017/12/201712201513772652309289.png)
#注意我特意标红了啊，因为当前是12月份所以你得把时间调整到十一月，也就是上图右上角的地方可以选择时间范围。
#ES上面的索引查看：
![图片.png](http://www.51niux.com/zb_users/upload/2017/12/201712201513772805166855.png)
#这其实挺有用的，因为你日志排序的时候包括分析的时候以及创建索引的时候如果你不用date转换的话，默认都是走的采集时间，如果你是实时日志那倒是差别不是太大，但是采集的日志如果不是很实时呢，比如说是离线转换格式日志，还比如说filebeat会把日志存会再发送等等,那你日志产生的时间戳就不对了，然后呢实现的思路肯定有很多，重点就是date插件这个知识点，了解掌握住就哦了，剩下的就是看需求去嗨了。
