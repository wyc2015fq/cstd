# socket数据报解析 - walkabc - 博客园
# [socket数据报解析](https://www.cnblogs.com/hitfire/articles/4504884.html)
　　记得以前总是对socket数据包解析很糊涂，相信也有很多人对此有疑惑。下面简单用C语言来解释如何解包。懂得了解包，封包自然就明白了。
　　socket发送和接收数据都是通过字符串的格式进行的。解包就是把这个字符串解析出来int , short , char。
　　假设现在有一个字符串，char src[],里面包含了一个short int 数据, 一个int数据 , 一个char[6]字符串。
　　解析的步骤如何。
　　1.首先解出来short int 
　　short int shortNum = *(short int*)src;
　　2.接着解int，注意src的下标,为什么会是2呢？因为short int 占据了前两个字节，如果int要从short int 的后面开始解的
　　int intNum = *(int*)(src+2);
　　3.接着解析char[]字符串，注意src+6，因为short int 和int已经占据了前面6个字节，所以字符串的解析是从他俩后面开始解的
　　char destChar[100];
　　memcpy(destChar, src+6, len);
　　通过上面的几个步骤就可以从字符串里面解出来想要的数据格式了。
　　下面是一段简单的代码，希望能加深对解包封包的理解
```
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main(int argc, char* argv[])
{
    short shortNum = 10;
    int intNum = 90909090;
    char str[] = "ABCDEFG";
    char buf[1000];
    //开始封包
    *(short int*)buf = shortNum;
    *(int*)(buf+2) = intNum;
    memcpy(buf+6, str, strlen(str));
    //封包结束
    //开始解包
    short int shortNum2 = *(short*)buf;
    int intNum2 = *(int*)(buf+2);
    char str2[7];
    memcpy(str2, buf+6, strlen(str));
    printf("shortNum2:%d\n", shortNum2);
    printf("intNum2:%d\n", intNum2);
    printf("str2:%s\n", str2);
    //解包结束
    return 0;
}
```
　　上面的代码是在gcc version 4.8.2 (Ubuntu 4.8.2-19ubuntu1)编译完成，并且可顺利运行。

