# error C2276: '&' : illegal operation on bound member function expression - 家家的专栏 - CSDN博客





2010年08月30日 20:41:00[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：4320








问题: 

      error C2276: '&' : illegal operation on bound member function expression

解决方法:

    在相应函数的.h头文件中的函数前面,添上static.

    问题就可解决了...



