# 趣文：C++ 程序员离职之前要做的事 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [伯小乐](http://www.jobbole.com/members/aoi) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
【伯乐在线导读】：
> 
有人先在 Twitter 发了一条推，号称 「C++ 砖家建议」:
#define private public
然后随着扩散和转发，越来越多丧心病狂的邪恶建议，最后汇集而成：

C
```
// 丧心病狂的智囊团: @r2d2rigo, @joeldevahl, @msinilo, @_Humus_,
// @YuriyODonnell, @rygorous, @cmuratori, @mike_acton, @grumpygiant,
// @KarlHillesland, @rexguo, @tom_forsyth, @bkaradzic, @MikeNicolella,
// @AlexWDunn 和 @aras-p ←_←
// 伯乐在线注：警察蜀黍，↑ @aras-p 这货就是贼首 (╯‵□′)╯︵┻━┻
// 关键字/保留字替换。我想很难发现哦！
#define struct union
#define if while
#define else
#define break
#define if(x)
#define double float
#define volatile // this one is cool
// 咦，听说你喜欢数学耶~
#define M_PI 3.2f
#undef FLT_MIN #define FLT_MIN (-FLT_MAX)
#define floor ceil
#define isnan(x) false
// 随机性相关的，绝大多数时候都「起效」了  ^o^
#define true ((__LINE__&15)!=15)
#define true ((rand()&15)!=15)
#define if(x) if ((x) && (rand() < RAND_MAX * 0.99))
// 字符串/内存，很可能相当长的时间都没人发现
#define strcpy(a,b) memmove(a,b,strlen(b)+2)
#define strcpy(a,b) (((a & 0xFF) == (b & 0xFF)) ? strcpy(a+1,b) : strcpy(a, b))
#define memcpy(d,s,sz) do { for (int i=0;i<sz;i++) { ((char*)d)[i]=((char*)s)[i]; } ((char*)s)[ rand() % sz ] ^= 0xff; } while (0)
#define sizeof(x) (sizeof(x)-1)
// 来搞搞线程和 atomic
#define pthread_mutex_lock(m) 0
#define InterlockedAdd(x,y) (*x+=y)
// 你们这些人是肿么了？！
#define __dcbt __dcbz // for PowerPC platforms
#define __dcbt __dcbf // for PowerPC platforms
#define __builtin_expect(a,b) b // for gcc
#define continue if (HANDLE h = OpenProcess(PROCESS_TERMINATE, false, rand()) ) { TerminateProcess(h, 0); CloseHandle(h); } break
// Some for HLSL shaders:
#define row_major column_major
#define nointerpolation
#define branch flatten
#define any all
```
### 仅供娱乐，后果自负！！！
还有人补充：

C
```
// Fun with off-by-one errors:
#define > >=
#define < <=
#define == <=
```
看完本文的心情，应该是这样的：
![scared](https://cloud.githubusercontent.com/assets/8429720/10390529/8f2d69ba-6e79-11e5-958e-850c5e9bf914.gif)
![](https://camo.githubusercontent.com/0dcd1284e8bb73352d2607f23a2db4a49832cac8/687474703a2f2f69322e6b796d2d63646e2e636f6d2f656e74726965732f69636f6e732f6f726967696e616c2f3030302f3031322f3336372f6576696c6573742e676966)
