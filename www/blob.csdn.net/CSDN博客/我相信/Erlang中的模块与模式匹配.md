# Erlang中的模块与模式匹配 - 我相信...... - CSDN博客





2015年03月05日 15:17:11[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1811








模块是Erlang的基本代码单元，erl文件编译后以.beam作为扩展名，采用UTF8字符集，.erl文件示意如下：




-module(模块名，与存放模块的文件名相同)

-export([方法名/输入参数的个数])

Method1( {a,b,c})->a*b*c;

Mehtod2({d,e})->d-e.

模块属性有两种类型：预定义型和用户定义型。



Erlang中用于代表函数的数据类型被称为fun,相当于python中的lambda，一般用于

1)      对列表里的每个元素执行相同的操作

2)      创建自己的控制

3)      实现可重入解析代码，解析组合器或者lazy evaluator



模式匹配是Erlang的根基，case和if表达式使Erlang小而一致。

case Expression  of

         Pattern1[ when Guard1] -> Expr-seq1;

         Pattern2[when Guard2]-> Expr-seq2;

         …

end

if 

         Guard1-> Expr_seq1;

         Guard2-> Expr_seq2;

         …

end



Erlang有两种方法来捕捉异常错误，一种是把抛出异常的调用函数封装在一个try_catch 表达式里，提供了概括信息，另一种是把调用封装在一个catch表达式里，提供了详细的栈跟踪信息。在捕捉到一个异常后，可以调erlang:get_stacktrace()来找到最近的栈信息。



把二进制型，位串，和位级模式匹配引入Erlang是为了简化网络编程。二进制型是置于双小于号和双大于号之间的一列整数或字符串。

例如： 1> Mybin1 =  << “ILOVE YOU”>>




Term_to_bingary(Term) ->Bin 转换为二进制型

Binary_to_Term(Bin) ->Term 二进制型转换为Erlang的数据类型

精心选择宏的名称和Erlang代码布局，能最大限度地缩小C和Erlang的语义鸿沟。在Erlang里，最小的寻址单元是1位，位串里的位序列可直接访问。



