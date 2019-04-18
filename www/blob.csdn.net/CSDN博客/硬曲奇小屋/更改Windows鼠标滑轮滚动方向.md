# 更改Windows鼠标滑轮滚动方向 - 硬曲奇小屋 - CSDN博客





2017年10月22日 17:42:35[HardCookies](https://me.csdn.net/james_616)阅读数：5858








经常在Mac和Windows PC切换的人往往会遇到这样一个问题，那就是鼠标滑轮的方向恰好相反。改变Mac的鼠标滑轮滚动方向很简单，系统偏好设置—鼠标—滚动方向，把“自然”的对勾勾掉就好了。 


![这里写图片描述](https://img-blog.csdn.net/20171022172200606?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


那么，这里的意思就是Mac本身的滚动方式是自然的咯！不过习惯了之后确实觉得Mac上的滚动方式自然，为了保持同样的滚动方式，下面我们更改Windows下的滚动方式。 

我们首先需要知道鼠标设备的实例路径，计算机—管理—鼠标和其他指针设备，选择你的设备，右键属性—详细信息—设备实例路径。我们需要记下它的值，稍后会用到。 

![这里写图片描述](https://img-blog.csdn.net/20171022173252220?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


按住Windows-R，键入regedit，进入注册表编辑器。在计算机\HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Enum\xxx\xxx\xxx\Device Parameters，将FlipFlopWheel的值改为1。 


![这里写图片描述](https://img-blog.csdn.net/20171022174129489?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


其中\xxx\xxx\xxx就是之前的设备实例路径，改完之后插拔一下鼠标的USB接口就好了。




