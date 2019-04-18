# perl写入文件时中文乱码终极解决方案 - Orisun - 博客园







# [perl写入文件时中文乱码终极解决方案](https://www.cnblogs.com/zhangchaoyang/articles/2283305.html)





在用perl写爬虫时，网页内容如果是英文可以正常写入文件，一旦网页中包含中文，文件是全部是乱码。可以网页的meta信息明明写着charset='utf8'，并且我把网页内容直接输出到STDOUT也可以正常显示。尝试了很多方法都不行：
- open文件时指定utf8编码
- 使用Encode::_to_utf8
- 调用系统命令enca或iconv转码
- 使用Unicode::MapUTF8::to_utf8

使用Encode::encode和Encode::decode，即

use Encode;

print FH encode("utf-8",decode("utf-8",$str));












