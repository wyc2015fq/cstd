# 图灵停机问题（The Halting Problem） - 数据之美的博客 - CSDN博客
2017年05月22日 11:30:27[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：252
不存在这样一个程序（算法），它能够计算任何程序（算法）在给定输入上是否会结束（停机）。
那么，如何来证明这个停机问题呢？
反证!假设我们某一天真做出了这么一个极度聪明的万能算法（就叫God_algo吧），你只要给它一段程序（二进制描述），再给它这段程序的输入，它就能告诉你这段程序在这个输入上会不会结束（停机），我们来编写一下我们的这个算法吧：
bool God_algo(char* program, char* input)
{
    if(<program> halts on <input>)
        return true;
    return false;
}
这里我们假设if的判断语句里面是你天才思考的结晶，它能够像上帝一样洞察一切程序的宿命。现在，我们从这个God_algo出发导出一个新的算法：
bool Satan_algo(char* program)
{
if( God_algo(program, program) )
{
       while(1);        // loop forever!
       return false;   // can never get here!
}
else
       return true;
}
正如它的名字所暗示的那样，这个算法便是一切邪恶的根源了。当我们把这个算法运用到它自身身上时，会发生什么呢？
Satan_algo(Satan_algo);
我们来分析一下这行简单的调用：
显然，Satan_algo(Satan_algo)这个调用要么能够运行结束返回（停机），要么不能返回（loop forever）。
如果它能够结束，那么Santa_algo算法里面的那个if判断就会成立（因为God_algo(Santa_algo,Santa_algo)将会返回true），从而程序便进入那个包含一个无穷循环while(1);的if分支，于是这个Satan_algo(Satan_algo)调用便永远不会返回（结束）了。
如果不能结束（停机），则if判断就会失败，从而选择另一个if分支并返回true，即Satan_algo(Satan_algo)又能够返回（停机）。
总之，我们有：
Satan_algo(Satan_algo)能够停机=> 它不能停机
Satan_algo(Satan_algo)不能停机=> 它能够停机
所以它停也不是，不停也不是，左右矛盾。
