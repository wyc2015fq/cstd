# VR中物理的网络同步 - 逍遥剑客 - CSDN博客
2018年03月09日 10:47:35[xoyojank](https://me.csdn.net/xoyojank)阅读数：2416
参考: [Networked Physics in Virtual Reality: Networking a stack of cubes with Unity and PhysX](https://developer.oculus.com/blog/networked-physics-in-virtual-reality-networking-a-stack-of-cubes-with-unity-and-physx/)
之前做VR游戏时也是尝试了几种物理的同步方案, 最近看到Oculus Blog上也分享了一些, 经验, 做个笔记.
多人VR游戏中的物理交互, 有这么几个要求:
- 玩家对物体交互不能有延迟
- 物体能静止后稳定, 不能抖动
- 玩家把物件扔给另一个玩家交互, 不能有延迟
通常网络模型有三种:
- 帧同步
- C/S
- 分布式模拟
帧同步不适合做物理同步, 因为物理引擎一般不是确定性的, 而且有比较大的延迟 
C/S会有延迟和网络带宽的问题 
分布式模拟即谁交互谁有拥有控制权, 跟我们在游戏中使用的方式一样
- 交互者自己的角色相当于Server, 所以没有延迟的问题
- 一个物体同一时间只能受一个玩家控制, 直到释放权限
物理对象需要同步的数据有:
- 位置(vector3)
- 旋转(quaternion)
- 速度(vector3)
- 角速度(vector3)
如果要优化网络数据带宽:
- 静止时不用同步速度和角速度
- 旋转四元数可以使用smallest three representation
- 为每个物理设计优先级累加器, 根据优先级排序发送有限的数量
- 进行delta compression
相比于我们只同步交互状态的做法, 他们这种方式可以保证客户端的一致性, 但是会有更大的带宽需求, 还是比较适合网络条件较好的情况, 对于安全性也没有做校验. 
