# matlab  Error using ==> mtimes Inner matrix dimensions must agree. - 家家的专栏 - CSDN博客





2011年03月09日 18:28:00[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：10352








??? Error using ==> mtimes Inner matrix dimensions must agree.
|**rho0和x均是向量****>> rhox = rho0*sqrt( 1 + ( (x - 0)/xd )*( (x - 0)/xd ) )****??? Error using ==> mtimes Inner matrix dimensions must agree.**上面的错误是因为向量相乘要在乘号前加上点号，也即下面的表达式才是正确的：**>> rhox = rho0.*sqrt( 1 + ( (x - 0)/xd ).*( (x - 0)/xd ) )**|
|----|

转自：[http://hi.baidu.com/linzch/blog/item/f667544321e4141d9313c6e7.html](http://hi.baidu.com/linzch/blog/item/f667544321e4141d9313c6e7.html)



