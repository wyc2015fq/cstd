# getopt();getopt_long();getopt_long_only();option - h13 - 博客园
## 如何分析命令行参数
Sun, 2006-07-16 01:27 — Marchday
GNU/Linux的命令行选项有两种类型：短选项和长选项，前者以 '-' 作为前导符，后者以 '--' 作为前导符
。比如有一个命令：
$ myprog -a vv --add -b --file a.txt b.txt - -- -e c.txt
在GNU/Linux系统，对这种情况的一种合理解释是：
a是短选项，带一个参数vv；
add是长选项，无参数；
b是短选项，无参数；
file是长选项，带一个参数a.txt；
b.txt是参数；
-是参数，通常表示标准输入，stdin；
--是一个指示符，表明停止扫描参数，其后所有部分都是参数，而不是选项；
-e是参数；
c.txt是参数
为了简化程序设计，有几个库函数可以优雅地分析命令行参数，原型如下：
#include <unistd.h>
int getopt(int argc, char * const argv[],
const char *optstring);
extern char *optarg;
extern int optind, opterr, optopt;
#define _GNU_SOURCE
#include <getopt.h>
int getopt_long(int argc, char * const argv[],
const char *optstring,
const struct option *longopts, int *longindex);
int getopt_long_only(int argc, char * const argv[],
const char *optstring,
const struct option *longopts, int *longindex);
我们先看看用于分析短参数的getopt函数。参数含义如下：
argc, argv是从main函数获取的参数，原样传给getopt；
optstring指示如何分析参数。
关于optstring，还有几点说明：
如果选项带参数，该选项后接冒号，比如上例中optstring为"a:b"，指示a带参数，b没有参数；
如果选项带可选参数，该选项后接两个冒号，比如"a::b"，表明a可能有参数，也可能没有；
如果optstring的开头字符为':'，表明如果指明选项带参数，而实际命令行没有参数时，getopt返回':'而不是'?'(默认情况下返回'?'，和无法识别的参数返回一样)；
如果optstring的开头字符为'+'，表明一但遇到一个无选项参数，马上停止扫描，随后的部分当作参数来解释；
如果optstring的开头字符为'-'，表明如果遇到无选项参数，则把它当作选项1（不是字符'1'）的参数
该函数每解析完一个选项，就返回该选项字符。
如果选项带参数，参数保存在optarg中。如果选项带可选参数，而实际无参数时，optarg为NULL。
当遇到一个不在optstring指明的选项时，返回字符‘?’。如果在optstring指明某选项带参数而实际没有参数时，返回字符‘?’或者字符‘:’，视optstring的第一个字符而定。这两种情况选项的实际值被保存在optopt中。
当解析错误时，如果opterr为1则自动打印一条错误消息（默认），否则不打印。
当解析完成时，返回-1。
每当解析完一个argv，optind就会递增。如果遇到无选项参数，getopt默认会把该参数调后一位，接着解析下一个参数。如果解析完成后还有无选项的参数，则optind指示的是第一个无选项参数在argv中的索引。
函数getopt_long()的工作方式类似于getopt()，不过它还能接收长选项。在接收长选项之前，我们必须定义个一个结构体数组变量longopts，指明我们希望获取的长选项。
struct option {
const char *name;
int has_arg;
int *flag;
int val;
};
含义如下：
name指明长选项的名称；
has_arg指明该选项是否带参数，1为是，0为否，2为可选；
flag指明长选项如何返回，如果flag为NULL，则getopt_long返回val。否则返回0,flag指向一个值为val的变量。如果该长选项没有发现，flag保持不变；
val指明返回的值，或者需要加载到被flag所指示的变量中。
option数组的最后一个元素必须全部填充0.
getopt_long的最后一个参数longindex在函数返回时指向被搜索到的选项在longopts数组中的下标。longindex可以为NULL，表明不需要返回这个值。
getopt_long_only类似于getopt_long，但是它把'-'开头的选项当作长选项来处理。如果该选项与长选项不匹配，而与短选项匹配，则可以作为短选项解析。
在短选项找到的时候，getopt_long和getopt_long_only的表现和getopt一样。如果长选项找到了，如果flag为 NULL，返回val，否则返回0。错误情况的处理和getopt一样，只是返回'?'时还可能是别的情况引起的：选项含糊不明确或者无关参数。
我们拿Linux手册的一个例子来说事。
#include <stdio.h> /* for printf */
#include <stdlib.h> /* for exit */
#include <getopt.h>
int
main (int argc, char **argv) {
int c;
int digit_optind = 0;
while (1) {
int this_option_optind = optind ? optind : 1;
int option_index = 0;
static struct option long_options[] = {
{"add", 1, 0, 0},
{"append", 0, 0, 0},
{"delete", 1, 0, 0},
{"verbose", 0, 0, 0},
{"create", 1, 0, ’c’},
{"file", 1, 0, 0},
{0, 0, 0, 0}
};
c = getopt_long (argc, argv, "abc:d:012",
long_options, &option_index);
if (c == -1)
break;
switch (c) {
case 0:
printf ("option %s", long_options[option_index].name);
if (optarg)
printf (" with arg %s", optarg);
printf ("/n");
break;
case ’0’:
case ’1’:
case ’2’:
if (digit_optind != 0 && digit_optind != this_option_optind)
printf ("digits occur in two different argv-elements./n");
digit_optind = this_option_optind;
printf ("option %c/n", c);
break;
case ’a’:
printf ("option a/n");
break;
case ’b’:
printf ("option b/n");
break;
case ’c’:
printf ("option c with value ‘%s’/n", optarg);
break;
case ’d’:
printf ("option d with value ‘%s’/n", optarg);
break;
case ’?’:
break;
default:
printf ("?? getopt returned character code 0%o ??/n", c);
}
}
if (optind < argc) {
printf ("non-option ARGV-elements: ");
while (optind < argc)
printf ("%s ", argv[optind++]);
printf ("/n");
}
exit (0);
}
我们用digit_optind和this_option_optind来跟踪选项012是否在一起，比如选项 -012 和-0 -1 -2 的optind情况是不一样的，前者返回0、1、2时optind相同，而后者optind的值依次大1。
## Comments
### [SHELL编程如何解析命令行参数](http://www.devfront.com:8080/?q=node/139#comment-24)
用命令getopt，提供以下代码片段供参考：
#!/bin/sh
while getopts xyz: arguments 2>/dev/null
do
case $arguments in
x) echo "option x";;
y) echo "option y";;
z) echo "option z with arg. $OPTARG";;
/?) echo "Usage: optdemo [-xy] [-z argment]"
exit 1;;
esac
done
echo "/$OPTIND is $OPTIND /$OPTERR is $OPTERR"
