# 关于g2o_viewer data/result_after.g2o使用过程中coredump、与lsd_slam依赖包libg2o冲突问题 - 站在巨人的肩膀上coding - CSDN博客





2018年04月12日 19:17:48[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：399








电脑上装的东西多了就很容引起版本或者依赖问题。。。

这不，按照高博教程做octomap实验时候运行g2o_viewer data/result_after.g2o时候就直接coredump。。。。

![](https://img-blog.csdn.net/2018041219104264)


回想起来自己ROS系统中装了libg2o，于是卸载之：sudo apt-get remove ros-indigo-libg2o

然后重新执行g2o_viewer data/result_after.g2o

![](https://img-blog.csdn.net/20180412190900860)


注意：既然把ROS下的libg2o卸载了自然会引某种神秘的问题或者埋下问题的种子【因为博主安装了lsd_slam，需要libg2o包】

卸载后编译lsd_slam会出错：

![](https://img-blog.csdn.net/20180412191601355)


所以用lsd_slam的时候在sudo apt-get install ros-indigo-libg2o

然后重新回到lsd_slam的工作空间执行：catkin_make




