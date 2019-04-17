# 【剑指offer】题47：不用加减乘除做加法 - xiaxzhou的博客 - CSDN博客





2017年07月06日 14:04:31[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：110








```
int Add(int num1, int num2)
{
    int tmp1 = num1&num2;
    int tmp2 = num1^num2;
    tmp1 = tmp1 << 1;
    if ((tmp1&tmp2 )== 0)
    {
        return tmp1 | tmp2;
    }
    else{
        return Add(tmp1, tmp2);
    }
}
```



