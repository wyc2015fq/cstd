# MATLAB面向对象编程指定类的优先级 - 小灰笔记 - CSDN博客





2017年05月22日 22:42:01[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：355










      前面总结了使用类的时候类的默认优先级，接下来总结练习一下指定类的优先级。

      把之前用过的代码修改如下：

classA.m

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



classB.m

classdef (InferiorClasses = {?classA}) classB

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



      利用创建的两个类进行操作，记录如下：

>> obj_classA = classA(3)



obj_classA = 



  classA withproperties:



    a: 3



>> obj_classB = classB(4)



obj_classB = 



  classB withproperties:



    b: 4



>> MyMethod(obj_classA,obj_classB)

The MyMethod of classB is called!

>> MyMethod(obj_classB,obj_classA)

The MyMethod of classB is called!

      与之前使用默认的优先级相比，执行结果不同。这次，不管方法中传入参数如何改变，最终方法都调用了classB的方法。这说明了classB的优先级要高于classA的优先级。

      正常来说，在指定低级优先级的列表中，类的优先级也是由左到右依次递减。总体看来，这还是遵循“先入为主”的原则。





