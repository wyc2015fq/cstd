# shell脚本read line丢失制表符问题 - nosmatch的专栏 - CSDN博客
2011年07月17日 15:26:10[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：6061
while read line
do
        echo $line
done < access_log
## [Shell脚本while read line 损失制表符的问题](http://www.cnblogs.com/pony/archive/2009/02/06/1385489.html)
描述：
虽然问题不大，但是确实挺恶心，我的目标是处理access日志
日志为了方便处理，采用了\t作为分隔，数据需要过滤再处理
不想先过滤成临时文件，再处理，这样等于多读了很多行
采用读取每行的数据，再处理的方式
while read line
do
done<$file
问题：
但是出现一个问题：read line后line中的\t不见了，导致我awk处理时找不到\t，这时默认的分隔符是空格，可是数据本身也会有空格，就会混淆
原因：
查找后得知read line会过滤掉行首、行尾的空格、制表符等。
处理办法：
**read -r line**
while read -r line
do
done<$file
另：
test.txt文本内容如下：
1 2                      3 4 5
# while read line;do echo $line;done<test.txt 
1 2 3 4 5
# while read line;do echo "$line";done<test.txt    
1 2                      3 4 5 
