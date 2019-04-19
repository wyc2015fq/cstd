# linux shell 和 vim快捷键 - Arthur的随笔 - CSDN博客
2012年12月05日 14:28:31[largetalk](https://me.csdn.net/largetalk)阅读数：3361
from: [http://blog.sina.com.cn/s/blog_7556be720100ryp2.html](http://blog.sina.com.cn/s/blog_7556be720100ryp2.html)
```
Ctrl+p重复上一次命令
Ctrl+a跳到第一个字符前
Ctrl+x同上但再按一次会从新回到原位置
Ctrl+b前移一个字符不删除字符情况下
Ctrl+h删除前一个字符
Ctrl+u删除提示符前的所有字符
Ctrl+w同上
Ctrl+d删除提示符后一个字符或exit或logout
Ctrl+e转到字符尾部
Ctrl+f后移一个字符
Ctrl+k删除提示符后全部字符
Ctrl+k取消
ctrl+r向前查找用过的命令
Ctrl+o Ctrl+y Ctrl+i Crtl+m这4个没搞清楚怎么用
<TAB> 命令补齐
CTRL-I 等同于按制表符<TAB>键
CTRL-W 不是删除光标前的所有字符, 它删除光标前的一个单词
CTRL-P 是recall出上一个命令 <===> CTRL-N 是recall出下一个命令
ESC-F 光标向前步进一个单词
ESC-B 光标向后步进一个单词
CTRL-M 等同于回车键
CTRL-O 等同于回车键
CTRL-V 使下一个特殊字符可以插入在当前位置, 如CTRL-V <TAB> 可以在当前位置插入一个<TAB>字符, 其ASCII是9, 否则一般情况下按<TAB>结果是命令补齐
CTRL-C 撤消当前命令行的编辑, 另起一行.
CTRL-S 暂时冻结当前shell的输入
CTRL-Q 解冻
ESC-c 使下一个单词首字母大写, 同时光标前进一个单词, 如光标停留在单词的某个字母上, 如word中的o字母上, 则o字母变大写. 而不是w
ESC-u 使下一个单词所有字母变大写, 同时光标前进一个单词, 同上, 如光标在o字母上, 则ord变大写, w不变.
ESC-l 同ESC-U, 但使之全变为小写.
把bash所有的ctrl组合键试了一遍，现总结如下(以下出现的所有键都是ctrl组合键)：
1. U K Y
U将光标（不包括）以前的字符删除
K将光标（包括）以后的字符删除
Y将刚才删除的字符粘出来
2. D H
D将光标处的字符删除
H将光标前的一个字符删除
3. A E
A将光标移动到行首
E将光标移动到行尾
4. F B
F将光标向右移动一个字符的位置
B将光标向左移动一个字符的位置
5. N P
N下一个命令
P上一个命令
6. L
L清屏
7. R
R搜索以前输入过的命令
8. T
T将光标处的字符和光标前一个字符替换位置
基本功:
用上下键看命令的历史
左右键区修改内容
tab补齐命令名字或者目录，文件名字，不是唯一的多按2次，会出来列表
!ls 重复运行最后一条以’ls’开头的命令，如果先ls -l 然后ls -lcrt，那么!ls，相当于ls -lcrt
ls abc.txt
vi !$
第二行的vi !$相当于vi abc.txt，!$等于上一个命令的参数， ‘$’ 是根据上下文来说的最后一行，列等。
ctrl键组合
ctrl+a:光标移到行首。
ctrl+b:光标左移一个字母
ctrl+c:杀死当前进程。
ctrl+d:退出当前 Shell。
ctrl+e:光标移到行尾。
ctrl+h:删除光标前一个字符，同 backspace 键相同。
ctrl+k:清除光标后至行尾的内容。
ctrl+l:清屏，相当于clear。
ctrl+r:搜索之前打过的命令。会有一个提示，根据你输入的关键字进行搜索bash的history
ctrl+u: 清除光标前至行首间的所有内容。
ctrl+w: 移除光标前的一个单词
ctrl+t: 交换光标位置前的两个字符
ctrl+y: 粘贴或者恢复上次的删除
ctrl+d: 删除光标所在字母;注意和backspace以及ctrl+h的区别，这2个是删除光标前的字符
ctrl+f: 光标右移
ctrl+z : 把当前进程转到后台运行，使用’ fg ‘命令恢复。比如top -d1 然后ctrl+z ，到后台，然后fg,重新恢复
esc组合
esc+d: 删除光标后的一个词
esc+f: 往右跳一个词
esc+b: 往左跳一个词
esc+t: 交换光标位置前的两个单词。
```
from: [http://nffish.com/archives/375](http://nffish.com/archives/375)
```
移动光标类命令
w或W ：光标右移一个字至字首
b或B ：光标左移一个字至字首
e或E ：光标右移一个字至字尾
) ：光标移至句尾
( ：光标移至句首
}：光标移至段落开头
{：光标移至段落结尾
nG：光标移至第n行首
n+：光标下移n行
n-：光标上移n行
n$：光标下移n-1行， 并到行尾
H ：光标移至屏幕顶行
M ：光标移至屏幕中间行
L ：光标移至屏幕最后行
屏幕翻滚类命令
Ctrl+u：向文件首翻半屏
~~Ctrl+d：向文件尾翻半屏~~  (已被我键绑定到 :q)
Ctrl+f：向文件尾翻一屏
Ctrl＋b；向文件首翻一屏
nzz：将第n行滚至屏幕中部，不指定n时将当前行滚至屏幕中部。
插入文本类命令
I ：在当前行首
A：在当前行尾
o：在当前行之下新开一行
O：在当前行之上新开一行
R：替换当前字符及其后的字符，直至按ESC键
s：从当前光标位置处开始，以输入的文本替代指定数目的字符
S：删除指定数目的行，并以所输入文本代替之
ncw或nCW：修改指定数目的字(word)
nC：修改指定数目的行
删除命令
ndw或ndW：删除光标处开始及其后的n-1个字(word)
d0：删至行首
d$：删至行尾
x或X：删除一个字符，x删除光标后的，而X删除光标前的
Ctrl+u：插入模式下， 删除上一行
搜索及替换命令
/pattern：从光标开始处向文件尾搜索pattern
?pattern：从光标开始处向文件首搜索pattern
n：在同一方向重复上一次搜索命令
N：在反方向上重复上一次搜索命令
：s/p1/p2/g：将当前行中所有p1均用p2替代
：n1,n2s/p1/p2/g：将第n1至n2行中所有p1均用p2替代
选项设置
all：列出所有选项设置情况
term：设置终端类型
ignorecase：在搜索中忽略大小写
list：显示制表位(Ctrl+I)和行尾标志（$)
number：显示行号
report：显示由面向行的命令修改过的数目
terse：显示简短的警告信息
warn：在转到别的文件时若没保存当前文件则显示NO write信息
nomagic：允许在搜索模式中，使用前面不带“\”的特殊字符
nowrapscan：禁止vi在搜索到达文件两端时，又从另一端开始
mesg：允许vi显示其他用户用write写到自己终端上的信息
最后行方式命令
：e filename：打开文件filename进行编辑
：x：保存当前文件并退出
：!command：执行shell命令command
：n1,n2 w!command：将文件中n1行至n2行的内容作为command的输入并执行之，若不指定n1，n2，则表示将整个文件内容作为command的输入
：r!command：将命令command的输出结果放到当前行
```
