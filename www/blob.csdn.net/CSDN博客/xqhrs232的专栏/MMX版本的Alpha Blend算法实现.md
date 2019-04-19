# MMX版本的Alpha Blend算法实现 - xqhrs232的专栏 - CSDN博客
2012年10月30日 22:48:58[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：752
个人分类：[DirectUI界面技术](https://blog.csdn.net/xqhrs232/article/category/1243806)
原文地址::[http://dev.gameres.com/Program/Visual/2D/mmxaddalpha.htm](http://dev.gameres.com/Program/Visual/2D/mmxaddalpha.htm)
相关网帖
MMX指令优化的32bit AlphaBlend----[http://bbs.sjtu.edu.cn/bbscon?board=GameDesign&file=M.1168164234.A](http://bbs.sjtu.edu.cn/bbscon?board=GameDesign&file=M.1168164234.A)
这次我们的目标是: 超越普通的CPU玩家,用CPU的母语来优化程序!
实验器材:VC++ 4.2以上的编译器或C++ Builder 4.0以上编译器
MMX技术到现在来说可以算是基本大众化了,目前大多数个人电脑都应该能支持它。P55C,K6,PII,PIII...按照惯例，Intel公司将在以后的x86版本永远支持它。
要了解MMX指令体系,请一定浏览以下URL,获得系统的MMX体系知识:
[http://www.game1st.com/database/mmxd/mmxd.htm](http://www.game1st.com/database/mmxd/mmxd.htm)
否则将会对我的讲解摸不着北,(象听最优控制或者线性控制理论一样)
提示：最好自己多写些Console App来实验和熟悉MMX指令特性,cout出结果，不要一来就搞Windows App.
MMX技术的优点是什么?
一言蔽之，就是一个指令操作多个整型数据.并且可以自动完成饱和运算。
MMX最适合的应用领域是数字图象处理,然后是语音，通讯等领域。
写图象处理算法时很影响效率的以下几点是:
1.不厌其烦的低效邻域操作: 不厌其烦的写GfxBuffer[i],GfxBuffer[i+1],GfxBuffer[i+2],...取RGB值等。
2.恼人的环绕:
像素运算溢出,必须拿个大点的数据类型来保存结果再判断溢出,否则结果就是错误的
short int GfxOut=GfxBuf1[i]+GfxBuf2[i]; ...
if(GfxOut>255)GfxOut=255; if(GfxOut<0)GfxOut=0;
问题是你知道那两个判断回会生成多少汇编代码吗?
3.不必要的高精度: 该死的RGB->YUV运算,我们最熟悉的彩色转灰度运算: Y=(BYTE)(Red*0.299+Green*0.587+Blue*0.114);
其实我们关心的是一个字节结果，谁愿意让协处理器做大量运算，得到float结果，还要再转换为byte结果呢?要知道IEEE浮点转整数可不象在C程序里看起来那么简单!(在你为51单片机这种低级的劣质的没有协处理器的CPU写过程序就知道了...)代价是相当大的。
尤其图面比较大的情况下，你会在屏幕面前等得生霉,我们不要那么高的精度啊。
而MMX技术就是黑暗中的灯塔，被老板咒骂的程序员的救命稻草，使图象处理模式识别和神经网络专业方向的研究生不被导师训斥的的葵花宝典...(本人不系Intel中国公司的营销部经理啊..)
以32-bit为例,下面具体讲下用简单的例子讲解MMX技术优化北航老王头提供的Alpha Blend算法:
绝对为本人在某个深夜所原创,不抄袭自任何资料
char *pSrc,*pOvr; { pSrc=pScanline1+j; pOvr=pScanLine2+j; j+=4; .....
假定我们的源像素在pSrc,覆盖图Overlay在pOvr,各指向一个32 bit像素。
( 为什么要以32bit像素?因为老赫有两年做字幕卡程序的背景,电视字幕是32 bit图文层嘛! )
如果你阅读了开始我提供的URL文档,你就清楚,MMX有八个64bitRegister,叫MM0->MM7,是浮点处理器的别名，也是他们的低64位映象。
__asm{ pxor mm2,mm2 //把MM2清0
mov edx,dword ptr [pSrc] //取32bit像素地址到edx
movd mm0,dword ptr [edx] //把Source像素取到mm0低32bit
mov edx,dword ptr [pOvr]
movd mm1,dword ptr [edx] //把32 bit Overlay像素取到mm1低32bit
punpcklbw mm0,mm2 //Source:8 bit到16 bit以容纳结果,32bit expand to 64 bit
punpcklbw mm1,mm2 //Overlay:8 bit到16 bit以容纳结果.32bit expand to 64 bit
movq mm3,mm1 // 因为要用Overlay的Alpha值
punpckhwd mm3,mm3 // 高字移动到双字
punpckhdq mm3,mm3 // 双字移动到四字,现在有八个像素的Alpha了!
movq mm4,mm0 //Source->mm4
movq mm5,mm1 //Overlay->mm5
psubusw mm4,mm1 //Source-Overlay,饱和减,小于0为0
psubusw mm5,mm0 //Overlay-Source,饱和减,小于0为0
pmullw mm4,mm3 //Alpha * (Source-Overlay)
pmullw mm5,mm3 //Alpha * (Overlay-Source)
psrlw mm4,8 //除以256,now mm4 get the result,(Source-Overlay)<0 部分
psrlw mm5,8 //除以256,now mm5 get the result,(Overlay-Source)>0 部分
paddusw mm0,mm5 //饱和加到原图象:D=Alpha*(O-S)+S,(Source-Overlay)<0 部分
psubusw mm0,mm4 //饱和加到原图象D=S-Alpha*(S-O),(Overlay-Source)>0 部分
packuswb mm0,mm0 //紧缩到低32bit
mov edx,dword ptr [pSrc]
movd dword ptr [edx],mm0 //保存结果
emms ///必要的!Empty MMX Status
} 以上算法和常规算法比较:
Legend PII 400 With damnble,extremely Slow Sis6326 Video Card: 640*480*32bit bitmap:
Normal algorithm:170-180 ms
This algorithm:Below 40 ms
均包含拷贝内存时间,纯处理时间只要20ms, 就是说如果用DirectX,Blt,BltFast通常比memcpy快很多.就小于40ms了
总结: 反复使用紧缩移位,紧缩扩展,紧缩压缩指令,并施加饱和运算等,是用MMX写算法的核心。
程序员必须对数制,内存结构,定点数很熟悉才能写出高效率的MMX加速的子程序。

