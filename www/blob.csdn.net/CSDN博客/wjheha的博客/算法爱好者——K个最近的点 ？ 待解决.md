# 算法爱好者——K个最近的点 ？ 待解决 - wjheha的博客 - CSDN博客
2017年10月24日 17:23:05[wjheha](https://me.csdn.net/wjheha)阅读数：86标签：[算法](https://so.csdn.net/so/search/s.do?q=算法&t=blog)
个人分类：[数据结构和算法](https://blog.csdn.net/wjheha/article/category/6854909)
给定一些points和一个origin，从points中找到k个离origin最近的点。按照距离由小到大返回。如果两个点有相同距离，则按照x值来排序；若x值也相同，就再按照y值排序。
格式： 
第一行输入一个points数组代表要查找的电集，第二行一行输入一个点originde的坐标，接下来一行输入一个k值的大小，最后输出离origin最近的k个点的坐标数组。
样例输入： 
points = [ [4,6],  [4,7],   [4,4]  [2,5],   [1,1]] 
origin = [0,0] 
k = 3
样例输出： 
[[1,1],  [2,5],  [4,4]]
***思路***：
