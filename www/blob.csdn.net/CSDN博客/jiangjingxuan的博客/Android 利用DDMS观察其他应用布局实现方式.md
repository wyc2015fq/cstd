# Android 利用DDMS观察其他应用布局实现方式 - jiangjingxuan的博客 - CSDN博客





2017年03月28日 12:58:34[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：506









做电商项目的小伙伴，相信很多[安卓](http://lib.csdn.net/base/android)开发都很讨厌产品经理拿着淘宝或者京东告诉我们，实现跟他们一样的功能就可以了，可能很多人开始抱怨，他们团队多，技术厉害——–不过是实话，工作还是要进行的。

下面就教大家，如果出现上面的情况，如何快速模仿出别人的布局。

**1.在studio里面，选择tools->[Android](http://lib.csdn.net/base/android)->android device monitor,如下所示：**

![这里写图片描述](https://img-blog.csdn.net/20160506134005592)

**2.选中你连接的电脑的手机，然后选中Dump View Hierarchy for UI Automator,如下图所示：**

![这里写图片描述](https://img-blog.csdn.net/20160506134112531)

**3.在操作二结束后，会出现你当前手机的dmup，右侧就是当前页面的布局，鼠标点击任何view，在右侧都能体现。这样不管产品提怎么样的展示，都可以轻松搞定。**

![这里写图片描述](https://img-blog.csdn.net/20160506134255909)

是不是比反编译，然后在一堆layout里面找实现方便快捷。




