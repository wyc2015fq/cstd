# UNIX中经常用到的命令 - 家家的专栏 - CSDN博客





2014年12月10日 10:18:50[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：855








查看内存和cpu消耗情况：nvidia-smi



查看文件大小 du –sh 文件夹   或者 du -sh *

百分比： df

编译： make  make -j8

清空编译：make clean




定位文件名，直接gf，即可直接打开另外一个文件。




查找文件：locate filename

查找文件：


- 
**查找当前目录下的指定的core文件 **find . -name "core"

- 
**查找根目录下的指定的core文件 **find / -name "core"

- 
**查找根目录下的指定的大小core文件 **find / -name "core" - size +1024c

- 
**查找文件中是否含有指定的字符 "10.71.110.89" **find / -name "*.tar.gz" - type f -exec rm -rf {} /;





查看文件是用的vim，

             写并退出  :wq

              退出： :q

              进入编辑模式 i

               退出编辑模式 按键Esc









