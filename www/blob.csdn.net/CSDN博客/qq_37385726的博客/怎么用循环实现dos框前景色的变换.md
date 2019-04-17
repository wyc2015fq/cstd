# 怎么用循环实现dos框前景色的变换~ - qq_37385726的博客 - CSDN博客





2017年02月04日 11:47:23[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：88标签：[C语言																[函数																[dos](https://so.csdn.net/so/search/s.do?q=dos&t=blog)
个人分类：[C语言](https://blog.csdn.net/qq_37385726/article/category/6697679)








***一：关于sprintf_s()函数~***

*作用：将 sprintf(A,“ ” )字符串里面的内容写入A中*


例子：
char* who = "I";
char* whom = "[CSDN](https://www.baidu.com/s?wd=CSDN&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1YdPAfkrHm4uHwbPvuBuW6Y0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3En1nkPWRsnH0v)";
sprintf_s(s, "%s love %s.", who, whom); //作用：**"I love [CSDN](https://www.baidu.com/s?wd=CSDN&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1YdPAfkrHm4uHwbPvuBuW6Y0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3En1nkPWRsnH0v). "  这字符串写到s中*****二：为什么要用sprintf_s()函数~***因为system("color  xx")中xx是不能有变量的，故想了这个办法，借用sprintf_s函数](https://so.csdn.net/so/search/s.do?q=函数&t=blog)](https://so.csdn.net/so/search/s.do?q=C语言&t=blog)




