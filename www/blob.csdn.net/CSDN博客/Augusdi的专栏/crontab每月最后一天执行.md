
# crontab每月最后一天执行 - Augusdi的专栏 - CSDN博客


2015年08月22日 13:15:16[Augusdi](https://me.csdn.net/Augusdi)阅读数：12838


﻿﻿
如何设定crontab在每月最后一天执行一种方法：
for Linux
0 8 28-31 * * [ `date -d tomorrow +\%e` -eq 1 ] && (shell script)
for other Unix,BSD
0 8 28-31 * * [ `echo \`cal\` | awk '{print $NF}'` -eq 1 ] && (shell script)
另一种方法：
单独靠crontab判断比较复杂，所以把判断部分写到执行脚本中
\#!/bin/bash
today=`date +%d`
last_day=`cal | xargs | awk '{print $NF}'`
if [ "$today" != "$last_day" ];then
exit 1
fi
....  \# other codes start from here

通过脚本实现的：
crontab里设定一条：
0 12 28-31 * * sh /[tmp](https://www.baidu.com/s?wd=tmp&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1dWnHf1uAn3myD3mHK-mhmL0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6K1TL0qnfK1TL0z5HD0IgF_5y9YIZ0lQzqlpA-bmyt8mh7GuZR8mvqVQL7dugPYpyq8Q1DdP1RznWfznj6Yrj04nWDdrj0)/start.sh
start.sh 脚本如下：
******************************************************************************
\#!/usr/[bin](https://www.baidu.com/s?wd=bin&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1dWnHf1uAn3myD3mHK-mhmL0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6K1TL0qnfK1TL0z5HD0IgF_5y9YIZ0lQzqlpA-bmyt8mh7GuZR8mvqVQL7dugPYpyq8Q1DdP1RznWfznj6Yrj04nWDdrj0)/ksh
\#this script is used to start the xxx in the last day of every month
ym=`date +%m" "%Y`
if [ `date +%d` = `cal $ym|xargs|awk '{print $NF}'` ]
then
sh yourscripts
fi
********************************************************************************
原理是每月的最后几天（28号到31号）定时执行脚本start.sh来判断当天是否是本月最后一天，如果是执行 yourscripts，不是则退出。
这是五年前为了完成一个特定审计而研究的，当时也是必须在每月的最后一天执行。希望今天仍旧能够帮到你：）

详细请参考：[http://www.codesky.net/article/201109/133201.html](http://www.codesky.net/article/201109/133201.html)

