# C#之结构、枚举和运算符 - Big Smile - CSDN博客
2015年10月10日 10:00:26[王啸tr1912](https://me.csdn.net/tr1912)阅读数：1255
所属专栏：[C#学习基础](https://blog.csdn.net/column/details/csharp-wx.html)
# 一、结构
        说起来结构，那就像是在数据库里面的第一范式是一样的，为了让一个主要的属相的多样化有地放表示而特殊存在的从属关系。例如在数据库中员工的电话号码不具有唯一值的属性，还可以表示成手机号，家庭座机号，传真号等等，那么这样的数据在C#中就可以用结构来表示。
```
<span style="font-size:18px;">Struct Contact
{
     Public  string WorNumber;
     Public  string  Name;
     Public  string  Sex;
    <span style="color:#33ff33;"> </span><span style="color:#ff0000;">Public  struct  Phone_Number
    {
          Public  string  Mobile;
          Public  string  HomeNumber;
          Public  string  Fax;
     }</span>
     public  Phone_Number TelNumber
}  </span>
```
在调用的时候就可以写成为
```
<span style="font-size:18px;">public static  void Main(string[] args)
{
     Contact c1;
     c1.WorNumber="10";
     c1.Name="小明";
     c1.Sex="男";
     <span style="color:#ff0000;">c1.TelNumber.Mobile="13154625896";
     c1.TelNumber.HomeNumber="5412687";
     c1.TelNumber.Fax="7256841";</span>
     console.WriteLine(c1.TelNumber.HomeNumber);
     console.WriteLine(c1.TelNumber.HomeNumber);
     console.WriteLine(c1.TelNumber.Fax);
}</span>
```
这样就好像把各种电话号码归为了号码这个属性中了一样，这也是在划分层次结构，在结构的分层上面可以用到它。
# 二、枚举
        说起来枚举，就好像是一个老师在教同学们举例子，举了什么，就是什么。枚举在C#中常用于有限的情况的举例，比如事件的发生与否，一年哪几个月，一个星期都有哪几天等等。代码如下：
```
<span style="font-size:18px;">enum WeekDay
{
     Sunday,Monday,Tuesday,Wednesday,Thursday,Friday,Saturday
}</span>
```
列举的是一个星期的每一天，还可以这么写：
```
<span style="font-size:18px;">enum WeekDay
{
     Monday=1,Tuesday=2,Wendesday=3,Thursday=4,Friday=5,Saturday=6,Sunday=7
}</span>
```
这么写是给每个枚举都附上了值，默认是从0开始的，上面的例子就是自己设定从1开始的。同理调用的时候也是要用角码来代替内容的，如
```
<span style="font-size:18px;">public static Main()
{
      WeekDay w1;
      w1=(weekday) 1;
      console.WriteLine(w1);
}
//输出结果为Monday</span>
```
还可以有
```
w1=weekday.monday
console.writeline(w1);
```
结果是一样的。
# 三、运算符
        说起来运算符，相信学过C语言和C++的人都应该不陌生，基本上的运算都是继承而来的，像++  --等自增和自减都是一样的，唯一不一样的就是在运算的过程中可以用+= ,*=,-=,/=，%=等类似于x=x+5的运算，效果是一样的，但是写法还真的是不一样。涉及到二进制数的加，减，左移，右移的运算符还是需要特殊记一下的，如：
~：按位取反
<<：左移运算
>>：右移运算
&：与
|：或
^：异或
**还有布尔型的逻辑操作符：**
&&：逻辑与
||：逻辑或
！：逻辑非
()：强制转换
**特殊的运算符：**
IS：检查对象兼容
sizeof：堆栈中值类型长度
# 总结：
在这段时间的复习学习中，看到了好多的知识，也知道了自己的不足，出光明老师也说过，这些基本的知识是在以后的工作中最容易忘记的，老程序猿在这里也会摔跤，所以基础知识的掌控还是必须的。
