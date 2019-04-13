
# 华为C语言编程规范—注释 -  芝麻软件工作室的专栏 - CSDN博客


2016年06月16日 17:35:40[seven-soft](https://me.csdn.net/softn)阅读数：2796


**2-1：一般情况下，源程序有效注释量必须在20％以上。**
说明：注释的原则是有助于对程序的阅读理解，在该加的地方都加了，注释不宜太多也不能太少，注释语言必须准确、易懂、简洁。
**2-2：文件头部应进行注释，注释必须列出：版权说明、版本号、生成日期、作者、内容、功能、修改日志等。**
示例：下面这段头文件的头注释比较标准，当然，并不局限于此格式，但上述信息建议要包含在内。
/*****************************************************************************
Copyright: 1988-1999, Huawei Tech. Co., Ltd.
File name: 文件名
Description: 用于详细说明此程序文件完成的主要功能，与其他模块或函数的接口，输出值、取值范围、含义及参数间的控制、顺序、独立或依赖等关系
Author: 作者
Version: 版本
Date: 完成日期
History: 修改历史记录列表， 每条修改记录应包括修改日期、修改者及修改内容简述。
*****************************************************************************/
**2-3：函数头部应进行注释，列出：函数的目的/功能、输入参数、输出参数、返回值、调用关系（函数、表）等。**
示例：下面这段函数的注释比较标准，当然，并不局限于此格式，但上述信息建议要包含在内。
/*************************************************
Function: // 函数名称
Description: // 函数功能、性能等的描述
Calls: // 被本函数调用的函数清单
Called By: // 调用本函数的函数清单
Table Accessed: // 被访问的表（此项仅对于牵扯到数据库操作的程序）
Table Updated: // 被修改的表（此项仅对于牵扯到数据库操作的程序）
Input: // 输入参数说明，包括每个参数的作// 用、取值说明及参数间关系。
Output: // 对输出参数的说明。
Return: // 函数返回值的说明
Others: // 其它说明
*************************************************/
**2-4：边写代码边注释，修改代码同时修改相应的注释，以保证注释与代码的一致性。**不再有用的注释要删除。
**2-5：注释的内容要清楚、明了，含义准确，防止注释二义性。**说明：错误的注释不但无益反而有害。
**2-6：注释应与其描述的代码相近，对代码的注释应放在其上方或右方（对单条语句的注释）相邻位置，不可放在下面，如放于上方则需与其上面的代码用空行隔开。**
示例：如下例子不符合规范。
例1：
/* get replicate sub system index and net indicator */
repssn_ind = ssn_data[index].repssn_index;
repssn_ni = ssn_data[index].ni;
例2：
repssn_ind = ssn_data[index].repssn_index;
repssn_ni = ssn_data[index].ni;
/* get replicate sub system index and net indicator */
应如下书写
/* get replicate sub system index and net indicator */
repssn_ind = ssn_data[index].repssn_index;
repssn_ni = ssn_data[index].ni;
**2-7：对于所有有物理含义的变量、常量，如果其命名不是充分自注释的，在声明时都必须加以注释，说明其物理含义。**变量、常量、宏的注释应放在其上方相邻位置或右方。
示例：
/* active statistic task number */
\#define MAX_ACT_TASK_NUMBER 1000
\#define MAX_ACT_TASK_NUMBER 1000 /* active statistic task number */
**2-8：数据结构声明(包括数组、结构、类、枚举等)，如果其命名不是充分自注释的，必须加以注释。对数据结构的注释应放在其上方相邻位置，不可放在下面；对结构中的每个域的注释放在此域的右方。**
示例：可按如下形式说明枚举/数据/联合结构。
/* sccp interface with sccp user primitive message name */
enum SCCP_USER_PRIMITIVE
{
N_UNITDATA_IND, /* sccp notify sccp user unit data come */
N_NOTICE_IND, /* sccp notify user the No.7 network can not */
/* transmission this message */
N_UNITDATA_REQ, /* sccp user's unit data transmission request*/
};
**2-9：全局变量要有较详细的注释，包括对其功能、取值范围、哪些函数或过程存取它以及存取时注意事项等的说明。**
示例：
/* The ErrorCode when SCCP translate */
/* Global Title failure, as follows */ // 变量作用、含义
/* 0 － SUCCESS 1 － GT Table error */
/* 2 － GT error Others － no use */ // 变量取值范围
/* only function SCCPTranslate() in */
/* this modual can modify it, and other */
/* module can visit it through call */
/* the function GetGTTransErrorCode() */ // 使用方法
BYTE g_GTTranErrorCode;
**2-10：注释与所描述内容进行同样的缩排。**
说明：可使程序排版整齐，并方便注释的阅读与理解。示例：如下例子，排版不整齐，阅读稍感不方便。
void example_fun( void )
{
/* code one comments */
CodeBlock One
/* code two comments */
CodeBlock Two
}
应改为如下布局。
void example_fun( void )
{
/* code one comments */
CodeBlock One
/* code two comments */
CodeBlock Two
}
**2-11：避免在一行代码或表达式的中间插入注释。**
说明：除非必要，不应在代码或表达中间插入注释，否则容易使代码可理解性变差。
**2-12：通过对函数或过程、变量、结构等正确的命名以及合理地组织代码的结构，使代码成为自注释的。**
说明：清晰准确的函数、变量等的命名，可增加代码可读性，并减少不必要的注释。
**2-13：在代码的功能、意图层次上进行注释，提供有用、额外的信息。**
说明：注释的目的是解释代码的目的、功能和采用的方法，提供代码以外的信息，帮助读者理解代码，防止没必要的重复注释信息。
示例：如下注释意义不大。
/* if receive_flag is TRUE */
if (receive_flag)
而如下的注释则给出了额外有用的信息。
/* if mtp receive a message from links */
if (receive_flag)
**2-14：在程序块的结束行右方加注释标记，以表明某程序块的结束。**
说明：当代码段较长，特别是多重嵌套时，这样做可以使代码更清晰，更便于阅读。示例：参见如下例子。
if (...)
{
// program code
while (index < MAX_INDEX)
{
// program code
} /* end of while (index < MAX_INDEX) */ // 指明该条while 语句结束
} /* end of if (...)*/ // 指明是哪条if 语句结束
**2-15：注释格式尽量统一，建议使用“/* …… */”。**
**2-16：注释应考虑程序易读及外观排版的因素，使用的语言若是中、英兼有的，建议多使用中文，除非能用非常流利准确的英文表达。**
说明：注释语言不统一，影响程序易读性和外观排版，出于对维护人员的考虑，建议使用中文。

