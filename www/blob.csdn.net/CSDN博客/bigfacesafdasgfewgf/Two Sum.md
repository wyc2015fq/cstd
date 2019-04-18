# Two Sum - bigfacesafdasgfewgf - CSDN博客





2015年01月13日 09:35:08[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：339标签：[leetcode																[twosum																[map																[算法																[hash](https://so.csdn.net/so/search/s.do?q=hash&t=blog)
个人分类：[LeetCode](https://blog.csdn.net/puqutogether/article/category/2832605)





**Two Sum**

# 1. Problem Description




Given an array of integers, find two numbers such that they add up to a specific target number.


The function twoSum should return indices of the two numbers such that they add up to the target, where index1 must be less than index2. Please note that your returned answers (both index1 and index2) are not zero-based.


You may assume that each input would have exactly one solution.

Input: numbers={2, 7, 11, 15}, target=9
Output: index1=1, index2=2

# 2. Solution 1




    先排序；

    然后在头部和尾部给放一个指针，二者相加，如果等于target，返回；如果小于target，头部指针++；如果大于target，尾部指针--；

    但是，注意！需要返回的是两个数字在原vector这个的位置哦~~好办，我们先把原vector复制下来，然后在复制的vector中查找这两个数。




    这种方法时间负责度为O(nlgn), 空间复杂度为O(n)。主要是排序花时间~~




代码如下：






```cpp
class Solution {
public:
    vector<int> twoSum(vector<int> &numbers, int target) 
	{
        vector<int> copiedNumbers(numbers);   //复制vector
        vector<int> results; 
        sort(numbers.begin(), numbers.end());   //vector库中sort函数
        
        vector<int>::iterator iter1=numbers.begin(); 
        vector<int>::iterator iter2=numbers.end()-1; 
        int num1, num2; 
		while(iter1<iter2)
		{
			if(*iter1+*iter2==target)  //如果相等
			{
				num1=*iter1; 
				num2=*iter2; 
				break; 
			}
			else if(*iter1+*iter2<target)  //如果小于
			{
				iter1++; 
			}
			else  //如果大于
			{
				iter2--; 
			}
		}

        
        int index1=1, index2=1; 
        for(iter1=copiedNumbers.begin(); iter1!=copiedNumbers.end(); iter1++)  //查找第一个数
        {
            if(*iter1==num1)
            {
                break; 
            }
            index1++; 
        }
        for(iter1=copiedNumbers.begin(); iter1!=copiedNumbers.end(); iter1++)  //查找第二个数
        {
            if((index2!=index1) && (*iter1==num2))
            {
                break; 
            }
            index2++; 
        }
        
        if(index1>index2)
        {
            swap(index1, index2); 
        }
        cout<<index1<<'-'<<index2<<endl; 
        results.push_back(index1); 
        results.push_back(index2); 
        
        return results; 
    }
};
```



其实，上面这种算法挺有思想，挺巧妙的。但是，这种方法比较适合原vector就是有序的情况，那样子时间负责度就是O(n)了~~




# 3. Solution 2




    首先，我们把这个vector存储到Hash map里面；

    key是数值，value是index；

    依次遍历其中每个数num[i]， 通过hash map查找target -num[i]，如果存在，则返回index。




代码如下：






```cpp
class Solution {
public:
    vector<int> twoSum(vector<int> &numbers, int target) {
    map<int,int> mapping;
    vector<int> result;
    
    int i=0; 
    for(; i<numbers.size(); i++)
    {
        mapping[numbers[i]]=i+1;  //copy the numbers into mapping
    }
    
    for(i=0; i<numbers.size(); i++)
    {
        int temp=target-numbers[i]; 
        if(mapping.find(temp) != mapping.end())  //find，注意判断找到的条件
        {
            if(i+1<mapping[temp])
            {
                result.push_back(i+1); 
                result.push_back(mapping[temp]); 
                return result; 
            }
            if(i+1>mapping[temp])
            {
                result.push_back(mapping[temp]); 
                result.push_back(i+1); 
                return result; 
            }
        }
    }
    return result;
}
};
```


需要注意的是，LeetCode中没有hash_map，所以我们只好用map代替了，关于二者的区别，前面的博文已经讲了~




这种方法的时间复杂度为O(n)，空间复杂度也是O(n).](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=map&t=blog)](https://so.csdn.net/so/search/s.do?q=twosum&t=blog)](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)




