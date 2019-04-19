# shell脚本基础1 - sxf_123456的博客 - CSDN博客
2018年05月01日 10:23:21[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：36标签：[shell](https://so.csdn.net/so/search/s.do?q=shell&t=blog)
个人分类：[shell](https://blog.csdn.net/sxf_123456/article/category/6881619)
1、shell脚本执行的三种方式
  bash  脚本文件名  或  sh 脚本文件名 
  path/脚本文件名   或 ./脚本文件名
  source  脚本文件名  或 .  脚本文件名
其中，第一种方法不需要将文件设置为可执行文件，第二种方法需要将文件使用chmod +x  脚本文件名  或者 使用命令 chmod 755  脚本文件名  将文件修改为可执行文件。第三种方法也不需要修改脚本权限。如果使用第三种方法，可以利用里边的变量在其他脚本中使用。
![](https://img-blog.csdn.net/201805011020559)
若在test_2.sh文件的最后一行加入unset test，则执行.   test_2.sh之后，使用命令echo  $test 输出test变量的值为空。
