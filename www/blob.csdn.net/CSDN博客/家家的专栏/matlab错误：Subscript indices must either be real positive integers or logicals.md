# matlab错误：Subscript indices must either be real positive integers or logicals. - 家家的专栏 - CSDN博客





2011年03月02日 19:25:00[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：60531








matlab错误：Subscript indices must either be real positive integers or logicals.



中文解释：下标索引必须是正整数类型或者逻辑类型 



出错原因：在访问矩阵（包括向量、二维矩阵、多维数组，下同）的过程中，下标索引要么从 0 开始，要么出现了负数。注：matlab 的语法规定矩阵的索引从 1 开始，这与 C 等编程语言的习惯不一样。 



解决办法：自己调试一下程序，把下标为 0 或者负数的地方修正。





出现此错误时自己在写程序是犯了错误，就是应该进行矩阵的转置，而忘记了。。。



