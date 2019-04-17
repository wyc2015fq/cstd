# VS奇奇怪怪的函数unsafe~ - qq_37385726的博客 - CSDN博客





2017年02月04日 11:55:38[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：118








像scanf函数，sprintf函数都会被标以不安全的错误~

那么该怎么办呢？




===================================================================

**百度的回答：**


scanf()在读取时不检查边界，所以可能会造成内存泄露，所以VS2013提供了[scanf_s](https://www.baidu.com/s?wd=scanf_s&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1YdP1whnAcLPjf1n1whnhcY0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnH6vnW0dPWTkPHT4PWcsPH6vPs)()来替代，在调用时，必须提供给[scanf_s](https://www.baidu.com/s?wd=scanf_s&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1YdP1whnAcLPjf1n1whnhcY0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnH6vnW0dPWTkPHT4PWcsPH6vPs)一个数字以表明读取多少位字符，用法为[scanf_s](https://www.baidu.com/s?wd=scanf_s&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1YdP1whnAcLPjf1n1whnhcY0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnH6vnW0dPWTkPHT4PWcsPH6vPs)("%c",&letter,sizeof(letter))，否则，即使Ctrl+F7和Ctrl+F5通过了，程序的运行结果也可能与自己所期望大相径庭的。

如果非要用scanf函数，那么通过以下两种方式可以禁止警告：
（1）右击项目——属性——[C/C++](https://www.baidu.com/s?wd=C%2FC%2B%2B&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1YdP1whnAcLPjf1n1whnhcY0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnH6vnW0dPWTkPHT4PWcsPH6vPs)——[预处理器](https://www.baidu.com/s?wd=%E9%A2%84%E5%A4%84%E7%90%86%E5%99%A8&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1YdP1whnAcLPjf1n1whnhcY0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnH6vnW0dPWTkPHT4PWcsPH6vPs)——[预处理器](https://www.baidu.com/s?wd=%E9%A2%84%E5%A4%84%E7%90%86%E5%99%A8&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1YdP1whnAcLPjf1n1whnhcY0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnH6vnW0dPWTkPHT4PWcsPH6vPs)定义中增加_CRT_SECURE_NO_WARNINGS，然后点确定，退出。
（2）在函数最顶头进行定义：#define _CRT_SECURE_NO_WARNINGS，也可以禁止报错警告。===================================================================






*但是，根据切身经验来选择，个人强烈推荐直接scanf_s(),sprintf_s()~*



