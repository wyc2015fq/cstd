# 【Linux】shell命令学习之find - 江南烟雨 - CSDN博客
2013年09月22日 09:44:01[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：5137
find命令主要用来进行文件或目录的查找。
-print指明打印出匹配的文件名
-print0指明使用'\0'定界符来打印每一个匹配的文件名
![](https://img-blog.csdn.net/20130922094254218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhanVuMDcwNjEyMjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
 -name：按文件名查找
![](https://img-blog.csdn.net/20130922094305000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhanVuMDcwNjEyMjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
-iname忽略字母大小写
如果想匹配多个条件中的一个，可以采用OR操作：
![](https://img-blog.csdn.net/20130922094313218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhanVuMDcwNjEyMjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
-path将文件路径作为一个整体来匹配。
![](https://img-blog.csdn.net/20130922094304734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhanVuMDcwNjEyMjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
-regex：与path类似，但是基于正则表达式来进行匹配
！否定参数的意义：
![](https://img-blog.csdn.net/20130922094337500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhanVuMDcwNjEyMjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
基于目录深度的搜索：
find命令在使用时会遍历所有的子目录。可以采用参数-maxdepth和-mindepth来限制遍历的深度。
根据文件类型搜索：
用-type对文件类型进行过滤。
列出所有目录：-type d
只列出普通文件：-type f
只列出符号链接：-type l
根据文件时间进行搜素：
UNIX/Linux文件系统中的每一个文件中都有三种时间戳：
- 访问时间（-atime）：用户最后一次访问时间
- 修改时间（-mtime）：文件内容最后一次被修改的时间
- 变化时间（-ctime）：文件元数据（metadata，例如权限或所有权）最后一次改变的时间
-表示小于，+表示大于
基于文件大小的搜索：
找出大于2KB的文件：find
 . -type f -size +2k
单位有：
b-块（512字节）
c-字节
w-字（2字节）
k-千字节
M-兆字节
G-吉字节
删除匹配的文件：
-delete
基于文件权限和所有权的匹配：
-perm权限值
让find跳过特定的目录：
比如在版本控制系统Git中查找特定文件的时候跳过.git目录：
find . \( -name ".git" -prune \) -o \( -type f -print \)
其中， \( -name ".git" -prune \)用于排除。
To ignore a directory and the files under it, use -prune; 
 \( -type f -print \)指明需要执行的动作。
