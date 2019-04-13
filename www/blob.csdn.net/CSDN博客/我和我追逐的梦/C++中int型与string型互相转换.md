
# C++中int型与string型互相转换 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月23日 15:53:18[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：4271


像是C\#,java等高级语言中,int转String类型都是很简单的,但是C++中比较麻烦：
代码来源：[http://gstarwd.iteye.com/blog/547591](http://gstarwd.iteye.com/blog/547591)
**int 转换 string**
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)stringint2str(intnum)
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)if(num==0)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)return"0";
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)stringstr="";
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)intnum_=num>0?num
 :-1*num;
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)while(num_)
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)str=(char)(num_%10+48)+str;
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)num_/=10;
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)}
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)if(num<0)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)str="-"+str;
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)returnstr;
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
**string 转换 int**
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)intstr2int(stringstr)
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)inti,len=str.size(),num=0;
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)i=0;
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)if(str[0]=='-')
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)i=1;
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)while(i<len)
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)num=num*10+(int)(str[i]-'0');
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)i++;
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)}
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)if(str[0]=='-')
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)num*=-1;
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)returnnum;
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}

