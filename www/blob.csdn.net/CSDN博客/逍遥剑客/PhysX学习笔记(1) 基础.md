# PhysX学习笔记(1): 基础 - 逍遥剑客 - CSDN博客
2009年07月22日 00:04:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：3515
简单形状
lNxBound3: AABB
lNxBox: OBB
lNxCapsule: 胶囊(线段+距离)
lNxPlane
lNxRay
lNxSegment: 线段
lNxSphere
SDK 初始化:
跟D3D差不多, 直接NxCreatePhysicsSDK就OK. 释放时不能delete, 因为那是DLL内部的对象, 只能release(). 多次创建返回的都是同一实例, 因为内部为单件实现.
对象实例:
一般都通过接口的create*(), 参数都用description类封装成一个. 注意返回值自己验证合法性. 释放时只能是谁创建了它谁释放, 不能自己释放自己. Down casting通过isXXX()这样的函数返回指针, 一个函数两用….
用户自定义数据:
物理对象都有一个公有的userData(void*类型)可以用来保存自定义数据. 另外, 每个物理对象都可以设置一个名字(const char*)做为标识. 可惜都是指针…这个地方极有可能变成野指针, 看来需要利用N3的Atom<T>.
状态保存:
通过对象的saveToDesc()/loadFromDesc()来保存/还原
参量:
l**rigid body transform**: 刚体变换
l**position vector**: 方位
l**rotation matrix**: 旋转矩阵(行列式必须为1)
l**unit quaternion**: 单位四元数, 表示旋转量
l**direction/extents vector**: 偏移/方向
l**force vector**: 力向量
l**torque vector**: 力矩
l**velocity vector**: 速度
l**angular velocity vector**: 角速度
l**momentum vector**: 动量
l**angular momentum vector**: 角动量
工具函数:
见NxUtilLib
