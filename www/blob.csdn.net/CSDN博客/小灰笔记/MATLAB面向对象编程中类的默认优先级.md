# MATLAB面向对象编程中类的默认优先级 - 小灰笔记 - CSDN博客





2017年05月21日 23:54:43[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：420








      通过简单的测试，我自己总结出来的默认规则是：在MATLAB面向对象编程的类使用中，如果出现同名的方法，那么在调用的过程中MATLAB会采用先入为主的方式来判断其优先级。

      通过一个例子简单示范说明，首先定义两个类classA和classB，具体的代码如下：

classA:

classdef classA

properties

        a;

end

methods

function obj = classA(value)

            obj.a = value;

end

function MyMethod(obj,oj_classB)

            disp('The MyMethod of classA is called!');

end

end

end



classB

classdef classB

properties

        b;

end

methods

function obj = classB(value)

            obj.b = value;

end

function MyMethod(obj,obj_classB)

            disp('The MyMethod of classB is called!');

end

end

end



   再MATLAB的命令窗中进行测试操作，具体如下：

>> obj_classA = classA(3)



obj_classA = 



  classA withproperties:



    a: 3



>> obj_classB = classB(4)



obj_classB = 



  classB withproperties:



    b: 4



>> MyMethod(obj_classA,obj_classB)

The MyMethod of classA is called!

>> MyMethod(obj_classB,obj_classA)

The MyMethod of classB is called!

      通过上面的操作记录可以看出，当classA首先被传入的时候，方法最终采用了classA的方法。此时，classA具有高出classB的优先级。但是，反过来，先传入classB的时候情况则恰恰想法。这样，在结论上我们就可以给开篇的结论再加一条：在方法调用过程中，在没有指明优先级的情况下，优先级根据函数参数传入的顺序从左到右依次降低。



