# C 语言sscanf复杂应用。 - xueyong4712816的专栏 - CSDN博客












2013年11月04日 09:09:34[xueyong4712816](https://me.csdn.net/xueyong4712816)阅读数：792








sscanf() - 从一个字符串中读进与指定格式相符的数据. 

　　函数原型: 

　　Int sscanf( string str, string fmt, mixed var1, mixed var2 ... ); 

　　int scanf( const char *format [,argument]... ); 

　　说明： 

　　sscanf与scanf类似，都是用于输入的，只是后者以屏幕(stdin)为输入源，前者以固定字符串为输入源。 

　　其中的format可以是一个或多个 {%[*] [width] [{h | l | I64 | L}]type | ' ' | '\t' | '\n' | 非%符号}


　　注： 

　　1、 * 亦可用于格式中, (即 %*d 和 %*s) 加了星号 (*) 表示跳过此数据不读入. (也就是不把此数据读入参数中) 

　　2、{a|b|c}表示a,b,c中选一，[d],表示可以有d也可以没有d。 

　　3、width表示读取宽度。 

　　4、{h | l | I64 | L}:参数的size,通常h表示单字节size，I表示2字节 size,L表示4字节size(double例外),l64表示8字节size。


　　5、type :这就很多了，就是%s,%d之类。 

　　6、特别的：%*[width] [{h | l | I64 | L}]type 表示满足该条件的被过滤掉，不会向目标参数中写入值 

　　支持集合操作： 

　　%[a-z] 表示匹配a到z中任意字符，贪婪性(尽可能多的匹配) 

　　%[aB'] 匹配a、B、'中一员，贪婪性 

　　%[^a] 匹配非a的任意字符，贪婪性 

例子： 

　　1. 常见用法。 

　　char buf[512] = ; 

　　sscanf("123456 ", "%s", buf); 

　　printf("%s\n", buf); 

　　结果为：123456 

　　2. 取指定长度的字符串。如在下例中，取最大长度为4字节的字符串。 

　　sscanf("123456 ", "%4s", buf); 

　　printf("%s\n", buf); 

　　结果为：1234 

　　3. 取到指定字符为止的字符串。如在下例中，取遇到空格为止字符串。 

　　sscanf("123456 abcdedf", "%[^ ]", buf); 

　　printf("%s\n", buf); 

　　结果为：123456 

　　4. 取仅包含指定字符集的字符串。如在下例中，取仅包含1到9和小写字母的字符串。 

　　sscanf("123456abcdedfBCDEF", "%[1-9a-z]", buf); 

　　printf("%s\n", buf); 

　　结果为：123456abcdedf 

　　5. 取到指定字符集为止的字符串。如在下例中，取遇到大写字母为止的字符串。 

　　sscanf("123456abcdedfBCDEF", "%[^A-Z]", buf); 

　　printf("%s\n", buf); 

　　结果为：123456abcdedf 

　　6、给定一个字符串iios/12DDWDFF@122，获取 / 和 @ 之间的字符串，先将 "iios/"过滤掉，再将非'@'的一串内容送到buf中 

　　sscanf("iios/12DDWDFF@122", "%*[^/]/%[^@]", buf); 

　　printf("%s\n", buf); 

　　结果为：12DDWDFF 

　　7、给定一个字符串““hello, world”，仅保留world。（注意：“，”之后有一空格） 

　　sscanf(“hello, world”, "%*s%s", buf); 

　　printf("%s\n", buf); 

　　结果为：world 

　　%*s表示第一个匹配到的%s被过滤掉，即hello被过滤了 

　　如果没有空格则结果为NULL。 

　　sscanf的功能很类似于正则表达式, 但却没有正则表达式强大,所以如果对于比较复杂的字符串处理,建议使用正则表达式. 

　　//------------------------------------------------------- 

　　sscanf,表示从字符串中格式化输入 

　　上面表示从str中，输入数字给x，就是32700 

　　久以前，我以为c没有自己的split string函数，后来我发现了sscanf；一直以来，我以为sscanf只能以空格来界定字符串，现在我发现我错了。 

　　sscanf是一个运行时函数，原形很简单： 

　　int sscanf( 

　　const char *buffer, 

　　const char *format [, 

　　argument ] ... 

　　); 

　　它强大的功能体现在对format的支持上。 

　　我以前用它来分隔类似这样的字符串2006:03:18: 

　　int a, b, c; 

　　sscanf("2006:03:18", "%d:%d:%d", a, b, c); 

　　以及2006:03:18 - 2006:04:18: 

　　char sztime1[16] = "", sztime2[16] = ""; 

　　sscanf("2006:03:18 - 2006:04:18", "%s - %s", sztime1, sztime2); 

　　但是后来，我需要处理2006:03:18-2006:04:18 

　　仅仅是取消了‘-’两边的空格，却打破了%s对字符串的界定。 

　　我需要重新设计一个函数来处理这样的情况？这并不复杂，但是，为了使所有的代码都有统一的风格，我需要改动很多地方，把已有的sscanf替换成我自己的分割函数。我以为我肯定需要这样做，并伴随着对sscanf的强烈不满而入睡；一觉醒来，发现其实不必。


　　format-type中有%[]这样的type field。如果读取的字符串，不是以空格来分隔的话，就可以使用%[]。 

　　%[]类似于一个正则表达式。[a-z]表示读取a-z的所有字符，[^a-z]表示读取除a-z以外的所有字符。 

　　所以那个问题也就迎刃而解了: 

　　sscanf("2006:03:18 - 2006:04:18", "%[0-9,:] - %[0-9,:]", sztime1, sztime2); 

下面就是我们的需求，需要从soldier(20).PK(monster(10)) 解析为"%s(%d).%s(%s(%d))"

```cpp
FILE *fin,*fout;
   if((fin = fopen("D:\\task\\oot\\test\\input.txt","rb"))==NULL)   
   {
      printf("Could not open input.txt");
	  return 0;
   }
   printf("open D:\\task\\oot\\test\\input.txt\n");
   if((fout = fopen("D:\\task\\oot\\test\\output.txt","wb"))==NULL)   
   {
      printf("Could not open output.txt");
	  return 0;
   }
   printf("open D:\\task\\oot\\test\\output.txt\n");
   char s[101];
   char act[100];
   char magic[100];
   char monster[100];
   int  actlevel,monsterlevel;
   int  acttype = 0 ;  /*0 代表战士，1代表魔法师*/
   int  usemagic = 0 ; /*0 代表使用，1代表不使用*/
   int  magictype = 0 ;/*0 代表第一种魔法，1代表第二种魔法*/
   int  result = 0;
   while(fgets(s,100,fin))
   {
		  sscanf(s,"%[a-z](%i).%[a-zA-Z](%[a-z](%i))",act,&actlevel,magic,monster,&monsterlevel);
		  if(strcmp(act,"soldier") == 0)
		  {
			  acttype = 0;
		  }
		  else
		  {
			  acttype = 1;
		  }
		  if(strcmp(magic,"PK") == 0)
		  {
			  usemagic = 0;
		  }
		  else
		  {
			  usemagic = 1;
			  if((strcmp(magic,"rampagemagic")== 0)||(strcmp(magic,"firemagic") == 0))
			  {
				  magictype = 0;
			  }
			  else
			  {
				  magictype = 1;
			  }
		  }
		  result = fight(acttype,usemagic,magictype,actlevel,monsterlevel);/*通过解析出来的参数进行相关函数处理*/
		  if(result == 0)
		  {
			  //printf("%s\n",monster);
			  fputs(monster,fout);
			  fputs("\n",fout);
		  }
		  else
		  {
			  //printf("%s\n",act);
			  fputs(act,fout);
			  fputs("\n",fout);
		  }

   }
   fclose(fin);
```










