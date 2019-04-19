# 算法爱好者——算法题：Nuts 和 Bolts 问题 ？ 待解决 - wjheha的博客 - CSDN博客
2018年01月08日 12:25:41[wjheha](https://me.csdn.net/wjheha)阅读数：244
给定一组 n 个不同大小的 nuts 和 n 个不同大小的 bolts。nuts 和 bolts 一一匹配。 不允许将 nut 之间互相比较，也不允许将 bolt 之间互相比较。也就是说，只许将 nut 与 bolt 进行比较， 或将 bolt 与 nut 进行比较。请写一个函数比较 nut 与 bolt 的大小。
提示： 
1、结果完全取决于比较函数，而不是字符串本身。 
2、因为你必须使用比较函数来进行排序。各自的排序当中nuts和bolts的顺序是无关紧要的，只要他们一一匹配就可以。
***格式***：
输入行第一行输入两个字符数组，最后输出排序后的数组。
***样例输入***
nuts = [ ‘ab’，’bc’，’dd’，’gg’ ] 
bolts = [ ‘AB’，’GG’，’DD’，’BC’ ]
***样例输出***
nuts = [ ‘ab’，’bc’，’dd’，’gg’ ] 
bolts = [ ‘AB’，’BC’，’DD’，’GG’ ]
***思路***：
