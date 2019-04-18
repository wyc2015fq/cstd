# ZigZag Conversion - bigfacesafdasgfewgf - CSDN博客





2015年01月19日 16:25:55[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：400标签：[leetcode																[zigzag conversion																[string](https://so.csdn.net/so/search/s.do?q=string&t=blog)
个人分类：[LeetCode																[Algorithm Probelm](https://blog.csdn.net/puqutogether/article/category/2812137)](https://blog.csdn.net/puqutogether/article/category/2832605)





**ZigZag Conversion**



题目描述：




The string `"PAYPALISHIRING"` is written in a zigzag pattern on a given number of rows like
 this: (you may want to display this pattern in a fixed font for better legibility)
P   A   H   N
A P L S I I G
Y   I   R
And then read line by line: `"PAHNAPLSIIGYIR"`



Write the code that will take a string and make this conversion given a number of rows:
string convert(string text, int nRows);`convert("PAYPALISHIRING", 3)` should
 return `"PAHNAPLSIIGYIR"`.




解法一：计算转换之后每个字符的下标。这种方法比较好想，但是整理下标的规律比较复杂，是个分段函数，也就是说需要分情况讨论。








![](https://img-blog.csdn.net/20150119163528400?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





所以需要按照行号i = 0 ~ nRows，分情况讨论。

代码如下，测试通过：



```cpp
string convert(string s, int nRows) {
  string result;
  
  if(nRows == 1) {
    return s;
  }
  
  for(int i = 0; i < nRows; i++) {
    int j = i;
    bool flag = true;
    while(j < s.size()) {
      result.push_back(s[j]);
      
      if(i == 0 || i == nRows - 1) {
        j += 2 * (nRows - 1);
      }
      else {
        if(flag) {
          j += 2 * (nRows - 1 - i);
          flag = false;
        }
        else {
          j += 2 * i;
          flag = true;
        }
      }
    }
  }
  return result;
 }
```



解法二：

把zigzag的形状看成一个二维的，可以是二维字符数组，也可以是多个string数组。比如我们看成nRows个string，这个时候我们只需要把nRows个string，最后再把它们串联起来就可以了。

代码如下：



```cpp
class Solution {
public:
    string convert(string s, int nRows) {
        if(nRows == 1){
            return s; 
        }
        int len = s.length(); 
        string *strArray = new string[nRows];  
        int i = 0; 
        while(i < len){
            int j; 
            //vertical line
            for(j = 0; i <len && j < nRows; j++){
                strArray[j] += s[i]; 
                i++; 
            }
            //diagonal 
            for(j = nRows-2; i < len && j > 0; j--){
                strArray[j] += s[i]; 
                i++; 
            }
        }
        string resString = ""; 
        for(i = 0; i < nRows; i++){
            resString += strArray[i]; 
        }
        
        return resString; 
    }
};
```](https://so.csdn.net/so/search/s.do?q=zigzag conversion&t=blog)](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)




