# sed 替换单引号 - 三少GG - CSDN博客
2013年08月10日 13:40:01[三少GG](https://me.csdn.net/scut1135)阅读数：3638
插入数据库print相关sql语句时，需要预处理文本。将出现的‘替换为“或其他符号。
### [sed 替换单引号问题](http://blog.csdn.net/wangbole/article/details/8250271)
sed 替换单引号'
eg:
echo "mmm'sss" > test
cat test
把test内容中单引号替换成双引号
sed 's/'"'"/'"''/g' test
解析下：
's/' => 要进行替换操作，后紧跟匹配字符
"'" => 用双引号包裹着单引号
/   =>分割符
'"'  => 用单引号包裹着双引号
'/g' =>分隔符，全局替换
当然还可以使用下面这两种方法替换：
sed s#\'#\"#g test
sed "s/'/\"/g" test
以下命令将a.txt文本中的所有的“127.0.0.1”替换为“192.168.0.8”
sed -i s/127.0.0.1/192.168.0.8/g a.txt
如果不想改动原文件a.txt，则可以用以下命令生成新文件new.txt
sed -e s/127.0.0.1/192.168.0.8/g a.txt > new.txt
