
# C\#新手经验点滴 - 西西吹雪 - 博客园
# [编程　&　生活      --       西西吹雪](https://www.cnblogs.com/watsonyin/)
开心工作，快乐生活  (原创文章，转载请注明出处)

### 导航
[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/watsonyin/)
[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
[联系](https://msg.cnblogs.com/send/%E8%A5%BF%E8%A5%BF%E5%90%B9%E9%9B%AA)[订阅](https://www.cnblogs.com/watsonyin/rss)![订阅](//www.cnblogs.com/images/xml.gif)[管理](https://i.cnblogs.com/)
统计
随笔 -		88文章 -		10评论 -		1793引用 -		18
公告

[C\#新手经验点滴](https://www.cnblogs.com/watsonyin/archive/2005/03/29/127679.html)
对于从其它语言转向C\#的朋友来说，C\#语言本身并不难。但对于一个C\#新手，可能还是有些东东用不惯，可能主要是因为很多以前在其它语言中能轻易找到或使用的东西，一时间不知如何使用。本文的目的，就是希望我的一点经验，对新上手的朋友有一点帮助。
一、VB中的一些函数
1、启用外部程序，相当于VB中的Shell
System.Diagnostics.Process.Start ();
2、Applicaiton.DoEvents()相当于VB中的Doevents
3、当前程序文件的路径：Application.ExecutablePath
二、对文件名的操作
(1)System.IO.Path.GetDirectoryName(FileName) 返回路经 。
如:GetDirectoryName("c:\test\tmp.txt") 返回 c:\test
(2)System.IO.Path.GetFileName(FileName) 返回不包含路的文件名。
如： GetFileName("c:\test\tmp.txt") 返回 tmp.txt
(3) System.IO.Path.GetExtension(FileName) 返回 后缀名。
如 ：GetExtension("c:\test\tmp.txt")返回.txt
三、格式化字符串
下面内容来自网上，作者不知。
可通过String.Format方法或通过Console.Write方法格式化数值结果，其中后一种方法调用String.Format。使用格式字符串指定格式。下表包含受支持的标准格式字符串。格式字符串采用的形式为Axx，其中A为“格式说明符”，而xx为“精度说明符”。格式说明符控制应用于数值的格式化类型，而精度说明符控制格式化输出的有效位数或小数位数。
字符说明示例输出|
C 或 c货币Console.Write({0:C}, 2.5);Console.Write({0:C}, -2.5);
$2.50($2.50)
|
D 或 d十进制Console.Write({0:D5}, 25);00025|
E 或 e科学型Console.Write({0:E}, 250000);2.500000E+005|
F 或 f固定点Console.Write({0:F2}, 25);Console.Write({0:F0}, 25);
25.0025
|
G 或 g常规Console.Write({0:G}, 2.5);2.5|
N 或 n数字Console.Write({0:N}, 2500000);2,500,000.00|
X 或 x十六进制Console.Write({0:X}, 250);Console.Write({0:X}, 0xffff);
FAFFFF
|

四、根据类名（字符串）创建类的对象实体
System.Type cls = System.Type.GetType("System.Windows.Forms.TextBox");
object obj = System.Activator.CreateInstance(cls);
这段代码很简单，但适当的时候使用，可以发挥巨大的威力。
五、Treeview控件的操作
.net中Treeview控件的设计更加科学，体现在以下两点：
1、节点集合在VB，VC或Delphi中，TreeView控件的节点集合表现出来只是列表，各节点通过一个key的对应关系来确字节点之间的关系，使用起来也相当麻烦。而在.net中，节点是真正以树的形式进行访问的。TreeView控件有一个Nodes属性，它是Node的集合，而Node对象包含一个Nodes属性，从而形成了一个“节点树”。下面遍历TreeView控件中的所有节点：
private void GetAllNode(System.Windows.Forms.TreeNodeCollection nodes)
{
foreach(System.Windows.Forms.TreeNode node in nodes)
{
Console.WriteLine(node.Text);
GetAllNode(node.nodes) //递归查找
}
}
真简单啊。
2、Tag属性
在使用TreeView的过程中，往往需要给节点更多的附加信息。在VB中，只有一个字符串类型的Tag属性可供使用，经常不够使用。在.net中，节点也有一个Tag属性，但它的类型是object，这意味着我们可以将任意一种对象实体赋给该属性，这无疑给我们的开发带来了极大的方便。（在Delphi 中，也提供了这个特性）。
六、IsNumeric函数
C\#中并未提供判断一个字符串是否为数值类型的函数，但可以利用正则表达式达到同样的目的。下面的函数供参考：
public static bool IsNumeric(string str)
{
if(str != null && Regex.IsMatch(str,@"^[-]?\d+[.]?\d*$"))
return true;
else
return false;
}
七、C\#中自定义事件
在C\#中为一个类自定义事件，看起来比较麻烦，实战起来是很简单的，只需经过以下三个步骤：
1、定义一个代理：
public delegate void MyEvent(object sender);
注意代理与要使用该事件的类是“平级”的，所以它不能放在类里面。
参数的类型和个数可以根据需要自行更改。
2、定义一个event
public event MyEvent MyEventHandler;
该event的定义属于类的成员
3、在需要激发事件的地方执行上面定义的event
if (MyEventHandler != null)
MyEventHandler(sender);
完成。是不是真的很简单啊？
八、取得目录下的所有文件
System.IO.Directory.GetFiles






posted on2005-03-29 10:04[西西吹雪](https://www.cnblogs.com/watsonyin/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=127679)[收藏](#)


[刷新评论](javascript:void(0);)[刷新页面](#)[返回顶部](#top)






Powered by:
博客园
Copyright © 西西吹雪
