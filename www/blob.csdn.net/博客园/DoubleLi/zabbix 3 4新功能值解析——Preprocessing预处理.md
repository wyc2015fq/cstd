# zabbix 3.4新功能值解析——Preprocessing预处理 - DoubleLi - 博客园






Zabbix 3.4版本更新了许多新功能，其中一个监控项功能Preprocessing，根据官方说明文档，在监控项收集的数据存储到数据库前，预先对数据进行处理，使用效果超过预期。这个功能存放位置在创建item后多了一个Preprocessing选项卡，截图如下



![技术分享](https://s5.51cto.com/oss/201710/19/f484255df77647d70b3a98915c5649ab.png)

简单解析下Preprocessing菜单：
|**转换**|**描述**|
|----|----|
|*Custom multiplier*|将值乘以指定的整数或浮点值。使用此选项将以KB，MBps等接收的值转换为B，Bps，否则Zabbix无法正确设置前缀（K，M，G等）。从Zabbix 2.2开始，也支持使用科学符号。 例如。 1e + 70。|
|*Right trim*|从值的末尾删除指定的字符。|
|*Left trim*|从值的起始处删除指定的字符。|
|*Trim*|从值的起始和结尾删除指定的字符。|
|*Regular expression*|将值与<pattern>正则表达式匹配，并用<output>替换值。 正则表达式支持用\N序列提取最多10个捕获的组。\ 参数:**pattern** - 正则表达式**output** - 输出格式化模板。一个\N（其中N = 1 … 9）转义序列被替换为第N个匹配组。|
|*Boolean to decimal*|将值从布尔格式转换为十进制。文本表示被转换为0或1.因此，“TRUE”存储为1，“FALSE”存储为0.所有值都以不区分大小写的方式进行匹配。当前被认为的布尔值值如下：*TRUE* - true, t, yes, y, on, up, running, enabled, available*FALSE* - false, f, no, n, off, down, unused, disabled, unavailable 此外，任何非零数值都被认为是TRUE，0被认为是FALSE。|
|*Octal to decimal*|将八进制格式的值转换为十进制。|
|*Hexadecimal to decimal*|将值从十六进制格式转换为十进制。|
|*Delta*|计算当前值和上一个值之间的差值。评估为**value-prev_value**，其中 value - current value; prev_value - 以前收到的值每个项目只允许一个delta操作。|
|*Delta per second*|计算每秒速度的值变化（当前值和上一个值的差值）。 计算为（value-prev_value）/（time-prev_time），其中 value - 当前值; prev_value - 当前收到的值; time - 当前时间戳，prev_time - 以前值的时间戳。 这个设置是非常有用的，以获得每秒不断增长的速度值。 如果当前值小于上一个值，Zabbix将丢弃该差异（不存储）并等待另一个值。这有助于正常工作，例如，32位SNMP计数器的包装（溢出）。*注意*: 由于此计算可能产生浮点数，建议将‘Type  of information‘ 设置为 *Numeric (float)*，即使传入的原始值是整数。  这对于小数部分尤其重要。 如果浮点值很大并且可能超过‘float‘字段长度，在这种情况下，整个值可能会丢失，实际上建议使用Numeric（无符号），因此只会修剪小数部分。 每个监控项只允许一个delta操作。|

## 1.1        正则表达式

1、选择字符或文本监控项

选择了收集系统CPU信息的监控项作为实验操作对象。未做处理时原始值为字符串

![技术分享](https://s2.51cto.com/oss/201710/19/9f421c2da9b90f2e9f733a12ff302436.png)

![技术分享](https://s2.51cto.com/oss/201710/19/dca71d0e179739f48effe9c294d49019.png)

2、现在把这个监控项的Preprocessing名称下拉菜单选择为正则表达式

![技术分享](https://s4.51cto.com/oss/201710/19/e1be2f2f59bb6a03b42b66a3271a8e97.png)

3、根据官方说明信息，监控项收集字符串如果符合^processor正则表达式的就会被替换为test，等待一段时候后结果符合预期

![技术分享](https://s2.51cto.com/oss/201710/19/d444a5b00d52bb01ab830d0570298bf7.png)

4、正则表达式还可以使用\N（N=1……9）序列方式，格式化输出替换的模版，大大丰富的正则替换的类型，并可以重组替换的输出值。下面修改正则表达式使用序列方式自定义输出值

![技术分享](https://s3.51cto.com/oss/201710/19/160021ab46736b0f25055c23c88f6cf4.png)

5、正则表达式为(GenuineIntel).*(working)，替换值为 \1 test \2，第一个括号的正则表达式代表第一个序列，第二个括号的正则表达式代表第二个序列，替换值分别使用\1和\2代表，不过必须要注意去掉括号后，整个正则表达式必须匹配监控项值的一部分，否则得出的结果为空值。

![技术分享](https://s4.51cto.com/oss/201710/19/73f20bf5d8e4773675ba41ce1b84e519.png)

得出的结果符合预期。



6、Proprocessing还有一个特别棒的功能，被转换了一次的数值可以在被下一层继续处理，如果熟悉使用过shell命令的管道符的话，对于这个就不难理解。如下图，增加多一层转换

![技术分享](https://s4.51cto.com/oss/201710/19/8b48924bccf69910dac8778129c49c6c.png)

结果为

![技术分享](https://s3.51cto.com/oss/201710/19/1deeac43f6b90b41084a3496ae0cd1f0.png)

7、可以在添加一层转换，使用\d+匹配其中的数字

![技术分享](https://s4.51cto.com/oss/201710/19/3542a0d282b91a80c6519e1aacce08c7.png)

结果为

![技术分享](https://s3.51cto.com/oss/201710/19/22a3556f87e4003388421ca08fddba14.png)

## 1.2        自定义倍数

1、根据上面得出的数字，使用Custom multiplier自定义倍数转换，可获得预期值

![技术分享](https://s5.51cto.com/oss/201710/19/8b4248c2b19f50af041e469ad7b78b04.png)

结果为

![技术分享](https://s1.51cto.com/oss/201710/19/0670a25ab3f3b6dbb59fe18adfbbbd78.png)

## 1.3        过滤尾字符

1、从值的末尾删除指定的字符

![技术分享](https://s2.51cto.com/oss/201710/19/358a405c4d980f931aa5e3edfeb6b1dc.png)

结果为

![技术分享](https://s2.51cto.com/oss/201710/19/0584a0945baf0b4c12fb12180954f454.png)

## 1.4        过滤首字符

1、从值的起始处删除指定的字符

![技术分享](https://s1.51cto.com/oss/201710/19/5c55063f4e2f8e606800770677545acd.png)

结果为

![技术分享](https://s4.51cto.com/oss/201710/19/a691bc1d5dc1364154c36d8a4b011caa.png)

注：其它的转换处理根据表格说明自行验证

## 1.5        存储值转换

      有意思是这些处理是在数据储存到数据库前的操作，而触发器的触发操作依据是数据库中储存的值，所以这一串处理储存到数据库的值是整数值类型的8，可以为这个监控项配置一个判断大于8等类型的触发器。



![技术分享](https://s1.51cto.com/oss/201710/19/26aceab7276903f9a7c50c2d274d67ed.png)

![技术分享](https://s2.51cto.com/oss/201710/19/1ff75b53ea77450be9186af48a7ccddd.png)

## 1.6        总结语

    preprocessing功能非常实用，如某些品牌的网络设备，使用snmp方式监控并收集回来的数据，由于厂商预先进行了处理，导致网络流量的值为4.5MB或者56KB等形式，人性化的表现形式，而对于3.4以前的zabbix来说，整数或浮点数后面多一两个字符，这整串数据就不能以整数值或者浮点数配置监控项的数据库收集类型，必须使用文本字符串类型，而文本和字符串类型监控项触发器不能使用数值监控类型的判断并触发流量报警。要变成数字类型，唯一的方式是使用外部检查预先编写字符串处理脚本，但增加的配置监控项的繁琐程度，且外部检查降低了zabbix的性能，3.4的preprocessing功能使得问题解决，并更加灵活的处理监控数据。









