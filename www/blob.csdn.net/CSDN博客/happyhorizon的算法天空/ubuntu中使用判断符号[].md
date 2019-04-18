# ubuntu中使用判断符号[] - happyhorizon的算法天空 - CSDN博客
2016年01月07日 22:22:24[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：618
个人分类：[linux](https://blog.csdn.net/happyhorizion/article/category/3079971)
鸟哥的私房菜p270中13.3.2使用[]符号有这样一个例子：
vim sh06.sh
脚本内容如下：
#!/bin/bash
# Program:
#    This program shows the user's choice
# History:
#2005/08/25    VBird    Firstrelease8 L" l& i( \4 d0 h2 A, e% Q
PATH=/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:~/bin& }. S" B. l9 X* D
export PATH  h3 Y9 ^, i6 z$ B( U* B; ]) m
read -p "Please input (Y/N): " yn
[ "$yn" == "Y" -o "$yn" == "y" ] &&echo "OK, continue" && exit0
[ "$yn" == "N" -o "$yn" == "n" ] &&echo "Oh, interrupt!" && exit0
echo "I don't know what your choice is"&& exit 0
9 u. O  D  |9 O2 ]) n
执行时总提示错误：
root@dustsnow-laptop:~/scripts# sh sh06.sh
Please input (Y/N): y
[: 10: y: unexpected operator
[: 11: y: unexpected operator
I don't know what your choice is
但是在终端直接输入这些语句，都可以执行，没有报错。
解决方法如下：
因为ubuntu默认的sh是连接到dash的,又因为dash跟bash的不兼容所以出错了.执行时可以把sh换成bash文件名.sh来执行.成功.dash是什么东西,查了一下,应该也是一种shell,貌似用户对它的诟病颇多.
by the way修改sh默认连接到bash的一种方法：
sudo dpkg-reconfigure dash
选择no即可.
参考：http://blog.csdn.net/panpan639944806/article/details/20069653
