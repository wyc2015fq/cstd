
# 多个变量如何传到awk中 - Augusdi的专栏 - CSDN博客


2015年05月18日 22:47:36[Augusdi](https://me.csdn.net/Augusdi)阅读数：2996


﻿﻿
## 多个变量如何传到awk中
|read a?"   请输入变量A:"
|read b?"   请输入变量B:"
|read c?"   请输入变量C:"
|read d?"   请输入变量D:"
|awk -v a=$a -v b=$b -v c=$c -v d=$d 'BEGIN {printf("%-10s %-15s %-12s %-18s\n",a,b,c,d)}'| tee file1

|



