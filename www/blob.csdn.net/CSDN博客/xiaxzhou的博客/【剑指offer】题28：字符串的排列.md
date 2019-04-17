# 【剑指offer】题28：字符串的排列 - xiaxzhou的博客 - CSDN博客





2017年07月03日 10:54:40[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：118








```cpp
void Permutation_core(string str,string tmp, set<string>& set)
{
    if (tmp.size() == str.size())
    {
        set.insert(tmp);
        return;
    }
    int i = tmp.size();
    for (auto j = i; j < str.size();++j)
    {
        std::swap(str[i], str[j]);
        tmp.push_back(str[i]);
        Permutation_core(str, tmp, set);
        std::swap(str[i], str[j]);
        tmp.pop_back();
    }
}

vector<string> Permutation(string str) 
{
    vector<string> vec;
    set<string> set;
    if (str.size() == 0)
    {
        return vec;
    }
    string tmp;
    Permutation_core(str,tmp, set);
    for (auto iter = set.begin(); iter != set.end();iter++)
    {
        vec.push_back(*iter);
    }
    return vec;
}
```

本题扩展： 

字符串的组合： 

字符组合可转化为0…1 到 1…1的所有取值可能，1表示取当前位的字符串。 

如abc 对应：001、010、100、011、101、110、111
```cpp
vector<string> Permutation(string str) 
{
    vector<string> vec;
    string tmp;
    int n = str.size();
    n = pow(2, n);
    for (auto i = 1; i < n;++i)
    {
        int j = i;
        int k(0);
        tmp.clear();
        while (j>>k)
        {
            if (j>>k&1)
            {
                tmp.push_back(str[k]);
            }
            k++;
        }
        vec.push_back(tmp);
    }
    return vec;
}
```




