# MATLAB面向对象编程get与set方法实现 - 小灰笔记 - CSDN博客





2017年05月19日 01:18:12[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1408








      在MATLAB面向对象编程中，set与get分别是重新定义参数设置以及查询功能的方法，类似于Java中类似的功能。在实现方式上，有一点区别差异。另外，在实现方式上，暂时没有发现MATLAB可以直接像eclipse中编辑Java代码那样生成默认的方法。

      写一个简单的例子演示set与get功能，创建以下类：

classdef GetSetDemo

properties

        ge;

        shi;

        bai;

end

methods

function obj = GetSetDemo(g,s,b)

            obj.ge = g;

            obj.shi = s;

            obj.bai = b;

end

function value = get.ge(obj)

            value = obj.ge;

end

function value = get.shi(obj)

            value = obj.shi;

end

function value = get.bai(obj)

            value = obj.bai;

end

function obj = set.ge(obj,value)

            obj.ge = value * 1;

end

function obj = set.shi(obj,value)

            obj.shi = value * 10;

end

function obj = set.bai(obj,value)

            obj.bai = value * 100;

end

end

end



      利用这个类创建对象：

>> GS = GetSetDemo(1,2,3)



GS = 



  GetSetDemowith properties:



     ge: 1

    shi: 20

    bai: 300



>> GS.ge



ans =



     1



>> GS.shi



ans =



    20



>> GS.bai



ans =



   300

      通过以上结果可以看出，原本默认的set以及get方式都被我们自己设计的方法所覆盖。



