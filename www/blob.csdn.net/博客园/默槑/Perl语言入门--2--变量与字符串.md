# Perl语言入门--2--变量与字符串 - 默槑 - 博客园







# [Perl语言入门--2--变量与字符串](https://www.cnblogs.com/modaidai/p/6690837.html)





一、$

简单变量：$          以$开头+字母或下划线

　　　　　　　　 区分大小写，变量长度不限，没有关键字，但不能和默认变量重复

1、整数的表示：

12

12e2　　#1200

-12　　　#-12

1_200_000     #perl可以使用下划线当做分隔符 数字还是1200000，但方便看

012           #八进制数，值为10

0x1f           #十六进制，值为31   f不分大小写



2、浮点数

11.4

0.3

.3

3.

5.4e2    e或E，+或-，e后面最多一到三位数字



3、注意

整数：最多15位 ，16位后截断

浮点数的精度：指数范围：-309~308 太小的话显示成0，太大显示为1.#INF



二、字符串变量

1.变量替换

$var ="str";

print "this is $var" ; -> '' this is str"

perl遵循最长匹配，当$var 和 $varing 都存在时，print "varing" ，匹配$varing  不匹配$var

想匹配$var需要这样：${var}ing          sh中$(ls)



2.转义字符





3. \ 的作用

作用：转义字符

取消字符含义：

　　取消$的变量标识       print "the \$var is $var \n";

　　取消双引号"             print "A quote \" in a string \n";

　　取消反斜杠\            print "A quote \\ in a string \n";

\nnn (n表示数字)   八进制表示的 ACSII码  print "\045";

\xnn (n表示数字)   十六进制表示的 ACSII码  print "\x45";



4.单引号

不进行变量替换  

不进行转义

字符创可以换行  ‘sadas

　　　　　　　　　　　　esdadad　　’

\的作用：

　　有单引号时取消其含义

　　有\时  取消含义



5、字符串的特殊表示

qq(string in qq)　　　  #两个q表示双引号 ,也就是这个  - ->"string in q"

q(string in q)                 #一个q表示单引号 ,也就是这个  - ->‘string in q’

 好处就是如果字符串里面有单引号、双引号的话，不用加反斜线注释掉

()可换为<>,{},[]等配对字符 可出现这样的情况：[string [in] q]    [string <in> q]      也是可以的

也可以这样：  qq/string in q/        qq;string in q;



6、变量初值

未创建状态为undef，到达文件尾也是undef

用法：

　　说明变量为未定义：undef $a;

　　用在条件判断中：if(undef $a)

 　　代替不关心的变量：$a="a:b:c:d"; ($a1,undef,undef,$d1)=split(/:/,$s);　　　　#这样稍微会省些内存

　　如果有undef变量又不知在哪，可加-w参数进行提示：  #!/usr/bin/perl -w

要是创建了，则创建后状态为defined  一般用在条件判断中  if(defined $a)

整数初值为0，字符串初值为空串""。



7、相关函数

length()串长度，数字位数

三角函数sin，数学函数sqrt，随机产生函数rand，srand

uc lc ucfrist lcfrist    大小写

substr index pos   字符串索引、位置

转换函数 ord，chr，pack，unpack



$lastchar=chop(str)   截去最后一个字符，无论什么字符

$result=chomp(str)  截去末尾的行分割符(\n),行分割符可由$/定义。比如 $/="ab" 把末尾是ab的切掉





**foreach (@array) { s/a/b/; } # 此处每次循环将从 @array 数组中取出一个元素存放在 $_ 变量中，并对 $_ 进行替换处理。**

**while (<FILE>) { print if (m/error/); } # 这一句稍微复杂一些，他将打印 FILE 文件中所有包含 error 字符串的行。**












