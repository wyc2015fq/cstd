# arduino编译过程 - V__KING__的专栏 - CSDN博客





2015年11月23日 11:05:42[v__king__](https://me.csdn.net/V__KING__)阅读数：2507









[https://www.arduino.cc/en/Hacking/BuildProcess](https://www.arduino.cc/en/Hacking/BuildProcess)



## arduino多个文件
- 把所有的.ino文件都给串联起来
- 把所有的.c , .cpp 文件都给单独的编译
- 对于那些.h文件在arduino IDE中是要被包含进来的



## arduino编译过程

其实是把所哟的.ino文件串联起来放到一个叫main.cpp文件中，然后再让avr-gcc去编译
- #include “Arduino.h” 

在Arduino.h中已经包含了 /hardware/cores// 的核心代码
- 
在所有的.ino 文件中寻找所有的函数，并且创建声明(即函数原型)。在include”file.h” 之后插入函数声明。所以有弊端：如果你在函数中用到了自定义的类型，就会出错。应该先在一个头文件中声明并包含进来。

> 
Next, the environment searches for function definitions within your main sketch file and creates declarations (prototypes) for them. These are inserted after any comments or pre-processor statements (#includes or #defines), but before any other statements
 (including type declarations). This means that if you want to use a custom type as a function argument, you should declare it within a separate header file. Also, this generation isn’t perfect: it won’t create prototypes for functions that have default argument
 values, or which are declared within a namespace or class. 


- 
最后，main.cxx文件的内容就会全部放在main.cpp的最后面





