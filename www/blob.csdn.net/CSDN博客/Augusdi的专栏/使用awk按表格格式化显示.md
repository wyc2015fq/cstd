
# 使用awk按表格格式化显示 - Augusdi的专栏 - CSDN博客


2015年06月19日 17:11:35[Augusdi](https://me.csdn.net/Augusdi)阅读数：1554


﻿﻿
本文链接：http://blog.csdn.net/kongxx/article/details/8284952

今天工作中碰到一个问题，需要过滤一些命令行的输出，并且将结果按类似表格的格式输出
这里假定有一个命令的输出如下，假定我们过滤掉了其中某些列或者行，这里我们将其存入test.txt文件
```python
ID   Name    Designation    Department  Salary
100  Thomas  Manager        Sales       $5,000
200  Jason   Developer      Technology  $5,500
300  Sanjay  Sysadmin       Technology  $7,000
400  Nisha   Manager        Marketing   $9,500
500  Randy   DBA            Technology  $6,000
```
下面是一个简单的awk文件，将其保存到test.awk文件里。
```python
function format(value, maxlen, align) {
	len=length(value);
	lendiff=maxlen-len;
	if (lendiff>0) {
		for (k=0; k<lendiff; k++) {
			if(align=="right") {
				value=value" ";
			} else {
				value=" "value;
			}
		}
	}
	return value;
}
BEGIN {
	rows=0;
	cols=5;
}
{
	if(NF==cols) {
		for(i=1; i<=NF; i++) {
			if (length($i) > lengths[i]) lengths[i]=length($i);
		}
		rows++;
		for(col=1; col<=NF; col++) {
			data[rows, col]=$col;
		}
	}
}
END {
	for(i=1; i<=length(lengths); i++) {
		lengths[i]=lengths[i]+4;
	}
	for(i=1; i<=rows; i++) {
		print format(data[i, 1], lengths[1]), format(data[i, 2], lengths[2], "right"), format(data[i, 3], lengths[3], "right"), format(data[i, 4], lengths[4], "right"), format(data[i, 5], lengths[5])
	}
}
```
首先是一个format函数，用来格式化某些字符串，这个函数包含三个参数：第一个参数value是需要格式化的字符串，第二个参数是需要把字符串格式化成多长的字符串，第三个参数是用来表示当字符串长度小于maxlen时，在前面还是在后面补空格。
接下来是BEGIN部分，定义了两个变量，rows是用来表示文本内容共有多少行需要处理，这里考虑到空行问题所以没有使用NR变量；cols是用来定义最终需要显示的列数，这里假定全部列都需要，所以定义成5.
Action部分，处理了所以列是5的的行，这里根据需要可以写自己的过滤条件，上面例子里为了方面没有做过多过滤；这部分使用了两个循环：第一个循环用来计算所以行中每一列的最大长度，并将其保存在lengths数组中；第二个循环是将所以需要的数据放到一个二维数组中；
最后是END部分，此部分也包括两个循环：第一个循环是将每一列的最大长度加4，目的是为了在每一列之间加一些间隔；第二个循环是用来格式化输出的，其中将Name，Designation和Department列左对齐，数字列右对齐。

测试，在命令行运行awk -f test.awk test.txt命令，可以得到如下结果
```python
ID Name       Designation     Department         Salary
    100 Thomas     Manager         Sales              $5,000
    200 Jason      Developer       Technology         $5,500
    300 Sanjay     Sysadmin        Technology         $7,000
    400 Nisha      Manager         Marketing          $9,500
    500 Randy      DBA             Technology         $6,000
```
[http://blog.csdn.net/kongxx/article/details/8284952](http://blog.csdn.net/kongxx/article/details/8284952)


