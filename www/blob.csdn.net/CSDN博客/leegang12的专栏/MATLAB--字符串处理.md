# MATLAB--字符串处理 - leegang12的专栏 - CSDN博客
2011年04月30日 18:13:00[leegang12](https://me.csdn.net/leegang12)阅读数：7261
1，matlab字符串处理函数   参考：[http://hi.baidu.com/henryacm/blog/item/44e686275386593dd50742cd.html](http://hi.baidu.com/henryacm/blog/item/44e686275386593dd50742cd.html)
%字符串处理
a=' a';b='b b';c='cccc';m=''
%获取字符串长度
length(a)   
%连接两个字符串,每个字符串最右边的空格被裁切
d=strcat(a,c)
length(d)
%连接多行字符串,每行长度可不等，自动把非最长字符串最右边补空格
   %使与最长字符串相等，会忽略空字符串
e=strvcat(a,b,m)
size(e)
%char连接，空字符串会被空格填满
f=char(a,b,m)
size(f)
%strcmp     比较两个字符串是否完全相等，是，返回真，否则，返回假
%strncmp    比较两个字符串前n个字符是否相等，是，返回真，否则，返回假
%strcmpi    比较两个字符串是否完全相等，忽略字母大小写
%strncmpi   比较两个字符串前n个字符是否相等，忽略字母大小写
%isletter   检测字符串中每个字符时否属于英文字母
%isspace    检测字符串中每个字符是否属于格式字符（空格，回车，制表，换行符等）
%isstrprop 检测字符每一个字符是否属于指定的范围
a='d sdsdsd 15#';
b=isletter(a)
c=isspace(a)
%字符串替换和查找
%strrep进行字符串替换，区分大小写
%   strrep(str1,str2,str3)
%      它把str1中所有的str2字串用str3来替换
%strfind(str,patten) 查找str中是否有pattern，返回出现位置，没有出现返回空数组
%findstr(str1,str2)   查找str1和str2中，较短字符串在较长字符串中出现的位置，没有出现返回空数组
%strmatch(patten,str) 检查patten是否和str最左侧部分一致
%strtok(str,char)     返回str中由char指定的字符串前的部分和之后的部分，
mm='youqwelcome';
[mm1,mm2]=strtok(mm,'q')
% blanks(n)             创建有n个空格组成的字符串
% deblank(str)          裁切字符串的尾部空格
% strtrim(str)          裁切字符串的开头和尾部的空格，制表，回车符
% lower(str)            将字符串中的字母转换成小写
% upper(str)            将字符串中的字母转换成大写
%sort(str)              按照字符的ASCII值对字符串排序
%num2str          将数字转换为数字字符串
%str2num          将数字字符串转换为数字
%mat2str          将数组转换成字符串
%int2str          把数值数组转换为整数数字组成的字符数组
2,matlab字符串操作总结
char(S1,S2,…)
利用给定的字符串或单元数组创建字符数组
double(S)
将字符串转化成ASC码形式
cellstr(S)
利用的给定的字符数组创建字符串单元数组
blanks(n)
生成一个由n个空格组成的字符串
deblank(S)
删除尾部的空格
eval(S) evalc(S)
使用MATLAB解释器求字符串表达式的值
ischar(S)
判断是不是字符串数组
iscellstr(C)
判断是不是字符串单元数组
isletter(S)
判断是不是字母
isspace(s)
判断是不是空格
strcat(S1,S2,…)
将多个字符串水平竖直排列
strvcat(S1,S2,…)
将多个字符串竖直排列
strcmp(S1,S2)
判断字符串是否相等
strncmp(S1,S2,n)
判断前n个字符串是否相等
strcmpi(S1,S2)
判断字符串是否相等（忽略大小写）
strncmpi(S1,S2,n)
判断前n个字符串是否相等（忽略大小写）
strtrim(S1)
删除结尾的空格
findstr(S1,S2)
查找
strfind(S1,S2)
在S1种查找S2
strjust(S1,type)
按照指定的type调整一个字符串数组
strmatch(S1,S2)
查找要求的字符串的下标
strrep(S1,S2,S3)
将字符串S1中出现的S2用S3代替
strtok(S1,D)
查找S1 中的第一个给定的分隔符之前和之后的字符串
upper(S)
将一个字符串成大写
lower(S)
将一个字符串转换为小写
num2str(k)
将数字转换成字符串
int2str(k)
将整数型转换为字符串
mat2str(k)
将矩阵转换为字符串，供eval使用
str2double(S)
将字符串数组转化为数值数组
sprintf(S)
创建含有指定格式的字符串
sscanf(S)
按照指定的控制格式读取字符串
C{3}对内容进行寻址
C（3）对单元索引
（24）利用正在表达式搜索
>> str=’asdfa sfdgdfg tyefgr tyj  sdfg qwfge r rety yi .,m zxfgc fdgh fgh’;
>> regexp(str,’fg’)
ans =
    12    18    29    34    54    63
我们可以使用字符类来匹配指定的字符，如一个字母，一个数字或一个空格符，也可以用来匹配一个字符集。最有用的一个字符类是一个句号（.）,它用来表示任意的单个字符。另外一个有用的字符类是位于方括号中的字符序列或某一部分字符，这一语法用来表示寻找与方括号中任何一个字符元素匹配的字符串子集。例如：
我们要在str中寻找第一个字符为d，最后一个字符为f或g的三个字符组合，可以使用下面的代码，
>> regexpi(str,’d.[fg]’)
ans =
11           28
   >> regexpi(str,’s.[fg]’,'match’)  列出来
ans =
’sdf’    ’sdf’ 
>> regexp(str,’[a-d]’)
ans =
     1     3     5     9    11    28    56    59  列出a-d所有
（25） 关系运算：
xor(x,y)  异或关系
any(X)  X中有一个不为零，结果就为一
all(X)  X中只要有一个为零即为零。
B=B+(B==0)*eps,这样可以避免零的出现
>> B=-2:2
B =
    -2    -1     0     1     2
>> x=sin(B)./B
Warning: Divide by zero.
x =
    0.4546    0.8415       NaN    0.8415    0.4546
>> B=B+(B==0)*eps
B =
   -2.0000   -1.0000    0.0000    1.0000    2.0000
>> x=sin(B)./B
x =
    0.4546    0.8415    1.0000    0.8415    0.4546
（26）流程控制
switch expression
case test_expression
  (commands1)
case test_expression
  (commands2)
case test_expression
  (commands3)
..
end
try
(commands1)
catch
(command2)
end
在这里，将（commands1）中的命令都执行，如果没有错误，程序控制将直接跳到end。如果哟错误，程序控制酒立即跳到catch语句。执行表达式（commands2）.
利用lasterr函数可以返回代码执行遇到的河中错误：
>> matlab(:)
??? Undefined function or variable “matlab”.
>> lasterr
ans =
Undefined function or variable “matlab”.
另外利用lasterror可以将更详细的错误信息返回到一个结构体中:
>> errstr=lasterror
errstr =
       message: ‘Undefined function or variable “matlab”.’
    identifier: ‘MATLAB:UndefinedFunction’
         stack: [0×1 struct]
>> errstr.message
ans =
Undefined function or variable “matlab”.
>>(27) 函数
error(‘afgfadgsdf’)  终止程序，并且返回括号中的内容
error和fprintf一样可以在字符串中格式化显示的数字变量
warning() 一样的 只是程序继续运行。
pcode myfunction  其中myfunction是要编译的m文件，利用这个命令可以创建伪码文件，提高运行速度。
（28）创建用户自己的工具箱
当一个工程鳊好之后，我们可以把它存在toolbox文件夹中，这样matlab就可以把它这些文件再如到高速缓存区去，提高速度。为了便于文件的管理，和日后的维护，我们最好添加两个脚本文件：Readme.m和Contents.m这两个文件不执行任何操作，而全部由一些注释行组成。
（29）getpref 获取  setpref  设置, addpref 添加 rmpref  删除, ispref, uigetpref, uisetpref  工具箱或者函数的优先权
（30）创建命令：
看一下edit命令就知道了。
（31） 函数句柄和匿名函数：
>> il_humps=inline(’1./((x-.3).^2+.01)+1./((x-.9).^2+.04)-6′,’x')
il_humps =
     Inline function:
     il_humps(x) = 1./((x-.3).^2+.01)+1./((x-.9).^2+.04)-6
inline函数从一个字符串创建一个函数，并以x为输入变量。
要验证一个字符串表示的函数或者内联函数，可以使用feval函数。
>> z=feval(’sin’,pi*(0:4)/4)
z =
         0    0.7071    1.0000    0.7071    0.000
>> z=feval(il_humps,[-1 0 1])
z =
   -5.1378    5.1765   16.0000
下面创建一个匿名函数：
>> af_humps=@(x)1./((x-.3).^2+.01)+1./((x-.9).^2+.04)-6
af_humps =
    @(x) 1./((x-.3).^2+.01)+1./((x-.9).^2+.04)-6
>> af_humps(-1:1)
ans =
   -5.1378    5.1765   16.0000
>> a=1;b=2;
>> asda=@(x) sin(a*x).*cos(b*x);
>> asda(-1:1)
ans =
    0.3502         0   -0.3502
>> fhan={@sin @cos}
fhan =
    @sin    @cos
>> fhan{1}(1:2)
ans =
0.8415          0.9093
str2func()
func2str()
(32) 嵌套函数
如果一个函数包含嵌套函数，则无论是嵌套函数还是住函数，都必须以end结尾：
嵌套函数与子函数的区别：
嵌套出现在主函数声明之后的任意位置，而子函数只能出现在主函数之后。
嵌套可以访问主函数工作区中的任何变量值，而子函数只能通过参数访问主函数工作区的某些变量。
主函数可以访问嵌套函数工作区中的所有变量，而只能够通过子函数的输出访问子函数工作区的某些变量
（33）m文件的调试和剖析
keyboard 
mlint
depfun
利用profile可以剖析程序的效率
一种办法：直接打开profile，另外一种办法利用如此下代码：
>> profile on
>> for i=1:100
test(1:100);
end
profile viewer
（34）Matlab数据文件
save and load
save var1 var2 var3
save filename var1 var2
save(‘filename’,’var1’,’var2’,…)
load功能和save相反
vnew=load(‘filename’,’var1’,’var2’)  将变量存在vnew结构体中。
fprintf
>> a=[1 2 3;4 5 6;7 8 9]
a =
     1     2     3
     4     5     6
     7     8     9
>>fid = fopen('exp.txt','w');
  fprintf(fid,'%2.0f/n',a);
  fclose(fid);
>> type exp.txt
1
4
7
2
5
8
3
6
9
fprintf输出是一列一列的从左到右输出。
>> dlmwrite('eg2.dat',a,'/n');
>> type eg2.dat
1
2
3
4
5
6
7
8
9
fprintf输出是一行一行的从上到下输出。
文件的读和写
clc
clear all
fid1 = fopen('F:/example/1.txt','r');
a=fgetl(fid1);
fclose(fid1);
fid2 = fopen('F:/example/2.txt','a');
fprintf(fid2,'%s/n',a);
fclose(fid2);
type F:/example/2.txt%显示文本的内容
fprintf(fid,'%s/r/n',x); %的情况下就可以把数据输出的文件并且换行
