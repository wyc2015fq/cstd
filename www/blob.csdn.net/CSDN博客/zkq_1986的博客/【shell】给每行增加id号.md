# 【shell】给每行增加id号 - zkq_1986的博客 - CSDN博客





2018年08月29日 16:51:16[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：143








awk -F ',' '{print NR","$0}' 1.txt > 2.txt



