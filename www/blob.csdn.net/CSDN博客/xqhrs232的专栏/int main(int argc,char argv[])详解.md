# int main(int argc,char* argv[])详解 - xqhrs232的专栏 - CSDN博客
2015年10月27日 22:47:38[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：360
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://www.cnblogs.com/avril/archive/2010/03/22/1691477.html](http://www.cnblogs.com/avril/archive/2010/03/22/1691477.html)
argc是命令行总的参数个数 
 argv[]是argc个参数，其中第0个参数是程序的全名，以后的参数 
 命令行后面跟的用户输入的参数，比如： 
 int  main(int  argc,  char*  argv[]) 
 { 
 int  i; 
 for  (i  =  0;  i<argc;  i++) 
 cout<<argv[i]<<endl; 
 cin>>i; 
 return  0; 
 } 
 执行时敲入 
 F:\MYDOCU~1\TEMPCODE\D1\DEBUG\D1.EXE  aaaa  bbb  ccc  ddd 
 输出如下： 
 F:\MYDOCU~1\TEMPCODE\D1\DEBUG\D1.EXE 
 aaaa 
 bbb 
 ccc 
 ddd 
--------------------------------------------------------------------
char  *argv[]是一个字符数组,其大小是int  argc,主要用于命令行参数  argv[]  参数，数组里每个元素代表一个参数;
比如你输入 
 test  a.c  b.c  t.c 
 则 
 argc  =  4 
 argv[0]  =  "test" 
 argv[1]  =  "a.c" 
 argv[2]  =  "b.c" 
 argv[3]  =  "t.c"
-------------------------------------------------------------------------------------------- 
argc记录了用户在运行程序的命令行中输入的参数的个数。 
arg[]指向的数组中至少有一个字符指针，即arg[0].他通常指向程序中的可执行文件的文件名。在有些版本的编译器中还包括程序
文件所在的路径。
-------------------------------------------------------------------------
在调用一个可执行程序时，某些情况下需要向程序传递参数。如我们可以在控制台中键入notepad.exe，
回车后将执行记事本程序。如果我们希望在打开notepad时同时打开一个文本文件，可以在notepad.exe 
后面跟上文件的路径和名字，如notepad.exe  example.txt（文件在当前路径）。 
 那么程序中如何能得到这些输入参数呢？这个工作是编译器帮我们完成的，编译器将输入参数的信息
放入main函数的参数列表中。 
 main函数的参数列表保存了输入参数的信息，第一个参数argc记录了输入参数的个数， 
 第二个参数是字符串数组的，字符串数组的每个单元是char*类型的，指向一个c风格字符串。 
 以notepad.exe  example.txt为例 
 argc是2，就是说argv数组中有两个有效单元 
 第一单元指向的字符串是"notepad.exe" 
 第二单元指向的字符串是"example.txt" 
 argv数组中的第一个单元指向的字符串总是可执行程序的名字，以后的单元指向的字符串依次是程序调用时的参数。 
 这个赋值过程是编译器完成的，我们只需要读出数据就可以了。
-----------------------------------------------------------------------------
int  main(  int  argc  ,  char  *argv[]  ,  char  *envp[]  ) 
 main()函数一般用int或者void形的。我比较喜欢用int型定义main。因为在结束的时候可以返回给操作系统一个值以表示执行情况。 
 int  argc 
 这个东东用来表示你在命令行下输入命令的时候，一共有多少个参数。比方说你的程序编译后，可执行文件是test.exe 
 D:\tc2>test 
 这个时候，argc的值是1 
 但是 
 D:\tc2>test.exe  myarg1  myarg2 
 的话，argc的值是3。也就是  命令名  加上两个参数，一共三个参数 
 char  *argv[] 
 这个东东用来取得你所输入的参数 
 D:\tc2>test 
 这个时候，argc的值是1，argv[0]的值是  "test" 
 D:\tc2>test  myarg1  myarg2 
 这个时候，argc的值是3，argc[0]的值是"test"，argc[1]的值是"myarg1"，argc[2]的值是"myarg2"。 
 这个东东一般用来为程序提供非常重要的信息，如：数据文件名，等等。 
 如：copy  a.c  b.txt 
 这个时候，a.c和b.txt就是所谓的“非常重要的信息”。不指定这两个文件，你没法进行拷贝。 
 当你的程序用到argc和argv这两个参数的时候，可以简单地通过判断argc的值，来看看程序的参数是否符合要求 
 char  *envp[] 
 这个东东相对来说用得比较少。它是用来取得系统的环境变量的。 
 如：在DOS下，有一个PATH变量。当你在DOS提示符下输入一个命令（当然，这个命令不是dir一类的内部命令）的时候，DOS会首先在当前目录下找这个命令的执行文件。如果找不到，则到PATH定义的路径下去找，找到则执行，找不到返回Bad  command  or  file  name 
 在DOS命令提示符下键入set可查看系统的环境变量 
 同样，在UNIX或者LINUX下，也有系统环境变量，而且用得比DOS要多。如常用的$PATH,$USER,$HOME等等。 
 envp保存所有的环境变量。其格式为（UNIX下） 
 PATH=/usr/bin;/local/bin; 
 HOME=/home/shuui 
 即： 
 环境变量名=值 
 DOS下大概也一样。 
 环境变量一般用来为程序提供附加信息。如，你做了一个显示文本的内容的程序。你想控制其一行中显示的字符的个数。你可以自己定义一个环境变量（UNIX下） 
 %setenv  NUMBER  =  10 
 %echo  $NUMBER 
 10 
 然后你可以在程序中读入这个环境变量。然后根据其值决定一行输出多少个字符。这样，如果你不修改环境变量的话，你每次执行这个程序，一行中显示的字符数都是不一样的 
 下面是一个例子程序 
 #include<stdio.h> 
 int
 main(  int  argc  ,  char  *argv[]  ,  char  *envp[]  ) 
 { 
 int  i; 
 printf(  "You  have  inputed  total  %d  argments\n"  ,  argc  ); 
 for(  i=0  ;  i<argc  ;  i++) 
 { 
 printf(  "arg%d  :  %s\n"  ,  i  ,  argv[i]  ); 
 } 
 printf(  "The  follow  is  envp  :\n"  ); 
 for(  i=0  ;  *envp[i]!='\0'  ;  i++  ) 
 { 
 printf(  "%s\n"  ,  envp[i]  ); 
 } 
 return  0; 
 } 
 D:\>argtest  this  is  a  test  programe  of  main()'s  argments 
 You  have  inputed  total  9  argments 
 arg0  :  D:\TC\NONAME.EXE 
 arg1  :  this 
 arg2  :  is 
 arg3  :  a 
 arg4  :  test 
 arg5  :  programe 
 arg6  :  of 
 arg7  :  main()'s 
 arg8  :  argments 
 The  follow  is  envp  : 
 TMP=C:\WINDOWS\TEMP 
 TEMP=C:\WINDOWS\TEMP 
 PROMPT=$p$g 
 winbootdir=C:\WINDOWS 
 PATH=C:\WINDOWS;C:\WINDOWS\COMMAND 
 COMSPEC=C:\WINDOWS\COMMAND.COM 
 SBPCI=C:\SBPCI 
 windir=C:\WINDOWS 
 BLASTER=A220  I7  D1  H7  P330  T6 
 CMDLINE=noname  this  is  a  test  programe  of  main()'s  argments 
-----------------------------------------------------------------------------------------
命令行参数啊。argc  是参数的个数，argv[]是参数，argv[0]是文件名，argv[1]是第一个参数... 
 如你得exe文件名是:myprog.exe，那么 
 myprog  12  22  32 
 则argv[0]="myprog"，argv[1]="12"，argv[2]="22"... 
 exit()是程序退出时的返回码。可以用其他程序接收，判断是否正常退出。如exit(-1)认为异常退出。
![](http://www.cnblogs.com/Emoticons/face/009.gif)---------------------------------------------------------------------------------------------
请问这里为什么要设置这两个参数呢？不要不行吗？谢谢
有些时候程序运行时需要提供一些参数。比如copy命令，需要指明源文件和目标文件名，就得通过argc和argv来传递
