# error C2668: 'abs' : ambiguous call to overloaded function - 家家的专栏 - CSDN博客





2010年09月04日 18:52:00[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：6942








C:/Program Files/Microsoft Visual Studio 8/VC/include/math.h(539): could be 'long double abs(long double)'
1>        C:/Program Files/Microsoft Visual Studio 8/VC/include/math.h(491): or 'float abs(float)'
1>        C:/Program Files/Microsoft Visual Studio 8/VC/include/math.h(487): or 'double abs(double)'
1>        C:/Program Files/Microsoft Visual Studio 8/VC/include/math.h(485): or 'long abs(long)'
1>        C:/Program Files/Microsoft Visual Studio 8/VC/include/stdlib.h(415): or 'int abs(int)'



解决方法：在报错的地方做强制转换就是了。

例如远程序段如：if (abs (dwRet - 2048000) <= 16000)，这样abs不知道该调用哪个，所以需要做以下修改：if(abs ((long)(dwRet - 2048000)) <= 16000)



转自： [http://hi.baidu.com/peter_niu_alex/blog/item/81b5e71ea9f98a184134170b.html](http://hi.baidu.com/peter_niu_alex/blog/item/81b5e71ea9f98a184134170b.html)





