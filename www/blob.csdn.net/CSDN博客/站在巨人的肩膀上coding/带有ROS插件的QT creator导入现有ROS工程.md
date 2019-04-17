# 带有ROS插件的QT creator导入现有ROS工程 - 站在巨人的肩膀上coding - CSDN博客





2018年04月24日 16:39:04[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：1433








        一般ROS工程没有XXX.workspace文件，无法以工程形式导入。但是我们可以通过新建一个ROS工程，并以与现有工程名相同的工程来打开。

第一步、打开装有ROS插件的QT creator


第二部、新建ROS工程

![](https://img-blog.csdn.net/20180424162836948)

第三步、设置工程名与现有工程名相同，点击next

![](https://img-blog.csdn.net/20180424163010781)

![](https://img-blog.csdn.net/20180424163051340)

点击finish。

第四步、打开后发现只有src源文件文件夹

![](https://img-blog.csdn.net/20180424163146454)

这时候我们打开工程工作空间配置文件，将仿照找到src目录的方式，添加其他需要的目录：

![](https://img-blog.csdn.net/20180424163252236)

修改后为：

![](https://img-blog.csdn.net/2018042416373698)

至此我们的额工程就导入成功了。






