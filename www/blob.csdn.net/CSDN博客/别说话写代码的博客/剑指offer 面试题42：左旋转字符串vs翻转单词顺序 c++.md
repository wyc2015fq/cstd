# 剑指offer 面试题42：左旋转字符串vs翻转单词顺序  c++ - 别说话写代码的博客 - CSDN博客





2018年11月21日 11:20:41[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：39
所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)









题目：汇编语言中有一种移位指令叫做循环左移（ROL），现在有个简单的任务，就是用字符串模拟这个指令的运算结果。对于一个给定的字符序列S，请你把其循环左移K位后的序列输出。例如，字符序列S=”abcXYZdef”,要求输出循环左移3位后的结果，即“XYZdefabc”。是不是很简单？OK，搞定它！

思路：通过旋转， 比如题目中S=”abcXYZdef”，第一次全部旋转S="fedZYXcba",第二次 旋转0至length-3为S="XYZdefcba",第三次旋转length-2至length为S="XYZdefabc"

```cpp
class Solution {
public:
    void reverse(string &str,int a,int b)
    {
        char c;
        while(a<=b)
        {
            c=str[a];
            str[a]=str[b];
            str[b]=c;
            a++,b--;
        }
    }
    string LeftRotateString(string str, int n) {
        if(str.empty() || n==0) return str;
        int len=str.length()-1;
        reverse(str,0,len);
        reverse(str,0,len-n);
        reverse(str,len-n+1,len);
        return str;
    }
};
```

题目：（翻转单词顺序列）牛客最近来了一个新员工Fish，每天早晨总是会拿着一本英文杂志，写些句子在本子上。同事Cat对Fish写的内容颇感兴趣，有一天他向Fish借来翻看，但却读不懂它的意思。例如，“student. a am I”。后来才意识到，这家伙原来把句子单词的顺序翻转了，正确的句子应该是“I am a student.”。Cat对一一的翻转这些单词顺序可不在行，你能帮助他么？

思路：还是基于 翻转，先全部翻转一次，然后碰到空格即为每个单词，翻转一次。可以自己写函数，也可以用stl的reverse函数

```cpp
class Solution {
public:
    void reverse(string &str,int a,int b)
    {
        char c;
        while(a<=b)
        {
            c=str[a];
            str[a]=str[b];
            str[b]=c;
            a++,b--;
        }
    }
    string ReverseSentence(string str) {
        if(str.empty())return str;
        int start=0,len=str.length()-1;
        reverse(str,start,len);
        for(int i=0;i<=len;++i)
        {
            if(str[i]==' ')
            {
                reverse(str,start,i-1);
                start=i+1;
            }
            if(i==len)
                reverse(str,start,len);
        }
        return str;
    }
};
```

用stl的reverse函数

```cpp
string ReverseSentence(string str) {
	int size = str.size();
	reverse(str.begin(), str.end()); // 注意：翻转后的第一个字符是空格
	int start = 0;
	for (int i = 0; i < size; ++i)
	{
		if (str[i] == ' ')
		{
			reverse(str.begin() + start, str.begin() + i);
			start = i + 1; // 每个单词的第一个下标
		}
		if (i == size - 1)  // 翻转最后一个单词
			reverse(str.begin() + start, str.end());
	}
	return str;
}
```





