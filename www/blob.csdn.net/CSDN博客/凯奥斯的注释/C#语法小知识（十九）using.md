# C#语法小知识（十九）using - 凯奥斯的注释 - CSDN博客





2016年10月30日 22:45:13[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：545
所属专栏：[C#编程小知识](https://blog.csdn.net/column/details/12898.html)









# using关键字有两种用途：一种是作为指令。一种是作为语句。

using指令又有三种用途：

1、使用命名空间：


`using System.Collections.Generic;`
2、为命名空间或者类型创建别名：


`using StringMap = System.Collections.Generic.Dictionary<string,string>;`

3、允许访问类型的静态变量：（.Net4.0以上）



`using static System.Math;`
using语句用于定义一个范围，在范围末尾释放对象（IDisposable对象），例如：


```
using (var fs = new System.IO.FileStream ("1.txt", System.IO.FileMode.OpenOrCreate)) {
			//TODO
			fs.Close ();
		}
```





