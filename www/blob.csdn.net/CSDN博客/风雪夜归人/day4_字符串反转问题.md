# day4_字符串反转问题 - 风雪夜归人 - CSDN博客
2017年09月01日 21:31:25[cdjccio](https://me.csdn.net/qq_34624951)阅读数：82
# 一、第一种方法 指针
```cpp
//二、字符串反转
//第一种方法：指针
int inverse(char *str1)
{
    int length = strlen(str1);
    char *p1 = str1;
    char *p2 = str1 + length - 1;
    if(str1 == NULL)
    {
        return -1;
    }
    while(p1 < p2)
    {
        char c = *p1;
        *p1 = *p2;
        *p2 = c;
        ++p1;
        --p2;
    }
    printf("buf:%s \n",str1);
}
int main04()
{
    char buf[] = "abcdefg";
    inverse(buf);
    printf("Hello world!\n");
    system("pause");
    return 0;
}
```
# 二、第二种方法  递归逆序
1、通过递归的方式逆向打印
2，递归和全局变量（把逆序的结果存入全局变量）
3、递归和非全局变量（递归指针做函数参数）
**理解递归的两点**
1、参数的入栈模型
2、函数调用返回模型
## 2.1、通过递归的方式逆向打印  但是不能保存数据
```cpp
void inverse2(char *p)
{
    if(p == NULL)//递归结束的一场条件
    {
        return;
    }
    if(*p == '\0')//递归结束的条件
    {
        return;
    }
    inverse2(p+1);//此时没有执行打印，而是执行了让字符串的每一个地址入栈
    printf("%c ",*p);
}
int main05()
{
    char buf[] = "abcdefg";
    inverse2(buf);
    system("pause");
    return 0;
}
```
## 示意图如下：
![](https://img-blog.csdn.net/20170905215726976?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzQ2MjQ5NTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 2.2、递归和全局变量（把逆序的结果存入全局变量）
```cpp
```
char g_buf[1000];// 全局变量 用于函数inverse3
void inverse3(char *p)
{
    if(p == NULL)//递归结束的一场条件
    {
        return;
    }
    if(*p == '\0')//递归结束的条件
    {
        return;
    }
    inverse3(p+1);//此时没有执行打印，而是执行了让字符串的每一个地址入栈
    //strcat()
    //strncpy(g_buf,p,1);// a 只会打印最后一个字符“a”
    strncat(g_buf,p,1);//gfedcba
}
int main06()
{
    char buf[] = "abcdefg";
    memset(g_buf,0,sizeof(g_buf));
    inverse3(buf);
    printf("g_buf:%s \n",g_buf);
    system("pause");
    return 0;
}
```cpp
```
# 2.3、递归和非全局变量（递归指针做函数参数）
```cpp
//3、递归和非全局变量（递归指针做函数参数）
void inverse4(char *p,char *bufresult)
{
    if(p == NULL || bufresult == NULL)//递归结束的一场条件
    {
        return;
    }
    if(*p == '\0')//递归结束的条件
    {
        return;
    }
    inverse4(p+1,bufresult);//此时没有执行打印，而是执行了让字符串的每一个地址入栈
    strncat(bufresult,p,1);//gfedcba
}
int main07()
{
    char buf[] = "abcdefg";
    char mybuf[1024] = {0};
    inverse4(buf,mybuf);
    printf("递归和局部变量在一起mybuf:%s \n",mybuf);
    system("pause");
    return 0;
}
```
