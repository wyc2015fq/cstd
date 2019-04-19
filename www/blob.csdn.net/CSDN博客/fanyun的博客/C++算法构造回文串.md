# C++算法构造回文串 - fanyun的博客 - CSDN博客
2018年09月28日 20:23:05[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：137

1.回文串的实现要求
       给定一个字符串s，你可以从中删除一些字符，使得剩下的串是一个回文串。 如何删除才能使得回文串最长呢？ 输出需要删除的字符个数。
2.输入描述:
      输入数据有多组，每组包含一个字符串s，且保证:1 <= s.length <= 1000.
3.输出描述 :
    对于每组数据，输出一个整数，代表最少需要删除的字符个数。
4. 输入示例
    abcda
    google
5.实现分析
       比较简单的想法就是求原字符串和其反串的最大公共子串的长度，然后用原字符串的长度减去这个最大公共子串的长度就得到了最小编辑长度。 （注：最大公共子串并不一定要连续的，只要保证出现次序一致即可看作公共子串） 可以使用 Needleman/Wunsch算法 ,牺牲内存换取简单的代码和CPU时间。
6.实现代码
```cpp
#include<iostream>
#include<string>
#include<algorithm>
using namespace std;
const int MAX = 1001;
int MaxLen[MAX][MAX]; //最长公共子序列，动态规划求法
int maxLen(string s1, string s2){
    int length1 = s1.size();
    int length2 = s2.size();
    for (int i = 0; i <= length1; ++i)
        MaxLen[i][0] = 0;
    for (int i = 0; i <= length2; ++i)
        MaxLen[0][i] = 0;
    for (int i = 1; i <= length1; ++i)
    {
        for (int j = 1; j <= length2; ++j)
        {
            if (s1[i-1] == s2[j-1]){
                MaxLen[i][j] = MaxLen[i-1][j - 1] + 1;
            }
            else
            {
                MaxLen[i][j] = max(MaxLen[i - 1][j], MaxLen[i][j - 1]);
            }
        }
    }
    return MaxLen[length1][length2];
}
int main(){
    string s;
    while (cin >> s){
        int length = s.size();
        if (length == 1){
            cout << 1 << endl;
            continue;
        }
        //利用回文串的特点
        string s2 = s;
        reverse(s2.begin(),s2.end());
        int max_length = maxLen(s, s2);
        cout << length - max_length << endl;
    }
    return 0;
}
```
程序执行结果：
