# main函数解析 - wishchinYang的专栏 - CSDN博客
2013年09月22日 13:53:11[wishchin](https://me.csdn.net/wishchin)阅读数：620
原文链接：http://parisliu2008.blog.163.com/blog/static/95070867200951510412959/
### main参数
2009-06-15 10:41:29|
每一C 程序都必须有一个main()函数，作为整个程序的入口，可以根据自己的爱好把它放在程序的任何地方。 
在Turbo C2.0启动过程中，传递main()函数三个参数：argc，argv和env。 
* argc: 整数，为传给main()的命令行参数个数。（argument count） 
* argv: 字符串数组。 
在DOS 3.X 版本中，argv[0] 为程序运行的全路径名； 
对DOS 3.0 以下的版本，argv[0]为空串("") 。 
argv[1] 为在DOS 命令行中执行程序名后的第一个字符串； 
argv[2] 为执行程序名后的第二个字符串； 
... 
argv[argc]为NULL。 
* env: 字符串数组。env[] 的每一个元素都包含ENVVAR=value形式的字符串。其中ENVVAR为环境变量如PATH或87。value 为ENVVAR的对应值如C:\DOS，C:\TURBOC(对于PATH) 或YES(对于87)。
TurboC2.0启动时总是把这三个参数传递给main()函数，可以在用户程序中说明(或不说明)它们，如果说明了部分(或全部)参数，它们就成为main()子程序的局部变量。
注意：一旦想说明这些参数，则必须按argc，argv， env的顺序，如以下的例子： 
```cpp
main()
main(int argc)
main(int argc, char *argv[])
main(int argc, char *argv[], char *env[])
其中第二种情况也是合法的，但不常见，因为在程序中很少有只用argc，而不用argv[]的情况。
以下提供一样例程序EXAMPLE.EXE， 演示如何在main()函数中使用三个参数：
#include <stdio.h>
#include <stdlib.h>
main(int argc,char *argv[],char *env[])
{
int i;
printf("These are the %d command-line arguments passed to \main:\n\n", argc);
for(i = 0; i <= argc; ++i)
printf("argv[%d]:%s\n", i, argv[i]);
printf("\nThe environment string(s)on this system are: \\n\n");
for(i = 0; env[i] != NULL; ++i)
printf(" env[%d]:%s\n", i, env[i]);
}
```
如果在DOS 提示符下，按以下方式运行： 
```cpp
EXAMPLE.EXE: C:\example first_argument "argument with blanks" 3 4　"last butone"　stop!
```
注意：可以用双引号括起内含空格的参数，如本例中的："argument　with blanks"和"Last but one")。
传送main() 函数的命令行参数的最大长度为128个字符 (包括参数间的空格)，这是由DOS 限制的。
任何EXE文件 ，本身并没有可执行能力 ，必须由其他的程序(通常是操作系统,但不是绝对的)装入内存 ，把EXE装入内存的程序也为该EXE提供了相应的环境(ENVIRONMENT) ，在这个环境之中就包括了与主函数 int  argc  ,  char*  argv[] ，相对应的实参部分 。
附注：我在实际运行中遇到的问题是：直接运行了.cpp程序，但是发现没有任何错误，就是输出结果异常，不是我们想要的运行结果，可是又不知道问题在哪，于是上网查找有关于
main(int argc,char* argv),的相关问题，上述解释就是相关问题的解决方法，但是，我的实际应用感想是：首先运行程序，生成Debug文件，找到相关的.exe路径，然后，在程序运行命令中输入这个.exe的路径，及相关其他有效值，或有效路径，这样才能得到正确的，我们想要的结果。
