# 正则之PCRE库 - miner_zhu的博客 - CSDN博客





置顶2018年10月19日 21:20:47[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：95








**目录**



[什么是PCRE库？](#%E4%BB%80%E4%B9%88%E6%98%AFPCRE%E5%BA%93%EF%BC%9F)

[1.PCRE库简介](#1.PCRE%E5%BA%93%E7%AE%80%E4%BB%8B)

[2.PCRE正则基础知识](#2.PCRE%E6%AD%A3%E5%88%99%E5%9F%BA%E7%A1%80%E7%9F%A5%E8%AF%86)

[PCRE库的安装（linux）](#PCRE%E5%BA%93%E7%9A%84%E5%AE%89%E8%A3%85%EF%BC%88linux%EF%BC%89)

[PCRE库的使用](#PCRE%E5%BA%93%E7%9A%84%E4%BD%BF%E7%94%A8)

[1.PCRE库的函数接口](#1.PCRE%E5%BA%93%E7%9A%84%E5%87%BD%E6%95%B0%E6%8E%A5%E5%8F%A3)

[2.PCRE使用过程](#2.PCRE%E4%BD%BF%E7%94%A8%E8%BF%87%E7%A8%8B)

[3.PCRE实例](#3.PCRE%E5%AE%9E%E4%BE%8B)

[参考文章](#%E5%8F%82%E8%80%83%E6%96%87%E7%AB%A0)

# 什么是PCRE库？

### 1.PCRE库简介

PCRE(Perl Compatible Regular Expressions）：perl语言兼容正则表达式。PCRE是一个用C语言编写的正则表达式函数库。PCRE是一个轻量级的函数库，比Boost之中的正则表达式库小得多。PCRE十分易用，同时功能也很强大，性能超过了POSIX正则表达式库和一些经典的正则表达式库。

        和Boost正则表达式库的比较显示，双方的性能相差无几，PCRE在匹配简单字符串时更快，Boost则在匹配较长字符串时胜出---但两者差距很小，考虑到PCRE的大小和易用性，我们可以认为PCRE更值得考虑。

        PCRE被广泛使用在许多开源软件之中，最著名的莫过于Apache HTTP服务器和PHP脚本语言、R脚本语言，此外，正如从其名字所能看到的，PCRE也是perl语言的缺省正则库。

        PCRE是用C语言实现的，其C++实现版本叫PCRE++。

### 2.PCRE正则基础知识

PCRE正则表达式主要用于字符串的模式分割、匹配、查找及替换操作。

正则中重要的几个概念有：元字符、转义、模式单元（重复）、反义、引用和断言。我们简单列一下常用字符：

**常用的元字符**(Meta-character)

        元字符    说明

        \A     匹配字符串串首的原子

        \Z     匹配字符串串尾的原子

        \b     匹配单词的边界/\bis/匹配头为is的字符串/is\b/ 匹配尾为is的字符串 /\bis\b/ 定界

        \B     匹配除单词边界之外的任意字符   /\Bis/   匹配单词“This”中的“is”

        \d      匹配一个数字；等价于[0-9]

        \D     匹配除数字以外任何一个字符；等价于[^0-9]

        \w     匹配一个英文字母、数字或下划线；等价于[0-9a-zA-Z_]

        \W    匹配除英文字母、数字和下划线以外任何一个字符；等价于[^0-9a-zA-Z_]

        \s      匹配一个空白字符；等价于[\f\t\v]   

        \S      匹配除空白字符以外任何一个字符；等价于[^\f\t\v]

        \f       匹配一个换页符等价于 \x0c 或 \cL

        \n      匹配一个换行符；等价于 \x0a 或 \cJ

        \r       匹配一个回车符等价于\x0d 或 \cM

        \t       匹配一个制表符；等价于 \x09\或\cl

        \v      匹配一个垂直制表符；等价于\x0b或\ck

        \oNN    匹配一个八进制数字

        \xNN     匹配一个十六进制数字

        \cC       匹配一个控制字符

**模式修正符**（Pattern Modifiers）

        模式修正符在忽略大小写、匹配多行中使用特别多，掌握了这一个修正符，往往能解决我们遇到的很多问题。

        i     －可同时匹配大小写字母

        M     －将字符串视为多行

        S     －将字符串视为单行，换行符做普通字符看待，使“.”匹配任何字符

        X     －模式中的空白忽略不计 

        U     －匹配到最近的字符串

        e     －将替换的字符串作为表达使用

       例子：/apple/i匹配“apple”或“Apple”等等，忽略大小写。

# PCRE库的安装（linux）

PCRE官网地址：[http://www.pcre.org/](http://www.pcre.org/)

安装包下载地址：[https://sourceforge.net/projects/pcre/files/pcre/](https://sourceforge.net/projects/pcre/files/pcre/)

解压安装参考：[linux 下安装 tar.gz包](https://blog.csdn.net/miner_zhu/article/details/83149361)

# PCRE库的使用

### 1.PCRE库的函数接口

PCRE共提供了19个接口函数。我们这里只介绍了几个主要和常用的接口函数，其中**前四个**函数是我们调用PCRE最主要使用的接口。
**1.pcre_compile**
pcre *pcre_compile(const char *pattern, int options,
            const char **errptr, int *erroffset,
            const unsigned char *tableptr);
功能：将一个正则表达式编译成一个内部表示，在匹配多个字符串时，可以加速匹配。
参数：pattern：  正则表达式
      options：  用来指定编译时的一些选项（选0，或者其他参数选项）
      errptr：   出错信息
      erroffset：出错位置
      tableptr： 用来指定一个字符数组，一般情况设置为NULL, 使用缺省的字符表
返回值：被编译好的正则表达式的pcre内部表示
例子：
**2**.**pcre_compile2**
pcre *pcre_compile2(const char *pattern, int options,
             int *errorcodeptr,const char **errptr, int *erroffset,
            const unsigned char *tableptr);
功能：其同pcre_compile功能一样只是多一个参数errorcodeptr。
参数： errorcodeptr：存放出错码
**3. pcre_exec**
int pcre_exec(const pcre *code, const pcre_extra *extra,
            const char *subject, int length, int startoffset,
            int options, int *ovector, int ovecsize);
功能：用来检查某个字符串是否与指定的正则表达式匹配
参数： code,    编译好的正则表达结构的指针
      extra,    用来向pcre_exec传一些额外的数据信息的结构的指针，可以为NULL
      subject,  需要匹配的字符串
      length,   匹配的字符串长度
      startoffset, 匹配的开始位置
      options,  选项位
      ovector,  用来返回匹配位置偏移量的数组
      ovecsize, 数组大小
返回值：匹配成功返回非负数，匹配返回负数**4.pcre_study**
pcre_extra *pcre_study(const pcre *code, int options,
                       const char **errptr);
功能：对编译的模式进行学习，提取可以加速匹配过程的信息。
参数： code, 编译好的正则表达结构的指针
      options, 选项位
      errptr， 出错消息
**5.pcre_version**
char *pcre_version(void)
功能：返回PCRE的版本信息
**6.pcre_config**
int pcre_config(int what, void *where)
功能：查询当前PCRE版本中使用的选项信息
参数： what   选项名
      where  存储结果的位置
**7.pcre_maketables**
const unsigned char *pcre_maketables(void)
功能：生成一个字符表，表中每一个元素的值不大于256，可以用它传给pcre_compile()替换掉内建的字符表。

### 2.PCRE使用过程

**   PCRE/PCRE++使用注意事项**

（1）使用pcre请包含pcre.h头文件

（2）使用pcre_compile, pcre_exec后，记得调用pcre_free释放内存，以免造成内存泄露

（3）使用pcre编译的时候需要依赖libpcre.a（**apt-get install libpcre3 libpcre3-dev**）

使用PCRE的过程。主要分三步，第一步编译正则表达式；第二匹配正则表达式；第三步释放正则表达式。

**1．编译正则表达式**

为了提高效率，在将一个字符串与正则表达式进行比较之前，首先要用pcre_compile() /pcre_compile2() 函数对它时行编译，转化成PCRE引擎能够识别的结构（struct real_pcre）。

这里还可以调用pcre_study()函数，对编译后的正则表达式结构（struct real_pcre）时行分析和学习，学习的结果是一个数据结构（struc pcre_extra），这个数据结构连同编译后的规则（struct real_pcre）可以一起送给pcre_exec单元进行匹配。

**2. 匹配正则表达式**

一旦用函数pcre_compile() /pcre_compile2()成功地编译了正则表达式，接下来就可以调用pcre_exec（）函数完成模式匹配。根据正则表达式到指定的字符串中进行查找和匹配,并输出匹配的结果。

**3. 释放正则表达式**

无论什么时候，当不再需要已经编译过的正则表达式时，都应该调用函数free()将其释放，以免产生内在泄漏。


### **3.PCRE实例**

```cpp
#include <pcre.h>
#include <stdio.h>
#include <string.h>
 
int main(int argc, char ** argv)
{
    if (argc != 3)
    {   
        printf("Usage: %s pattern text\n", argv[0]);
        return 1;
    }   
 
    const char * pPattern = argv[1];
    const char * pText = argv[2];
    const char * pErrMsg = NULL;
    pcre * pPcre = NULL;
    int nOffset = -1; 
 
    if (NULL == (pPcre = pcre_compile(pPattern, 0, &pErrMsg, &nOffset, NULL)))
    {   
        printf("ErrMsg=%s, Offset=%d\n", pErrMsg, nOffset);
        return 1;
    }   
    else
    {   
        if (pcre_exec(pPcre, NULL, pText, strlen(pText), 0, 0, NULL, 0) < 0)
        {   
            printf("%s doesn't match %s\n", pText, pPattern);
        }   
        else
        {   
            printf("%s matches %s\n", pText, pPattern);
        }
    }
}
```

注意，如果你在编译时出现如下提示：没有定义pcre.h文件里面的函数时，是因为没有链接到库文件里，这时可以能过添加一个lpcre即可。然后重新编译便可成功。

![](https://img-blog.csdn.net/20181019211827399?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21pbmVyX3podQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

运行结果：

![](https://img-blog.csdn.net/20181019211723443?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21pbmVyX3podQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 参考文章

[pcre使用例子](http://blog.chinaunix.net/uid-26575352-id-3517146.html)

[深入浅出C/C++中的正则表达式库(三)——PCRE, PCRE++](http://www.wuzesheng.com/?p=994)



