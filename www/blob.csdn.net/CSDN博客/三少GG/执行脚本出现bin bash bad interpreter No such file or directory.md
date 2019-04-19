# 执行脚本出现bin/bash: bad interpreter: No such file or directory - 三少GG - CSDN博客
2013年08月12日 16:11:00[三少GG](https://me.csdn.net/scut1135)阅读数：1629

SHELL 写了一个简单的例子发生如下错误 
-bash: ./test.sh: /bin/bash^M: bad interpreter: No such file or directory 
VI打开文件，没发现任何问题， 
把/bin/bash改成/bin/sh 
-bash: ./test.sh: /bin/sh^M: bad interpreter: No such file or directory 
依然报错，脚本内容在VI下没有任何问题，仔细一看错误消息，^M..明显在第一行末尾被加入了个^M， 
这种情况应该有两种原因 
1.在WIN底下用文本编辑工具修改过参数变量，在保存的时候没注意编码格式造成的， 
2.也有可能是在VIM里修改，第一行末尾按到ctrl_v 
错误原因之一很有可能是你的脚本文件是DOS格式的, 即每一行的行尾以**\r\n**来标识, 其ASCII码分别是0x0D, 0x0A. 
可以有很多种办法看这个文件是DOS格式的还是UNIX格式的, 还是MAC格式的 。
***********************************************************************************************************************************************
（1）. vi filename 
然后用命令 
**:set ff? **
可以看到dos或unix的字样. 如果的确是dos格式的, 那么你可以用**set ff=unix**把它强制为unix格式的, 然后**存盘**退出. 再运行一遍看.**(OK)**
*************************************************************************************************************************************************
2.可以用执行dos2unix 命令转换编码 
#dos2unix myshell.sh 
3..也可以用sed 这样的工具来做: 
sed 's/^M//' filename > tmp_filename 
mv -f tmp_filename filename 
来做 
特别说明:^M并不是按键shift + 6产生的^和字母M, 它是一个字符, 其ASCII是0x0D, 生成它的办法是先按CTRL+V, 然后再回车(或CTRL+M) 
关于^M 
new line of DOS/Windows 
new line of dos/win:   0X0d0a 
new line of linux/unix: 0X0a 

