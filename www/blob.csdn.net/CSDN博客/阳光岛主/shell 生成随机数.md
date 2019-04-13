
# shell 生成随机数 - 阳光岛主 - CSDN博客

2016年05月26日 09:29:54[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：4710


**1. 使用系统的 $RANDOM 变量**
|1
|2
|mimvp@ubuntu:~$|echo|$RANDOM
|17617
|

$RANDOM 的范围是[0, 32767]
如需要生成超过32767的随机数，可以用以下方法实现。
例：生成400000~500000的随机数
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
|\#!/bin/bash
|function|rand(){
|min=$1
|max=$(($2-$min+1))
|num=$(($RANDOM+1000000000))|\#增加一个10位的数再求余
|echo|$(($num%$max+$min))
|}
|rnd=$(rand 400000 500000)
|echo|$rnd
|exit|0
|

**2. 使用date +%s%N**
例：生成1~50的随机数
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
|\#!/bin/bash
|function|rand(){
|min=$1
|max=$(($2-$min+1))
|num=$(|date|+%s%N)
|echo|$(($num%$max+$min))
|}
|rnd=$(rand 1 50)
|echo|$rnd
|exit|0
|

**3. 使用/dev/random 和 /dev/urandom**
/dev/random 存储着系统当前运行环境的实时数据，是阻塞的随机数发生器，读取有时需要等待。
/dev/urandom 非阻塞随机数发生器，读取操作不会产生阻塞。
例：使用/dev/urandom生成100~500的随机数，使用urandom避免阻塞。
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
|\#!/bin/bash
|function|rand(){
|min=$1
|max=$(($2-$min+1))
|num=$(|cat|/dev/urandom|||head|-n 10 | cksum ||awk|-F|' '|'{print $1}'|)
|echo|$(($num%$max+$min))
|}
|rnd=$(rand 100 500)
|echo|$rnd
|exit|0
|

**4. 使用 linux uuid**
uuid 全称是通用唯一识别码，格式包含32个16进制数字，以’-‘连接号分为5段。形式为8-4-4-4-12 的32个字符。
|1
|2
|mimvp@ubuntu:~|/shell|$|cat|/proc/sys/kernel/random/uuid
|fd496199-372a-403e-8ec9-bf4c52cbd9cd
|

例：使用linux uuid 生成100~500随机数
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
|\#!/bin/bash
|function|rand(){
|min=$1
|max=$(($2-$min+1))
|num=$(|cat|/proc/sys/kernel/random/uuid|| cksum ||awk|-F|' '|'{print $1}'|)
|echo|$(($num%$max+$min))
|}
|rnd=$(rand 100 500)
|echo|$rnd
|exit|0
|

**5. 生成随机字符串**
例：生成10位随机字符串
|1
|2
|3
|4
|5
|\#使用date 生成随机字符串
|date|+%s%N | md5sum ||head|-c 10
|\#使用 /dev/urandom 生成随机字符串
|cat|/dev/urandom|||head|-n 10 | md5sum ||head|-c 10

|



