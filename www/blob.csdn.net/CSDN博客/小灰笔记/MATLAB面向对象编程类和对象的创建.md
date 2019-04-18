# MATLAB面向对象编程类和对象的创建 - 小灰笔记 - CSDN博客





2017年05月16日 08:27:34[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：6989








      在此之前对面向对象编程了解不多，更不用说MATLAB面向对象编程。不过，在第一份工作的经历中还是遇到过一点使用，只是那时候我还不是很清楚。那一次使用是为了实现Simulink中模型的使用。

      今天简单做一个类的构建以及对象的创建。首先实现一个类的定义，代码如下：

classdef Person

properties

        name;

        age;

        birth_year;

end

methods

function obj = Person(n,a,b)

            obj.name = n;

            obj.age = a;

            obj.birth_year = b;

end

function Show(obj)

            disp(['name: ',obj.name]);

            disp(['age: ',num2str(obj.age)]);

            disp(['birth_year: ',num2str(obj.birth_year)]);

end

end

end



      类的定义首先需要指定一个关键词作为类的名称，接下来的内容中分为属性以及方法两部分。其中，方法中与类名相同的函数是类的构造函数。对类的使用示范如下：

>> Tom = Person('Tom',10,2007)



Tom = 



  Person withproperties:



          name:'Tom'

           age:10

    birth_year:2007



>> Tom.Show()

name: Tom

age: 10

birth_year: 2007

      通过上面的这种使用方式即可实现通过已经定义的类创建一个对象并且调用对象所具备的方法。总体上看起来，这些方法也并不是很难。后期的使用中，看看是否能够在生活或者工作中把面向对象的技术运用到位。



