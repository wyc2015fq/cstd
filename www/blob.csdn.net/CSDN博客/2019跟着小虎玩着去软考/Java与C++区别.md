# Java与C++区别 - 2019跟着小虎玩着去软考 - CSDN博客
2009年11月05日 15:03:00[littletigerat](https://me.csdn.net/littletigerat)阅读数：1519标签：[java																[c++																[跨平台																[虚拟机																[string																[byte](https://so.csdn.net/so/search/s.do?q=byte&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=虚拟机&t=blog)](https://so.csdn.net/so/search/s.do?q=跨平台&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[J2ME](https://blog.csdn.net/littletigerat/article/category/613912)
**Java与C++区别**
|**比较项**|**Java**|**C++**|
|----|----|----|
|指针类型|没有|有|
|多重继承|不支持|支持|
|操作符重载|不支持|支持|
|结构和联合|取消|有|
|函数与变量|必须是类的一部分|允许将函数和变量定义为全局|
|new|自动内存管理|由程序员自动释放内存资源|
|自动类型转换|不支持|支持|
|goto语句|不支持|支持|
|字符串|用类对象实现字符串|不支持字符串变量，在c和c十十程序中使用Null终止符代表字符串的结束|
|预处理器|没有|有|
|缺省函数参数|不支持|支持|
|速度|慢|快|
|面向对象语言|纯粹|不是纯粹|
|应用|做业务,逻辑,分布式.跨平台操作|底层操作|
|byte和 boolean|有|无|
|逗号运算符|不支持|支持|
|全局变量|没有|有|
|枚举类型|没有|有|
|typedef|没有|有|
|类的定义|一个类必须位于一个与类同名、但具有后缀.java的文件中|类名与类定义所在的文件名没有要求|
|main的定义|Public static void main(String[] args)|int main()或者 int main(int argc,char* argv[])|
|包含文件|import|#include|
|对象的创建|所有的对象都创建在堆中,没有对象被创建在stack中|有的对象和变量是创建在stack中的|
|数组越界|java虚拟机要检测数组是否越界|不检测|
|long double|无|有|
|模板|没有|有|
|类定义|没有标志结束的分号|有标志结束的分号|
|类声明|无|有|
|拷贝构造函数|无|有|
|作用域范围运算符“::”|无|有|
|析构函数|无|有|
|virtual关键字|无|有|
|指向成员的指针|无|有|
