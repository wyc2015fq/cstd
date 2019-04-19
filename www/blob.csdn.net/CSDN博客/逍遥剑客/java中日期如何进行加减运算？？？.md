# java中日期如何进行加减运算？？？ - 逍遥剑客 - CSDN博客
2006年12月01日 00:15:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2038标签：[java																[date](https://so.csdn.net/so/search/s.do?q=date&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[基础知识](https://blog.csdn.net/xoyojank/article/category/259553)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        Date date =new Date();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        Calendar cal = Calendar.getInstance();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        cal.setTime(date);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        cal.add(Calendar.DATE, 30);//加30天
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        System.out.println(cal.getTime());// 结束日期
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
