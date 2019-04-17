# Linux Shell 编程语法 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年01月04日 14:04:47[boonya](https://me.csdn.net/boonya)阅读数：525标签：[linux																[shell](https://so.csdn.net/so/search/s.do?q=shell&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[Linux](https://blog.csdn.net/boonya/article/category/1740759)









**编写代码**

在计划好要程序干什么以及如何使用程序的使用变得更加简单易学之后，程序员就必须以计算机能够理解的方式把他的想法记录下来，告诉计算机他想干什么。这就是要用到的所谓的代码。一般按照如下顺序组织自己的思维：

1. 决定让计算机做哪些任务

2. 确定计算机执行任务的顺序

3. 从shell脚本语言中选择适当的命令来执行那些任务

4. 用linux文本编辑器编辑一个shell脚本，并按任务的执行顺序来组织命令

**关于编程的几个术语**

输送给计算机的单一一条指令称之为命令——command

集合在一起的多个命令称之为代码——code

为了完成某项任务而组织在一起的所有指令统称为程序——program

**Linux中bash shell语音保留的关键字： **
|**case**|**elif**|**let**|
|----|----|----|
|**clear**|**else**|**read**|
|**continue**|**if**|**then**|
|**echo**|**for in**|**while**|

**变量**

什么是数据？

程序从预付那儿获得的信息成为数据——data。一个好的程序遵从了下面三个步骤：

1. 从硬盘或文件中获得数据

2. 对数据进行某种适当的处理

3. 把数据显示在屏幕上火存放在一个文件中

字符串——string

字符串其实就是多个字母或字符的集合。一个字符即可能是一个连字符，也可能是在键盘上所能发现的任何东西。字符串也可能经由单一字母或字符组成。一个完整的句子可能是一个字符串。事实上，任何字母、空格、甚至是数字的组合，都可以看成是一个字符串。

数——number

一个数可由任何数字组成，如金钱数、某人的年龄或者身高、体重等。数既可以是负数也可以是正数、整数、分数、小数。在复杂一点，数也可以是字符。数的这种双重身份可能让我们感到疑惑，但是它无法欺骗计算机。如果告诉计算机是某个地址或电话号码的一部分，那么它就会把该数当做一个字母或字符。但是，如果让计算机把某数加起来，那么计算机就会把它当做数而不是字符。

**理解什么是变量和值**

计算机是知道如何区分用户输入的数据和字符串，但对人来说可能就像“迷×魂×汤”。就对我们读者而言，很容易把55-55-55当做是某个人的保险号，而计算机仅把它当做一个数或一个字符串。

当程序要用那个保险号时，它还必须告诉计算机：“现在是用保险号的时候了，你汉子道它放在哪儿了，对吗？它就放在变量名为SSS下了”

所谓的变量——variable，其实就是计算机中某一特定数的值（如保险号）的地方。而输给这个变量名sss的保险号就是一个值。

变量的分类

**Linux使用下列两种分类：**

局部变量：有程序员自己建立，且仅供程序员所设计的程序使用。

环境变量：由程序员或他人建立，程序员和他人的程序都可以使用。

Linux条件控制语句

我们一生都在决定。如，你想到哪儿去旅游，是去美国，日本还是法国？

决定是其实是一种明智的选择。但是我们得把它们建立在符合某种条件基础上，如我们有足够的资金去旅游吗？如果条件不成立，即使是最让人神往的选择也只是一种空想。

**条件**

条件是一种必须存在或不存在的状态。如果存在，则条件为真，否则为假。

Linux中，条件存在有如下两种表达形式：

Ø 单一变量

Ø 一个表达式

比如说，把所有的钱放进钱包（wallet）里。Wallet被称为变量，钱包里的钱被称为变量的值。是否去美国夏威夷，得要看自己的钱包中的钱是否足够。如果不够，则去不了。因此，钱包这个变量的值是零，这意味着去美国夏威夷旅游的钱不够，条件为假。

又比如说，钱包里有许多许钱。但对于这次旅行来说这些钱足够吗？这得问问我们自己了，看看钱包里的钱是否大于或等于$1000（旅行所需的money）。这就是一个表达式，它将导致真或假两种结果之一。

**用变量表示的条件**

