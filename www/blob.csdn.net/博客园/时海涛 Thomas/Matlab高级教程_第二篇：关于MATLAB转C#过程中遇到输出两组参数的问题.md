# Matlab高级教程_第二篇：关于MATLAB转C#过程中遇到输出两组参数的问题 - 时海涛|Thomas - 博客园




- [博客园](https://www.cnblogs.com/)
- [首页](https://www.cnblogs.com/noah0532/)
- [新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
- [联系](https://msg.cnblogs.com/send/%E6%97%B6%E6%B5%B7%E6%B6%9B%7CThomas)
- [管理](https://i.cnblogs.com/)
- [订阅](https://www.cnblogs.com/noah0532/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# [Matlab高级教程_第二篇：关于MATLAB转C#过程中遇到输出两组参数的问题](https://www.cnblogs.com/noah0532/p/9893984.html)





1. 在matlab的m函数很可能遇到原函数[a,b] = func(a);

这样的两个输出参数。

2. 在观察C#生成后定义中我们发现：

 public MWArray HP();
        public MWArray HP(MWArray Y);
        public MWArray HP(MWArray Y, MWArray smoothing);
        public MWArray[] HP(**int numArgsOut**);
        public MWArray[] HP(**int numArgsOut**, MWArray Y);
        public MWArray[] HP(**int numArgsOut**, MWArray Y, MWArray smoothing);
        public void HP(int numArgsOut, ref MWArray[] argsOut, MWArray[] argsIn);
        public void WaitForFiguresToDie();
        protected virtual void Dispose(bool disposing);

3. 这个地方表示有几个输出参数，如果numArgsOut为2的话为2个输出参数。因此在C#调用中可以这样调用。

 MWArray array = (MWNumericArray)new double[] {50140, 50140, 50110, 50130, 50170, 50200, 50160, 50130, 50130, 50120};
            MWArray smoothing = 1600;
            HPFilter.HPFilter demo = new HPFilter.HPFilter();
            //其中对应的2位置为几个输出参数
**MWArray[] result** = demo.HP(2,array, smoothing);
            MWNumericArray Trend = (MWNumericArray)**result[0]**;
            MWNumericArray Cycle = (MWNumericArray)**result[1]**;
            Console.WriteLine("趋势分析的结果如下:");
            Console.WriteLine(Trend);
            Console.WriteLine("周期分析的结果如下:");
            Console.WriteLine(Cycle);

根据上一篇的知识：MWNumericArray为MWArray的子类。因此我们输出的是MWArray的父类，因此我们要用子类的函数把这两个子类调用出来

**result[0]：为第一个序列**

**result[1]：为第二个序列**














