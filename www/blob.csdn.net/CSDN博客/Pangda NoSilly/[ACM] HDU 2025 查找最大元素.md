# [ACM] HDU 2025 查找最大元素 - Pangda NoSilly - CSDN博客

2017年05月25日 19:09:12[hepangda](https://me.csdn.net/hepangda)阅读数：331


```cpp
#include<iostream>
#include<algorithm>
#include<string>

using namespace std;

int main() {
    string st;
    while (getline(cin, st)) {
        char max = *max_element(st.begin(), st.end());

        for (auto i : st) {
            cout << i;
            if (i == max)
                cout << "(max)";
        }
        cout << endl;
    }
    return 0;
}
```

