
# 华为C语言编程规范--变量、结构 -  芝麻软件工作室的专栏 - CSDN博客


2016年06月16日 17:38:44[seven-soft](https://me.csdn.net/softn)阅读数：592


**5-1：去掉没必要的公共变量。**
说明：公共变量是增大模块间耦合的原因之一，故应减少没必要的公共变量以降低模块间的耦合度。
**5-2：仔细定义并明确公共变量的含义、作用、取值范围及公共变量间的关系。**
说明：在对变量声明的同时，应对其含义、作用及取值范围进行注释说明，同时若有必要还应说明与其它变量的关系。
**5-3：明确公共变量与操作此公共变量的函数或过程的关系，如访问、修改及创建等。**
说明：明确过程操作变量的关系后，将有利于程序的进一步优化、单元测试、系统联调以及代码维护等。这种关系的说明可在注释或文档中描述。
示例：在源文件中，可按如下注释形式说明。
RELATION System_Init Input_Rec Print_Rec Stat_Score Student Create Modify Access Access Score Create Modify Access
 Access, Modify
注：RELATION 为操作关系；System_Init、Input_Rec、Print_Rec、Stat_Score 为四个不同的函数；Student、Score 为两个全局变量；Create 表示创建，Modify 表示修改，Access 表示访
问。其中，函数Input_Rec、Stat_Score 都可修改变量Score，故此变量将引起函数间较大的耦合，并可能增加代码测试、维护的难度。
**5-4：当向公共变量传递数据时，要十分小心，防止赋与不合理的值或越界等现象发生。**
说明：对公共变量赋值时，若有必要应进行合法性检查，以提高代码的可靠性、稳定性。
**5-5：防止局部变量与公共变量同名。**
说明：若使用了较好的命名规则，那么此问题可自动消除。
**5-6：严禁使用未经初始化的变量作为右值。**
说明：特别是在C/C++中引用未经赋值的指针，经常会引起系统崩溃。
**5-7：结构的功能要单一，是针对一种事务的抽象。**
说明：设计结构时应力争使结构代表一种现实事务的抽象，而不是同时代表多种。结构中的各元素应代表同一事务的不同侧面，而不应把描述没有关系或关系很弱的不同事务的元素放到同一结构中。
示例：如下结构不太清晰、合理。
typedef struct STUDENT_STRU
{
unsigned char name[8]; /* student's name */
unsigned char age; /* student's age */
unsigned char sex; /* student's sex, as follows */
/* 0 - FEMALE; 1 - MALE */
unsigned char
teacher_name[8]; /* the student teacher's name */
unisgned char
teacher_sex; /* his teacher sex */
} STUDENT;
若改为如下，可能更合理些。
typedef struct TEACHER_STRU
{
unsigned char name[8]; /* teacher name */
unisgned char sex; /* teacher sex, as follows */
/* 0 - FEMALE; 1 - MALE */
} TEACHER;
typedef struct STUDENT_STRU
{
unsigned char name[8]; /* student's name */
unsigned char age; /* student's age */
unsigned char sex; /* student's sex, as follows */
/* 0 - FEMALE; 1 - MALE */
unsigned int teacher_ind; /* his teacher index */
} STUDENT;
**5-8：不要设计面面俱到、非常灵活的数据结构。**
说明：面面俱到、灵活的数据结构反而容易引起误解和操作困难。
**5-9：不同结构间的关系不要过于复杂。**
说明：若两个结构间关系较复杂、密切，那么应合为一个结构。
示例：如下两个结构的构造不合理。
typedef struct PERSON_ONE_STRU
{
unsigned char name[8];
unsigned char addr[40];
unsigned char sex;
unsigned char city[15];
} PERSON_ONE;
typedef struct PERSON_TWO_STRU
{
unsigned char name[8];
unsigned char age;
unsigned char tel;
} PERSON_TWO;
由于两个结构都是描述同一事物的，那么不如合成一个结构。
typedef struct PERSON_STRU
{
unsigned char name[8];
unsigned char age;
unsigned char sex;
unsigned char addr[40];
unsigned char city[15];
unsigned char tel;
} PERSON;
**5-10：结构中元素的个数应适中。**若结构中元素个数过多可考虑依据某种原则把元素组成不同的子结构，以减少原结构中元素的个数。
说明：增加结构的可理解性、可操作性和可维护性。
示例：假如认为如上的_PERSON 结构元素过多，那么可如下对之划分。
typedef struct PERSON_BASE_INFO_STRU
{
unsigned char name[8];
unsigned char age;
unsigned char sex;
} PERSON_BASE_INFO;
typedef struct PERSON_ADDRESS_STRU
{
unsigned char addr[40];
unsigned char city[15];
unsigned char tel;
} PERSON_ADDRESS;
typedef struct PERSON_STRU
{
PERSON_BASE_INFO person_base;
PERSON_ADDRESS person_addr;
} PERSON;
**5-11：仔细设计结构中元素的布局与排列顺序，使结构容易理解、节省占用空间，并减少引起误用现象。**
说明：合理排列结构中元素顺序，可节省空间并增加可理解性。
示例：如下结构中的位域排列，将占较大空间，可读性也稍差。
typedef struct EXAMPLE_STRU
{
unsigned int valid: 1;
PERSON person;
unsigned int set_flg: 1;
} EXAMPLE;
若改成如下形式，不仅可节省1 字节空间，可读性也变好了。
typedef struct EXAMPLE_STRU
{
unsigned int valid: 1;
unsigned int set_flg: 1;
PERSON person ;
} EXAMPLE;
**5-12：编程时，要注意数据类型的强制转换。**
说明：当进行数据类型强制转换时，其数据的意义、转换后的取值等都有可能发生变化，而这些细节若考虑不周，就很有可能留下隐患。
**5-13：对编译系统默认的数据类型转换，也要有充分的认识。**
示例：如下赋值，多数编译器不产生告警，但值的含义还是稍有变化。
char chr;
unsigned short int exam;
chr = -1;
exam = chr; // 编译器不产生告警，此时exam 为0xFFFF。
**5-14：尽量减少没有必要的数据类型默认转换与强制转换。**
**5-15：合理地设计数据并使用自定义数据类型，避免数据间进行不必要的类型转换。**
**5-16：对自定义数据类型进行恰当命名，使它成为自描述性的，以提高代码可读性。注意其命名方式在同一产品中的统一。**
说明：使用自定义类型，可以弥补编程语言提供类型少、信息量不足的缺点，并能使程序清晰、简洁。
示例：可参考如下方式声明自定义数据类型。下面的声明可使数据类型的使用简洁、明了。
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
下面的声明可使数据类型具有更丰富的含义。
typedef float DISTANCE;
typedef float SCORE;

