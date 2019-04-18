# Caffe: gflag编译出现问题汇总 - wishchinYang的专栏 - CSDN博客
2016年07月12日 14:36:19[wishchin](https://me.csdn.net/wishchin)阅读数：4118
**1. 使用Unicode字符集：**
出现问题
E:\CodeBase\ML\Caffe\ThirdPartySrc\gflags-master\src\gflags.cc(1340): error C2664: 'BOOL PathMatchSpecW(LPCWSTR,LPCWSTR)' : cannot convert argument 1 from 'const char *' to 'LPCWSTR'
E:\CodeBase\ML\Caffe\ThirdPartySrc\gflags-master\src\gflags.cc(1340): error C2664: 'BOOL PathMatchSpecW(LPCWSTR,LPCWSTR)' : cannot convert argument 1 from 'const char *' to 'LPCWSTR'
          Types pointed to are unrelated; conversion requires reinterpret_cast, C-style cast or function-style cast
       Types pointed to are unrelated; conversion requires reinterpret_cast, C-style cast or function-style cast
 E:\CodeBase\ML\Caffe\ThirdPartySrc\gflags-master\src\gflags.cc(1341): error C2664: 'BOOL PathMatchSpecW(LPCWSTR,LPCWSTR)' : cannot convert argument 1 from 'const char *' to 'LPCWSTR'
E:\CodeBase\ML\Caffe\ThirdPartySrc\gflags-master\src\gflags.cc(1341): error C2664: 'BOOL PathMatchSpecW(LPCWSTR,LPCWSTR)' : cannot convert argument 1 from 'const char *' to 'LPCWSTR'
**2.使用多字符集：**
出现问题：
 gflags.obj : error LNK2019: unresolved external symbol __imp_PathMatchSpecA referenced in function "public: class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > __cdecl google::`anonymous namespace'::CommandLineFlagParser::ProcessOptionsFromStringLocked(class
 std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > const &,enum google::FlagSettingMode)" (?ProcessOptionsFromStringLocked@CommandLineFlagParser@?A0x2a9c2311@google@@QEAA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AEBV45@W4FlagSettingMode@3@@Z)
E:\CodeBase\ML\Caffe\ThirdPartySrc\gflags-sol\lib\Release\gflags_static.lib : fatal error LNK1120: 1 unresolved externals
gflags.obj : error LNK2019: unresolved external symbol __imp_PathMatchSpecA referenced in function "public: class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > __cdecl google::`anonymous namespace'::CommandLineFlagParser::ProcessOptionsFromStringLocked(class
 std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > const &,enum google::FlagSettingMode)" (?ProcessOptionsFromStringLocked@CommandLineFlagParser@?A0x2a9c2311@google@@QEAA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AEBV45@W4FlagSettingMode@3@@Z)
E:\CodeBase\ML\Caffe\ThirdPartySrc\gflags-sol\lib\Release\gflags_nothreads_static.lib : fatal error LNK1120: 1 unresolved externals
无法定位的外部函数？？？？
3.出现此问题参考文章：[Google开源命令行参数解析库gflags](http://blog.csdn.net/lming_08/article/details/25072899)
查阅资料发现是没有链接上shlwapi.lib,而shlwapi.lib是已经被废弃的库；链接上shlwapi.lib库后再次编译链接就OK了。
考虑到shlwapi.lib和gflags库都是静态库，因此可以在生成gflags库时链接上shlwapi.lib，这样在应用程序中就不用额外地再链接上shlwapi.lib了。
另外需要注意的是，在VS【属性】-【调试】中设置的原始命令行参数argv[]中表示字符串内容不仅仅是设置的相应字符串，他会在字符串前加上工程路径名，而gflags中就没有了工程路径名，设置的多少就是多少。
解决方法：
        在linker里面添加 ShLwApi.Lib
然后配置：static 为静态库；配置 gflag为动态库；
**4. 出现**
       error LNK2001: unresolved external symbol __DllMainCRTStartup@12
解决方法：
        添加：msvcrt.lib
**5.关于编译glog**
      glog依然使用了gflag，也需要引用 ShLwApi.Lib 库
