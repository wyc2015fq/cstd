# bash shell命令（1）；、&&、|| - happyhorizon的算法天空 - CSDN博客
2015年12月23日 16:40:51[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：413标签：[linux																[shell																[command](https://so.csdn.net/so/search/s.do?q=command&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[linux](https://blog.csdn.net/happyhorizion/article/category/3079971)
**；命令**
按照先后顺序一次执行多个命令，命令之间用；分割： 
command 1 ; command 2; command 3
**&&命令**
如果前一个命令(command 1)顺利执行，接着执行&&后的命令(command 2)： 
command 1 && command 2
**||命令**
如果前一个命令(command 1)没有顺利执行，接着执行&&后的命令(command 2)： 
command 1 || command 2
**注意**
如果command 1顺利执行，返回数值0，经过&&判断，开始执行command 2。如果没有顺利执行，返回数值非0，则不执行command 2。例如/tmp文件夹中有test1，没有test2。则： 
输入：ls /tmp/test1 && echo “exist” 
输出： 
/tmp/test1 
exist
输入：ls /tmp/test2 &&  echo “exist” 
输出： 
ls: cannot access /tmp/test2: No such file or directory
因此，由于命令是一个接一个地执行，所以&&和||的顺序不能错，判断条件最多也只有三个： 
command 1 && command 2 || command 3
