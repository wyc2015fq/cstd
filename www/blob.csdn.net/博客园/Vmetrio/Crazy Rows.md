# Crazy Rows - Vmetrio - 博客园







# [Crazy Rows](https://www.cnblogs.com/wangmengmeng/p/5323218.html)





Problem

You are given an **N** x **N** matrix with 0 and 1 values. You can swap any two *adjacent* rows of the matrix.

Your goal is to have all the 1 values in the matrix below or on the main diagonal. That is, for each X where 1 ≤ X ≤ N, there must be no 1 values in row X that are to the right of column X.

Return the minimum number of row swaps you need to achieve the goal.

Input

The first line of input gives the number of cases, **T**. **T** test cases follow.
The first line of each test case has one integer, **N**. Each of the next **N** lines contains **N**characters. Each character is either 0 or 1.

Output

For each test case, output
Case #X: K
where **X** is the test case number, starting from 1, and **K** is the minimum number of row swaps needed to have all the 1 values in the matrix below or on the main diagonal.

You are guaranteed that there is a solution for each test case.

Limits

1 ≤ **T** ≤ 60

Small dataset

1 ≤ **N** ≤ 8

Large dataset

1 ≤ **N** ≤ 40

Sample

|Input|Output|
|----|----|
|```321011300110001041110110011001000```|```Case #1: 0Case #2: 2Case #3: 4```|




代码:

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)


 1 int n;
 2 int mp[MAX][MAX];  //矩阵
 3 
 4 int a[MAX];   //表示第i行最后出现1的位置
 5 
 6 void solve()
 7 {
 8     int ans=0;
 9     for(int i=0; i<n; i++){
10         a[i]=-1;
11         for(int j=0; j<n; j++){
12             if(mp[i][j]==1)
13                 a[i]=j;
14         }
15     }
16     for(int i=0; i<n; i++){
17         int pos=-1;
18         for(int j=i; j<n; j++){
19             if(a[j]<=i){
20                 pos=j;
21                 break;
22             }
23         }
24 
25         for(int j=pos; j>i; j--){
26             swap(a[j],a[j-i]);
27             ans++;
28         }
29     }
30     printf("%d",&ans);
31 }


View Code
```














