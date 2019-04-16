# scala简要：注解 - 我相信...... - CSDN博客





2015年01月02日 14:55:38[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：2744








                注解可以在程序的各个条目中添加信息，是插入到代码中以便有工具可以对他们进行处理的标签。可以对是scala类使用java注解，也可以使用scala特有的注解。

在scala中，可为类，方法，字段，局部变量和参数添加注解。Java注解的参数类型只能是：
1）数值型变量
2）字符串
3）类变量
4）java枚举
5）其他注解
6）上述类型的数组。

如果要实现一个新的Java注解，则需要用Java来编写该注解类。scala用@clonable和@remote来标记可被克隆的和远程的对象。@varargs注解可以从Java调用Scala的带有变长参数的方法。

Scala类库中的有些注解可以控制编译器的优化，@tailrec 用于消除递归，@switch 注解可以检查scala的match语句是否真的被编译成了跳转表，用@inline来建议编译器做内联，@editable给那些可以在生产代码中移除的方法打上标记，对被省略的方法的调用，编译器会替换成Unit对象，@uncheckVariance会取消与型变相关的错误提示。            


