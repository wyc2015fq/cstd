# struct 与class的关系 - Ms.Fox的专栏 - CSDN博客
2015年03月04日 10:50:39[lulu-lu](https://me.csdn.net/smbluesky)阅读数：335
1、花括号初始化
class和struct 如果定义了构造函数，则都不能用花括号进行初始化，若无，则可以用花括号进行初始化。
struct A{int x；int y；}    A a={1,2}；
2、默认访问权限
struct 默认访问权限是public；
class 默认访问权限是private；
3、默认继承方式
struct 默认继承方式是public；
class 默认继承方式是private；
除此之外，class可以继承于struct，struct可以继承于class。
            
