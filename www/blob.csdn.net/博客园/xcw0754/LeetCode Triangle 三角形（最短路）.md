# LeetCode Triangle 三角形（最短路） - xcw0754 - 博客园
# [LeetCode Triangle 三角形（最短路）](https://www.cnblogs.com/xcw0754/p/4679648.html)
题意：给一个用序列堆成的三角形，第n层的元素个数为n，从顶往下，每个元素可以选择与自己最近的两个下层元素往下走，类似一棵二叉树，求最短路。
     [2],
    [3,4],
   [6,5,7],
  [4,1,8,3]
　　注意：这里可以2->3>5>1，也可以2->4>5->1，隔层相邻就可以走。
思路：可以从下往上走，也可以从上往下走。都是O(n)的空间，平方阶的复杂度。
　　从下往上可能更简洁，因为比较到最后只有一个元素，就是为答案了，速度自然也就快，每遍历一层就有1个被淘汰。
　　然而我一开始想到的是从上往下，以为会很简单，结果就这样了。记得从右往左扫，不然会覆盖掉之前记录的那些值。
```
1 class Solution {
 2 public:
 3     int minimumTotal(vector<vector<int>>& triangle) {
 4         if(triangle.empty() || triangle[0].empty() )    return 0;
 5 
 6         vector<int> path( triangle.size() ,0 );
 7         path[0]=triangle[0][0];
 8 
 9         for(int i=1; i<triangle.size(); i++)
10         {
11             path[ i ]=path[i-1]+triangle[i][i];
12             for( int j=triangle[i].size()-2 ; j>0; j--)     //注意从后扫，不然会覆盖到前面
13                 path[j]=triangle[i][j] + min( path[j-1] , path[j]  );
14             path[0]+=triangle[i][0];//因此这个要摆在最后
15         }
16 
17         return *min_element(path.begin(),path.end());
18     }
19 };
AC代码
```

