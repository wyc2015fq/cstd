# day4_字符串的两头堵模型 - 风雪夜归人 - CSDN博客
2017年09月01日 15:08:19[cdjccio](https://me.csdn.net/qq_34624951)阅读数：98
# 一、求非空格的字符串长度
## 1、方法一 普通函数
```cpp
//方法一
int main01()
{
    //求非空格的字符串长度
    char *p = "     abcdefgg    ";
    int i = 0;
    int j = 0;
    j = strlen(p) - 1;
    int count = 0;
    while(isspace(p[i]) && p[i] != '\0')
    {
        i++;
    }
    while(isspace(p[j]) && p[j] != '\0')
    {
        j--;
    }
    count = j - i + 1;
    printf("count:%d \n",count);
    printf("Hello world!\n");
    system("pause");
    return 0;
}
```
## 2、方法二 构造API函数
```cpp
//求非空格的字符串长度
//方法二 API函数
void getCount(char *str,int *Pcount)
{
    char *p = str;
    int i = 0;
    int j = 0;
    j = strlen(p) - 1;
    int count = 0;
    if(str == NULL && Pcount == NULL)
    {
        return;
    }
    while(isspace(p[i]) && p[i] != '\0')
    {
        i++;
    }
    while(isspace(p[j]) && p[j] != '\0')
    {
        j--;
    }
    count = j - i + 1;
    *Pcount = count;
    printf("count:%d \n",*Pcount);
    return 0;
}
```
## 3、方法三 去除字符串前后的空格并返回新串
```cpp
//去除字符串前后的空格并返回新串
//方法三
int trimSpace(char *str,char *newstr)
{
    char *p = str;
    int i = 0;
    int j = 0;
    j = strlen(p) - 1;
    int count = 0;
    if(str == NULL && newstr == NULL)
    {
        return;
    }
    while(isspace(p[i]) && p[i] != '\0')
    {
        i++;
    }
    while(isspace(p[j]) && p[j] != '\0')
    {
        j--;
    }
    count = j - i + 1;
    strncpy(newstr,str+i,count);
    newstr[count] = '\0';
    return 0;
}
int main02()
{
    char *p = "     abcdefgg    ";
    char buf[1024] = {0};
    int getcount = 0;
    getCount(p,&getcount);
    printf("getcount:%d \n",getcount);
    trimSpace(p,buf);
    printf("buf:%s \n",buf);
    printf("Hello world!\n");
    system("pause");
    return 0;
}
```
## 4、方法四 典型错误及修改原空间
```cpp
//去除字符串前后的空格并返回新串
//方法四
//典型错误  不能修改全局区的字符串
//修改 将字符串写到字符串数组里面 修改栈区的数据
int trimSpace02(char *str)
{
    char *p = str;
    int i = 0;
    int j = 0;
    j = strlen(p) - 1;
    int count = 0;
    if(str == NULL)
    {
        return -1;
    }
    while(isspace(p[i]) && p[i] != '\0')
    {
        i++;
    }
    while(isspace(p[j]) && p[j] != '\0')
    {
        j--;
    }
    count = j - i + 1;
    strncpy(str,str+i,count);//error  不能向str所指向的内存空间写入数据
    str[count] = '\0';
    return 0;
}
int main()
{
//    char *p = "     abcdefgg    ";//数据在全局区里面 不能被修改
    char buf[1024] = "     abcdefgg    ";
    int getcount = 0;
    trimSpace02(buf);
//    printf("p:%s \n",p);
     printf("buf:%s \n",buf);
    printf("Hello world!\n");
    system("pause");
    return 0;
}
```

