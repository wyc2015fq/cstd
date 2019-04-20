# API函数--字符串模型（strstr do-while,while） - 风雪夜归人 - CSDN博客
2017年08月31日 20:34:53[cdjccio](https://me.csdn.net/qq_34624951)阅读数：167
# 题目：求字符串'abcd'出现的次数
要求：自定义函数接口，完成上述需求；自定义的函数和main函数分开
## 1、do-while模型
```cpp
int main08()
{
    //strstr(str,str2);
    int cout = 0;
    //初始化 让p指针达到查找的条件
    char *p = "abcd11122abcd3344abcd444abdqq";
    do
    {
        p = strstr(p,"abcd");
        if(p != NULL)
        {
            cout ++;
            p = p + strlen("abcd");//指针达到下次查找的条件
        }
        else
        {
            break;
        }
    }while(*p != '\0');
    printf("cout:%d \n",cout);
    printf("Hello world!\n");
    system("pause");
    return 0;
}
```
## 2、while模型
```cpp
//while模型
int main09()
{
    int cout = 0;
    //初始化 让p指针达到查找的条件
    char *p = "abcd11122abcd3344abcd444abdqq";
    while(p = strstr(p,"abcd"))
    {
        cout ++;
        p = p + strlen("abcd");//指针达到下次查找的条件
        if(*p == '\0')
        {
            break;
        }
    }
    printf("cout:%d \n",cout);
    printf("Hello world!\n");
    system("pause");
    return 0;
}
```
## 3、定义函数接口
```cpp
//函数接口
int getNum(char *mystr,/*in*/char *substr,/*in*/int *ncount)
{
    char *p = mystr;
    int tmpcount = 0;
    int ret = 0;
    if(mystr == NULL || substr == NULL || ncount == NULL)
    {
        ret = -1;
        printf("%d:func getNum() error:%d (mystr == NULL || substr == NULL || ncount == NULL)\n",__LINE__,ret);
        return ret;
    }
    do
    {
        p = strstr(p,substr);
        if(p != NULL)
        {
            tmpcount ++;
            p = p + strlen(substr);//指针达到下次查找的条件
        }
        else
        {
            break;
        }
    }while(*p != '\0');
    *ncount = tmpcount;//间接赋值是指针存在的最大的意义
    return 0;
}
int main()
{
    //指针做形参并做输入类型时，需要在主调函数中分配内存
    char *p = "abcd11122abcd3344abcd444abdqq";
    char sub[] = "abcd";
    int count = 0;
    int ret = 0;
//    ret = getNum(p,sub,&count);
    ret = getNum(p,NULL,&count);
    if(ret != 0)
    {
        printf("func getNum error!\n");
        return ret;
    }
    printf("count = %d \n",count);
    system("pause");
    return 0;
}
```
