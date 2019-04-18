# SED 手册 - 深之JohnChen的专栏 - CSDN博客

2007年01月19日 11:00:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1877


**SED ****手册**

来源:http://phi.sinica.edu.tw aspac@phi.sinica.edu.tw (2001-07-29 08:05:00)

**SED ****手册**** - 1. Introduction**

1.Introduction  
  Sed(Stream EDitor)为 UNIX 系统上提供将编辑工作自动化的编辑器 , 使用者无需直接编辑资料。使用者可 
  利用 sed 所提供 20 多种不同的函数参数 , 组合(解 [1])它们完成不同的编辑动作。此外 , 由於 sed 都 
  以行为单位编辑文件 , 故其亦是行编辑器(line editor)。  
  一般 sed 最常用在编辑那些需要不断重覆某些编辑动作的文件上 , 例如将文件中的某个字串替换成另一个字 
  串等等。这些相较於一般 UNIX 编辑器(交谈式的, 如 vi、emacs)用手动的方式修改文件 , sed 用起来较省力。 
  下面几节将分别介绍:  

何时使用 sed  
何处获得 sed  
sed 能做那些编辑动作  
sed 如何工作  
1.1 何时使用 sed  
  在修改文件时 , 如果不断地重覆某些编辑动作 , 则可用 sed 自动一次执行这些编辑动作。例如要使 received 
  档内 1000 封电子信件内的发信人属名 "Tom" 改成 "John" , 此时只要在命令列上执行一简单的 sed 命令就可 
  把档内所有的 "Tom" 字串替换成 "John"。  
  再者 , 当文件需要许多不同编辑动作时 , 则 sed 一次可执行那些不同的编辑动作。例如 sed 能一次执行完将 
  文件中所有空白行删除、替换字串、并将使用者输入的文字添加在文件的第六行等等不同的编辑动作。  

1.2 何处获得 sed  
  一般的 UNIX 系统 , 本身即附有 sed。不同的 UNIX 系统所附的 sed 版本亦不尽相同。若读者所使用的 UNIX  
  系统上未附有 sed , 则可透过 anonymous ftp 到下列地方去取得 :  
phi.sinica.edu.tw:/pub/GNU/gnu 
gete.sinica.edu.tw:/unix/gnu 
ftp.edu.tw:/UNIX/gnu 
ftp.csie.nctu.edu.tw:/pub/Unix/GNU 
ftp.fcu.edu.tw: /pub3/UNIX/gnu 
axp350.ncu.edu.tw:/Packages/gnu 
leica.ccu.edu.tw :/pub2/gnu 
mail.ncku.edu.tw :/pub/unix/gnu 
bbs.ccit.edu.tw :/pub1/UNIX/gnu 
prep.ai.mit.edu.tw:/pub/gnu 

1.3 sed 能做那些编辑动作  
  sed 可删除(delete)、改变(change)、添加(append)、插入(insert)、合、交换文件中的资料行 , 或读入其它 
  档的资料到文件中 , 也可替换(substuite)它们其中的字串、或转换(tranfer)其中的字母等等。例如将文件中的 
  连续空白行删成一行、 "local" 字串替换成 "remote" 、"t" 字母转换成 "T"、将第 10 行资料与第 11 资料合 
  等。  

1.4 sed 如何工作  
  如同其它 UNIX 命令 , sed 由标准输入读入编辑文件并由标准输出送出结果。下图表示 sed 将资料行 "Unix" 替 
  换成 "UNIX" ,  

在图中 , 上方 standard input 为标准输入 , 是读取资料之处 ; standard output 为标准输出 , 是送出结果之处 ;  
中间 sed 方块的下面两个虚线方块表示 sed 的工作流程。其中 , 左边虚线方块表示 sed 将标准输入资料置入  
pattern space , 右边虚线方块表示 sed 将 pattern space 中编辑完毕後的资料送到标准输出。  

在虚线方块中 , 两个实线方块分别表示 pattern space 与 sed script。其中 , pattern space 为一缓区 , 它是  
sed 工作场所 ; 而 sed script 则表示一组执行的编辑指令。  

在图中, 左边虚线方块 "Unix" 由标准输入置入 pattern space ; 接着 , 在右边虚线方块中 , sed 执行 sed script  
中的编辑指令 s/Unix/UNIX/ (解 [2]) , 结果 "Unix" 被替换成 "UNIX" , 之後 , "UNIX" 由 pattern space 送到 
标准输出。 

  总合上述所言 , 当 sed 由标准输入读入一行资料并放入 pattern space 时 , sed 依照 sed script 的编辑指令逐 
  一对 pattern space 内的资料执行编辑 , 之後 , 再由 pattern space 内的结果送到标准输出 , 接着再将下一行资 
  料读入。如此重执行上述动作 , 直至读完所有资料行为止。 

**SED 手册 - 2. 使用 sed**

Sed 命令列可分成编辑指令与文件档部份。其中 , 编辑指令负责控制所有的编辑工作 ; 文件档表示所处理的档案。 
   sed 的编辑指令均由位址(address)与函数(function)两部份组成 , 其中 , 在执行时 , sed 利用它的位址参数来 
   决定编辑的对象;而用它的函数参数(解[3])编辑。  
   此外 , sed 编辑指令 , 除了可在命令列上执行 , 也可在档案内执行。其中差别只是在命令列上执行时 , 其前必 
   须加上选项 -e ; 而在档案(解[4])内时 , 则只需在其档名前加上选项 -f。另外 , sed 执行编辑指令是依照它 
   们在命令列上或档内的次序。  

   下面各节 , 将介绍执行命令列上的编辑指令 、sed 编辑指令、执行档案内的编辑指令、执行多个档案的编辑、及 
   执行 sed 输出控制。  

