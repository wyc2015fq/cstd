# shell学习3 - nosmatch的专栏 - CSDN博客
2011年08月26日 08:47:52[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：397标签：[shell																[工作](https://so.csdn.net/so/search/s.do?q=工作&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)
个人分类：[shell](https://blog.csdn.net/HDUTigerkin/article/category/874908)
                
  expr的用法：
  主要通过一些列子来说明
expr 3 + 5 :计算得8，但是若是expr 3+5 则是3+5，没有计算出值
- [max@localhost ~]$ expr 3 + 5 
- 8 
- [max@localhost ~]$ expr 3+5 
- 3+5 
expr 3 \* 5 :做乘法时，要转义。
- [max@localhost ~]$ expr 3 \* 5 
- 15 
- [max@localhost ~]$ expr 3\*5 
- 3*5 
y=`expr $y + 1`等价于let y=y+1 或者 y=$(($y+1))
- [max@localhost ~]$ y=1
- [max@localhost ~]$ expr $y+1  #注意空格
- 1+1 
- [max@localhost ~]$ expr $y + 1 
- 2 
- [max@localhost ~]$ y=y+1 
- [max@localhost ~]$ echo $y #注意直接y=y+1操作时，得到的是字符串。在shell中类似于直接使用y++也不行
- y+1 
- [max@localhost ~]$ y=1
- [max@localhost ~]$ let y=y+1;echo $y 
- 2 
- [max@localhost ~]$ let y=y + 1;echo $y #注意这样有语法错误
- -bash: let: +: syntax error: operand expected (error token is "+") 
- 2 
- [max@localhost ~]$ let y = y + 1;echo $y #同样语法错误
- -bash: let: =: syntax error: operand expected (error token is "=") 
- 2 
- [max@localhost ~]$ echo $y 
- 2 
- [max@localhost ~]$ y=$(($y+1)) 
- [max@localhost ~]$ echo $y 
- 3 
- [max@localhost ~]$ y=$(($y + 1)) #这时候的空格不会出错，但是为了统一，最好不使用空格
- [max@localhost ~]$ echo $y 
- 4 
expr length $a : 计算$a长度
- [max@localhost ~]$ a=abcdefghijk
- [max@localhost ~]$ expr length $a 
- 11 
expr index $a pattarn : 索引：从字符串开头查找匹配且第一个匹配的字符串的位置,记过是一个整数
- [max@localhost ~]$ a=abcdefghijk
- [max@localhost ~]$ expr index $a s 
- 0 
- [max@localhost ~]$ expr index $a 3 
- 0 
- [max@localhost ~]$ expr index $a 1 
- 0 
- [max@localhost ~]$ expr index $a c 
- 3 
- [max@localhost ~]$ expr index $a f 
- 6 
- [max@localhost ~]$ expr index $a 'f' 
- 6 
- [max@localhost ~]$ expr index $a fgh 
- 6 
expr substr $a n m :从自定n位置取m长度的字符串
- [max@localhost ~]$ a=abcdefghijk
- [max@localhost ~]$ expr substr $a 2 3 
- bcd 
- [max@localhost ~]$ expr substr $a 3 5 
- cdefg 
expr match $a 'pattern' :'match' 操作的默认行为就是从字符串的开始进行搜索并匹配第一个匹配的字符串,，结果将是输出的整数，要输出字符串时，要对利用\( \)对pattern进行转义。这这里，可以用":"代替match。expr match "$a" 'pattern' 等价于 expr "$a" : 'pattern' 
- [max@localhost ~]$ a="134oa;slkdf*(&&*^(asdljk"
- [max@localhost ~]$ expr match "$a" '\(.*\)' 
- 134oa;slkdf*(&&*^(asdljk 
- [max@localhost ~]$ expr match "$a" '.*' 
- 24 
- [max@localhost ~]$ expr "$a" : '.*' 
- 24 
- [max@localhost ~]$ expr "$a" : '\(.*\)' 
- 134oa;slkdf*(&&*^(asdljk 
    expr的操作到此就没了。
一些文本处理命令
uniq:
    这个过滤器将会删除一个已排序文件中的重复行,-c用来统计每行出现的次数, 并把次数作为前缀放到输出行的前面。
    sort INPUTFILE | uniq -c | sort -nr命令先对INPUTFILE文件进行排序, 然后统计每行出现的次数(sort命令的-nr选项会产生一个数字的反转排序)。
grep ：
  -i 选项在搜索时忽略大小写. 
  -w 选项用来匹配整个单词. 
  -l 选项仅列出符合匹配的文件, 而不列出匹配行. 
  -r (递归) 选项不仅在当前工作目录下搜索匹配, 而且搜索子目录. 
  -n 选项列出所有匹配行, 并显示行号. 
  -v (或者--invert-match)选项将会显示所有不匹配的行.
  -c (--count) 选项将只会显示匹配到的行数的总数,而不会列出具体的匹配.
wc：
  -w 统计单词数量. 
  -l 统计行数量. 
  -c 统计字节数量. 
  -m 统计字符数量. 
  -L 给出文件中最长行的长度
tr
  -d选项删除指定范围的字符. 
  --squeeze-repeats (或-s)选项用来在重复字符序列中除去除第一个字符以外的     所有字符. 这个选项在删除多余空白的时候非常有用。
   -c"complement"选项将会反转匹配的字符集. 通过这个选项, tr将只会对那些不匹配的字符起作用. 
   'cat "$@"' 结构既可以从stdin获得输入, 也可以从文件中获得输入.如果使用stdin, 那么要想结束输入就使用 Control-D. 否则就要在命令行上指定文件名。
本文出自 “[alkshao](http://alkshao.blog.51cto.com)” 博客，请务必保留此出处[http://alkshao.blog.51cto.com/1953382/510408](http://alkshao.blog.51cto.com/1953382/510408)
