# 【比赛纪实】BOP2014_CodeHunt （新手初见杀~） - CD's Coding - CSDN博客





2014年04月12日 16:50:15[糖果天王](https://me.csdn.net/okcd00)阅读数：1722标签：[编程之美																[c#																[codehunt](https://so.csdn.net/so/search/s.do?q=codehunt&t=blog)
个人分类：[Coding Competition](https://blog.csdn.net/okcd00/article/category/2133459)







编程之美今年多了一个叫做CodeHunt的平台，但是要用C#或者JAVA才能玩，作为一个比较水的Coder，Hihocoder里A了一道小数据之后就来这儿晃悠了。

然而，感觉相当有趣呢！ 就一边翻着MSDN，一边慢悠悠的写，一边发发微博，然后~居然被官微转载了哎~~

![](https://img-blog.csdn.net/20140412164539953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



**不知道到时会不会清除数据呢~ 这么有意义的事件一定要好好保留着呀~ 顺便作为以后C#的SCL用~就贴在这里吧~~~当然咯~ 比赛结束了再发布~ 先存盘~~~**

**疑问：Q^Q 这么赏心悦目的代码为什么不是3SP…… Level 02.04**

![](https://img-blog.csdn.net/20140415161551062)








CodeHunt All Source Code Here(Final All 3SP)

_______________________________________________________

2014.04.13 Noooooo……

CodeHunt更新了……增加了C# Switch To JAVA的功能……我昨天确实在吐槽这个啦……你更新了我确实也很高兴啦……




但是……但是我的代码你倒是别给我刷新了呀Q^Q

想存一下都没有尸骸了……

![](https://img-blog.csdn.net/20140413161742609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


那……C#的就放弃了Q^Q

JAVA的两道最后写出来的最难的俩Metal_3SP题目放这儿好了……






```java
//Metal-02.03 JAVA
public class Program {
	public static String firstLetterToUpper(String str){
		if(str == "")return "";
		String temp = "";
		temp += str.substring(0,1).toUpperCase() + str.substring(1);
		return temp;
	}
  public static String Puzzle(String s) {
	  
	String res = "";
	String[] arr = s.split(" ");
	res += firstLetterToUpper(arr[0]);
    for(int i=1; i<arr.length;i++){
		res += " " + firstLetterToUpper(arr[i]);
	}
	return res;
  }
}

//Metal-02.04 JAVA
public class Program 
{
	
  public static String Puzzle(String s, String p)
	{
		
	String ans="";
	
			ans+=s.charAt(0);
			for(int i=1;i<s.length();i++)
	
		{
			
		ans+= p + s.charAt(i);
		}
   
		 return ans;
  
	 }
}
```](https://so.csdn.net/so/search/s.do?q=c#&t=blog)](https://so.csdn.net/so/search/s.do?q=编程之美&t=blog)