2.1 执行命令列上的编辑指令  
2.2 sed 编辑指令  
2.3 执行档案内的编辑指令  
2.4 执行多个档案的编辑  
2.5 执行 sed 输出控制  
2.1.执行命令列上的编辑指令  
   当编辑指令(参照[section 2.2])在命令列上执行时 , 其前必须加上选项 -e 。其命令格式如下 :  
     sed -e '编辑指令1' -e '编辑指令2' ... 文件档   

其中 , 所有编辑指令都紧接在选项 -e 之後 , 并置於两个 " ' " 特殊字元间。另外 , 命令上编辑指令的执行是由 
左而右。 

   一般编辑指令不多时 , 使用者通常直接在命令上执行它们。例如 , 删除 yel.dat 内 1 至 10 行资料 , 并将其 
   馀文字中的 "yellow" 字串改成 "black" 字串。此时 , 可将编辑指令直接在命令上执行 , 其命令如下 :  

     sed -e '1,10d' -e 's/yellow/black/g' yel.dat   

在命令中 , 编辑指令 '1,10d'(解[5])执行删除 1 至 10 行资料 ; 编辑指令 's/yellow/black/g'(解[6]) ,  
"yellow" 字串替换(substuite)成 "black" 字串。  
2.2 sed 的编辑指令  
   sed 编辑指令的格式如下 :  
              [address1[,address2]]function[argument] 

其中 , 位址参数 address1 、address2 为行数或 regular expression 字串 , 表示所执行编辑的资料行 ; 函数参 
数 function[argument] 为 sed 的内定函数 , 表示执行的编辑动作。  
下面两小节 , 将仔细介绍位址参数的表示法与有哪些函数参数供选择。  

2.2.1 位址(address)参数的表示法 
   实际上 , 位址参数表示法只是将要编辑的资料行 , 用它们的行数或其中的字串来代替表示它们。下面举几个例子 
   说明(指令都以函数参数 d(参照[section4.2]) 为例) :  
删除档内第 10 行资料 , 则指令为 10d。  
删除含有 "man" 字串的资料行时 , 则指令为 /man/d。  
删除档内第 10 行到第 200 行资料, 则指令为 10,200d。  
删除档内第 10 行到含 "man" 字串的资料行 , 则指令为 10,/man/d。  
   接下来 , 以位址参数的内容与其个数两点 , 完整说明指令中位址参数的表示法(同样也以函数参数 d 为例)。  
位址参数的内容:  
位址为十进位数字 : 此数字表示行数。当指令执行时 , 将对符合此行数的资料执行函数参数指示的编辑动作。例如 ,  
删除资料档中的第 15 行资料 , 则指令为 15d(参照[section4.2])。其馀类推 ,如删除资料档中的第 m 行资料 , 则 
指令为 md 。  

位址为 regular expression(参照[附录 A]): 
当资料行中有符合 regular expression 所表示的字串时 , 则执行函数参数指示的编辑动作。另外 , 在  
regular expression 前後必须加上 "/"。例如指令为 /t.*t/d , 表示删除所有含两 "t" 字母的资料行。其中 , "."  
表示任意字元; "*" 表示其前字元可重任意次 , 它们结合 ".*" 表示两 "t" 字母间的任意字串。  

位址参数的个数 : 在指令中 , 当没有位址参数时 , 表示全部资料行执行函数参数所指示的编辑动作; 当只有一位址 
参数时 , 表示只有符合位址的资料行才编辑 ; 当有两个位址参数 , 如 address1,address2 时 , 表示对资料区执行 
编辑 , address1 代表起始资料行 , address2 代表结束资料行。对於上述内容 , 以下面例子做具说明。  

例如指令为  

d  

其表示删除档内所有资料行。  
例如指令为  

5d  

其表示删除档内第五行资料。  
例如指令为  

1,/apple/d 

其表示删除资料区 , 由档内第一行至内有 "apple" 字串的资料行。  
例如指令为  

/apple/,/orange/d  

其表示删除资料区 , 由档内含有 "apple" 字串至含有 "orange" 字串的资料行  
2.2.2 有那些函数(function)参数  
   下页表中介绍所有 sed 的函数参数(参照[chapter 4])的功能。 
函数参数 功能  
: label  建立 script file 内指令互相参考的位置。   
#  建立解   
{ }  集合有相同位址参数的指令。   
!  不执行函数参数。   
=  印出资料行数( line number )。   
a/  添加使用者输入的资料。   
b label  将执行的指令跳至由 : 建立的参考位置。   
c/  以使用者输入的资料取代资料。  
d  删除资料。   
D  删除 pattern space 内第一个 newline 字母 / 前的资料。   
g  拷贝资料从 hold space。   
G  添加资料从 hold space 至 pattern space 。   
h  拷贝资料从 pattern space 至 hold space 。   
H  添加资料从 pattern space 至 hold space 。   
l  印出 l 资料中的 nonprinting character 用 ASCII 码。   
i/  插入添加使用者输入的资料行。   
n  读入下一笔资料。   
N  添加下一笔资料到 pattern space。   
p  印出资料。   
P  印出 pattern space 内第一个 newline 字母 / 前的资料。   
q  跳出 sed 编辑。   
r  读入它档内容。   
s  替换字串。   
t label  先执行一替换的编辑指令 , 如果替换成牛p>则将编辑指令跳至 : label 处执行。   
w  写资料到它档内。   
x  交换 hold space 与 pattern space 内容。   
y  转换(transform)字元。   
虽然 , sed 只有上表所述几个拥有基本编辑功能的函数 , 但由指令中位址参数和指令与指令间的配合 , 也能使  
sed 完成大部份的编辑任务。  
2.3 执行档案内的编辑指令  
   当执行的指令太多 , 在命令列上撰写起来十分混乱 , 此时 , 可将这些指令整理储存在档案 
   (譬如档名为 script_file )内 , 用选项 -f script_file , 则让 sed 执行 script_file 内的编辑指令。其命 
   令的格示如下 :  
      sed -f script_file 文件档   

