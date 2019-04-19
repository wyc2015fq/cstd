# LeetCode-Longest Substring Without Repeating Characters - yixianfeng41的专栏 - CSDN博客
2015年12月03日 15:57:13[第2梦](https://me.csdn.net/yixianfeng41)阅读数：685
个人分类：[数据结构](https://blog.csdn.net/yixianfeng41/article/category/5829745)
所属专栏：[数据结构与算法](https://blog.csdn.net/column/details/14515.html)
### 题目：给一个字符串，找出没有重复的最长字串
### 方法一：
```cpp
int lengthOfLongestSubstring(string s) 
{
	if(s=="")
	{
		return 0;
	}
	map<char,int> mymap;
	int p1=0,p2=1; //两个指针
	int longthest=1;  //最长的
	mymap.insert(pair<char,int>(s[p1],p1));
	while(p2<s.size())
	{	
		if(mymap.find(s[p2])==mymap.end())
		{
			mymap.insert(pair<char,int>(s[p2],p2));			
		}
		else  //重复出现
		{	
			if((p2-p1)>longthest) //看看当前字串长度
				longthest=p2-p1;
			if(mymap[s[p2]]+1>p1) //更新字串起始位置，注意起始位置不能倒退
			{
		    	    p1=mymap[s[p2]]+1;
			}			
			mymap[s[p2]]=p2; //重复字符（key)对应下标(value)也更新
		}			
		p2++; //p2一直往后走		
	}
	if((p2-p1)>longthest)  //加入这个是防止这样的情况：abcabcdefg,后面那个abcdefg没有重复的，while直接到p2=s.size()结束了。
		longthest=p2-p1;
	return longthest;        
}
```
### 方法二：
因为字符的ASCLL码为0-255，因此建一个大小为256，类型为int的数组，就涵盖了所有字符。数组的值就是给定字符串中字符的下标，初始为-1,。当字符串中有重复的字符出现时，就要更新当前字串起始位置，当前字串起始位置为这个字符上次出现的位置加一，如abcab;到第二个a的时候重复了，那么当前字串就要从第一个b开始算起。这里要注意，在更新字串起始位置时，可能会有这样的情况，abcbade,字串已经是从第一个c开始了，当遇到后面a时(钱吗a出现过)，我们不能又让字串起始位置回到初始a后面的那个b上，因此，与当前起始位置判断下，这点很重要（locas[s[i]]>idx).代码如下：
```cpp
int lengthOfLongestSubstring(string s) 
{
    //
    int locs[256];//保存字符上一次出现的位置
    memset(locs, -1, sizeof(locs));
    int idx = -1, max = 0;//idx为当前子串的开始位置-1
    for (int i = 0; i < s.size(); i++)
    {
        if (locs[s[i]] > idx)//如果当前字符出现过，那么当前子串的起始位置为这个字符上一次出现的位置+1
        {
            idx = locs[s[i]];
        }
        if (i - idx > max)
        {
            max = i - idx;
        }
        locs[s[i]] = i;
    }
    return max;
}
```
方法一是我写的，方法二是网上的，这就是菜鸟和大神的区别，加油，加油！！！
