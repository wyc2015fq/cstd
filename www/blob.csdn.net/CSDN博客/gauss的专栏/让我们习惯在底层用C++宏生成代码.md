# 让我们习惯在底层用C++宏生成代码 - gauss的专栏 - CSDN博客
2013年01月03日 18:34:45[gauss](https://me.csdn.net/mathlmx)阅读数：419
个人分类：[C/C++/内存管理](https://blog.csdn.net/mathlmx/article/category/605906)
                
春天最难堪的是没有收成的垦植；人生最难堪的是失踪往决意信念的寂寞 我们知道操作C++泛型编程可以写出扩年夜性很是强的法式，操作C++的接***术也可以很等闲的扩年夜出已经存在的法式。只不外有一件工作它们可都做不到，那就是操作宏来产生代码。![](http://pic.cnpetweb.com//M0/S1/1541-0.jpg)操作宏来生成代码凡是对于库作者和底层架构者具有不凡的意义，而在操作层小我认为该当尽量削减它的操作，当然，这并不影响我们往进修并认识它。
思惟好比火星：一颗火星会点燃另一颗火星。一个沉思熟虑的教师和班主任，老是力争在集体中缔造一种配合热爱科学和渴求常识的气氛，使智力快乐喜爱成为一些线索，以其诚挚的、复杂的关系——即思惟的彼此关系把一个个的学生毗连在一路。        那么什么叫“操作宏来产生代码”呢？
       首先我提出如下的需求：我现在需要一个函数JoinString，它能吸收1到20个不等的参数，并将每一个参数转换成字符串，最后在函数内部把这些字符串拼接起来然后作为一个字符串返回。![](http://pic.cnpetweb.com//M0/S1/1541-1.jpg)近似于这样：
string JoinString（ 参数列表... ）
       {
               转换
               返回参数列表组合成的字符串
        }
       有人说为什么不用sprintf_s？我说，那是因为它不是类型平安的，我们的方针是类型平安。简略的例子，假如你在名目化字符串中传进的是％s却传进一个int的话，编译器是搜检不出来的，然则运行时会犯错，这是它的严重错误谬误。
      假如是你往怎么实现呢？
       现在我提出一种实现方法。首先，我们针对每一种类型实现一个具有一个参数的JoinString，好比对于int ， char， float甚至Windows的SYSTEMTIME，Ogre的Vect3实现。然后再基于这个参数，实现2个参数的3个参数的....20个参数的。
       您头晕了吗？我还好。只不外想着这个工作量就很恶心是吧？想象吧，假如我们要建造10种类型的转换函数，那么一个参数需要10个。2个参数呢？不是20种哦，因为第一个参数和第二个参数可以分歧哦，那么他们是10*10 = 100种。好，那么20个分歧的参数的JoinString，实现10种类型，或者100种类型。哇靠，那是一个什么数字啊！！！！！！！！！！！！！！
       好的，假如您足够聪慧，您确定不会这么干，对吧。好了，让我们来说说第二种方法。
       c++有一种模板技巧，假如我们操作模板的话，那么会变得很好办。对于一个参数的JoinString，那么它可能的声明是这样的：
template < typename T >
string JoinString（ const T& value ）
{
     返回成效
}
         然后我们可以供给拜别具有2-20个参数的模板参数。有的伴侣会说，不行啊你这样。这样你不是还需要对每个JoinString做特化么？因为对于分歧类型的T，转换方法分歧啊！！！是的，是这样的，那么标题就聚焦在分歧类型的T，具有分歧的转换方法。好的，让我们来解决这个标题。
         首先，向人人介绍“垫片”的概念。什么是垫片呢？的桌子的四个脚分歧高的时辰，我们需要拿工具塞在短的脚下面让他们同样的高对短处？那好，这个工具就是垫片。在C++中，也有近似的垫片的概念。好比现在我们需要某种类型的描写，也就是只对字符串感快乐喜爱，那么我们可以写这样一个函数：
    string Desc（ 类型 ）{ 返回描写的字符串 }
        然后对分歧的类型供给这个函数的重载（操作模板特化亦可），即可在遭遇任何类型的时辰我们都能获得它的描写（前提是你实现了它的垫片），这样，函数的挪用者永远不需要做任何批改，这样实现了模块间的自力性。
         好了让我们来看看一个简略的垫片法式吧~~呵呵。
＃include <iostream>
＃include <string>
＃include <sstream>
using namespace std;
string Desc（ int a ）
{
ostringstream ostr;
ostr<< a;
return ostr.str（）;
}
string Desc（ double dd ）
{
ostringstream ostr;
ostr<< dd;
return ostr.str（）;
}
void print（ string str ）
{
cout<< str << endl;
}
void main（）
{
print（ Desc（100） ）;
print（ Desc（12.4） ）;
}
        关于垫片就说到这里，更多的内容建议人人往看Matthew Wilson的书《Imperfect c++》，上面有提到哦。
       好了，现在我们可以用模板来实现我们的JoinString了。我的字符串垫片是这样实现的：
template < typename T >
inline std::string GetString（ const T& value ）
{
   std::string strResult;
   strResult = boost::lexical_cast< std::string >（value）;
   return strResult;
}
＃ifndef HERO_BUILD_NOT_WITH_OGRE
template <>
inline std::string GetString（ const Ogre::Vect3& value ）
{
   std::ostringstream ostr;
   ostr << value;
   return ostr.str（）;
}
＃endif
       这是一个基于boost::lexical_cast，因为它可以帮我们实现内置类型到字符串的转换，让我不用自己往做，如允许以俭仆良多时刻哦。（PS：boost是一个优良的C++法式库，它的主页是：http://www.boost.g）
       首先，我们可能需要措置赏罚一个吸收字符串的函数，然后我们在模板函数中操作GetString<T>（value）获取模板参数的字符串值并传递给我们的真正的措置赏罚函数。好比这样：
template <typename T >
   class Join_Hler
   {
   public:
    typedef T            value_type;
    typedef T&           reference_type;
   public:
    Join_Hler（ reference_type value ）:m_objValue（value）
    {
    }
   public:
    template < typename T1 >
    inline T& SmartWrite（ const T1& t1 ）
    { 
     return m_objValue.SmartWirte<T1>（ t1 ）;
    }
   private:
    reference_type m_objValue;
   };
   // 可以经由过程特化更多的Join_Hler来支撑更多的PIPE。
   template <>
   class Join_Hler< std::string >
   {
   public:
    typedef std::string         value_type;
    typedef value_type&         reference_type;
   public:
    Join_Hler（ reference_type str ） : m_strPipe（str）
    {
    }
   public:
template < typename T1 >
    inline std::string& SmartWrite（ const T1& t1 ）
    {
     m_strPipe.append（ Hero::GetString<T1>（t1） ）;
     return m_strPipe;
    }
   private:
    reference_type     m_strPipe;
   };
      请承诺我为人人介绍一下这个法式的设计思绪。首先，这是一个Join_Hler，从字面上来懂得就是拼接字符串的措置赏罚。为什么要操作模板来实现呢？重要的原因就是为了扩年夜性，因为我想我的法式最后不仅仅可以拼接到字符串中往还可以品接到其它合适我们接口的Pipe中往好比ostream。
       请人人留心上面红色部门的代码，很好懂得是吗？它会把我们吸收的一个模板类型操作字符串垫片转换成字符串，然后操作C++尺度库字符串的append函数拼接到已经存在的字符串上面往。
       好了，那么我们的可以实现近似这样的JoinString：
template < typename T1， typename T2 >
string JoinString（ const T1& t1， const T2& t2 ）
{
         string strResult;
          Join_Hler< std::string > pipe（strResult）;
         pipe.SmartWrite（ GetString<T1>（t1） ）;
         pipe.SmartWrite（ GetString<T2>（t2） ）;
        return strResult;
}
      OK了，是不是？好的，让我们往实现一个拜别支撑1个到20个参数的JoinString吧。是的，这可是一个体力活。当然，这并不是竣事的时辰，因为现在才进进了我们今天的主题。
       首先，要让人人知道一些关于宏的工具。＃＃是什么呢？它就是贴上的意思。＃代表了把后面的部门用引号括起来。此外，当一个宏碰着自己时将会竣事更调，这是为了防止宏无限递回的涌现。还有，宏更调不是一次性就能完成的，有可能需要编译器跑多趟~~![](http://pic.cnpetweb.com//M0/S1/1541-2.jpg)
      好了，这时辰是时辰上代码了，请看：
＃define FMT_TEMPLATE_PARAM_LIST（ z， n， text ） ， typename text＃＃n
＃define FMT_FUNCTION_PARAM_LIST（ z， n， text ） ， const text＃＃n& t＃＃n
＃define FMT_FUNCTION_CONTENT（ z， n， text ） 
her.SmartWrite（ text＃＃n ）;
＃define FMT_DEFINE_TOTAL_FUNCTION（ z， n， text ） 
   template < typename T， typename T0 BOOST_PP_REPEAT_FROM_TO（ 1， n， FMT_TEMPLATE_PARAM_LIST， text ） >
   inline T& WritePipe（ T& pipe， const T0& t0 BOOST_PP_REPEAT_FROM_TO（ 1， n， FMT_FUNCTION_PARAM_LIST， text ） ）
   { 
 Join_Hler<T> her（pipe）; 
    her.SmartWrite（ t0 ）; 
    BOOST_PP_REPEAT_FROM_TO（ 1， n， FMT_FUNCTION_CONTENT， t ） 
    return pipe; 
   }
   // 生成各类参数个数的名目化函数
   BOOST_PP_REPEAT_FROM_TO（ 1， 20， FMT_DEFINE_TOTAL_FUNCTION， T ）
＃undef FMT_TEMPLATE_PARAM_LIST
＃undef FMT_FUNCTION_PARAM_LIST
＃undef FMT_FUNCTION_CONTENT
＃undef FMT_DEFINE_TOTAL_FUNCTION
＃define FMT_TOSTRING_TEMPLATE_PARAM_LIST（ z， n， text ） ， typename text＃＃n
＃define FMT_TOSTRING_FUNCTION_PARAM_LIST（ z， n， text ） ， const text＃＃n& t＃＃n
＃define FMT_TOSTRING_FUNCTION_CONTENT（ z， n， text ） 
Hero::Fmt::WritePipe< std::string， text＃＃n >（ strResult， t＃＃n ）;
＃define FMT_TOSTRING_DEFINE_TOTAL_FUNCTION（ z， n， text ） 
template < typename T0 BOOST_PP_REPEAT_FROM_TO（ 1， n， FMT_TOSTRING_TEMPLATE_PARAM_LIST， text ） >
inline std::string JoinString（ const T0& t0 BOOST_PP_REPEAT_FROM_TO（ 1， n， FMT_TOSTRING_FUNCTION_PARAM_LIST， text ） ）
   { 
std::string strResult; 
   BOOST_PP_REPEAT_FROM_TO（ 0， n， FMT_TOSTRING_FUNCTION_CONTENT， T ） 
   return strResult; 
   }
   BOOST_PP_REPEAT_FROM_TO（ 1， 20， FMT_TOSTRING_DEFINE_TOTAL_FUNCTION， T ）
＃undef FMT_TOSTRING_DEFINE_TOTAL_FUNCTION
＃undef FMT_TOSTRING_FUNCTION_CONTENT
＃undef FMT_TOSTRING_FUNCTION_PARAM_LIST
＃undef FMT_TOSTRING_TEMPLATE_PARAM_LIST
       这个实现是基于boost的预措置赏罚库的，因为已经有好的轮子了，为什么我们还要造轮子呢？设计JoinString的思惟是这样的，JoinString是基于Fmt::WritePipe来实现的，而它们两个都是用宏来生成的。它们的分歧就是WritePipe的第一个参数是一个模板Pipe，这个Pipe只要知足Join_Hler下面的SmartWrite函数就可以了。有了这个Pipe，那么我们就可以把各类类型的数据转换成字符串写进往。
       这些实现的重点就是分成模板参数序列、函数参数序列以及函数体生成序列。如上红色的部门。操作宏就可以生成我们需要用手往实现的支撑1到20个不等个数的参数的函数。现实上，我们消费不到100行代码，而且很机动。
      此外的一个操作的例子：
namespace Hero
{
class LogManager : public Hero::StaticSingleton< LogManager >
{
   DEFINE_STATIC_SINGLETON_CLASS（ LogManager ）
private:
   LogManager（）
   {
    RegisterHle（ 
     Hero::IOutputHlerSP（ 
     new Hero::DebugPipe（）， 
     boost::mem_fn（&IOutputHler::Release） 
     ） 
     ）;
    RegisterHle（ 
     Hero::IOutputHlerSP（ 
     new Hero::ConsolePipe（）， 
     boost::mem_fn（&IOutputHler::Release） 
     ） 
     ）;
    SetDefaultFile（ "Hero.log" ）;
    m_spDefaultLogFilePipe.reset（
     new Hero::FilePipe（ m_strDefaultLogFile ），
     boost::mem_fn（&IOutputHler::Release） 
     ）;
    RegisterHle（ m_spDefaultLogFilePipe ）;
   }
   ~LogManager（）
   {
   }
public:
   inline void        RegisterHle（ Hero::IOutputHlerSP spHle ）
   {
    m_lstHles.push_back（ spHle ）;
   }
   inline void        UnRegisterHle（ Hero::IOutputHlerSP spHle ）
   {
    m_lstHles.remove（ spHle ）;
   }
   inline void        RemoveAll（）
   {
    m_lstHles.clear（）;
   }
   inline void        SetDefaultFile（ const std::string& strFile ）
   {
    m_strDefaultLogFile = strFile;
    if（ !m_spDefaultLogFilePipe ）
    {
     return;
    }
    boost::shared_ptr< Hero::FilePipe > spFilePipe = 
     boost::dynamic_pointer_cast< Hero::FilePipe >（ m_spDefaultLogFilePipe ）;
    if（ !spFilePipe ）
    {
     return;
    }
    if（ !boost::algithm::iequals（ spFilePipe->GetFileName（）， strFile ） ）
    {
     spFilePipe->Close（）;
     spFilePipe->Create（ strFile ）;
    }
   }
public:
   inline LogManager& Write（ const std::string& strValue， bool bAppendSpace = true ）
   {
    std::f_each（ 
     m_lstHles.begin（）， 
     m_lstHles.end（）， 
     boost::bind（ &IOutputHler::Write， _1， strValue， bAppendSpace ）
     ）;
    return *this;
   }
   inline LogManager& FmatWrite（ const char* strFmt， ... ）
   {
    va_list args;
    va_start（ args， strFmt ）;
    int iSize = _vscprintf（ strFmt， args ）;
    if（ iSize <= 0 ）
    {
     return *this;
    }
    Hero::AutoBuffer<char> buf（iSize+1）;
    vsprintf_s（ buf.GetBuffer（）， buf.GetSize（） ， strFmt， args ）;
    this->Write（ buf.GetBuffer（）， false ）;
    va_end（ args ）;
    return *this;
   }
   // SEH支撑函数
   inline int WriteSEHLog（ 
    const char* strFile，
    int iLine，
    const char* strFunction，
    LPEXCEPTION_POINTERS pep
    ）
   {
＃define HERO_ENUM_CASE（ Info ） 
case Info : 
    { 
    strInfo = ＃Info; 
    break; 
   }
    std::string strInfo;
    switch （ pep->ExceptionRecd->ExceptionCode ）
    {
     HERO_ENUM_CASE（ EXCEPTION_ACCESS_VIOLATION ）; 
     HERO_ENUM_CASE（ EXCEPTION_DATATYPE_MISALIGNMENT ）; 
     HERO_ENUM_CASE（ EXCEPTION_BREAKPOINT ）; 
     HERO_ENUM_CASE（ EXCEPTION_SINGLE_STEP ）; 
     HERO_ENUM_CASE（ EXCEPTION_ARRAY_BOUNDS_EXCEEDED ）; 
     HERO_ENUM_CASE（ EXCEPTION_FLT_DENORMAL_OPERAND ）; 
     HERO_ENUM_CASE（ EXCEPTION_FLT_DIVIDE_BY_ZERO ）; 
     HERO_ENUM_CASE（ EXCEPTION_FLT_INEXACT_RESULT ）; 
     HERO_ENUM_CASE（ EXCEPTION_FLT_INVALID_OPERATION ）; 
     HERO_ENUM_CASE（ EXCEPTION_FLT_OVERFLOW ）; 
     HERO_ENUM_CASE（ EXCEPTION_FLT_STACK_CHECK ）; 
     HERO_ENUM_CASE（ EXCEPTION_FLT_UNDERFLOW ）; 
     HERO_ENUM_CASE（ EXCEPTION_INT_DIVIDE_BY_ZERO ）; 
     HERO_ENUM_CASE（ EXCEPTION_INT_OVERFLOW ）; 
     HERO_ENUM_CASE（ EXCEPTION_PRIV_INSTRUCTION ）; 
     HERO_ENUM_CASE（ EXCEPTION_IN_PAGE_ERROR ）; 
     HERO_ENUM_CASE（ EXCEPTION_ILLEGAL_INSTRUCTION ）; 
     HERO_ENUM_CASE（ EXCEPTION_NONCONTINUABLE_EXCEPTION ）; 
     HERO_ENUM_CASE（ EXCEPTION_STACK_OVERFLOW ）; 
     HERO_ENUM_CASE（ EXCEPTION_INVALID_DISPOSITION ）; 
     HERO_ENUM_CASE（ EXCEPTION_GUARD_PAGE ）; 
     HERO_ENUM_CASE（ EXCEPTION_INVALID_HANDLE ）; 
    default:
     strInfo = std::string（ "SEH异常， 异常码:" ） + Hero::GetString（ pep->ExceptionRecd->ExceptionCode ）;
     break;
    }
    Fmt::WritePipe（ strInfo， " 短处信息："， Hero::GetLastErrString（ pep->ExceptionRecd->ExceptionCode&0 x0fffffff ） ）;
    this->SmartWrite（ "Hero Log: Message: "， strInfo， " " ）;    
    WriteCallStackLog（ NULL， pep->ContextRecd ）;
    return EXCEPTION_EXECUTE_HANDLER; 
   }
   inline void WriteCallStackLog（ LPCSTR pcszTitle， PCONTEXT pContext ） 
   { 
    std::string strInfo;
    if（ （ NULL != pcszTitle ） && （ 0 != *pcszTitle ） ） 
    { 
     // 有提示标题信息 
     strInfo += pcszTitle;    
    }
    strInfo += " 挪用仓库：";
    // 初始化dbghelp.dll 
    HANDLE hProcess = ::GetCurrentProcess（） ; // 当前过程句柄
    if（ !SymInitialize（ hProcess ， NULL， TRUE ） ） 
    { 
     // 初始化失踪败     
     strInfo += "SymInitialise（）挪用失踪败，可能PDB文件完整路径中存在中文";
     this->SmartWrite（ "Hero Log: Message: "， Hero::GetLastErrString（::GetLastErr（）） ）;
    } 
    else
    {
     STACKFRAME sf ; 
     ::memset（ &sf ， 0 ， sizeof（ STACKFRAME ） ） ; 
     sf.AddrPC.Offset    = pContext->Eip ; 
     sf.AddrPC.Mode      = AddrModeFlat ; 
     sf.AddrStack.Offset = pContext->Esp ; 
     sf.AddrStack.Mode   = AddrModeFlat ; 
     sf.AddrFrame.Offset = pContext->Ebp ; 
     sf.AddrFrame.Mode   = AddrModeFlat ; 
     DWORD machineType = IMAGE_FILE_MACHINE_I386 ; // 机械类型 
     HANDLE hThread = GetCurrentThread（） ; // 当前方程句柄
     // 输出挪用仓库信息 
     f（ ; ; ） 
     { 
      // 遍历挪用仓库 
      if（ !StackWalk（ machineType ， hProcess ， hThread ， &sf ， pContext ， 0 ， SymFunctionTableAccess ， SymGetModuleBase ， 0 ） ） 
      { 
       break ; 
      } 
      if（ sf.AddrFrame.Offset == 0 ） 
      { 
       break ; 
      }
      BYTE symbolBuffer[ sizeof（ SYMBOL_INFO ） + 1024 ] ; 
      ::memset（ symbolBuffer ， 0 ， sizeof（ symbolBuffer ） ） ; 
      PSYMBOL_INFO pSymbol = （ PSYMBOL_INFO ） symbolBuffer ; 
      pSymbol->SizeOfStruct = sizeof（ symbolBuffer ） ; 
      pSymbol->MaxNameLen   = 1024 ;
      // 从地址获得函数信息 
      DWORD64 symDisplacement = 0 ; 
      if（ SymFromAddr（ hProcess ， sf.AddrPC.Offset ， 0 ， pSymbol ） ） 
      {
       // 获得函数信息成功 
       // 获得函数地址文件和行号信息 
       IMAGEHLP_LINE lineInfo ; 
       ::memset（ &lineInfo ， 0 ， sizeof（ lineInfo ） ） ; 
       lineInfo.SizeOfStruct = sizeof（ IMAGEHLP_LINE ） ; 
       DWORD dwLineDisplacement = 0 ;
       if（ SymGetLineFromAddr（ hProcess ， sf.AddrPC.Offset ， &dwLineDisplacement ， &lineInfo ） ） 
       { 
        // 获得文件和行数信息成功 
        // 函数名 ， 文件名 : 行号 
        Fmt::WritePipe（ strInfo， pSymbol->Name， " "， lineInfo.FileName， "Line: "， lineInfo.LineNumber， " " ）;
       } 
       else 
       { 
        // 获得文件和行数信息失踪败 
        Fmt::WritePipe（ strInfo， " "， pSymbol->Name ）;
       }
      } 
      else 
      { 
       // 获得函数信息失踪败 
       // 很年夜的可能是该exe地址目录下没有响应的pdb文件 
       strInfo = "挪用::SymFromAddr（）失踪败，请搜检pdb文件是否存在!或者是否存在中文路径 ";
      } 
     } 
     // 清算dbghelp.dll 
     SymCleanup（ hProcess ） ; 
    }
    this->SmartWrite（ "Hero Log: Message: "， strInfo， " " ）; 
   } 
   // 生成具有拜别具有1到20个参数的SmartWrite
＃define HERO_TEMPLATE_PARAM_LIST（ z， n， text ） ， typename text＃＃n
＃define HERO_FUNCTION_PARAM_LIST（ z， n， text ） ， const text＃＃n& t＃＃n
＃define HERO_FUNCTION_CONTENT（ z， n， text ） this->Write（ GetString（ text＃＃n ）， false ）;
＃define HERO_SMART_WRITE_SUPPORT_MAX_PARAM_COUNT 20
＃define HERO_DEFINE_TOTAL_FUNCTION（ z， n， text ） 
   template < typename T0 BOOST_PP_REPEAT_FROM_TO（ 1， n， HERO_TEMPLATE_PARAM_LIST， text ） >
   inline LogManager& SmartWrite（ const T0& t0 BOOST_PP_REPEAT_FROM_TO（ 1， n， HERO_FUNCTION_PARAM_LIST， text ） ）
   { 
    BOOST_PP_REPEAT_FROM_TO（ 0， n， HERO_FUNCTION_CONTENT， t ） 
    return *this; 
   }
   // 生成模板函数参数个数从1到20个的SmartWrite成员函数
   BOOST_PP_REPEAT_FROM_TO（ 1， HERO_SMART_WRITE_SUPPORT_MAX_PARAM_COUNT， HERO_DEFINE_TOTAL_FUNCTION， T ）
＃undef HERO_TEMPLATE_PARAM_LIST
＃undef HERO_FUNCTION_PARAM_LIST
＃undef HERO_FUNCTION_CONTENT  
private:
   // 所有需要Debug动静的Hler都可以注册到manager中
   std::list< Hero::IOutputHlerSP >       m_lstHles;
   std::string                               m_strDefaultLogFile;
   Hero::IOutputHlerSP                    m_spDefaultLogFilePipe;
};
DECLARE_STATIC_SINGLETON_VALUE（ LogManager ）
＃if !defined（_DEBUG）
＃define HERO_USE_SIMPLE_LOG
＃endif
＃ifdef HERO_USE_SIMPLE_LOG
＃define HERO_SYSTEM_LOG 
＃else
＃define HERO_SYSTEM_LOG ， " File: "， __FILE__， " Function: "， __FUNCTION__， " Line: "， __LINE__， " "
＃endif
＃define LogFmtMsg（ fmt， ... ） 
Hero::LogManager::GetSingleton（）.SmartWrite（ Hero::GetTimeString（） HERO_SYSTEM_LOG ， " Msg:" ）;
Hero::LogManager::GetSingleton（）.FmatWrite（ fmt， __VA_ARGS__ ）; 
Hero::LogManager::GetSingleton（）.Write（ " "， false ）
＃ifdef HERO_DEBUG
＃define LogMsg（ ... ） 
Hero::LogManager::GetSingleton（）.SmartWrite（ Hero::GetTimeString（） HERO_SYSTEM_LOG ， " Msg: "， __VA_ARGS__ ）;
Hero::LogManager::GetSingleton（）.Write（ " "， false ）
＃else
＃define LogMsg（ ... ） 
Hero::LogManager::GetSingleton（）.SmartWrite（ Hero::GetTimeString（） HERO_SYSTEM_LOG ， " Msg:"， __VA_ARGS__ ）;
Hero::LogManager::GetSingleton（）.Write（ " "， false ）
＃endif
＃define LogLineBoundary 
Hero::LogManager::GetSingleton（）.Write（ "-----------------------------------------------------------------------"， true ）
＃define HERO_SEH_HANDLE 
Hero::LogManager::GetSingletonPtr（）->WriteSEHLog（ __FILE__， __LINE__， __FUNCTION__， GetExceptionInfmation（） ）
}
        这是一个日志系统，假如您对这一系列的法式感快乐喜爱的话请在此答复您的电子邮箱，我会收拾一份可直接操作的代码给人人发送过往。当然，您也可以选择这个：
       http://ishare.iask.sina.com.cn/f/6122708.html
       这是一份未批悛改的代码，您无法操作它直接编译，因为我有在项目中作编译预措置赏罚的，而预措置赏罚文件貌似没上传。不外代码仍是可以看的，看的是思惟。（PS：该代码并没有经由完整的测试，好比说是否能够支撑20个参数等等，其实年夜年夜都时辰参数是很少会跨越10个的，呵呵）。
         其实整篇文章只是简略色彩了宏生成代码的浸染，然则却很少说起它的事理，其一是因为我自己对此的懂得并没有达到“豁然宽年夜旷达”的高度，其二也是没有足够的时刻来做这件工作。那么，我信任这篇日志也不会完整没有浸染，至少它可以告诉我的伴侣们：哦，正本还有这种编程的方法。呵。
        宏是一把双刃剑，用到好会获得宏壮的利益，就像上面的演示一样。用得欠好，呵，你就等着愁闷吧~~关于Boost的预措置赏罚库，除了Boost的文档之外，还有此外的一本书的附录有提到，他就是《C++.Template.Metaprogramming》。
        关于宏的根本常识和一些根本技巧可以参考这两篇我转贴的好文章：
        代动生成-宏带来的奇技***巧:http://hi.baidu.com/_％E2d_％B7％B3_％DE％B2％C2％D2/blog/item/0a33517be6aaadfe0bd18730.html
       代动生成-宏递回思惟:http://hi.baidu.com/_％E2d_％B7％B3_％DE％B2％C2％D2/blog/item/a7f7a31948bbe070dbb4bd31.html
        信任看过这两篇文章之后，人人再回头看这篇日志会有分歧的感应感染。此外对于最匹面劈脸的名目化字符串的例子，其实有一个很富强的库可以做这个工作，它就是Matthew Wilson写的fastfmat，地址是：http://www.fastfmat.g/ 它需要Matthew Wilson的stlsoft库的支撑。效率很是高呵呵。![](http://pic.cnpetweb.com//M0/S1/1541-3.jpg)MRMY
