# C++ 中的替换空格 - 文章 - 伯乐在线
原文出处： [sanqima，2015-01-03](http://blog.csdn.net/sanqima/article/details/42360949)
例，请实现一个函数，把字符串中的每个空格替换成“%20”。例如输入“We are happy.”，则输出“We%20are%20happy.”。
分析：
我们可以先遍历一次字符串，这样就能统计出字符串中空格的总数，并可以由此计算出替换之后的字符串的总长度。每替换一个空格，长度增加2，因此替换以后字符串的长度等于原来的长度加上2乘以空格数目。我们还是以前面的字符串’We are happy,”为例，”We are happy‘”这个字符串的长度是14(包括结尾符号”)，里面有两个空格，因此替换之后字符串的长度是18。
我们从字符串的后面开始复制和替换。首先准备两个指针，P1和P2。P1指向原始字符串的末尾，而P2指向替换之后的字符串的末尾(如图(a)所示)。接下来我们向前移动指针P1，逐个把它指向的字符复制到P2指向的位置，直到碰到第一个空格为止。此时字符串包含如图（b)所示，灰色背景的区域是做了字符拷贝(移动)的区域。碰到第一个空格之后，把Pl向前移动1格，在P2之前插入字符串”%20″。由于”%20″的长度为3，同时也要把P2向前移动3格如图(c)所示。
我们接着向前复制，直到到碰到第二个空格(如图2.4(d)所示)。和上次一样，我们再把P1向前移动1格，并把P2向前移动3格插入”%20″(如图2.4(e)所示)。此时P1和P2指向同一位置，表明所有空格都已经替换完毕。   从上面的分析我们可以看出，所有的字符都只复制(移动)一次，因此这个算法的时间效率是O(n)，效率比较高。
![](http://jbcdn2.b0.upaiyun.com/2016/12/3995dbfad0654f2d87cb85a3af9159ef.png)
注:图中带有阴影的区域表示被移动的字符。
(a)把第一个指针指向原字符串的末尾，把第二个指针指向替换之后的字符串的末尾。
(b)依次复制字符串的内容，直至第一个指针碰到第一个空格。
(c)把倒数第一个空格替换成”%20″，接着，把第一个指针向前移动1格，把第二个指针向前移动3格。
(d)依次向前复制字符串中的字符，直至碰到空格。
(e)替换字符串中的倒数第二个空格，接着，把第一个指针向前移动1格，把第二个指针向前移动3格。
在面试的过程中，我们也可以和前面的分析一样，画一两个示意图，解释自己的思路，这样既能帮助我们理清思路，也能使我们和面试官的交流变得更加高效。在面试官肯定我们的思路之后，就可以开始写代码了。下面是参考代码：


```
//length为字符数组string的总容量  
void ReplaceBlank(char str[],int length)  
{  
    if(str==NULL && length<=0)  
        return;  
  
    //originalLength为字符串str的实际长度  
    int originalLength=0;  
    int numberOfBlank=0;  
    int i=0;  
    while (str[i]!='\0')  
    {  
        ++originalLength;  
        if(str[i]==' ')  
            ++numberOfBlank;  
        ++i;  
    }  
  
    //newLength为把空格替换成'%20'之后的长度  
    int newLength=originalLength + numberOfBlank*2;  
    if(newLength>length)  
        return;  
      
    int indexOfOriginal=originalLength;  
    int indexOfNew=newLength;  
    while (indexOfOriginal>=0 && indexOfNew>indexOfOriginal)  
    {  
        if (str[indexOfOriginal]==' ')  
        {  
            str[indexOfNew--]='0';  
            str[indexOfNew--]='2';  
            str[indexOfNew--]='%';  
        }   
        else  
        {  
            str[indexOfNew--]=str[indexOfOriginal];  
        }  
  
        --indexOfOriginal;  
    }  
  
}
```
测试用例：
1)输入的字符串中包含空格(空格位于字符串的最前面，空格位于字符串的最后面，空格位于字符串的中间，字符串中有连续多个空格)。
2)输入的字符串中没有空格。
3)特殊输入测试(字符串是个NULL指针、字符串是个空字符串、字符串只有一个空格字符、字符串中只有连续多个空格)。
//测试代码：


```
void Test(char* testName, char string[], int length, char expected[])  
{  
    if(testName != NULL)  
        printf("%s begins: ", testName);  
  
    ReplaceBlank(string, length);  
  
    if(expected == NULL && string == NULL)  
        printf("passed.\n");  
    else if(expected == NULL && string != NULL)  
        printf("failed.\n");  
    else if(strcmp(string, expected) == 0)  
        printf("passed.\n");  
    else  
        printf("failed.\n");  
}  
  
// 空格在句子中间  
void Test1()  
{  
    const int length = 100;  
  
    char string[length] = "hello world";  
    Test("Test1", string, length, "hello%20world");  
}  
  
// 空格在句子开头  
void Test2()  
{  
    const int length = 100;  
  
    char string[length] = " helloworld";  
    Test("Test2", string, length, "%20helloworld");  
}  
  
// 空格在句子末尾  
void Test3()  
{  
    const int length = 100;  
  
    char string[length] = "helloworld ";  
    Test("Test3", string, length, "helloworld%20");  
}  
  
// 连续有两个空格  
void Test4()  
{  
    const int length = 100;  
  
    char string[length] = "hello  world";  
    Test("Test4", string, length, "hello%20%20world");  
}  
  
// 传入NULL  
void Test5()  
{  
    Test("Test5", NULL, 0, NULL);  
}  
  
// 传入内容为空的字符串  
void Test6()  
{  
    const int length = 100;  
  
    char string[length] = "";  
    Test("Test6", string, length, "");  
}  
  
//传入内容为一个空格的字符串  
void Test7()  
{  
    const int length = 100;  
  
    char string[length] = " ";  
    Test("Test7", string, length, "%20");  
}  
  
// 传入的字符串没有空格  
void Test8()  
{  
    const int length = 100;  
  
    char string[length] = "helloworld";  
    Test("Test8", string, length, "helloworld");  
}  
  
// 传入的字符串全是空格  
void Test9()  
{  
    const int length = 100;  
  
    char string[length] = "   ";  
    Test("Test9", string, length, "%20%20%20");  
}
```
//主函数


```
int _tmain(int argc, _TCHAR* argv[])  
{  
    Test1();  
    Test2();  
    Test3();  
    Test4();  
    Test5();  
    Test6();  
    Test7();  
    Test8();  
    Test9();  
  
    return 0;  
}
```
效果如下：
![](http://jbcdn2.b0.upaiyun.com/2016/12/e3e78cf9ace207e54b3c6138797cd591.png)
参考文献：何海涛.《剑指Offer名企面试官精讲典型编程题》.2012年.电子工业出版社
