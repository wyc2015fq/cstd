# VC 宏与预处理使用方法总结 - xqhrs232的专栏 - CSDN博客
2012年11月29日 21:56:41[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1292
原文地址::[http://blog.csdn.net/breakerzy/article/details/6908820](http://blog.csdn.net/breakerzy/article/details/6908820)
相关网帖
1.[VC预处理指令与宏定义的妙用](http://blog.csdn.net/ningyaliuhebei/article/details/7545403)----[http://blog.csdn.net/ningyaliuhebei/article/details/7545403](http://blog.csdn.net/ningyaliuhebei/article/details/7545403)

作者：Breaker Zhao
转载请注明作者和原文链接
VC 2005 中的宏 (#define) 与预处理 (#if/#ifdef/#pragma) 的使用方法总结。
**关键字**：宏, 预定义宏, 预处理, 预编译头, VC, #pragma, 编译选项, 程序区段
**目录**
- [C/C++ 预定义宏](http://blog.csdn.net/breakerzy/article/details/6908820#C_C++%20%E9%A2%84%E5%AE%9A%E4%B9%89%E5%AE%8F)- [例子：C/C++ 预定义宏的取值](http://blog.csdn.net/breakerzy/article/details/6908820#%E4%BE%8B%E5%AD%90%EF%BC%9AC_C++%20%E9%A2%84%E5%AE%9A%E4%B9%89%E5%AE%8F%E7%9A%84%E5%8F%96%E5%80%BC)
- [C/C++
 预定义宏用途：诊断与调试输出](http://blog.csdn.net/breakerzy/article/details/6908820#C_C++%20%E9%A2%84%E5%AE%9A%E4%B9%89%E5%AE%8F%E7%94%A8%E9%80%94%EF%BC%9A%E8%AF%8A%E6%96%AD%E4%B8%8E%E8%B0%83%E8%AF%95%E8%BE%93%E5%87%BA)- [CRT
 的诊断与调试输出：assert, _ASSERT/_ASSERTE, _RPTn/_RPTFn/_RPTWn/_RPTFWn](http://blog.csdn.net/breakerzy/article/details/6908820#CRT%20%E7%9A%84%E8%AF%8A%E6%96%AD%E4%B8%8E%E8%B0%83%E8%AF%95%E8%BE%93%E5%87%BA%EF%BC%9Aassert,%20_ASSERT__ASSERTE,%20_RPTn__RPTFn__RPTWn__RPTFWn)
- [MFC
 的诊断与调试输出：ASSERT/VERIFY, ASSERT_VALID, TRACE/TRACEn](http://blog.csdn.net/breakerzy/article/details/6908820#MFC%20%E7%9A%84%E8%AF%8A%E6%96%AD%E4%B8%8E%E8%B0%83%E8%AF%95%E8%BE%93%E5%87%BA%EF%BC%9AASSERT_VERIFY,%20ASSERT_VALID,%20TRACE_TRACEn)
- [MFC 的调试版 new](http://blog.csdn.net/breakerzy/article/details/6908820#MFC%20%E7%9A%84%E8%B0%83%E8%AF%95%E7%89%88%20new)
- [CRT 和 C 标准库中的宏](http://blog.csdn.net/breakerzy/article/details/6908820#CRT%20%E5%92%8C%20C%20%E6%A0%87%E5%87%86%E5%BA%93%E4%B8%AD%E7%9A%84%E5%AE%8F)- [NULL 空指针](http://blog.csdn.net/breakerzy/article/details/6908820#NULL%20%E7%A9%BA%E6%8C%87%E9%92%88)- [例子：NULL
 隐式转换和 0 是类型自动的](http://blog.csdn.net/breakerzy/article/details/6908820#%E4%BE%8B%E5%AD%90%EF%BC%9ANULL%20%E9%9A%90%E5%BC%8F%E8%BD%AC%E6%8D%A2%E5%92%8C%200%20%E6%98%AF%E7%B1%BB%E5%9E%8B%E8%87%AA%E5%8A%A8%E7%9A%84)
- [limits.h 整数类型常量](http://blog.csdn.net/breakerzy/article/details/6908820#limits.h%20%E6%95%B4%E6%95%B0%E7%B1%BB%E5%9E%8B%E5%B8%B8%E9%87%8F)
- [float.h 浮点类型常量](http://blog.csdn.net/breakerzy/article/details/6908820#float.h%20%E6%B5%AE%E7%82%B9%E7%B1%BB%E5%9E%8B%E5%B8%B8%E9%87%8F)- [例子：浮点数极限值：判断浮点数是否相等](http://blog.csdn.net/breakerzy/article/details/6908820#%E4%BE%8B%E5%AD%90%EF%BC%9A%E6%B5%AE%E7%82%B9%E6%95%B0%E6%9E%81%E9%99%90%E5%80%BC_%E5%88%A4%E6%96%AD%E6%B5%AE%E7%82%B9%E6%95%B0%E6%98%AF%E5%90%A6%E7%9B%B8%E7%AD%89)
- [math.h 数学常量](http://blog.csdn.net/breakerzy/article/details/6908820#math.h%20%E6%95%B0%E5%AD%A6%E5%B8%B8%E9%87%8F)
- [EOF 常量](http://blog.csdn.net/breakerzy/article/details/6908820#EOF%20%E5%B8%B8%E9%87%8F)- [例子：标准输入的 EOF](http://blog.csdn.net/breakerzy/article/details/6908820#%E4%BE%8B%E5%AD%90%EF%BC%9A%E6%A0%87%E5%87%86%E8%BE%93%E5%85%A5%E7%9A%84%20EOF)
- [errno.h 错误代码](http://blog.csdn.net/breakerzy/article/details/6908820#errno.h%20%E9%94%99%E8%AF%AF%E4%BB%A3%E7%A0%81)
- [locale 类别](http://blog.csdn.net/breakerzy/article/details/6908820#locale%20%E7%B1%BB%E5%88%AB)
- [_MAX_PATH 等文件名与路径长度限制](http://blog.csdn.net/breakerzy/article/details/6908820#_MAX_PATH%20%E7%AD%89%E6%96%87%E4%BB%B6%E5%90%8D%E4%B8%8E%E8%B7%AF%E5%BE%84%E9%95%BF%E5%BA%A6%E9%99%90%E5%88%B6)
- [RAND_MAX 随机数最大值](http://blog.csdn.net/breakerzy/article/details/6908820#RAND_MAX%20%E9%9A%8F%E6%9C%BA%E6%95%B0%E6%9C%80%E5%A4%A7%E5%80%BC)- [例子：用
 RAND_MAX 产生某个范围内的随机数](http://blog.csdn.net/breakerzy/article/details/6908820#%E4%BE%8B%E5%AD%90%EF%BC%9A%E7%94%A8%20RAND_MAX%20%E4%BA%A7%E7%94%9F%E6%9F%90%E4%B8%AA%E8%8C%83%E5%9B%B4%E5%86%85%E7%9A%84%E9%9A%8F%E6%9C%BA%E6%95%B0)
- [va_arg/va_start/va_end 访问变长函数参数](http://blog.csdn.net/breakerzy/article/details/6908820#va_arg_va_start_va_end%20%E8%AE%BF%E9%97%AE%E5%8F%98%E9%95%BF%E5%87%BD%E6%95%B0%E5%8F%82%E6%95%B0)
- [宏实现的 CRT 函数](http://blog.csdn.net/breakerzy/article/details/6908820#%E5%AE%8F%E5%AE%9E%E7%8E%B0%E7%9A%84%20CRT%20%E5%87%BD%E6%95%B0)
- [Microsoft 预定义宏](http://blog.csdn.net/breakerzy/article/details/6908820#Microsoft%20%E9%A2%84%E5%AE%9A%E4%B9%89%E5%AE%8F)- [平台与系统类](http://blog.csdn.net/breakerzy/article/details/6908820#%E5%B9%B3%E5%8F%B0%E4%B8%8E%E7%B3%BB%E7%BB%9F%E7%B1%BB)
- [版本号类](http://blog.csdn.net/breakerzy/article/details/6908820#%E7%89%88%E6%9C%AC%E5%8F%B7%E7%B1%BB)
- [工程配置管理类](http://blog.csdn.net/breakerzy/article/details/6908820#%E5%B7%A5%E7%A8%8B%E9%85%8D%E7%BD%AE%E7%AE%A1%E7%90%86%E7%B1%BB)
- [辅助类](http://blog.csdn.net/breakerzy/article/details/6908820#%E8%BE%85%E5%8A%A9%E7%B1%BB)- [例子：用 __VA_ARGS__
 打印跟踪函数调用](http://blog.csdn.net/breakerzy/article/details/6908820#%E4%BE%8B%E5%AD%90%EF%BC%9A%E7%94%A8%20__VA_ARGS__%20%E6%89%93%E5%8D%B0%E8%B7%9F%E8%B8%AA%E5%87%BD%E6%95%B0%E8%B0%83%E7%94%A8)
- [例子：用 __VA_ARGS__ 格式化 std::string](http://blog.csdn.net/breakerzy/article/details/6908820#%E4%BE%8B%E5%AD%90%EF%BC%9A%E7%94%A8%20__VA_ARGS__%20%E6%A0%BC%E5%BC%8F%E5%8C%96%20std::string)
- [例子：用
 __COUNTER__ 计数值定义掩码常量](http://blog.csdn.net/breakerzy/article/details/6908820#%E4%BE%8B%E5%AD%90%EF%BC%9A%E7%94%A8%20__COUNTER__%20%E8%AE%A1%E6%95%B0%E5%80%BC%E5%AE%9A%E4%B9%89%E6%8E%A9%E7%A0%81%E5%B8%B8%E9%87%8F)
- [例子：用 __FUNCTION__
 打印跟踪函数调用](http://blog.csdn.net/breakerzy/article/details/6908820#%E4%BE%8B%E5%AD%90%EF%BC%9A%E7%94%A8%20__FUNCTION__%20%E6%89%93%E5%8D%B0%E8%B7%9F%E8%B8%AA%E5%87%BD%E6%95%B0%E8%B0%83%E7%94%A8)
- [Windows API 中的注释性宏](http://blog.csdn.net/breakerzy/article/details/6908820#Windows%20API%20%E4%B8%AD%E7%9A%84%E6%B3%A8%E9%87%8A%E6%80%A7%E5%AE%8F)
- [Windows API 中的常用宏](http://blog.csdn.net/breakerzy/article/details/6908820#Windows%20API%20%E4%B8%AD%E7%9A%84%E5%B8%B8%E7%94%A8%E5%AE%8F)- [类型辅助类](http://blog.csdn.net/breakerzy/article/details/6908820#%E7%B1%BB%E5%9E%8B%E8%BE%85%E5%8A%A9%E7%B1%BB)
- [GDI 类](http://blog.csdn.net/breakerzy/article/details/6908820#GDI%20%E7%B1%BB)
- [错误处理类](http://blog.csdn.net/breakerzy/article/details/6908820#%E9%94%99%E8%AF%AF%E5%A4%84%E7%90%86%E7%B1%BB)- [标记没有使用的参数、变量辅助宏](http://blog.csdn.net/breakerzy/article/details/6908820#%E6%A0%87%E8%AE%B0%E6%B2%A1%E6%9C%89%E4%BD%BF%E7%94%A8%E7%9A%84%E5%8F%82%E6%95%B0%E3%80%81%E5%8F%98%E9%87%8F%E8%BE%85%E5%8A%A9%E5%AE%8F)
- [错误码、状态码](http://blog.csdn.net/breakerzy/article/details/6908820#%E9%94%99%E8%AF%AF%E7%A0%81%E3%80%81%E7%8A%B6%E6%80%81%E7%A0%81)
- [调用规范类](http://blog.csdn.net/breakerzy/article/details/6908820#%E8%B0%83%E7%94%A8%E8%A7%84%E8%8C%83%E7%B1%BB)
- [国际化类](http://blog.csdn.net/breakerzy/article/details/6908820#%E5%9B%BD%E9%99%85%E5%8C%96%E7%B1%BB)
- [资源类](http://blog.csdn.net/breakerzy/article/details/6908820#%E8%B5%84%E6%BA%90%E7%B1%BB)
- [网络类](http://blog.csdn.net/breakerzy/article/details/6908820#%E7%BD%91%E7%BB%9C%E7%B1%BB)
- [字符串化操作符 #](http://blog.csdn.net/breakerzy/article/details/6908820#%E5%AD%97%E7%AC%A6%E4%B8%B2%E5%8C%96%E6%93%8D%E4%BD%9C%E7%AC%A6%20#)- [用 # 操作构造字符串化宏 STRINGIZE](http://blog.csdn.net/breakerzy/article/details/6908820#%E7%94%A8%20#%20%E6%93%8D%E4%BD%9C%E6%9E%84%E9%80%A0%E5%AD%97%E7%AC%A6%E4%B8%B2%E5%8C%96%E5%AE%8F%20STRINGIZE)
- [CRT 中的 STRINGIZE 定义](http://blog.csdn.net/breakerzy/article/details/6908820#CRT%20%E4%B8%AD%E7%9A%84%20STRINGIZE%20%E5%AE%9A%E4%B9%89)
- [STRINGIZE 的展开规则](http://blog.csdn.net/breakerzy/article/details/6908820#STRINGIZE%20%E7%9A%84%E5%B1%95%E5%BC%80%E8%A7%84%E5%88%99)- [例子：用 STRINGIZE
 查看宏的展开结果](http://blog.csdn.net/breakerzy/article/details/6908820#%E4%BE%8B%E5%AD%90%EF%BC%9A%E7%94%A8%20STRINGIZE%20%E6%9F%A5%E7%9C%8B%E5%AE%8F%E7%9A%84%E5%B1%95%E5%BC%80%E7%BB%93%E6%9E%9C)
- [拼接操作符 ##](http://blog.csdn.net/breakerzy/article/details/6908820#%E6%8B%BC%E6%8E%A5%E6%93%8D%E4%BD%9C%E7%AC%A6%20##)- [例子：_T() 的定义](http://blog.csdn.net/breakerzy/article/details/6908820#%E4%BE%8B%E5%AD%90%EF%BC%9A_T%28%29%20%E7%9A%84%E5%AE%9A%E4%B9%89)
- [例子：Windows
 API 通用句柄类型的定义](http://blog.csdn.net/breakerzy/article/details/6908820#%E4%BE%8B%E5%AD%90%EF%BC%9AWindows%20API%20%E9%80%9A%E7%94%A8%E5%8F%A5%E6%9F%84%E7%B1%BB%E5%9E%8B%E7%9A%84%E5%AE%9A%E4%B9%89)
- [例子：用 ## 构造函数名](http://blog.csdn.net/breakerzy/article/details/6908820#%E4%BE%8B%E5%AD%90%EF%BC%9A%E7%94%A8%20##%20%E6%9E%84%E9%80%A0%E5%87%BD%E6%95%B0%E5%90%8D)
- [TCHAR 统一字符类型和处理](http://blog.csdn.net/breakerzy/article/details/6908820#TCHAR%20%E7%BB%9F%E4%B8%80%E5%AD%97%E7%AC%A6%E7%B1%BB%E5%9E%8B%E5%92%8C%E5%A4%84%E7%90%86)- [_TCHAR, _TEXT()/_T(), _t 系列函数](http://blog.csdn.net/breakerzy/article/details/6908820#_TCHAR,%20_TEXT%28%29__T%28%29,%20_t%20%E7%B3%BB%E5%88%97%E5%87%BD%E6%95%B0)
- [TCHAR, LPTSTR/LPCTSTR, TEXT(), A/W 版本 Windows API](http://blog.csdn.net/breakerzy/article/details/6908820#TCHAR,%20LPTSTR_LPCTSTR,%20TEXT%28%29,%20A_W%20%E7%89%88%E6%9C%AC%20Windows%20API)
- [宏的缺点和替代方法](http://blog.csdn.net/breakerzy/article/details/6908820#%E5%AE%8F%E7%9A%84%E7%BC%BA%E7%82%B9%E5%92%8C%E6%9B%BF%E4%BB%A3%E6%96%B9%E6%B3%95)- [宏难于调试](http://blog.csdn.net/breakerzy/article/details/6908820#%E5%AE%8F%E9%9A%BE%E4%BA%8E%E8%B0%83%E8%AF%95)
- [宏的使用缺陷](http://blog.csdn.net/breakerzy/article/details/6908820#%E5%AE%8F%E7%9A%84%E4%BD%BF%E7%94%A8%E7%BC%BA%E9%99%B7)
- [宏造成全局名字空间污染](http://blog.csdn.net/breakerzy/article/details/6908820#%E5%AE%8F%E9%80%A0%E6%88%90%E5%85%A8%E5%B1%80%E5%90%8D%E5%AD%97%E7%A9%BA%E9%97%B4%E6%B1%A1%E6%9F%93)
- [优先使用宏的情况](http://blog.csdn.net/breakerzy/article/details/6908820#%E4%BC%98%E5%85%88%E4%BD%BF%E7%94%A8%E5%AE%8F%E7%9A%84%E6%83%85%E5%86%B5)
- [条件编译](http://blog.csdn.net/breakerzy/article/details/6908820#%E6%9D%A1%E4%BB%B6%E7%BC%96%E8%AF%91)- [例子：注释大量代码](http://blog.csdn.net/breakerzy/article/details/6908820#%E4%BE%8B%E5%AD%90%EF%BC%9A%E6%B3%A8%E9%87%8A%E5%A4%A7%E9%87%8F%E4%BB%A3%E7%A0%81)
- [例子：MFC 中的调试版代码示例](http://blog.csdn.net/breakerzy/article/details/6908820#%E4%BE%8B%E5%AD%90%EF%BC%9AMFC%20%E4%B8%AD%E7%9A%84%E8%B0%83%E8%AF%95%E7%89%88%E4%BB%A3%E7%A0%81%E7%A4%BA%E4%BE%8B)
- [例子：DLL 工程导出符号](http://blog.csdn.net/breakerzy/article/details/6908820#%E4%BE%8B%E5%AD%90%EF%BC%9ADLL%20%E5%B7%A5%E7%A8%8B%E5%AF%BC%E5%87%BA%E7%AC%A6%E5%8F%B7)
- [例子：用
 #undef 解决 wxWidgets 自定义事件链接 BUG](http://blog.csdn.net/breakerzy/article/details/6908820#%E4%BE%8B%E5%AD%90%EF%BC%9A%E7%94%A8%20#undef%20%E8%A7%A3%E5%86%B3%20wxWidgets%20%E8%87%AA%E5%AE%9A%E4%B9%89%E4%BA%8B%E4%BB%B6%E9%93%BE%E6%8E%A5%20BUG)
- [预编译头文件](http://blog.csdn.net/breakerzy/article/details/6908820#%E9%A2%84%E7%BC%96%E8%AF%91%E5%A4%B4%E6%96%87%E4%BB%B6)- [使用 PCH 的编译命令](http://blog.csdn.net/breakerzy/article/details/6908820#%E4%BD%BF%E7%94%A8%20PCH%20%E7%9A%84%E7%BC%96%E8%AF%91%E5%91%BD%E4%BB%A4)
- [例子：典型的
 MFC 工程预编译头 stdafx.h 代码](http://blog.csdn.net/breakerzy/article/details/6908820#%E4%BE%8B%E5%AD%90%EF%BC%9A%E5%85%B8%E5%9E%8B%E7%9A%84%20MFC%20%E5%B7%A5%E7%A8%8B%E9%A2%84%E7%BC%96%E8%AF%91%E5%A4%B4%20stdafx.h%20%E4%BB%A3%E7%A0%81)
- [常用预处理指令](http://blog.csdn.net/breakerzy/article/details/6908820#%E5%B8%B8%E7%94%A8%E9%A2%84%E5%A4%84%E7%90%86%E6%8C%87%E4%BB%A4)- [#error 产生人工编译错误](http://blog.csdn.net/breakerzy/article/details/6908820##error%20%E4%BA%A7%E7%94%9F%E4%BA%BA%E5%B7%A5%E7%BC%96%E8%AF%91%E9%94%99%E8%AF%AF)
- [#line 改变行号和源文件名](http://blog.csdn.net/breakerzy/article/details/6908820##line%20%E6%94%B9%E5%8F%98%E8%A1%8C%E5%8F%B7%E5%92%8C%E6%BA%90%E6%96%87%E4%BB%B6%E5%90%8D)
- [# 空指令](http://blog.csdn.net/breakerzy/article/details/6908820##%20%E7%A9%BA%E6%8C%87%E4%BB%A4)
- [#pragma 预处理指令](http://blog.csdn.net/breakerzy/article/details/6908820##pragma%20%E9%A2%84%E5%A4%84%E7%90%86%E6%8C%87%E4%BB%A4)- [#pragma once 只包含一次头文件](http://blog.csdn.net/breakerzy/article/details/6908820##pragma%20once%20%E5%8F%AA%E5%8C%85%E5%90%AB%E4%B8%80%E6%AC%A1%E5%A4%B4%E6%96%87%E4%BB%B6)
- [#pragma message 编译时输出消息](http://blog.csdn.net/breakerzy/article/details/6908820##pragma%20message%20%E7%BC%96%E8%AF%91%E6%97%B6%E8%BE%93%E5%87%BA%E6%B6%88%E6%81%AF)- [例子：用
 #pragma message 和 STRINGIZE 查看宏的展开结果](http://blog.csdn.net/breakerzy/article/details/6908820#%E4%BE%8B%E5%AD%90%EF%BC%9A%E7%94%A8%20#pragma%20message%20%E5%92%8C%20STRINGIZE%20%E6%9F%A5%E7%9C%8B%E5%AE%8F%E7%9A%84%E5%B1%95%E5%BC%80%E7%BB%93%E6%9E%9C)
- [#pragma push_macro/pop_macro 保存和恢复宏定义](http://blog.csdn.net/breakerzy/article/details/6908820##pragma%20push_macro_pop_macro%20%E4%BF%9D%E5%AD%98%E5%92%8C%E6%81%A2%E5%A4%8D%E5%AE%8F%E5%AE%9A%E4%B9%89)
- [#pragma warning 禁用和启用编译警告](http://blog.csdn.net/breakerzy/article/details/6908820##pragma%20warning%20%E7%A6%81%E7%94%A8%E5%92%8C%E5%90%AF%E7%94%A8%E7%BC%96%E8%AF%91%E8%AD%A6%E5%91%8A)
- [#pragma
 comment 目标文件注释和编译选项传递](http://blog.csdn.net/breakerzy/article/details/6908820##pragma%20comment%20%E7%9B%AE%E6%A0%87%E6%96%87%E4%BB%B6%E6%B3%A8%E9%87%8A%E5%92%8C%E7%BC%96%E8%AF%91%E9%80%89%E9%A1%B9%E4%BC%A0%E9%80%92)- [例子：用 #pragma comment(lib)
 实现库的 auto-link](http://blog.csdn.net/breakerzy/article/details/6908820#%E4%BE%8B%E5%AD%90%EF%BC%9A%E7%94%A8%20#pragma%20comment%28lib%29%20%E5%AE%9E%E7%8E%B0%E5%BA%93%E7%9A%84%20auto-link)
- [#pragma comment(linker) 传递链接选项](http://blog.csdn.net/breakerzy/article/details/6908820##pragma%20comment%28linker%29%20%E4%BC%A0%E9%80%92%E9%93%BE%E6%8E%A5%E9%80%89%E9%A1%B9)
- [#pragma comment(linker, "/SECTION")
 设置区段属性](http://blog.csdn.net/breakerzy/article/details/6908820##pragma%20comment%28linker,%20__SECTION_%29%20%E8%AE%BE%E7%BD%AE%E5%8C%BA%E6%AE%B5%E5%B1%9E%E6%80%A7)
- [#pragma 区段操作](http://blog.csdn.net/breakerzy/article/details/6908820##pragma%20%E5%8C%BA%E6%AE%B5%E6%93%8D%E4%BD%9C)- [#pragma section 在目标文件中创建区段](http://blog.csdn.net/breakerzy/article/details/6908820##pragma%20section%20%E5%9C%A8%E7%9B%AE%E6%A0%87%E6%96%87%E4%BB%B6%E4%B8%AD%E5%88%9B%E5%BB%BA%E5%8C%BA%E6%AE%B5)
- [#pragma
 alloc_text 将 C 链接约定的函数放置到区段](http://blog.csdn.net/breakerzy/article/details/6908820##pragma%20alloc_text%20%E5%B0%86%20C%20%E9%93%BE%E6%8E%A5%E7%BA%A6%E5%AE%9A%E7%9A%84%E5%87%BD%E6%95%B0%E6%94%BE%E7%BD%AE%E5%88%B0%E5%8C%BA%E6%AE%B5)
- [#pragma code_seg 将函数放置到代码区段](http://blog.csdn.net/breakerzy/article/details/6908820##pragma%20code_seg%20%E5%B0%86%E5%87%BD%E6%95%B0%E6%94%BE%E7%BD%AE%E5%88%B0%E4%BB%A3%E7%A0%81%E5%8C%BA%E6%AE%B5)
- [#pragma data_seg/bss_seg/const_seg
 将数据放置到区段](http://blog.csdn.net/breakerzy/article/details/6908820##pragma%20data_seg_bss_seg_const_seg%20%E5%B0%86%E6%95%B0%E6%8D%AE%E6%94%BE%E7%BD%AE%E5%88%B0%E5%8C%BA%E6%AE%B5)
- [#pragma pack 设置成员字节对齐](http://blog.csdn.net/breakerzy/article/details/6908820##pragma%20pack%20%E8%AE%BE%E7%BD%AE%E6%88%90%E5%91%98%E5%AD%97%E8%8A%82%E5%AF%B9%E9%BD%90)
- [#pragma inline 函数设置](http://blog.csdn.net/breakerzy/article/details/6908820##pragma%20inline%20%E5%87%BD%E6%95%B0%E8%AE%BE%E7%BD%AE)- [#pragma auto_inline 禁用和启用 auto-inline](http://blog.csdn.net/breakerzy/article/details/6908820##pragma%20auto_inline%20%E7%A6%81%E7%94%A8%E5%92%8C%E5%90%AF%E7%94%A8%20auto-inline)
- [#pragma
 inline_depth 设置函数调用的 inline 化深度](http://blog.csdn.net/breakerzy/article/details/6908820##pragma%20inline_depth%20%E8%AE%BE%E7%BD%AE%E5%87%BD%E6%95%B0%E8%B0%83%E7%94%A8%E7%9A%84%20inline%20%E5%8C%96%E6%B7%B1%E5%BA%A6)
- [#pragma
 inline_recursion 禁用和启用递归函数的 inline 化](http://blog.csdn.net/breakerzy/article/details/6908820##pragma%20inline_recursion%20%E7%A6%81%E7%94%A8%E5%92%8C%E5%90%AF%E7%94%A8%E9%80%92%E5%BD%92%E5%87%BD%E6%95%B0%E7%9A%84%20inline%20%E5%8C%96)
- [#pragma 优化指令](http://blog.csdn.net/breakerzy/article/details/6908820##pragma%20%E4%BC%98%E5%8C%96%E6%8C%87%E4%BB%A4)- [#pragma optimize 禁用或启动特定优化](http://blog.csdn.net/breakerzy/article/details/6908820##pragma%20optimize%20%E7%A6%81%E7%94%A8%E6%88%96%E5%90%AF%E5%8A%A8%E7%89%B9%E5%AE%9A%E4%BC%98%E5%8C%96)
- [#pragma intrinsic 使用 intrinsic 函数](http://blog.csdn.net/breakerzy/article/details/6908820##pragma%20intrinsic%20%E4%BD%BF%E7%94%A8%20intrinsic%20%E5%87%BD%E6%95%B0)
- [#pragma function 使用普通函数](http://blog.csdn.net/breakerzy/article/details/6908820##pragma%20function%20%E4%BD%BF%E7%94%A8%E6%99%AE%E9%80%9A%E5%87%BD%E6%95%B0)
- [#pragma deprecated 声明废弃函数](http://blog.csdn.net/breakerzy/article/details/6908820##pragma%20deprecated%20%E5%A3%B0%E6%98%8E%E5%BA%9F%E5%BC%83%E5%87%BD%E6%95%B0)
- [#pragma omp 使用 OpenMP 指令](http://blog.csdn.net/breakerzy/article/details/6908820##pragma%20omp%20%E4%BD%BF%E7%94%A8%20OpenMP%20%E6%8C%87%E4%BB%A4)
- [#pragma region/endregion 折叠代码块](http://blog.csdn.net/breakerzy/article/details/6908820##pragma%20region_endregion%20%E6%8A%98%E5%8F%A0%E4%BB%A3%E7%A0%81%E5%9D%97)
- [#pragma
 setlocale 设置源代码中字符串字面量的编码](http://blog.csdn.net/breakerzy/article/details/6908820##pragma%20setlocale%20%E8%AE%BE%E7%BD%AE%E6%BA%90%E4%BB%A3%E7%A0%81%E4%B8%AD%E5%AD%97%E7%AC%A6%E4%B8%B2%E5%AD%97%E9%9D%A2%E9%87%8F%E7%9A%84%E7%BC%96%E7%A0%81)
- [#pragma include_alias 定义头文件别名](http://blog.csdn.net/breakerzy/article/details/6908820##pragma%20include_alias%20%E5%AE%9A%E4%B9%89%E5%A4%B4%E6%96%87%E4%BB%B6%E5%88%AB%E5%90%8D)
- [预处理相关编译选项](http://blog.csdn.net/breakerzy/article/details/6908820#%E9%A2%84%E5%A4%84%E7%90%86%E7%9B%B8%E5%85%B3%E7%BC%96%E8%AF%91%E9%80%89%E9%A1%B9)- [/D 定义宏](http://blog.csdn.net/breakerzy/article/details/6908820#_D%20%E5%AE%9A%E4%B9%89%E5%AE%8F)- [定义数字](http://blog.csdn.net/breakerzy/article/details/6908820#%E5%AE%9A%E4%B9%89%E6%95%B0%E5%AD%97)
- [定义字符串](http://blog.csdn.net/breakerzy/article/details/6908820#%E5%AE%9A%E4%B9%89%E5%AD%97%E7%AC%A6%E4%B8%B2)
- [空定义](http://blog.csdn.net/breakerzy/article/details/6908820#%E7%A9%BA%E5%AE%9A%E4%B9%89)
- [CL 环境变量使用 /D](http://blog.csdn.net/breakerzy/article/details/6908820#CL%20%E7%8E%AF%E5%A2%83%E5%8F%98%E9%87%8F%E4%BD%BF%E7%94%A8%20_D)
- [/E, /EP, /P 预处理选项](http://blog.csdn.net/breakerzy/article/details/6908820#_E,%20_EP,%20_P%20%E9%A2%84%E5%A4%84%E7%90%86%E9%80%89%E9%A1%B9)- [例子：预处理展开源文件](http://blog.csdn.net/breakerzy/article/details/6908820#%E4%BE%8B%E5%AD%90%EF%BC%9A%E9%A2%84%E5%A4%84%E7%90%86%E5%B1%95%E5%BC%80%E6%BA%90%E6%96%87%E4%BB%B6)
- [例子：过滤查看预处理展开结果](http://blog.csdn.net/breakerzy/article/details/6908820#%E4%BE%8B%E5%AD%90%EF%BC%9A%E8%BF%87%E6%BB%A4%E6%9F%A5%E7%9C%8B%E9%A2%84%E5%A4%84%E7%90%86%E5%B1%95%E5%BC%80%E7%BB%93%E6%9E%9C)
- [/showIncludes 输出头文件列表](http://blog.csdn.net/breakerzy/article/details/6908820#_showIncludes%20%E8%BE%93%E5%87%BA%E5%A4%B4%E6%96%87%E4%BB%B6%E5%88%97%E8%A1%A8)- [例子：查看 #include 头文件列表](http://blog.csdn.net/breakerzy/article/details/6908820#%E4%BE%8B%E5%AD%90%EF%BC%9A%E6%9F%A5%E7%9C%8B%20#include%20%E5%A4%B4%E6%96%87%E4%BB%B6%E5%88%97%E8%A1%A8)
VC 中的宏使用方法参考 MSDN: [Macros (C/C++)](http://msdn.microsoft.com/en-us/library/503x3e3s.aspx)
### C/C++ 预定义宏[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
**__LINE__**: 当前源文件的行号，整数
**__FILE__**: 当前源文件名，char 字符串，使用 /FC 选项产生全路径
**__DATE__**: 当前编译日期，char 字符串，格式 Aug 28 2011
**__TIME__**: 当前编译时间，char 字符串，格式 06:43:59
**__STDC__**: 整数 1，表示兼容 ANSI/ISO C 标准，配合 #if 使用
**__TIMESTAMP__**: 最后一次修改当前文件的时间戳，char 字符串，格式 Sun Aug 28 06:43:57 2011
**__cplusplus**: 以 C++ 方式而非 C 语言方式编译时定义，VC 2005 中定义为 199711L，配合 #ifdef 使用
**例子：C/C++ 预定义宏的取值[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
- // MacroTest.h
- void PrintSourceInfo()  
- {  
- const _TCHAR* pszstdc;  
- const _TCHAR* pszcpp;  
- 
- #if __STDC__
-     pszstdc = _T("YES");  
- #else
-     pszstdc = _T("NO");  
- #endif
- 
- #ifdef __cplusplus
-     pszcpp = _T("YES");  
- #else
-     pszcpp = _T("NO");  
- #endif
- 
-     _tprintf(_T("File: %s, Line: %d, Date: %s, Time: %s, Timestamp: %s, ANSI/ISO C: %s, C++: %s\n"),  
-              _T(__FILE__), __LINE__, _T(__DATE__), _T(__TIME__), _T(__TIMESTAMP__), pszstdc, pszcpp);  
- }  
- 
- // 宏化的 PrintSourceInfo()
- #define PRINT_SOURCE_INFO() \
-     _tprintf(_T("File: %s, Line: %d, Date: %s, Time: %s, Timestamp: %s\n"), \  
-              _T(__FILE__), __LINE__, _T(__DATE__), _T(__TIME__), _T(__TIMESTAMP__));  
MacroTest.h 中定义函数 PrintSourceInfo() 和 PRINT_SOURCE_INFO()，在 MacroTest.cpp include=> MacroTest.h，并调用它们
**输出结果**：
(1). 使用函数 PrintSourceInfo()，无论 Debug/Release 方式编译，无论是否 inline 化 PrintSourceInfo()，输出结果相同，均是 MacroTest.h 的信息：
File: d:\source\macrotest\macrotest.h, Line: 64, Date: Aug 28 2011, Time: 06:43:59, Timestamp: Sun Aug 28 06:43:57 2011, ANSI/ISO C: NO, C++: YES
(2). 使用宏 PRINT_SOURCE_INFO()，Debug/Release 方式编译输出结果大致相同，均是 MacroTest.cpp 的信息，只是 Debug 输出的 __FILE__ 是全路径，而 Release 输出的是相对路径：
File: d:\source\macrotest\macrotest.cpp, Line: 14, Date: Aug 28 2011, Time: 07:42:30, Timestamp: Sun Aug 28 07:38:25 2011
**说明**：
(1). __FILE__、__DATE__、__TIME__ 是 char 字符串，而不是 wchar_t 宽字符字符串，需配合 _T()、_t 系列函数使用
(2). 如果在函数 PrintSourceInfo() 中使用宏，则 __FILE__、__LINE__、__TIME__ 等表示的是 PrintSourceInfo() 所在文件，即例 1 中的 MacroTest.h 的信息；如果在宏 PRINT_SOURCE_INFO() 中使用宏，因为宏 PRINT_SOURCE_INFO() 嵌套展开的缘故，__FILE__ 等表示的是 PRINT_SOURCE_INFO() 展开所在文件，即 MacroTest.cpp 的信息
(3). 无论使用 PrintSourceInfo() 还是 PRINT_SOURCE_INFO()，__LINE__ 总是文件 .h/.cpp 的固有行号，而非 [MacroTest.cpp include=> MacroTest.h] 预处理展开后的行号
(4). 在 VC 2005 中，上述编译方式下没有定义 __STDC__，要使 __STDC__ = 1，应同时满足以下条件：
- (a). 以 C 方式编译
- (b). 使用编译选项 /Za，表示禁止 Microsoft C/C++ 语言扩展，从而兼容 ANSI C/C++
#### C/C++ 预定义宏用途：诊断与调试输出[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
参考 VC CRT 和 MFC 的代码，注意：需要在宏中使用 __FILE__、__LINE__，原因见上面“说明 (2)”
**CRT 的诊断与调试输出：assert, _ASSERT/_ASSERTE, _RPTn/_RPTFn/_RPTWn/_RPTFWn[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
CRT 的诊断宏 assert()、_ASSERT()/_ASSERTE()
- // assert.h
- 
- _CRTIMP void __cdecl _wassert(__in_z constwchar_t * _Message, __in_z constwchar_t *_File, __in unsigned _Line);  
- 
- #define assert(_Expression) (void)( (!!(_Expression)) || (_wassert(_CRT_WIDE(#_Expression), _CRT_WIDE(__FILE__), __LINE__), 0) )
- 
- // crtdbg.h
- 
- #define _ASSERT_EXPR(expr, msg) \
-         (void) ((!!(expr)) || \  
-                 (1 != _CrtDbgReportW(_CRT_ASSERT, _CRT_WIDE(__FILE__), __LINE__, NULL, msg)) || \  
-                 (_CrtDbgBreak(), 0))  
- 
- #ifndef _ASSERT
- #define _ASSERT(expr)   _ASSERT_EXPR((expr), NULL)
- #endif
- 
- #ifndef _ASSERTE
- #define _ASSERTE(expr)  _ASSERT_EXPR((expr), _CRT_WIDE(#expr))
- #endif
CRT 的调试输出宏 _RPTn()/_RPTFn()，n: 0 ~ 5
_RPTWn()/_RPTFWn() 是宽字符版
- // crtdbg.h
- 
- #define _RPT_BASE(args) \
-         (void) ((1 != _CrtDbgReport args) || \  
-                 (_CrtDbgBreak(), 0))  
- 
- #define _RPTF0(rptno, msg) \
-         _RPT_BASE((rptno, __FILE__, __LINE__, NULL, "%s", msg))  
**MFC 的诊断与调试输出：ASSERT/VERIFY, ASSERT_VALID, TRACE/TRACEn[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
MFC 的诊断宏 ASSERT()/VERIFY()、ASSERT_VALID()
- // afx.h
- 
- #define ASSERT(f)          DEBUG_ONLY((void) ((f) || !::AfxAssertFailedLine(THIS_FILE, __LINE__) || (AfxDebugBreak(), 0)))
- #define ASSERT_VALID(pOb)  DEBUG_ONLY((::AfxAssertValidObject(pOb, THIS_FILE, __LINE__)))
MFC 的调试输出宏 TRACE()/TRACEn()，n: 0 ~ 3
- // atltrace.h
- 
- #ifndef ATLTRACE
- #define ATLTRACE ATL::CTraceFileAndLineInfo(__FILE__, __LINE__)
- #define ATLTRACE2 ATLTRACE
- #endif
- 
- // afx.h
- 
- #include <atltrace.h>
- #define TRACE ATLTRACE
- 
- #define THIS_FILE          __FILE__
- #define VERIFY(f)          ASSERT(f)
- #define DEBUG_ONLY(f)      (f)
- 
- #define TRACE0(sz)              TRACE(_T("%s"), _T(sz))
- #define TRACE1(sz, p1)          TRACE(_T(sz), p1)
- #define TRACE2(sz, p1, p2)      TRACE(_T(sz), p1, p2)
- #define TRACE3(sz, p1, p2, p3)  TRACE(_T(sz), p1, p2, p3)
**MFC 的调试版 new[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
- // afx.h
- 
- void* AFX_CDECL operator new(size_t nSize, LPCSTR lpszFileName, int nLine);  
- #define DEBUG_NEW new(THIS_FILE, __LINE__)
- 
- // 用户代码
- 
- // 调试版 new
- #ifdef _DEBUG
- #define new DEBUG_NEW
- #endif
### CRT 和 C 标准库中的宏[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
VC CRT 和 C 标准库中的宏参考 MSDN: [Global Constants](http://msdn.microsoft.com/en-us/library/0ht71tb6.aspx)
#### NULL 空指针[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
NULL 在 stddef.h, stdio.h, stdlib.h 等多个头文件中定义，是地址/指针类型的 0，如下：
- #ifdef __cplusplus
- #define NULL    0
- #else
- #define NULL    ((void *)0)
- #endif
C++ 中的 0 是类型自动的，所以用 0 定义 NULL；而 C 中 0 是确定的 int 类型，所以需要强制
C++ 中，当 NULL 的相关操作数，如：对比操作 ptr == NULL，或函数的形参是指针类型时，或者能够“从指针类型隐式转换”时，0 被自动转换为指针类型
**例子：NULL 隐式转换和 0 是类型自动的[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
- // baby pointer wrapper
- class Pointer  
- {  
- public:  
- // 非 explicit 构造函数，说明 Pointer 可以从指针类型 void* 隐式转换
-     Pointer(void* p) : m_Ptr(p)  
-     {}  
- 
- bool IsNull() const
-     {  
- return (m_Ptr == NULL);  
-     }  
- 
- private:  
- void*    m_Ptr;  
- };  
- 
- // 形参可以从指针类型 void* 隐式转换
- void TestPointer(Pointer ptr)  
- {  
-     _tprintf(_T("ptr is %sNULL\n"), ptr.IsNull() ? _T("") : _T("NOT "));  
- }  
- 
- // 用户代码
- TestPointer(0);         // OK，0 是类型自动的，0 被自动转换为 void*，再次隐式转换为 Pointer
- TestPointer(NULL);      // OK，NULL 就是 0，同上
- TestPointer(1);         // Error，C++ 中 1 不同于 0，它是确定的 int 类型，
- // 只能提升转换到 float/double 类型，不能自动转换为指针
- TestPointer((int*)1);   // OK，强制转换 1 为 int*，int* 自动转换为 void*，再次隐式转换为 Pointer
- // 注意：void* 到 int* 不能自动转换，需要强制，参考 malloc() 的返回值
#### limits.h 整数类型常量[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
在 limits.h 中定义，定义了各种 int 类型 (unsigned, char, short, long, __int64) 的最小、最大值，如 SCHAR_MAX (signed char MAX)、UCHAR_MAX (unsigned char MAX)、USHRT_MAX (unsigned short MAX) 等。编译时，如果 int 字面量超出这些范围，会编译出错
参考 MSDN: [Integer Limits](http://msdn.microsoft.com/en-us/library/296az74e.aspx)
#### float.h 浮点类型常量[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
在 float.h 中定义，定义各种浮点类型 (float, double, long double) 的极限值，如最小、最大值，最小浮点差量 (epsilon) 等
参考 MSDN: [Floating Limits](http://msdn.microsoft.com/en-us/library/6bs3y5ya.aspx)
**例子：浮点数极限值：判断浮点数是否相等[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
- // 对比一个 double 是否为 0
- inline
- bool double_equal0(double n)  
- {  
- return (n >= 0 ? n < DBL_MIN : n > -DBL_MIN);  
- }  
- 
- // 对比两个 double 是否相等
- inline
- bool double_equal(double l, double r)  
- {  
- return (l >= r ? l - r < DBL_EPSILON : r - l < DBL_EPSILON);  
- }  
- 
- // 打印函数的结果
- #define TEST_BOOL_FUNC(func) _tprintf(_T("%s: %s\n"), _TSTRINGIZE(func), func ? _T("TRUE") : _T("FALSE"))
- 
- // 用户代码
- // 对比 double 是否为 0 时，double_equal0() 更精确
- // 对比两个 double 是否相等时，最好用 double_equal()
- 
- TEST_BOOL_FUNC(double_equal0(0));                       // TRUE
- TEST_BOOL_FUNC(double_equal0(DBL_EPSILON));             // FALSE
- TEST_BOOL_FUNC(double_equal0(-DBL_EPSILON));            // FALSE
- TEST_BOOL_FUNC(double_equal0(DBL_MIN));                 // FALSE
- TEST_BOOL_FUNC(double_equal0(-DBL_MIN));                // FALSE
- 
- TEST_BOOL_FUNC(double_equal(0, 0));                     // TRUE
- TEST_BOOL_FUNC(double_equal(DBL_EPSILON, 0));           // FALSE
- TEST_BOOL_FUNC(double_equal(DBL_MIN, 0));               // TRUE
- TEST_BOOL_FUNC(double_equal(1.0, 1.0 + DBL_EPSILON));   // FALSE
- TEST_BOOL_FUNC(double_equal(1.0, 1.0 - DBL_EPSILON));   // FALSE
- TEST_BOOL_FUNC(double_equal(1.0, 1.0 + DBL_MIN));       // TRUE
- TEST_BOOL_FUNC(double_equal(1.0, 1.0 - DBL_MIN));       // TRUE
#### math.h 数学常量[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
数学计算常用的浮点数常量，如 M_PI (pi), M_E (e), M_SQRT2 (sqrt(2)) 等。这些数学常量不是标准 C/C++ 的一部分，而是 Microsoft 的扩展，使用前需要定义 _USE_MATH_DEFINES：
- #define _USE_MATH_DEFINES
- #include <math.h>
#### EOF 常量[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
EOF (end-of-file) 常量，定义为 (-1)，有宽字符版 WEOF ((wint_t)(0xFFFF))，EOF 和 WEOF 在 stdio.h 中定义，还有 _TCHAR 版 _TEOF，在 tchar.h 中定义。EOF 在流、I/O 操作中表示到达流、文件末尾（EOF 条件），也用来表示发生错误情况
**例子：标准输入的 EOF[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
- // 设置 locale
- // 定义宽字符流与控制台 I/O 字符之间的转换字符集编码为系统 ANSI 字符集
- // 这样在中文 Windows 上可输入、显示中文字符
- _tsetlocale(LC_ALL, _T(""));  
- 
- // 要用存储空间 >= _gettchar() 返回值类型的变量保存其返回值
- // 而不要用 char ch = _getchar()，那样会截断其返回值类型
- int ch;  
- while ((ch = _gettchar()) != _TEOF)  
-     _tprintf(_T("[%c]"), (_TCHAR)ch);  
- 
- _tprintf(_T("\nread stdin: %s\n"), (feof(stdin) ? _T("EOF") : _T("Error")));  
测试输出，用 Ctrl + Z 产生 EOF 信号：
- abc汉字  
- [a][b][/c][汉][字][  
- ]^Z  
- 
- read stdin: EOF  
#### errno.h 错误代码[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
在 errno.h 中定义，是测试全局量 errno 的值，errno 在 VC 中实现为线程安全的函数，而非全局变量。错误代码以 E 打头如 EINVAL：不合法的参数错误
错误代码具体值参考 MSDN: [errno Constants](http://msdn.microsoft.com/en-us/library/5814770t.aspx) 和 [errno,
 _doserrno, _sys_errlist, and _sys_nerr](http://msdn.microsoft.com/en-us/library/t3ayayh1.aspx)
#### locale 类别[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
locale 类别 (Categories)，在 locale.h 中定义，如 LC_ALL、LC_CTYPE
#### _MAX_PATH 等文件名与路径长度限制[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
包括全路径与各部分路径的限制，即 FILENAME_MAX、_MAX_PATH、_MAX_DRIVE、_MAX_EXT、_MAX_FNAME、_MAX_DIR，在 stdlib.h 中定义。最大全路径长度限制在 260，和 Windows 的 MAX_PATH 相同，这是为了兼容 Windows 98 FAT32 文件系统。CRT 支持 32767 长度的文件名，方法和 Windows API 相同，即使用 "\\?\" 路径前缀，并调用 Unicode 宽字符版的 CRT 函数
#### RAND_MAX 随机数最大值[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
在 stdlib.h 中定义为 32767，rand() 函数会产生 0 ~ RAND_MAX 之间的伪随机 int 值
**例子：用 RAND_MAX 产生某个范围内的随机数[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
- template<bool seed, typename Type>  
- inline
- Type get_rand(Type min, Type max)  
- {  
-     _ASSERT(max >= min);  
- 
- if (seed)       // Release 方式编译时，这个判断语句会被优化掉
-         srand((unsigned int) time(NULL));  
- 
- return (Type) (((double) rand() / (double) RAND_MAX) * (max - min) + min);  
- }  
- 
- template<typename Type>  
- inline
- Type get_rand_seed(Type min, Type max)  
- {  
- return get_rand<true>(min, max);  
- }  
- 
- template<typename Type>  
- inline
- Type get_rand_noseed(Type min, Type max)  
- {  
- return get_rand<false>(min, max);  
- }  
- 
- // 用户代码
- #define RANGE_MIN   10
- #define RANGE_MAX   100
- 
- int randnum;  
- randnum = get_rand_seed(RANGE_MIN, RANGE_MAX);  
- randnum = get_rand_noseed(RANGE_MIN, RANGE_MAX);  
#### va_arg/va_start/va_end 访问变长函数参数[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
用于访问类似 printf(const char* format, ...) 等变长函数参数的辅助宏，在 stdarg.h 中声明，参考 MSDN: [va_arg, va_end, va_start](http://msdn.microsoft.com/en-us/library/kb57fad8.aspx)
#### 宏实现的 CRT 函数[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
在 VC CRT 中有些函数以宏和函数两种方式实现，如 getchar()，并优先使用宏版本，
强制使用函数版的方法：
(1). 调用时给函数名加括号，如 (getchar)()
(2). 调用前，取消宏版本的定义，如 #undef getchar
两种实现方式的比较见 MSDN: [Recommendations for Choosing Between Functions and Macros](http://msdn.microsoft.com/en-us/library/a8130zc6.aspx)
### Microsoft 预定义宏[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
VC C/C++ 和 Microsoft 预定义宏参考 MSDN: [Predefined Macros](http://msdn.microsoft.com/en-us/library/b0084kay.aspx)
这些宏可以分类如下：
#### 平台与系统类[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
**_M_IX86**: IA32/x86 平台
**_M_IA64**: IA64/IPF (Itanium Processor Family) 64bit 平台
**_M_X64**: x64/x86-64/AMD64 平台
**WIN32**, **_WIN32**: Win32 和 Win64 程序开发都会定义
**_WIN64**: Win64 程序开发
**_CONSOLE**: 控制台 Windows 程序开发，链接 Console 子系统：/SUBSYSTEM:CONSOLE
**_WINDOWS**: 非控制台 Windows 程序开发，链接 Windows 子系统：/SUBSYSTEM:WINDOWS
#### 版本号类[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
通常定义为数字，配合 #if (XXX >= 1000) 使用，启动、禁用特定部分的代码、特性
**_MSC_VER**: VC 编译器 cl 版本号。VC 2003 编译器版本号 13.10 (_MSC_VER = 1310)，VC 2005 编译器版本号 14.00 (_MSC_VER = 1400)。用 cl /? 查看编译器版本号
**_MFC_VER**: MFC 版本号
**_ATL_VER**: ATL 版本号
**__CLR_VER**: CLR 版本号
**WINVER**: 目标 Windows 版本号
**_WIN32_WINNT**: 目标 Windows NT 版本号
**_WIN32_WINDOWS**: 目标 Windows 9x 版本号
**_WIN32_IE**: 目标 IE 版本号
#### 工程配置管理类[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
**_DEBUG**, **NDEBUG**: Debug/Release 编译方式
**UNICODE**, **_UNICODE**, **_MBCS**: ANSI/UNICODE/MBCS 字符集支持
**_AFXDLL**: 动态链接 MFC (DLL)
**_ATL_STATIC_REGISTRY**, **_ATL_DLL**: 静态/动态链接 ATL
**_DLL**: 动态链接 CRT (DLL)，对应 /MD、/MDd 编译选项
**_MT**: CRT 多线程支持，目前 4 种 CRT 链接方式 /MD、/MDd、/MT、/MTd 都支持多线程（VC 2005 已没有单线程版 CRT），加上创建 DLL 模块的 /LD、/LDd，都定义 _MT
**_MANAGED**: 以 /clr、/clr:pure、/clr:safe 托管方式编译时，定义为 1
**__cplusplus_cli**: 以 /clr、/clr:pure、/clr:safe 方式编译时定义，VC 2005 中定义为 200406L
上面 1、2、3 类宏通常和条件编译预处理指令 #if/#ifdef/#ifndef 配合使用
#### 辅助类[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
**__VA_ARGS__**: 在函数式宏中，代表变长部分参数 (...)，参考 MSDN: [Variadic Macros](http://msdn.microsoft.com/en-us/library/ms177415.aspx)
**__COUNTER__**: include 展开编译单元后，编译时第一次遇到 __COUNTER__ 替换为 0，以后在这个编译每遇到一次 __COUNTER__ 自增一。不同的编译单元之间 __COUNTER__ 不互相积累叠加，均从 0 开始计数，但预编译头 .pch 文件会记录 __COUNTER__ 的历史值，则每个编译单元均从历史值 + 1 开始计数。__COUNTER__ 支持宏的嵌套展开
**__FUNCTION__**, **__FUNCDNAME__**, **__FUNCSIG__**: 表示所在函数的函数名的 char 字符串。例如，对于 void test_funcname_macro() 函数原型，它们的值如下：
(1). __FUNCTION__ = test_funcname_macro: 函数的原始名/非修饰名 (undecorated)
(2). __FUNCDNAME__ = ?test_funcname_macro@@YAXXZ: 函数的修饰名 (decorated)，可用工具 undname "decorated_name" 得出函数原型和调用规范，即 __FUNCSIG__ 所表示的
(3). __FUNCSIG__ = void __cdecl test_funcname_macro(void): 函数的 signature 名，即调用约定、返回值类型、参数类型
**例子：用 __VA_ARGS__ 打印跟踪函数调用[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
这个 CALL_TRACE 功能不实用，只为说明 __VA_ARGS__ 用法：
- // 针对参数不为 void，且需要保存返回值的函数
- #define CALL_TRACE(func, ret, ...)      { _tprintf(_T("call: %s\n"), _TSTRINGIZE(func)); ret = func(__VA_ARGS__); }
- // 针对返回值为 void 或不关心返回值的函数
- #define CALL_TRACE_VOID(func, ...)      { _tprintf(_T("call: %s\n"), _TSTRINGIZE(func)); func(__VA_ARGS__); }
- 
- // 针对参数为 void 的函数
- // NOTE: 函数 func() 使用 func(__VA_ARGS__) 展开时，会影响前面的变长参数函数 _tprintf()，
- // 导致运行时缓冲区访问违例（Debug 方式产生保护中断），所以不能用前两版带 func(__VA_ARGS__) 的 CALL_TRACE
- #define CALL_TRACE_VOIDPARM(func, ret)  { _tprintf(_T("call: %s\n"), _TSTRINGIZE(func)); ret = func(); }
- 
- // 针对返回值、参数均为 void 的函数
- #define CALL_TRACE_VOID_VOIDPARM(func)  { _tprintf(_T("call: %s\n"), _TSTRINGIZE(func)); func(); }
- 
- // 用户代码
- // Unicode 方式编译时，输出 call: CreateFileW，并将返回值传给 hFile
- CALL_TRACE_RET(CreateFile, hFile, _T("bbb"), 0, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);  
**例子：用 __VA_ARGS__ 格式化 std::string[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
- namespace std  
- {  
- typedef std::basic_string<_TCHAR>   _tstring;  
- }  
- 
- #define FORMAT_STRING(str, buf, sz, ...)    { sprintf_s(buf, sz, __VA_ARGS__); str = buf; }
- #define FORMAT_WSTRING(str, buf, sz, ...)   { swprintf_s(buf, sz, __VA_ARGS__); str = buf; }
- #define FORMAT_TSTRING(str, buf, sz, ...)   { _stprintf_s(buf, sz, __VA_ARGS__); str = buf; }
- 
- // 用户代码
- _TCHAR buf[512];  
- _tstring str;  
- FORMAT_TSTRING(str, buf, _countof(buf), _T("%s is: %f"), _T("Pi"), M_PI);  
**例子：用 __COUNTER__ 计数值定义掩码常量[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
这种方法限制很多，并不实用，如 MyMask 之后再定义另一个掩码列举型时，会从 __COUNTER__ 的历史值而非 0 开始：
- // 保证 MAKE_MASK 在所有其它使用 __COUNTER__ 代码之前，这样才能
- // 保证第一次 MAKE_MASK 时，产生 2 << 0
- #define MAKE_MASK0(maskname)    maskname = 1
- #define MAKE_MASK(maskname)     maskname = (2 << __COUNTER__)   // 说明 __COUNTER__ 是支持嵌套展开的
- 
- // 用户代码
- enum MyMask  
- {  
-     MAKE_MASK0(MASK_0), //  2^0:    1
-     MAKE_MASK(MASK_1),  //  2^1:    2 << 0
-     MAKE_MASK(MASK_2),  //  2^2:    2 << 1
-     MAKE_MASK(MASK_3),  //  2^3:    2 << 2
-     MAKE_MASK(MASK_4)   //  2^4:    2 << 3
- // 最大 MASK = MASK_31  2^31:   2 << 30
- };  
**例子：用 __FUNCTION__ 打印跟踪函数调用[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
- #define BEGIN_FUNC  _tprintf(_T("%s BEGIN\n"), _T(__FUNCTION__));
- #define END_FUNC    _tprintf(_T("%s END\n"), _T(__FUNCTION__));
- 
- // 用户代码
- void test_funcname_macro()  
- {  
-     BEGIN_FUNC  
- // 函数的功能代码
-     END_FUNC  
- }  
### Windows API 中的注释性宏[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
注释性宏，即是否使用它们不影响编译结果，通常定义为空
目的：
(1). 在源代码中起到注解 (annotation) 和标注 (marker) 作用，便于阅读和理解代码功能
(2). 指导 lint 等静态代码检查工具检查代码缺陷
(3). 指导文档自动生成工具扫描源文件，生成类、函数/API 参考文档
如 WinDef.h 中定义的 IN、OUT、OPTIONAL 用来说明函数参数或类型成员的传入、传出、可选性质
sal.h 中有更完整和复杂的注释性宏，SAL (Source code Annotation Language) 参考 sal.h 源文件和 MSDN: [SAL Annotations](http://msdn.microsoft.com/en-us/library/ms235402.aspx)
Windows API 和 CRT 都用 SAL 注释，几个常用的如下：
__in: 传入参数
__out: 传出参数
__inout: 传入且传出参数
__in_opt, __out_opt, __inout_opt: 可选参数，可以为 NULL
如 CreateFileW() 的声明：
- // WinBase.h
- 
- WINBASEAPI  
- __out  
- HANDLE
- WINAPI  
- CreateFileW(  
-     __in     LPCWSTR lpFileName,  
-     __in     DWORD dwDesiredAccess,  
-     __in     DWORD dwShareMode,  
-     __in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes,  
-     __in     DWORD dwCreationDisposition,  
-     __in     DWORD dwFlagsAndAttributes,  
-     __in_opt HANDLE hTemplateFile  
-     );  
### Windows API 中的常用宏[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
Windows API 包含大量旗标、掩码、状态码、错误码等常量式宏
函数式宏最常用的有：
#### 类型辅助类[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
- BYTE    HIBYTE(WORD wValue)  
- BYTE    LOBYTE(WORD wValue)  
- 
- WORD    HIWORD(DWORD dwValue)  
- WORD    LOWORD(DWORD dwValue)  
- WORD    MAKEWORD(BYTE bLow, BYTE bHigh)  
- 
- LONG    MAKELONG(WORD wLow, WORD wHigh)  
- LRESULT MAKELRESULT(WORD wLow, WORD wHigh)  
- LPARAM  MAKELPARAM(WORD wLow, WORD wHigh)  
- WPARAM  MAKEWPARAM(WORD wLow, WORD wHigh)  
#### GDI 类[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
- DWORD       MAKEROP4(DWORD fore, DWORD back): used in MaskBlt()  
- 
- LONG        DIBINDEX(WORD wColorTableIndex)  
- COLORREF    PALETTEINDEX(WORD wPaletteIndex)  
- 
- COLORREF    PALETTERGB(BYTE bRed, BYTE bGreen, BYTE bBlue)  
- COLORREF    RGB(BYTE byRed, BYTE byGreen, BYTE byBlue)  
- 
- BYTE        GetBValue(DWORD rgb)  
- BYTE        GetGValue(DWORD rgb)  
- BYTE        GetRValue(DWORD rgb)  
- 
- POINTS      MAKEPOINTS(DWORD dwValue)  
另外，BITMAP_WIDTHBYTES(bits) 不在 Windows API 中，但比较常用于位图：
- // 输入：位图图像中一行的逻辑位数 = 位图像素宽 x 每像素位数
- // 输出：位图图像中一行占用的字节数，按 4 Bytes 对齐
- #define BITMAP_WIDTHBYTES(bits)     (((bits) + 31) >> 5 << 2)
#### 错误处理类[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
**标记没有使用的参数、变量辅助宏[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
UNREFERENCED_PARAMETER(P)
DBG_UNREFERENCED_PARAMETER(P)
DBG_UNREFERENCED_LOCAL_VARIABLE(V)
让没有使用的参数、变量不产生编译警告，并且关闭 lint 缺陷检查报告
**错误码、状态码[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
Windows 有三大错误码、状态码空间：
(1). Win32 状态码：GetLastError() 所返回，DWORD 类型，WinError.h 中定义
(2). COM 状态码：COM 函数用，HRESULT 类型，WinError.h 中定义
(3). 内核状态码：内核函数和低级 API 用，NTSTATUS 类型，ntstatus.h 中定义
状态码有关的宏：
- MAKE_HRESULT(sev, fac, code): 将 severity、facility、code 合并为 HRESULT  
- HRESULT_CODE(hr): 取得 HRESULT 的 code 部分  
- HRESULT_FACILITY(hr): 取得 HRESULT 的 facility 部分  
- HRESULT_SEVERITY(hr): 取得 HRESULT 的 severity 位  
- 
- HRESULT_FROM_NT(nt_stat): 从 NTSTATUS 变换到 HRESULT  
- HRESULT_FROM_WIN32(win_err): 从 Win32 状态码变换到 HRESULT  
- 
- SUCCEEDED(hr): HRESULT 是否表示成功  
- FAILED(hr): HRESULT 是否表示失败  
- IS_ERROR(hr): HRESULT 是否表示一个错误  
Win32 状态码没有类似 MAKE_HRESULT 的宏，自定义 Win32 状态码时可以用 mc (Message Compiler) 工具处理 .mc 脚本，自动生成含自定义 Win32 状态码的头文件，同时生成用于 FormatMessage() 的状态码文本描述，参考 MSDN:[Message
 Compiler](http://msdn.microsoft.com/en-us/library/aa385638%28VS.85%29.aspx)
也可以自定义用于 Win32 状态码的 MAKE_WINERR()：
- //  copy from WinError.h
- //
- //  Values are 32 bit values layed out as follows:
- //
- //   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
- //   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
- //  +---+-+-+-----------------------+-------------------------------+
- //  |Sev|C|R|     Facility          |               Code            |
- //  +---+-+-+-----------------------+-------------------------------+
- //
- //  where
- //
- //      Sev - is the severity code
- //
- //          00 - Success
- //          01 - Informational
- //          10 - Warning
- //          11 - Error
- //
- //      C - is the Customer code flag
- //
- //      R - is a reserved bit
- //
- //      Facility - is the facility code
- //
- //      Code - is the facility's status code
- //
- 
- // Win32 状态码的各部分起始位、位掩码和位长度
- 
- #define WINERR_SEVERITY_BIT_LOW         30
- #define WINERR_SEVERITY_MASK            0xC0000000
- #define WINERR_SEVERITY_BIT_LEN         2
- #define WINERR_SEVERITY_VALUE(val)      (((val) << WINERR_SEVERITY_BIT_LOW) & WINERR_SEVERITY_MASK)
- 
- #define WINERR_CUSTOM_DEFINE_BIT_LOW    29
- #define WINERR_CUSTOM_DEFINE_MASK       0x20000000
- #define WINERR_CUSTOM_DEFINE_BIT_LEN    1
- #define WINERR_CUSTOM_DEFINE_FLAG       (1 << WINERR_CUSTOM_DEFINE_BIT_LOW)
- 
- #define WINERR_FACILITY_BIT_LOW         16
- #define WINERR_FACILITY_MASK            0x0FFF0000
- #define WINERR_FACILITY_BIT_LEN         12
- #define WINERR_FACILITY_VALUE(val)      (((val) << WINERR_FACILITY_BIT_LOW) & WINERR_FACILITY_MASK)
- 
- #define WINERR_CODE_BIT_LOW             0
- #define WINERR_CODE_MASK                0x0000FFFF
- #define WINERR_CODE_BIT_LEN             16
- #define WINERR_CODE_VALUE(val)          (val) & WINERR_CODE_MASK
- 
- // Win32 状态码中的严重级别 severity
- 
- #define WINERR_SEVERITY_SUCCESS         0
- #define WINERR_SEVERITY_INFORM          1
- #define WINERR_SEVERITY_WARNING         2
- #define WINERR_SEVERITY_ERROR           3
- #define WINERR_SEVERITY_NOT_CARE        3
- 
- // 自定义 Win32 状态码的宏
- #define MAKE_WINERR(sev, fac, code)     \
-     ((DWORD)(WINERR_SEVERITY_VALUE(sev) | WINERR_CUSTOM_DEFINE_FLAG | WINERR_FACILITY_VALUE(fac) | WINERR_CODE_VALUE(code)))  
#### 调用规范类[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
调用规范/约定参考 MSDN: [Calling Conventions](http://msdn.microsoft.com/en-us/library/k2b2ssfy.aspx)
Windows API 使用的调用规范名称宏，在 WinDef.h 中定义：
- #define CALLBACK    __stdcall
- #define WINAPI      __stdcall
- #define WINAPIV     __cdecl
- #define APIENTRY    WINAPI
- #define APIPRIVATE  __stdcall
- #define PASCAL      __stdcall
COM 常用的调用规范辅助宏：
- EXTERN_C: C 链接约定  
- 
- STDAPI: __stdcall，C 链接约定，返回 HRESULT  
- STDAPI_(type): __stdcall，C 链接约定，返回 type 类型  
- 
- STDMETHOD(method): __stdcall，返回 HRESULT 的类成员虚函数  
- STDMETHOD_(type, method): __stdcall，返回 type 类型的类成员虚函数  
- STDMETHODIMP: __stdcall，返回 HRESULT，对应 STDMETHOD(method) 实现  
- STDMETHODIMP_(type): __stdcall，返回 type 类型，对应 STDMETHOD_(type, method) 实现  
#### 国际化类[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
- WORD    LANGIDFROMLCID(LCID lcid)  
- WORD    MAKELANGID(USHORT primaryLang, USHORT subLang)  
- DWORD   MAKELCID(WORD langID, WORD sortID)  
- DWORD   MAKESORTLCID(WORD langID, WORD sortID, WORD sortVersion)  
- WORD    PRIMARYLANGID(WORD lgid)  
- WORD    SORTIDFROMLCID(LCID lcid)  
- WORD    SORTVERSIONFROMLCID(LCID lcid)  
- WORD    SUBLANGID(WORD lgid)  
#### 资源类[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
- LPTSTR  MAKEINTRESOURCE(WORD wInt)  
- BOOL    IS_INTRESOURCE(WORD wInt)  
#### 网络类[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
- LPARAM  MAKEIPADDRESS(BYTE b0, BYTE b1, BYTE b2, BYTE b3)  
- BYTE    FIRST_IPADDRESS(LPARAM lParam)  
- BYTE    SECOND_IPADDRESS(LPARAM lParam)  
- BYTE    THIRD_IPADDRESS(LPARAM lParam)  
- BYTE    FOURTH_IPADDRESS(LPARAM lParam)  
- LPARAM  MAKEIPRANGE(BYTE low, BYTE high)  
### 字符串化操作符 #[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
将代码中某个名字转换为字符串字面量，即“加引号”，参考 MSDN: [Stringizing Operator](http://msdn.microsoft.com/en-us/library/7e3a913x.aspx)
#### 用 # 操作构造字符串化宏 STRINGIZE[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
- #define __STRINGIZE(x)  # x
- #define _STRINGIZE(x)   __STRINGIZE(x)
- #define _TSTRINGIZE(x)  _T(_STRINGIZE(x))
说明：
(1). # x 产生的是 char 字符串，非 wchar_t 字符串，需配合 _T() 使用
(2). _MACRO() 再次调用 __MACRO() 是一种针对 # 和 ## 操作的常用编写技巧。因为 #、## 操作比较特殊，当它处于宏体中时，不会进行嵌套展开，如 __TSTRINGIZE(NULL) 展开为 "NULL" 而非 "0"，要想嵌套展开，再定义一层 _STRINGIZE() 调用 __STRINGIZE() 即可，_TSTRINGIZE(NULL) 展开为 "0"
#### CRT 中的 STRINGIZE 定义[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
CRT 中有类似上面的 STRINGIZE()，以及宽字符化字面量宏 _CRT_WIDE() 的定义：
- // crtdefs.h
- #ifndef _CRT_STRINGIZE
- #define __CRT_STRINGIZE(_Value) #_Value
- #define _CRT_STRINGIZE(_Value) __CRT_STRINGIZE(_Value)
- #endif
- 
- #ifndef _CRT_WIDE
- #define __CRT_WIDE(_String) L ## _String
- #define _CRT_WIDE(_String) __CRT_WIDE(_String)
- #endif
#### STRINGIZE 的展开规则[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
1. 如果 _STRINGIZE() 的参数是宏，那么宏代表的实际值也将被展开，即嵌套展开
**例子：用 STRINGIZE 查看宏的展开结果[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
查看某个宏在当前编译配置 (Debug/Release, ANSI/Unicode) 下，实际表示的东西，如某个 _t 系列函数、Windows API 究竟表示哪个函数，可以利用 _STRINGIZE()：
- // 将输出实际的行号、数字，而非字符串 "__LINE__"、"MAX_PATH"
- _tprintf(_T("Line: %s\n"), _TSTRINGIZE(__LINE__));  
- _tprintf(_T("MAX_PATH: %s\n"), _TSTRINGIZE(MAX_PATH));  
- 
- // 判断宏的当前值、调用了哪个版本的 _t 系列函数、Windows API
- _tprintf(_T("_DEBUG: %s, _UNICODE: %s\n"), _TSTRINGIZE(_DEBUG), _TSTRINGIZE(_UNICODE));  
- _tprintf(_T("_tprintf: %s\n"), _TSTRINGIZE(_tprintf));  
- _tprintf(_T("CreateFile: %s\n"), _TSTRINGIZE(CreateFile));  
输出结果：
- Line: 24  
- MAX_PATH: 260  
- _DEBUG: 1, _UNICODE: 1  
- _tprintf: wprintf  
- CreateFile: CreateFileW  
2. 如果 _STRINGIZE() 的参数单纯的变量、函数、类型、const、enum 常量，那么只是将 _STRINGIZE() 括号中的东西加引号而已，如下：
- // 非 const、其它内部类型 double、char，结果都一样
- constint val = 260;  
- 
- // 枚举常量
- enum MUSIC_STATE  
- {  
-     ST_STOP,  
-     ST_PLAY,  
-     ST_PAUSE,  
-     ST_BUTT  
- };  
- 
- // 自定义结构、类
- ClassTest obj;  
- 
- // 函数
- void func(int a);  
- 
- // 下面输出 _TSTRINGIZE() 括号中名字加上引号得到的字符串，而非实际变量值
- _tprintf(_T("int: %s, val: %s\n"), _TSTRINGIZE(int), _TSTRINGIZE(val));  
- _tprintf(_T("MUSIC_STATE: %s, ST_STOP: %s\n"), _TSTRINGIZE(MUSIC_STATE), _TSTRINGIZE(ST_STOP));  
- _tprintf(_T("ClassTest: %s, obj: %s\n"), _TSTRINGIZE(ClassTest), _TSTRINGIZE(obj));  
- _tprintf(_T("func: %s\n"), _TSTRINGIZE(func));  
输出结果：
- int: int, val: val  
- MUSIC_STATE: MUSIC_STATE, ST_STOP: ST_STOP  
- ClassTest: ClassTest, obj: obj  
- func: func  
### 拼接操作符 ##[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
将代码中两个名字拼接到一起，形成一个名字。## 操作“不加引号”，参考 MSDN: [Token-Pasting Operator](http://msdn.microsoft.com/en-us/library/09dwwt6y.aspx)
- #define __CONCAT(x, y)  x ## y
- #define _CONCAT(x, y)   __CONCAT(x, y)
## 与 # 一样对其操作数不进行嵌套展开，所以 __CONCAT(aaa, __CONCAT(bbb, ccc)) 的展开结果是 aaa__CONCAT(bbb, ccc)，而 _CONCAT(aaa, _CONCAT(bbb, ccc)) 的展开结果是 aaabbbccc。## 的结果是名字拼接，而不是字符串字面量，即不是 "aaabbbccc"
通常用 ## 操作拼接构造类型、变量、函数的名字
**例子：_T() 的定义[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
- // tchar.h
- #ifdef _UNICODE
- #define __T(x)      L ## x
- #else
- #define __T(x)      x
- 
- #define _T(x)       __T(x)
**例子：Windows API 通用句柄类型的定义[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
- // winnt.h
- typedefvoid *HANDLE;  
- #define DECLARE_HANDLE(name) struct name##__ { int unused; }; typedef struct name##__ *name
- 
- // 因此多数 Windows 句柄是指向桩结构的指针，如 HWND：
- 
- // windef.h
- DECLARE_HANDLE  (HWND);  
- 
- // HWND 定义展开后是：
- struct HWND__  
- {  
- int unused;  
- };  
- typedefstruct HWND__ *HWND;  
**例子：用 ## 构造函数名[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
- // 音乐播放状态常量
- enum MUSIC_STATE  
- {  
-     ST_STOP,  
-     ST_PLAY,  
-     ST_PAUSE,  
-     ST_BUTT  
- };  
- 
- // 音乐播放状态结构
- // 里面有一个用于处理特定状态的回调函数 stat_proc
- typedefstruct _MusicState  
- {  
-     MUSIC_STATE     stat;  
- const _TCHAR*   stat_name;  
- int             (*stat_proc)(void*);  
- } MusicState;  
- 
- // 处理特定音乐播放状态的函数
- // 函数名的统一形式 proc_ ## stat，stat 是状态常量的名字
- int proc_ST_STOP(void*);  
- int proc_ST_PLAY(void*);  
- int proc_ST_PAUSE(void*);  
- 
- // 初始化音乐播放状态结构
- #define INIT_MUSIC_STATE(stat)  {stat, _TSTRINGIZE(stat), proc_ ## stat}
- 
- MusicState g_MusicState[ST_BUTT] =  
- {  
-     INIT_MUSIC_STATE(ST_STOP),  
-     INIT_MUSIC_STATE(ST_PLAY),  
-     INIT_MUSIC_STATE(ST_PAUSE)  
- };  
### TCHAR 统一字符类型和处理[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
_TCHAR、_T()、_t 系列函数等东西叫做 Generic-Text Mapping，即使用宏进行统一字符类型编写，在不同的字符集编码工程配置 ANSI/UNICODE/MBCS 下替换为不同的实际函数或类型，参考 MSDN:[Generic-Text
 Mappings](http://msdn.microsoft.com/en-us/library/ms861474.aspx),[Using Generic-Text Mappings](http://msdn.microsoft.com/en-us/library/7dzey6h6.aspx), [Using
 TCHAR.H Data Types with _MBCS](http://msdn.microsoft.com/en-us/library/46azw2kc.aspx)
工程的字符集配置的宏定义：
ANSI (SBCS, ASCII): _UNICODE 和 _MBCS 均未定义，使用 char 单字节字符集编码
UNICODE: _UNICODE 定义，使用 wchar_t 宽字符集编码，VC 默认 wchar_t 2 字节
MBCS: _MBCS 定义，使用 char 变长字符集编码，一个字符占一个或多个 char
#### _TCHAR, _TEXT()/_T(), _t 系列函数[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
根据 _UNICODE、_MBCS 的定义，调用 ANSI/UNICODE/MBCS 不同字符集版本的 CRT 函数，或产生字面量，多在 tchar.h 中声明。_t 字符操作函数参考 MSDN:[String
 Manipulation (CRT)](http://msdn.microsoft.com/en-us/library/f0151s4x.aspx)
#### TCHAR, LPTSTR/LPCTSTR, TEXT(), A/W 版本 Windows API[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
根据 UNICODE 的定义，调用 ANSI/UNICODE 不同字符集版本的 Windows API，或产生字面量，多在 WinBase.h、Windows.h 中声明
不成文约定：带 _ 前缀的代码，通常对应 CRT，而不带 _ 前缀的东西，通常对应 Windows API。A/W 版本 API 都是接收字符串参数的函数，但并非所有接收字符串的 API 都有 A/W 两个版本，如 GetProcAddress() 只是 A 版本函数，因为 DLL 中的导出符号用 ASCII 英文足够了
### 宏的缺点和替代方法[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
宏是预编译行为，所做的是名字替换，它的缺点和替代方法如下：
#### 宏难于调试[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
编译时，宏不会产生用于调试的名字符号。如 #define MAX_PATH 260，在调试时，无法找到其符号名 MAX_PATH，而只是 260 数字
常量式宏可以用 const 和 enum 代替，在调试中可以查看 const、enum 的符号名，并且 const、enum 和宏的运行时开销是相同的（有使用 const、enum 时才会分配内存）：
- constchar* DEF_FILENAME = "default.txt";  
- 
- enum BUF_SIZE  
- {  
-     BUF_SIZE_SMALL      = 64,  
-     BUF_SIZE_MEDIUM     = 256,  
-     BUF_SIZE_LARGE      = 1024  
- };  
另外，在 VC 2005 中，进行 C/C++ 源码级别调试时，函数式宏无法像 inline 或普通函数一样使用 Step into 进入宏定义体的代码，宏调用被视为一条语句，只能使用 Go To Definition (F12) 跳转到宏定义处查看代码，而不能调试
#### 宏的使用缺陷[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
(1). 宏以字面形式展开，有副作用，典型的有两种：
(a). 宏参数不加括号展开时改变逻辑，如 #define RECT_AREA(x, y) (x * y)
解决方法：定义宏时给参数的使用加上括号，如 #define RECT_AREA(x, y) ((x) * (y))
(b). 宏体为多行语句，如果放到判断语句中，并且不加 {} 包起来，只有第一句在判断语句下执行，其它在判断语句外，如下例：
- #define SWAP(v1, v2, tmp)   \
-     tmp = v1;               \  
-     v1 = v2;                \  
-     v2 = tmp;  
- 
- // 用户代码
- if (condition)  
-     SWAP(a, b, t);  // 逻辑问题
- 
- if (condition) {  
-     SWAP(a, b, t);  // OK
- }  
解决方法：定义宏时用 {} 或 do {} while(0) 包起来，如下：
- #define SWAP(v1, v2, tmp)   \
- do {                    \  
-         tmp = v1;           \  
-         v1 = v2;            \  
-         v2 = tmp;           \  
-     } while (0)  
(2). 宏对参数没有类型检查，宏的返回也不具有类型
(3). 函数式宏，不是函数，不能将其宏名作为函数指针，即不能进行函数回调；也不能进行递归调用
函数式宏大多能用 inline 函数 + 函数 template 的方式代替，并保持相同的运行时开销。但因为 inline 函数是一种 尽力而为 (Try My Best) 的编译器指示（inline 函数不一定 inline 化，inline 化的程度也不同），实际的开销根据 inline 函数调用复杂程度（是否有递归、作为函数指针）、不同编译器、不同的工程配置（Debug/Release、编译选项、编译优化级别），inline 化有所不同
参考 MSDN: [Inline Functions versus Macros](http://msdn.microsoft.com/en-us/library/bf6bf4cf.aspx)
#### 宏造成全局名字空间污染[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
宏是全局名字空间的，容易造成名字污染、干扰，可用 const、enum、inline 解决。如下：
- class TestClass1  
- {  
- private:  
- int m_Val;  
- 
- // private 限制对宏 MACRO_DEF_VAL 不起作用
- #define MACRO_DEF_VAL   128
- 
- public:  
- staticconstint CONST_DEF_VAL = 128;  
- enum { ENUM_DEF_VAL = 128 };  
- };  
- 
- class TestClass2  
- {  
- private:  
- int m_Val;  
- 
- // 产生 C4005 警告：MACRO_DEF_VAL 被重复定义
- #define MACRO_DEF_VAL   256
- 
- public:  
- staticconstint CONST_DEF_VAL = 256;  
- enum { ENUM_DEF_VAL = 256 };  
- };  
- 
- // 用户代码
- 
- // 宏 MACRO_DEF_VAL 是全局的，不能写为 TestClass1::MACRO_DEF_VAL
- _tprintf(_T("TestClass1: %d, %d, %d\n"), MACRO_DEF_VAL, TestClass1::CONST_DEF_VAL, TestClass1::ENUM_DEF_VAL);  
- _tprintf(_T("TestClass2: %d, %d, %d\n"), MACRO_DEF_VAL, TestClass2::CONST_DEF_VAL, TestClass2::ENUM_DEF_VAL);  
输出结果：
后面定义的宏 MACRO_DEF_VAL 的值将前面的覆盖了：
- TestClass1: 256, 128, 128  
- TestClass2: 256, 256, 256  
#### 优先使用宏的情况[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
不是所有的宏都能用 const、enum、inline 函数代替：
(1). 对于一些不对应单个函数、变量、常量，并且编码量大、结构重复的整块代码，宏是最合适的选择，如 MFC 的 RTTI 支持和消息映射结构
(2). 如 例子：C/C++ 预定义宏的取值 “说明 (2)”中所示，需要嵌套展开 __FILE__、__LINE__、__FUNCTION__ 等预定义宏的情况，必需用宏，而不能用 inline 函数
### 条件编译[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
#if/#else/#elif/#ifdef/#ifndef/#if defined/#if !defined
#ifdef XXX 等价于 #if defined (XXX)
#ifndef XXX 等价于 #if !defined (XXX)
参考 MSDN: [The #if, #elif, #else, and #endif Directives](http://msdn.microsoft.com/en-us/library/ew2hz0yd.aspx)
**例子：注释大量代码[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
- #if 0
- XXXXXXXXX  
- #endif
- 
- #if FALSE
- XXXXXXXXX  
- #endif
**例子：MFC 中的调试版代码示例[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
见上文“MFC 的调试版 new”
AssertValid() 参考 MSDN: [MFC ASSERT_VALID and CObject::AssertValid](http://msdn.microsoft.com/en-us/library/38z04tfa.aspx)
- // FrameWindow、Doc、View 等类均可覆盖以下用于调试的诊断函数
- // 然后可以用 ASSERT_VALID() 诊断其对象状态是否有效
- 
- #ifdef _DEBUG
- virtualvoid AssertValid() const;  
- virtualvoid Dump(CDumpContext& dc) const;  
- #endif
- 
- // Release 版本的 GetDocument() 是 inline 的
- #ifndef _DEBUG
- inline CMyDoc* CMyView::GetDocument() const
- {  
- returnreinterpret_cast<CMyDoc*>(m_pDocument);  
- }  
- #endif
**例子：DLL 工程导出符号[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
工程 DllProj 中导出符号（变量、函数、类）的方法：在工程中定义宏 DLLPROJ_EXPORTS (/D "DLLPROJ_EXPORTS")，并在使用工程中保证没有定义 DLLPROJ_EXPORTS
DllProj 导出符号的声明文件 DllProj.h 如下，在使用工程中 #include 该文件：
- // DllProj.h
- 
- #ifndef _DLLPROJ_H_
- #define _DLLPROJ_H_
- 
- #ifdef DLLPROJ_EXPORTS
- #define DLLPROJ_API __declspec(dllexport)
- #else
- #define DLLPROJ_API __declspec(dllimport)
- #endif
- 
- #ifdef __cplusplus
- #define EXTERN_C        extern "C"
- #define EXTERN_C_BEGIN  extern "C" {
- #define EXTERN_C_END    }
- #else   // __cplusplus defined
- #define EXTERN_C        extern
- #define EXTERN_C_BEGIN
- #define EXTERN_C_END
- #endif  // __cplusplus NOT defined
- 
- // 导出类
- class DLLPROJ_API TestClass  
- {  
- public:  
-     TestClass();  
- };  
- 
- // 导出全局变量，以 C 的链接方式（修饰名、调用约定）
- EXTERN_C DLLPROJ_API int g_TestVal;  
- 
- // 导出函数
- DLLPROJ_API int TestFunc();  
- 
- #endif  // _DLLPROJ_H_
**例子：用 #undef 解决 wxWidgets 自定义事件链接 BUG[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
BUG 参考：[wxEvent derived event](http://forums.wxwidgets.org/viewtopic.php?t=17943&highlight=),[Custom
 Events](http://wiki.wxwidgets.org/Custom_Events#Creating_a_Custom_Event_.28Method_4.29)
**BUG 触发条件**：以 DLL 方式使用 wxWidgets Windows 版本，即定义了 WXUSINGDLL，使用 DECLARE_EVENT_TYPE() 定义自定事件
**BUG 表现**：以 __declspec(dllimport) 修饰事件标识，实际上事件标识应该是一个模块内变量，而非导入变量，出现链接问题。MinGW GCC 4 报链接错误，VC 2005 报链接警告：warning C4273: inconsistent dll linkage。BUG 的具体原因请跟踪 wxWidgets 源码 event.h 中的 DECLARE_EVENT_TYPE() 和 dlimpexp.h 中的 WXDLLIMPEXP_CORE 定义
**BUG 典型工程**：开源下载器 [MultiGet](http://multiget.sourceforge.net/) svn version 3
**BUG 解决方法**：
方法 1. 不使用旧的 DECLARE_EVENT_TYPE() 而使用 DECLARE_LOCAL_EVENT_TYPE() 定义自定事件
方法 2. 使用 DECLARE_EVENT_TYPE() 前后包含 undefine_WXDLLIMPEXP_CORE.h、redefine_WXDLLIMPEXP_CORE.h 头文件，以便取消和重定义 WXDLLIMPEXP_CORE
方法 2 中的 undefine_WXDLLIMPEXP_CORE.h、redefine_WXDLLIMPEXP_CORE.h 以及使用方法如下：
- // undefine_WXDLLIMPEXP_CORE.h
- 
- // 不要用 #pragma once 等包含一次技巧，因为一个源文件中可能有多个
- // BEGIN_DECLARE_EVENT_TYPES 自定义事件块，这时要多次包含本文件
- 
- #ifdef WXDLLIMPEXP_CORE
- #   define REMOVE_WXDLLIMPEXP_CORE
- #   undef WXDLLIMPEXP_CORE      // 先取消 WXDLLIMPEXP_CORE 定义
- #   define WXDLLIMPEXP_CORE     // 再将其定义为空
- #endif
- // redefine_WXDLLIMPEXP_CORE.h
- 
- // 不要用 #pragma once 等包含一次技巧
- 
- #ifdef REMOVE_WXDLLIMPEXP_CORE
- #   undef WXDLLIMPEXP_CORE
- // 以下块拷贝自 wx-2.8.10 dlimpexp.h，用于恢复 WXDLLIMPEXP_CORE 的原有定义
- // BEGIN
- #   ifdef WXMAKINGDLL_CORE
- #       define WXDLLIMPEXP_CORE WXEXPORT
- #       define WXDLLIMPEXP_DATA_CORE(type) WXEXPORT type
- #   elif defined(WXUSINGDLL)
- #       define WXDLLIMPEXP_CORE WXIMPORT
- #       define WXDLLIMPEXP_DATA_CORE(type) WXIMPORT type
- #   else /* not making nor using DLL */
- #       define WXDLLIMPEXP_CORE
- #       define WXDLLIMPEXP_DATA_CORE(type) type
- #   endif
- // END
- #   undef REMOVE_WXDLLIMPEXP_CORE
- #endif
- // 用户代码
- 
- // 定义 WXDLLIMPEXP_CORE 为空
- #include <undefine_WXDLLIMPEXP_CORE.h>
- 
- // 自定义事件
- BEGIN_DECLARE_EVENT_TYPES()  
-     DECLARE_EVENT_TYPE(wxEVENT_TEST_TRIGGERED, wxID_ANY)  
- // DECLARE_LOCAL_EVENT_TYPE(wxEVENT_TEST_TRIGGERED, wxID_ANY)   // 用这个不会有 BUG
- END_DECLARE_EVENT_TYPES()  
- 
- // 重定义 WXDLLIMPEXP_CORE
- #include <redefine_WXDLLIMPEXP_CORE.h>
**说明**：
(1). #undef 是 #define 的反操作，取消宏定义，而不是将宏定义为空。取消后的宏名可以再次定义，而不产生重定义问题
(2). 大量嵌套的 #if 条件编译结构，可使用这种预处理缩进方法
### 预编译头文件[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
预编译头文件 PCH (Precompiled Header) 是对某个编译单元的编译结果 (.pch)，通常这个编译单元命名为 [stdafx.cpp include => stdafx.h]（VC 工程标准）或 [common.cpp include => common.h]。与常规的编译结果 (.obj) 不同的是，如果 .pch 的编译单元源码在两次工程编译期间不改变，则重新编译工程时，不会重新编译 .pch
PCH 的特点使它的源码如 stdafx.h，适合放入很少更改的代码，如标准库、运行时库、系统 API、第三方库的头文件，以及工程全局的设置和名字符号，在重新编译工程时，这些代码便不会重新编译，以加快编译速度
#### 使用 PCH 的编译命令[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
以 VC 工程标准的 [stdafx.cpp include => stdafx.h] 预编译头文件编译单元为例，产生、使用 PCH 的编译命令选项如下：
- 
对除了 stdafx.cpp 之外的其它编译单元 .c/.cpp（没有 .h/.hpp，.h/.hpp 是通过 #include 展开到 .c/.cpp 形成编译单元的），使用如下编译选项（Debug 工程配置），如果使用 VC IDE 则在工程属性页中设置：
/Yu"stdafx.h" /Fp"Debug\ProjName.pch"
/Yu 表示通过 stdafx.h 使用 PCH，/Fp 指定使用的 PCH 路径为 Debug\ProjName.pch
- 
对 stdafx.cpp，如果使用 VC IDE 则在 stdafx.cpp 的属性页中设置，使用如下编译选项：
/Yc"stdafx.h" /Fp"Debug\ProjName.pch"
/Yc 表示通过 stdafx.h 产生 PCH，/Fp 指定产生的 PCH 路径为 Debug\ProjName.pch
PCH 的详细方法参考 MSDN: [Creating Precompiled Header Files](http://msdn.microsoft.com/en-us/library/szfdksca.aspx)
**例子：典型的 MFC 工程预编译头 stdafx.h 代码[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
- ////////////////////////////////////////////////////////////////////////////////
- ///
- /// @file       stdafx.h
- /// @brief      Windows 标准预编译头文件
- ///
- /// 将标准库、运行时库、基本库、Windows API、第三方库的头文件在这里包含，生成
- /// 预编译头文件 MFCBasic.pch
- ///
- /// 如果不修改 stdafx.h，增量编译时便不会重新编译 stdafx.h 中包含的头文件，这样
- /// 加快了编译速度
- ///
- /// @version    <version>
- /// @author     <author>
- /// @date       2011-07
- ///
- /// Copyright (c) 2011, <company>
- /// All rights reserved.
- ///
- ////////////////////////////////////////////////////////////////////////////////
- 
- // 典型的“只包含一次”条件编译技巧
- // VC cl 编译器版本 10 以上 (_MSC_VER > 1000) 也可以使用 #pragma once 指令
- #ifndef _STDAFX_H_
- #define _STDAFX_H_
- 
- // 排除很少使用的 Windows 头文件
- 
- #define WIN32_LEAN_AND_MEAN     // 适用于 Windows API
- 
- #ifndef VC_EXTRALEAN
- #define VC_EXTRALEAN            // 适用于 MFC
- #endif
- 
- // 指定目标系统和环境 (Windows, IE) 的版本号
- 
- #ifndef WINVER
- #define WINVER          0x0501  // 目标系统具有 Windows XP 及以上特性
- #endif
- 
- #ifndef _WIN32_WINNT
- #define _WIN32_WINNT    0x0501  // 目标系统具有 Windows XP 及以上特性
- #endif
- 
- #ifndef _WIN32_WINDOWS
- #define _WIN32_WINDOWS  0x0410  // 目标系统具有 Windows 98 及以上特性
- #endif
- 
- #ifndef _WIN32_IE
- #define _WIN32_IE       0x0600  // 目标系统具有 IE 6.0 及以上特性
- #endif
- 
- ////////////////////////////////////////////////////////////////////////////////
- /// Include Header
- ////////////////////////////////////////////////////////////////////////////////
- 
- // C 标准库与运行时库 (CRT)
- // BEGIN
- //
- #define _CRT_SECURE_NO_DEPRECATE    // 使用废弃 (deprecated) 的 CRT 函数时，不产生编译警告
- #define _CRT_SECURE_NO_WARNINGS     // 典型的废弃函数有不带缓冲区大小检查的 strcpy()、strcat()、sprintf() 等
- 
- #include <stdlib.h>
- #include <tchar.h>
- #include <crtdbg.h>
- #include <string.h>
- //
- // END
- 
- // C++ 标准库
- // BEGIN
- //
- #include <exception>
- #include <typeinfo>
- //
- // END
- 
- // MFC 库
- // BEGIN
- //
- #ifndef _SECURE_ATL
- #define _SECURE_ATL 1                       // ATL/MFC 的安全设置
- #endif
- 
- #define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS  // 使 ATL/MFC 的 CString 具有显式地构造函数 (explicit)
- 
- #define _AFX_ALL_WARNINGS                   // 打开 MFC 的所有警告，包括一般可以安全忽略的警告
- 
- #include <afxwin.h>     // MFC 核心和标准支持
- #include <afxext.h>     // MFC 扩展支持
- 
- #ifndef _AFX_NO_OLE_SUPPORT
- #include <afxdtctl.h>   // MFC 的 IE 4 通用控件支持
- #endif
- 
- #ifndef _AFX_NO_AFXCMN_SUPPORT
- #include <afxcmn.h>     // MFC 的 Windows 通用控件支持
- #endif
- //
- // END
- 
- // Windows API
- // BEGIN
- //
- // #include <Windows.h>     // 使用 MFC 库时不要包含 Windows.h，MFC 头文件中已包含
- #include <Winsock2.h>
- //
- // END
- 
- // Windows 通用控件 ComCtl32.dll 版本 6.0 的内嵌 manifest
- // BEGIN
- //
- #ifdef _UNICODE
- #if defined _M_IX86
- #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
- #elif defined _M_IA64
- #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
- #elif defined _M_X64
- #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
- #else
- #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
- #endif
- #endif  // _UNICODE
- //
- // END
- 
- #endif  // _STDAFX_H_
### 常用预处理指令[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
VC 支持的预处理指令参考 MSDN: [Preprocessor Directives](http://msdn.microsoft.com/en-us/library/3sxhs2ty.aspx)
#### #error 产生人工编译错误[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
#error 产生 fatal error，编译器输出 #error 后的提示文本。指示该源文件必需使用 C++ 方式编译，如下：
- // test.cpp
- #ifndef __cplusplus
- #error MUST use C++ compilation
- #endif
以 C 语言方式编译上面源文件 (/Tc test.cpp) 时报错：
fatal error C1189: #error :  MUST use C++ compilation
#### #line 改变行号和源文件名[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
#line 改变 __FILE__ 和 __LINE__ 的取值，例如：
- // 实际文件名：test_01.cpp
- #line 1 "test_02.cpp"
-     _tprintf(_T("File: %s, Line: %d\n"), _T(__FILE__), __LINE__);   // 实际第 200 行
输出：
File: test_02.cpp, Line: 1
#### # 空指令[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
没有作用的合法预处理指令行正则表达式：[\t ]*#[\t ]*
### #pragma 预处理指令[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
#pragma 是一组编译器特定的预处理指令，每种编译器的 #pragma 的子指令都有所不同。VC 的 #pragma 指令参考 MSDN: [Pragma Directives and the __Pragma
 Keyword](http://msdn.microsoft.com/en-us/library/d9x1s805.aspx)
常用的 #pragma 指令如下：
#### #pragma once 只包含一次头文件[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
对头文件只包含一次，如下：
- // test.h
- #if _MSC_VER > 1000
- #pragma once
- #endif
- 
- // 头文件中的代码
它和传统的 #ifndef 只包含一次技巧的功能相同：
- // test.h
- #ifndef _TEST_H_
- #define _TEST_H_
- 
- // 头文件中的代码
- 
- #endif  // _TEST_H_
在源文件中多次 #include 包含 test.h 时，不会出现 redefinition 错误
#### #pragma message 编译时输出消息[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
#pragma message 在编译过程中，向标准输出或 VC 的 Output 窗口打印指定消息，作用：(1) 告知程序员代码编译和使用的注意事项 (2) 用于查看和诊断实际的编译代码
**例子：用 #pragma message 和 STRINGIZE 查看宏的展开结果[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
例 11 是用 STRINGIZE 在运行时输出宏的展开结果，其实在编译时也可以用 #pragma message 输出，诊断编译的实际代码：
- // 将输出实际的行号、数字，而非字符串 "__LINE__"、"MAX_PATH"
- #pragma message("Line: " _STRINGIZE(__LINE__))
- #pragma message("MAX_PATH: " _STRINGIZE(MAX_PATH))
- 
- // 判断宏的当前值、调用了哪个版本的 _t 系列函数、Windows API
- #pragma message("_DEBUG: " _STRINGIZE(_DEBUG) ", _UNICODE: " _STRINGIZE(_UNICODE))
- #pragma message("_tprintf: " _STRINGIZE(_tprintf))
- #pragma message("CreateFile: " _STRINGIZE(CreateFile))
在标准输出或 VC 的 Output 窗口输出：
- Line: 209  
- MAX_PATH: 260  
- _DEBUG: 1, _UNICODE: 1  
- _tprintf: wprintf  
- CreateFile: CreateFileW  
#### #pragma push_macro/pop_macro 保存和恢复宏定义[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
#pragma push_macro/pop_macro 用来解决宏命名冲突问题，如下：
- // 保存来自 windef.h 的 MAX_PATH 定义
- #pragma message("MAX_PATH: " _STRINGIZE(MAX_PATH))
- #pragma push_macro("MAX_PATH")
- 
- // 对 MAX_PATH 进行新的定义
- // 即使之前没有定义 MAX_PATH，#undef MAX_PATH 也不会报错
- #undef MAX_PATH
- #define MAX_PATH    512
- #pragma message("MAX_PATH: " _STRINGIZE(MAX_PATH))
- 
- // 使用新的 MAX_PATH
- 
- // 恢复 windef.h 的 MAX_PATH 定义
- #pragma pop_macro("MAX_PATH")
- #pragma message("MAX_PATH: " _STRINGIZE(MAX_PATH))
#pragma message 输出如下：
- MAX_PATH: 260  
- MAX_PATH: 512  
- MAX_PATH: 260  
#### #pragma warning 禁用和启用编译警告[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
例子：
- // 禁用 C4507、C4034 警告（VC 的 warning 编号从 C4001 开始）
- // 只报告一次 C4385 警告
- // 将 C4164 警告作为编译错误
- #pragma warning(disable: 4507 34; once: 4385; error: 164)
- 
- #pragma warning(push)           // 保存当前的警告设置：全局警告级别和 disable 的
- #pragma warning(disable: 4705)  // 禁用某些警告
- #pragma warning(disable: 4706)
- #pragma warning(disable: 4707)
- 
- // 会产生 C4705、C4706、C4707 警告的代码
- #pragma warning(pop)            // 恢复保存的警告设置
#### #pragma comment 目标文件注释和编译选项传递[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
#pragma comment 的作用是在编译或链接过程中，向 COFF 二进制目标文件或 PE 可执行文件 (.obj/.lib/.exe/.dll) 中插入字符串注释。目的：
(1). 将版本、版权等信息插入到 COFF/PE 文件中，以便发布
(2). 插入的字符串会作为后续编译阶段（如链接）的选项，以便支持如 auto-link 等在源码中设置编译、链接选项
**例子：用 #pragma comment(lib) 实现库的 auto-link[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
以例 17 DllProj 工程为基础，DllProj 定义有 DLLPROJ_EXPORTS 宏，DllProj.h 如下：
- // DllProj.h
- 
- #ifndef _DLLPROJ_H_
- #define _DLLPROJ_H_
- 
- #ifdef DLLPROJ_EXPORTS
- #define DLLPROJ_API __declspec(dllexport)
- #else
- #pragma comment(lib, "DllProj.lib")     // 指示在导入符号时链接 DllProj.lib
- #define DLLPROJ_API __declspec(dllimport)
- #endif
- 
- // 省略代码
- 
- #endif  // _DLLPROJ_H_
在使用工程中 #include 该文件，并设置库搜索路径 /LIBPATH，不必指定链接导入库 DllProj.lib
**#pragma comment(linker) 传递链接选项[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
#pragma comment(linker, "link_option")
link_option 只能为以下链接选项：
/DEFAULTLIB
/EXPORT
/INCLUDE
/MANIFESTDEPENDENCY
/MERGE
/SECTION
**#pragma comment(linker, "/SECTION") 设置区段属性[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
设置区段属性的方法有：
(1). 使用模块定义文件 .def 的 SECTIONS 定义，见 MSDN: [SECTIONS (C/C++)](http://msdn.microsoft.com/en-us/library/h82ftb38.aspx)
(2). 使用 /SECTION 链接选项，见 MSDN: [/SECTION (Specify Section Attributes)](http://msdn.microsoft.com/en-us/library/sf9b18xk.aspx)
(3). 使用 #pragma section 指令创建区段
对于上面的方法 (2)，可用 #pragma comment(linker) 指定链接选项
例子：#pragma comment(linker, "/SECTION") 设置可读写、共享区段
- #pragma comment(linker, "/SECTION:.mydata,RWS")
- #pragma data_seg(".mydata")
- 
- // .mydata 区段中的变量定义
- 
- #pragma data_seg()
#### #pragma 区段操作[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
区段属性和标准区段名参考 /SECTION 链接选项。查看区段偏移地址 (RVA)、起始地址和属性，用 dumpbin 工具：
dumpbin /SECTION:secname xxx.exe|xxx.dll
自定义区段名不能和标准区段名冲突。自定义区段名长度限制为 8 个字符，超过 8 个会截断
**#pragma section 在目标文件中创建区段[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
#pragma section 在.obj 中创建区段，并设置属性 read, write, execute, shared, nopage, nocache, discard, remove
#pragma section 新建的区段不包括任何内容，需向新建的区段放置数据或代码：
(1). 对于数据区段（全局变量）
(a). __declspec(allocate("secname")) 修饰
(b). #pragma bss_seg/const_seg/data_seg 指令
(2). 对于代码区段（函数）
(a). #pragma alloc_text 指令
(b). #pragma code_seg 指令
**例子：#pragma section 创建可读写、共享区段**
- #pragma section(".mydata", read, write, shared)     // 在 .obj 中新建可读写、共享区段 .mydata
- // #pragma comment(linker, "/SECTION:.mydata,RWS")  // 作用与上类似：在链接时调整区段属性
- #pragma data_seg(".mydata")                         // 将以下初始化数据放置到 .mydata
- 
- // .mydata 区段中的变量定义
- 
- #pragma data_seg()                                  // 恢复默认的初始化数据区段 .data
- 
- __declspec(allocate(".mydata"))                     // 用  __declspec(allocate) 放置数据到区段
- int g_Var = 0;  
**#pragma alloc_text 将 C 链接约定的函数放置到区段[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
#pragma alloc_text 只能应用于 C 链接约定的函数，即对于 C++ 编译方式，需用 extern "C" 声明函数。所以 #pragma alloc_text 不支持重载函数和类成员函数
#pragma alloc_text 在函数声明与函数定义体之间使用
例子：在可执行、非分页区段中，用 #pragma alloc_text 放置 C 链接约定函数
- extern"C"void TestFunc_1();                           // 必需在 .mycode 之前有函数声明
- extern"C"void TestFunc_2();                           // 并且是 C 链接约定的
- 
- #pragma section(".mycode", read, execute, nopage)       // 建立可执行、非分页区段
- // #pragma comment(linker, "/SECTION:.mycode,RE!P")     // 作用与上类似：在链接时调整区段属性
- #pragma alloc_text(".mycode", TestFunc_1, TestFunc_2)   // 将指定函数放到 .mycode 中
- 
- void TestFunc_1()  
- {  
- // TestFunc_1 函数体
- }  
- 
- void TestFunc_2()  
- {  
- // TestFunc_2 函数体
- }  
**#pragma code_seg 将函数放置到代码区段[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
#pragma code_seg/bss_seg/const_seg/data_seg 会新建区段，之前不必有 #pragma section；如果之前有 #pragma section，会使用 #pragma section 设置的区段属性
如果省略参数，会放置到标准区段：
#pragma code_seg(): .text
#pragma data_seg(): .data
#pragma bss_seg(): .bss
#pragma const_seg(): .rdata
例子：在可执行、非分页区段中，用 #pragma code_seg 放置函数
- #pragma section(".mycode", read, execute, nopage)       // 建立可执行、非分页区段
- // #pragma comment(linker, "/SECTION:.mycode,RE!P")     // 作用与上类似：在链接时调整区段属性
- 
- #pragma code_seg(".mycode")                             // 将以下函数放到 .mycode 区段中
- void TestFunc_1()  
- {  
- // TestFunc_1 函数体
- }  
- 
- void TestFunc_2()  
- {  
- // TestFunc_2 函数体
- }  
- #pragma code_seg()                                      // 恢复默认的标准代码区段 .text
**#pragma data_seg/bss_seg/const_seg 将数据放置到区段[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
数据性质和区段有对应关系，如果放置区段和数据性质冲突，则不会实际放到该区段中：
(1). .data 标准区段，放置初始化非 0 全局数据。用 #pragma data_seg 放置初始化数据，必需显示初始化其中变量（可以初始化为 0），否则不会放入 #pragma data_seg 指定的区段
(2). .bss 标准区段，放置未初始化、默认或显式初始化为 0 的全局数据。注意链接时 .bss 会向 .data 中合并，所以在 .exe/.dll 中看不到 .bss 区段，可查看 .obj 中的 .bss 区段。用 #pragma bss_seg 放置未初始化数据，必需不初始化其中变量（也不能初始化为 0），否则不会放入 #pragma bss_seg 指定的区段
(3). .rdata 标准区段，放置只读的全局常量数据。const 数字类型会编码到代码中（指令立即数），所以不放到 .rdata 中。用 #pragma const_seg 放置只读常量数据
**例子：自定义区段和数据性质冲突**
以下错误编译器不会报错，但实际没有放置到期望的区段中
- int g_Var1 = 1;     // 放置到 .data 中
- 
- int g_Var2 = 0;     // 放置到 .obj 的 .bss 中，链接时合并到 .data
- int g_Var3;         // 同上
- 
- constint g_Var4 = 1;           // 编码到代码中，没有放置到 .rdata 中
- constchar g_szVar5[]= "foo";   // 放置到 .rdata 中
- 
- #pragma const_seg(".myrdata")
- constchar g_szVar6[]= "foo";   // 放置到 .myrdata 中
- #pragma const_seg()
- 
- #pragma bss_seg(".mybss")
- int g_Var7 = 1;     // 错误：放置到 .data 中
- int g_Var8 = 0;     // 错误：放置到 .obj 的 .bss 中，链接时合并到 .data
- int g_Var9;         // 正确放置到 .mybss 中
- #pragma bss_seg()
- 
- #pragma data_seg(".mydata")
- int g_Var10 = 0;    // 正确放置到 .mydata 中
- int g_Var11;        // 错误：放置到 .obj 的 .bss 中，链接时合并到 .data
- #pragma data_seg()
#### #pragma pack 设置成员字节对齐[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
设置 struct, union, class 成员字节对齐的方法：
(1). 编译选项 /Zp，x86 缺省为 /Zp8，即以 8 byte 对齐，参考 MSDN: [/Zp (Struct Member Alignment)](http://msdn.microsoft.com/en-us/library/xh3e3fd0.aspx)
(2). 使用 __declspec(align(#)) 修饰，参考 MSDN: [align (C++)](http://msdn.microsoft.com/en-us/library/83ythb65.aspx)
(3). 使用 #pragma pack
#pragma pack(): 不带参数的 #pragma pack() 表示恢复到编译选项 /Zp 设置的字节对齐
#pragma pack(show): 产生一条 C4810 编译警告，报告当前的字节对齐：
Test.cpp(140) : warning C4810: value of pragma pack(show) == 8
例子：使用 #pragma pack 设置成员字节对齐
- // x86 32bit
- 
- #pragma pack(4)             // 用一对 #pragma pack(4) | #pragma pack()
- // #pragma pack(push, 4)    // 用一对 #pragma pack(push, 4) | #pragma pack(pop)
- struct TestStruct  
- {  
- double  a;              // sizeof(double) = 8
- int     b;              // sizeof(int) = 4
- };                          // sizeof(TestStruct) = 12
- 
- // #pragma pack(4) 会一直作用，直到改变 pack 
- 
- #pragma pack()              // 恢复编译选项 /Zp 设置的字节对齐
- // #pragma pack(pop)        // 恢复 #pragma pack(push, 4) 之前的字节对齐
#### #pragma inline 函数设置[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
inline 函数修饰，参考 MSDN: [inline, __inline, __forceinline](http://msdn.microsoft.com/en-us/library/z8y1yy88.aspx)
inline 函数编译优化选项，参考 MSDN: [/Ob (Inline Function Expansion)](http://msdn.microsoft.com/en-us/library/47238hez.aspx)
**#pragma auto_inline 禁用和启用 auto-inline[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
例子：
使用 /O2 编译优化选项，含 /Ob2：启动 auto-inline
- #pragma auto_inline(off)
- int simple_func(int a)      // 不会 inline 化
- 
- inline// #pragma auto_inline(off) 不会作用于显式指定的 inline 函数
- int simple_func2(int a)     // 会 inline 化
**#pragma inline_depth 设置函数调用的 inline 化深度[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
#pragma inline_depth(n) 作用于 inline、__inline 和 /Ob2 选项下的 auto-inline 化函数，不作用于 __forceinline 函数。需要 /Ob1 或 /Ob2 编译选项
n: 0 ~ 255，255 表示无限制调用深度 inline 化，0 表示禁止 inline 化，省略参数 #pragma inline_depth() 时 n = 254
递归函数 inline 化的最大调用深度为 16 次调用
**#pragma inline_recursion 禁用和启用递归函数的 inline 化[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
作用于 inline、__inline 和 /Ob2 选项下的 auto-inline 化函数。需要 /Ob1 或 /Ob2 编译选项
默认为 #pragma inline_recursion(off)，这时一个可 inline 化的递归调用函数只 inline 展开一次。如果 #pragma inline_recursion(on)，则 inline 展开深度由 #pragma inline_depth 限制，并不超过 16 次
#### #pragma 优化指令[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
编译优化选项 /O，参考 MSDN: [/O Options (Optimize Code)](http://msdn.microsoft.com/en-us/library/k1ack8f1.aspx)
**#pragma optimize 禁用或启动特定优化[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
#pragma optimize("gp(s|t)y", on|off)
优化参数和编译优化选项之间的对应关系：
g: /Og
p: /fp:precise 浮点数一致性
s: /Os 生成最小代码
t: /Ot 生成最快代码
y: /Oy
- #pragma optimize("pt", on)  // 对下面的代码使用 fp:precise, /Ot 优化
- 
- // 函数定义
- 
- #pragma optimize("", off)   // 关闭上次 #pragma optimize 指定的优化
- #pragma optimize("", on)    // 恢复到编译器 /O 选项指定的优化
**#pragma intrinsic 使用 intrinsic 函数[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
使用 intrinsic 函数编译选项 /Oi，参考 MSDN: [/Oi (Generate Intrinsic Functions)](http://msdn.microsoft.com/en-us/library/f99tchzc.aspx)
#pragma intrinsic，参考 MSDN: [intrinsic](http://msdn.microsoft.com/en-us/library/tzkfha43.aspx)
- #include <string.h>
- 
- // 使用 /Oi 编译选项
- 
- #pragma intrinsic(memcpy)
**#pragma function 使用普通函数[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
和 #pragma intrinsic 对应，改变 /Oi 选项或之前的 #pragma intrinsic 设置，使用指定函数名的普通函数版本
#### #pragma deprecated 声明废弃函数[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
#pragma deprecated 用来声明废弃的函数、类型、宏，编译器产生 C4995 警告
__declspec(deprecated) 修饰也可用来声明废弃的函数、类型，编译器产生 C4996 警告
例子：
- // 使用 #pragma deprecated
- // BEGIN
- //
- #pragma deprecated(OldClass)
- class OldClass1;  
- 
- #pragma deprecated(test_func1)
- void old_func1();  
- //
- // END
- 
- // 使用 __declspec(deprecated)
- // BEGIN
- #define DEPRECATED_WILL_RMOVED  "** will be removed in next version **"
- 
- // deprecated() 中的字符串不是必需的，如果有，会在警告时输出
- __declspec(deprecated(DEPRECATED_WILL_RMOVED)) void old_func2();  
- 
- // 注意 __declspec(deprecated) 修饰 class 时的位置
- class__declspec(deprecated) OldClass2;  
- //
- // END
- 
- void test()  
- {  
-     old_func1();    // 产生 C4995 警告
-     OldClass1 obj;  // 产生 C4995 警告
- 
-     old_func2();    // 产生 C4996 警告，并输出 "** will be removed in next version **"
-     OldClass2();    // 产生 C4996 警告
- }  
#### #pragma omp 使用 OpenMP 指令[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
指令形式：
#pragma omp omp_directive
用于多线程、并发编程的 OpenMP 指令，子指令 omp_directive 参考 MSDN: [OpenMP Directives](http://msdn.microsoft.com/en-us/library/0ca2w8dk.aspx)
#### #pragma region/endregion 折叠代码块[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
标记一整块代码，在 VC 编辑器可折叠成一行 (+) 和展开，见 VC 的 Edit->Outlining 菜单
VC Outlining 常用快捷键：
Ctrl + M, Ctrl + L: 折叠或展开所有的代码块
Ctrl + M, Ctrl + M: 折叠或展开光标所在的代码块
- #pragma region FuncTestCode             // 折叠成一行后，(+) 后显示的名字
- 
- // 这里是一整块代码
- 
- #pragma endregion Test Code of Func()   // 折叠后在名字后显示的注释
#### #pragma setlocale 设置源代码中字符串字面量的编码[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
#pragma setlocale("locale")
#pragma setlocale() 使用的 locale 参数和 CRT 函数 setlocale() 的相同，参考 MSDN: [Language Strings](http://msdn.microsoft.com/en-us/library/39cwe7zf.aspx)，如 简体中文 "chs"
 (GBK)，繁体中文 "cht" (BIG5)，日文 "jpn" (JIS)。注意：GBK 包括简体中文、繁体中文、日文，所以繁体中文的源文件不一定是 BIG5，也可能是 GBK，要看实际的编码
例子：
默认源代码的设置为 #pragma setlocale("")，"" 表示 Windows 用户默认 ANSI 代码页，在控制面板中区域和语言选项中设置，默认简体中文系统为 GBK，繁体中文系统为 BIG5 等。所以在简体系统下编写简体字面量代码，或在繁体系统下编写繁体字面量代码等，无需设置源文件的 #pragma setlocale
在简体中文 Windows 下源文件使用 BIG5 编码源文件，代码中有 L"xxx" 的宽字符字面量，且 "xxx" 在 BIG5 - ASCII 的字符集范围，则应当使用 #pragma setlocale("cht")
#### #pragma include_alias 定义头文件别名[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
- #pragma include_alias(<stdio.h>, <newstdio.h>)
- #pragma include_alias("api.h", "test\api.h")
- 
- #include <stdio.h>
- #include "api.h"
### 预处理相关编译选项[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
#### /D 定义宏[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
/D: 定义宏，作用类似 #define，但会去掉选项中的引号
/U: 取消指定的预定义宏，类似 #undef，如 /U _DEBUG
/u: 取消所有的预定义宏。/U 和 /u 都不能取消在源码中用 #define 定义的宏
**定义数字[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
- /DTESTMACRO: 等价 #define TESTMACRO 1，整数
- /DTESTMACRO=1: 同上
- /DTESTMACRO="1": 同上
- /DTESTMACRO=3.14: 等价 #define TESTMACRO 3.14，浮点数
- /DTESTMACRO="3.14": 同上
**定义字符串[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
- /DTESTMACRO="abcdef": 等价 #define TESTMACRO abcdef，非字符串字面量（没有引号）
- /DTESTMACRO=\"abcdef\": 等价 #define TESTMACRO "abcdef"，字符串字面量
- /DTESTMACRO="\"abcdef\"": 同上
**空定义[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
- /DTESTMACRO=: 等价 #define TESTMACRO
- /DTESTMACRO="": 同上
- /DTESTMACRO=\"\": 等价 #define TESTMACRO ""，非空定义，而是空字符串
**CL 环境变量使用 /D[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
SET CL=/DTESTMACRO#1: 用 # 代替 =，等价 /DTESTMACRO=1，即 #define TESTMACRO 1
#### /E, /EP, /P 预处理选项[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
/E: 预处理源文件，结果输出到标准输出，去掉注释，在 #inlcude 展开和条件编译周围产生 #line 行号指示
/EP: 和 /E 相似，结果输出到标准输出，但不产生 #line
/P: 和 /E 相似，产生 #line，结果输出到文件 (test.cpp => test.i)，相当于 cl /E test.cpp > test.i
/P /EP 联用: 结果输出到文件 test.i，且不产生 #line。/E、/EP、/P 不能和预编译头 PCH 联用
/C: 预处理时保留注释，和 /E、/P、/EP 联用
**例子：预处理展开源文件[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
源文件 test.cpp：
- #include <stdio.h>
- 
- int main()  
- {  
- #ifdef _DEBUG
-     printf("Debug config\n");  
- #else
-     printf("Release config\n");  
- #endif
- 
- // MARK: TESTMACRO value
-     printf("TESTMACRO: %d\n", TESTMACRO);  
- return 0;  
- }  
预处理编译命令：
cl /P /C /DTESTMACRO test.cpp
预处理输出到 test.i：
- #line 1 "test.cpp"
- #line 1 "d:\\Visual Studio 8\\VC\\INCLUDE\\stdio.h"
- 
- // #line 中 stdio.h 的路径由实际 VC 安装路径而定
- // 这里省略 stdio.h 展开后的大量代码
- 
- #line 706 "d:\\Visual Studio 8\\VC\\INCLUDE\\stdio.h"
- 
- #line 2 "test.cpp"
- 
- int main()  
- {  
- 
- 
- 
-     printf("Release config\n");  
- #line 10 "test.cpp"
- 
- // MARK: TESTMACRO value
-     printf("TESTMACRO: %d\n", 1);  
- return 0;  
- }  
**例子：过滤查看预处理展开结果[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
用这种方法可以查看编译过程中，实际的宏展开、预处理结果
以上面的 test.cpp 为例，预处理编译命令和 grep 过滤：
cl /EP /C /DTESTMACRO test.cpp 2>NUL | egrep -A 5 -B 5 "MARK: TESTMACRO"
2>NUL: 用于屏蔽输出 VC 编译器 banner 和提示、错误信息，用 /nologo 选项也可以
egrep -A 5 -B 5: 表示输出匹配正则表达式前后 5 行
输出结果如下：
-     printf("Release config\n");  
- 
- // MARK: TESTMACRO value  
-     printf("TESTMACRO: %d\n", 1);  
-     return 0;  
- }  
#### /showIncludes 输出头文件列表[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)
输出源文件的 #include 的头文件列表到 stderr，包括嵌套 #include
**例子：查看 #include 头文件列表[^](http://blog.csdn.net/breakerzy/article/details/6908820#cpp-macro-preprocess-menu)**
以上面 test.cpp 为例，编译命令：
cl /nologo /showIncludes /EP test.cpp >NUL
输出头文件列表，由实际 VC 安装路径而定。嵌套 #include 用空格缩进表示，如 stdio.h include=> crtdefs.h：
- test.cpp  
- Note: including file: d:\Visual Studio 8\VC\INCLUDE\stdio.h  
- Note: including file:  d:\Visual Studio 8\VC\INCLUDE\crtdefs.h  
- Note: including file:   d:\Visual Studio 8\VC\INCLUDE\sal.h  
- Note: including file:   d:\Visual Studio 8\VC\INCLUDE\vadefs.h  
- Note: including file:  d:\Visual Studio 8\VC\INCLUDE\swprintf.inl  