一个变量的值要么是真要么是假。这通常用非零表示真，零表示假。下面就介绍确定一个变量的值是真还是假的两种方法。第一种方法显示检查变量的值，如下例所示：

If [$HaveEnoughMoney –eq 1]

程序将用等号算符（- eq）来确定变量的值是真还是假。

如果想判断一个条件是否是真，也可以让程序来检测变量的值是否是假，如下所示：

If [$HaveEnoughMoney – ne 0]

不等于算符（- ne）做出的是一种与逻辑相反的判断。如果用- eq算符，那么程序将检测该变量是否是真。相反，如果用- ne算符，那么程序将检测该变量是否不真，即假。

**用表达式表示的条件**

条件也可以说一个表达式，它要求程序对信息执行的某种操作。譬如，要求程序检测一个变量（如你的银行存款，O(∩_∩)O）的值是否大于某一个特定数。在这种情况下，一个表达式的结果也只可能有两种：真或假，且只能是其中之一，如下例所示：

If [$BankBalance –gt 3000] 
|**-eq**|**等于**|**-gt**|**大于**|**-lt**|**小于**|
|----|----|----|----|----|----|
|**-ne**|**不等于**|**-ge**|**大于或等于**|**-le**|**小于或等于**|

程序检测变量BankBalance的值，看它是否大于3000，而这个表达式只有两种可能性：真或假。如果存款大于3000，则为真；存款小于3000，则为假。

If语句

当需要检测一个条件是真还是假的时候，就可以使用if语句。If语句仅仅是告诉程序：“如果条件为真，就执行这些指令，否则跳过这些指令。”

If语法的规则如下：

If {Condition}

Then

Instrution

fi

一般说来，如果Condition存在（即条件Condition为真），程序则执行项目的Instruction。

下面列举两个例子来说明if语句的用法。第一个例子是让程序判断一个盒子是否已经装满了，条件是盒子最多只能装下100个小甜饼。

if [$Quantity –eq 100]

then

echo “The box is full”

fi

这个语句让程序检测一下变量Quantity的值，看它是否为100.如果是，则在屏幕上显示：

The box is full.

否则，就跳过这条语句并移到fi的下一条语句。注意，在这里，fi是一个保留字，它有特殊的含义。（fi表示if语句的结束）

另一个例子，要求程序检测盒子是否已满，及是否还有其他的盒子可以获得。当且仅当这两个条件同时是真时，程序才会报告无法在装甜饼了，如下所示：

if [$Quantity –eq 100] && {$AvailableBoxes –it 1}

then

echo “The box is full and you have no more boxes”

fi

这条语句要求程序检测一下变量Quantity的值，看它是否100，及变量AvailableBoxes的值是否小于1.如果这两个条件都为真，则显示信息：

The box is full and you have no more boxes”

如果变量Quantity的值不是100，且分子也有剩余，程序则跳过这条语句，移到保留字fi的下一条语句。

**if else语句**

if语句可以使程序一句一个条件来做出某种判断，如果天哦见是真则执行一组规定的指令。

但是，在使用if语句时也存在着一个问题，即条件是假时，可能还需要为程序提供另外一组指令。当然，也可以再用一条if语句来解决这个问题，即在条件为假时让程序执行另一组指令。但是，有一种方法比这更简单，这就是if else语句。if else语句的语法规则是：

if [Condition]

then

Instruction1

else

Instruction2

fi

这条语句表示，如果Condition（条件）为真，程序则执行第一组指令（IInstruction1）；如果Condition（条件）为假，则程序执行第二组指令（Instruction2）。

现在进行一个小小的测验，看我们是否知道如何修改下面的语句，以便让程序在屏幕上显示信息：

The box is not full?

如下所示：

if [$Quantity –eq 100]

then

echo “The box is full”

fi

可以用如下两种方法来修改这条语句，这要取决于我们如何来表达那个条件。如果想让条件不变，则可以按如下修改：

if [$Quantity –eq 100]

then

echo “The box is full”

else

echo “The box is not full”

不过，也可以用条件$Quantity –ne 100来修改，如：

if [$Qurantity –ne 100]

then

echo “The box is not full”

else

echo “The box is full”

fi

if else语句

担心！如果条件为假时，程序将无条件地执行第二组指令。有时，我们并不希望这要。相反，希望程序在执行第二组指令前，再做一次判断。

