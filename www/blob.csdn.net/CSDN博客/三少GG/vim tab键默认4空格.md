# vim tab键默认4空格 - 三少GG - CSDN博客
2013年07月31日 17:15:51[三少GG](https://me.csdn.net/scut1135)阅读数：1031
个人分类：[Ubuntu/OS X系统](https://blog.csdn.net/scut1135/article/category/680808)
# vim tab键默认4空格
为了vim更好的写代码,修改tab默认4个空格有两种设置方法：
1. vim /etc/vimrc 
|`1`|`set``ts=4`|
|`2`|`set``sw=4`|
2. vim /etc/vimrc 
|`1`|`set``ts=4`|
|`2`|`set``expandtab`|
|`3`|`set``autoindent`|
推荐使用第二种，按tab键时产生的是4个空格，这种方式具有最好的兼容性。 
