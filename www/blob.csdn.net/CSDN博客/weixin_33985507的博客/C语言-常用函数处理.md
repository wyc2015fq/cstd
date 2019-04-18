# C语言-常用函数处理 - weixin_33985507的博客 - CSDN博客
2015年07月01日 10:03:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
1.使用fgets
```
#define SLEN 50
char str[SLEN];
fgets(str, SLEN, stdin);
i = 0;
while (str[i] != '\n' && str[i] != '\0')
    i++
if (str[i] == '\n')
    str[i] = '\0'
else
    while (getchar() != '\n')
        continue;
```
2.使用getchar()过滤掉输入缓存中的剩余部分
```
void eatline(void) {
    while (getchar() != '\n')
        continue;
}
```
3.
