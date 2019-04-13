
# Unix下去掉^M的方法 - Augusdi的专栏 - CSDN博客


2015年06月08日 11:30:21[Augusdi](https://me.csdn.net/Augusdi)阅读数：6385


﻿﻿
在vi中：
按Esc切换到命令模式；
:%s/^M//g
即可。
其中：^M是通过 Ctrl+v+Shift+m输入的，即按住ctrl键按v键,不放ctrl键,再按shift键和m键,完成后全部放开。

在linux上经常遇到这种问题，从网上下载文件到 linux 上后，就多了很多 ^M这种东西，如何集体删除这种东西呢！
(1)方法一
最简单的办法就是用里linux下vim行的替换文法:
进入vim的命令行模式输入
:%s/\r//gc
同理：如果出现^V则用:%s/^V//gc替换
(2)方法二
也是在vim命令行模式下
:set ff=unix
:w
先把格式换成unix的，然后重新保存以下就可以了！
(3)方法三
用一个工具dos2unix(注意:不同系统不一样 ) ，例如我使用的是ubuntu，其下有一个替代工具tofrodos,具体安装可以参考我的另一篇文章
[http://www.cnblogs.com/xiaofengkang/archive/2012/12/06/2805642.html](http://www.cnblogs.com/xiaofengkang/archive/2012/12/06/2805642.html)
使用:
将DOS格式文本文件转换成Unix格式，最简单的用法就是dos2unix直接跟上文件名。
格式：dos2unix file
如果一次转换多个文件，把这些文件名直接跟在dos2unix之后。（注：也可以加上-o参数，也可以不加，效果一样）
格式：dos2unix file1 file2 file3
格式：dos2unix -o file1 file2 file3
上面在转换时，都会直接在原来的文件上修改，如果想把转换的结果保存在别的文件，而源文件不变，则可以使用-n参数。
格式：dos2unix oldfile newfile
如果要保持文件时间戳不变，加上-k参数。所以上面几条命令都是可以加上-k参数来保持文件时间戳的。
格式：dos2unix -k file
格式：dos2unix -k file1 file2 file3
格式：dos2unix -k -o file1 file2 file3
格式：dos2unix -k -n oldfile newfile
注：unix2dos命令的使用方式与dos2unix命令的类似。


cat -A filename 就可以看到windows下的断元字符 ^M
要去除他，最简单用下面的命令：
dos2unix filename

第二种方法：
sed -i 's/^M//g' filename
\#注意：^M的输入方式是 Ctrl + v ，然后Ctrl + M
第三种方法：
\#vi filename
:1,$ s/^M//g
^M 输入方法： ctrl+V ,ctrl+M
第四种方法：
\#cat filename |tr -d '\r' > newfile
\#^M 可用 \r 代替



