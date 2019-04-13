
# 华为C语言编程规范—质量保证 -  芝麻软件工作室的专栏 - CSDN博客


2016年06月17日 07:26:37[seven-soft](https://me.csdn.net/softn)阅读数：329


8-1：在软件设计过程中构筑软件质量。
8-2：代码质量保证优先原则
（1）正确性，指程序要实现设计要求的功能。
（2）稳定性、安全性，指程序稳定、可靠、安全。
（3）可测试性，指程序要具有良好的可测试性。
（4）规范/可读性，指程序书写风格、命名规则等要符合规范。
（5）全局效率，指软件系统的整体效率。
（6）局部效率，指某个模块/子模块/函数的本身效率。
（7）个人表达方式/个人方便性，指个人编程习惯。
8-3：只引用属于自己的存贮空间。
说明：若模块封装的较好，那么一般不会发生非法引用他人的空间。
8-4：防止引用已经释放的内存空间。
说明：在实际编程过程中，稍不留心就会出现在一个模块中释放了某个内存块（如C 语言指针），而另一模块在随后的某个时刻又使用了它。要防止这种情况发生。
8-5：过程/函数中分配的内存，在过程/函数退出之前要释放。
8-6：过程/函数中申请的（为打开文件而使用的）文件句柄，在过程/函数退出之前要关闭。
说明：分配的内存不释放以及文件句柄不关闭，是较常见的错误，而且稍不注意就有可能发生。这类错误往往会引起很严重后果，且难以定位。
示例：下函数在退出之前，没有把分配的内存释放。
typedef unsigned char BYTE;
int example_fun( BYTE gt_len, BYTE *gt_code )
{
BYTE *gt_buf;
gt_buf = (BYTE *) malloc (MAX_GT_LENGTH);
... //program code, include check gt_buf if or not NULL.
/* global title length error */
if (gt_len > MAX_GT_LENGTH)
{
return GT_LENGTH_ERROR; // 忘了释放gt_buf
}
... // other program code
}
应改为如下。
int example_fun( BYTE gt_len, BYTE *gt_code )
{
BYTE *gt_buf;
gt_buf = (BYTE * ) malloc ( MAX_GT_LENGTH );
... // program code, include check gt_buf if or not NULL.
/* global title length error */
if (gt_len > MAX_GT_LENGTH)
{
free( gt_buf ); // 退出之前释放gt_buf
return GT_LENGTH_ERROR;
}
... // other program code
}
8-7：防止内存操作越界。
说明：内存操作主要是指对数组、指针、内存地址等的操作。内存操作越界是软件系统主要错误之一，后果往往非常严重，所以当我们进行这些操作时一定要仔细小心。
示例：假设某软件系统最多可由10 个用户同时使用，用户号为1-10，那么如下程序存在问题。
\#define MAX_USR_NUM 10
unsigned char usr_login_flg[MAX_USR_NUM]= "";
void set_usr_login_flg( unsigned char usr_no )
{
if (!usr_login_flg[usr_no])
{
usr_login_flg[usr_no]= TRUE;
}
}
当usr_no 为10 时，将使用usr_login_flg 越界。可采用如下方式解决。
void set_usr_login_flg( unsigned char usr_no )
{
if (!usr_login_flg[usr_no - 1])
{
usr_login_flg[usr_no - 1]= TRUE;
}
}
8-8：认真处理程序所能遇到的各种出错情况。
8-9：系统运行之初，要初始化有关变量及运行环境，防止未经初始化的变量被引用。
8-10：系统运行之初，要对加载到系统中的数据进行一致性检查。
说明：使用不一致的数据，容易使系统进入混乱状态和不可知状态。
8-11：严禁随意更改其它模块或系统的有关设置和配置。
说明：编程时，不能随心所欲地更改不属于自己模块的有关设置如常量、数组的大小等。
8-12：不能随意改变与其它模块的接口。
8-13：充分了解系统的接口之后，再使用系统提供的功能。
示例：在B 型机的各模块与操作系统的接口函数中，有一个要由各模块负责编写的初始化过程，此过程在软件系统加载完成后，由操作系统发送的初始化消息来调度。因此就涉及到初始化消息的类型与消息发送的顺序问题，特别是消息顺序，若没搞清楚就开始编程，很容易引起严重后果。以下示例引自B
 型曾出现过的实际代码，其中使用了FID_FETCH_DATA与FID_INITIAL 初始化消息类型，注意B 型机的系统是在FID_FETCH_DATA 之前发送FID_INITIAL 的。
MID alarm_module_list[MAX_ALARM_MID];
int FAR SYS_ALARM_proc( FID function_id, int handle )
{
_UI i, j;
switch ( function_id )
{
... // program code
case FID_INITAIL:
for (i = 0; i < MAX_ALARM_MID; i++)
{
if (alarm_module_list[i]== BAM_MODULE // **）
|| (alarm_module_list[i]== LOCAL_MODULE)
{
for (j = 0; j < ALARM_CLASS_SUM; j++)
{
FAR_MALLOC( ... );
}
}
}
... // program code
break;
case FID_FETCH_DATA:
... // program code
Get_Alarm_Module( ); // 初始化alarm_module_list
break;
... // program code
}
}
由于FID_INITIAL 是在FID_FETCH_DATA 之前执行的，而初始化alarm_module_list 是在FID_FETCH_DATA 中进行的，故在FID_INITIAL 中（**）处引用alarm_module_list
 变量时，它还没有被初始化。这是个严重错误。应如下改正：要么把Get_Alarm_Module 函数放在FID_INITIAL 中（**）之前；要么就必须考虑（**）处的判断语句是否可以用（不使用alarm_module_list 变量的）其它方式替代，或者是否可以取消此判断语句。
8-14：编程时，要防止差1 错误。
说明：此类错误一般是由于把“<=”误写成“<”或“>=”误写成“>”等造成的，由此引起的后果，很多情况下是很严重的，所以编程时，一定要在这些地方小心。当编完程序后，应对这些操作符进行彻底检查。
8-15：要时刻注意易混淆的操作符。当编完程序后，应从头至尾检查一遍这些操作符，以防止拼写错误。
说明：形式相近的操作符最容易引起误用，如C/C++中的“=”与“==”、“|”与“||”、“&”与“&&”等，若拼写错了，编译器不一定能够检查出来。
示例：如把“&”写成“&&”，或反之。
ret_flg = (pmsg->ret_flg & RETURN_MASK);
被写为：
ret_flg = (pmsg->ret_flg && RETURN_MASK);
rpt_flg = (VALID_TASK_NO( taskno ) && DATA_NOT_ZERO( stat_data ));
被写为：
rpt_flg = (VALID_TASK_NO( taskno ) & DATA_NOT_ZERO( stat_data ));
8-16：有可能的话，if 语句尽量加上else 分支，对没有else 分支的语句要小心对待；switch语句必须有default 分支。
8-17：Unix 下，多线程的中的子线程退出必需采用主动退出方式，即子线程应return 出口。
8-18：不要滥用goto 语句。
说明：goto 语句会破坏程序的结构性，所以除非确实需要，最好不使用goto 语句。
8-19：精心地构造、划分子模块，并按“接口”部分及“内核”部分合理地组织子模块，以提高“内核”部分的可移植性和可重用性。
说明：对不同产品中的某个功能相同的模块，若能做到其内核部分完全或基本一致，那么无论对产品的测试、维护，还是对以后产品的升级都会有很大帮助。
8-20：精心构造算法，并对其性能、效率进行测试。
8-21：对较关键的算法最好使用其它算法来确认。
8-22：时刻注意表达式是否会上溢、下溢。
示例：如下程序将造成变量下溢。
unsigned char size ;
while (size-- >= 0) // 将出现下溢
{
... // program code
}
当size 等于0 时，再减1 不会小于0，而是0xFF，故程序是一个死循环。应如下修改。
char size; // 从unsigned char 改为char
while (size-- >= 0)
{
... // program code
}
8-23：使用变量时要注意其边界值的情况。
示例：如C 语言中字符型变量，有效值范围为-128 到127。故以下表达式的计算存在一定风险。
char chr = 127;
int sum = 200;
chr += 1; // 127 为chr 的边界值，再加1 将使chr 上溢到-128，而不是128。
sum += chr; // 故sum 的结果不是328，而是72。
若chr 与sum 为同一种类型，或表达式按如下方式书写，可能会好些。
sum = sum + chr + 1;
8-24：留心程序机器码大小（如指令空间大小、数据空间大小、堆栈空间大小等）是否超出系统有关限制。
8-25：为用户提供良好的接口界面，使用户能较充分地了解系统内部运行状态及有关系统出错情况。
8-26：系统应具有一定的容错能力，对一些错误事件（如用户误操作等）能进行自动补救。
8-27：对一些具有危险性的操作代码（如写硬盘、删数据等）要仔细考虑，防止对数据、硬件等的安全构成危害，以提高系统的安全性。
8-28：使用第三方提供的软件开发工具包或控件时，要注意以下几点：
（1）充分了解应用接口、使用环境及使用时注意事项。
（2）不能过分相信其正确性。
（3）除非必要，不要使用不熟悉的第三方工具包与控件。
说明：使用工具包与控件，可加快程序开发速度，节省时间，但使用之前一定对它有较充分的了解，同时第三方工具包与控件也有可能存在问题。
8-29：资源文件（多语言版本支持），如果资源是对语言敏感的，应让该资源与源代码文件脱离，具体方法有下面几种：使用单独的资源文件、DLL 文件或其它单独的描述文件（如数据库格式）

