# 【又见C#】CH常用战略回忆 _CodeHunt战场 @ 编程之美2015资格赛 - CD's Coding - CSDN博客





2015年04月20日 01:58:06[糖果天王](https://me.csdn.net/okcd00)阅读数：702








Code Hunt 那真的是相当好玩~~ Section2 HuangHe（黄河Section）12分钟秒杀的快感，那种第一次看到数据闹钟就灵光一闪然后敲完直接3分的感觉简直欲罢不能~！

赛时不敢写解题报告，比赛刚刚结束来写解题报告，结果……给锁住了不让看了……只好凭着记忆写一些给今后玩CH的自己，以及来看我博客的孩纸们耐心做个Hint了。




## 小脑筋篇

### 1. 给了也不用

身边的好多同学都卡在了哪题呢？ return a[a.length-1]; 这题上，给了个数组，要求返回一个值，但是数组千奇百怪一时半会儿还真没看出问题来，但是……这题已经很良心了哟，Input的数组最后一个元素和Output离得那么近，就是提醒你，Output的不是数组内各种东西都用上，而是只要最后一个元素的值哦~

除此之外印象中以前的CH也见过给三个数，只要用其中两个进行运算得到输出的题目存在。

所以，不要傻乎乎的被给的那么多数据骗了哦~![偷笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/titter.gif)

### 2. 常数去哪儿

常常会有一个常数或者符号阻碍着获得分数的道路，还记得当年的 return 4/x; 么？还记得这次的 return(x*x*x-2*x)/2; 么？问题是他们是咋弄出来的呢？确实灵光一闪很重要，但其实如果真的想不出来，有个技巧是这样的（我们叫它骗数据法，想不出来的大家肯定都这么用过），没错，就是if/else大法，多刷点数据出来，当你一大排的if/else排的整整齐齐的时候，总能发现，等会这里有点蹊跷，或者说，啊怎么每一行我都得加一个取非逻辑运算呀，对了~这个就是关键，朝着共性去想吧~~~

### 3.逻辑送分题

一旦看到几个True/False输入，一个True/False输出，就该开心送分来了哇！为啥呢？直接能看出问题的咱忽略不计，要知道逻辑学里有一种传说级别的东西叫做真值表，咱就算打表也能轻松获得与非、与或非式不是么？当然个人还是觉得这是最终手段，咱还是猜着好玩是最有趣的~

### 4.提示最亲切

不说比赛，平日里打CodeHunt那14个Section的一堆题的时候，经常会有绿色的提示哦，往往里面会告诉你，学会某种函数会相当有好处，不要犹豫快去学就好~




## 小函数篇

### 1. 获得子串位置 s.IndexOf(String s);



```
string s="this is a long string";
int k=s.IndexOf("long");
//k=10，索引从0开始，不包含则返回-1
```






### 2. 获得元素出现次数 Arr.Count(s=>s=="sth");


`int count = tmp.Count(s=>s=="example");`



### 3. 以子串开头/结尾 StartsWith(string value)/EndWith(string value);




`bool flag= s.StartWith("example");`





### 4. 连接数组，以某物做链接 s.Join(link_str,strArr);



```
//Join, 哈哈哈还记得那个aXbXcXdXe怎么都不给3SP么，就是它！
newStr="";
String[] strArr={strA,strB};
newStr=String.Join("^^",strArr);
Console.WriteLine(newStr);//"Hello^^World"
```




### 5.分隔字符串，以某物作分隔 public string[] Split(paramschar[] separator);





```
//Split
newStr="Hello^^World";
char[] separator={'^'};
String[] splitStrings=new String[100];
splitStrings=newStr.Split(separator);
inti=0;
while(i<splitStrings.Length)
{
<span style="white-space:pre">	</span>Console.WriteLine("item{0}:{1}",i,splitStrings[i]);
<span style="white-space:pre">	</span>i++;
}

//输出结果如下：Item0:Hello Item1:  Item2:World
```




### 6.插入填充字符串 str.Insert(pos,Insert_str);





```
//Insert
newStr="";
newStr=strA.Insert(1,strB);
Console.WriteLine(newStr);//"HWorldello"，与Remove用法一致
```




### 7. 进制转换





```
int d = 10;

//十进制转二进制字符串
 Console.WriteLine(Convert.ToString(d,2));
//输出: 1010

//十进制转十六进制字符串
Console.WriteLine(Convert.ToString(d,16));
//输出: a

//二进制字符串转十进制数
string bin = "1010";
Console.WriteLine(Convert.ToInt32(bin,2));
//输出: 10

//二进制字符串转十六进制数
string bin = "1010";
Console.WriteLine(string.Format("{0:x}",Convert.ToInt32(bin,2));
//输出: a

//十六进制转二进制字符串
Console.WriteLine(Convert.ToString(0xa,2));
//输出: 1010

//十六进制转十进制数
Console.WriteLine(Convert.ToString(0xa,10));
//输出: 10
```


```
int a = 188;  
this.textBox1.Text = "";  
this.textBox1.AppendText("a(10) = " + a.ToString() + "\n");  
this.textBox1.AppendText("a(16) = " + a.ToString("x") + "\n");  
this.textBox1.AppendText("a(16) = " + a.ToString("X") + "\n");  

//输出
a(10) = 188  
a(16) = bc  
a(16) = BC
```




### 8.二维数组定义





```
int[][] ret= new int[m][];
for(int i=0;i<m;i++)
{
 ret[i]=new int[n];
 for(int j=0;j<n;j++) ret[i][j]=(i+1)*(j+1);
}
return ret;
```
来个错误示例：（在这里错了无数次，几近放弃）



![](https://img-blog.csdn.net/20150420015302971?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20150420015110408?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)









写的有些困了，今天先到这里吧，发几个当时瞬秒的题出来供大家参(de)考(se)参(de)考(se)。

（最初一上来给的第一组数据为Attempt1，第一次提交时为Attempt2，所以这些尝试次数为2的代表了我看完第一次给的数据一次3SP通过~~~）

以下为手气好+头脑灵光时刻的成就：

## Sec2 / 3SP连续秒杀纪念

![](https://img-blog.csdn.net/20150420015717680?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20150420015722968?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20150420015608362?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20150420015614711?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)















