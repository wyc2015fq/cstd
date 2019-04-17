# Linux 简单文本处理命令 - Alex_McAvoy的博客 - CSDN博客





2018年07月10日 01:22:50[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：234








# 【tr 命令】

1.作用：tr 命令可以对来自标准输入的字符进行替换、压缩和删除。

2.格式：**tr [选项] '字符集1' ['字符集2']**



字符集1：指定要转换或删除的原字符集。当执行转换操作时，必须使用参数“字符集2”指定转换的目标字符集；当执行删除操作时，不需要参数“字符集2”。

字符集2：指定要转换成的目标字符集。

注：tr 命令可以使用的字符类


|[:alnum:]|字母和数字|
|----|----|
|[:alpha:]|字母|
|[:digit:]|数字|
|[:lower:]|小写字母|
|[:upper:]|大写字母|
|[:punct:]|标点符号|
|[:space:]|空白字符|
|[:graph:]|图形字符|
|[:print:]|可打印字符|
|[:cntrl:]|控制（非打印）字符|
|[:xdigit:]|十六进制字符|

3.常见选项：


|-c|取代所有不属于第一字符集的字符|
|----|----|
|-d|删除所有属于第一字符集的字符|
|-s|把连续重复的字符以单独一个字符表示|
|-t|先删除第一字符集较第二字符集多出的字符|

4.实例

1）将输入字符由大写转换为小写

![](https://img-blog.csdn.net/20180710002831568?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

2）删除指定字符

![](https://img-blog.csdn.net/20180710002957265?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

3）将连续的字符去重

![](https://img-blog.csdn.net/20180710003344572?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 【col 命令】

1.作用：在 Linux 说明文件里，都有 RLF 控制字符，当我们运用 shell 特殊字符 > 和 >> ，把说明文件的内容输出成纯文本文件时，控制字符会变成乱码，col 命令能有效滤除这些控制字符。它从标注输入设备读取文本内容，并把内容显示到标注输出设备。

2.格式：**col [选项]**

3.常见选项：


|-b|过滤掉所有的控制字符，包括RLF和HRLF|
|----|----|
|-f|滤掉RLF字符，但允许将HRLF字符呈现出来|
|-x|将 Tab 字符以多个空格字符表示|
|-h|将空格字符转换为 Tab 字符（默认选项）|

4.实例：

在查看 text.txt 中的不可见字符时，会看到很多 ^I ，实际上就是 Tab 转义成可见字符的符号

![](https://img-blog.csdn.net/20180710004738580?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

因此可以使用 col 命令将其转换为可见的空格符号再输出

![](https://img-blog.csdn.net/20180710004753911?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 【join 命令】

1.作用：找出两个文件中，指定栏位内容相同的行，并加以合并，再输出到标准输出设备。

2.格式：join [选项] 文件1 文件2

3.常用选项：


|-t<字符>|指定分隔符（默认为空格）|
|----|----|
|-i|比较栏位内容时，忽略大小写的差异|
|-1<栏位>|连接 [文件1] 指定的栏位|
|-2<栏位>|连接 [文件2] 指定的栏位|
|-a<1或2>|除显示原来的输出内容外，还显示指令文件中没有相同栏位的行|
|-v<1或2>|与-a相同，但是只显示文件中没有相同栏位的行|
|-e<字符串>|若[文件1]与[文件2]中找不到指定的栏位，则在输出中填入选项中的字符串|
|-o<格式>|按照指定的格式来显示结果|

4.实例：

现有两个文件（注意两个文件的不同）

![](https://img-blog.csdn.net/20180710012156517?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://img-blog.csdn.net/20180710012232677?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

1）直接进行合并

![](https://img-blog.csdn.net/20180710012452930?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

2）左外连接，显示左边文件中的所有记录，右边文件中没有匹配的显示空白

![](https://img-blog.csdn.net/20180710012609447?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

3）右外连接，显示右边文件中的所有记录，左边文件中没有匹配的显示空白

![](https://img-blog.csdn.net/20180710012700761?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

4）全连接，显示左边、右边文件中的所有记录

![](https://img-blog.csdn.net/20180710012808943?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

5）显示左边文件中没有匹配的记录

![](https://img-blog.csdn.net/20180710013503680?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

6）显示右边文件中没有匹配的记录

![](https://img-blog.csdn.net/2018071001351415?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

7）显示左边、右边文件中的所有没有匹配的记录

![](https://img-blog.csdn.net/20180710013519851?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

8）指定输出字段（例：参数 -o 1.1 表示只输出第一个文件的第一个字段）

![](https://img-blog.csdn.net/20180710013111421?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

9）指定分隔符

![](https://img-blog.csdn.net/20180710013217205?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



# 【paste 命令】

1.作用：将多个文件按照列队列进行合并。

2.格式：paste [选项] 文件列表

3.常用选项：


|-d<间隔字符>|指定的间隔字符（默认为 Tab）|
|----|----|
|-s|串列进行而非平行处理（即：不合并到一行，每个文件为一行）|

4.实例：

创建三个文件

![](https://img-blog.csdn.net/20180710010709303?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

以空格字符为分隔符将文件合并到一行

![](https://img-blog.csdn.net/20180710010951841?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

以串列合并方式合并

![](https://img-blog.csdn.net/20180710011021365?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



