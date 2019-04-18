# Wildcard Matching - 李鑫o_O - CSDN博客

2016年03月28日 14:47:39[hustlx](https://me.csdn.net/HUSTLX)阅读数：196


```cpp
bool isMatch(string s, string p) {
    int m = s.size(), n = p.size();
    vector<vector<bool>> temp(m + 1, vector<bool>(n + 1, false));
    temp[0][0] = true;
    for (int i = 1; i <= n; i++) temp[0][i] = (p[i-1] == '*'&&temp[0][i - 1]);
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (p[j-1] != '*')
                temp[i][j] = temp[i - 1][j - 1] && (p[j-1] == s[i-1] || p[j-1] == '?');
            else
                temp[i][j] = temp[i - 1][j]||temp[i][j-1];
        }
    }
    return temp[m][n];
}
```

