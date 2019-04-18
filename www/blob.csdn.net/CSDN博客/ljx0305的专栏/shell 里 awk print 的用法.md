# shell 里 awk print 的用法 - ljx0305的专栏 - CSDN博客
2012年11月07日 15:57:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：630
个人分类：[Linux](https://blog.csdn.net/ljx0305/article/category/364133)
# [shell 里 awk print 的用法](http://www.cnblogs.com/image-eye/archive/2011/10/31/2230280.html)
首先， 你需要先了解awk 的工作原理:1。AWK读取输入文件一次一行。 2。对于每一行，它匹配在给定的顺序模式，如果匹配，执行相应的动作。 3。如果没有模式匹配，将执行任何行动。 4。在上面的语法，无论是搜索模式，或行动是可选的，但不能同时。 5。如果没有给出搜索模式，然后awk要执行每一行输入给定的行动。 6。如果没有给出动作，打印，这是默认的操作与模式相匹配的所有行。 7。空出的任何行动括号什么都不做。它不会执行默认的打印操作。 8。中的每个行动的声明应该用分号分隔。让我们创建employee.txt文件，其中有
首先， 你需要先了解awk 的工作原理:
1。AWK读取输入文件一次一行。
   2。对于每一行，它匹配在给定的顺序模式，如果匹配，执行相应的动作。
   3。如果没有模式匹配，将执行任何行动。
   4。在上面的语法，无论是搜索模式，或行动是可选的，但不能同时。
   5。如果没有给出搜索模式，然后awk要执行每一行输入给定的行动。
   6。如果没有给出动作，打印，这是默认的操作与模式相匹配的所有行。
   7。空出的任何行动括号什么都不做。它不会执行默认的打印操作。
   8。中的每个行动的声明应该用分号分隔。
让我们创建employee.txt文件，其中有以下内容，这将是在使用
下文提到的例子。
$cat employee.txt
100  Thomas  Manager    Sales       $5,000
200  Jason   Developer  Technology  $5,500
300  Sanjay  Sysadmin   Technology  $7,000
400  Nisha   Manager    Marketing   $9,500
500  Randy   DBA        Technology  $6,000
Awk的范例1。 awk的默认行为
默认情况下的awk打印文件的每一行。
1234567
$ awk '{print;}' employee.txt
100  Thomas  Manager    Sales       $5,000
200  Jason   Developer  Technology  $5,500
300  Sanjay  Sysadmin   Technology  $7,000
400  Nisha   Manager    Marketing   $9,500
500  Randy   DBA        Technology  $6,000
在上面的例子中模式中没有给出。因此，是适用于所有的行。
action与任何参数的打印输出默认情况下，输出整行.
Awk的范例2。打印与模式匹配的行。
1234$ awk '/Thomas/
> /Nisha/' employee.txt
100  Thomas  Manager    Sales       $5,000
400  Nisha   Manager    Marketing   $9,500
在上面的例子，它打印的“Thomas”或“Nisha”匹配的所有行。它有两种模式。
Awk的范例3。只打印特定的字段​​。
awk有内置的变量。对于每一个记录，即行，分隔空白字符分隔记录默认情况下，它存储在$ n个变量。如果该行有4个词，它会被存储在$ 1，$2，$ 3和$ 4。 $0表示整行。 NF是一个内置变量，它代表这一行有多少个被分隔的域.
12345678910111213$ awk '{print $2,$5;}' employee.txt
Thomas $5,000
Jason $5,500
Sanjay $7,000
Nisha $9,500
Randy $6,000
$ awk '{print $2,$NF;}' employee.txt
Thomas $5,000
Jason $5,500
Sanjay $7,000
Nisha $9,500
Randy $6,000
Awk 范例4. 初始化and Final Action
Awk 两个重要的pattern , 由关键词 BEGIN and END 标识
Syntax:
BEGIN { Actions}
{ACTION} # Action for everyline in a file
END { Actions }
# is for comments in Awk
这是一个例子
12345678910111213
$ awk 'BEGIN {print "Name\tDesignation\tDepartment\tSalary";}
> {print $2,"\t",$3,"\t",$4,"\t",$NF;}
> END{print "Report Generated\n--------------";
> }' employee.txt
Name Designation Department Salary
Thomas   Manager   Sales           $5,000
Jason   Developer   Technology   $5,500
Sanjay   Sysadmin   Technology   $7,000
Nisha   Manager   Marketing   $9,500
Randy   DBA     Technology   $6,000
Report Generated
--------------
在上面的例子，它打印出的报告和最后文件的标题
Awk的范例6。打印技术部门的雇员列表。
部门名称现在​​是第四个字段，所以需要检查与“Techology”的字符串，如果$4 匹配，则打印这一行
$ awk '$4 ~/Technology/' employee.txt
200  Jason   Developer  Technology  $5,500
300  Sanjay  Sysadmin   Technology  $7,000
500  Randy   DBA        Technology  $6,000
操作符〜是正则表达式比较。如果匹配的默认操作，即打印整行.
Awk的范例7。打印在技术部门的雇员人数
下面的例子,  检查  department 是否是Technology, 如果是 则Action中，只增加变量的计数. 这个变量在BEGIN 中被初始化为0
123456
$ awk 'BEGIN { count=0;}
$4 ~ /Technology/ { count++; }
END { print "Number of employees in Technology Dept =",count;}' employee.txt
Number of employees in Tehcnology Dept = 3
在END 中打印出这个变量，就是Technology 部门的员工数目
引用:[http://www.cnblogs.com/image-eye/archive/2011/10/31/2230280.html](http://www.cnblogs.com/image-eye/archive/2011/10/31/2230280.html)
