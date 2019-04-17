# c/c++中int main(int argc,char *argv[])的具体含义 - 在思索中前行！ - CSDN博客





2015年03月28日 17:53:54[_Tham](https://me.csdn.net/txl16211)阅读数：5684标签：[agrc																[agrv参数																[main函数参数](https://so.csdn.net/so/search/s.do?q=main函数参数&t=blog)
个人分类：[C/C++](https://blog.csdn.net/txl16211/article/category/2284665)






int main(int argc,char * argv[ ])

argv为指针的指针

argc为整数




char **argv or: char *argv[ ] or: char argv[ ][ ]

main()括号内是固定的写法。

下面给出一个例子来理解这两个参数的用法：

假设程序的名称为prog，

当只输入prog，则由操作系统传来的参数为：

argc=1,表示只有一程序名称。

argc只有一个元素，argv[0]指向输入的程序路径及名称：./prog

当输入prog para_1，有一个参数，则由操作系统传来的参数为：

argc=2，表示除了程序名外还有一个参数。

argv[0]指向输入的程序路径及名称。

argv[1]指向参数para_1字符串。

当输入prog para_1 para_2 有2个参数，则由操作系统传来的参数为：

argc=3，表示除了程序名外还有2个参数。

argv[0]指向输入的程序路径及名称。

argv[1]指向参数para_1字符串。

argv[2]指向参数para_2字符串。

void    main(    int    argc,    char    *argv[]    )  

char    *argv[]    :    argv 是一个指针数组，他的元素个数是argc，存放的是指向每一个参数的指针，

他的第一个元素即argv[0]为编译生成的可执行文件名(包括路径eg:"F:\VC\Ex1\Debug\Ex1.exe")，从二个元素(argv[1])开始，是每一个参数   int    argc    表示argv的大小，是实际参数个数＋1，其中＋1是因为argv[0]是编译后的可执行文件名

main()主函数

  每一C 程序都必须有一main()函数, 可以根据自己的爱好把它放在程序的某 个地方。有些程序员把它放在最前面, 而另一些程序员把它放在最后面, 无论放 

在哪个地方, 以下几点说明都是适合的。

1. main() 参数 

     在Turbo C2.0启动过程中, 传递main()函数三个参数: argc, argv和env。 

      * argc:   整数, 为传给main()的命令行参数个数。 

      * argv:   字符串数组。 

               在DOS 3.X 版本中, argv[0] 为程序运行的全路径名; 对DOS 3.0 

               以下的版本, argv[0]为空串("") 。 

               argv[1] 为在DOS命令行中执行程序名后的第一个字符串; 

               argv[2] 为执行程序名后的第二个字符串; 

               ... 

               argv[argc]为NULL。 

      *env:   安符串数组。env[] 的每一个元素都包含ENVVAR=value形式的字符 串。其中ENVVAR为环境变量如PATH或87。value 为ENVVAR的对应值如C:\DOS, C: \TURBOC(对于PATH) 或YES(对于87)。 

     Turbo C2.0启动时总是把这三个参数传递给main()函数, 可以在用户程序中说明(或不说明)它们, 如果说明了部分(或全部)参数, 它们就成为main()子程序 的局部变量。 

     请注意: 一旦想说明这些参数, 则必须按argc, argv, env 的顺序, 如以下 的例子: 

      main() 

      main(int argc) 

      main(int argc, char *argv[]) 

      main(int argc, char *argv[], char *env[]) 

     其中第二种情况是合法的, 但不常见, 因为在程序中很少有只用argc, 而不 用argv[]的情况。 

     以下提供一样例程序EXAMPLE.EXE,   演示如何在main()函数中使用三个参数: 




**[cpp]**[view plain](http://blog.csdn.net/ajioy/article/details/6962419#)[copy](http://blog.csdn.net/ajioy/article/details/6962419#)

- /*program name EXAMPLE.EXE*/
-     ＃include <stdio.h>  
-     main(int argc, char *argv[], char *env[])   
-     {   
- int i;   
-          printf("These are the %d   command- line   arguments passed   to main:\n\n", argc);   
- for(i=0; i<=argc; i++)   
-            printf("argv[%d]:%s\n", i, argv[i]);   
-          printf("\nThe environment string(s)on this system are:\n\n");   
- for(i=0; env[i]!=NULL; i++)   
-               printf(" env[%d]:%s\n", i, env[i]);   
-     }   





     如果在DOS 提示符下, 按以下方式运行EXAMPLE.EXE: 

     C:\example first_argument "argument with blanks"   3   4   "last   but one" stop! 

     注意: 可以用双引号括起内含空格的参数, 如本例中的:    "   argument with blanks"和"Last but one")。 

     结果是这样的: 

      The value of argc is 7 

      These are the 7 command-linearguments passed to main: 

      argv[0]:C:\TURBO\EXAMPLE.EXE 

      argv[1]:first_argument 

      argv[2]:argument with blanks 

      argv[3]:3 

      argv[4]:4 

      argv[5]:last but one 

      argv[6]:stop! 

_______________________________________________________

命令行界面的程序，通常都需要输入命令行参数帮助程序执行。假定有一个可执行程序名为test。那么运行该程序的的命令行如下： 

test 

带命令行参数是同一行中的附加项： 

test –c TEST 

其中 –c 和 TEST就是命令行参数。C程序可以将这些附加参数读出来，并为自己所用，比如作为程序运行的条件（经常看到调试参数 –D 就是这么一个）。C程序通过使用main()的参数来读取这些附加参数，下面的repeat.c给出一个读出main参数的例子： 

repeat.c： 





**[cpp]**[view plain](http://blog.csdn.net/ajioy/article/details/6962419#)[copy](http://blog.csdn.net/ajioy/article/details/6962419#)

- #include <stdio.h> 
- int main(int argc, char *argv[])   
- {   
- int count;   
- printf("The command line has %d arguments:\n", argc - 1);   
- for(count = 1; count < argc; count++)   
- {   
- printf("%d: %s\n", count, argv[count]);   
- }   
- printf("\n");   
- //system("PAUSE"); 
- return 0;   
- }   



这里先解释一下main(int argc, char*argv[])这个函数中两个参数的意义，argc记录的是命令行中输入参数的数目，argv是一个拥有argc个元素的字符串数组，每个元素 保存一个命令行中输入的参数。 

编译这个文件为可执行文件repeat： 

gcc repeat.c -o repeat 

按下 列方式执行 repeat 程序 

./repeat I "love you" 3 

输出如下： 

The command line has 3 arguments: 

1: I 

2: love you 

3: 3 

在这个例子中，argc的 值为4，命令行一共输入了四个参数“./repeat”、“I”、“love you”、“3”。在DOS和UNIX环境下，命令行参数中用””符号表示其是一个字符串，视为一个参数。](https://so.csdn.net/so/search/s.do?q=agrv参数&t=blog)](https://so.csdn.net/so/search/s.do?q=agrc&t=blog)




