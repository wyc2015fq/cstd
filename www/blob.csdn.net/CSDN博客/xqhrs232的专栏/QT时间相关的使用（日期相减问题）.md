# QT时间相关的使用（日期相减问题） - xqhrs232的专栏 - CSDN博客
2018年08月23日 21:14:06[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：187
原文地址::[https://blog.csdn.net/qq_18286031/article/details/78538769](https://blog.csdn.net/qq_18286031/article/details/78538769)
# Qt两个日期相减的问题
首先，由日期与时间戳的相互转换
- 
`QDateTime time = QDateTime::currentDateTime(); //获取当前时间 `
- 
`int timeT = time.toTime_t(); //将当前时间转为时间戳 `
将一定格式的日期转换为时间戳
- 
`QString starttime ="2017-11-11 12:00:00";`
- 
`QString endtime = "2017-11-13 12:00:01";`
- 
`QDateTime start = QDateTime::fromString(starttime, "yyyy-MM-dd hh:mm:ss");`
- 
`QDateTime end = QDateTime::fromString(endtime, "yyyy-MM-dd hh:mm:ss");`
- 
`uint stime = start.toTime_t();`
- 
`uint etime = end.toTime_t();`
与toTime_t()功能相反的是fromTime_t()，把时间戳转为QDateTime类型。因为这个方法是静态的，所以使用方法为
`QDateTime time = QDateTime::fromTime_t(timeT);  `
两个日期相隔的天数由时间戳来相减得到
- 
`int ndaysec = 24*60*60;`
- 
`qDebug() << "Day = " << (etime - stime)/(ndaysec) + ((etime - stime)%(ndaysec)+(ndaysec-1))/(ndaysec) - 1;`
注意：上面的计算中，天数相隔时间差24小时内为0天，可以这样理解，26小时大于24小时（一天），那么他们相差1天了，如果24小时内为1天计算，26小时大于24小时，那么就将后面的减一去掉就可以了。
Qt获取前几天的时间
- 
`QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间`
- 
`QString str = time.toString("yyyy-MM-dd"); //设置显示格式`
- 
`QString BeforeDaystr=time.addDays(-1).toString("yyyy-mm-dd");//获取前一天时间`
具体上代码：
- 
`#include <QCoreApplication>`
- 
`#include <QString>`
- 
`#include <QDateTime>`
- 
`#include <QTime>`
- 
`#include <QDebug>`
- 
- 
`int main(int argc, char *argv[])`
- 
`{`
- 
`QCoreApplication a(argc, argv);`
- 
`QString starttime ="2017-11-11 12:00:00";`
- 
`QString endtime = "2017-11-13 12:00:01";`
- 
`QDateTime start = QDateTime::fromString(starttime, "yyyy-MM-dd hh:mm:ss");`
- 
`QDateTime end = QDateTime::fromString(endtime, "yyyy-MM-dd hh:mm:ss");`
- 
`uint stime = start.toTime_t();`
- 
`uint etime = end.toTime_t();`
- 
`qDebug() << "starttime" << starttime;`
- 
`qDebug() << "endtime" << endtime;`
- 
`qDebug() << "......................";`
- 
- 
`qDebug() << "etime" << etime;`
- 
`qDebug() << "stime" << stime;`
- 
`qDebug() << "etime - stime" << etime - stime;`
- 
- 
`int ndaysec = 24*60*60;`
- 
`qDebug() << "Day = " << (etime - stime)/(ndaysec) + ((etime - stime)%(ndaysec)+(ndaysec-1))/(ndaysec) - 1;`
- 
- 
`return a.exec();`
- 
`}`
结果如下：
![](https://img-blog.csdn.net/20171115113356004?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTgyODYwMzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
