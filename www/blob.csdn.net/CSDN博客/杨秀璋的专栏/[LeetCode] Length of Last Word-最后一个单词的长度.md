
# [LeetCode] Length of Last Word - 最后一个单词的长度 - 杨秀璋的专栏 - CSDN博客

2015年09月08日 05:08:56[Eastmount](https://me.csdn.net/Eastmount)阅读数：1725


**题目概述：**
Given a string s consists of upper/lower-case alphabets and empty space characters ' ', return the length of last word in the string.
If the last word does not exist, return 0.
Note: A word is defined as a character sequence consists of non-space characters only.
For example,
Given s = "Hello World",
return 5.
**题目解析：**
在字符串含空格中计算最后一个单词的长度，主要考察字符串操作。最初我采用寻找空格找到最后一个单词的起始位置begin，计算该单词的长度end-begin即可。但是会遇到各种错误，如：
1.全空格字符串 如'     ' 返回0
2.最后一个单词后面存在空格 如'day   ' 返回3
3.单词前面存在多个空格 如'    day' 返回3
4.复杂的情况 如'  ab day ' 返回3
最后修改成判断当前字符不是空格时计数，有效避免全空格等用例。(作者AC的第一题)
**我的代码：**

```python
int lengthOfLastWord(char* s) {
    int length;            //数组长度
    int num=0;             //返回最后一个单词的长度
    int i,j;
    length = strlen(s);
    for(i=0; i<length; i++)
    {
        //建议当不是空格时计数 避免全空格
        if(s[i]!=' ') {
            num=0;
            for(j=i; j<length && s[j]!=' '; j++) { //计算单词长度 始终记录最后一个单词
                num++;
            }
            i=j;    //防止出现计算单词'word'后再计算'ord'覆盖前一个长度 
            if(j>=length) break;  //最后一个单词时直接跳出循环
        }
    }
    return num;
}
```
**推荐代码：**

```python
class Solution {  
public:  
    int lengthOfLastWord(const char *s) {  
        // Start typing your C/C++ solution below  
        // DO NOT write int main() function  
        if(s == NULL) return 0;  
        int ans = 0;  
        while(*s != '\0')  
        {  
            if(*s != ' ')  
            {  
                int curLen = 0;  
                while(*s != '\0' && *s != ' ') s++, curLen++;  
                ans = curLen;  
            }  
            else s++;  
        }  
          
        return ans;  
    }  
};
```
**心得感受：**
最近找工作发现很多基础的东西都忘记了，同时看《编程之美》、《剑指offer》又心不在焉，怎么办呢？只好找回自己最初的状态，督促自己A题来捡起遗忘的基础知识。LeetCode就是一个监督的平台吧！网上关于它的代码非常之多，我只想记录自己一些A题心得和当前的状态。作者真心想找到一份工作，开始新的生活，享受编程的乐趣了。
虽然我已经想好了不论去到什么公司都认认真真地学习干三五年，但在这之前当下这种找工作的随意心态还是需要改正，需要付出和认真对待。当然并不是写博客的人就多么的厉害（很多牛人只专注于编码），并不是学习好的就多么的有成就，并不是找到好工作后就能抓住幸福，但是脚踏实地的做事和享受编程分享的乐趣还是永存的。
正如钱钟书在《围城》里说的一样：“城外的人拼命往里挤，城里的人拼命往外跑”。其实不管是城里人还是城外人，所做的一切，无非就是为了幸福。然后幸福在哪里？所以且行且珍惜，享受生活和工作，换种心态，程序员的生活同样美好！
(By:Eastmount 2015-9-8 晚上5点半[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))


