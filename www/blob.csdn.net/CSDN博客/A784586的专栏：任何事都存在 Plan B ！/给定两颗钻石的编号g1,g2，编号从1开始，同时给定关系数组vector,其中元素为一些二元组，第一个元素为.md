# 给定两颗钻石的编号g1,g2，编号从1开始，同时给定关系数组vector,其中元素为一些二元组，第一个元素为 - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2016年12月05日 10:21:58[QuJack](https://me.csdn.net/A784586)阅读数：546标签：[算法](https://so.csdn.net/so/search/s.do?q=算法&t=blog)
个人分类：[算法](https://blog.csdn.net/A784586/article/category/6556570)









1.题目一 (java实现)


 题目： <<<声明：水平有限，初步书写，欢迎指正，优化！>>>

      小明陪小红去看钻石，他们从一堆钻石中随机抽取两颗并比较她们的重量。这些钻石的重量各不相同。在他们们比较了一段时间后，它们看中了两颗钻石g1和g2。现在请你根据之前比较的信息判断这两颗钻石的哪颗更重。

      给定两颗钻石的编号g1,g2，编号从1开始，同时给定关系数组vector,其中元素为一些二元组，第一个元素为一次比较中较重的钻石的编号，第二个元素为较轻的钻石的编号。最后给定之前的比较次数n。请返回这两颗钻石的关系，若g1更重返回1，g2更重返回-1，无法判断返回0。输入数据保证合法，不会有矛盾情况出现。

   测试样例：

          2,3,[[1,2],[2,4],[1,3],[4,3]],4

    返回: 1






  思路分析：由于无法判断的情形较多，因此逆向思维，剩余的为无法判断

 *    首先：判断一步g1>g2情形，这样可以做到部分代码优化，简单的测试案例可以很快给出解

 *            再判断g1>g2连环成立的情形；

 *   第二步：代码复用

 *          判断一步g1

 *            再判断g1

 *   第三步：剩余的为无法判断            

 *   时间复杂度：O（n^2）

 *   总结：本题涉及基本的数组操作

 *   注意事项：Java数组与C数组的区别

  * */









package com.test;

public class Test {

public static int cmp(int g1, int g2, int records[][], int n){

for (int i = 0; i < n; i++)

if (records[i][0] == g1) {// g1>g2

{

if (records[i][1] == g2)

return 1;

 else {//g1>一个非g2数字，扫描该数字是否比G2大

 for (int k = 0; k < n; k++) {

if (records[k][0] == records[i][1])//其他队第一个数字是该非g2数字

if (records[k][1] == g2)

return 1;

}

}

}

} else {// g1

if (records[i][0] == g2) {

if (records[i][1] == g1)

return -1;

else {

for (int k = 0; k < n; k++) {

if (records[k][0] == records[i][1])

if (records[k][1] == g1)

return -1;

}

}

}




}

// 无法判断

return 0;

}




public static void main(String[] args) {

int g1 = 2, g2 = 3;

int records[][] = new int[][] { { 1, 2 }, { 2, 4 }, { 1, 3 },{ 4, 3 } };

int n = 4;

System.out.println(cmp(g1, g2, records, n));

}




}