为了达到上述目的，可以使用if elif语句来让计算机再做一次判断。着看起来似乎有点让人混淆不清，但在习惯了之后是相当容易理解的。if elif语句的语法规则如下：

if [Condition1]

then

Instruction1

elif

Instruction2

fi

在这条语句中，如果条件Condition1是真，程序则执行第一组指令Instruction1；否则，程序在判断条件Condition2是否是真。如果条件不为真，程序则执行第二组指令Instruction2；如果Condition2是假，则跳过第二组指令。

无论什么时候使用if语句，程序至少执行一组指令。但是，如果使用的是if elif语句，程序将有可能跳过这两组指令，如下例所示：

if [$$Quantity –eq 100]

then

echo “The box is full”

elif [$Quantity –eq 95]

then

echo “You can add 5 cookies to the box”

fi

于是，当Quantity的值是95时，会有什么反应？首先，程序判断第一个条件表达式的值发现表达式$Quantity –eq 100的值是假。

然后，程序确定第二个条件表达式的值，发现表达式$Quantity –eq 95是真，因此，程序将显示：

You can add 5 cookies to the box

但是，当Quantity的值是94的时候，会有什么结果呢？

同上所述，程序首先检测第一个条件表达式$Quantity –eq 100，并确定该表达式的值是假，于是程序忽略第一组指令；然后，程序检测第二个条件表达式Quantity –eq 95，发现它也是假，因此再忽略第二组指令。

这时，程序就达到了if else语句体的最后。由于没有一个条件是真，所以程序不执行任何指令。

**用if else语句进行多重的选择**

在程序中使用多重if elif语句进行条件检测，程序能处理多种可能。多重if elif语句结构规则如下：

if [Condition1]

then

Instruction1

elif [Condition2]

then

Instruction2

elif [Condition3]

then

Instruction3

fi

在这条语句中，如果Condition1是真，程序执行第一组指令Instruction1；否则，检测条件Condition2是否为真。如果条件Condition2是真，则执行第二组指令Instruction2；否则，检测Condition3是否是真。若条件Condition3为真，则执行第三组指令Instruction3；否则，跳过第三组指令Instruction3。

注意，在程序中，if elif语句嵌套得越多，就越 眼花缭乱。

**保证Linux至少执行一组指令**

想象一下，如果我们写了一个结果庞大的if elif语句，却发现它什么也干不了，回事什么感觉？O(∩_∩)O哈哈~，在实际编程中确实有这样的事儿发生。不过，要是在if elif语句的最后发一条elif fi语句，就可以避免这样的“灾难”，如下例所示：

if [Condition1]

then

Instruction1

elif

Instruction2

elif

then

Instruction3

elif

Instruction4

fi

在这条语句中，如果Condition1是真，程序执行第一组指令Instruction1；否则，检测Condition2是否为真。如果条件Condition2是真，则执行第二组指令Instruction2；否则，检测Condition3是否是真。若条件Condition3是真，则执行第三组指令Instruction3；否则，执行第四组指令Instruction4.

**case语句**

你是否碰见过这种情况，让计算机在众多可能的情况下做一个决定？比如说，让计算机为公司五个办公室中的每一个做一次特色服务。要完成这项任务，程序可能要用一大堆if elif语句，如下所示：

if [$region –eq “1”]

then

echo “Hello, Bob”

elif [$region –eq “2”]

then

echo “hello, Mary”

elif [$region –eq “3”]

then

echo “hello, Joan”

elif [$region –eq “4”]

then

echo “hello, Mike”

elif [$region –eq “5”]

then

echo “hello, Tom”

fi

这个程序读起来可能有点头疼——尽管计算机读它很容易。但是，再想想，如果办公室增加到20个，处理20条if语句，又会如何呢！

case语句

假设要设计这样一个程序，对每一个办公室的用户程序显示不同的信息。当然，读者完全可以像刚开头说的那样设计这个程序：给给每一个办公室分配一个号码，每个号码对应一条特殊的信息。

case语句的功能是：把保留字case右边的值和闭括号“）”左边的值比较，其语法规则如下：

case VariableName in

value1)

Instruction

;;

value2)

Instruction

;;

esac

