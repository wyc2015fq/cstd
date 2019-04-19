# LeetCode-TwoSum - yixianfeng41的专栏 - CSDN博客
2015年12月05日 10:08:49[第2梦](https://me.csdn.net/yixianfeng41)阅读数：663
个人分类：[数据结构](https://blog.csdn.net/yixianfeng41/article/category/5829745)
所属专栏：[数据结构与算法](https://blog.csdn.net/column/details/14515.html)
## 题目：
给一串数字和一个目标数字，在数字串中寻找两个数加起来等于目标数字的数，返回其下标。
## 方法一（12ms)：
```cpp
vector<int> twoSum(vector<int>& nums, int target) 
{
	vector<int> oldvector=nums;  //保存一份原始vector
	int low,high,mid;
	int num1=0,num2=0;
	int index1=0,index2=0;  //两个加起来等于target的数
	vector<int> vec(0,0); //用于返回的
	sort(nums.begin(),nums.end());	//先排序
	for(int i=0;i<nums.size();i++)
	{
		low=0,high=nums.size()-1;
		while (low<=high)
		{
			mid=(low+high)/2;
			if((target-nums[i])==nums[mid])   //找到相加等于target的两个数
			{
				num1=nums[i];
				num2=nums[mid];
				break;
			}
			else if((target-nums[i])<nums[mid])
				high=mid-1;
			else
			        low=mid+1;
		}
		if(num1>0||num2>0)
			break;
	}
	for(int j=0;j<oldvector.size();j++)
	{
		if(num1==oldvector[j]||num2==oldvector[j])
		{
			if(index1>0)
			{
				index2=j+1;
				break;
			}
			index1=j+1;		
		}
	}
	vec.push_back(index1);
	vec.push_back(index2);	
	return vec;       
}
```
### 方法一升级版：
```cpp
vector<int> twoSum(vector<int>& nums, int target) {
        vector<int> ns=nums;
        sort(ns.begin(), ns.end());
        int a=0, b=ns.size()-1;
        while (a < b)
            if (ns[a]+ns[b] > target) b--;
            else if (ns[a]+ns[b] < target) a++;
            else break;
        vector<int> ans;
        for (int i=0; i<nums.size(); i++) {
            if (nums[i]==ns[a]) ans.push_back(i+1);
            else if (nums[i]==ns[b]) ans.push_back(i+1);
        }
        if (ans[0]>ans[1])
            swap(ans[0], ans[1]);
        return ans;
    }
```
### 方法二（用数组，网上的）：
```cpp
vector<int> twoSum(vector<int>& nums, int target) 
    {
        static int MAX = 99999;
        static int DELT = 49999;
        vector<int> ans;
        int x[MAX];
        memset(x, 0, sizeof(x));
        for (int i = 0; i < nums.size(); i++)
        {
            if (x[nums[i] + DELT])
            {
                ans.push_back(((i + 1) < x[nums[i] + DELT] ? 
                                          (i + 1) : x[nums[i] + DELT]));
                ans.push_back(((i + 1) > x[nums[i] + DELT] ? 
                                          (i + 1) : x[nums[i] + DELT]));
                return ans;
            }
            x[target - nums[i] + DELT] = i + 1;
        }
    }
```
### 方法三（用map，网上的）：
```cpp
vector<int> twoSum(vector<int>& nums, int target) {
        map<int,int> map1;
        for(int i = 0; i < nums.size(); i++) {
            if(map1.find(target-nums[i]) != map1.end()) {
                vector<int> a = { map1[target-nums[i]], i+1};
                return a;
            }
            map1[nums[i]] = i+1;
        }
    }
```
