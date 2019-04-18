# C中#号的用法（宏转换字符串） - ljx0305的专栏 - CSDN博客
2010年04月07日 17:19:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1114
最近发现在C中有很多的东西值得我们学习。
C中的#号不仅是#include，#define等的关键字首使用而已，#在宏中还有很多用法。具体请看下面我来举例
1.双#号的宏用法。
我相信双##号的用法应该大部分人都见过吧，主要是用来连接宏变量名使用，在GCC参考手册中这样记载。
可用于宏内部将两个源代码权标连接成一个的连接指示字，可用来构造不会被解析器错误解释的名字。例如，下面两个宏会实现连接操作：
#define PASTE1(a) a##house
#define PASTE2(a,b) a##b
result = PASTE1(farm);
result = PASTE1(ranch);
result = PASTE2(front,back);
下面是上面预处理的结果
result = farmhouse;
result =ranchhouse;
result = frontback;
这就是双##的用法。
2.单#号的用法。对于单#号的用法。我用一个例子来描述一下
#define tostring(s)                       #s
#define STRING       "Lavf" tostring(52 ##.## 36 ##.## 0)
int main()
{
char *p = STRING;
    printf("%s", p);
    getchar();
 return 0;    
}
上面的结果是
Lavf52.36.0
此文章仅作参考，若我的理解有更新，我会更改对本文章进行修改，并且希望知道#号用法的朋友能给予我宝贵的建议。谢谢大家看我的文章