从上面可以看出，case语句以保留字case开头，以esac结尾。它首先检查变量VariableName的值，如果它与value1相等则执行第一组指令集，如果它与value2相等则执行第二组指令集以此类推。同时，在每组指令集的最后用两个分号（;;）。

case $region in

1)

echo “Hello, Bob”

;;

2)

echo “Hello, Mary”

;;

3)

echo “Hello, Joan”

;;

4)

echo “Hello, Mike”

;;

5)

echo “Hello, Tom”

;;

esac

这段代码很容易阅读，因为它去掉了重复的elif。当然，只要实际需要还可以加入更多可能的值。实际上case语句使得程序像流水线一样，让人很容易看懂。

但是，在使用case语句的时候，必须记住以下几点：

Ø 程序要匹配的值必须包含一个或多个字符

Ø 可以用一个变量或值作为程序要匹配的值

Ø 一个变量或值的右边必须以闭括号“）”结尾

case语句中的默认情况

在case语句中，如果用户没有给程序提供所需要的匹配值，那么程序就找不到它所要匹配的对象。如果是由程序设计本人提供程序所必须的匹配值，这种情况有时就不会发生。但是，若有他人来提供，问题可能就要来了。

假设又增开了一件办公室，而我们有没有时间来更新程序，让它能够处理这间新办公室。这时，程序由于找不到任何匹配值，就将那件新办公室忽略。

不过，我们完全可以使用保留字符（*）来为此留一条后路，即提供一种默认情况，让程序在此执行一些必要的操作。请看如下示例：

case $region in

1)

echo “Hello, Bob”

;;

2)

echo “Hello, Mary”

;;

3)

echo “Hello, Joan”

;;

4)

echo “Hello, Mike”

;;

5)

echo “Hello, Tom”

;;

*)

echo “Sorry, your region is not on my list”

esac

这段代码。如果$office的值等于某一间办公室的号码，那么它就显示某条适当的问候语；如果case语句中没有与$office相匹配的值，就执行最后一条指令，显示信息：

Sorry, your region is not on my list

**多层嵌套控制结构**

收到过装在一个大盒子中礼物吗？当你迅速打开的时候，里面还有一个稍小的盒子。哎打开这个小盒子，发现里面又有一个小盒子。。。。。。直至找到礼物。

在编程的过程中，盒子中又有盒子的现象称为嵌套（nesting）。如果把多个if语句或case语句组合到一起，就是嵌套了。

if嵌套结构

通常情况下，一个if语句要包含一条或多条指令，如下例所示：

if [$office –eq “1”]

then

echo “hello, Bob”

else

echo “Sorry, your region is not on my first”

fi

下面，再用case语句代替if语句重写上一个例子：

case $office in

“1”)

echo “Hello, Bob”

;;

*)

echo “sorry, your region is not on my list”

esac

当然，也可以不在if或case语句中放入一大堆命令，而在其中再放入其他的if和case语句。下面是一个使用if语句的例子：

if [$office –eq “1”]

then

if [$RegionalMgr –eq “Bob”]

then

echo “Hello, Bob”

fi

此程序首先检查变量$office的值。如果它是1，则再检查变量$RegionalMgr的值。如果它的值是Bob，这显示欢迎Bob的问候语。

**case嵌套结构**

case语句同样可以嵌套（与上例的方法相同），请看下面的例子：

case $office in 

“1”)

case $RegionalMgr in

“Bob”)

echo “Hello, Bob”

;;

esac

;;

esac

**while循环**

while循环的执行过程，如果站在屋檐下躲雨的人们，经常需要检查雨是否还在下一样。如果雨还在下，他们就得继续呆着，否则就可以开车走。在这里，雨就是一个条件，决定着他们是否能走。

我们也可以写出一个程序，让用户在继续运行本程序之前先不断回答一个问题，一直到问题回答正确为止。用户没回答一次，程序就检查一次，看是否与正确答案相同。这种检查类似于站在屋檐下的人看天空是否还在下雨一样。如果程序发现了用户的答案不对，就告诉他回答不正确，并返回到while循环的顶部，等待下一次回答。

while循环的一般格式如下：

while [Condition]

do

Instruction

done

下面，我们将学习while循环的用法，以便程序仅当满足某一特定条件时——譬如用户从键盘上输入的回答正确的时，程序才执行某部分指令。

