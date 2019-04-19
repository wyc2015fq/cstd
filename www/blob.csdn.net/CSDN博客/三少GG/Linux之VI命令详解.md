# Linux之VI命令详解 - 三少GG - CSDN博客
2010年04月27日 23:19:00[三少GG](https://me.csdn.net/scut1135)阅读数：664标签：[linux																[insert																[command																[buffer																[unix																[string](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=unix&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=command&t=blog)](https://so.csdn.net/so/search/s.do?q=insert&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[Ubuntu/OS X系统](https://blog.csdn.net/scut1135/article/category/680808)
vi是所有UNIX系统都会提供的屏幕编辑器，它提供了一个视窗设备，通过它可以编辑文件。当然，对UNIX系统略有所知的人，
或多或少都觉得vi超级难用，但vi是最基本的编辑器，所以希望读者能好好把它学起来，以后在UNIX世界里必将畅行无阻、
游刃有余，因为其他几种文本处理器并非UNIX标准配备。说不定别人的Linux机器没安装joe或pico，如果您不会vi，那您可能就没辄了。
vi的基本概念
基本上vi可分为三种操作状态，分别是命令模式（Command mode）、插入模式（Insert mode）和底线命令模式（Last line mode），
各模式的功能区分如下：
1． Comand mode：控制屏幕光标的移动，**字符或光标的删除**，移动复制某区段及进入Insert mode下，或者到Last line mode。
2． Insert mode：唯有在Insert mode下，才可做**文字数据输入**，按Esc等可回到Comand mode。
3． Last line mode：将**储存文件或离开编辑器**，也可设置编辑环境，如寻找字符串、列出行号等。
不过可以把vi简化成两个模式，即是将Last line mode也算入Command mode,把vi分成Command 和Insert mode。
vi的基本操作
进入vi
在系统提示符号输入vi及文件名称后，即可进入vi全屏幕编辑画面：
$ vi testfile
有一点要特别注意，就是您进入vi之后是处于“Command mode”下，您要切换到Insert mode才能输入文字。
初次用vi的用户都会想先用上下左右键移动光标，结果电脑一直叫，把自己气个半死，所以进入vi后，先不要乱动，
转换入Insert后再说。**切换至Insert mode编辑文件在Command mode下按‘i’、‘a’或‘o’三键就可进入Insert mode。**这时候您就可以开始输入文字了。
i: 插入，从目前光标所在之处插入所输入的文字。
a: 增加，目前光标所在的下一个字开始输入文字。
o: 插入新的一行，从行首开始输入文字。
**Insert的切换→Command mode，按Esc键**
您目前处于Insert mode，您就只能一直打字。假如您发现打错字了，想用光标键往回移动，将该字删除，
就要按ESC键转换回Command mode，再删除文字。
离开vi及存文件
在Command mode下，可按冒号“：”键入入Last line mode，例如：
:w filename (输入“w filename”，将文章存入指定的文件名filename)
:wq (输入“wq”，因为进入之时已经指定文件名testfile，所以会写入testfile并离开vi)
:q! (输入“q!”，强制离开并放弃编辑的文件)
Command mode功能键列表
在介绍command mode指令的时后，指令后面加上“常用”字眼的功能键，表示比较常用的vi指令，
请读者您一定要学会、记住。
（1）I、a、o切换进入Insert mode。[超级常用]
（2）移动光标
vi可以直接用键盘上的光标键来上下左右移动，但正规的vi是用小写英文字母
h、j、k、l，分别控制光标左、下、上、右移一格。
按Ctrl+B：屏幕往后移动一页。[常用]
按Ctrl+F：屏幕往前移动一页。[常用]
按Ctrl+U：屏幕往后移动半页。
按Ctrl+D：屏幕往前移动半页。
按 0 （数字零）：移动文章的开头。[常用]
按 G：移动到文章的最后。[常用]
按 w：光标跳到下个word的开头。[常用]
按 e：光标跳到下个word的字尾。
按 b：光标回到上个word的开头。
按 $：移到光标所在行的行尾。[常用]
按 ^：移到该行第一个非空白的字符。
按 0：移到该行的开头位置。[常用]
按 #：移到该行的第#个位置，例：51、121。[常用]
（3）删除文字
x：每按一次删除光标所在位置的后面一个字符。[超常用]
#x：例如，6x 表删除光标所在位置的后面6个字符。[常用]
X：大字的X，每按一次删除光标所在位置的前面一个字符。
#X：例如，20X 表删除光标所在位置的前面20个字符。
dd：删除光标所在行。[超常用]
#dd：例如，6dd表删除从光标所在的该行往下数6行之文字。[常用]
（4）复制
yw：将光标所在处到字尾的字符复制到缓冲区中。
（想在和#x、#X的功能相反）
p：将缓冲区内的字符粘贴到光标所在位置（指令‘yw’与‘p必须搭配使用）。
yy：复制光标所在行。[超常用]
p：复制单行到您想粘贴之处。（指令‘yy’与‘p’必须搭配使用）
#yy：如：6yy表示拷贝从光标所在的该行往下数6行之文字。[常用]
p:复制多行到您想粘贴之处。（指令‘#yy’与‘p’必须搭配使用）
“ayy：将复制行放入buffer a, vi提供buffer功能，可将常用的数据存在buffer
“ap：将放在buffer a的数据粘贴。
“b3yy：将三行数据存入buffer b。
“b3p：将存在buffer b的资料粘贴
（5）取代
r: 取代光标所在处的字符：[常用]
R：取代字符直到按Esc为止。
（6）复原（undo）上一个指令
u：假如您误操作一个指令，可以马上按u，回复到上一个操作。[超常用]
.: .可以重复执行上一次的指令。
（7）更改
cw：更改光标所在处的字到字尾$处。
c#w：例如，c3w代表更改3个字。
（8）跳至指定行
Ctrl+G：列出光标所在行的行号。
#G：例如，15G，表示移动光标至文章的第15行行首。[常用]
Last line mode下指令简介
读者您要使用Last line mode之前，请记得先按Esc键确定您已经处于Command mode下后，再按冒号“：”或“/”或“？”
三键的其中一键进入Last line mode。
1．列出行号
set nu: 输入“set nu”后，会在文章的每一行前面列出行号。
2．跳到文章的某一行
#：井号代表一个数字，在Last line mode提示符号“：”前输入数字，再按Enter就会跳到该行了，
如：15[Enter]就会跳到文章的第15行。[常用]
3．寻找字符串
/关键字：先按/，再输入您想寻找的字，如果第一次找的关键字不是您相尽可能的，可以一直按n会往下寻找到您要的关键字为止。
？关键字：先按？，再输入您想寻找的字，如果第一次找的关键字不是您想要的，可以按n会往前寻找到您要的关键字为止。
4．取代字符串
1，$s/string/replae/g：在last line mode输入“1，$s/string/replace/g”会将全文的string字符串取代为replace字符串，
其中1,$s就是指搜寻区间为文章从头至尾的意思，g则是表示全部取代不必确认。
%s/string/replace/c：同样会将全文的string字符串取代为replace字符串，和上面指令不同的地方是，%s和1,$s是相同的功能，
c则是表示要替代之前必须再次确认是否取代。
1,20s/string/replace/g：将1至20行间的string替代为relpace字符串。
5．存文件
w：在last line mode提示符号“：”前按w即可将文件存起来。[超常用]
#，# w filename：如果您想摘取文章的某一段，存成另一个文件，可用这个指令#代表行号，例如30,50 w nice，
将您正在编辑文章的第30~50行存成nice这个文件。
6．离开
q：按q就离开，有时如果无法离开vi，可搭配“！：强置离开vi，如“q！”
qw：一般建议离开时，搭配w一起使用，如此离开时还可存文件。[常用]
[http://chhff.javaeye.com/blog/404914](http://chhff.javaeye.com/blog/404914)
**对于第一次用vi，有几点注意要提醒一下：**
1、 用vi打开文件后，是处于「命令行模式（command mode）」，您要切换到「插入模式（Insert mode）」才能够输入文字。切换方法：在「命令行模式（command mode）」下按一下字母「i」就可以进入「插入模式（Insert mode）」，这时候你就可以开始输入文字了。
2、编辑好后，需从插入模式切换为命令行模式才能对文件进行保存，切换方法：按「ESC」键。
3、保存并退出文件：在命令模式下输入:wq即可！（别忘了wq前面的:）  
