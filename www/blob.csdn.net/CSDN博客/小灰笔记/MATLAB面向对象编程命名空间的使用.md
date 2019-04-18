# MATLAB面向对象编程命名空间的使用 - 小灰笔记 - CSDN博客





2017年05月18日 01:50:19[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：480








      命名空间可以理解为专门的内存分区，在MATLAB面向对象编程中，命名空间通常通过包文件夹来实现。包文件夹是以+开头的，简称为包。

      创建以下包结构：

├─+Pack1

│     MyClassA.m

│     MyClassB.m

│

└─+Pack2

       MyClassA.m

       MyClassB.m

      Pack1中文件中的代码分别如下：

MyClassA.m

classdef MyClassA

properties

        age;

        hight;

end

methods

function obj = MyClassA(a,h)

            obj.age = a;

            obj.hight = h;

end

function Show(obj)

            disp(['age for ClassA in pack1: ',num2str(obj.age)]);

            disp(['hight for ClassA in pack1: ',num2str(obj.hight)]);

end

end

end



MyClassB.m

classdef MyClassB

properties

        age;

        hight;

end

methods

function obj = MyClassB(a,h)

            obj.age = a;

            obj.hight = h;

end

function Show(obj)

            disp(['age for ClassB in pack1: ',num2str(obj.age)]);

            disp(['hight for ClassB in pack2: ',num2str(obj.hight)]);

end

end

end



Pack2中文件代码如下：

MyClassA.m

classdef MyClassA

properties

        age;

        hight;

end

methods

function obj = MyClassA(a,h)

            obj.age = a;

            obj.hight = h;

end

function Show(obj)

            disp(['age for ClassA in pack2: ',num2str(obj.age)]);

            disp(['hight for ClassA in pack2: ',num2str(obj.hight)]);

end

end

end



MyClassB.m

classdef MyClassB

properties

        age;

        hight;

end

methods

function obj = MyClassB(a,h)

            obj.age = a;

            obj.hight = h;

end

function Show(obj)

            disp(['age for ClassB in pack2: ',num2str(obj.age)]);

            disp(['hight for ClassB in pack2: ',num2str(obj.hight)]);

end

end

end



      使用这些类构建对象并并简单执行方法过程如下：

>> A1 = Pack1.MyClassA(12,170)



A1 = 



  MyClassA withproperties:



      age: 12

    hight: 170



>> B1 = Pack1.MyClassB(1,2)



B1 = 



  MyClassB withproperties:



      age: 1

    hight: 2



>> A1.Show()

age for ClassA in pack1: 12

hight for ClassA in pack1: 170

>> B1.Show()

age for ClassB in pack1: 1

hight for ClassB in pack2: 2

>> A2 = Pack2.MyClassA(2,1)



A2 = 



  MyClassA withproperties:



      age: 2

    hight: 1



>> A2.Show()

age for ClassA in pack2: 2

hight for ClassA in pack2: 1

>> B2 = Pack2.MyClassB(2,2)



B2 = 



  MyClassB withproperties:



      age: 2

    hight: 2



>> B2.Show()

age for ClassB in pack2: 2

hight for ClassB in pack2: 2

>> 

      简单小结：

            1，使用包管理的类在使用的时候需要加包的名字；

            2，不同包中可以有相同名称的类；

            3，不同包中相同名称类可以不同；

            4，不同包中自然也可以有不同名称的类；



