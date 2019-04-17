# 【剑指offer】题53：正则表达式匹配 - xiaxzhou的博客 - CSDN博客





2017年07月06日 21:00:29[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：155








```
bool match_core(char* str, char * pattern)
{
    if (*str == '\0'&&*pattern == '\0')
    {
        return true;
    }
    if (*str != '\0' && *pattern == '\0') 
    {
        return false;
    }
    if (*(pattern+1)=='*')
    {
        if ((*pattern == *str)||(*pattern=='.'&&*str!='\0'))
        {
            return match_core(str + 1, pattern + 2)
                || match_core(str + 1, pattern)
                || match_core(str, pattern + 2);
        }
        else
        {
            return match_core(str, pattern + 2);
        }
    }
    if ((*str == *pattern) || (*pattern == '.'&&*str != '\0'))
    {
        return match_core(str + 1, pattern + 1);
    }
    return false;
}

bool match(char* str, char* pattern)
{
    if (str==NULL||pattern==NULL)
    {
        return false;
    }
    return match_core(str, pattern);
}
```



