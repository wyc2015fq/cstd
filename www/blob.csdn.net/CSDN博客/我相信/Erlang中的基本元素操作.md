# Erlang中的基本元素操作 - 我相信...... - CSDN博客





2015年03月05日 13:55:50[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1833








Erlang shell中，用句号加空格、tab或回车来结束表达式,%表示注释的起点，;隔离子句。模块是.erl 文件，库的头文件.hrl, shell中的编译时c(),外编译命令时erlc, 退出shell用q()，或erlang:halt(). 




变量以大写字母开头，且不能重新绑定变量，只能一次性赋值，具有不可变状态。原子是全局的，不需要宏定义或包含文件，以小写字母开头，还可放在单引号内，是极简表达式。



元组(tuple)是一些数量固定的项目归组成单一实体{,}, 由于是匿名的，通常在第一个元素上贴标签，来增加可读性。提取元组中的值使用模式匹配操作符=，_为匿名变量，多个_不必绑定相同的值。但是，_Mode则是常规变量。例如：

2> Family={family,father,mother,son}.

{family,father,mother,son}

3> {_,X,Y,Z}=Family.

{family,father,mother,son}

4> X.

father

5> Y.

mother

6> Z.

Son




记录(record)是元组的另一种形式，可以给元组的各个元素关联一个名称。使用记录的情形：

1）  用一些预先确定且数量固定的原子表示数据

2）  元素数量和元素名称不会随时间改变

3）  大元组中每个元组有相同的结构

采用#myrecord{k1=v1,k2=v2..}来创建record。




列表(list)形如[，，]可以存放任意数量的事物。Head可以是任何事物，Tail通常仍然是个列表。只要用[…|T]构建一个列表，就应确保T是一个列表。同样使用模式匹配来提取列表中的元素。列表推导的常规形式：

[X||Qualifier1,Qualifier2,…]

X是任意一表达式，限定符qualifier可以生成器，位串生成器或过滤器。生成器的写法

Pattern<- ListExpr

反转一个列表时，要调用lists:reverse.



Erlang中没有字符串，字符串是个整数列表，”HelloCloud”是一个列表的简写，io:format来指定打印输出。



映射组(map)是键值对的关联性集合，内部作为有序计划存储，适用情形：

1）  键不能预知时来表示键值对数据结构

2）  存在大量不同键来表示数据

3）  效率不重要时的万能数据结构

4）  自解释型数据结构

5）  用来表示键值解析树，如xml或配置文件

6）  用Json来通信




映射组的语法：

#{key1 op val1,key2 op val2,…,KeyN op valN}




#后没有名称，op是=>或：=之一。 => 将现有键值更新为新值或给映射组增加一个新键值对。 ：=用于更新。 健不能包含任何变量，值可以包含未绑定变量，在模式匹配成功后绑定。




映射组可以通过io:format 里的~p选项输出，并用io:read 或file：consult读取。

Maps:to_json(Map)->Bin 转化为二进制型json

Maps:from_json(bin)-> Nap, 将二进制json转化为map




Json与映射组的对应关系：

1）  Json数字：Erlang的整数或浮点数

2）  Json字符串：Erlang二进制型

3）  Json列表：Erlang列表

4）  true和false 对应

5）  映射组中的健必须是原子，字符串或二进制型，值必须用JSON的数据类型表示



