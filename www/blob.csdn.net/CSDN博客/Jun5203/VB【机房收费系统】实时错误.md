# VB【机房收费系统】实时错误 - Jun5203 - CSDN博客
2018年09月09日 21:28:35[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：155
个人分类：[【机房收费系统】](https://blog.csdn.net/Ellen5203/article/category/7907465)
所属专栏：[机房收费系统](https://blog.csdn.net/column/details/26589.html)
###前言
在做机房的时候遇到了不少困难，但最后都迎刃而解，这都还得益于在做学生信息管理系统时自己做了充分的准备，不过机房与学生也有很多不一样的地方，而且自己通过做机房也对学生又加深了不少印象，下面就来看一下吧~
###分享
（一）实时错误‘3021’
![这里写图片描述](https://img-blog.csdn.net/20180909211606455?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180909211615281?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
出现这个错误的原因，是因为一开始我打算只来判断用户名是否正确，其中包含了用户名为空的情况，然后校验密码。以上报错证明，我的想法是错的，如果用户名存在，而密码输入错误或者是不输入，且没有相应的密码代码与之对应，那么这个流程就进入了一个死循环。但只要进了登录界面，不做卡号为空的情况却是可以的，这是为什么呢？有待考究~
（二）实时错误‘91’
![这里写图片描述](https://img-blog.csdn.net/20180909211557269?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
创建对象变量有两个步骤：
第一，必须先声明对象变量，然后必须用 Set 语句将一个正确的引用赋值给对象变量。
同样地，With…End With 块必须先用 With 语句进入点来执行初始化。
出现‘3021’和‘91’错误还有一个原因就是逻辑错误，所以建议大家在敲窗体之前把思路理清，画出流程图再进行代码实验。
（三）实时错误‘2147217873’
![这里写图片描述](https://img-blog.csdn.net/20180909211549168?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
解决方法：vb调试看哪个表出现问题，打开数据库——找到所对应的表——右键“设计”，将“允许Null值”打勾——保存
（四）实时错误’3265’
![这里写图片描述](https://img-blog.csdn.net/20180909211642775?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
错误原因：其中的某个字段在数据库中不存在，或是把字段名敲错
解决方法：检查代码
（五）实时错误‘-2147217885’
![这里写图片描述](https://img-blog.csdn.net/20180909211540104?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
解决方法：删除mrc.Delete语句（具体原因不明）
（六）[Microsoft][ODBC SQL Server Driver][SQL Server]在将 varchar 值 ‘sdf’ 转换成数据类型 int 时失败。
![这里写图片描述](https://img-blog.csdn.net/2018090921162547?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
1）启动SQL Server2014-工具菜单-选项；
2）设计器-表设计器和数据库设计器；
3）将右侧“阻止保存要求重新创建表的更改”前的对勾去掉即可
（七）实时错误‘3704’
![这里写图片描述](https://img-blog.csdn.net/20180909211634740?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
出现这个错误其实就是把关闭数据库的代码给放在了前面，而后边还有部分功能没有执行，所以就出现了这个错误，只要我们把他们给换个位置（后挪）即可
mrc.Close
mrc1.Close
###小结
做完机房收费系统我最大的感触就是“细节决定成败”，有时候你多加或少加一个字母都会导致你的程序报错，所以这个真的很考验人的细致水平，所谓“慢工出细活”大概说的就是这个道理吧！
