
# shell中的${}，\#\#和%%的使用 - 嵌入式Linux - CSDN博客

2016年01月07日 18:22:35[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：272个人分类：[Shell																](https://blog.csdn.net/weiqifa0/article/category/2102961)


假设我们定义了一个变量为：
file=/dir1/dir2/dir3/my.file.txt
可以用${ }分别替换得到不同的值：
${file\#*/}：删掉第一个/及其左边的字符串：dir1/dir2/dir3/my.file.txt
${file\#\#*/}：删掉最后一个/及其左边的字符串：my.file.txt
${file\#*.}：删掉第一个.及其左边的字符串：file.txt
${file\#\#*.}：删掉最后一个.及其左边的字符串：txt
${file%/*}：删掉最后一个/及其右边的字符串：/dir1/dir2/dir3
${file%%/*}：删掉第一个/及其右边的字符串：(空值)
${file%.*}：删掉最后一个.及其右边的字符串：/dir1/dir2/dir3/my.file
${file%%.*}：删掉第一个.及其右边的字符串：/dir1/dir2/dir3/my
记忆的方法为：
\# 是 去掉左边（键盘上\#在 $ 的左边）
%是去掉右边（键盘上% 在$ 的右边）
单一符号是最小匹配；两个符号是最大匹配
${file:0:5}：提取最左边的5个字节：/dir1
${file:5:5}：提取第5个字节右边的连续5个字节：/dir2
也可以对变量值里的字符串作替换：
${file/dir/path}：将第一个dir替换为path：/path1/dir2/dir3/my.file.txt
${file//dir/path}：将全部dir替换为path：/path1/path2/path3/my.file.txt
利用${ }还可针对不同的变数状态赋值(沒设定、空值、非空值)：
${file-my.file.txt}：假如$file沒有设定，則使用my.file.txt作传回值。(空值及非空值時不作处理)
${file:-my.file.txt}：假如$file沒有設定或為空值，則使用my.file.txt作傳回值。(非空值時不作处理)
${file+my.file.txt}：假如$file設為空值或非空值，均使用my.file.txt作傳回值。(沒設定時不作处理)
${file:+my.file.txt}：若$file為非空值，則使用my.file.txt作傳回值。(沒設定及空值時不作处理)
${file=my.file.txt}：若$file沒設定，則使用my.file.txt作傳回值，同時將$file賦值為my.file.txt。(空值及非空值時不作处理)
${file:=my.file.txt}：若$file沒設定或為空值，則使用my.file.txt作傳回值，同時將$file賦值為my.file.txt。(非空值時不作处理)
${file?my.file.txt}：若$file沒設定，則將my.file.txt輸出至STDERR。(空值及非空值時不作处理)

${file:?my.file.txt}：若$file没设定或为空值，则将my.file.txt输出至STDERR。(非空值時不作处理)
${\#var}可计算出变量值的长度：

${\#file}可得到27，因为/dir1/dir2/dir3/my.file.txt是27个字节
转自：http://space.baidu.com.cn/ugo5/blog/item/c550bbc54d1644079c163dbd.html

