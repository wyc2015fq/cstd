# 捕捉到socket相关函数的错误 - ljx0305的专栏 - CSDN博客
2008年07月23日 11:31:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：665
                 如果想捕捉到socket相关函数的错误，可以用下面的语句：fprintf(stderr,"Sendmsg failed.  errno : %s/n",strerror(errno));   错误信息就会显示出来了。            
