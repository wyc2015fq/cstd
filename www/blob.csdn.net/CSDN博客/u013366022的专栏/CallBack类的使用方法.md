# CallBack类的使用方法 - u013366022的专栏 - CSDN博客
2015年07月01日 09:15:00[slitaz](https://me.csdn.net/u013366022)阅读数：490

假设有两个类 分别为解码类A 、编码类B。
A类的decode方法 需要将解码后的数据 传送给类B相对应的方法encodeAudio和encodeVideo。
考虑类的封装性 可移植性 
比较好的做法 如下：
在Decode.h中
#include ….
//定义一个callback类
class CallBack{
publibc:
virtual 返回值 方法1 (参数1、参数2......) = 0；
virtual 返回值 方法2 (参数1、参数2......) = 0；
}
class A
{
public:
decode(CallBack *pCB)
{
……
pCB->方法1（参数1、参数2…...）；
…..
pCB->方法2（参数1、参数2…...）；

}
//其他方法
…….
private:
CallBack *pCB;
 …….
}
class A
{
…….
//A类里的内容
}
新建个头文件，创建接口类C
#include “B.h"
class C :public CallBack
{
public:
返回值 方法1 (参数1、参数2......) 
{
pb-> encodeAudio（参数1,参数2 …...）；
}
  返回值 方法2 (参数1、参数2) 
{
pb-> encodeVideo（参数1,参数2 ……）;
}
private:
B *pb;
}
完毕！
