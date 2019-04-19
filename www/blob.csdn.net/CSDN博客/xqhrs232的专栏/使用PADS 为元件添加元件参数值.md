# 使用PADS 为元件添加元件参数值 - xqhrs232的专栏 - CSDN博客
2014年04月13日 00:43:08[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：823
原文地址::[http://www.dzkf.cn/html/PCBjishu/2009/0821/3899.html](http://www.dzkf.cn/html/PCBjishu/2009/0821/3899.html)
相关文章
1、[怎么样将元件Value值显示在PCB上](http://blog.csdn.net/glowu/article/details/7239545)----[http://blog.csdn.net/glowu/article/details/7239545](http://blog.csdn.net/glowu/article/details/7239545)
 相信不少朋友都喜欢用ORCAD 绘制电路图，使用POWER PCB 绘制PCB 的工作经历。但是我们做出的PCB 在安装元件时，发现PCB 板上的元件没有元件值的参数，怎么办呢？传统的办法是一个个的加上去,元件少的板子，这样加还可以，但是要是元件很多呢？是不是增加了很大的工作量？HE…HE…为此我特做此教程给大家参考：
第一步：打开你要准备导出网表的原理图文件。
按图操作：
1，增加Value,注意用逗号分隔开。
2，选择你输出网表的格式。这里我们选Pads2k.dll.
3， 改写输出网表的扩展名为ASC 文件
![](http://www.dzkf.cn/upimg/userup/0908/210Z3435V8.jpg)
第二步：打开你POWER PCB 软件导入刚才ORCAD 生成的网络表
按图操作：
单击File 下的Import导入刚才ORCAD 生成的网络表文件
![](http://www.dzkf.cn/upimg/userup/0908/210Z432a14.jpg)
单击打开，完成网络表文件的导入
![](http://www.dzkf.cn/upimg/userup/0908/210Z500F41.jpg)
第三步：所有元件成功导入后，我们执行鼠标右键选择Select Components
如图所示：
![](http://www.dzkf.cn/upimg/userup/0908/210Z5412301.jpg)
第四步：执行快捷键Ctrl+A(全选)选择所有元件，再执行快捷键Ctrl+Q（查看/修改），便会出现左图所示的对话框。
![](http://www.dzkf.cn/upimg/userup/0908/210Z61OM7.jpg)
第五步：在出现在对话话框中我们选择Labels 选项卡，并从下拉列表中选择NEW。
如图所示：
![](http://www.dzkf.cn/upimg/userup/0908/210ZI3B41.jpg)
第六步：单击上图所示的图标，软件会进入标注状态。
第七步：经过一定时间的标注后，程序会弹出左图所示的对话框，在对话框中我们找到Attribu 并从它的下拉列表中选择Value然后单击OK，完成元件参数值的添加。
![](http://www.dzkf.cn/upimg/userup/0908/210ZTG3P.jpg)
呵呵…有些新手朋友可能还会问：为什么我的还是看不到元件的参数值呢？没关系，请往下看：
执行快捷键Ctrl+Alt+C 打开颜色设置对话框，查看你的颜色设置是否把元件参数给关闭了：
如图所示：
![](http://www.dzkf.cn/upimg/userup/0908/2109105UM1.jpg)
                                                                       注意：此选项若关闭，将不能显示元件的参数值。
E-mail:linchanggao2006@163.com
MSN:linchanggao@hotmial.com
本资料由林昌高原创，感谢你的观看。
