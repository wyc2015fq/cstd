# LeetCode-String To Integer - yixianfeng41的专栏 - CSDN博客
2015年12月02日 10:03:37[第2梦](https://me.csdn.net/yixianfeng41)阅读数：731
个人分类：[数据结构](https://blog.csdn.net/yixianfeng41/article/category/5829745)
所属专栏：[数据结构与算法](https://blog.csdn.net/column/details/14515.html)

拿到这道题不要一分钟就写出了如下代码,
```cpp
int myAtoi(string str) 
{
    int num=0;
    int i=0;
    for(int i=0;i<str.size();i++)
    {
        num=num*10+str[i]-'o';
    }
	return num;
}
```
好简单啊，真有这么简单吗，当然不是啦！
得考虑如下几点：
1. 字串为空或者全是空格的情况，返回0； 
2. 字串的前缀如果有空格需要忽略掉；
3. 忽略掉前缀空格后，遇到的第一个字符，如果是‘+’或‘－’号，继续往后读；如果是数字，则开始处理数字；
4. 处理数字的过程中，如果之后的字符非数字，就停止转换，返回当前值；
5. 在上述处理过程中，如果转换出的值超出了int型的范围，就返回int的最大值或最小值。
代码（12ms)：
```cpp
int myAtoi(string str) 
{
    long long num=0; //整数
    int flag=1;
    int max_int=2147483647;
    int min_int=-2147483648;
    int i=0;
    while(str[i]!='\0'&&str[i]==' ')
    {
        i++;
    }
    if(str[i]!='\0'&&str[i]=='-')
    {
        flag=-1;
        i++;
    }
    else if(str[i]!='\0'&&str[i]=='+')
    {
        flag=1;
        i++;
    }
    while(str[i]!='\0')
    {
        if(str[i]<='9'&&str[i]>='0')
	{
            num=num*10+(str[i]-'0')*flag;
	    if (num > max_int || num < min_int)   //溢出处理  
                return num > 0 ? max_int : min_int;  
	}
        else
        {
            break;
        }
	i++;
    }       
    return num;
}
```
其实我觉得本题要是不限制返回类型为int,那么还要复杂一点，因为还要考虑小数。
