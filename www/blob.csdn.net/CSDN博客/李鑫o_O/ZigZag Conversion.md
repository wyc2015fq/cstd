# ZigZag Conversion - 李鑫o_O - CSDN博客





2016年03月16日 10:44:40[hustlx](https://me.csdn.net/HUSTLX)阅读数：190








```cpp
string convert(string s, int numRows) {
    vector<string> temp(numRows);
    if (numRows == 1) return s;
    int flag = 1;
    int count = 0;
    for (int i = 0; i < s.size(); i++) {
        if (count == 0) flag = 1;
        else if (count == numRows - 1) flag = -1;
        temp[count] += s[i];
        count += flag;
    }
    string res;
    for (int i = 0; i < temp.size(); i++) {
        res += temp[i];
    }
    return res;
}
```




