# C#自定义结构的强制转换 - 左直拳的马桶_日用桶 - CSDN博客
2010年04月21日 16:28:00[左直拳](https://me.csdn.net/leftfist)阅读数：3055
C#自定义结构的强制转换
我在ASP.NET页面上定义了一个自定义结构，将该结构类型的对象存放于会话（SESSION），结果到读取SESSION的时候，强制转换回自定义结构类型失败，显示的错误也莫名其妙：
当一个数字执行强制转换时 值必须是一个小于无限大的数字
页面后台代码：
struct StruMyInfo
    {
        public int UserId;
        public string Name;
  }
StruMyInfo MyInfo
    {
        get
        {
            StruMyInfo myinfo;
            string sessionname = "MyInfo";
            if (Session[sessionname] != null)
            {
myinfo = (StruMyInfo) Session[sessionname];//失败
……
            }
            else
{
 ……
Session[sessionname] = myinfo
}
    }
}
到网上搜索，查不出一个屁来。网络的作用其实有时候相当有限。
后来想了一下，大概是StruMyInfo为自定义的结构，而非通用，并且定义在一个页面上。由于网页的生存期非可持续，提交一次执行一次，造成自定义结构不可辨识。假如是WINFORM程序应该就没问题。
于是将StruMyInfo放到独立的基类里定义，就是一个独立的DLL或者位于其他地方的一个公用类，问题解决
。
