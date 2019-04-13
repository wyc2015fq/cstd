
# Java字节码与Dalvik字节码 - 谢杨易的博客 - CSDN博客

2016年07月13日 12:40:46[谢杨易](https://me.csdn.net/u013510838)阅读数：933


JVM语言无关性
---JVM只与字节码关联，而不与Java语言直接关联。事实上，JRuby，Groovy等语言也可以由相应的编译器编译为字节码，然后由JVM解释执行。甚至可以自己写一个class字节码文件，然后由JVM来执行。

---class类文件结构

---任何一个class文件都对应着唯一一个类或接口的定义信息。


---class文件是一组8字节为基础单位的二进制流，各个数据项完全按照规范要求排列，中间无空隙。数据项只有两种：无符号数和表。

---无符号数分为u1，u2，u4，u8


---表是多个无符号数或者其他表作为数据项构成的复合数据类型，所有表都以_info结尾。


---由于中间无空隙，所以要表示同一类型，但长度不确定的数据时，一般会将数据长度放在前面，后面紧跟数据。


---字节码结构

![这里写图片描述](http://i2.piimg.com/567571/56daf95b6c511674.png)
---magic：4B，标示是Java class文件，为0xCAFEBABE


---version: class：文件版本号。JVM必须拒绝执行任何超过其版本号的class文件。


---constant_pool: 常量池，最复杂的区域。

---放在最前面的为常量池计数器。由于index0用来表示了“不引用任何常量池”，故index都是从1开始的。计数也是从1开始的。


---常量池存放了两大常量

---a) 字面量：字符串，常量值等
 		b) 符号引用：包括类的全限定名，字段名称和描述符，方法名称和描述符。


---access_flags: 访问标志

---由标志位组成，包括class是类还是接口，是否public，是否abstract，是否final，是否enum，annotation等


---this_class, super_class, interfaces: 类索引，父类索引和接口索引集合


---fields：字段集合表


---methods：方法集合表。


---attributes: 属性表，

---1) code：存放方法的字节码
 		2) constantValue：final字段的常量值
 		3) Deprecated：被声明为deprecated的方法和字段
 		4) exceptions：异常表
 		5) EnclosingMethod：当类为局部类（方法里面的内部类）时，才有这个字段。它表示类处在哪个方法中
 		6) InnerClasses：内部类列表

---字节码指令

---方法编译出的字节码会变成字节码指令，一般包含指令和操作数两部分。

---对于Hotspot，由于采用了栈结构，操作数一般放在栈中，只需要入栈和出栈。故字节码中一般只需要指令即可


---对于dalvik，由于采用了register结构，操作数需要从指令中读取，然后放到register中。故字节码中一般会包含指令和操作数。


---操作码严格控制为1B，故只能定义256种指令。但大大缩小了字节码文件大小


---大多数指令中包含了要处理的类型信息，如iload  fload

---但不可能所有的类型信息都包含，因为只有256种指令。解决方法：byte short带符号扩展为int，Boolean和char无符号扩展为int。故实际上，最终都是使用了int来作为运算类型。


---指令集：

---load, store：将数值从操作数栈存储到局部变量表，或从局部变量表加载到操作数栈中

---add sub mul div rem neg shl or and xor inc cmpg:

---运算指令，加减乘除，移位，位或与异或，比较等

---i2b，i2c：类型转换

---new，newarray，getfield，arraylength，instanceof：对象创建和访问

---pop pop2 swap dup dup2：操作数栈入栈和出栈

---ifeq ifne goto ret：分支控制

---invokeVirtual invokeInterface invokeStatic：方法调用和返回

---athrow：异常处理

---monitorenter，monitorexit：线程同步，用来支持synchronized关键字


---Dalvik字节码与Java字节码的区别

---JVM运行的是Java字节码，dalvik运行的是dalvik字节码。二者指令集不同


---dalvik可执行文件体积更小。dalvik只有一个dex文件(不使用dex分包情况下），所有类文件共享同一个常量池，所以相同的常量只出现一次。

![这里写图片描述](http://i2.piimg.com/567571/f999fc577cea6c2a.png)
---JVM基于栈，dalvik基于寄存器。寄存器与CPU紧耦合，速度远远快于基于内存的栈。基于寄存器使得代码执行速度快很多，但由于指令中要存储操作数，故代码会稍大。


