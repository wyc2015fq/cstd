# C#编程之格式化日期时间 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年11月30日 18:34:42[boonya](https://me.csdn.net/boonya)阅读数：1277标签：[c#																[date																[格式化](https://so.csdn.net/so/search/s.do?q=格式化&t=blog)
个人分类：[C#](https://blog.csdn.net/boonya/article/category/1776321)





1、DateTime.Now.ToString("yyyy年MM月dd日 dddd")

输出结果格式为：2013年11月30日 星期六



2、DateTime.Now.ToString("dddd yyyy年MM月dd日")

输出结果格式为：星期六 2013年11月30日



3、DateTime.Now.ToString("yyyy年MM月dd日")

输出结果格式为：2013年11月30日




4、 DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss")

输出结果格式为：2013-11-30 18:24:03


原理：yyyy标识年，MM表示月，dd标识日，HH小时，mm分钟，ss秒，dddd表示星期。](https://so.csdn.net/so/search/s.do?q=date&t=blog)](https://so.csdn.net/so/search/s.do?q=c#&t=blog)




