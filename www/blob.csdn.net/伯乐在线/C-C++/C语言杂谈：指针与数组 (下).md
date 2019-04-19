# C语言杂谈：指针与数组 (下) - 文章 - 伯乐在线
原文出处： [川山甲](http://www.cnblogs.com/baochuan/archive/2012/03/26/2414062.html)
**思维导图**
![](http://ww4.sinaimg.cn/mw690/5e4d414cgw1erlc58qmihj20et06hjrq.jpg)
### **介绍**
前接上文C 杂谈之 指针与数组 (一),接续往下谈指针和数组。
### **指针与数组 ——承接上文进行扩展**
你知道X = Y，在编译运行过程中，是什么样吗？
![](http://ww1.sinaimg.cn/mw690/5e4d414cgw1erlc5945l1j20go09aq45.jpg)
### **字符指针与函数**
1> 字符串是一个以”结尾的字符数组。
看一个例子:printf接受的是一个指向字符数组第一个字符的指针。
![](http://ww4.sinaimg.cn/mw690/5e4d414cgw1erlc59diasj20b505z0t5.jpg)
这个例子与下面两个代码是一个道理.
![](http://ww1.sinaimg.cn/mw690/5e4d414cgw1erlc59wqunj209202qaac.jpg)
![](http://ww2.sinaimg.cn/mw690/5e4d414cgw1erlc5abhwaj209002o0t0.jpg)
2> 几个常用字符函数的编写。
1>>> strcat(s,t)函数,把t指向的字符复制到s指向的字符后面？——注意”

C
```
#include <stdio.h>
#include <assert.h>
/*  strcat(ps, t): Copy the charactor pointed 
 *  by t  append to the character pointed by s  
 */
void *strcat(char *ps, char *t){
    char *addr = ps;
    assert((ps != NULL) && (t != NULL));
    while(*ps){  /*  The s point to the last character   */
        ps++;
    }
    while(*ps++ = *t++){ /*  Copy t append to the s*/
    }
    return addr;
}
int main(){
    char s[5] =  "AB";
    char *t = "E";
    printf("%s\n", strcat(s, t));
    return 0;
}
```
注意:在strcat里的两个指针ps和t，方法结束后，这两个函数都指向”后面，因为ps++操作是”先取结果，后自增”
内存变化:
![](http://ww3.sinaimg.cn/mw690/5e4d414cgw1erlc5amn05j20no0fnjto.jpg)
2>>> 函数strend(s,t):字符串t出现在字符串s的尾部，返回1,否则返回0。

C
```
strend
int strlen(char *p){
    int iLen = 0;
    while(*p){
        iLen++;
        p++;
    }   
    return iLen;
}
/*  strend:find t in s  */
int strend(char *s, char *t){
    int sLen = strlen(s);
    int tLen = strlen(t);
    if(tLen > sLen) return 0;
    while(*s)
        s++;
    while(*t)
        t++;
    for(;tLen--;){
        if(*--t != *--s) return 0;
    }   
    return 1;
}
int main(){
    char *s = "Hell Wold , Chuanshanjia";
    char *t = "Chuanshanjia";
    printf("%d\n", strend(s, t));
    return 0;
}
```
3>>> strncpy(s,t,n) 将t中前n个字符复制到s中。

C
```
strncpy(s,t,n)
#include <stdio.h>
int strlen(char *p){
    int i = 0;
    while(*p){
        p++;
        i++;
    }   
    return i;
}
/*  t中最多前n个字符复制到s中   */
char *strncpy(char *s, char *t, int n){ 
    char *addr = s;
    if(!strlen(s) || !n) return 0;
    int tLen = strlen(t);
    if(!tLen) return 0;
    if(tLen < n)
        n = tLen;
    // Move the pointer s to the last
    while(*++s) ;
    while(n--){
        *s++ = *t++;
    }   
    return addr;
}
int main(){
    char s[20] = "Hell World ";
    char *t = "Chuanshanjia";
    printf("%s\n", strncpy(s, t, 2222));
    return 0;
}
```
### **命令行参数**
1.第一个参数(常用argc表示）(:运行时命令行参数数目;第二个参数(常用argv表示)是一个指向字符串数组的指针。
2.C语言规定，argv[0]，表示启动程序的名称。
3.实例
![](http://ww3.sinaimg.cn/mw690/5e4d414cgw1erlc5bhcoaj20ay05eq3v.jpg)
运行结果:
![](http://ww1.sinaimg.cn/mw690/5e4d414cgw1erlc5c0zl4j20aa01zt8v.jpg)
内存分配:
![](http://ww3.sinaimg.cn/mw690/5e4d414cgw1erlc5cfg3vj207l04ewee.jpg)
解释:
>> argv是一个指向指针数组的指针。
所以接受参数也可以写成:
![](http://ww4.sinaimg.cn/mw690/5e4d414cgw1erlc5cw39lj208u00r3yh.jpg)
结合上图,我们可以这样理解:从里向外看
![](http://ww4.sinaimg.cn/mw690/5e4d414cgw1erlc5dbwf4j20c4067aaz.jpg)
### **复杂声明**
1.阅读——从右向左规则。
>> 规则符号:
———————————————————–
* 读作”指向…的指针”
[] 读作”…的数组”
() 读作”返回…的函数”
———————————————————–
下面两个示例:
int *f() ; // f: 返回指向int型的指针
>>步骤:
1)找标识符f：读作”f是…”
2)向右看,发现”()”读作”f是返回…的函数”
3)向右看没有什么，向左看，发现*，读作”f是返回指向…的指针的函数”
4)继续向左看,发现int,读作”f是返回指向int型的指针的函数”
int (*pf)(); // pf是一个指针——指向返回值为int型的函数
1)标识符pf，读作“pf是…”
2)向右看,发现),向左看，发现*,读作 “pf是指向…的指针”
3)向右看，发现”()”,读作“pf是指向返回…的函数的指针”
4)向右看，没有，向左看发现int，读作”pf是指向返回int型的函数的指针”
### **总结**
我最近着重看C，是因为我想深入了解一下PHP内核语言和服务器模块开发。现在的C语言，能够让我更深入的了解计算机内部。
