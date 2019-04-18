# c++多行输入（带空格） - 李鑫o_O - CSDN博客

2016年03月18日 21:35:24[hustlx](https://me.csdn.net/HUSTLX)阅读数：733


```cpp
#include <string>
#include <stack>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <math.h>
using namespace std;

int main()
{
    int T;
    cin >> T;
    int N;
    cin >> N;
    while (T--) {
        vector<string> name(N);
        vector<vector<int>> m(N);
        cin.ignore();
        for (int i = 0; i < N; i++)
        {
            string temp;
            getline(cin, temp);
            int j = 0;
            while (temp[j] != ' ') j++;
            name[i] = temp.substr(0, j);
            //cout << name[i] << endl;
            while (j < temp.size()) {
                if(temp[j]!=' ')
                    m[i].push_back(temp[j] - '0');
                j++;
            }
        }
        return 0;
    }
    
}
```

