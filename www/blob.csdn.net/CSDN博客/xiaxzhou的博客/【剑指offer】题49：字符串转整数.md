# 【剑指offer】题49：字符串转整数 - xiaxzhou的博客 - CSDN博客





2017年07月06日 16:08:36[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：117
个人分类：[剑指offer](https://blog.csdn.net/xiaxzhou/article/category/6949916)









注意 int 的正负取值范围不对称， 

需要根据符号位分开判定是否正溢出或者负溢出

```
int strtoint_core(string str,int sign)
{
    long long num(0);
    for (auto iter = str.begin(); iter != str.end();++iter)
    {
        if ((*iter)<'0'||(*iter)>'9')
        {
            return -1;
        }
        num = num * 10 + ((*iter) - '0');
        if (sign==1&&num > INT_MAX)
        {
            return -1;
        }
        else if (sign ==-1&&num<INT_MIN)
        {
            return -1;
        }
    }
    return num;
}

int StrToInt(string str) {
    if (str.size()==0)
    {
        return 0;
    }
    int sign(1);
    if (str[0] == '-')
    {
        sign = -1;
        str.erase(str.begin());
    }
    else if (str[0]=='+')
    {
        str.erase(str.begin());
    }
    int num = strtoint_core(str,sign);
    if (num == -1)
    {
        return 0;
    }
    return num*sign;
}
```



