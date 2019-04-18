# shell学习4 - nosmatch的专栏 - CSDN博客
2011年08月26日 08:48:20[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：402
1,nl
    计算行号过滤器。nl filename将会把filename文件的所有内容都输出到stdout上, 但是会在每个非空行的前面加上连续的行号，如果没有filename参数, 那么就操作stdin，nl命令的输出与cat -n非常相似, 然而, 默认情况下nl不会列出空行。
- [root@localhost shell]# cat 1 
- 1 2 3 
- 
- 2 3 4 
- 
- 3 4 5 
- 4 5 6 
- [root@localhost shell]# cat -n 1 #6行 
-      1  1 2 3 
-      2 
-      3  2 3 4 
-      4 
-      5  3 4 5 
-      6  4 5 6 
- [root@localhost shell]# nl 1 #注意看，只在非空行前面加上数字 
-      1  1 2 3 
- 
-      2  2 3 4 
- 
-      3  3 4 5 
-      4  4 5 6 
- 
- [root@localhost shell]# cat 1 | wc -l # 另外一种处理方法 
- 6 
2，strings
   使用strings命令在二进制或数据文件中找出可打印字符。它将在目标文件中列出所有找到的可打印字符的序列，也可以用来检查一个未知格式的图片文件件。(strings image-file | more可能会搜索出像JFIF这样的字符串, 那么这就意味着这个文件是一个jpeg格式的。
- [root@localhost vuser]# file sed.jpg 
- sed.jpg: JPEG image data, JFIF standard 1.01 
- [root@localhost vuser]# strings sed.jpg | grep JFI* 
- JFIF 
3，basename 和 dirname 
   basename和dirname可以操作任意字符串,简单点说basename截取文件名，去掉路径。而dirname的作用恰好相反。只能说好用.....
- [root@localhost shell]# a=/home/max/shell 
- [root@localhost shell]# basename $a 
- shell 
- [root@localhost shell]# dirname $a 
- /home/max 
4，bc
   bc是个交互式的计算器，当然还有很强大的作用。
- [root@localhost shell]# bc #进入交互模式 
- bc 1.06 
- Copyright 1991-1994, 1997, 1998, 2000 Free Software Foundation, Inc. 
- This is free software with ABSOLUTELY NO WARRANTY. 
- For details type `warranty'. 
- 1*2  #不仅仅支持乘法 
- 2 
- 23%2 #取模也可以 
- 1 
- obase=16 #设置进制 
- 123 #输入123回车后，输出16进制的123 
- 7B 
- 1*23 #输入1*23后，得23,也就是16进制的23 
- 17 
   你要乐意的话也可以采取下面这些方式
- [root@localhost shell]# echo "1+2*5" | bc 
- 11 
- [root@localhost shell]# echo '1+2*5' | bc 
- 11 
- [root@localhost shell]# echo 1+2*5 | bc # 重复了这么多下只是想说明些问题 
- 11 
- [root@localhost shell]# echo 1 + 2 * 5 | bc 
- (standard_in) 1: parse error 
- [root@localhost shell]# echo 1+2 * 5 | bc 
- (standard_in) 1: parse error 
- [root@localhost shell]# echo 1+2*5 | bc 
- 11 
- [root@localhost shell]# echo "2*8/4;1+2*5" | bc 
- 4 
- 11 
- [root@localhost shell]# echo '2*8/4;1+2*5' | bc 
- 4 
- 11 
- [root@localhost shell]# echo 'obase=16; 12' | bc 
- C 
- [root@localhost shell]# echo 'obase=16; (5*2)+1' | bc 
- B 
- [root@localhost shell]# echo 'obase=16;ibase=2;1101' | bc # 这个不难理解，obase说明输出时的进制，ibase说明输入时的进制，这句的意思是将2进制1101转换成16进制 
- D 
5，seq 
      用来生成一系列整数, 用户可以指定生成范围,每个产生出来的整数一般都占一行, 但是可以使用-s选项来改变这种设置。
- [root@localhost shell]# seq 3 # 产生3个数，默认步进是1 
- 1 
- 2 
- 3 
- [root@localhost shell]# seq -s : 3 # 改变分隔符 
- 1:2:3 
- [root@localhost shell]# seq -s $ 3 # 你想用啥就用啥 
- 1$2$3 
- [root@localhost shell]# seq -s 1 3 # 来个数字1 
- 11213 
- [root@localhost shell]# seq -s # 3 # 注意了哦，直接#行不通？为什么呢？ 
- seq：选项需要一个参数 -- s 
- 请尝试执行“seq --help”来获取更多信息。 
- [root@localhost shell]# seq -s /# 3 # 看见了吗？要对#进行转义，或者说让#不能有特殊的含义 
- 1/#2/#3 
- [root@localhost shell]# seq -s '#' 3 
- 1#2#3 
- [root@localhost shell]# seq -s "#" 3 
- 1#2#3 
- [root@localhost shell]# seq -s : 20 5 35 
- 20:25:30:35 
    seq可以用来产生 具有规律的一些文件，想想在foreach操作中是不是经常遇到一连串连续的数字呢？为啥不用seq来减少输入呢？来让我们试试：
- #!/bin/bash
- # 进入特定目录用touch命令创建10个文件
- 
- E_NOARGS=65 
- COUNT=10 
- PREFIX=file 
- if [ -z $1 ] 
- then 
-     echo "Usage: $0 directory"
-     exit $E_NOARGS 
- fi 
- 
- cd $1 
- 
- for filename in `seq $COUNT` 
- do
-   touch $PREFIX.$filename 
- done 
- 
- exit 0 
     shell需要不断的练习。
本文出自 “[alkshao](http://alkshao.blog.51cto.com)” 博客，请务必保留此出处[http://alkshao.blog.51cto.com/1953382/514137](http://alkshao.blog.51cto.com/1953382/514137)
