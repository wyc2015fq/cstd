# Lua的调试 - 逍遥剑客 - CSDN博客
2007年11月12日 04:56:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：1622
改BUG改到脚本里,郁闷了,没法调试
google一下,找到了云风大牛上的一个Lua调试器
看了一下,估计转内网我也不会用
通常没法单步调试的程序我都是LOG/TRACE
本想用Lua的print来着,可是游戏的控制台里愣是没有,汗~
最后想了想,注册了一个C++函数:
void LuaTrace( constchar* text )
{
    Assert( NULL != text && strlen(text) >0 );
    TRACE( "%s ", text );
} 
汗一个先
不过,最后终于通过这个方法找出问题所在了,呵呵
