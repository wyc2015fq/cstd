
# Linux基础：timedatectl命令使用介绍 - 知行合一 止于至善 - CSDN博客

2019年03月11日 21:01:49[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：365


timedatectl是Linux下的一条命令，用于控制系统时间和日期。可以用来查询和更改系统时钟于设定，同时可以设定和修改时区信息。这篇文章简单介绍一下如何使用timedatectl进行常见操作。
# 基础知识
关于RTC/系统时钟/NTP/时区等基本信息和设定的介绍，可参看如下文章，本文不再赘述。
[https://blog.csdn.net/liumiaocn/article/details/79250793](https://blog.csdn.net/liumiaocn/article/details/79250793)
# 常见用法
使用timedatectl可以进行如下常见操作
查看当前时间/日期/时区：timedatectl或者timedatectl status
查看所有可用时区：timedatectl list-timezones
设置时区：timedatectl set-timezone “时区信息”
设置UTC：timedatectl set-timezone UTC
设置时间：timedatectl set-time HH:MM:SS
设置日期：timedatectl set-time YYYY-MM-DD
设置日期时间：timedatectl set-time “YYYY-MM-DD HH:MM:SS”
设置硬件时钟为本地时间：timedatectl set-local-rtc 1
设置硬件时钟为UTC时间：timedatectl set-local-rtc 0
启动NTP时间同步（启用NTP服务或者Chrony服务）：timedatectl set-ntp true
禁用NTP时间同步：timedatectl set-ntp false
# 使用示例
## 查看当前时间/日期/时区
> 命令：timedatectl或者timedatectl status
执行示例
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
# timedatectl status
```
```python
Local time: Fri 2019-03-08 06:07:19 EST
  Universal time: Fri 2019-03-08 11:07:19 UTC
        RTC time: Fri 2019-03-08 09:41:48
       Time zone: America/New_York
```
```python
(
```
```python
EST, -0500
```
```python
)
```
```python
NTP enabled:
```
```python
yes
```
```python
NTP synchronized: no
 RTC
```
```python
in
```
```python
local TZ: no
      DST active: no
 Last DST change: DST ended at
                  Sun 2018-11-04 01:59:59 EDT
                  Sun 2018-11-04 01:00:00 EST
 Next DST change: DST begins
```
```python
(
```
```python
the clock jumps one hour forward
```
```python
)
```
```python
at
                  Sun 2019-03-10 01:59:59 EST
                  Sun 2019-03-10 03:00:00 EDT
```
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
#
```
## 查看所有可用时区
> 命令：timedatectl list-timezones
执行示例
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
# timedatectl list-timezones
```
```python
Africa/Abidjan
Africa/Accra
Africa/Addis_Ababa
Africa/Algiers
Africa/Asmara
Africa/Bamako
Africa/Bangui
Africa/Banjul
Africa/Bissau
Africa/Blantyre
Africa/Brazzaville
Africa/Bujumbura
Africa/Cairo
Africa/Casablanca
Africa/Ceuta
Africa/Conakry
Africa/Dakar
Africa/Dar_es_Salaam
Africa/Djibouti
Africa/Douala
Africa/El_Aaiun
Africa/Freetown
Africa/Gaborone
Africa/Harare
Africa/Johannesburg
Africa/Juba
Africa/Kampala
Africa/Khartoum
Africa/Kigali
lines 1-29
```
## 设置时区
> 命令：timedatectl set-timezone “时区信息”
执行示例
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
# timedatectl set-timezone "Asia/ShangHai"
```
```python
Failed to
```
```python
set
```
```python
time
```
```python
zone: Invalid
```
```python
time
```
```python
zone
```
```python
'Asia/ShangHai'
```
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
# timedatectl set-timezone "Asia/Shanghai"
```
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
# timedatectl
```
```python
Local time: Fri 2019-03-08 19:13:38 CST
  Universal time: Fri 2019-03-08 11:13:38 UTC
        RTC time: Fri 2019-03-08 09:47:38
       Time zone: Asia/Shanghai
```
```python
(
```
```python
CST, +0800
```
```python
)
```
```python
NTP enabled:
```
```python
yes
```
```python
NTP synchronized: no
 RTC
```
```python
in
```
```python
local TZ: no
      DST active: n/a
```
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
#
```
注意可以看到如果时区信息不正确的话会提示Invalid time zone的信息
## 设置UTC
> 命令：timedatectl set-timezone UTC
执行示例
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
# timedatectl
```
```python
Local time: Fri 2019-03-08 19:14:48 CST
  Universal time: Fri 2019-03-08 11:14:48 UTC
        RTC time: Fri 2019-03-08 09:48:42
       Time zone: Asia/Shanghai
```
```python
(
```
```python
CST, +0800
```
```python
)
```
```python
NTP enabled:
```
```python
yes
```
```python
NTP synchronized: no
 RTC
```
```python
in
```
```python
local TZ: no
      DST active: n/a
```
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
# timedatectl set-timezone UTC
```
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
# timedatectl status
```
```python
Local time: Fri 2019-03-08 11:15:05 UTC
  Universal time: Fri 2019-03-08 11:15:05 UTC
        RTC time: Fri 2019-03-08 09:48:59
       Time zone: UTC
```
```python
(
```
```python
UTC, +0000
```
```python
)
```
```python
NTP enabled:
```
```python
yes
```
```python
NTP synchronized: no
 RTC
```
```python
in
```
```python
local TZ: no
      DST active: n/a
```
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
#
```
## 设置时间
> 命令：timedatectl set-time HH:MM:SS
执行示例
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
# timedatectl set-timezone "Asia/Shanghai"
```
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
# date
```
```python
Fri Mar  8 19:15:36 CST 2019
```
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
#
```
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
# timedatectl set-time "20:45:00"
```
```python
Failed to
```
```python
set
```
```python
time: Automatic
```
```python
time
```
```python
synchronization is enabled
```
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
# date
```
```python
Fri Mar  8 19:16:20 CST 2019
```
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
#
```
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
# timedatectl set-ntp false
```
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
# timedatectl set-time "20:45:00"
```
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
# date
```
```python
Fri Mar  8 20:45:01 CST 2019
```
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
#
```
注意: 如果ntp时间同步为true时无法修改时间设定
## 设置日期
> 命令：timedatectl set-time YYYY-MM-DD
执行示例
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
# timedatectl set-ntp false
```
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
# timedatectl set-time 2019-03-10
```
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
# date
```
```python
Sun Mar 10 00:00:01 CST 2019
```
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
#
```
注意: 如果ntp时间同步为true时无法修改时间设定
## 设置日期时间
> 命令：timedatectl set-time “YYYY-MM-DD HH:MM:SS”
执行示例
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
# timedatectl set-ntp false
```
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
# timedatectl set-time "2019-03-11 20:45:00"
```
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
# date
```
```python
Mon Mar 11 20:45:01 CST 2019
```
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
#
```
注意: 如果ntp时间同步为true时无法修改时间设定
## 设置硬件时钟为本地时间
> 命令：timedatectl set-local-rtc 1
执行示例
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
# timedatectl
```
```python
Local time: Mon 2019-03-11 20:58:12 CST
  Universal time: Mon 2019-03-11 12:58:12 UTC
        RTC time: Mon 2019-03-11 12:46:14
       Time zone: Asia/Shanghai
```
```python
(
```
```python
CST, +0800
```
```python
)
```
```python
NTP enabled:
```
```python
yes
```
```python
NTP synchronized:
```
```python
yes
```
```python
RTC
```
```python
in
```
```python
local TZ: no
      DST active: n/a
```
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
# timedatectl set-local-rtc 1
```
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
# timedatectl status
```
```python
Local time: Mon 2019-03-11 20:58:16 CST
  Universal time: Mon 2019-03-11 12:58:16 UTC
        RTC time: Mon 2019-03-11 20:58:16
       Time zone: Asia/Shanghai
```
```python
(
```
```python
CST, +0800
```
```python
)
```
```python
NTP enabled:
```
```python
yes
```
```python
NTP synchronized:
```
```python
yes
```
```python
RTC
```
```python
in
```
```python
local TZ:
```
```python
yes
```
```python
DST active: n/a
Warning: The system is configured to
```
```python
read
```
```python
the RTC
```
```python
time
```
```python
in
```
```python
the local
```
```python
time
```
```python
zone.
         This mode can not be fully supported. It will create various problems
         with
```
```python
time
```
```python
zone changes and daylight saving
```
```python
time
```
```python
adjustments. The RTC
```
```python
time
```
```python
is never updated, it relies on external facilities to maintain it.
         If at all possible, use RTC
```
```python
in
```
```python
UTC by calling
```
```python
'timedatectl set-local-rtc 0'
```
```python
.
```
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
#
```
## 设置硬件时钟为UTC时间
> 命令：timedatectl set-local-rtc 0
执行示例
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
# timedatectl
```
```python
Local time: Mon 2019-03-11 20:59:18 CST
  Universal time: Mon 2019-03-11 12:59:18 UTC
        RTC time: Mon 2019-03-11 20:59:17
       Time zone: Asia/Shanghai
```
```python
(
```
```python
CST, +0800
```
```python
)
```
```python
NTP enabled:
```
```python
yes
```
```python
NTP synchronized:
```
```python
yes
```
```python
RTC
```
```python
in
```
```python
local TZ:
```
```python
yes
```
```python
DST active: n/a
Warning: The system is configured to
```
```python
read
```
```python
the RTC
```
```python
time
```
```python
in
```
```python
the local
```
```python
time
```
```python
zone.
         This mode can not be fully supported. It will create various problems
         with
```
```python
time
```
```python
zone changes and daylight saving
```
```python
time
```
```python
adjustments. The RTC
```
```python
time
```
```python
is never updated, it relies on external facilities to maintain it.
         If at all possible, use RTC
```
```python
in
```
```python
UTC by calling
```
```python
'timedatectl set-local-rtc 0'
```
```python
.
```
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
#
```
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
# timedatectl set-local-rtc 0
```
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
# timedatectl status
```
```python
Local time: Mon 2019-03-11 20:59:27 CST
  Universal time: Mon 2019-03-11 12:59:27 UTC
        RTC time: Mon 2019-03-11 12:59:27
       Time zone: Asia/Shanghai
```
```python
(
```
```python
CST, +0800
```
```python
)
```
```python
NTP enabled:
```
```python
yes
```
```python
NTP synchronized:
```
```python
yes
```
```python
RTC
```
```python
in
```
```python
local TZ: no
      DST active: n/a
```
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
#
```
## 启动NTP时间同步（启用NTP服务或者Chrony服务）
> 命令：timedatectl set-ntp true
执行示例
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
# timedatectl status
```
```python
Local time: Mon 2019-03-11 21:00:19 CST
  Universal time: Mon 2019-03-11 13:00:19 UTC
        RTC time: Mon 2019-03-11 13:00:19
       Time zone: Asia/Shanghai
```
```python
(
```
```python
CST, +0800
```
```python
)
```
```python
NTP enabled: no
NTP synchronized:
```
```python
yes
```
```python
RTC
```
```python
in
```
```python
local TZ: no
      DST active: n/a
```
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
# timedatectl set-ntp true
```
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
# timedatectl status
```
```python
Local time: Mon 2019-03-11 21:00:24 CST
  Universal time: Mon 2019-03-11 13:00:24 UTC
        RTC time: Mon 2019-03-11 13:00:24
       Time zone: Asia/Shanghai
```
```python
(
```
```python
CST, +0800
```
```python
)
```
```python
NTP enabled:
```
```python
yes
```
```python
NTP synchronized: no
 RTC
```
```python
in
```
```python
local TZ: no
      DST active: n/a
```
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
#
```
## 禁用NTP时间同步
> 命令：timedatectl set-ntp false
执行示例
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
# timedatectl status
```
```python
Local time: Mon 2019-03-11 21:00:45 CST
  Universal time: Mon 2019-03-11 13:00:45 UTC
        RTC time: Mon 2019-03-11 13:00:44
       Time zone: Asia/Shanghai
```
```python
(
```
```python
CST, +0800
```
```python
)
```
```python
NTP enabled:
```
```python
yes
```
```python
NTP synchronized:
```
```python
yes
```
```python
RTC
```
```python
in
```
```python
local TZ: no
      DST active: n/a
```
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
# timedatectl set-ntp false
```
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
# timedatectl status
```
```python
Local time: Mon 2019-03-11 21:00:50 CST
  Universal time: Mon 2019-03-11 13:00:50 UTC
        RTC time: Mon 2019-03-11 13:00:50
       Time zone: Asia/Shanghai
```
```python
(
```
```python
CST, +0800
```
```python
)
```
```python
NTP enabled: no
NTP synchronized:
```
```python
yes
```
```python
RTC
```
```python
in
```
```python
local TZ: no
      DST active: n/a
```
```python
[
```
```python
root@platform ~
```
```python
]
```
```python
#
```
# 参考文章
[https://blog.csdn.net/liumiaocn/article/details/79250793](https://blog.csdn.net/liumiaocn/article/details/79250793)

