# LeetCode Permutations II （全排列） - xcw0754 - 博客园
# [LeetCode Permutations II （全排列）](https://www.cnblogs.com/xcw0754/p/4933493.html)
题意：
　　给出n个元素（可能有重复的），请产生出所有的全排列。
思路：
　　同版本1的有点不同，这次有可能含有重复的元素，很容易就TLE，节省时间才是关键点。
　　如果将一个序列中两个相同的元素交换，这个序列是仍然没有发生改变的，这也是省时间的关键点。考虑第i个位置可取的元素是nums[i-1，nums.size()-1]，那么交换的时候不必要将与num[i-1]相同的元素交换到第i位了。这可以预先通过排一次序解决。排序后变成多段相同的元素接在一起，而通常只会将每段的第一个元素被换到第i个位置，考虑每段的第2个时就会检测到相同的，自然不会交换了。
　　先观察一下代码的执行过程，会很好理解的。　
 　　递归：
```
1 class Solution {
 2     vector<vector<int> > ans;     
 3 public:
 4 
 5     void DFS(vector<int> num,int pos)//注意第一个参数不能为引用
 6     {
 7         if(pos+1==num.size())    ans.push_back(num);
 8         else
 9         {
10             for(int i=pos; i<num.size(); i++)
11             {
12                 if(i!=pos && num[i]==num[pos])    continue;//注意这里
13                 swap(num[i],num[pos]);
14                 DFS(num,pos+1);
15             }
16         }
17     }
18     
19     vector<vector<int> > permuteUnique(vector<int> &nums)
20     {
21         sort(nums.begin(),nums.end());
22         if(!nums.empty())    DFS(nums,0);
23         return ans;
24     }
25 };
AC代码
```
　　迭代：模拟了STL中的nextPermutation函数。速度很慢。
```
1 class Solution {
 2 public:
 3     bool nextPermute(vector<int> &nums){
 4         int i=nums.size()-1;
 5         while(i>0 && nums[i-1]>=nums[i])    i-- ;//检查是否已经降序了。
 6         if (i==0) return false;
 7     i--;
 8     //需要在i后面找第一个比它大的来跟nums[i]交换,
 9         int j=nums.size()-1; 
10         while(j>i && nums[j]<=nums[i])     j--;
11         swap(nums[i], nums[j]);//交换他们，并使这一段都是升序。
12         sort(nums.begin()+i+1, nums.end());
13         return true;
14     }
15     vector<vector<int> > permuteUnique(vector<int> &nums) {
16         sort(nums.begin(), nums.end());
17         vector<vector<int>> res;
18         res.push_back(nums);
19         while (true)
20     {
21             if(!nextPermute(nums)) break;
22             res.push_back(nums);
23         }
24         return res;
25     }
26     
27     
28 };
AC代码
```

