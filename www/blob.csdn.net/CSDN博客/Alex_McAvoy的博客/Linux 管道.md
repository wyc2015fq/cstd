# Linux 管道 - Alex_McAvoy的博客 - CSDN博客





2018年07月08日 22:34:21[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：57
个人分类：[Linux 学习笔记](https://blog.csdn.net/u011815404/article/category/7715914)









# 【概述】



管道是一种通信机制，通常用于进程间的通信，它表现出来的形式就是将前面每一个进程的输出（stdout）直接作为下一个进程的输入（stdin）。

管道分为匿名管道和具名管道。

匿名管道，通常在过滤程序时使用时，在命令行中由 | 分隔符表示。

具名管道，就是有名字的管道，通常只会在源程序使用。

常用的可以使用管道的过滤程序有：cut 打印命令、grep 匹配命令、wc 计数命令、sort 排序命令、uniq 去重命令等等。

# 【cut 命令】



1.作用：cut 命令常用来显示文件中的内容，使用不同的参数可以用来显示指定部分。

2.格式：cut [选项] 文件名

3.常用选项
|-f|显示指定范围的字段|
|----|----|
|-b|显示行中指定范围的字节|
|-c|显示行中指定范围的字符|
|-d|指定字段的分隔符，默认的字段分隔符为 Tab|
|--complement|补足被选择的字节、字符或字段|
|--out-delimiter=<字段分隔符>|指定输出内容是的字段分割符|

注：



1）字符字段的记法：

① N-：从第N个字节、字符、字段到结尾

② N-M：从第N个字节、字符、字段到第M个（包括M在内）字节、字符、字段

③ -M：从第1个字节、字符、字段到第M个（包括M在内）字节、字符、字段

2）当文件较大时，文本在屏幕上迅速闪过（滚屏），用户往往看不清所显示的内容。因此，可以使用管道用more、less等命令分屏显示。

4.实例

1）假设有一个文本，包含如下信息

![](https://img-blog.csdn.net/2018070820511378?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

① 使用 -f 选项提取指定字段

![](https://img-blog.csdn.net/20180708205047153?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180708205156162?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

② 使用 -c 选项提取行中指定范围的字符

![](https://img-blog.csdn.net/20180708210249558?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180708210259584?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

2）使用管道，结合 ls 命令的应用：打印主目录下的以空格为分隔符的第一字段的内容

![](https://img-blog.csdn.net/20180708211751990?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 【wc 命令】

1.作用：wc 命令用于统计并输出一个文件中行、单词和字节的数目。

2.格式：**wc [选项] 文件名**

3.常用选项：
|-m|统计字符数|
|----|----|
|-c|统计字节数|
|-w|统计单词数|
|-l|统计行数|
|-L|统计最长行字节数|

**注：对于西文字符来说，一个字符就是一个字节，但对于中文字符一个汉字是大于2个字节的，其具体数目是由字符编码决定的。**

4.实例

1）假设有一个文本，包含如下信息

![](https://img-blog.csdn.net/2018070820511378?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

①统计字符数

![](https://img-blog.csdn.net/20180708212747469?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

② 统计行数

![](https://img-blog.csdn.net/20180708212552554?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

2）使用管道，结合 ls 命令的应用：统计 /etc 下面所有目录数

![](https://img-blog.csdn.net/2018070821285414?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 【sort 命令】



1.作用：将输入按照一定方式排序，然后再输出，它支持的排序方式有按字典排序、数字排序、按月份排序、随机排序、反转排序、指定特定字段排序等等。

2.格式：**sort [选项] 待排序的文件列表**

3.常用选项：
|-r|指定为反转排序|
|----|----|
|-n|指定为按数值大小排序|
|-c|检查文件是否已经按照顺序排序|
|-b|忽略每行前面开始出的空格字符|
|-t <分隔符>|指定排序时所用的分隔符|
|-k <字段号>|指定对哪一字段排序|
|-o <输出文件>|将排序后的结果存入指定的文件|

**注：sort 默认为字典序排序**

4.实例

![](https://img-blog.csdn.net/2018070821584754?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 【uniq 命令】

1.作用：uniq 命令用于过滤或输出文件中的重复行，一般与 sort 命令结合使用。

2.格式：**uniq [选项] 文件名**

3.常用选项：


|-c|在每列旁边显示该行重复出现的次数|
|----|----|
|-d|仅显示重复出现的行列|
|-u|仅显示出一次的行列|
|-w<字符位置>|指定要比较的字符|
|-s<字符位置>|忽略比较指定的字符|
|-f<栏位>|忽略比较指定的栏位|

4.实例：

假设有一个文本，包含如下信息

![](https://img-blog.csdn.net/2018070820511378?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

1）删除重复行

![](https://img-blog.csdn.net/20180708223953778?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

2）在文件中找出重复的行

![](https://img-blog.csdn.net/20180708224156287?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

3）统计各行在文件中出现的次数

![](https://img-blog.csdn.net/2018070822425564?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



