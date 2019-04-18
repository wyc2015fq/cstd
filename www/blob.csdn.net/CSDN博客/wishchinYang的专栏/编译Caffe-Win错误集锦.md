# 编译Caffe-Win错误集锦 - wishchinYang的专栏 - CSDN博客
2015年04月28日 18:45:39[wishchin](https://me.csdn.net/wishchin)阅读数：10562

# Caffe在Windows下编译还是遇到不少麻烦的...
# 1.[visual studio 2013 error C2371: 'int8_t' : redefinition;](http://stackoverflow.com/questions/16415058/visual-studio-2008-error-c2371-int8-t-redefinition-different-basic-types)
      引入的unistd.h文件里面重定义了int8_t  ，用记事本 打开文件注销之。
2. error C3861: 'getpid': identifier not found    C:\Tools\caffe-master\src\caffe\common.cpp    26
     解决：在common.cpp 里面添加 #include <process.h>
     修改：
```cpp
pid = getpid(); ——>pid = _getpid();
```
3.  error C3861: 'usleep': identifier not found    C:\Tools\caffe-master\src\caffe\test\test_benchmark.cpp    65
参考：[ffmpeg编译总结](http://my.oschina.net/u/589721/blog/80192)
      7.出现 'usleep': identifier not found
     这里因为VC中没有usleep，usleep是微妙级别的，所以需要把代码改为
```cpp
usleep(is->audio_st && is->show_audio ? rdftspeed*1000 : 5000);
     —>    Sleep (is->audio_st && is->show_audio ? rdftspeed*1 : 5);
     usleep(300 * 1000); —> Sleep(300);
```
      添加：#include <windows.h>
4. error C3861: 'snprintf': identifier not found    C:\Tools\caffe-master\src\caffe\solver.cpp    331
参考：[http://blog.163.com/wanghuajie@126/blog/static/452312862009111114434838/](http://blog.163.com/wanghuajie@126/blog/static/452312862009111114434838/)
     在solver.cpp里面添加      #include <stdio.h>
   snprintf  修改为 _snprintf
5. error C3861: '__builtin_popcount': identifier not found    C:\Tools\caffe-master\src\caffe\util\math_functions.cpp    346
参考：[http://blog.csdn.net/rappy/article/details/1788969](http://blog.csdn.net/rappy/article/details/1788969)
      __builtin_popcount 这是一个GCC的函数：计算一个 32 位无符号整数有多少个位为1 
     解决：自己写一个函数__builtin_popcount
```cpp
template <typename Dtype>
unsigned int __builtin_popcount(Dtype u)
{
    u = (u & 0x55555555) + ((u >> 1) & 0x55555555);
    u = (u & 0x33333333) + ((u >> 2) & 0x33333333);
    u = (u & 0x0F0F0F0F) + ((u >> 4) & 0x0F0F0F0F);
    u = (u & 0x00FF00FF) + ((u >> 8) & 0x00FF00FF);
    u = (u & 0x0000FFFF) + ((u >> 16) & 0x0000FFFF);
    return u;
}//wishchin!!!
```
6.error : identifier "::caffe::kBNLL_THRESHOLD" is undefined in device code    C:\Tools\caffe-master\src\caffe\layers\bnll_layer.cu    36
参考：
        解决：在bnll_layer.cu    里修改
```cpp
Dtype expval = exp(min(in_data[index], Dtype(kBNLL_THRESHOLD)));
        ——>Dtype expval = exp(min(in_data[index], Dtype(50)));
```
7. error C2660: 'mkdir' : function does not take 2 arguments    C:\Tools\caffe-master\src\caffe\test\test_data_layer.cpp    71
参考：
     解决：
```cpp
CHECK_EQ(mkdir(filename_->c_str(), 0744), 0) << "mkdir " << filename_<< "failed";
```
     里面的第二个参数去掉。
8.error C2784: '_Ty std::max(std::initializer_list<_Elem>,_Pr)' : could not de
     解决：调用函数处 把std::max 用括号 括起来  (std::max)(std::initializer_list<
9.error C4996: 'std::_Copy_impl': Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. To disable this warning, use -D_SCL_SECURE_NO_WARNINGS.
    参考：http://www.zhihu.com/question/26242158
   解决：应该这样添加 -D去掉 属性-> c\c++ -> 预处理器 -> 预处理器定义 里添加 _SCL_SECURE_NO_WARNINGS 编译成功 ，
XXX：不断出现的
        error C1075: end of file found before the left parenthesis '(' at '  test_infogain_loss_layer.cpp    71
       也没有找到哪里错了。应该是Define语句出现问题， 貌似可以不用管它.............
