# STL:Numberic_limit() - wishchinYang的专栏 - CSDN博客
2013年11月26日 10:27:45[wishchin](https://me.csdn.net/wishchin)阅读数：1121
（1）：与MFC的兼容性
MFC程序过程中使用STL一些类编译出错，开始我认为是我写错了，放到Console Application里一切正常。
比如：
void CMyDialog::OnBnClickedButton1()
{
    double min=std::numeric_limits<double>::max();
    void *p=:![](http://www.itpub.net/static/image/smiley/qqbiaoqin/yiw.gif)perator
 new(count);
}
若在Console中根本没问题。但在MFC中numeric_limits错误提示:
Error 2 error C2589: '(' : illegal token on right side of '::' 
operator new错误提示：
Error 1 error C2665: 'operator new' : none of the 5 overloads could convert all the argument types 
解决方案：
跟Windows中定义的宏想混淆了
可以用括号改变语句的顺序，强制转换为stl中的函数名
double min=（std::numeric_limits<double>::max）();
（2）： 小例展示numeric_limits的基本用法：
[cpp][view
 plain](http://blog.csdn.net/netrookie/article/details/5530578#)[copy](http://blog.csdn.net/netrookie/article/details/5530578#)
- #include <limits>
- #include <iostream>
- **using****namespace** std;  
- 
- **int** main() {  
-     cout << boolalpha;  
- 
-     cout << "max(short): " << numeric_limits<**short**>::max() << endl;  
-     cout << "min(short): " << numeric_limits<**short**>::min() << endl;  
- 
-     cout << "max(int): " << numeric_limits<**int**>::max() << endl;  
-     cout << "min(int): " << numeric_limits<**int**>::min() << endl;  
- 
-     cout << "max(long): " << numeric_limits<**long**>::max() << endl;  
-     cout << "min(long): " << numeric_limits<**long**>::min() << endl;  
- 
-     cout << endl;  
- 
-     cout << "max(float): " << numeric_limits<**float**>::max() << endl;  
-     cout << "min(float): " << numeric_limits<**float**>::min() << endl;  
- 
-     cout << "max(double): " << numeric_limits<**double**>::max() << endl;  
-     cout << "min(double): " << numeric_limits<**double**>::min() << endl;  
- 
-     cout << "max(long double): " << numeric_limits<**long****double**>::max() << endl;  
-     cout << "min(long double): " << numeric_limits<**long****double**>::min() << endl;  
- 
-     cout << endl;  
- 
-     cout << "is_signed(char): "
-         << numeric_limits<**char**>::is_signed << endl;  
-     cout << "is_specialized(string): "
-         << numeric_limits<string>::is_specialized << endl;  
- }  
我机器上的运行结果：
[c-sharp][view
 plain](http://blog.csdn.net/netrookie/article/details/5530578#)[copy](http://blog.csdn.net/netrookie/article/details/5530578#)
- max(**short**): 32767  
- min(**short**): -32768  
- max(**int**): 2147483647  
- min(**int**): -2147483648  
- max(**long**): 2147483647  
- min(**long**): -2147483648  
- 
- max(**float**): 3.40282e+038  
- min(**float**): 1.17549e-038  
- max(**double**): 1.79769e+308  
- min(**double**): 2.22507e-308  
- max(**long****double**): 1.79769e+308  
- min(**long****double**): 2.22507e-308  
- 
- is_signed(**char**): **true**
- is_specialized(**string**): **false**
- 请按任意键继续. . .  
关于为什么float的最小值竟然是正的？我也存在疑问，从结果中，我们看出，min返回的是float型别可以表示的最小的正值，
而不是最小的float数。从这个例子中，我们差不多了解到numeric_limits的基本用法。
3. 基本成员函数
我以float类型来展示：
[c-sharp][view
 plain](http://blog.csdn.net/netrookie/article/details/5530578#)[copy](http://blog.csdn.net/netrookie/article/details/5530578#)
- #include <limits>
- #include <iostream>
- **using****namespace** std;  
- 
- **int** main() {  
-     cout << boolalpha;  
- // 可以表示的最大值
-     cout << "max(float): " << numeric_limits<**float**>::max() << endl;  
- // 可以表示的大于0的最小值，其他类型的实现或与此不同
-     cout << "min(float): " << numeric_limits<**float**>::min() << endl;  
- // 标准库是否为其实现了特化
-     cout << "is_specialized(float): " << numeric_limits<**float**>::is_specialized << endl;  
- // 是否是有符号的，即可以表示正负值
-     cout << "is_signed(float): " << numeric_limits<**float**>::is_signed << endl;  
- // 不否是整形的
-     cout << "is_integer(float): " << numeric_limits<**float**>::is_integer << endl;  
- // 是否是精确表示的
-     cout << "is_exact(float): " << numeric_limits<**float**>::is_exact << endl;  
- // 是否存在大小界限
-     cout << "is_bounded(float): " << numeric_limits<**float**>::is_bounded << endl;  
- // 两个比较大的数相加而不会溢出，生成一个较小的值
-     cout << "is_modulo(float): " << numeric_limits<**float**>::is_modulo << endl;  
- // 是否符合某某标准
-     cout << "is_iec559(float): " << numeric_limits<**float**>::is_iec559 << endl;  
- // 不加+-号可以表示的位数
-     cout << "digits(float): " << numeric_limits<**float**>::digits << endl;  
- // 十进制数的个数
-     cout << "digits10(float): " << numeric_limits<**float**>::digits10 << endl;  
- // 一般基数为2
-     cout << "radix(float): " << numeric_limits<**float**>::radix << endl;  
- // 以2为基数的最小指数
-     cout << "min_exponent(float): " << numeric_limits<**float**>::min_exponent << endl;  
- // 以2为基数的最大指数
-     cout << "max_exponent(float): " << numeric_limits<**float**>::max_exponent << endl;  
- // 以10为基数的最小指数
-     cout << "min_exponent10(float): " << numeric_limits<**float**>::min_exponent10 << endl;  
- // 以10为基数的最大指数
-     cout << "max_exponent10(float): " << numeric_limits<**float**>::max_exponent10 << endl;  
- // 1值和最接近1值的差距
-     cout << "epsilon(float): " << numeric_limits<**float**>::epsilon() << endl;  
- // 舍入方式
-     cout << "round_style(float): " << numeric_limits<**float**>::round_style << endl;  
- }  
运行结果：
[cpp][view
 plain](http://blog.csdn.net/netrookie/article/details/5530578#)[copy](http://blog.csdn.net/netrookie/article/details/5530578#)
- max(**float**): 3.40282e+038  
- min(**float**): 1.17549e-038  
- is_specialized(**float**): **true**
- is_signed(**float**): **true**
- is_integer(**float**): **false**
- is_exact(**float**): **false**
- is_bounded(**float**): **true**
- is_modulo(**float**): **false**
- is_iec559(**float**): **true**
- digits(**float**): 24  
- digits10(**float**): 6  
- radix(**float**): 2  
- min_exponent(**float**): -125  
- max_exponent(**float**): 128  
- min_exponent10(**float**): -37  
- max_exponent10(**float**): 38  
- epsilon(**float**): 1.19209e-007  
- round_style(**float**): 1  
- 请按任意键继续. . .
（3）：关于STL:Numberic_limit()
原文链接：[http://www.cplusplus.com/reference/limits/numeric_limits/](http://www.cplusplus.com/reference/limits/numeric_limits/)
class template
<limits>
# std::numeric_limits
template <class T> numeric_limits;
Numeric limits type
Provides information about the properties of [arithmetic types](http://www.cplusplus.com/is_arithmetic) (either integral or floating-point) in the specific platform
 for which the library compiles.
This class template is specialized for every *fundamental arithmetic type*, with its members describing the properties of type T. This template shall not be specialized for any other type.
### Template parameters
- 
T
A type.
If this is a *fundamental arithmetic type*, the members of the class describe its properties.
### Template instantiations
- [C++98](http://www.cplusplus.com/reference/limits/numeric_limits/)
- [C++11](http://www.cplusplus.com/reference/limits/numeric_limits/)
- [](http://www.cplusplus.com/site/versions/)
`long double`
For any other type, its default definition is used.
Members that produce a value of type T are member functions, while members of specific types are static member constants:
### Members
|member|type|property|
|----|----|----|
|is_specialized|`bool`|`true` for all [arithmetic types](http://www.cplusplus.com/is_arithmetic) (i.e., those for which numeric_limits is specialized).`false` for all other types.|
|min()|T|Minimum finite value.For floating types with denormalization (variable number of exponent bits): minimum positive normalized value.Equivalent to [CHAR_MIN](http://www.cplusplus.com/climits), [SCHAR_MIN](http://www.cplusplus.com/climits), [SHRT_MIN](http://www.cplusplus.com/climits), [INT_MIN](http://www.cplusplus.com/climits), [LONG_MIN](http://www.cplusplus.com/climits), [LLONG_MIN](http://www.cplusplus.com/climits), [FLT_MIN](http://www.cplusplus.com/cfloat),[DBL_MIN](http://www.cplusplus.com/cfloat), [LDBL_MIN](http://www.cplusplus.com/cfloat) or `0`, depending on type.|
|max()|T|Maximum finite value.Equivalent to [CHAR_MAX](http://www.cplusplus.com/climits), [SCHAR_MAX](http://www.cplusplus.com/climits), [UCHAR_MAX](http://www.cplusplus.com/climits), [SHRT_MAX](http://www.cplusplus.com/climits), [USHRT_MAX](http://www.cplusplus.com/climits), [INT_MAX](http://www.cplusplus.com/climits), [UINT_MAX](http://www.cplusplus.com/climits),[LONG_MAX](http://www.cplusplus.com/climits), [ULONG_MAX](http://www.cplusplus.com/climits), [LLONG_MAX](http://www.cplusplus.com/climits), [ULLONG_MAX](http://www.cplusplus.com/climits), [UINT_LEAST16_MAX](http://www.cplusplus.com/cstdint), [UINT_LEAST32_MAX](http://www.cplusplus.com/cstdint), [FLT_MAX](http://www.cplusplus.com/cfloat),[DBL_MAX](http://www.cplusplus.com/cfloat) or [LDBL_MAX](http://www.cplusplus.com/cfloat), depending on type.|
|lowest()|T|Minimum finite value. (since C++11)For integral types: the same as min().For floating-point types: implementation-dependent; generally, the negative ofmax().|
|digits|int|For integer types: number of non-sign bits (radix base digits) in the representation.For floating types: number of digits (in radix base) in the mantissa (equivalent to[FLT_MANT_DIG](http://www.cplusplus.com/cfloat), [DBL_MANT_DIG](http://www.cplusplus.com/cfloat) or [LDBL_MANT_DIG](http://www.cplusplus.com/cfloat)).|
|digits10|`int`|Number of digits (in decimal base) that can be represented without change.Equivalent to [FLT_DIG](http://www.cplusplus.com/cfloat), [DBL_DIG](http://www.cplusplus.com/cfloat) or [LDBL_DIG](http://www.cplusplus.com/cfloat) for floating types.|
|is_signed|`bool`|`true` if type is signed.|
|is_integer|`bool`|`true` if type is integer.|
|is_exact|`bool`|`true` if type uses exact representations.|
|radix|`int`|For integer types: base of the representation.For floating types: base of the exponent of the representation (equivalent to[FLT_RADIX](http://www.cplusplus.com/cfloat)).|
|epsilon()|T|Machine epsilon (the difference between 1 and the least value greater than 1 that is representable).Equivalent to [FLT_EPSILON](http://www.cplusplus.com/cfloat), [DBL_EPSILON](http://www.cplusplus.com/cfloat) or [LDBL_EPSILON](http://www.cplusplus.com/cfloat) for floating types.|
|round_error()|T|Measure of the maximum rounding error.|
|min_exponent|`int`|Minimum negative integer value such that radix raised to `(min_exponent-1)`generates a normalized floating-point number.Equivalent to [FLT_MIN_EXP](http://www.cplusplus.com/cfloat), [DBL_MIN_EXP](http://www.cplusplus.com/cfloat) or [LDBL_MIN_EXP](http://www.cplusplus.com/cfloat) for floating types.|
|min_exponent10|`int`|Minimum negative integer value such that 10 raised to that power generates a normalized floating-point number.Equivalent to [FLT_MIN_10_EXP](http://www.cplusplus.com/cfloat), [DBL_MIN_10_EXP](http://www.cplusplus.com/cfloat) or [LDBL_MIN_10_EXP](http://www.cplusplus.com/cfloat) for floating types.|
|max_exponent|`int`|Maximum integer value such that radix raised to `(max_exponent-1)` generates a representable finite floating-point number.Equivalent to [FLT_MAX_EXP](http://www.cplusplus.com/cfloat), [DBL_MAX_EXP](http://www.cplusplus.com/cfloat) or [LDBL_MAX_EXP](http://www.cplusplus.com/cfloat) for floating types.|
|max_exponent10|`int`|Maximum integer value such that 10 raised to that power generates a normalized finite floating-point number.Equivalent to [FLT_MAX_10_EXP](http://www.cplusplus.com/cfloat), [DBL_MAX_10_EXP](http://www.cplusplus.com/cfloat) or [LDBL_MAX_10_EXP](http://www.cplusplus.com/cfloat) for floating types.|
|has_infinity|`bool`|`true` if the type has a representation for positive infinity.|
|has_quiet_NaN|`bool`|`true` if the type has a representation for a quiet (non-signaling) "Not-a-Number".|
|has_signaling_NaN|`bool`|`true` if the type has a representation for a signaling "Not-a-Number".|
|has_denorm|[float_denorm_style](http://www.cplusplus.com/float_denorm_style)|Denormalized values (representations with a variable number of exponent bits). A type may have any of the following enum values:[denorm_absent](http://www.cplusplus.com/float_denorm_style), if it does not allow denormalized values.[denorm_present](http://www.cplusplus.com/float_denorm_style), if it allows denormalized values.[denorm_indeterminate](http://www.cplusplus.com/float_denorm_style), if indeterminate at compile time.|
|has_denorm_loss|`bool`|`true` if a *loss of accuracy* is detected as a denormalization loss, rather than an inexact result.|
|infinity()|T|Representation of *positive infinity*, if available.|
|quiet_NaN()|T|Representation of *quiet* (non-signaling) *"Not-a-Number"*, if available.|
|signaling_NaN()|T|Representation of *signaling "Not-a-Number"*, if available.|
|denorm_min()|T|Minimum positive denormalized value.For types not allowing denormalized values: same as `min()`.|
|is_iec559|`bool`|`true` if the type adheres to IEC-559 / IEEE-754 standard.An IEC-559 type always has has_infinity, has_quiet_NaN and has_signaling_NaN set to `true`; And infinity, quiet_NaN and signaling_NaN return some non-zero value.|
|is_bounded|`bool`|`true` if the set of values represented by the type is finite.|
|is_modulo|`bool`|`true` if the type is modulo. A type is *modulo* if it is possible to add two positive numbers and have a result that wraps around to a third number that is less.|
|traps|`bool`|`true` if trapping is implemented for the type.|
|tinyness_before|`bool`|`true` if tinyness is detected before rounding.|
|round_style|[float_round_style](http://www.cplusplus.com/float_round_style)|Rounding style. A type may have any of the following enum values:[round_toward_zero](http://www.cplusplus.com/float_round_style), if it rounds toward zero.[round_to_nearest](http://www.cplusplus.com/float_round_style), if it rounds to the nearest representable value.[round_toward_infinity](http://www.cplusplus.com/float_round_style), if it rounds toward infinity.[round_toward_neg_infinity](http://www.cplusplus.com/float_round_style), if it rounds toward negative infinity.[round_indeterminate](http://www.cplusplus.com/float_round_style), if the rounding style is indeterminable at compile time.|
**下面是参数的解释**
|digits10|返回目标类型在十进制下可以表示的最大位数|
|----|----|
|epsilon|返回目标数据类型能表示的最逼近1的正数和1的差的绝对值|
|has_denorm|测试目标类型是不是可以非规范化表示示|
|has_denorm_loss|测试所有类型是不是能测出因为非规范化而造成的精度损失（不是因为结果本身的不精确）|
|has_infinity|测试目标类型是不是能表示无限（比如被0除，或者其他一些情况）|
|has_quiet_NaN|检查目标类型是不是支持安静类型的NaN|
|has_signaling_NaN|检查目标类型是不是支持信号类型的NaN|
|infinity|检查目标类型的无限类型（如果支持无限表示）|
|is_bounded|检查目标类型的取值是否有限|
|is_exact|测试目标类型的计算结果是不是**不会**造成舍入误差（比如float是0）|
|is_iec559|测试目标类型是不是符合IEC559标准|
|is_integer|测试目标类型是不是可以用整型来表示（比如char是1，float是0）|
|is_modulo|Tests if a type has a modulo representation.|
|is_signed|测试目标类型是否是带符号的|
|is_specialized|测试目标类型是不是在**numeric_limits **.模板类中有特殊定义|
|max|返回可取的有限最大值|
|max_exponent|Returns the maximum positive integral exponent that the floating-point type can represent as a finite value when a base of radix is raised to that power.|
|max_exponent10|Returns the maximum positive integral exponent that the floating-point type can represent as a finite value when a base of ten is raised to that power.|
|min|返回可取的最小值（规范化）|
|min_exponent|Returns the maximum negative integral exponent that the floating-point type can represent as a finite value when a base of radix is raised to that power.|
|min_exponent10|Returns the maximum negative integral exponent that the floating-point type can represent as a finite value when a base of ten is raised to that power.|
|quiet_NaN|返回目标类型的安静NAN的表示|
|radix|Returns the integral base, referred to as radix, used for the representation of a type.|
|round_error|返回目标类型的最大可能的舍入误差|
|round_style|Returns a value that describes the various methods that an implementation can choose for rounding a floating-point value to an integer value.|
|signaling_NaN|返回目标类型关于信号NAN的表示|
|tinyness_before|测试目标类型是不是能测定出微小的舍入误差|
|traps|Tests whether trapping that reports on arithmetic exceptions is implemented for a type.|
For all types that are not *fundamental arithmetic types*, the default template definition is used:
- [C++98](http://www.cplusplus.com/reference/limits/numeric_limits/)
- [C++11](http://www.cplusplus.com/reference/limits/numeric_limits/)
- [](http://www.cplusplus.com/site/versions/)
|```12345678910111213141516171819202122232425262728293031323334353637```|```template <class T> class numeric_limits {public:  static const bool is_specialized = false;  static T min() throw();  static T max() throw();  static const int  digits = 0;  static const int  digits10 = 0;  static const bool is_signed = false;  static const bool is_integer = false;  static const bool is_exact = false;  static const int radix = 0;  static T epsilon() throw();  static T round_error() throw();  static const int  min_exponent = 0;  static const int  min_exponent10 = 0;  static const int  max_exponent = 0;  static const int  max_exponent10 = 0;  static const bool has_infinity = false;  static const bool has_quiet_NaN = false;  static const bool has_signaling_NaN = false;  static const float_denorm_style has_denorm = denorm_absent;  static const bool has_denorm_loss = false;  static T infinity() throw();  static T quiet_NaN() throw();  static T signaling_NaN() throw();  static T denorm_min() throw();  static const bool is_iec559 = false;  static const bool is_bounded = false;  static const bool is_modulo = false;  static const bool traps = false;  static const bool tinyness_before = false;  static const float_round_style round_style = round_toward_zero;};```|
### Example
|```12345678910111213```|```// numeric_limits example#include <iostream>     // std::cout#include <limits>       // std::numeric_limitsint main () {  std::cout << std::boolalpha;  std::cout << "Minimum value for int: " << std::numeric_limits<int>::min() << '\n';  std::cout << "Maximum value for int: " << std::numeric_limits<int>::max() << '\n';  std::cout << "int is signed: " << std::numeric_limits<int>::is_signed << '\n';  std::cout << "Non-sign bits in int: " << std::numeric_limits<int>::digits << '\n';  std::cout << "int has infinity: " << std::numeric_limits<int>::has_infinity << '\n';  return 0;}```|
Possible output:
Minimum value for int: -2147483648
Maximum value for int: 2147483647
int is signed: true
Non-sign bits in int: 31
int has infinity: false
