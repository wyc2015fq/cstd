# g++编译连接命令简介 - 建建的博客 - CSDN博客
2017年02月28日 22:19:37[纪建](https://me.csdn.net/u013898698)阅读数：131
个人分类：[Linux](https://blog.csdn.net/u013898698/article/category/6754383)
gcc & g++现在是gnu中最主要和最流行的c & c++编译器 。
g++是c++的命令，以.cpp为主，对于c语言后缀名一般为.c。这时候命令换做gcc即可。其实是无关紧要的。
其实编译器是根据gcc还是g++来确定是按照C标准还是C++标准编译链接。
下面以Test.cpp为例：
命令： g++ Test.cpp
功能：生成默认为a.exe的文件，这个过程包含了编译和链接。
再说下-o命令，-o命令表示输出的意思，gcc/g++命令是非常灵活的，你不指定输出的文件名的时候默认生成的是.exe文件。
你要输出Test.exe的话可以用：g++ -o Test.exe Test.cpp。-o命令是输出的意思，这样就输出了Test.exe。
gcc/g++在执行编译工作的时候，总共需要以下几步:
- <span style="font-family:'Microsoft YaHei';font-size:16px;">  /*************************
-         Test.cpp
-     *************************/
-     #include <IOSTREAM>
- 
- staticint t = 1;  
-     #define T 9
- 
- usingnamespace std;  
- 
- typedefint Status;  
- 
- int main()  
-     {  
-         Status i = 1;  
-         cout << T * i << endl; //Test Cout
- return 0;  
-     }</span><span style="font-family: 'Microsoft YaHei'; font-size: 18px; ">  
- </span>  
## 1.预处理,生成.i的文件[预处理器cpp]
命令：g++ -E Test.cpp > Test.i 
> 
功能：输出预处理后的文件，linux下以.i为后缀名。只激活预处理,这个不生成文件,你需要把它重定向到一个输出文件里 。这一步主要做了这些事情：宏的替换，还有注释的消除，还有找到相关的库文件。用编辑器打开Test.i会发现有很多很多代码，你只需要看最后部分就会发现，预处理做了宏的替换，还有注释的消除，可以理解为无关代码的清除。下面是Test.i文件的最后部分，可以看见宏的替换和注释的消除。
- <span style="font-family:'Microsoft YaHei';font-size:16px;"># 5 "Test.cpp" 2  
- 
- staticint t = 1;  
- 
- usingnamespace std;  
- 
- typedefint Status;  
- 
- int main()  
- {  
-  Status i = 1;  
-  cout << 9 * i << endl;  
- return 0;  
- }</span>  
## 2.将预处理后的文件不转换成汇编语言,生成文件.s[编译器egcs]
命令:g++ -S Test.cpp
功能:会生成Test.s文件，.s文件表示是汇编文件，用编辑器打开就都是汇编指令。
## 3.有汇编变为目标代码(机器代码)生成.o的文件[汇编器as]
命令:g++ -c Test.cpp 
功能：.o是GCC生成的目标文件，除非你是做编译器和连接器调试开发的，否则打开这种.o没有任何意义。二进制机器码一般人也读不了。
## 4.连接目标代码,生成可执行程序[链接器ld]
命令：g++ Test.o
 -L F:\vs2008\VC\include\iostream
功能：将.o文件与所需的库文件链接整合形成.exe文件，这就是可执行文件。-L 表示链接，这里我后面写的是绝对路径，相对各人电脑不同
在上面各个步骤中你可以用-o命令输出你自己想要的各种名字。比如最后一个命令，用下面的输出Test.exe
你可以g++ Test.o -o Test.exe -L F:\vs2008\VC\include\iostream
