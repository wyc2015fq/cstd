# JAVA中的循环语句 - bigfacesafdasgfewgf - CSDN博客





2014年09月27日 15:41:23[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：1321








- Java也提供了3种循环语句：for, while, do-while语句；
- 计算时间可以使用如下代码来计算而得：long startTime=System.currentTimeMillis();   long endTime=System.currentTimeMillis();  long testTime= endTime - stratTime;
- 在循环控制中，一定不可以使用浮点数来比较值是否相等，这是因为浮点值都是某些值的近似值，这样就可能导致不精确的循环次数和不准确的结果。浮点数可以非常接近判断所需的整数，但是绝不是相等的，所以不能使用等号==来判断；
- Java中输入和输出的重定向（从文件中输入和把结果写到文件中）。例如：输入重定向，java SentinelValue < input.txt; 其中SentinelValue 是这个java程序的文件名；输出重定向，java ClassName > output.txt; 也可以将二者结合起来：java SentinelValue < input.txt > output.txt; 
- 如果循环中的语句至少需要执行一次，那么建议使用do-while循环语句。如果使用while循环，那么这些语句必须在循环前和循环内都出现，因为我们必须在while语句前面出现依次以便可以执行一次，然后再判断while是否执行的条件。其实，while和do-while语句的区别就是前者是先判断再执行循环体，后者是先执行一次循环体，然后再判断；
- for循环语句适用于循环次数已知的情况。for语句中的循环控制变量必须在循环控制结构体内或者循环前给定说明，如果循环控制变量只需要在循环体内使用，那么最好在for()的括号中初始化，这样一来只能在循环体内使用，其他地方不可以使用了；而这种情况也是经常遇到的，所以一般情况都是在for的括号中定义一些控制变量；
- 为了避免浮点数在循环语句中判断条件容易出错的缺点，我们可以使用整数计算器count来确保循环次数正确；





