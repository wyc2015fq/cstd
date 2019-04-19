# C语言文件操作 - maopig的专栏 - CSDN博客
2011年09月07日 14:52:06[maopig](https://me.csdn.net/maopig)阅读数：634标签：[语言																[c																[fp																[file																[string																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=fp&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)
个人分类：[c/C++](https://blog.csdn.net/maopig/article/category/847675)
文件
文件的基本概念
　　所谓“文件”是指一组相关数据的有序集合。 这个数据集有一个名称，叫做文件名。 实际上在前面的各章中我们已经多次使用了文件，例如源程序文件、目标文件、可执行文件、库文件 (头文件)等。文件通常是驻留在外部介质(如磁盘等)上的， 在使用时才调入内存中来。从不同的角度可对文件作不同的分类。从用户的角度看，文件可分为普通文件和设备文件两种。
　　普通文件是指驻留在磁盘或其它外部介质上的一个有序数据集，可以是源文件、目标文件、可执行程序； 也可以是一组待输入处理的原始数据，或者是一组输出的结果。对于源文件、目标文件、可执行程序可以称作程序文件，对输入输出数据可称作数据文件。
　　设备文件是指与主机相联的各种外部设备，如显示器、打印机、键盘等。在操作系统中，把外部设备也看作是一个文件来进行管理，把它们的输入、输出等同于对磁盘文件的读和写。通常把显示器定义为标准输出文件， 一般情况下在屏幕上显示有关信息就是向标准输出文件输出。如前面经常使用的printf,putchar 函数就是这类输出。键盘通常被指定标准的输入文件， 从键盘上输入就意味着从标准输入文件上输入数据。scanf,getchar函数就属于这类输入。 
　　从文件编码的方式来看，文件可分为ASCII码文件和二进制码文件两种。
　　ASCII文件也称为文本文件，这种文件在磁盘中存放时每个字符对应一个字节，用于存放对应的ASCII码。例如，数5678的存储形式为：
ASC码： 　00110101001101100011011100111000
　　　　　↓ 　　　　↓　　　　↓ 　　　↓
十进制码： 5678 共占用4个字节。ASCII码文件可在屏幕上按字符显示， 例如源程序文件就是ASCII文件，用DOS命令TYPE可显示文件的内容。 由于是按字符显示，因此能读懂文件内容。
　　二进制文件是按二进制的编码方式来存放文件的。 例如， 数5678的存储形式为： 00010110 00101110只占二个字节。二进制文件虽然也可在屏幕上显示， 但其内容无法读懂。C系统在处理这些文件时，并不区分类型，都看成是字符流，按字节进行处理。 输入输出字符流的开始和结束只由程序控制而不受物理符号(如回车符)的控制。 因此也把这种文件称作“流式文件”。
　　本章讨论流式文件的打开、关闭、读、写、 定位等各种操作。文件指针在Ｃ语言中用一个指针变量指向一个文件， 这个指针称为文件指针。通过文件指针就可对它所指的文件进行各种操作。定义说明文件指针的一般形式为： FILE* 指针变量标识符； 其中FILE应为大写，它实际上是由系统定义的一个结构，该结构中含有文件名、文件状态和文件当前位置等信息。 在编写源程序时不必关心FILE结构的细节。例如：FILE *fp； 表示fp是指向FILE结构的指针变量，通过fp 即可找存放某个文件信息的结构变量，然后按结构变量提供的信息找到该文件， 实施对文件的操作。习惯上也笼统地把fp称为指向一个文件的指针。文件的打开与关闭文件在进行读写操作之前要先打开，使用完毕要关闭。 所谓打开文件，实际上是建立文件的各种有关信息，并使文件指针指向该文件，以便进行其它操作。关闭文件则断开指针与文件之间的联系，也就禁止再对该文件进行操作。
　　在Ｃ语言中，文件操作都是由库函数来完成的。 在本章内将介绍主要的文件操作函数。
文件打开函数ｆｏｐｅｎ
　　fopen函数用来打开一个文件，其调用的一般形式为：文件指针名=fopen(文件名，使用文件方式) 其中，“文件指针名”必须是被说明为FILE 类型的指针变量，“文件名”是被打开文件的文件名。 “使用文件方式”是指文件的类型和操作要求。“文件名”是字符串常量或字符串数组。例如： 
FILE *fp；
fp=("file a","r");
其意义是在当前目录下打开文件file a，只允许进行“读”操作，并使fp指向该文件。
又如：
FILE *fphzk
fphzk=("c:\\hzk16',"rb")
其意义是打开C驱动器磁盘的根目录下的文件hzk16， 这是一个二进制文件，只允许按二进制方式进行读操作。两个反斜线“\\ ”中的第一个表示转义字符，第二个表示根目录。使用文件的方式共有12种，下面给出了它们的符号和意义。 
文件使用方式 　　　　　　　意 义
“rt”　　　　　　只读打开一个文本文件，只允许读数据 
“wt”　　　　　　只写打开或建立一个文本文件，只允许写数据
“at”　　　　　　追加打开一个文本文件，并在文件末尾写数据
“rb”　　　　　　只读打开一个二进制文件，只允许读数据
“wb”　　　　 　 只写打开或建立一个二进制文件，只允许写数据
“ab” 　　　　 　追加打开一个二进制文件，并在文件末尾写数据
“rt+”　　　　　 读写打开一个文本文件，允许读和写
“wt+”　　　　　 读写打开或建立一个文本文件，允许读写
“at+”　　　　　 读写打开一个文本文件，允许读，或在文件末追加数 据
“rb+”　　　　　 读写打开一个二进制文件，允许读和写 
“wb+”　　　　　 读写打开或建立一个二进制文件，允许读和写
“ab+” 　　　　　读写打开一个二进制文件，允许读，或在文件末追加数据
对于文件使用方式有以下几点说明：
1. 文件使用方式由r,w,a,t,b，+六个字符拼成，各字符的含义是：
r(read): 读
w(write): 写
a(append): 追加
t(text): 文本文件，可省略不写
b(banary): 二进制文件
+: 读和写
2. 凡用“r”打开一个文件时，该文件必须已经存在， 且只能从该文件读出。
3. 用“w”打开的文件只能向该文件写入。 若打开的文件不存在，则以指定的文件名建立该文件，若打开的文件已经存在，则将该文件删去，重建一个新文件。
4. 若要向一个已存在的文件追加新的信息，只能用“a ”方式打开文件。但此时该文件必须是存在的，否则将会出错。
5. 在打开一个文件时，如果出错，fopen将返回一个空指针值NULL。在程序中可以用这一信息来判别是否完成打开文件的工作，并作相应的处理。因此常用以下程序段打开文件：
if((fp=fopen("c:\\hzk16","rb")==NULL)
{
printf("\nerror on open c:\\hzk16 file!");
getch();
exit(1);
}
　　这段程序的意义是，如果返回的指针为空，表示不能打开C盘根目录下的hzk16文件，则给出提示信息“error on open c:\ hzk16file!”，下一行getch()的功能是从键盘输入一个字符，但不在屏幕上显示。在这里，该行的作用是等待，只有当用户从键盘敲任一键时，程序才继续执行， 因此用户可利用这个等待时间阅读出错提示。敲键后执行exit(1)退出程序。
6. 把一个文本文件读入内存时，要将ASCII码转换成二进制码， 而把文件以文本方式写入磁盘时，也要把二进制码转换成ASCII码，因此文本文件的读写要花费较多的转换时间。对二进制文件的读写不存在这种转换。
7. 标准输入文件(键盘)，标准输出文件(显示器 )，标准出错输出(出错信息)是由系统打开的，可直接使用。文件关闭函数ｆｃｌｏｓｅ文件一旦使用完毕，应用关闭文件函数把文件关闭， 以避免文件的数据丢失等错误。
fclose函数
调用的一般形式是： fclose(文件指针)； 例如：
fclose(fp); 正常完成关闭文件操作时，fclose函数返回值为0。如返回非零值则表示有错误发生。文件的读写对文件的读和写是最常用的文件操作。 
在Ｃ语言中提供了多种文件读写的函数： 
·字符读写函数 ：fgetc和fputc
·字符串读写函数：fgets和fputs
·数据块读写函数：freed和fwrite
·格式化读写函数：fscanf和fprintf
　　下面分别予以介绍。使用以上函数都要求包含头文件stdio.h。字符读写函数ｆｇｅｔｃ和ｆｐｕｔｃ字符读写函数是以字符(字节)为单位的读写函数。 每次可从文件读出或向文件写入一个字符。
一、读字符函数fgetc
　　fgetc函数的功能是从指定的文件中读一个字符，函数调用的形式为：字符变量=fgetc(文件指针)； 例如：ch=fgetc(fp);其意义是从打开的文件fp中读取一个字符并送入ch中。
　　对于fgetc函数的使用有以下几点说明：
1. 在fgetc函数调用中，读取的文件必须是以读或读写方式打开的。
2. 读取字符的结果也可以不向字符变量赋值，例如：fgetc(fp);但是读出的字符不能保存。
3. 在文件内部有一个位置指针。用来指向文件的当前读写字节。在文件打开时，该指针总是指向文件的第一个字节。使用fgetc 函数后， 该位置指针将向后移动一个字节。 因此可连续多次使用fgetc函数，读取多个字符。 应注意文件指针和文件内部的位置指针不是一回事。文件指针是指向整个文件的，须在程序中定义说明，只要不重新赋值，文件指针的值是不变的。文件内部的位置指针用以指示文件内部的当前读写位置，每读写一次，该指针均向后移动，它不需在程序中定义说明，而是由系统自动设置的。
[例10.1]读入文件e10-1.c，在屏幕上输出。
#include<stdio.h>
main()
{
FILE *fp;
char ch;
if((fp=fopen("e10_1.c","rt"))==NULL)
{
printf("Cannot open file strike any key exit!");
getch();
exit(1);
}
ch=fgetc(fp);
while (ch!=EOF)
{
putchar(ch);
ch=fgetc(fp);
}
fclose(fp);
}
　　本例程序的功能是从文件中逐个读取字符，在屏幕上显示。 程序定义了文件指针fp,以读文本文件方式打开文件“e10_1.c”， 并使fp指向该文件。如打开文件出错， 给出提示并退出程序。程序第12行先读出一个字符，然后进入循环，只要读出的字符不是文件结束标志(每个文件末有一结束标志EOF)就把该字符显示在屏幕上，再读入下一字符。每读一次，文件内部的位置指针向后移动一个字符，文件结束时，该指针指向EOF。执行本程序将显示整个文件。
二、写字符函数fputc
　　fputc函数的功能是把一个字符写入指定的文件中，函数调用的形式为： fputc(字符量，文件指针)； 其中，待写入的字符量可以是字符常量或变量，例如：fputc('a',fp);其意义是把字符a写入fp所指向的文件中。
　　对于fputc函数的使用也要说明几点：
1. 被写入的文件可以用、写、读写，追加方式打开，用写或读写方式打开一个已存在的文件时将清除原有的文件内容，写入字符从文件首开始。如需保留原有文件内容，希望写入的字符以文件末开始存放，必须以追加方式打开文件。被写入的文件若不存在，则创建该文件。
2. 每写入一个字符，文件内部位置指针向后移动一个字节。
3. fputc函数有一个返回值，如写入成功则返回写入的字符， 否则返回一个EOF。可用此来判断写入是否成功。
[例10.2]从键盘输入一行字符，写入一个文件， 再把该文件内容读出显示在屏幕上。
#include<stdio.h>
main()
{
FILE *fp;
char ch;
if((fp=fopen("string","wt+"))==NULL)
{
printf("Cannot open file strike any key exit!");
getch();
exit(1);
}
printf("input a string:\n");
ch=getchar();
while (ch!='\n')
{
fputc(ch,fp);
ch=getchar();
}
rewind(fp);
ch=fgetc(fp);
while(ch!=EOF)
{
putchar(ch);
ch=fgetc(fp);
}
printf("\n");
fclose(fp);
}
　　程序中第6行以读写文本文件方式打开文件string。程序第13行从键盘读入一个字符后进入循环，当读入字符不为回车符时，则把该字符写入文件之中，然后继续从键盘读入下一字符。 每输入一个字符，文件内部位置指针向后移动一个字节。写入完毕， 该指针已指向文件末。如要把文件从头读出，须把指针移向文件头，程序第19行rewind函数用于把fp所指文件的内部位置指针移到文件头。 第20至25行用于读出文件中的一行内容。
[例10.3]把命令行参数中的前一个文件名标识的文件， 复制到后一个文件名标识的文件中， 如命令行中只有一个文件名则把该文件写到标准输出文件(显示器)中。
#include<stdio.h>
main(int argc,char*argv[])
{
FILE *fp1,*fp2;
char ch;
if(argc==1)
{
printf("have not enter file name strike any key exit");
getch();
exit(0);
}
if((fp1=fopen(argv[1],"rt"))==NULL)
{
printf("Cannot open %s\n",argv[1]);
getch();
exit(1);
}
if(argc==2) fp2=stdout;
elseif((fp2=fopen(argv[2],"wt+"))==NULL)
{
printf("Cannot open %s\n",argv[1]);
getch();
exit(1);
}
while((ch=fgetc(fp1))!=EOF)
fputc(ch,fp2);
fclose(fp1);
fclose(fp2);
}
　　本程序为带参的main函数。程序中定义了两个文件指针 fp1 和fp2，分别指向命令行参数中给出的文件。如命令行参数中没有给出文件名，则给出提示信息。程序第18行表示如果只给出一个文件名，则使fp2指向标准输出文件(即显示器)。程序第25行至28行用循环语句逐个读出文件1中的字符再送到文件2中。再次运行时，给出了一个文件名(由例10.2所建立的文件)， 故输出给标准输出文件stdout，即在显示器上显示文件内容。第三次运行，给出了二个文件名，因此把string中的内容读出，写入到OK之中。可用DOS命令type显示OK的内容：字符串读写函数ｆｇｅｔｓ和ｆｐｕｔｓ
一、读字符串函数fgets函数的功能是从指定的文件中读一个字符串到字符数组中，函数调用的形式为： fgets(字符数组名，n，文件指针)；其中的n是一个正整数。表示从文件中读出的字符串不超过 n-1个字符。在读入的最后一个字符后加上串结束标志'\0'。例如：fgets(str,n,fp);的意义是从fp所指的文件中读出n-1个字符送入字符数组str中。
[例10.4]从e10_1.c文件中读入一个含10个字符的字符串。
#include<stdio.h>
main()
{
FILE *fp;
char str[11];
if((fp=fopen("e10_1.c","rt"))==NULL)
{
printf("Cannot open file strike any key exit!");
getch();
exit(1);
}
fgets(str,11,fp);
printf("%s",str);
fclose(fp);
}
　　本例定义了一个字符数组str共11个字节，在以读文本文件方式打开文件e101.c后，从中读出10个字符送入str数组，在数组最后一个单元内将加上'\0'，然后在屏幕上显示输出str数组。输出的十个字符正是例10.1程序的前十个字符。
　　对fgets函数有两点说明：
1. 在读出n-1个字符之前，如遇到了换行符或EOF，则读出结束。
2. fgets函数也有返回值，其返回值是字符数组的首地址。
二、写字符串函数fputs
fputs函数的功能是向指定的文件写入一个字符串，其调用形式为： fputs(字符串，文件指针) 其中字符串可以是字符串常量，也可以是字符数组名，或指针 变量，例如：
fputs(“abcd“，fp)；
其意义是把字符串“abcd”写入fp所指的文件之中。[例10.5]在例10.2中建立的文件string中追加一个字符串。
#include<stdio.h>
main()
{
FILE *fp;
char ch,st[20];
if((fp=fopen("string","at+"))==NULL)
{
printf("Cannot open file strike any key exit!");
getch();
exit(1);
}
printf("input a string:\n");
scanf("%s",st);
fputs(st,fp);
rewind(fp);
ch=fgetc(fp);
while(ch!=EOF)
{
putchar(ch);
ch=fgetc(fp);
}
printf("\n");
fclose(fp);
}
　　本例要求在string文件末加写字符串，因此，在程序第6行以追加读写文本文件的方式打开文件string 。 然后输入字符串， 并用fputs函数把该串写入文件string。在程序15行用rewind函数把文件内部位置指针移到文件首。 再进入循环逐个显示当前文件中的全部内容。
数据块读写函数fread和fwrite
Ｃ语言还提供了用于整块数据的读写函数。 可用来读写一组数据，如一个数组元素，一个结构变量的值等。读数据块函数调用的一般形式为： fread(buffer,size,count,fp); 写数据块函数调用的一般形式为： fwrite(buffer,size,count,fp); 其中buffer是一个指针，在fread函数中，它表示存放输入数据的首地址。在fwrite函数中，它表示存放输出数据的首地址。 size 表示数据块的字节数。count 表示要读写的数据块块数。fp 表示文件指针。
例如：
fread(fa,4,5,fp); 其意义是从fp所指的文件中，每次读4个字节(一个实数)送入实数组fa中，连续读5次，即读5个实数到fa中。
[例10.6]从键盘输入两个学生数据，写入一个文件中， 再读出这两个学生的数据显示在屏幕上。
#include<stdio.h>
struct stu
{
char name[10];
int num;
int age;
char addr[15];
}boya[2],boyb[2],*pp,*qq;
main()
{
FILE *fp;
char ch;
int i;
pp=boya;
qq=boyb;
if((fp=fopen("stu_list","wb+"))==NULL)
{
printf("Cannot open file strike any key exit!");
getch();
exit(1);
}
printf("\ninput data\n");
for(i=0;i<2;i++,pp++)
scanf("%s%d%d%s",pp->name,&pp->num,&pp->age,pp->addr);
pp=boya;
fwrite(pp,sizeof(struct stu),2,fp);
rewind(fp);
fread(qq,sizeof(struct stu),2,fp);
printf("\n\nname\tnumber age addr\n");
for(i=0;i<2;i++,qq++)
printf("%s\t%5d%7d%s\n",qq->name,qq->num,qq->age,qq->addr);
fclose(fp);
}
　　本例程序定义了一个结构stu,说明了两个结构数组boya和 boyb以及两个结构指针变量pp和qq。pp指向boya,qq指向boyb。程序第16行以读写方式打开二进制文件“stu_list”，输入二个学生数据之后，写入该文件中， 然后把文件内部位置指针移到文件首，读出两块学生数据后，在屏幕上显示。
格式化读写函数ｆｓｃａｎｆ和ｆｐｒｉｎｔｆ
fscanf函数，fprintf函数与前面使用的scanf和printf 函数的功能相似，都是格式化读写函数。 两者的区别在于 fscanf 函数和fprintf函数的读写对象不是键盘和显示器，而是磁盘文件。这两个函数的调用格式为： fscanf(文件指针，格式字符串，输入表列)； fprintf(文件指针，格式字符串，输出表列)； 例如：
fscanf(fp,"%d%s",&i,s);
fprintf(fp,"%d%c",j,ch); 
用fscanf和fprintf函数也可以完成例10.6的问题。修改后的程序如例10.7所示。
[例10.7]
#include<stdio.h>
struct stu
{
char name[10];
int num;
int age;
char addr[15];
}boya[2],boyb[2],*pp,*qq;
main()
{
FILE *fp;
char ch;
int i;
pp=boya;
qq=boyb;
if((fp=fopen("stu_list","wb+"))==NULL)
{
printf("Cannot open file strike any key exit!");
getch();
exit(1);
}
printf("\ninput data\n");
for(i=0;i<2;i++,pp++)
scanf("%s%d%d%s",pp->name,&pp->num,&pp->age,pp->addr);
pp=boya;
for(i=0;i<2;i++,pp++)
fprintf(fp,"%s %d %d %s\n",pp->name,pp->num,pp->age,pp->
addr);
rewind(fp);
for(i=0;i<2;i++,qq++)
fscanf(fp,"%s %d %d %s\n",qq->name,&qq->num,&qq->age,qq->addr);
printf("\n\nname\tnumber age addr\n");
qq=boyb;
for(i=0;i<2;i++,qq++)
printf("%s\t%5d %7d %s\n",qq->name,qq->num, qq->age,
qq->addr);
fclose(fp);
}
　　与例10.6相比，本程序中fscanf和fprintf函数每次只能读写一个结构数组元素，因此采用了循环语句来读写全部数组元素。还要注意指针变量pp,qq由于循环改变了它们的值，因此在程序的25和32行分别对它们重新赋予了数组的首地址。
