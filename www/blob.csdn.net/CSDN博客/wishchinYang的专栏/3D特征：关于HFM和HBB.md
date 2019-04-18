# 3D特征：关于HFM和HBB - wishchinYang的专栏 - CSDN博客
2013年09月10日 09:44:06[wishchin](https://me.csdn.net/wishchin)阅读数：773
**1.HBB    三维绑定框**
  (1): 要用到HBB,定义还不太清楚,来自于 VALVE Developer Community ([https://developer.valvesoftware.com/wiki/Bounding_box](https://developer.valvesoftware.com/wiki/Bounding_box))的 解释:
         A **Bounding Box** is an invisible box that defines the rough size of an entity. It's chiefly used to determine when the entity is visible and to perform[cheap](https://developer.valvesoftware.com/wiki/Cheap)[QPhysics](https://developer.valvesoftware.com/wiki/QPhysics)
 collision tests (often before moving on to more expensive ones: for example, bullets are only tested against[hitboxes](https://developer.valvesoftware.com/wiki/Hitbox) if they are first found to intersect with the
 given entity's bounding box).
          一个绑定框是比较好的方法，对于物体在空间的范围表示来说。绑定球体的方法可以直接用于（x，y，z）直接使用三维下标计算点云的方法，不过这种方法必须寻找到一种合适的数据结构，否则会面对占有极大的内存空间情况。    
## Rotation（绑定球体）
      Bounding boxes are always aligned to the world's axes. They never rotate. This is presumably because of their use for collision detection of players: it would be impossible to turn around in a tight corridor if the four corners of your bounding box
 rotated with you. Axis-aligned boxes are also far cheaper to compute.
      While this isn't a huge issue for objects that are roughly square from above, it makes oblong dimensions difficult to manage without switching to[VPhysics](https://developer.valvesoftware.com/wiki/VPhysics).
 When VPhysics isn't an option there isn't much that can be done: a [Day of Defeat: Source](https://developer.valvesoftware.com/wiki/Day_of_Defeat:_Source) player who has gone prone has an oblong shape that can rotate through a full 360°, which Valve could only accommodate for by creating a 'squished' bounding box that leaves the head and feet sticking out at all times (an image of this
 would be nice).
      关于HBB 的论文 我最先查到的是 2009-ICCV 的论文:   Multiple  Kernels for Object Detection ,具体使用方法 详见 论文内容....
**2. HFM 前景假设**
            (2): 关于HFM  前景假设掩模，使用假设前景的方法，用于获取全局特征
                   详见论文 Extracting Foreground Masks towards Object Recognition,
                   论文详细介绍了 提取方法 和使用过程..
                   类似于二维图像的检测框，不过好在三维图像特别容易分割，容易从HFM中区分出目标和背景。
