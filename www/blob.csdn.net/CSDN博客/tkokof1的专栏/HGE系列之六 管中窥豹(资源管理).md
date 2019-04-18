# HGE系列之六 管中窥豹(资源管理) - tkokof1的专栏 - CSDN博客

2010年06月07日 08:58:00[tkokof1](https://me.csdn.net/tkokof1)阅读数：3082


HGE系列之六管中窥豹(资源管理)

记的上次浮光掠影的讲了一些HGE中的基础类别，不知大家了解了多少，仔细看过的朋友肯定知道当时在讲述一个类别的构造函数时我打了个马虎，直接略过了，原因说的好像是比较繁琐，想起来了吗？对，就是**hgeStringTable **，为了免去“避重就轻”的嫌疑，我想这次也该好好的将他说一说了（但说实话，讲解这个函数确实比较繁琐……），而**hgeStringTable **是HGE资源管理中的一个基础（**Base**）类别，所以我想顺着讲述一下HGE的资源管理实现，相信这种叙述顺序还是比较符合逻辑的：）不过在开始我们又一次HGE源码之旅时，我必须提醒一句：这次旅行可没上次那般轻松惬意了……什么，你不相信？那么不妨来看看我们旅程的“第一站”，也就是那个**hgeStringTable **“神秘”的构造函数吧：

hgeStringTable::hgeStringTable(const char *filename)

{

// 一些变量声明

int i;

void *data;

DWORD size;

char *desc, *pdesc;

// 上次分析过的 NamedString 结构

NamedString *str;

char str_name[MAXSTRNAMELENGTH];

char *str_value, *pvalue;

// 创建HGE

hge=hgeCreate(HGE_VERSION);

// 设置链表头结点为空

strings=0;

// 使用HGE提供的Resource_Load接口加载文件

// 注意返回的 void* 指针所指向的便是文件中的内容

// 相关文件大小则存放在size中

data=hge->Resource_Load(filename, &size);

// 如果data为空，则加载失败，直接返回

if(!data) return;

// 将文件内容全部拷贝至动态申请的desc数组中，以进行之后的解析

desc = new char[size+1];

memcpy(desc,data,size);

desc[size]=0;

// 释放由之前Resource_Load函数分配的内存（即data所指向的区域）

hge->Resource_Free(data);

// 查看文件头（是否是“[HGESTRINGTABLE]”）

if(memcmp(desc, STRHEADERTAG, sizeof(STRHEADERTAG)-1))

{

// 如果不是则使用System_Log接口将错误记录至日志文件，

// 并释放之前分配的desc资源

hge->System_Log(STRFORMATERROR, filename);

delete[] desc;

return;

}

// 将pdesc设置为desc偏移sizeof(STRHEADERTAG)位置，即实际的数据位置

pdesc=desc+sizeof(STRHEADERTAG);

// 用于存储字符串值的动态数组

str_value=new char[8192];

for(;;)

{

// 跳过空白

while(isspace(*pdesc)) pdesc++;

// 如果pdesc指向元素为空（'/0'），则跳出循环

if(!*pdesc) break;

// 跳过注释，即以 ';' 开头的行

if(*pdesc==';')

{

while(*pdesc && *pdesc != '/n') pdesc++;

pdesc++;

continue;

}

// 获取字符串的名字

i=0;

// 注意循环条件

while(pdesc[i] && pdesc[i]!='=' && !isspace(pdesc[i]) && i<MAXSTRNAMELENGTH)

{

str_name[i]=pdesc[i];

i++;

}

// 设置字符串结束符

str_name[i]=0;

// 当然，不能忘了设置pdesc的偏移

pdesc+=i;

// 由于有 MAXSTRNAMELENGTH 的字符串名称长度限制，

// 所以需要处理超出的名称字符

while(*pdesc && *pdesc!='=' && !isspace(*pdesc)) pdesc++;

if(!*pdesc) break;

// 跳过 '=' 之前的空白符号

while(isspace(*pdesc)) pdesc++;

// 如果所指不是 '=' ，那么调用Sysytem_Log记录至日志文件，并跳出循环

if(*pdesc!='='){ hge->System_Log(STRFORMATERROR, filename); break; }

pdesc++;

// 跳过 ' " ' 符号之前的空格

while(isspace(*pdesc)) pdesc++;

// 如果所指不是 ' " ', 那么调用Sysytem_Log记录至日志文件，并跳出循环

if(*pdesc!='"'){ hge->System_Log(STRFORMATERROR, filename); break;}

pdesc++;

// 解析字符串的实际内容（至最后的 ' " ' 符号）

// 注意一下特殊符号的处理

pvalue=str_value;

// 注意一下循环条件

while(*pdesc && *pdesc!='"')

{

// 处理 '/n' 以及 '/r'

if(*pdesc=='/n' || *pdesc=='/r')

{

while(isspace(*pdesc)) pdesc++;

pvalue--;

while(pvalue>=str_value && isspace(*pvalue)) pvalue--;

pvalue++; *pvalue=' '; pvalue++;

continue;

}

// 处理 '/' 转义字符

if(*pdesc=='//')

{

pdesc++;

if(!*pdesc) continue;

if(*pdesc=='n') *pvalue='/n';

else *pvalue=*pdesc;

pvalue++;

pdesc++;

continue;

}

// 将pdesc的内容复制入 pvalue 中

*pvalue=*pdesc; pvalue++;

pdesc++;

}

// 设置结束标志

*pvalue=0;

// 至此我们已经成功解析了一个字符串“定义”,将其添加入链表中（链表头部）

str=new NamedString;

strcpy(str->name, str_name);

str->string=new char[strlen(str_value)+1];

strcpy(str->string, str_value);

str->next=strings;

strings=str;

if(!*pdesc) break;

pdesc++;

}// end of for(;;)

// 不要忘了释放缓存资源

delete[] str_value;

delete[] desc;

}

呼，终于将这个“庞大”的构造函数注释完了，相信大家看的也是够呛，如果实在看的不是太清晰，那也没有多大关系，我再来提纲挈领的讲一讲：

一开始，构造函数调用HGE提供的资源加载接口加载文件资源，然后程序开始解析，首先他会检查该文件是否拥有名为**HGESTRINGTABLE **的字段（section），接着他开始解析实际的字符串定义，相应格式为：

字符串名 ="字符串值"

然后么自然将解析的结果构造成一个NamedString，然后添加入链表。接着便是重复执行，知道内容结束。

大体流程就是这样，如果你还不满意，那就请自行研习源码吧，我的讲解便到此为止了，因为我讲述这个函数的目的并不是想细究其每个细节，然后学以致用，恰恰相反，我的目的是“学以不用”，是的，你没有看错，的确是“学以不用”：

首先，这个函数过于“庞大”，这便造成了可读性上的大打折扣（相信你已经有所体会……）以及非常差的可维护性（你可以试试走一走程序的控制流……）

再者，该函数的逻辑相对复杂，本应该将个中功能分解至单一函数来进行一一处理，然而目前的情况却是一股脑儿的凑成了一大碗[意大利面条](http://en.wikipedia.org/wiki/Spaghetti_code)……

最后，程序的目的是解析一种相关的配置文件格式，然而这种格式是完全可以与 hgeStringTable 类相分离的，也就是说，处理这类配置文件的功能完全可以抽象出来形成另外一个辅助类（helper），大可不必写成现在的这幅模样……

所以总的说来，这个函数是一个非常鲜明生动的“反面教材” ：）

至此，虽然有些不敢苟同，但hgeStringTable 终算是讲解完毕了，那么稍稍缓口气，让我们继续向 HGE 资源管理模块迈进：

**类名**：ResDesc

**功能**：资源描述类

**头文件**：hge/hge181/include/hgeresource.h

**实现文件**：无

老方法，让我们首先来看一看他的头文件：

struct ResDesc

{

// 资源名称

charname[MAXRESCHARS];

// 所属资源的组号

intresgroup;

// 资源“句柄”

DWORDhandle;

// 自向指针（看来大抵上又是用来构成单链表的）

ResDesc*next;

// 相对简易的构造函数和析构函数

ResDesc(){ hge=hgeCreate(HGE_VERSION); }

~ResDesc(){ hge->Release(); }

// 获取资源“句柄”，注意参数

virtual DWORD Get(hgeResourceManager *rm) = 0;

// 释放资源

virtual voidFree() = 0;

protected:

// 静态的HGE指针

static HGE*hge;

};

很明显，ResDesc是一个纯虚类，程序中是不能创建该类的对象的，一般而言这种类别也仅是作为基类，然后进行派生，考虑到该类别是一个资源描述类，那么自然想到是不是HGE中的各类资源都继承于他呢？如果你也是这么考虑的，那么恭喜你，回答正确，加十分！

但就目前而言，我们对于这个类别中的一些细节还是有些模糊，譬如Get函数（或者说接口）中的 hgeResourceManager ，好吧，为了解决这些疑问，让我们继续看下去：

**类名**：hgeResourceManager

**功能**：资源管理类

**头文件**：hge/hge181/include/hgeresource.h

**实现文件**：hge/hge181/src/helpers/hgeresource.cpp

首先来看看他的头文件：

class hgeResourceManager

{

public:

hgeResourceManager(const char *scriptname=0);

~hgeResourceManager();

// 变换资源脚本

voidChangeScript(const char *scriptname=0);

// 查看给定组别资源是否已经缓存

boolPrecache(int groupid=0);

// 清除指定资源号的资源

voidPurge(int groupid=0);

// 通用的获取资源函数，注意返回类型为 void*

void*GetResource(const char *name, int resgroup=0);

// 获取资源句柄的函数“群”

HTEXTUREGetTexture(const char *name, int resgroup=0);

HEFFECTGetEffect(const char *name, int resgroup=0);

HMUSICGetMusic(const char *name, int resgroup=0);

HSTREAMGetStream(const char *name, int resgroup=0);

HTARGETGetTarget(const char *name);

// 获取相关资源的函数“群”

hgeSprite*GetSprite(const char *name);

hgeAnimation*GetAnimation(const char *name);

hgeFont*GetFont(const char *name);

hgeParticleSystem*GetParticleSystem(const char *name);

hgeDistortionMesh*GetDistortionMesh(const char *name);

hgeStringTable*GetStringTable(const char *name, int resgroup=0);

// 资源描述的指针数组

ResDesc*res[RESTYPES];

private:

// 将拷贝构造函数设置为私有，以起到禁用拷贝构造的作用

hgeResourceManager(const hgeResourceManager &);

// 将赋值操作设置为私有，以起到“屏蔽”作用

hgeResourceManager&operator= (const hgeResourceManager&);

// 私有的辅助函数

// 移除所有资源

void_remove_all();

// 解析资源脚本

void_parse_script(const char *scriptname=0);

static HGE*hge;

};

看来hgeResourceManager的定义还是比较清晰的，让我们一次来看一看个中函数的实现方法：

首先自然是其的构造函数与析构函数：

hgeResourceManager::hgeResourceManager(const char *scriptname)

{

// 创建HGE

hge=hgeCreate(HGE_VERSION);

// 初始化res数组

for(int i=0;i<RESTYPES;i++) res[i]=0;

// 解析给定的资源脚本

_parse_script(scriptname);

}

hgeResourceManager::~hgeResourceManager()

{

// 释放所有的资源

_remove_all();

// 释放HGE

hge->Release();

}

看来这两个函数还是比较简易的，一个加载，一个释放，比较“中规中矩” ：）

那么至此，对于hgeResourceManager的数据结构模型，大家应该能够想像了：

大体上便是这副样子：

![](http://hi.csdn.net/attachment/201006/7/0_1275872135Htpu.gif)

接着，然我们顺着来看一看构造函数和析构函数中涉及的_parse_script和_remove_all

void hgeResourceManager::_parse_script(const char *scriptname)

{

ResDesc *rc, *rcnext;

// 如果资源脚本名不为空

if(scriptname)

{

// 使用了RScript函数的Parse函数进行解析

RScript::Parse(this, NULL, scriptname, NULL);

// 释放res[RES_SCRIPT]

rc=res[RES_SCRIPT];

while(rc)

{

rc->Free();

rcnext=rc->next;

delete rc;

rc=rcnext;

}

res[RES_SCRIPT]=0;

}

}

其中RES_SCRIPT是一个宏定义，位于hge/hge181/src/helpers/resources.h

#define RES_SCRIPT0

另外值得一提的是，该文件中另外还定义了其他的资源类别，有兴趣的朋友可以看一看。

_parse_script函数中除了一个目前对我们来说还是“黑盒”的RScript::Parse外，对于res[RES_SCRIPT]（即res[0]）的处理也是有些令人费解，不过我们大可不必在此过分纠结，与其停步苦思冥想，不如继续前行，说不定马上会豁然开朗：）

void hgeResourceManager::_remove_all()

{

int i;

ResDesc *rc, *rcnext;

// 释放从res[0] 至res[RESTYPES] 链表资源

for(i=0;i<RESTYPES;i++)

{

rc=res[i];

while(rc)

{

rc->Free();

rcnext=rc->next;

delete rc;

rc=rcnext;

}

res[i]=0;

}

}

看来 _remove_all 也是不出所料啊，依次遍历并释放资源：）

接着让我们看看析构函数之后的三个公有函数：

// 变换资源脚本

void hgeResourceManager::ChangeScript(const char *scriptname)

{

_remove_all();

_parse_script(scriptname);

}

不出所料，首先释放，然后便是重新加载

// 查看给定的组别资源是否已经缓存（有效），如果groupid为0则检查所有组别

bool hgeResourceManager::Precache(int groupid)

{

int i;

ResDesc *rc;

bool bResult=true;

// 遍历res[0] 到 res[RESTYPES-1]

for(i=0;i<RESTYPES;i++)

{

rc=res[i];

while(rc)

{

// 注意条件的判断

if(!groupid || groupid==rc->resgroup) bResult=bResult && (rc->Get(this)!=0);

rc=rc->next;

}

}

return bResult;

}

// 清除给定组别的资源，如果groupid为0则清除所有组别

void hgeResourceManager::Purge(int groupid)

{

int i;

ResDesc *rc;

// 遍历res[0] 到 res[RESTYPES-1]

for(i=0;i<RESTYPES;i++)

{

rc=res[i];

while(rc)

{

// 注意条件判断

if(!groupid || groupid==rc->resgroup) rc->Free();

rc=rc->next;

}

}

}

好了，看来这三个函数也并不复杂，接下来剩下的便是一堆获取资源函数，让我们首先来看看其中的GetStringTable，没有错，就是获取字符串表的函数，由于目前来讲我们对于hgeStringTable这个类别比较熟悉，以此函数开始阅览，相信能让我们更快的找到关键点（key point）：

hgeStringTable* hgeResourceManager::GetStringTable(const char *name, int resgroup)

{

hgeStringTable *strtable;

RStringTable *resource;

// 使用FindRes函数来查找给定的资源名称

ResDesc *Res=FindRes(this, RES_STRTABLE, name);

// 如果找到，则返回

if(Res) return (hgeStringTable*)Res->Get(this);

// 没有找到

else

{

// 以name新建一个hgeStringTable

strtable=new hgeStringTable(name);

// 如果(加载)成功

if(strtable)

{

// 新建一个RStringTable类型

resource=new RStringTable();

// 设置资源句柄

resource->handle=(DWORD)strtable;

// 设置资源组别

resource->resgroup=resgroup;

// 设置资源名称

strcpy(resource->name, name);

// 设置RStringTable的资源脚本名称，与其资源名称相同

strcpy(resource->filename, name);

// 通过AddRes函数增加资源

AddRes(this, RES_STRTABLE, resource);

return strtable;

}

}

return 0;

}

这个函数看起来有点磕碰，首先我们便遇到了一个没见过的类型：RStringTable，查看代码，其实他就是一个之前ResDesc类别的派生类：

**类名**：RStringTable

**功能**：字符串表资源类

**头文件**：hge/hge181/ src/helpers/resources.h

**实现文件**：hge/hge181/src/helpers/resources.cpp

struct RStringTable : public ResDesc

{

charfilename[MAXRESCHARS];

staticvoidParse(hgeResourceManager *rm, RScriptParser *sp, const char *name, const char *basename);

virtual DWORD Get(hgeResourceManager *rm);

virtual voidFree();

};

相当简单，实现了Get和Free两个自ResDesc继承而来的纯虚函数，并且添加了名为filename的数据成员以及一个静态的Parse函数，然我们就势来看看他们的实现：

DWORD RStringTable::Get(hgeResourceManager *rm)

{

// 如果handle为空，则直接新建返回hgeStringTable(filename)

if(!handle)handle = (DWORD)new hgeStringTable(filename);

// 否则直接返回

return handle;

}

void RStringTable::Free()

{

// 如果handle不为空，则使用delete删除

if(handle) delete (hgeStringTable *)handle;

handle=0;

}

Get和Free看来还是相当简易的，那么那个静态的Parse呢？

void RStringTable::Parse(hgeResourceManager *rm, RScriptParser *sp, const char *name, const char *basename)

{

ScriptParseFileResource(rm, sp, name, basename, new RStringTable(), RES_STRTABLE);

}

可以看到，Parse函数只是一个对ScriptParseFileResource 的简单调用，那么ScriptParseFileResource有时那个函数呢？不用急，其实他也位于hge/hge181/src/helpers/resources.cpp 中，但是对他讲解我们会一下子牵扯到其他许多我们未有接触的内容，所以我在此先暂时放一放，留作下次讲解（当然，如果你实在等不及，那还是那句老话，请打开文件自己观赏……）

好了，然我们回到GetStringTable的讲解（是不是有点像堆栈调用：）），现在我们已经大体上了解了一下RStringTable，但接着我们又遇到了一个没见过的函数：FindRes，他又是怎么一回事呢？其实顾名思义，我们也基本能够猜出这个函数的作用：定位资源，但是他的定义又在哪里呢？其实他也在hge/hge181/src/helpers/resources.cpp中，让我们来看一下：

ResDesc *FindRes(hgeResourceManager *rm, int type, const char *name)

{

ResDesc *rc;

// 首先定位组别资源

rc=rm->res[type];

while(rc)

{

// 比较资源名称，如果相同则返回

if(!strcmp(name, rc->name)) return rc;

rc=rc->next;

}

// 没有找到，怎返回0

return 0;

}

代码还是相当简易的，但是对于不将这个函数封装成hgeResourceManager的成员函数，我个人是不敢苟同的，相应的，虽然我们还没有遇到AddRes函数，但是也不妨碍我们来也先瞧上一瞧：

void AddRes(hgeResourceManager *rm, int type, ResDesc *resource)

{

// 将resource置于相应资源组别的链表头部

resource->next=rm->res[type];

rm->res[type]=resource;

}

同样道理，这个函数我认为也应该封装成hgeResourceManager的成员函数。

好了，至此，GetStringTable这个函数带给我们的阅读障碍应该已经基本消除，大体的流程便是：首先查找是否存在给定的资源，如果有则直接返回，否则新建相应的hgeStringTable，并根据其构造出一个RStringTable，然后加入hgeResourceManager的资源管理链表中。

不过对于GetStringTable在未有找到资源时采取的添加策略，我个人还是有些微词的，毕竟其显然违反了[单一职责原则](http://en.wikipedia.org/wiki/Single_responsibility_principle)，而且也不足够灵活，试想，如果我想查找一些暂时没有加入hgeResourceManager的资源时，GetStringTable就不是那么方便了：）最后一点就是，改函数的名字和干的事情并不匹配，GetStringTable如果可以添加资源的话，那SetStringTable又应该干什么呢……？

呼呼，至此我们终于讲解完了GetStringTable，而剩下的Get* 函数还有很多很多，但是可幸的是，其间的原理并没有多少差异，毕竟我们阅读源码的首要目的不是了解细节，而是解其原理，所以我想说的是，既然我们现在对于Get* 函数的原理有了不错的了解，那么就暂可不必急着查看其他的Get* 函数，慢慢来，相信终归有一天能够了然于胸：）

好了，这次源码之旅我想是时候停一停了（说实话，我觉得早该停了……），大家也该好好休息整理一下了，当然，HGE的资源管理还不止这些内容（还记得那个神秘的RScript::Parse吗……？），但这些就让我们留在下次“品尝”吧，好了，就这样吧，我呢还是那句老话：下次再见吧：）

