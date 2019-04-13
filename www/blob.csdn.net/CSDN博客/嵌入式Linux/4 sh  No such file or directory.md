
# .-4.sh: No such file or directory - 嵌入式Linux - CSDN博客

2019年02月18日 11:52:50[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：61个人分类：[Linux																](https://blog.csdn.net/weiqifa0/article/category/1388863)[Shell																](https://blog.csdn.net/weiqifa0/article/category/2102961)[
							](https://blog.csdn.net/weiqifa0/article/category/1388863)



# sh push到目标板后提示出错
`#!/bin/bash
echo "ladjfaosdjfoia"`
# 头bin/bash
我们要看，sh在哪里
`130|rk3399_idpad:/data # which sh
/system/bin/sh
rk3399_idpad:/data #`所以上面的代码应该写成
`#!/system/bin/sh
echo "ladjfaosdjfoia"`
# 修改后还是不行
看文件是不是unix格式，在notepad++里面转换一下
![](https://img-blog.csdnimg.cn/20190218115116763.png)
# 再push进去
修改权限chmod u+x[4.sh](http://4.sh)
![](https://img-blog.csdnimg.cn/20190218115220362.png)
**完毕！！！！！！！！！！！！**

