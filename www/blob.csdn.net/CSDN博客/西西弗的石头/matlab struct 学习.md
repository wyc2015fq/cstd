# matlab struct 学习 - 西西弗的石头 - CSDN博客





2014年01月07日 16:09:13[hzhaly](https://me.csdn.net/hzhaly)阅读数：2335









转载地址：[http://blog.csdn.net/wangzhix123/article/details/8807737](http://blog.csdn.net/wangzhix123/article/details/8807737)


在matlab里面，struct结构体数组功能强大，在处理很多复杂数据上很有用，刚好最近需要用到，顺便学习总结一下。

**1，什么是结构体？**


              简单的说，结构体是一个可包含不同数据类型的结构。可以在一个结构体中声明不同的数据类型，这点跟数据区别很大，也是它优势的体现。举一个例子来说，比如班里面有53个学生，每个学生有学号，名字，性别等三个身份特征，那么我们可以用结构体来描述这53个学生，每个学生是一个结构体，结构体含有三个字段，分别是‘学号’，‘名字’，‘性别’。这样由53个结构体组成的一个大的结构体便可以表示这53个学生。例如，student（‘number’，‘name’，‘sex’）




**2 ，如何定义结构体**

**MATLAB**提供了两种定义结构的方式：直接应用和使用struct函数。

            2.1. 使用直接引用方式定义结构


> 
     与建立数值型数组一样，建立新struct对象不需要事先申明，可以直接引用，而且可以动态扩充。比如建立一个上面的学生：

student.number = 2013001; % 创建字段名为number，并为该字段赋值为2013001

student.name ='王尼玛' % 为student创建一个新的字段name，并为该字段赋值‘王尼玛’

student.sex ='female' % 为student创建一个新的字段female，并为该字段赋值‘female’


student =

number: 2013001

name: '王尼玛'

sex：female

然后可以将旗动态扩充为数组：

student（2）.number = 2013002; % 将student扩充为1×2的结构数组

student(2).name =‘曹尼玛’;

在任何需要的时候，也可以为数组动态扩充字段，如增加字段age：

student(1).age = 18;

这样，所有student都增加了一个age字段，而student(1)之外的其他变量的age字段为空：

student(1) % 查看结构数组的第一个元素的各个字段的内容

ans =

number: 2013001

name: '王尼玛'

sex：female

age: 18



student(2) % 查看结构数组的第二个元素的各个字段的内容，注意没有赋值的字段为空

ans =




> 
number: 2013001

name: '曹尼玛'

sex：[]

age:  []


> 


应该注意的是，student的number、name、age字段不一定是单个数据元素，它们可以是任意数据类型，可以是向量、数组、矩阵甚至是其他结构变量或元胞数组，而且不同字段之间其数据类型不需要相同。

数组中不同元素的同一字段的数据类型也不要求一样;甚至还可以通过引用数组字段来定义结构数据类型的某字段。

2.2 使用struct函数创建结构

使用struct函数也可以创建结构，该函数产生或吧其他形式的数据转换为结构数组。

struct的使用格式为：

s = sturct('field1',values1,'field2',values2,…);

该函数将生成一个具有指定字段名和相应数据的结构数组，其包含的数据values1、valuese2等必须为具有相同维数的数据，数据的存放位置域其他结构位置一一对应的。对于struct的赋值用到了元胞数组。数组values1、values2等可以是**元胞数组**、标量元胞单元或者单个数值。每个values的数据被赋值给相应的field字段。

当valuesx为元胞数组的时候，生成的结构数组的维数与元胞数组的维数相同。而在数据中不包含元胞的时候，得到的结构数组的维数是1×1的。例如：

s = struct('number',{'2013001','2013002'},'name',{'王尼玛','曹尼玛'},'sex',{’female‘,‘male’})

s = 

1x2 struct array with fields:

number

name

sex

得到维数为1×2的结构数组s，包含了number、name和sex共3个字段。


其中，我们在matlab里面创建很多struct并大量赋值的时候，可能经常会遇到这样的提示，

![](http://blog.csdn.net/wangzhix123/article/details/8807737)


The variable 'xx' appears to change size on every loop iteration (within a script).Consider preallocating for speed.

这种因为在循环里动态增加结构体而导致效率低下的问题就可以通过事先定义来解决，如：

a=cell（1,500）% 具体大小可根据实际需求来定

student=struct（‘name’，a,'number',a）

则会得到一个1*500的结构体。




**3，如何操作结构体**

> 
**MATLAB**中专门用于对结构数组的操作的函数并不多，通过 help datatypes获取数据类型列表，可以看到其中的结构数据类型的有关的函数，主要如表4.3.1所示。

表4.3.1 结构数组的操作函数

函数名   功能描述                                               函数名            功能描述

deal        把输入处理成输出                            fieldnames   获取结构的字段名

getfield   获取结构中指定字段的值                rmfield          删除结构的字段(不是字段内容)

setfield   设置结构数组中指定的字段的值    struct             创建结构数组

struct2cell   结构数组转化成元胞数组           isfield            判断是否存在该字段

isstruct   判断某变量是否是结构类型  