在while循环中，必须放入一个倒置结果为真或假的变量或表达式，而在while循环体中则可以放入任意多的指令。

确定是否使用while循环

在编写程序的时候，我们一般会列出计算机需要运行的全部指令，其中的一部分指令需要重复执行多次。这时，往往不是把那些指令在重写，而是把它们放在一个称之为while的循环之中。

如果需要重复执行某一组指令，且条件是真，就可以使用while循环。例如，设计一个提示用户输入密码的程序，就可以把那些指令放入一个while循环之中，直至用户输入的密码正确才终止重复运行。

不过，如果你不想程序连续地重复执行某一部分指令，就不要使用while循环。比如，当用户在使用程序出现错误的时候，我们总想让计算机把统一错误信息在屏幕上显示出来。显然显示该出错信息的指令在程序中重复执行，但仅当错误发生时，指令才会重复执行。若把这些指令放入一个while循环中，必将导致程序连续不断地显示该信息。而要解决这个问题的最好办法就是把这些指令放入一个子程序中，仅当需要显示该信息的时候，程序才调用该子程序。

while循环在Linux代码中的使用

当程序遇到一个while循环时，首先检测它的条件，询问它“这个东西是真还是假？”仅当条件为真时，程序才会读入while循环体中的指令。

请看下面的演示while循环的例子：

declare raining=”1”

while [“$raining” –eq “1”]

do

echo “Still raining”

done

关于这个while循环中的指令具体说明如下：

declare声明建立一个名为raining的字符串变量，并将它赋值为1。这个值表面是一个真值。

关键字while指示程序检测$raining的值是否仍然是1。如果是，则意味着雨还在下。如果雨还在下，echo将信息Still raining显示在屏幕上。

done是while循环的结束。

当然，为使这个程序有点实际意义，还需要在这个例子中增加一些指令。例如，在上面的例子中，由于$raining的值从不改变，程序总是显示该信息。提醒读者注意，上述程序是一个死循环，因此while循环的条件永远都是真。

为了避免死循环的发生，需要在程序中增加一些指令，以检查雨是否还在下。譬如，在while循环设置一些指令，当雨停止时将$raining的值为0。

**for in循环**

假设你中了大奖，想写信邀请朋友来参加一个庆祝会。你可能会写一个程序来打印这些信件，结果却发现程序没运行一次，都要求输入一个朋友的名字。

如果使用for in循环，就可以避免这类问题。因为for in循环可以让你把所有朋友的名字都存放一个列表中，然后让程序一句每个人的名字来打印这些邀请信。这有点类似于邮件合并。因此，当想让程序在条件为真时执行一系列的指令，就是用while循环。它会一直运行到程序中有一条指令条件修改为假时终止。

但是，如果知道循环体中指令执行的具体次数，那么久使用for in循环。

for in循环一般格式如下：

for VariableName in wordlist

do

Instruction

done

wordlist是一个将赋给变量（VariableName）的字符串值的列表。循环体中的指令可以使用该变量。

在循环体中究竟放入多少指令，取决于程序的实际需要。而你所遇到的最大挑战，就是是否掌握了那些程序指令。

什么是否使用for in循环

没有什么准则可以表明什么时候使用for in循环，这需要自己分析。

不过，下面两种情况常常需要使用for in循环：

Ø 需要对多个字符值做同样操作

Ø 需要按照某一确定的次序使用字符串值系列

例如，用一个程序给5个雇员加薪水，必然会遵循下列步骤：

1. 在工资表中查找一个雇员的工资

2. 计算新的薪水

3. 将新的薪水存入工资表

可以先把每个雇员的名字放在一个与for in循环相联系的列表中。然后，按照雇员的名字顺序，让程序查找他们的记录，进行一些必要的数学运算，及把记录重新放入工资文件中。

如何使用for in循环

如果你想在朋友前面炫耀下一番新学习到的编程知识，不妨写一个程序，当他们聚在计算机面前显示，分别显示对他们的问候语。要写这个程序，有许多不同的方法。

请先看下面的示例：

echo “Hello, Mary”

echo “Hello, Joe”

echo “Hello, Sue”

这段程序仅仅是将同一指令重复三次，变换一下其中的名字。

再用另一种写法重写上面的例子，请看：

let counter=1

while [“$counter” –it 5]

do

