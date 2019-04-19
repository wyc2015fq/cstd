# iOS数据类型简介及NSLog打印输出 - xqhrs232的专栏 - CSDN博客
2019年01月04日 16:42:58[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：147
个人分类：[Mac OS/i OS技术](https://blog.csdn.net/xqhrs232/article/category/1252162)
原文地址::[https://blog.csdn.net/tanhj/article/details/72526901](https://blog.csdn.net/tanhj/article/details/72526901)
相关文章
1、iOS数据类型----[https://www.jianshu.com/p/c7dbc5b9ac10](https://www.jianshu.com/p/c7dbc5b9ac10)
## 1. 数据类型简介及输出
### (1) 数据类型简介 
**数据类型简介** : Object - C 数据类型 分为 基本数据类型, 构造类型 和 指针类型;
-- **基本数据类型 **: 整型, 字符型, 浮点型 (float 和 double), 枚举型;
-- **构造类型** : 数组类型, 结构体类型, 共用体类型;
-- **指针类型** : 最终要的数据类型, 所有的系统类, 自定义类都是指针;
-- **空类型 **: 空类型只有一个值 nil, 该类型没有名称, 因此没有空类型的变量, 变量不能转换成空类型, 但是空类型可以转换成任何引用类型;
### (2) 数据类型输出
**NSLog()简介** : NSLog 是 Foundation 框架中功能强大的函数, 可以输出任何类型的数据;
-- **占位符** : NSLog() 函数中各式与 C 语言中大致相似, 第一个参数是加了 @ 的字符串, 后面的参数是用于替换占位符的;
**整型占位符说明** : 
-- **%d** : 十进制整数, 正数无符号, 负数有 "-" 符号;
-- **%o** : 八进制无符号整数, 没有 0 前缀;
-- **%x** : 十六进制无符号整数, 没有 0x 前缀;
--** %u** : 十进制无符号整数;
-- **%zd**     NSInteger
--  **%tu  **无符号NSUInteger
**字符占位符说明** : 
-- **%c** : 单个字符输出;
-- **%s** : 输出字符串;
**浮点占位符说明** : 
-- **%f **: 以小数形式输出浮点数, 默认 6 位小数;
-- **%e **: 以指数形式输出浮点数, 默认 6 位小数;
-- **%g** : 自动选择 %e 或者 %f 各式;
**其它形式占位符** :
-- **%p **: 输出十六进制形式的指针地址;
-- **%@** : 输出 Object-C 对象;
**占位符附加字符** : 
-- **l** : 在整型 和 浮点型占位符之前, %d %o %x %u %f %e %g 代表长整型 和 长字符串;
-- **n(任意整数)** : %8d 代表输出8位数字, 输出总位数;
-- **.n** : 浮点数 **限制小数位数**, %5.2f 表示 5位数字 2位小数, 字符串 **截取字符个数**;
-- **- **: 字符左对齐;
**示例代码** : 
**[objc]**[view plain](http://blog.csdn.net/shulianghan/article/details/38544659#)[copy](http://blog.csdn.net/shulianghan/article/details/38544659#)
- #import <Foundation/Foundation.h>  
- 
- **int** main(**int** argc, **char****char** **argv)  
- {  
- **@autoreleasepool**{  
- **int** a = 38;  
- **long** b = 38;  
- **double** c = 3.8;  
- **double** d = 2.3;  
- **char****char** * oct_c = "Octopus";  
- **NSString** *oct_oc = @"Octopus"; /* 定义字符串, 在字符串前加上 @ 说明是Ovject-C变量, 值赋给一个 NSString 指针 */  
- **NSDate** *date = [[NSDate alloc]init];  
- 
-         NSLog(@"%d 十进制整型", a);  
-         NSLog(@"%5d 五位十进制整型", a);  
-         NSLog(@"%-5d 左对齐的五位十进制整型", a);  
-         NSLog(@"%o 八进制整型", a);  
-         NSLog(@"%x 十六进制整型\n", a);  
- 
-         NSLog(@"%ld 长整型", b);  
-         NSLog(@"%lx 十六进制长整型\n", b);  
- 
-         NSLog(@"%f 小数形式输出浮点数", c);  
-         NSLog(@"%e 指数形式输出浮点数", c);  
-         NSLog(@"%g 以最简短形式输出浮点数", c);  
-         NSLog(@"%5f 以五位小数形式输出浮点数", c);  
-         NSLog(@"%5.3f 小数行书输出, 一共五位, 小数3位\n", c);  
- 
-         NSLog(@"%lf 小数形式输出长浮点数", d);  
-         NSLog(@"%le 指数形式输出长浮点数", d);  
-         NSLog(@"%lg 以最短形式输出长浮点数", d);  
-         NSLog(@"%5lf 5位小数形式输出长浮点数", d);  
-         NSLog(@"%5.3lf 5位小数形式输出长浮点数, 其中3位是小数\n", d);  
- 
-         NSLog(@"%s 输出C字符串", oct_c);  
-         NSLog(@"%@ 输出Object-C字符串\n", oct_oc);  
- 
-         NSLog(@"%@ 输出日期", date);  
- 
- 
- 
-     }  
- }  
执行结果 : 
**[plain]**[view plain](http://blog.csdn.net/shulianghan/article/details/38544659#)[copy](http://blog.csdn.net/shulianghan/article/details/38544659#)
- octopus-2:ios octopus$ clang -fobjc-arc -framework Foundation 02-NSLogDemo.m   
- octopus-2:ios octopus$   
- octopus-2:ios octopus$ ./a.out   
- 2014-08-16 19:32:44.210 a.out[1300:507] 38 十进制整型  
- 2014-08-16 19:32:44.212 a.out[1300:507]    38 五位十进制整型  
- 2014-08-16 19:32:44.213 a.out[1300:507] 38    左对齐的五位十进制整型  
- 2014-08-16 19:32:44.214 a.out[1300:507] 46 八进制整型  
- 2014-08-16 19:32:44.214 a.out[1300:507] 26 十六进制整型  
- 2014-08-16 19:32:44.215 a.out[1300:507] 38 长整型  
- 2014-08-16 19:32:44.215 a.out[1300:507] 26 十六进制长整型  
- 2014-08-16 19:32:44.216 a.out[1300:507] 3.800000 小数形式输出浮点数  
- 2014-08-16 19:32:44.216 a.out[1300:507] 3.800000e+00 指数形式输出浮点数  
- 2014-08-16 19:32:44.217 a.out[1300:507] 3.8 以最简短形式输出浮点数  
- 2014-08-16 19:32:44.217 a.out[1300:507] 3.800000 以五位小数形式输出浮点数  
- 2014-08-16 19:32:44.218 a.out[1300:507] 3.800 小数行书输出, 一共五位, 小数3位  
- 2014-08-16 19:32:44.218 a.out[1300:507] 2.300000 小数形式输出长浮点数  
- 2014-08-16 19:32:44.219 a.out[1300:507] 2.300000e+00 指数形式输出长浮点数  
- 2014-08-16 19:32:44.219 a.out[1300:507] 2.3 以最短形式输出长浮点数  
- 2014-08-16 19:32:44.219 a.out[1300:507] 2.300000 5位小数形式输出长浮点数  
- 2014-08-16 19:32:44.220 a.out[1300:507] 2.300 5位小数形式输出长浮点数, 其中3位是小数  
- 2014-08-16 19:32:44.220 a.out[1300:507] Octopus 输出C字符串  
- 2014-08-16 19:32:44.220 a.out[1300:507] Octopus 输出Object-C字符串  
- 2014-08-16 19:32:44.226 a.out[1300:507] 2014-08-16 11:32:44 +0000 输出日期  
## 2. 整型
### (1) 整型类型介绍
**四种整型 **: 
-- **short int **: 短整型, 占16位, mac 上占 2 字节, [iOS](http://lib.csdn.net/base/1) 上占 2 字节, 范围 -32768(-2^15) ~ 32767(2^15 - 1), 3万;
-- **int** : 整型, 占32位, mac 上占 4 字节, ios 上占 4 字节, 范围 -2147483648(-2^31) ~ 2147483647(2^31 - 1), 21亿;
-- **long int **: 长整型, 占64位, mac 上占 8 字节, ios 上占 4 字节, (-2^63) ~ (2^63 - 1), 922亿亿;
-- **long long** : 占64位, mac 上占 8 字节, ios 上占 8 字节;
**整数赋值 **: 整型赋值时注意范围, 如果超出赋值范围, 会出现警告, 数据也会丢失;
-- **示例程序** : 
**[objc]**[view plain](http://blog.csdn.net/shulianghan/article/details/38544659#)[copy](http://blog.csdn.net/shulianghan/article/details/38544659#)
- /************************************************************************* 
-     > File Name: a.m 
-     > Author: octopus 
-     > Mail: [octopus_truth.163.com](/)
-     > Created Time: 日  8/17 00:44:19 2014 
-  ************************************************************************/  
- 
- #import <Foundation/Foundation.h>  
- 
- **int** main(**int** argc, **char****char** * argv[])  
- {  
- **@autoreleasepool** {  
- **short****int** a = 50000; /* short int 范围是 正负3万, 因此超出范围报错 */  
- **int** a = 50000;  
- 
-         NSLog(@"short int a = %d", a);  
-         NSLog(@"int a = %d", a);  
-     }  
- }  
-- **执行结果** : 
**[plain]**[view plain](http://blog.csdn.net/shulianghan/article/details/38544659#)[copy](http://blog.csdn.net/shulianghan/article/details/38544659#)
- octopus-2:~ octopus$ clang -fobjc-arc -framework Foundation a.m   
- a.m:14:7: error: redefinition of 'a' with a different type: 'int' vs 'short'  
-                 int a = 50000;  
-                     ^  
- a.m:13:13: note: previous definition is here  
-                 short int a = 50000;  
-                           ^  
- 1 error generated.  
### (2) 进制介绍
**八进制 十六进制赋值 **: 八进制由 "0" 开头, 十六进制由 "0x" 或者 "0X" 开头;
-- **示例程序 **: 
**[objc]**[view plain](http://blog.csdn.net/shulianghan/article/details/38544659#)[copy](http://blog.csdn.net/shulianghan/article/details/38544659#)
- #import <Foundation/Foundation.h>  
- 
- **int** main(**int** argc, **char****char** **argv)  
- {  
- **@autoreleasepool**
-     {  
- **int** a = 010;    /* 八进制赋值 */  
- **int** b = 0x10;   /* 十六进制赋值 */  
- **int** c = 10;     /* 正常十进制赋值 */  
- 
-         NSLog(@"a = %d", a);  
-         NSLog(@"b = %d", b);  
-         NSLog(@"c = %d", c);  
- 
-         NSLog(@". . . . . ");  
- 
-         NSLog(@"a = %o", a);  
-         NSLog(@"b = %x", b);  
-     }  
- **return** 0;  
- }  
-- **执行结果** : 
**[plain]**[view plain](http://blog.csdn.net/shulianghan/article/details/38544659#)[copy](http://blog.csdn.net/shulianghan/article/details/38544659#)
- octopus-2:ios octopus$ clang -fobjc-arc -framework Foundation 03-IntDemo1.m   
- octopus-2:ios octopus$ ./a.out   
- 2014-08-17 00:58:50.513 a.out[1726:507] a = 8  
- 2014-08-17 00:58:50.516 a.out[1726:507] b = 16  
- 2014-08-17 00:58:50.516 a.out[1726:507] c = 10  
- 2014-08-17 00:58:50.516 a.out[1726:507] . . . . .   
- 2014-08-17 00:58:50.517 a.out[1726:507] a = 10  
- 2014-08-17 00:58:50.517 a.out[1726:507] b = 10  
### (3) 无符号整型
**无符号整型 **: 无符号整型 第一位 不是符号位, 范围比原来要大, 例如 short int 范围是 正负3万, unsigned short int 范围是 0到6万, 将5万赋值给 该类型不报错;
-- **示例程序** : 
**[objc]**[view plain](http://blog.csdn.net/shulianghan/article/details/38544659#)[copy](http://blog.csdn.net/shulianghan/article/details/38544659#)
- /************************************************************************* 
-     > File Name: 03-IntDemo2.m 
-     > Author: octopus 
-     > Mail: [octopus_truth.163.com](/)
-     > Created Time: 日  8/17 01:02:39 2014 
-  ************************************************************************/  
- 
- #import <Foundation/Foundation.h>  
- 
- **int** main(**int** argc, **char****char** * argv[])  
- {  
- **@autoreleasepool** {  
-         unsigned **short****int** a = 50000;  
- 
-         NSLog(@"a = %d", a);  
-     }  
- }  
-- **执行结果** : 
**[plain]**[view plain](http://blog.csdn.net/shulianghan/article/details/38544659#)[copy](http://blog.csdn.net/shulianghan/article/details/38544659#)
- octopus-2:ios octopus$ clang -fobjc-arc -framework Foundation 03-IntDemo2.m   
- octopus-2:ios octopus$ ./a.out   
- 2014-08-17 01:03:35.508 a.out[1745:507] a = 50000  
## 3. 字符型
**字符型数据简介** : 
-- **单个字符表示** : 使用 '' 将单个字符引起来, 便是字符常量, 如 'a', 'b' 等;
-- **转义字符表示** : 使用转义符号 \ 来指定字符, 如 '\n' 等;
-- **字符占用空间大小** : 每个字符占用一个字节, 因此 Object-C 不支持中文字符, 因为中文字符都是占 2 ~ 3 个字节;
**转义字符 **: 分为特殊空白字符 和 区别 单双引号 和 反斜线的转义;
-- **退格符** : '\b' ;
-- **换行符** : '\n' ;
-- **回车符** : '\r' ;
-- **制表符** : '\t' ;
-- **双引号** : '\"' ;
-- **单引号** : '\'' ;
-- **反斜线** : '\\' ;
**int 与 char 类型关系** : char 类型可以当做 8 位无符号整数, 取值范围 0 ~ 255;
-- **int 赋值给 char** : 将 int 类型赋值给 char 变量的时候, 会自动将类型转为 char 类型;
**char 类型 示例** : 
**[objc]**[view plain](http://blog.csdn.net/shulianghan/article/details/38544659#)[copy](http://blog.csdn.net/shulianghan/article/details/38544659#)
- /************************************************************************* 
-     > File Name: 05-charDemo.m 
-     > Author: octopus 
-     > Mail: [octopus_truth.163.com](/)
-     > Created Time: 二  8/19 21:10:37 2014 
-  ************************************************************************/  
- 
- #import <Foundation/Foundation.h>  
- 
- **int** main(**int** argc, **char****char** * argv[])  
- {  
- **@autoreleasepool** {  
- **char** a = 'a';  
- **char** b = 100;  
- **char** c = '\n';  
- 
-         NSLog(@"a = %d", a);  
-         NSLog(@"b = %c", b);  
-         NSLog(@"c = %d", c);  
-     }  
- }  
执行结果 : 
**[plain]**[view plain](http://blog.csdn.net/shulianghan/article/details/38544659#)[copy](http://blog.csdn.net/shulianghan/article/details/38544659#)
- octopus-2:ios octopus$ clang -fobjc-arc -framework Foundation 05-charDemo.m   
- octopus-2:ios octopus$ ./a.out   
- 2014-08-19 21:13:24.815 a.out[441:507] a = 97  
- 2014-08-19 21:13:24.816 a.out[441:507] b = d  
- 2014-08-19 21:13:24.817 a.out[441:507] c = 10  
## 4. 浮点型
**浮点型数据类型** : 
-- **float** : 占 4 字节;
-- **double** : 占 8 字节;
-- **long double** : 占 16 字节;
**浮点数表示形式** : 
-- **十进制形式** : 简单的浮点数, 包含一个小数点, 如 3.8, 38.0, .38 等;
-- **科学计数法** : 3.8E2 或者 3.8e2 代表 3.8*10^2, 只有浮点数才能使用科学计数法;
**Object-C 与 [Java](http://lib.csdn.net/base/17) 浮点数区别** : 
-- **Java 浮点数** : Java 的 double 与 float 不同, 浮点数赋值给 float 需要加上 f 后缀;
-- **Object-C浮点数** : 不区分 double 与 float, 一个浮点数 3.8 可以赋值给两种类型的变量;
**浮点数的特殊值** : 
-- **正无穷大** : 正浮点数除以 0.0 得到正无穷大, 正无穷大都相等, 正整数除以 0.0 得到整数的边界值, short int 除以 0.0 得到 32767(2^15 -1);
-- **负无穷大** : 负浮点数除以 0.0 得到负无穷大, 负无穷大都相等, 负整数除以 0.0 得到整数的边界值, short int 除以 0.0 得到 -32768(2^15);
-- **非数** : 0.0 除以 0.0 得到一个非数, 非数与任何数包括其本身都不相等;
**代码示例 **: 
**[objc]**[view plain](http://blog.csdn.net/shulianghan/article/details/38544659#)[copy](http://blog.csdn.net/shulianghan/article/details/38544659#)
- /************************************************************************* 
-     > File Name: 06-floatDemo.m 
-     > Author: octopus 
-     > Mail: [octopus_truth.163.com](/)
-     > Created Time: 三  8/20 01:08:35 2014 
-  ************************************************************************/  
- 
- #import <Foundation/Foundation.h>  
- 
- **int** main(**int** argc, **char****char** * argv[])  
- {  
- **@autoreleasepool** {  
-         /* 定义的10位的小数, 使用10位 %g 格式输出, 结果 float 只能接收6位小数 */  
- **float** a = 3.888888888;  
-         NSLog(@"a = %10g", a);  
- 
-         /* double 类型也只能接收6位有效值 */  
- **double** b = 388.8888888;  
-         NSLog(@"b = %10g", b);  
- 
-         NSLog(@"5.0 / 0.0 = %g", 5.0 / 0.0);  
-         NSLog(@"正无穷大对比结果 = %d", 5.0/0.0 == 500/0.0);  
- 
- **double** feiNum = 0.0 / 0.0;  
-         NSLog(@"0.0 / 0.0 = %g", feiNum);  
-         NSLog(@"非数对比结果 = %d", feiNum == feiNum);  
- 
-         /* 获取边界 */  
- **int** bound1 = 5/0.0;  
- **int** bound2 = -5/0.0;  
-         NSLog(@"正边界 = %d, 负边界 = %d", bound1, bound2);  
- 
-     }  
- }  
-- 执行效果 : 
**[plain]**[view plain](http://blog.csdn.net/shulianghan/article/details/38544659#)[copy](http://blog.csdn.net/shulianghan/article/details/38544659#)
- octopus-2:ios octopus$ clang -fobjc-arc -framework Foundation 06-floatDemo.m   
- octopus-2:ios octopus$ ./a.out   
- 2014-08-20 01:22:22.712 a.out[776:507] a =    3.88889  
- 2014-08-20 01:22:22.714 a.out[776:507] b =    388.889  
- 2014-08-20 01:22:22.714 a.out[776:507] 5.0 / 0.0 = inf  
- 2014-08-20 01:22:22.714 a.out[776:507] 正无穷大对比结果 = 1  
- 2014-08-20 01:22:22.715 a.out[776:507] 0.0 / 0.0 = nan  
- 2014-08-20 01:22:22.715 a.out[776:507] 非数对比结果 = 0  
- 2014-08-20 01:22:22.715 a.out[776:507] 正边界 = 2147483647, 负边界 = -2147483648  
-- **注意情况** : 不能将 5/0.0 直接以 %d 形式打印, 否则会报以下错误;
**[plain]**[view plain](http://blog.csdn.net/shulianghan/article/details/38544659#)[copy](http://blog.csdn.net/shulianghan/article/details/38544659#)
- octopus-2:ios octopus$ clang -fobjc-arc -framework Foundation 06-floatDemo.m   
- 06-floatDemo.m:29:44: warning: format specifies type 'int' but the argument has type 'double' [-Wformat]  
-                 NSLog(@"正边界 = %d, 负边界 = %d", 5/0.0, -5/0.0);  
-                                  ~~                ^~~~~  
-                                  %f  
- 06-floatDemo.m:29:51: warning: format specifies type 'int' but the argument has type 'double' [-Wformat]  
-                 NSLog(@"正边界 = %d, 负边界 = %d", 5/0.0, -5/0.0);  
-                                               ~~          ^~~~~~  
-                                               %f  
- 2 warnings generated.  
## 5. 枚举类型
### (1) 定义普通枚举
**定义方式** : 格式 **enum enum_name {elem1, elem2, elem3 ...}**;
-- **示例 **: enum day{Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday};
**定义枚举变量** : 格式 **enum enum_name var1, var2**;
-- **示例** : enum day today, tomorrow, 注意 today tomorrow 两个变量的取值只能是 day 枚举中定义的值;
**枚举变量赋值** : 格式 **variable = elm1** ;
-- **示例** : today = Sunday; tomorrow = Friday;
### (2) 定义匿名枚举
**匿名枚举格式** : enum {Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday} today, tomorrow ;
-- **说明** : 两个枚举变量 today 和 tomorrow 只能取值 enum 中得枚举值;
### (3) 枚举值简介
**枚举值常量** : 在 {} 中得是枚举常量 或者 枚举元素, 该元素不是变量, 不能对齐进行赋值, 枚举的值按照定义的顺序 0, 1, 2, 3 以此类推;
**枚举值本质** : 枚举值属于无符号整数, 可以使用 %u 占位符打印出来, 其值也能进行大小比较, 和四则运算;
**枚举初值** : 枚举值可以在定义的时候赋予一个初值;
### (4) 枚举示例
**示例程序** : 
**[objc]**[view plain](http://blog.csdn.net/shulianghan/article/details/38544659#)[copy](http://blog.csdn.net/shulianghan/article/details/38544659#)
- /************************************************************************* 
-     > File Name: 07-enumDemo.m 
-     > Author: octopus 
-     > Mail: [octopus_truth.163.com](/)
-     > Created Time: 四  8/21 21:36:09 2014 
-  ************************************************************************/  
- 
- #import <Foundation/Foundation.h>  
- 
- **int** main(**int** argc, **char****char** * argv[])  
- {  
- **@autoreleasepool** {  
-         /* 定义一个枚举 */  
- **enum** day {Sunday = 6, Monday = 2, Tuesday, Wednesday, Thursday, Friday, Saturday};  
- 
-         /* 定义枚举变量 */  
- **enum** day today, tomorrow;  
- 
-         /* 为枚举变量赋值 */  
-         today = Wednesday;  
-         tomorrow = Thursday;  
- 
-         /* 打印枚举值 */  
-         NSLog(@"today = %u", today);  
-         NSLog(@"tomorrow = %u", tomorrow);  
- 
- 
- **enum** {alive, die} state;  
-         state = alive;  
- 
-         NSLog(@"state = %d",  state);  
- 
- 
-     }  
- }  
执行结果 : 
**[plain]**[view plain](http://blog.csdn.net/shulianghan/article/details/38544659#)[copy](http://blog.csdn.net/shulianghan/article/details/38544659#)
- octopus-2:ios octopus$ clang -fobjc-arc -framework Foundation 07-enumDemo.m   
- octopus-2:ios octopus$ ./a.out   
- 2014-08-21 21:49:01.414 a.out[664:507] today = 4  
- 2014-08-21 21:49:01.416 a.out[664:507] tomorrow = 5  
- 2014-08-21 21:49:01.416 a.out[664:507] state = 0  
## 6. 布尔型数据
**BOOL 类型简介** : 
-- **BOOL 类型值** : 该类型至右两个值 YES 和 NO ;
-- **BOOL 类型本质** : 该类型本质是 signed char, **YES 是 1**, **NO 是 0**, 在处理的时候 **YES 会被当成真处理**, **NO 会被当成假处理**;
**定义 BOOL 类型的系统源码** : 
**[objc]**[view plain](http://blog.csdn.net/shulianghan/article/details/38544659#)[copy](http://blog.csdn.net/shulianghan/article/details/38544659#)
- #define OBJC_BOOL_DEFINED  
- 
- /// Type to represent a boolean value.  
- **typedef****signed****char****BOOL**;   
- // BOOL is explicitly signed so @encode(BOOL) == "c" rather than "C"   
- // even if -funsigned-char is used.  
- 
- #if __has_feature(objc_bool)  
- #define YES             __objc_yes  
- #define NO              __objc_no  
- #else  
- #define YES             ((BOOL)1)  
- #define NO              ((BOOL)0)  
- #endif  
代码示例 : 
**[objc]**[view plain](http://blog.csdn.net/shulianghan/article/details/38544659#)[copy](http://blog.csdn.net/shulianghan/article/details/38544659#)
- /************************************************************************* 
-     > File Name: 08-boolDemo.m 
-     > Author: octopus 
-     > Mail: [octopus_truth.163.com](/)
-     > Created Time: 二  8/26 00:29:58 2014 
-  ************************************************************************/  
- 
- #import <Foundation/Foundation.h>  
- 
- /* 判断 a 是否大于 b */  
- **BOOL** isBig(**int** a, **int** b)  
- {  
- **if**(a > b)  
-     {  
- **return****YES**;  
-     }  
- **return****NO**;  
- }  
- **int** main(**int** argc, **char****char** * argv[])  
- {  
- **@autoreleasepool** {  
- **int** a = 5;  
- **int** b = 3;  
- 
-         NSLog(@"a 是否大于 b : %d", isBig(a, b));  
- 
- **BOOL** c = 1;  
- **BOOL** d = 2;  
- 
-         NSLog(@"c = %d", c);  
-         NSLog(@"d = %d", d);  
-     }  
- }  
执行结果 : 
**[objc]**[view plain](http://blog.csdn.net/shulianghan/article/details/38544659#)[copy](http://blog.csdn.net/shulianghan/article/details/38544659#)
- octopus-2:ios octopus$ clang -fobjc-arc -framework Foundation 08-boolDemo.m   
- octopus-2:ios octopus$ ./a.out   
- 2014-08-26 00:40:48.928 a.out[1758:507] a 是否大于 b : 1  
- 2014-08-26 00:40:48.930 a.out[1758:507] c = 1  
- 2014-08-26 00:40:48.931 a.out[1758:507] d = 2  
BOOL 类型赋值 : 
-- **BOOL 类型判断** : BOOL 类型会将非 0 数字当做 YES 处理;
-- **大数字赋值** : 注意 BOOL 类型只有 8 位, 赋值的时候, 只要最后8位不为0, 那么 BOOL 类型就是 YES, 如果后 8 位为0, 那么 BOOL 为 NO;
-- **示例代码** : 
**[objc]**[view plain](http://blog.csdn.net/shulianghan/article/details/38544659#)[copy](http://blog.csdn.net/shulianghan/article/details/38544659#)
- /************************************************************************* 
-     > File Name: 08-boolDemo1.m 
-     > Author: octopus 
-     > Mail: [octopus_truth.163.com](/)
-     > Created Time: 二  8/26 00:48:05 2014 
-  ************************************************************************/  
- 
- #import <Foundation/Foundation.h>  
- 
- **int** main(**int** argc, **char****char** * argv[])  
- {  
- **@autoreleasepool** {  
- **BOOL** a = 256;  
- **BOOL** b = 512;  
- 
-         NSLog(@"a = %d", a);  
-         NSLog(@"b = %d", b);  
-     }  
- }  
-- 编译警告 : 
**[objc]**[view plain](http://blog.csdn.net/shulianghan/article/details/38544659#)[copy](http://blog.csdn.net/shulianghan/article/details/38544659#)
- octopus-2:ios octopus$ clang -fobjc-arc -framework Foundation 08-boolDemo1.m   
- 08-boolDemo1.m:13:12: warning: implicit conversion from 'int' to 'BOOL' (aka 'signed char') changes value from 256 to 0  
-       [-Wconstant-conversion]  
- **BOOL** a = 256;  
-                      ~   ^~~  
- 08-boolDemo1.m:14:12: warning: implicit conversion from 'int' to 'BOOL' (aka 'signed char') changes value from 512 to 0  
-       [-Wconstant-conversion]  
- **BOOL** b = 512;  
-                      ~   ^~~  
- 2 warnings generated.  
-- 执行结果 : 
**[objc]**[view plain](http://blog.csdn.net/shulianghan/article/details/38544659#)[copy](http://blog.csdn.net/shulianghan/article/details/38544659#)
- octopus-2:ios octopus$ ./a.out   
- 2014-08-26 00:50:00.705 a.out[1802:507] a = 0  
- 2014-08-26 00:50:00.708 a.out[1802:507] b = 0  
