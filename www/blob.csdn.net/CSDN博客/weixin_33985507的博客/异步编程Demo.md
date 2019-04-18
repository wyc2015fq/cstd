# 异步编程Demo - weixin_33985507的博客 - CSDN博客
2010年12月20日 23:27:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
# 1.问题
static int TakesAWhile(int data, int ms)
{
    Console.WriteLine("TakesAWhile started");
    Thread.Sleep(ms);
    Console.WriteLine("TakesAWhile completed");
    return ++data;
}
public delegate int TakesAWhileDelegate(int data, int ms);
委托可以直接异步进行(BeginInvoke),而不用再开线程(Thread),实质是一样的
# 直接执行委托
TakesAWhile(1, 3000);
# 第一次异步执行
IAsyncResult ar = d1.BeginInvoke(1, 3000, null, null);
while (!ar.IsCompleted)
{
    // doing something else
    Console.Write(".");
    Thread.Sleep(50);
}
Console.WriteLine(d1.EndInvoke(ar));
IsCompleted属性用于判断异步是否执行完毕.
这里用一个while语句语句判断委托是否执行完毕.
  
虽然委托执行的方法是异步的,但问题是无法知道什么时候执行完毕,却当委托有返回值时,还需要执行EndInvoke来获取返回值,还不是彻底的异步.
  
只有在BeginInvoke执行完毕后才能获取到EndInvoke的返回值,否则就失去了异步的效果,所以需要判断IsCompleted以阻止执行EndInvoke
# 使用WaitHandle
// wait handle
 IAsyncResult ar = d1.BeginInvoke(1, 3000, null, null);
 while (true)
 {
     Console.Write(".");
     if (ar.AsyncWaitHandle.WaitOne())
     {
         Console.WriteLine("Can get the result now");
         break;
     }
 }
 int result = d1.EndInvoke(ar);
 Console.WriteLine("result: {0}", result);
使用后的效果同上,无法真正意义上的解决问题.如果内部提供一个callback的方法就好了。
# 使用异步回调
这是真正解决问题的方法
d1.BeginInvoke(1, 3000, TakesAWhileCompleted, d1);static void TakesAWhileCompleted(IAsyncResult ar)
{
    if (ar == null) throw new ArgumentNullException("ar");
    TakesAWhileDelegate d1 = ar.AsyncState as TakesAWhileDelegate;
    Trace.Assert(d1 != null, "Invalid object type");
    int result = d1.EndInvoke(ar);
    Console.WriteLine("result: {0}", result);
}
第3个参数为异步委托,第4个执行的对象.
更方便的执行匿名委托
d1.BeginInvoke(1, 3000,
   ar =>
   {
       int result = d1.EndInvoke(ar);
       Console.WriteLine("result: {0}", result);
   },
   null);
只方便记忆
