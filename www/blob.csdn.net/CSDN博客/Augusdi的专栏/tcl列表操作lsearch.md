
# tcl列表操作lsearch - Augusdi的专栏 - CSDN博客


2015年05月07日 23:34:51[Augusdi](https://me.csdn.net/Augusdi)阅读数：1234


﻿﻿
### 名称
### lsearch - 在列表中寻找特定元素
### 语法
### lsearch
### ?
### options
### ?
### list pattern

### 描述
### 这个命令寻找
### list
### 中与
### pattern
### 匹配的元素。如果匹配到了返回第一次找到这个元素的索引（除非指定了-all或-inline），如果没有匹配到返回-1。可选变元指出了列表的元素怎样去匹配
### pattern
### ，可选项如下：
#### 匹配风格可选项
默认的匹配风格为**-glob**，如果给出了多个匹配风格，那么最后给出的匹配风格有效。
**-exact***pattern*是字符串并且必须严格匹配每一个*list*元素。**-glob***pattern*是通配风格匹配每一个列表元素，与**string match**命令类似。**-regexp***pattern*被当作一个正则表达式来匹配列表中的每一个元素，与**re_syntax**命令类似。**-sorted**列表元素被排序，如果指定了这个可选项，**lsearch**将会使用更加有效的查询算法去查列表。如果没有指定其它的可选项，*list*将被排列成升序并且包含ASCII码。这个可选项与**-glob**和**-regexp**互斥，当指定**了-all**或**-not**时与**-exact**非常象。
#### 一般修饰可选项
这些可选项可以在任何一种匹配风格中给出。
**-all**返回一个列表，列表的元素为所有匹配到的索引，返回的索引按照数字顺序排列，如果同时指定了**-inline**返回数值，数值的顺序就是在列表中的顺序。**-inline**返回匹配到的第一个元素。如果**-all**也指定了，则返回一个列表，列表的元素为所有匹配到的数值。**-not**与匹配相反，返回第一个不匹配的数值的索引。**-start***index*从列表的*index*个索引开始搜索。
#### 内容描述可选项
描述如何去解释在列表中寻找到的元素，只有当**-exact**和**-sorted**指定时才有效。如果给出了多个可选项，那么最后给出的可选项有效。默认为-ascii。
**-ascii**列表元素作为Unicode字符串来检查。**-dictionary**列表元素在比较时使用字典关系（查看**lsort**以获取详细描述），只有当**-sorted**可选项指定时才有有效。**-integer**列表中的元素被当作整数来处理。**-nocase**忽略大小写。与**-dictionary**、**-integer**和**-real**搭配无效。**-real**列表中的元素被当作浮点数来处理。
#### 列表排序可选项
这个可选项指定了列表如何排序，只有指定了**-sorted**时才有效。如果给出了多个可选项，那么最后给出的可选项有效。
**-decreasing**列表元素为降序排列，只有指定了**-sorted**时才有效。**-increasing**列表元素为升序排列，只有指定了**-sorted**时才有效。
#### 内嵌列表可选项
这些可选项被用来寻找子列表，可以与任何其它可选项一起使用。
**-index***indexList*这个可选项寻找内嵌的列表，*indexList*变元必须给出一个索引列表（与**lindex**和**lset**类似），*indexList*给出的索引列表在*list*中必须存在对应的索引，否则会出错。**-subindices**这个可选项只返回寻找到的子列表元素，必须和**-index**配合使用。
### 示例
基本搜索：
**lsearch**{a b c d e} c*→**2***lsearch**-all {a b c a b c} c*→**2 5*过滤列表：
**lsearch**-inline {a20 b35 c47} b**→**b35***lsearch**-inline -not {a20 b35 c47} b**→**a20***lsearch**-all -inline -not {a20 b35 c47} b**→**a20 c47***lsearch**-all -not {a20 b35 c47} b**→**0 2*甚至可以做切除操作：
**lsearch**-all -inline -not -exact {a b c a d e a f g a} a*→**b c d e f g*搜索从半途开始：
**lsearch**-start 3 {a b c a b c} c*→**5*搜索子列表中的元素：
**lsearch**-index 1 -all -inline {{a abc} {b bcd} {c cde}} *bc**→**{a abc} {b bcd}*转自：[http://blog.csdn.net/dulixin/article/details/](http://blog.csdn.net/dulixin/article/details/2185458)[2185458](http://blog.csdn.net/dulixin/article/details/2185458)[ ](http://blog.csdn.net/dulixin/article/details/2185458)

