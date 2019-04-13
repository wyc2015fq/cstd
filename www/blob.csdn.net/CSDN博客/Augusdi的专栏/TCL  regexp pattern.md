
# TCL: regexp pattern - Augusdi的专栏 - CSDN博客


2015年10月13日 17:47:09[Augusdi](https://me.csdn.net/Augusdi)阅读数：1719


﻿﻿
**1. substition and grouping**
{} ""grouping. The difference is that {} prevent substitution,while "" allow substitution.
$varname var substitution
[]command substition
grouping before substitution

**2. Pattern Matching**
%string matcha* alpha
1
%set env(DISPLAY) sunsvr02:0.0
%regexp{([^:]*):} $env(DISPLAY) m1 m2
%set m1  \#all matched part
sunsvr02:
%set m2  \#matched part in ()
sunsvr02
%set cfile asd.c
%regsub{([^\.]*)\.c} $cfile {cc -c & -o \1.o} ccCmd
%set ccCmd
cc -c asd.c -o sad.o
My Comments: & substited by the string $cfile.

