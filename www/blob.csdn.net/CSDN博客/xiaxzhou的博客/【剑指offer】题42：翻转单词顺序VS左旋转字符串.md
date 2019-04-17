# 【剑指offer】题42：翻转单词顺序VS左旋转字符串 - xiaxzhou的博客 - CSDN博客





2017年07月05日 10:48:50[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：113








```
void Reverse_Word(string& str, int left, int right)
{
    while (left<right)
    {
        using std::swap;
        swap(str[left++], str[right--]);
    }
}

string ReverseSentence(string str)
{
    if (str.size()==0)
    {
        return str;
    }
    int left(0), right(0);
    while (left<str.size())
    {
        if (str[left]==' ')
        {
            left++;
            right++;
        }else if (str[right]==' '||right==str.size())
        {
            Reverse_Word(str, left, right-1);
            left = right;
        }
        else
        {
            right++;
        }
    }
    Reverse_Word(str, 0, str.size() - 1);
    return str;
}
int main()
{
    string str("a stdudent id  ");
    auto tmp = ReverseSentence(str);
#ifdef debug_
#else
#endif
    return 0;
}
```

```
void reverse(string& str, int left, int right)
{
    while (left<right)
    {
        using std::swap;
        swap(str[left++], str[right--]);
    }
}

string LeftRotateString(string str, int n)
{
    if (str.size()==0||n<=1)
    {
        return str;
    }
    n = n%str.size();
    reverse(str, 0, n - 1);
    reverse(str, n, str.size()-1);
    reverse(str, 0, str.size() - 1);
    return str;
}

int main()
{
    string str("a stdudent id  ");
    auto tmp = LeftRotateString(str, 3);
#ifdef debug_
#else
#endif
return 0;
}
```



