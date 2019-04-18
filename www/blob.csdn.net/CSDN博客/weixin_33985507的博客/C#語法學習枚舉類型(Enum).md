# C#語法學習枚舉類型(Enum) - weixin_33985507的博客 - CSDN博客
2008年09月05日 14:15:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
```
/*枚舉
 * 當你想讓一組數字代表特定的意義,並且希望是安全的,可讀性強.
 * 
 * 枚舉類型是以"自己的方式使用整型"
 * 枚舉類型可以是:byte,sbyte,short,ushort,int,uint,long,ulong的子類
 * 如果沒有指定類型,則默認為int類型
 * 
 * 枚舉類型的大小是所有元索的大小的總和
 * 
 */
 using System;
 class Test
 {
     //enum Color{Red,Gree,Blue}
     //enum Color{Red=6,Gree=8,Blue=10};//也可以指定其值
     enum Color:byte {Red=6,Gree=8,Blue=10};//指定存儲類型
     private static string StringFromColor(Color color)
     {
         return color.ToString();
     }
     static void Main()
     {
         Color c= Color.Red;
         Console.WriteLine(c.ToString());
         Console.WriteLine(StringFromColor(c));
         int n=(int)Color.Red;
         Console.WriteLine(n);
     }
 }
```
