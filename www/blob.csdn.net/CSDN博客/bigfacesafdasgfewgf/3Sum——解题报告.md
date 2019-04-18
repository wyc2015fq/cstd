# 3Sum——解题报告 - bigfacesafdasgfewgf - CSDN博客





2015年05月07日 10:01:46[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：708











    【题目】

     Given an array S of n integers, are there elements a, b, c in S such that a + b + c = 0? Find all unique triplets in the array which gives the sum of zero.


Note:

Elements in a triplet (a,b,c) must be in non-descending order. (ie, a ≤ b ≤ c)

The solution set must not contain duplicate triplets.

For example, given array S = {-1 0 1 2 -1 -4},


A solution set is:

(-1, 0, 1)

(-1, -1, 2)




    【分析】

    我们做过这样的题目，给定一个数组和一个target，找出数组中的a+b = target。那么这道题目变成了a，b，c三个数。

    其实，我们遍历每个a，然后target' = targer - a. 也就变成了找出b+c = target'。和上面的题目一样吧~




    【代码】

    上述解法的时间复杂度最快也只能达到O(n^2)。

    需要解释的是，下面给出的代码在LeetCode提交的时候，一直是LTE。线下测试时对的，查找了答案之后发现，主体思路一样，只不过我用到的是set去重，可能这部分比较耗时。。。






```cpp
vector< vector<int> > threeSum(vector<int>& nums) 
{
    //if(nums.empty())
        //return;
        
    sort(nums.begin(), nums.end());
    set< vector<int> > res1; 
    set< vector<int> >::iterator iter;
    vector< vector<int> > res2;
	vector<int> tmp;
    
    if(nums.size() < 3)
        return res2;
        
    for(int i = 0; i < nums.size() - 2; i++)
    {
        int current = nums[i]; 
        int diff = 0 - current; // the target
        
        int head = i + 1, tail = nums.size() - 1;
        while(head < tail)
        {
            if(head == i)  // skip the current
                {head++; continue;}
            if(tail == i)
                {tail--; continue;}
            
            if(nums[head] + nums[tail] == diff)
            {
				//cout<<i<<' '<<head<<' '<<tail<<endl;
                tmp.push_back(current); tmp.push_back(nums[head]); tmp.push_back(nums[tail]);
                sort(tmp.begin(), tmp.end());  // sort
				//cout<<tmp[0]<<' '<<tmp[1]<<' '<<tmp[2]<<endl;
				//cout<<current<<' '<<nums[head]<<' '<<nums[tail]<<endl;
                res1.insert(tmp);  // insert into the set
				tmp.clear();
                head++;
                tail--;
                continue;
            }
            else if(nums[head] + nums[tail] < diff)
            {
                head++;
                continue;
            }
            else
            {
                tail--;
                continue;
            }
        }
    }
    
    for(iter = res1.begin(); iter != res1.end(); ++iter)
        res2.push_back(*iter);
    
    return res2;
}
```


下面给出一个ac的参考答案：

http://bbs.csdn.net/topics/390931100







