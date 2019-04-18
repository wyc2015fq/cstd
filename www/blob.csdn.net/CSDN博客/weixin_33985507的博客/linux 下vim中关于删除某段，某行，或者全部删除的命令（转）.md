# linux 下vim中关于删除某段，某行，或者全部删除的命令（转） - weixin_33985507的博客 - CSDN博客
2018年03月08日 07:16:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
本文转自： [http://blog.csdn.net/jiesa/article/details/53319755](https://link.jianshu.com?t=http%3A%2F%2Fblog.csdn.net%2Fjiesa%2Farticle%2Fdetails%2F53319755)
```
1.先打开某个文件
vim filename
```
```
2.转到文件尾行
直接在命令模式下输入 G
```
```
3.转到第 N 行
在命令模式下，输入 NG (N代表第几行)
```
```
4.删除所有内容
先在命令模式下输入 G
再输入 
:1,.d
```
```
5.删除第n行到第n+m行的内容
先在命令模式下输入 n+mG (n+m 代表大于n的某一行)
再执行如下命令
:m,.d
```
```
6.关于删除的一些说明
* 在vim中，"."表示当前行，“1,.”表示从第一行到当前行，“d”表示删除。
* 如果只是想删除某一行，直接将光标移至该行，输入dd即可。
```
```
7.设置显示行号
在vim里执行
:set number
```
