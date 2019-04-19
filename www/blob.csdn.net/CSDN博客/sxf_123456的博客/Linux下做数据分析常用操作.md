# Linux下做数据分析常用操作 - sxf_123456的博客 - CSDN博客
2018年05月17日 12:55:19[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：339标签：[Linux](https://so.csdn.net/so/search/s.do?q=Linux&t=blog)
个人分类：[Linux command](https://blog.csdn.net/sxf_123456/article/category/6748440)
1、查询文件中某一列，包含某个字符串的个数
cat data.txt | awk -F ',' '{print $8}' | grep '路由器' | wc -l
2、查询文件中某一列去重后的数据个数
cat data.txt | awk -F ',' '{print $1}' | sort | uniq | wc -l
3、查询文件有多少行
wc -l data.txt
4、将两个文件的并集输出到另一个文件
cat 1.txt  2.txt | sort |uniq > 5.txt
5、将两个文件的交集(某行相同的值)输出到另一个文件
cat 1.txt  2.txt | sort |uniq -d > 6.txt
6、去掉两个文件中的交集部分，剩下的部分输出到另一个文件
cat 1.txt 2.txt | sort | uniq -u > 7.txt
7、按行合并文件
cat 1.txt  2.txt  > 8.txt
8、按列合并文件
文件中间用tab键分开
paste 1.txt  2.txt  > 9.txt
文件中间用逗号分开
paste 1.txt 2.txt -d ','  > 10.txt
9、重复行
重复行保留一行输出
sort file | uniq
重复行全部去掉
sort file | uniq -u
10、批量替换文件中某个字符
将3.txt文件中的tab键换为逗号
sed  -i 's/\t/,/g' 3.txt
将3.txt文件中的空格换为逗号
sed  -i 's/ /,/g' 3.txt
将3.txt文件中的tab键换为逗号
perl -pi -e"s/\t/,/g" 3.txt
