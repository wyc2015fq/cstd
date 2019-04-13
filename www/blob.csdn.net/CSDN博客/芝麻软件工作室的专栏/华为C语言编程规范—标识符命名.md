
# 华为C语言编程规范—标识符命名 -  芝麻软件工作室的专栏 - CSDN博客


2016年06月16日 17:36:40[seven-soft](https://me.csdn.net/softn)阅读数：392


**3-1：标识符的命名要清晰、明了，有明确含义，同时使用完整的单词或大家基本可以理解的缩写，避免使人产生误解。**
说明：较短的单词可通过去掉“元音”形成缩写；较长的单词可取单词的头几个字母形成缩写；一些单词有大家公认的缩写。示例：如下单词的缩写能够被大家基本认可。
temp 可缩写为 tmp ;
flag 可缩写为 flg ;
statistic 可缩写为 stat ;
increment 可缩写为 inc ;
message 可缩写为 msg ;
**3-2：命名中若使用特殊约定或缩写，则要有注释说明。**
说明：应该在源文件的开始之处，对文件中所使用的缩写或约定，特别是特殊的缩写，进行必要的注释说明。
**3-3：自己特有的命名风格，要自始至终保持一致，不可来回变化。**
说明：个人的命名风格，在符合所在项目组或产品组的命名规则的前提下，才可使用。（即命名规则中没有规定到的地方才可有个人命名风格）。
**3-4：对于变量命名，禁止取单个字符（如i、j、k...），建议除了要有具体含义外，还能表明其变量类型、数据类型等，但i、j、k 作局部循环变量是允许的。**
说明：变量，尤其是局部变量，如果用单个字符表示，很容易敲错（如i 写成j），而编译时又检查不出来，有可能为了这个小小的错误而花费大量的查错时间。
示例：下面所示的局部变量名的定义方法可以借鉴。
int liv_Width
其变量名解释如下：
l 局部变量（Local） （其它：g 全局变量（Global）...）
i 数据类型（Interger）
v 变量（Variable） （其它：c 常量（Const）...）
Width 变量含义
这样可以防止局部变量与全局变量重名。
**3-5：命名规范必须与所使用的系统风格保持一致，并在同一项目中统一，比如采用UNIX的全小写加下划线的风格或大小写混排的方式，不要使用大小写与下划线混排的方式，用作特殊标识如标识成员变量或全局变量的m_和g_，其后加上大小写混排的方式是允许的。**
示例： Add_User 不允许，add_user、AddUser、m_AddUser 允许。
**3-6：除非必要，不要用数字或较奇怪的字符来定义标识符。**
示例：如下命名，使人产生疑惑。
\#define _EXAMPLE_0_TEST_
\#define _EXAMPLE_1_TEST_
void set_sls00( BYTE sls );
应改为有意义的单词命名
\#define _EXAMPLE_UNIT_TEST_
\#define _EXAMPLE_ASSERT_TEST_
void set_udt_msg_sls( BYTE sls );
**3-7：在同一软件产品内，应规划好接口部分标识符（变量、结构、函数及常量）的命名，防止编译、链接时产生冲突。**
说明：对接口部分的标识符应该有更严格限制，防止冲突。如可规定接口部分的变量与常量之前加上“模块”标识等。
**3-8：用正确的反义词组命名具有互斥意义的变量或相反动作的函数等。**
说明：下面是一些在软件中常用的反义词组。
add / remove begin / end create / destroy
insert / delete first / last get / release
increment / decrement put / get
add / delete lock / unlock open / close
min / max old / new start / stop
next / previous source / target show / hide
send / receive source / destination
cut / paste up / down
示例：
int min_sum;
int max_sum;
int add_user( BYTE *user_name );
int delete_user( BYTE *user_name );
**3-9：除了编译开关/头文件等特殊应用，应避免使用_EXAMPLE_TEST_之类以下划线开始和结尾的定义。**

