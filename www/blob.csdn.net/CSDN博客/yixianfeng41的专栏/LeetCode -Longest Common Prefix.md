# LeetCode -Longest Common Prefix - yixianfeng41的专栏 - CSDN博客
2015年12月07日 11:00:18[第2梦](https://me.csdn.net/yixianfeng41)阅读数：736
个人分类：[数据结构](https://blog.csdn.net/yixianfeng41/article/category/5829745)
所属专栏：[数据结构与算法](https://blog.csdn.net/column/details/14515.html)
### 代码最简洁的：
```cpp
string longestCommonPrefix(vector<string>& strs) 
{
	if(strs.size()==0)  return "";
	for(int i=0;i<strs[0].size();i++)//每一字符串位置下标
                for(int j=1;j<strs.size();j++)//第几个字符串
			if((i>=strs[j].size())||(strs[j][i]!=strs[0][i]))  return strs[0].substr(0,i);
	return strs[0];
}
```
### 递归二分法：
```cpp
string longestCommonPrefix(vector<string>& strs) 
{
    int size = strs.size();
    if (size==0)
        return "";
    if (size == 1)
        return strs[0];
    vector<string> v1(strs.begin(), strs.begin()+size/2);
    vector<string> v2(strs.begin()+size/2, strs.end());
    string s1 = longestCommonPrefix(v1);
    string s2 = longestCommonPrefix(v2);
    return longestCommon(s1, s2);
}
string longestCommon(string& s1, string& s2) {
    int i = 0;
    while (i<min(s1.size(), s2.size()) && s1[i] == s2[i])
        i++;
    return s1.substr(0, i);
}
```
