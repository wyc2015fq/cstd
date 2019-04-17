# CMake脚本编写简单入门示例 - zp704393004的专栏 - CSDN博客





2018年05月08日 19:09:23[原来未知](https://me.csdn.net/zp704393004)阅读数：207








﻿﻿





原文地址：[https://blog.csdn.net/onion_autotest/article/details/7222954](https://blog.csdn.net/onion_autotest/article/details/7222954)

CMake是一个跨平台的编译配置工具，CMake运行时，有两种主要模式：执行脚本模式，和执行编译模式 。本文主要介绍CMake执行脚本模式，以及如何编写CMake脚本，并给出简单的示例。至于CMake时如何安装、配置等内容，网上到处都有资料可查，本文在此不做介绍。

# 一、CMake版“Hello World”

先来看一段示例：新建一个文本文件，"cmake.txt"；在文件中输入内容：



**[plain]**[view plain](https://blog.csdn.net/onion_autotest/article/details/7222954#)[copy](https://blog.csdn.net/onion_autotest/article/details/7222954#)[print](https://blog.csdn.net/onion_autotest/article/details/7222954#)[?](https://blog.csdn.net/onion_autotest/article/details/7222954#)

- message("Hello world")  

`message("Hello world")`




保存文件，打开控制台，切换到cmake.txt所在的目录，输入以下命令：



**[plain]**[view plain](https://blog.csdn.net/onion_autotest/article/details/7222954#)[copy](https://blog.csdn.net/onion_autotest/article/details/7222954#)[print](https://blog.csdn.net/onion_autotest/article/details/7222954#)[?](https://blog.csdn.net/onion_autotest/article/details/7222954#)

- cmake -P cmake.txt  

`cmake -P cmake.txt`
回车后，可以看到控制台上打印出了"Hello World"字符。

 好了，示例到此结束，我们来讲解一下。
 首先，需要传递参数"-P" 才能切换到脚本模式下运行；
 其次，CMake对于"-P"后面传入的文件名没有限制，可以为.txt，也可以为其他后缀，如".cmake"，甚至没有后缀也可以。
 不过建议是采用以".cmake"作为脚本文件的后缀，便于识别区分和管理。
 然后，这里使用了CMake脚本中一个最基本、最常用的方法："message()"，
 关于这个方法的详细使用可以参照CMake官方文档中的使用说明，
 或者在控制台下输入"cmake --help-command message"查看帮助。





# 二、CMake脚本语法特性概览

## 1、注释。

CMake中使用"#"表示注释该行代码。

## 2、内置命令。
与其他语言编程语言不同的是，CMake脚本的语法中没有赋值操作，无论是赋值，还是比较、判断操作，都是通过内置命令来完成的，例如"set(),math()等"。并且CMake中的内置命令不区分大小写。
 所有的内置命令调用形式为：




**[plain]**[view plain](https://blog.csdn.net/onion_autotest/article/details/7222954#)[copy](https://blog.csdn.net/onion_autotest/article/details/7222954#)[print](https://blog.csdn.net/onion_autotest/article/details/7222954#)[?](https://blog.csdn.net/onion_autotest/article/details/7222954#)

- command(arg1 arg2 arg3 ... argn)  

`command(arg1 arg2 arg3 ... argn)`
每个参数均以空格，或者分号分割。注：不建议使用分号分割参数


## 3、变量。
CMake中的变量无需声明，并且没有类型概念，这一点类似于python；
 变量可以认为都是全局的，哪怕在一个宏中定义的变量，也可以在宏的外面被访问到；
 所有的变量都是一个列表变量，下文在举例时会详细说明这一点；
CMake对于变量是大小写敏感的。


## 4、变量的引用。
在CMake中，有两种引用方式：对于变量值的引用，和直接引用这个变量本身，使用方式分别是：
${varName} 和 varName

# 三、CMake脚本中常见方法的使用及示例



## 1、赋值
CMake中所有的赋值操作都是通过这个命令完成的：



**[plain]**[view plain](https://blog.csdn.net/onion_autotest/article/details/7222954#)[copy](https://blog.csdn.net/onion_autotest/article/details/7222954#)[print](https://blog.csdn.net/onion_autotest/article/details/7222954#)[?](https://blog.csdn.net/onion_autotest/article/details/7222954#)

- set(variable value1 value2 value3 ... valueN)。  

`set(variable value1 value2 value3 ... valueN)。`
 调用这个命令后，variable变量将是一个列表，其中包含值"value1,value2,...valueN"。具体使用范例见下：



**[plain]**[view plain](https://blog.csdn.net/onion_autotest/article/details/7222954#)[copy](https://blog.csdn.net/onion_autotest/article/details/7222954#)[print](https://blog.csdn.net/onion_autotest/article/details/7222954#)[?](https://blog.csdn.net/onion_autotest/article/details/7222954#)

- set(myVar "This is a test print!")  
- message("${myVar}")  


```
set(myVar "This is a test print!")
message("${myVar}")
```

此时，控制台中将打印出"This is a test print!"。


 这里需要注意的是，message函数中的变量参数，可以写在引号中，并且带引号和不带引号时，有一点差异，下面再看一例：



**[plain]**[view plain](https://blog.csdn.net/onion_autotest/article/details/7222954#)[copy](https://blog.csdn.net/onion_autotest/article/details/7222954#)[print](https://blog.csdn.net/onion_autotest/article/details/7222954#)[?](https://blog.csdn.net/onion_autotest/article/details/7222954#)

- set(myvar "a" "b")  
- message("${myvar}")  
- message(${myvar})  


```
set(myvar "a" "b")
message("${myvar}")
message(${myvar})
```

此时，控制台中将分别打印出"a;b"和"ab"。这是因为，不带引号时，${myvar}是一个列表，包含了两个值，而message中相当于接收到了两个参数"a"、"b"，因此输出"ab"。而带有引号时，引号中的内容整体将作为一个参数存在。


 另一个比较特别的地方是，给变量赋值时，可以不用使用引号，例如：set(myvar a "b" c d)。

## 2、循环
CMake中的循环有两种：foreach()...endforeach()和while()...endwhile()。
while循环没什么特别的，会使用判断就会使用while循环，所以这里只讲一下foreach循环。
 首先来看下面的例子：



**[plain]**[view plain](https://blog.csdn.net/onion_autotest/article/details/7222954#)[copy](https://blog.csdn.net/onion_autotest/article/details/7222954#)[print](https://blog.csdn.net/onion_autotest/article/details/7222954#)[?](https://blog.csdn.net/onion_autotest/article/details/7222954#)

- set(mylist "a" "b" c "d")  
- foreach(_var ${mylist})  
-     message("当前变量是：${_var}")  
- endforeach()  


```
set(mylist "a" "b" c "d")
foreach(_var ${mylist})
	message("当前变量是：${_var}")
endforeach()
```

以上是foreach最基本、最常见的用法，还有一种比较实用的方法是：foreach(loop_var RANGE start stop [step]) 。使用示例见下：



**[plain]**[view plain](https://blog.csdn.net/onion_autotest/article/details/7222954#)[copy](https://blog.csdn.net/onion_autotest/article/details/7222954#)[print](https://blog.csdn.net/onion_autotest/article/details/7222954#)[?](https://blog.csdn.net/onion_autotest/article/details/7222954#)

- set(result 0)  
- foreach(_var RANGE 0 100)  
-     math(EXPR result "${result}+${_var}")  
- endforeach()  
- message("from 0 plus to 100 is:${result}")  


```
set(result 0)
foreach(_var RANGE 0 100)
	math(EXPR result "${result}+${_var}")
endforeach()
message("from 0 plus to 100 is:${result}")
```

这里是演示了计算从0一直加到100的结果。注：math()是数学计算函数，具体使用方法可以参考官方帮助文档


 好了，foreach循环的基本用法就是这些，其实在循环中，有一个方法是会经常用到的，它是：list()。
 关于这个方法的几种常见用法见下示例：



**[plain]**[view plain](https://blog.csdn.net/onion_autotest/article/details/7222954#)[copy](https://blog.csdn.net/onion_autotest/article/details/7222954#)[print](https://blog.csdn.net/onion_autotest/article/details/7222954#)[?](https://blog.csdn.net/onion_autotest/article/details/7222954#)

- # list(LENGTH <list> <output variable>)  
- set(mylist a b c d )  
- list(LENGTH mylist _length)  
- message("列表中元素的个数为:${_length}")  


```
# list(LENGTH <list> <output variable>)
set(mylist a b c d )
list(LENGTH mylist _length)
message("列表中元素的个数为:${_length}")
```




**[plain]**[view plain](https://blog.csdn.net/onion_autotest/article/details/7222954#)[copy](https://blog.csdn.net/onion_autotest/article/details/7222954#)[print](https://blog.csdn.net/onion_autotest/article/details/7222954#)[?](https://blog.csdn.net/onion_autotest/article/details/7222954#)

- # 稍微综合一点的示例:  
- # 生成一个列表a，再将这个列表反序，得到列表b，然后将列表a、b中相应位置的元素合并  
- set(strList "a" "b" "c" "d")  
- set(reverList ${strList})  
- list(REVERSE reverList)  
- message("${reverList}")  
- 
- foreach(_var ${reverList})    
-     list(FIND strList ${_var} temp)  
-     list(GET reverList ${temp} reverItem)  
-     list(APPEND  result "${_var}${reverItem}")    
-     message("当前元素序号 :${temp}")  
- endforeach()  
- message("${result}")  


```
# 稍微综合一点的示例:
# 生成一个列表a，再将这个列表反序，得到列表b，然后将列表a、b中相应位置的元素合并
set(strList "a" "b" "c" "d")
set(reverList ${strList})
list(REVERSE reverList)
message("${reverList}")

foreach(_var ${reverList})	
	list(FIND strList ${_var} temp)
	list(GET reverList ${temp} reverItem)
	list(APPEND  result "${_var}${reverItem}")	
	message("当前元素序号 :${temp}")
endforeach()
message("${result}")
```

## 3、判断
CMake中，判断的用法如下：



**[plain]**[view plain](https://blog.csdn.net/onion_autotest/article/details/7222954#)[copy](https://blog.csdn.net/onion_autotest/article/details/7222954#)[print](https://blog.csdn.net/onion_autotest/article/details/7222954#)[?](https://blog.csdn.net/onion_autotest/article/details/7222954#)

- if(expression)  
-     # then section.  
-     COMMAND1(ARGS ...)  
-     COMMAND2(ARGS ...)  
-     ...  
-   elseif(expression2)  
-     # elseif section.  
-     COMMAND1(ARGS ...)  
-     COMMAND2(ARGS ...)  
-     ...  
-   else(expression)  
-     # else section.  
-     COMMAND1(ARGS ...)  
-     COMMAND2(ARGS ...)  
-     ...  
-   endif(expression)  


```
if(expression)
    # then section.
    COMMAND1(ARGS ...)
    COMMAND2(ARGS ...)
    ...
  elseif(expression2)
    # elseif section.
    COMMAND1(ARGS ...)
    COMMAND2(ARGS ...)
    ...
  else(expression)
    # else section.
    COMMAND1(ARGS ...)
    COMMAND2(ARGS ...)
    ...
  endif(expression)
```

相比于其他“正常的编程语言”，CMake中的判断可以说是比较变态的。
 首先看看官方文档中的说明：对于表达式"if(<constant>)",True if the constant is 1, ON, YES, TRUE, Y, or a non-zero number. False if the constant is 0, OFF, NO, FALSE, N, IGNORE, "", or ends in the suffix '-NOTFOUND'. 
就是说，CMake中if语句中对于条件真假的判断，不完全是以变量的真假为标准的，有时还和变量名有关。
 总之，使用if 时，要特别注意这一点。


 下面来看一下 if 的具体使用示例：



**[plain]**[view plain](https://blog.csdn.net/onion_autotest/article/details/7222954#)[copy](https://blog.csdn.net/onion_autotest/article/details/7222954#)[print](https://blog.csdn.net/onion_autotest/article/details/7222954#)[?](https://blog.csdn.net/onion_autotest/article/details/7222954#)

- if(WIN32)  
-     message("this operation platform is windows")  
- elseif(UNIX)  
-     message("this operation platform is Linux")  
- endif()  


```
if(WIN32)
	message("this operation platform is windows")
elseif(UNIX)
	message("this operation platform is Linux")
endif()
```

其中，"WIN32"和"UNIX"均为CMake中的常量，具体用法可以参考官方文档。


## 4、宏、函数
同大多数脚本语言一样，CMake中也有宏和函数的概念，关键字分别为"macro"和"function",具体用法如下：



**[plain]**[view plain](https://blog.csdn.net/onion_autotest/article/details/7222954#)[copy](https://blog.csdn.net/onion_autotest/article/details/7222954#)[print](https://blog.csdn.net/onion_autotest/article/details/7222954#)[?](https://blog.csdn.net/onion_autotest/article/details/7222954#)

- macro(<name> [arg1 [arg2 [arg3 ...]]])  
-     COMMAND1(ARGS ...)  
-     COMMAND2(ARGS ...)  
-     ...  
- endmacro(<name>)  
- function(<name> [arg1 [arg2 [arg3 ...]]])  
-     COMMAND1(ARGS ...)  
-     COMMAND2(ARGS ...)  
-     ...  
- endfunction(<name>)  


```
macro(<name> [arg1 [arg2 [arg3 ...]]])
    COMMAND1(ARGS ...)
    COMMAND2(ARGS ...)
    ...
endmacro(<name>)
function(<name> [arg1 [arg2 [arg3 ...]]])
    COMMAND1(ARGS ...)
    COMMAND2(ARGS ...)
    ...
endfunction(<name>)
```


以简单的求和函数为例，我们来看宏的一个示例：



**[plain]**[view plain](https://blog.csdn.net/onion_autotest/article/details/7222954#)[copy](https://blog.csdn.net/onion_autotest/article/details/7222954#)[print](https://blog.csdn.net/onion_autotest/article/details/7222954#)[?](https://blog.csdn.net/onion_autotest/article/details/7222954#)

- macro(sum outvar)  
-     set(_args ${ARGN})  
-     set(result 0)  
- 
-     foreach(_var ${_args})  
-         math(EXPR result "${result}+${_var}")  
-     endforeach()  
- 
-     set(${outvar} ${result})  
- endmacro()  
- 
- 
- sum(addResult 1 2 3 4 5)  
- message("Result is :${addResult}")   


```
macro(sum outvar)
	set(_args ${ARGN})
	set(result 0)
	
	foreach(_var ${_args})
		math(EXPR result "${result}+${_var}")
	endforeach()
	
	set(${outvar} ${result})
endmacro()


sum(addResult 1 2 3 4 5)
message("Result is :${addResult}")
```


上面是一段求和函数，我们来解读一下代码：
"${ARGN}"是CMake中的一个变量，指代宏中传入的多余参数。因为我们这个宏sum中只定义了一个参数"outvar"，其余需要求和的数字都是不定形式传入的，所以需要先将多余的参数传入一个单独的变量中。当然，在这个示例中，第一行代码显得多余，因为似乎没必要将额外参数单独放在一个变量中，但是建议这么做。


 对上面这个宏再进一步加强：如果我们想限制这个宏中传入的参数数目（尽管在这个宏中实际上是不必要的），那么可以将宏改写一下：



**[plain]**[view plain](https://blog.csdn.net/onion_autotest/article/details/7222954#)[copy](https://blog.csdn.net/onion_autotest/article/details/7222954#)[print](https://blog.csdn.net/onion_autotest/article/details/7222954#)[?](https://blog.csdn.net/onion_autotest/article/details/7222954#)

- macro(sum outvar)  
-     set(_args ${ARGN})  
-     list(LENGTH _args argLength)  
-     if(NOT argLength LESS 4)　# 限制不能超过4个数字  
-         message(FATAL_ERROR "to much args!")  
-     endif()  
-     set(result 0)  
- 
-     foreach(_var ${ARGN})  
-         math(EXPR result "${result}+${_var}")  
-     endforeach()  
- 
-     set(${outvar} ${result})  
- endmacro()  
- 
- 
- sum(addResult 1 2 3 4 5)  
- message("Result is :${addResult}")   


```
macro(sum outvar)
	set(_args ${ARGN})
	list(LENGTH _args argLength)
	if(NOT argLength LESS 4)　# 限制不能超过4个数字
		message(FATAL_ERROR "to much args!")
	endif()
	set(result 0)
	
	foreach(_var ${ARGN})
		math(EXPR result "${result}+${_var}")
	endforeach()
	
	set(${outvar} ${result})
endmacro()


sum(addResult 1 2 3 4 5)
message("Result is :${addResult}")
```

而CMake中的函数("function")与宏唯一的区别就在于，函数不能像宏那样将计算结果传出来(也不是完全不能，只是复杂一些)，并且函数中的变量是局部的，而宏中的变量在外面也可以被访问到，请看下例：



**[plain]**[view plain](https://blog.csdn.net/onion_autotest/article/details/7222954#)[copy](https://blog.csdn.net/onion_autotest/article/details/7222954#)[print](https://blog.csdn.net/onion_autotest/article/details/7222954#)[?](https://blog.csdn.net/onion_autotest/article/details/7222954#)

- macro(macroTest)  
-     set(test1 "aaa")  
- endmacro()  
- 
- 
- function(funTest)  
-     set(test2 "bbb")  
- endfunction()  
- 
- 
- macroTest()  
- message("${test1}")  
- 
- 
- funTest()  
- message("${test2}")  


```
macro(macroTest)
	set(test1 "aaa")
endmacro()


function(funTest)
	set(test2 "bbb")
endfunction()


macroTest()
message("${test1}")


funTest()
message("${test2}")
```

运行这段代码后，只会打印出一条信息"aaa"，由此可以看到宏与函数的区别。


## 5、综合示例
最后我们来通过一个稍微复杂综合一点的宏来结束本文。
 下面的这个宏是找出指定数值范围内全部素数，并输出。



**[plain]**[view plain](https://blog.csdn.net/onion_autotest/article/details/7222954#)[copy](https://blog.csdn.net/onion_autotest/article/details/7222954#)[print](https://blog.csdn.net/onion_autotest/article/details/7222954#)[?](https://blog.csdn.net/onion_autotest/article/details/7222954#)

- macro(GetPrime output maxNum)  
-     set(extArg ${ARGN})  
-     if(extArg)  
-         message(SEND_ERROR "To much args!")  
-     endif()  
- 
-     # 没有判断传入的变量是否为数字类型  
-     set(result)  
-     foreach(_var RANGE 2 ${maxNum})  
-         set(isPrime 1)  
-         math(EXPR upplimit ${_var}-1)         
-         foreach(_subVar RANGE 2 ${upplimit})  
-             math(EXPR _temp "${_var}%${_subVar}")  
-             if(_temp EQUAL 0)  
-                 set(isPrime 0)  
-                 break()  
-             endif()  
-         endforeach()  
- 
-         if(isPrime)  
-             list(APPEND result ${_var})  
-         endif()  
-     endforeach()      
-     set(output ${result})  
- endmacro()  
- 
- 
- GetPrime(output 100)  
- message("${output}")  





