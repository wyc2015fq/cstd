# Linux vi 查找和替换字符串 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2019年01月31日 22:39:13[boonya](https://me.csdn.net/boonya)阅读数：975








## 字符串查找

a.查找一个字符串使用：“/你要查询的内容”

b.查找下一个 ：“/你要查询的内容” 再输入”n“跳到下一个

## 字符串替换

a. 文件内全部替换：

%s#abc#def#g（用def替换文件中所有的abc）

例如把一个文本文件里面的“linuxidc.com”全部替换成“linuxidc.net”：

:%s#linuxidc.com#xwen.net#g (如文件内有#，可用/替换,比如:%s/linuxidc.com/xwen.net/g)

b. 文件内局部替换：

把10行到50行内的“abc”全部替换成“def”

:10,50s#abc#def#g（如文件内有#，可用/替换,:%s/abc/def/g）

以上命令如果在g后面再加上c，则会在替换之前显示提示符给用户确认（conform）是否需要替换。 比如

:%s#linuxidc.com#linuxidc.net#gc