其中 , 执行 script_file 内编辑指令的顺序是由上而下。例如上一节的例子 , 其可改成如下命令:  
      sed -f ysb.scr yel.dat   

其中 , ysb.scr 档的内容如下 :  
1,10d 
s/yellow/black/g 

   另外 , 在命令列上可混合使用选项 -e 与 -f , sed 执行指令顺序依然是由命令列的左到右, 如执行至 -f 後 
   档案内的指令 , 则由上而下执行。  

2.4 执行多个文件档的编辑  
   在 sed 命令列上 , 一次可执行编辑多个文件档 , 它们跟在编辑指令之後。例如 , 替换 
    white.dat、red.dat、black.dat 档内的 "yellow" 字串成 "blue" , 其命令如下:  
       sed -e 's/yellow/blue/g' white.dat red.dat black.dat  

上述命令执行时 , sed 依 white.dat、red.dat、black.dat 顺序 , 执行编辑指令 s/yellow/blue/(请参照[section 4.1] ,  
进行字串的替换。 

2.5.执行输出的控制  
   在命令列上的选项 -n (解[7]) 表示输出由编辑指令控制。由前章内容得知 , sed 会 "自动的" 将资料由 
    pattern space 输送到标准输出档。但藉着选项 -n , 可将 sed 这 "自动的" 的动作改成 "被动的" 由它所执行的 
    编辑指令(解[8])来决定结果是否输出。  
   由上述可知 , 选项 -n 必须与编辑指令一起配合 , 否则无法获得结果。例如 , 印出 white.dat 档内含有 "white"  
   字串的资料行 , 其命令如下:  

       sed -n -e '/white/p' white.dat    

上面命令中 , 选项 -n 与编辑指令 /white/p (参照[section4.6]) 一起配合控制输出。其中 , 选项 -n 将输出控制权 
移给编辑指令;/white/p 将资料行中含有 "white" 字串印出萤幕。 

**SED 手册 - 3.例**

**一般在实际使用编辑器的过程中 , 常需要执行替换文件中的字串、搬移、删除、与搜寻资料行等等动作。    当然 , 一般交谈式编辑器(如 vi、emacs)都能做得到上述功能 , 但文件一旦有大量上述编辑需求时 , 则    用它们编辑十分没有效率。本章将用举例的方式说明如何用 sed 自动执行这些编辑功能。此外 , 在本章    例中 , 均以下述方式描述文件的需求 :              将文件中...资料 , 执行...(动作) 如此 , 目的是为了能将它们迅速的转成编辑指令。其中 , " ...资料" 部份 , 转成指令中的位址参数表示 ;  "执行...动作" 部份 , 则转成函数参数表示 。另外 , 当 "执行...动作" 要由数个函数参数表示时 , 则可利 用 "{ "与 " }" 集合这些函数参数(解[9]) , 其指令形式如下 :           位址参数{                  函数参数1                  函数参数2                  函数参数3                      .                      :                   } 上述指令表示 , 将对符合位址参数的资料 , 依次执行函数参数1、函数参数2、函数参数3 ... 表示的动作。 下面各节 , 分别举例说明 sed 替换资料、移动、删除资料、及搜寻资料的命令。  3.1 替换文件中的资料  3.2 搬动文件中的资料  3.3 删除文件中的资料  3.4 搜寻文件中的资料  3.1 替换文件中的资料     Sed 可替换文件中的字串、资料行、甚至资料区。其中 , 表示替换字串的指令中的函数参数为 s(参照[section4.1]);     表示替换资料行、或资料区的指令中的函数参数为 c(参照[section4.5])。上述情况以下面三个例子说明。上述情况    以下面三个例子说明。  例一. 将文件中含 "machine" 字串的资料行中的 "phi" 字串 , 替换成为 "beta" 字串。其命令列如下 :  sed -e '/machine/s/phi/beta/g' input.dat(以後文件档都以 input.dat 代表)  例二. 将文件中第 5 行资料 , 替换成句子 "Those who in quarrels interpose, must often wipe a bloody nose."。 其命令列如下  sed -e '5c/ Those must often wipe a bloody nose. ' input.dat 例三. 将文件中 1 至 100 行的资料区 , 替换成如下两行资料 :  How are you? data be deleted! 则其命令列如下  sed -e '1,100c/ How are you?/ data be deleted! ' input.dat 3.2 搬动文件中的资料     使用者可用 sed 中的 hold space 暂存编辑中的资料、用函数参数 w(参照[section4.9])将文件资料搬动到它档内储存、    或用函数参数 r(参照[section4.8])将它档内容搬到文件内。Hold space 是 sed 用来暂存 pattern space 内资料的暂    存器 , 当 sed 执行函数参数 h、H(参照[section4.19])时 , 会将 pattern space 资料暂存到 hold space;当执行函    数参数 x、g、G(参照[section4.22])时 , 会将暂存的资料取到 pattern space 。下面举三个例子说明。  例一. 将文件中的前 100 资料 , 搬到文件中第 300 後输出。其命令列如下 : sed -f mov.scr 文件档 mov.scr 档的内容为  1,100{ H d } 300G 其中 ,  1,100{ H d } 它表示将文件中的前 100 资料 , 先储存(参照[section4.19])在 hold space 之後删除 ;指令 300G (参照[section4.22])  表示 , 将 hold space 内的资料 , 添加在文件中的第 300 资料後输出。  例二. 将文件中含 "phi" 字串的资料行 , 搬至 mach.inf 档中储存。其命令列如下 :  sed -e '/phi/w mach.inf' 文件档 例三. 将 mach.inf 档内容 , 搬至文件中含 "beta" 字串的资料行。其命令列如下 :  sed -e '/beta/r mach.inf' 文件档 另外 , 由於 sed 是一 stream(参照[section1.4])编辑器 , 故理论上输出後的文件资料不可能再搬回来编辑。  3.3 删除文件中的资料     因为 sed 是一行编辑器 , 所以 sed 很容易删除个别资料行或整个资料区。一般用函数参数 d(参照[section4.2])或     D(参照[section4.17]) 来表示。下面举两个例子说明。  将文件内所有空白行全部删除。其命令列为        sed -e '/^$/d' 文件档 regular expression(解[附录 A]) , ^$ 表示空白行。 其中 , ^ 限制其後字串必须在行首; $ 限制其前字串必须在行尾。  将文件内连续的空白行 , 删除它们成为一行。其命令列为     sed -e '/^$/{    N    /^$/D     }' 文件档 其中 , 函数参数 N(参照[section4.16])表示 , 将空白行的下一行资料添加至 pattern space 内。函数参数 /^$/D 表示 ,  当添加的是空白行时 , 删除第一行空白行 , 而且剩下的空白行则再重新执行指令一次。指令重新执行一次 , 删除一行空白行 ,  如此反覆直至空白行後添加的为非空白行为止 , 故连续的空白行最後只剩一空白行被输出。  3.4 搜寻文件中的资料     Sed 可以执行类似 UNIX 命令 grep 的功能。理论上 , 可用 regular expression(参照[附录 A])。例如 , 将文件中含    有 "gamma" 字串的资料行输出。则其命令列如下:                   sed -n -e '/gamma/p' 文件档    但是 , sed 是行编辑器 , 它的搜寻基本上是以一行为单位。因此 , 当一些字串因换行而被拆成两部份时 , 一般的方法    即不可行。此时 , 就必须以合两行的方式来搜寻这些资料。其情况如下面例子:  例. 将文件中含 "omega" 字串的资料输出。其命令列如下  sed -f gp.scr 文件档 gp.scr 档的内容如下 :  /omega/b N h s/.*/n// /omega/b g D 在上述 sed script(解[10]), 因藉着函数参数 b 形成类似 C 语言中的 case statement 结构 , 使得 sed 可分别处理当 资料内含 "omega" 字串 ; 当 "omega" 字串被拆成两行 ; 以及资料内没有"omega" 字串的情况。接下来就依上述的三种情 况 , 将 sed script 分成下面三部份来讨论。  当资料内含 "omega" , 则执行编辑指令  /omega/b 它表示当资料内含 "omega" 字串时 , sed 不用再对它执行後面的指令 , 而直接将它输出。  当资料内没有"omega" , 则执行编辑指令如下  N h s/.*/n// /omega/b 其中 , 函数参数 N(参照[section 4.16]) , 它表示将下一行资料读入使得 pattern space 内含前後两行资料 。函数参 数 h(参照[section 4.19]) , 它表示将 pattern space 内的前後两行资料存入 hold space 。函数参数 s/.*/n// , 它 表示将 pattern space 内的前後两行资料合(解[11])成一行。/omega/b , 它表示如果合後的资料内含 "omega" 字 串 , 则不用再执行它之後的指令 , 而将此资料自动输出 ;  当合後的资料依旧不含 "omega" , 则执行编辑指令如下  g D 其中 , 函数参数 g(参照[section4.21]) , 它表示将 hold space 内合前的两行资料放回 pattern space。 函数参数  D(参照[section4.17]) , 它表示删除两行资料中的第一行资料 , 并让剩下的那行资料 , 重新执行 sed script。如此 ,  无论的资料行内或行间的字串才可搜寻完全。**

**SED ****手册**** - 4.****介绍函数参数**

**本章将以一节一个函数参数的方式 ,介绍所有 sed 提供的函数参数 , 其中有  | s | d | a | i | c | p | l | r | w | y | ! | n | q | = | # | N | D | P | h | H | g | G | x | b | t |  另外 , 在各节中 , 首先简单介绍函数参数功能 , 接着说明函数参数与位址参数配合的格式 , 而其中也一描述  sed 执行此函数参数的工作情形。  4.1 s     函数参数 s 表示替换(substitute)文件内字串。其指令格式如下 :  [address1[ ,address2]] s/pattern/replacemen/[flag]  对上述格式有下面几点说明 :  函数参数 s 最多与两个位址参数配合。  关於 "s/pattern/replacement/[flag]"(解[12]) 有下面几点说明:  pattern : 它为 reguler expression 字串。它表示文件中要被替换的字串。  replacement : 它为一般字串。但其内出现下列字元有特别意义 :  & : 代表其前 pattern 字串。例如  sed -e 's/test/& my car/' 资料档名     指令中 , & 代表 pattern 字串 "test"。故执行後 , 资料档的 "test" 被替换成 "test my car"。  /n : 代表 pattern 中被第 n 个 /( 、/)(参照[附录 A]) 所括起来的字串。例如  sed -e  's//(test/) /(my/) /(car/)/[/2 /3 /1]/' 资料档名     指令中 , /1 表示 "test"、/2 表示 "my"、/1 表示 "car" 字串。故执行後 , 资料档的 "test my car" 被替换 成 "[my car test]"。  / : 可用它来还原一些特殊符号(如上述的 & 与 / )本身字面上的意义 , 或用它来代表换行。  flag : 主要用它来控制一些替换情况 :  当 flag 为 g 时 , 代表替换所有符合(match)的字串 。  当 flag 为十进位数字 m 时 , 代表替换行内第 m 个符合的字串。  当 flag 为 p 时 , 代表替换第一个符合 pattern 的字串後 , 将资料输出标准输出档。  当 flag 为 w wfile 时 , 代表替换第一个符合 pattern 的字串後 , 输出到 wfile 档内(如果 wfile 不存在 , 则会 重新开启名为 wfile 的档案)。  当没有 flag 时 , 则将资料行内第一个符合 pattern 的字串以 replacement 字串来替换 。  delimiter : 在 "/pattern/replace/[flag] " 中 "/" 被当成一 delimiter。除了空白(blank)、换行(newline) 之外 ,  使用者可用任何字元作为 delimiter。例如下述编辑指令          s#/usr#/usr1#g 上述命令中 /verb|#| 为 delimiter。如果用 "/" 做 delimiter , 则 sed 会将 pattern 与 replacement 中的 "/"  当成 delimiter 而发生错误。  例:  题目 : 替换 input.dat 档(後面如果没有特别指定 , 均假设文件档名为 input.dat)内 "1996" 字串成 "1997" , 同时 将这些资料行存入 year97.dat 档内。  说明 : 用函数参数 s 指示 sed 将 "1996" 字串替换成 "1997" , 另外用 s argument 中的 flag w 指示 sed 将替换 过的资料行存入 year97.dat 档内。  sed 命令列:  sed -e 's/1996/1997/w year97.dat' input.dat 4.2 d 函数参数 d 表示删除资料行 , 其指令格式如下:        [address1[ ,address2]] d  对上述格式有下面几点说明:  函数参数 d 最多与两个位址参数配合。  sed 执行删除动作情况如下 :  将 pattern space 内符合位址参数的资料删除。  将下一笔资料读进 pattern space 。  重新执行 sed script。  例 : 可参考 section 3.3。  4.3 a  函数参数 a 表示将资料添加到文件中。其指令格式如下:         [address1] a/        使用者所输入的资料  对上述格式有下面几点说明: 函数参数 a 最多与一个位址参数配合。  函数参数 a 紧接着 "/" 字元用来表示此行结束 , 使用者所输入的资料必须从下一行输入。如果资料超过一行 , 则须在 每行的结尾加入"/"。  sed 执行添加动作情况如下 : 当 pattern space 内资料输出後 , sed 跟着输出使用者所输入的资料。  例 :  题目: 添加 "多工作业系统" 在含 "UNIX" 字串的资料行後。假设 input.dat 档的内容如下 :  UNIX 说明: 用函数参数 a 将所输入的资料添加在含 "UNIX" 字串的资料行後。  sed 命令列如下 :  sed -e '/UNIX/a/ 多工作业系统 ' input.dat  执行上述命令後 , 其输出结果如下 :  UNIX 多工作业系统 4.4 i  函数参数 i 表示将资料插入文件中。其指令格式如下:         [address1] i/        使用者所输入的资料 对上述格式有下面几点说明:  函数参数 i 最多与一个位址参数配合。  函数参数 i 紧接着 "/" 字元用来表示此行结束 , 使用者所输入的资料必须从下一行输入。如果资料超过一行 , 则须在 每行的结尾加入"/"。  sed 执行插入动作的情况如下 : 在 pattern space 内资料输出前 , sed 先输出使用者所输入的资料。  例 :  题目: 将 "文章版权属於中央研究院" 插在 input.dat 档中含 "院长 : 李远哲" 的资料行之前。假设 input.dat 档内 容如下 :  院长 : 李远哲 说明: 用函数参数 i 将资料行 "文章版权属於中央研究院" 插在含 "院长 : 李远哲" 的资料行之前。  sed 命令列如下:  sed -e '/院长 : 李远哲/i/ 文章版权属於中央研究院 ' input.dat 执行上述命令後的输出如下 :  文章版权属於中央研究院 院长 : 李远哲 4.5 c  函数参数 c 表示改变文件中的资料。其格式如下:       [address1[ ,address2]]c/      使用者所输入的资料 对上述格式有下面几点说明:  函数参数 c 最多与两个位址参数配合。  函数参数 c 紧接着 "/" 字元用来表示此行结束 , 使用者所输入的资料必须从下一行输入。如果资料超过一行 , 则须在 每行的结尾加入"/"。  sed 执行改变动作的情况 : 在 pattern space 内资料输出时 , sed 改变它成为使用者所输入的资料。  例 : 参考 section 3.1 之例二、三。  4.6 p  函数参数 p 表示印出资料。其指令格式如下 :       [address1[ , address2]] p  对於上述格式有下面几点说明 :  函数参数 p 最多与两个位址参数配合。  sed 执行印出动作的情况如下 : sed 拷备一份 pattern space 内容至标准输出档。  例 : 参考 section 3.4 开头的内容。  4.7 l  函数参数 l , 除可将资料中的 nonprinting character 以 ASCII码列出外 , 其於均与函数参数 p 相同。例如 , 将下 面 input.dat 档中的 ^[ 以 ASCII 码印出  The Great ^[ is a movie starring Steve McQueen. 执行命令 sed -e 'l' input.dat 後 , 则输出结果如下 :  The Great /003 is a movie starring Steve McQueen. The Great     is a movie starring Steve McQueen. 上述第二行资料为 sed 的自动输出(请参照解[]。  4.8 r  函数参数 r 表示读入它档案内容到文件中。其指令格式如下 :         [address1] r 它档名称 对於上述格式有下面几点说明 :  函数参数 r 最多与一个位址参数配合。  在指令中 , 函数参数 r 与它档名称间 , 只能有一空格。  sed 执行读入动作的情况如下 : 在 pattern space 内资料输出後 , sed 读出它档的内容跟着输出。当它档不存在时 ,  sed 照样执行其它指令而不会有任何错误讯息产生。 例 : 参考 section 3.1 之例三。  4.9 w  函数参数 w 表示将文件中的写到它档内。其指令格式如下 :       [address1[ ,address2]] w 它档名称 对於上述格式有下面几点说明 :  函数参数 w 最多与两个位址参数配合。  在指令中 , 函数参数 w 与它档名称间 , 只能有一空格。  sed 执行写出动作的情况如 : 将 pattern space 内资料写到它档内。资料写入时 , 会取代(overwrite)原来档案内的资 料。另外 , 当它档不存在时 , sed 会重新产生(creat)它。  例:参考 section 3.1 之例二。  4.10 y  函数参数 y 表示转换资料中的字元。其指令格式如下 :       [address1[ ,address2]]y /xyz.../abc.../ 对於上述格式有下面几点说明 :  函数参数最多配合两个位址参数。  指令中 , /abc.../xyz.../(x、y、z、a、b、c 代表某些字元) 为 y 的 argument 。其中 abc... 与 xyz... 的字元个数 必须相同。  sed 执行转换时 , 将 pattern space 内资料内的 a 字元转换成 x 字元 、b 字元转换成 y 字元 、c 字元转换成 z 字 元 ...。  例:  题目: 将 input.dat 档中的小写字母改成大写。假设 input.dat 档的内容如下 :  Sodd's Second Law:         Sooner or later, the worst possible set of          circumstances is bound to occur. 说明:利用函数参数 y 指示 sed 做字母大小的转换。  sed 命令列如下 :   sed -e ' y/abcdefghijklmnopqrstuvwxyz/ABCDEFGHIJKLMNOPQRSTUVWXYZ/ ' input.dat 执行上述命令输出结果如下 :  SODD'S SECOND LAW:         SOONER OR LATER, THE WORST POSSIBLE SET OF          CIRCUMSTANCES IS BOUND TO OCCUR. 4.11 !  函数参数 ! 表示不执行函数参数。当有如下指令时 ,       [address1[ , address2]] ! 函数参数 表示 , 对符合位址参数之资料不执行函数参数。例如删除 , 除了含 "1996" 字串 , 所有资料行 , 则执行如下命令  sed -e '/1996/!d' input.dat 4.12 n  函数参数 n 表示读入下一行资料。其指令格式如下:       [address1[ ,address2]] n  对上述格式有下面几点说明 :  函数参数 n 最多配合两个位址参数。  sed 执行读入下一行动作的情况如下 :  输出在 pattern space 的资料。  将下一笔资料读到 pattern space。  执行下一个编辑指令。  例(可与[section4.18]中的例比较):  题目 : 输出 input.dat 档内偶数行资料。假设 input.dat 档内容如下:  The UNIX Operation System 说明: 在命令列上  以选项 -n , 将资料输出的控制权(参照[section2.5])转给指令。  利用函数参数 n 将下一行资料(偶数行)取代 pattern space 内的资料行(奇数行)。  利用函数参数 p 将 pattern space 内的资料(偶数行)输出。  最後 , 整个输出只有原先档内的偶数行资料。  sed 命令列如下 :  sed -n -e 'n' -e 'p' infro.dat 执行上述命令後 , 输出的结果如下 :  UNIX System 4.13 q  函数参数 q 表示跳离 sed 。其指令格式如下:       [address1] q  对上述格式有下面几点说明 :  函数参数 q 最多配合一个位址参数。  sed 执行跳离动作时 , 它停止输入 pattern space 资料 , 同时停止资料送到标准输出档。  例 :  题目: 对文件档执行 script_file 内的编辑指令 , 除非遇到 "Linux" 字串。  说明: 无论 script_file 内是何种指令 , 使用者只要在命令列上用指令/Linux/q , 函数参数 q 会强迫 sed 遇 到 "Linux" 时做跳离动作。  sed 命令列如下 :  sed -e '/Linux/q' -f script_file  input.dat 4.14 =  函数参数 = 表示印出资料的行数。其指令格式如下:       [address1 ,[address2]] =  对上述格式有下面几点说明 :  函数参数 = 最多配合两个位址参数。  执行时 , 行数将在资料输出前先输出。  例 :  题目: 印出 input.dat 档内资料行数。假设 input.dat 的内容如下 :  The UNIX Operating System 说明 : 用函数参数 = 来印出资料的行数。  sed 命令列如下 :   sed -e '=' input.dat 执行上述命令後 , 输出的结果如下 :  1 The UNIX 2 Operating System 4.15 #  在 script file 内 , 函数参数 # 後的文字为注解。当注解文字超过多行时 , 其行间须以 "/" 换行字元相隔。 4.16 N  函数参数 N 表示添加下一笔资料在 pattern space 内。其指令格式如下:       [address1 ,[address2]] N  对上述格式有下面几点说明 :  函数参数 N 最多配合两个位址参数。  sed 执行时 , 将下一行资料读入并添加在 pattern space 内 , 资料行间以换行字元(embedded newline character) 分隔。此外 , 在替换时 , 换行字元可用 /n 来 match。  例 :  题目: 将下述两行资料合。假设 input.dat 的内容如下 :  The UNIX Operating System 说明 : 先利用函数参数 N 将两行资料置於 pattern space 内 , 在利用函数参数 s//n/ / 将两行资料间的分隔号 /n  以空白替代 , 如此两行资料变成一行输出。  sed 命令列如下 :   sed -e 'N' -e 's//n/ /' input.dat 执行上述命令後 , 其输出的结果如下:  The UNIX Operating System 4.17 D  函数参数 D 表示删除 pattern space 内的第一行资料。其指令格式如下:       [address1,address2]D 对上述格式有下面几点说明 :  函数参数 D 最多配合两个位址参数。  函数参数 D 与 d 的比较如下 :  当 pattern space 内只有一资料行时 , D 与 d 作用相同。  当 pattern space 内有多行资料行时  D 表示只删除 pattern space 内第一行资料 ; d 则全删除。  D 表示执行删除後 , pattern space 内不添加下一笔资料 , 而将剩下的资料重新执行 sed script ; d 则读入下一行後 执行 sed script。  例 : 参考 section 3.3 的第二个例子。  4.18 P  函数参数 P 表示印出 pattern space 内的第一行资料。其指令格式如下:       [address1,address2] P  对上述格式有下面几点说明 :  函数参数 P 最多配合两个位址参数。  P 与 p , 除了面对的 pattern space 内的资料行数不同外 , 其它均相同。  例(可与[section4.12]中的例):  题目 : 输出 input.dat 档内奇数行资料。假设 input.dat 档内容如下:  The UNIX System 说明: 在命令列上  以选项 -n , 将资料输出的控制权(参照[section2.5])转给指令。  利用函数参数 N 将偶数行添加至 pattern space 内奇数行後。  利用函数参数 P 将 pattern space 内的第一行(奇数行)输出。  在奇数行输出後 , pattern space 内剩下的资料行(偶数行)则被放弃输出。最後 , 整个输出只有原先的奇数行资料。  sed 命令列 :  sed -n -e 'N' -e 'P' infro.dat 执行上述命令後 , 输出的结果如下 :  The System 4.19 h  函数参数 h 表示暂存 pattern space 的资料至 hold space。其指令格式如下:       [address1 ,[address2]] h  对上述格式有下面几点说明 :  函数参数 h 最多配合两个位址参数。  sed 执行暂存动作时 , 会盖掉(overwrite) hold space 内原来的资料。  当 sed 全部执行结束时 , hold space 内资料会自动清除。  例 :参考 section 3.4 的例子。  4.20 H  函数参数 H 与 h 唯一差别是 , sed 执行 h 时 , 资料盖掉(overwrite) hold space 内原来的资料 , 而 H , 资料则 是 "添加(append)" 在 hold space 原来资料後。例题请参考 section 3.2 之例一。  4.21 g     函数参数 g 表示与函数参数 h 相反的动作 , 它表示将 hold space 内资料放回 pattern space 内。其指令格式如下 :       [address1,address2]g 函数参数 g 最多配合两个位址参数。  sed 执行放回动作时 , 资料盖掉(overwrite)(解[13]) pattern space 内原来的资料。  例题 :参考 section 3.4 的例子。  4.22 G  函数参数 G 与 g 唯一差别是 , sed 执行 g 时 , 资料盖掉(overwrite) pattern space 内原来的资料 , 而 G , 资料则 是 "添加(append)" 在 pattern space 原来资料後。例子请参考 section 3.2 例一。  4.23 x     函数参数 x 表示交换 hold space 与 pattern space 内的资料。其指令格式如下 :       [address1 ,[address2]] x  函数参数 x 大部份与其它处理 hold space 的函数参数一起配合。例如 , 将 input.dat 档内第 1 行资料取代第 3 行资 料。此时 , 用函数参数 h 与 x 来配合。其中 , 以函数参数 h 将第 1 资料存入 hold space ; 当第 3 行资料出现在  pattern space , 以函数参数 x 交换 hold space 与 pattern space 的内容。如此 , 第 3 行资料就被第 1 资料替代。 其命令列如下:  sed -e '1h' -e '3x' input.dat 4.24 b、:label  函数参数 : 与函数参数 b 可在 sed script 内建立类似 BASIC 语言中 GOTO 指令的功能。其中 , 函数参数 : 建立标记; 函数参数 b 将下一个执行的指令 branch 到标记处执行。函数参数 : 与 b , 在 script file 内配合的情况如下                     .                    .                    .               编辑指令m1               :记号               编辑指令m2                    .                    .                    .                              [address1,[address2]]b [记号]                   其中 , 当 sed 执行至指令 [address1,[address2]]b [记号] 时 , 如 pattern space 内的资料符合位址参数 , 则 sed  将下一个执行的位置 branch 至由 :记号(解[14])设定的标记处 , 也就是再由 "编辑指令m2" ... 执行。另外 , 如果 指令中函数参数 b 後没有记号 , 则 sed 将下一个执行的指令 branch 到 script file 的最後 , 利用此可使 sed script  内有类似 C 语言中的 case statement 结构。 例 :  题目: 将 input.dat 档内资料行的开头字母重覆印 40 次。假设 input.dat 档的内容如下 : A B C 说明: 用指令 b p1 与 :p1 构成执行增加字母的圈(loop) , 同时在字母出现 40 个时 , 也用指令 b 来跳出圈。下 面就以档内第一行资料 "A" 为例 , 描述它如何连续多添加 39 个 "A" 在同一行:  用指令 s/A/AA/(参照 section4.1)将 "A" 替换成 "AA"。  用指令 b p1 与 :p1 构成圈(loop) , 它目的使上述动作被反覆的执行。每执行一次圈 , 则资料行上的 "A" 就多出 一个。例如 , 第一次圈资料行变成 "AA" , 第二次圈资料行变成 "AAA" ...。  用指令 [ABC]/{40/}/b(解[15]) 来作为停止圈的条件。当资料行有连续 40 个 A 出现时 , 函数参数 b 将执行的指 令跳到最後 , 停止对此行的编辑。  同样 , 对其它资料行也如同上述的方式执行。  sed 命令列如下 :  sed -e '{ :p1 /A/s/A/AA/ /B/s/B/BB/ /C/s/C/CC/ /[ABC]/{40/}/b b p1 }' input.dat 4.25 t  基本上 , 函数参数 t 与 函数参数 b 的功能类似 , 除了在执行 t 的 branch 前 , 会先去测试其前的替换指令有没有 执行替换成功外。在 script file 内的情况如下:                     .                    .                    .               编辑指令m1               :记号               编辑指令m2                    .                    .                    .               s/.../.../               [address1,[address2]]t [记号]               编辑指令m3 其中 , 与函数参数 b 不同处在於 , 执行函数参数 t branch 时 , 会先检查其前一个替换指令成功与否。如成功 , 则 执行 branch ; 不成功 , 则不 branch , 而继续执行下一个编辑指令 , 例如上面的编辑指令m3。  例:  题目 : 将 input.dat 档中资料 A1 替换成 C1、C1 替换成 B1、B1 替换成 A1。input.dat 档的内容如下:  代号  B1  A1  B1  C1   A1  C1 说明 : input.dat 档中全部资料行只需要执行一次替换动作 , 但为避免资料被替换多次 , 所以利用函数参数 t 在  sed script 内形成一类似 C 语言中 case statement 结构 , 使每行资料替换一次後能立即用函数参数 t 跳离替换编辑。  sed 命令列 :  sed -e '{ s/A1/C1/ t s/C1/B1/ t s/B1/A1/ t }' input.dat **

**SED 手册 - 5.常用的 regular expression**

**普通字元  由普通字元所组成的 regular expression 其意义与原字串字面意义相同。   ^字串  限制字串必须出现於行首 。  $字串  限制字串必须出现行尾。   .  表示任意一字元。  [...]  字元集合, 用以表示两中括号间所有字元当中的任一个 ,如 [^...]表示两中括号间所有字元以外的字元。 -&  字元集合中可用"&"指定字元的围。   *  用以形容其前的字元(或字元集合)可重覆任意多次 。  /n  表示嵌入新行字元(imbedded new line character)。   /(.../)  於 regular expression 中使用"/(" "/)"来括住一部份的 regular expression ; 其後可用"/1"来 表示第一个被"/(" "/)"括住的部份。若 regular expression 中使用数次的"/(" "/)"来括住不同的部份 , 则依 次使用"/1","/2","/3",...(最多可到"/9")。  另外 , 在不同平台上 , regular expression 会有一些不同的限制 , 详细情况参照 appendix B。 **

**SED 手册 - 6.解**

**解一. 就是後面将会提到的 sed script。  解二. 指令 s/Unix/UNIX/ 表示将 "Unix" 替换成 "UNIX"。请参照 section 4.1。  解三. 在指令中有 20 几个函数参数可供选择。  解四. 以後这档案称作 script file。  解五. 编辑指令 1,10d 中 , 地址参数为 1,10 , 故 1 至 10 行的资料执行函数参数 d 所指定的删除动作。  解六. 编辑指令 s/yellow/black/g 中 , 由於没有地址参数 , 故所有的资料行都要执行函数参数 s/yellow/black/g  所指定替换动作。在函数参数 s/yellow/black/g 中 , /yellow/black/g 为 s 的 argument , 其表示替换资料 行中所有的 "yellow" 成 "black"。  解七. 其命令格式如下 :  sed -n [-e 编辑指令].. [-f script_file].. [文件档..]  解八. 这些编辑指令中的函数参数可能是 p、l、s 的其中之一。  解九. 在有些情况下 , 也可用编辑指令代替函数参数。例如 section3.3 之例二。  解十. 这里 , sed script 是指 gp.scr 档的内容。它表示这一次 sed 执行的编辑指令。  解十一. 此函数参数 , 表示替换掉(除掉) pattern space 内两行间的换行记号。 故 pattern space 内只有一行资料。  解十二. /pattern/replacement/[flag] 为函数参数 s 的 argument。  解十三. 注意此时 , 虽然资料是放回 pattern space , 但 hold space 的内容还是不变。  解十四. 注意 ":" 与记号间不可有空格。  解十五. 位址参数 [ABC]/{40/} , 表示 40 个 A 字母或 40 个 B 字母或 40 个 C 字母。其中 [ABC] 表示 "A" 或 "B"  或 "C"; 其後的 /{40/} 表示其前的字母有 40 个。regular expression 请参照附录 A**

