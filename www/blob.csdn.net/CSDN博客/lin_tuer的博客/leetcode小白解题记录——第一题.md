
# leetcode小白解题记录——第一题 - lin_tuer的博客 - CSDN博客


2016年08月12日 10:19:00[lin_tuer](https://me.csdn.net/lin_tuer)阅读数：601



```cpp
<span style="font-family: Arial, Helvetica, sans-serif; background-color: rgb(255, 255, 255);">第一题题目：</span>
```
Given an array of integers, find two numbers such that they add up to a specific target number.
The function twoSum should return indices of the two numbers such that they add up to the target, where index1 must be less than index2. Please note that your returned answers (both index1 and
 index2) are not zero-based.
You may assume that each input would have exactly one solution.
Input: numbers={2, 7, 11, 15}, target=9
Output: index1=1, index2=2
所需基础知识:
**1. hash函数**
$ 基本概念：
>Hash，一般翻译做“散列”，也有直接音译为“哈希”的;
>就是把任意长度的输入（又叫做预映射，
 pre-image），通过散列算法，变换成固定长度的输出，该输出就是散列值;
>意义：这种转换是一种压缩映射，也就是，散列值的空间通常远小于输入的空间;
>不同的输入可能会散列成相同的输出，而不可能从散列值来唯一的确定输入值;
>简单的说,就是一种将任意长度的消息压缩到某一固定长度的消息摘要的函数。
$常用HASH函数
·直接取余法： f(x):= x mod maxM ; maxM一般是不太接近 2^t 的一个质数。
·乘法取整法： f(x):=trunc((x/maxX)*maxlongit) mod maxM，主要用于实数。
·平方取中法： f(x):=(x*x div 1000 ) mod 1000000); 平方后取中间的，每位包含信息比较多。
**2. map函数**
**>Map是c++的一个标准容器，提供了很好一对一的关系**
**>详细用法参见这里：****[http://blog.sina.com.cn/s/blog_61533c9b0100fa7w.html](http://blog.sina.com.cn/s/blog_61533c9b0100fa7w.html)**
**3. vector**
>在c++中，vector是一个十分有用的容器;
>容器常用操作：[http://www.cnblogs.com/wang7/archive/2012/04/27/2474138.html](http://www.cnblogs.com/wang7/archive/2012/04/27/2474138.html)

解题代码：


```cpp
class Solution {  
public:  
    /*Below is the 2 sum algorithm that is O(NlogN) + O(N)*/  
    /*Alternative: hash从左往右扫描一遍，然后将数及坐标，存到map中。然后再扫描一遍即可。时间复杂度O(n)*/ 
    /*算法复杂度 O（NlogN）+O（N）*/
	/*方法：hash从左往右扫描一遍，然后将数及坐标，存在map中。在扫描一遍即可。O(N)*/
	/*1. 查hash
	  2. 查map这个东西
	  3. 查vector<int> 
	  4. 查vertor<int> &number  **************************  vector<int> &num 类似于 int num [];
	  5. 查数组的方法 numberCopy.push_back(number[i]);
	  6. 查sort(numberCopy.begin(),numberCopy.end());
	  7. 查returnIndexes[0]=returnIndexes[0]^returnIndexes[1];
        */
    vector<int> twoSum(vector<int> &numbers, int target) {  
        vector<int> numberCopy;       //创建一个vector对象来存储数组
		for(int i=0;i<numbers.size();i++){
			numberCopy.push_back(numbers[i]);  //vector对象尾部插入数字：vec.push_back(a);
		}
		sort(numberCopy.begin(),numberCopy.end()); //O(NlogN)  使用sort排序：需要头文件#include<algorithm>，
		vector<int> returnNumber = twoSumAlgorithm(numberCopy,target); //O(N) 这里得到了是哪两个数的和
		vector<int> returnIndexes; 
		int flag1=-1;
		//寻找返回数的下标
		for(int j=0;j<returnNumber.size();j++){
			for(int i=0;i<numbers.size();i++){
				if(numbers[i] == returnNumber[j]) {
				    if(i != flag1){
					returnIndexes.push_back(i);
					flag1=i;
					break;//跳出内层循环
				    }
					
				}
			}
		}
        //将returnIndexes排序，序号小的放在前面
        /*
		if(returnIndexes[0]>returnIndexes[1]){
			returnIndexes[0]=returnIndexes[0]^returnIndexes[1];
			returnIndexes[1]=returnIndexes[0]^returnIndexes[1];
			returnIndexes[0]=returnIndexes[0]^returnIndexes[1];
		}
		*/
		if(returnIndexes[0] > returnIndexes[1]){
             reverse(returnIndexes.begin(),returnIndexes.end());//将数组翻转
         }
         
		return returnIndexes;
    }  
      
    /*Core algorithm is linear*/  
    vector<int> twoSumAlgorithm(vector<int> &numbers, int target) {  
        int len = numbers.size();  
        vector<int> r;  
        int i = 0; int j = len - 1;  
        while(i < j){  
            int x = numbers[i] + numbers[j];  
            if(x == target){   
                r.push_back(numbers[i]);  
                r.push_back(numbers[j]);  
                i++; j--;
            }else if(x > target) j--;  
            else i++;
        }  
        return r;  
    }  
};
```