if [“$counter” –eq 1]

then

echo “hello, Mary”

fi

if [“$counter” –eq 2]

then

echo “hello, Joe”

fi

if [“$counter” –eq 3]

then

echo “hello, Sue”

fi

done

现将上面程序有关指令说明如下：

while循环中的指令每执行一次，计算机就检查counter的值一次。

显示对某一个人的欢迎信息取决于counter的值。

while循环体中的最后一条指令（let counter=$counter + 1）是给$counter的值加1.并将新值重新赋给counter。这条指令能让计算机显示对所有人的信息。

在所有这些指令都执行完成以后，计算机检测counter的值看它是否小于5。若小于5，则在执行一次。若大于5或等于5，循环中止。

上一个程序共用了16行代码，再看下面使用for in循环的例子，它仅用了3行就取得了同样的结果。

请看下面for in循环写成的例子：

for friend in Mary Joe Sue

do

echo “hello, $friend”

done

在for in循环的第一行，在for friend in后面紧跟着3个人的人名，这样就建立了一个单词表，以后就可以把这些名字当做值赋给变量friend。

注意，在$friend和句号之间有一个空格，如果没有留这个空格，程序将根据friend。（有句号）而不是friend（无句号）来寻找变量的。

这个程序的运行过程如下：

1. 计算机从单词表中取出第一个值，并将它赋给变量friend。

2. 然后，计算机将欢迎词显示在屏幕上。

3. 计算机返回，再取第二个值将它赋给friend。

这个程序一直运行到单词表中的值使用完，也即问候完了所有人之后，for in循环中止。

因此，通过for in循环的使用，可以把用while循环写的程序代码从16行缩减到3行，而且程序读起来更容易。

使用单词表

赋给变量的那些值统称为单词表（wordlist）。它是一个关于字符串值（字符和数字的组合）的列表，我们可以按顺序从中一次取一个值赋给变量。

关于单词表有以下几点值得注意：

Ø 赋给单词表的值必须是一个字符串的值

Ø 每一个值之间必须用空格隔开

Ø 如果空格也是字符串的一部分，则需要在字符串的两边加上引号

为了说明含空格的字符串的用法，请看下面的例子：

for friend in “Mary” “Joe” “Sue”

do

echo “hello, $friend”

done

**循环嵌套与快速退出**

还记得什么时候妈妈叫你清洁玩具吗？抱怨一阵子之后，你不得不抓起玩具把它们塞进玩具箱内。你一遍一遍地重复着这个动作，可以由于每个玩具都是由许多积木平成的，再把它放进玩具箱之前，需要把积木放在各自的包装盒内。

在本例中，拿玩具相当于外层循环（outside loop），拿积木相当于内层循环（indside loop）。在一个循环之中又嵌入另一循环形成一个总的循环系列，称之为嵌套循环（nested loop）。嵌套循环的使用，使得程序组织起来更容易，尽管读起来有点费劲。

使用嵌套循环

嵌套循环表示的是一个外层虚幻每执行一次，内需和就执行许多次。请看下面包含一个嵌套循环的例子：

let flag=1

while [“$flah” –lt 4]

do

for friend in Bob Mary Sue

echo “hello, $friend”

done

let flag=”$flag + 1”

总而言之，屏幕上将显示欢迎Bob、Mary、Sue的信息三次。这个程序包含两个循环：while循环和for in循环。while循环（外层循环）共循环三次，而内层循环即嵌套循环的for in循环，在while循环每循环一次，就很不现实对每个人的问候语。

现将程序的有关指令按顺序说明如下：

let flag=1定义了一个数字变量，并给它赋值为1。

while [“$flag” –lt 4]检测$flag的值，若它的值为1，程序则执行while循环体内的指令。

for friend in Bob Mary Sue是嵌套循环的起始部分，它把单词表中的每一个值（Bob Mary Sue）赋给变量friend。

echo “hello, $friend”显示欢迎变量$friend的值的问候语

第一个done表示它是for in循环的结尾。

let flag=”$flag + 1”表示给变量$flag加1并重新赋值给变量flag。

第二个done表示while循环的结尾。

Refference From :[http://www.cnblogs.com/fhefh/archive/2011/04/13/2014967.html](http://www.cnblogs.com/fhefh/archive/2011/04/13/2014967.html)






