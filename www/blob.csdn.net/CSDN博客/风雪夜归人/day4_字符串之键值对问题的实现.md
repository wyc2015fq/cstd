# day4_字符串之键值对问题的实现 - 风雪夜归人 - CSDN博客
2017年09月01日 22:41:28[cdjccio](https://me.csdn.net/qq_34624951)阅读数：129
键值对（”key = valude”）字符串，在开发中经常使用；
要求1：请自己定义一个接口，实现根据key获取valude；40分
要求2：编写测试用例。30分
                   要求3：键值对中间可能有n多空格，请去除空格30分
注意：键值对字符串格式可能如下：
                   “key1 =valude1”
“key2 =       valude2          
“key3  = valude3” 
“key4        = valude4” 
“key5   =   “
“key6   =“
“key7   =   “
int getKeyByValude(char *keyvaluebuf,  char *keybuf, char *valuebuf, int * valuebuflen);
int main()
{
getKeyByValude(“key1 = valude1”, ” key1”,buf, &len);
}
```cpp
//1、首先查找key是不是在母串中
//2、看有没有等号
//3、在等号后面去除空格
int trimSpace11(char *str,char *newstr)
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
int getKeyByValude(char *keyvaluebuf,  char *keybuf,  char *valuebuf)
{
    char *p = NULL;
    int ret = 0;
    if(keyvaluebuf == NULL || keybuf == NULL || valuebuf == NULL)
    {
        ret = -1;
        printf("func getKeyByValude error:%d",ret);
        return ret;
    }
//1、首先查找key是不是在母串中
    p = keyvaluebuf;//初始化赋值指针变量
    p = strstr(keyvaluebuf,keybuf);//找出str2字符串在str1字符串中第一次出现的位置（不包括str2的串结束符）。
                                   //返回该位置的指针，如找不到，返回空指针。
    if(p == NULL)
    {
        return -1;
    }
    //让赋值指针变量重新达到下一次检索的条件
    p = p + strlen(keybuf);
//2、看有没有等号
    p = strstr(p,"=");
    if(p == NULL)
    {
        return -1;
    }
    //让赋值指针变量重新达到下一次检索的条件
    p = p + strlen("=");
//3、在等号后面去除空格
    ret = trimSpace11(p,valuebuf);
    if(ret != 0)
    {
        printf("func trimSpace11 error:%d!",ret);
        return ret;
    }
    return ret;
}
int main()
{
    int ret = 0;
    int buf[1024];
    int buflen = 0;
    char *keyandvalue = "key2 =       valude2    ";
    char *key = "key2";
    ret = getKeyByValude(keyandvalue, key, buf,&buflen);
    if(ret != 0)
    {
        printf("func getKeyByValude() error!");
    }
    printf("buf:%s \n",buf);
    system("pause");
    return 0;
}
```

