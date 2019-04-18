# 把inline函数的定义放在头文件中 - weixin_33985507的博客 - CSDN博客
2013年04月06日 00:56:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8

inline是加在实现上，就算加在声明上，编译器也会忽略掉。内联展开是在编译时进行的，只有链接的时候源文件之间才有关系。所以内联要想跨源文件必须把实现写在头文件里。如果一个inline函数会在多个源文件中被用到，那么必须把它定义在头文件中。
注意：将函数的定义（{…}之间的部分）放在头文件中是强制的，除非该函数仅仅被单个 .cpp 文件使用。尤其是，如果你将内联函数的定义放在 .cpp 文件中并且在其他 .cpp文件中调用它，连接器将给出 “unresolved external” 错误。
最佳实践是：仅把inline关键字放在类外部函数的定义前。
inline函数的特征是在调用的地方插入相应函数的代码，所以编译之后的目标文件里是没有inline函数体的，因为在要调用的地方它都已经用相应的语句替换掉了（当然这只限于内联成功的情况）。如果我们将inline函数写在cpp文件里，但是绝大多数情况下，在我们用第三方类库的时候，我们只有头文件和目标文件（没有cpp文件），当你调用那个内联函数时，编译器没办法找到它。所以说将inline函数写在cpp文件中是没什么用的。
原文
http://bbs.csdn.net/topics/10198835
http://www.caole.net/diary/inline_function.html
http://huxiongwei.spaces.eepw.com.cn/articles/article/item/85841
