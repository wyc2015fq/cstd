# PhysX学习笔记(3): 动力学(2) Actor - 逍遥剑客 - CSDN博客
2009年07月23日 23:28:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：4199
Actor
Actor扮演两种角色: 静态对象, 动态刚体(也叫body). 
Actor包含shape. Shape之间相交会触发很多行为. 
Static actor主要作用是碰撞检测, 所以一般都会赋于对应的shape
Dynamic actor可能只表示一个抽象的连接点, 所以不需要对应一个shape
创建actor:
NxActorDesc actorDesc;
actorDesc.globalPose = ...;
gScene->createActor(actorDesc);
注意: 一些actor相关的资源在simulate()执行之前不会释放.
**Actor****的Shape:**
创建:
actorDesc.shapes.pushBack(&shapeDesc);// 1st way
actor->createShape(shapeDesc);// 2nd way
销毁用actor的releaseShape()一些限制:lStatic actor至少需要有一个shapel一个actor如果没有solid shape(没有shape或只有trigger shape), 那么它必须有一个body(dynamic的), 包括质量和惯性张量(inertia tensor)l包含solid shape的dynamic actor需要满足下列之一:n非零质量, 零密度, 没有惯性张量(会根据质量计算)n零质量, 非零密度, 没有惯性张量(会根据密度计算)n零密度, 但指定质量和惯性惯性张量
复合(compound)shape:
当多个shape指定给actor时就会形成一个复合shape. 它们作为一个整体参与物理模拟, 相互之间没有物理性能消耗. 
l添加一个新的shape到已有shape集合会创建一个新的复合shape再加入, 所以最好一次性创建.
l尽量避免static actor成为复合的, 因为每个actor的mesh三角形数是有限的, 多掉的会被忽略.
**参考系(reference frame):**
Actor最重要的属性是姿势(pose, 位置和朝向. 好像havok才支持缩放…). 
所有actor位于的空间称作世界(world)或全局空间(global space). 一个空间也叫参考系. 
Actor的重心是SDK自动计算的(当然也可以手动设置), 作为物体的旋转中心. 
通常这样获得渲染所需要的变换矩阵:
    float glmat[16];//OpenGL matrix.
    actor->getGlobalPose()->getColumnMajor44(glmat);
    glMultMatrixf(glmat);//Send to OpenGL.
注意:　设置朝向时使用四元数与矩阵没有性能差异．
**刚体(Rigid Body)****属性:**
|**线性分量**|**角度分量**|
|----|----|
|质量(mass), scalar|惯性(inertia), 质量分布|
|位置(position), 3-vector|朝向(orientation), quotation/3x3-matrix|
|速度(velocity), 3-vector|角速度(angular velocity), 3-vector, 方向代表转轴, 长度代表速度大小.|
|受力(force), 3-vector|力矩(torque), 表示方法同上|
**作用力和力矩**
中学学过:f = m*a (force = mass * acceleration)
NxActor的下列方法:
    void addForceAtPos(const NxVec3 & force, const NxVec3 & pos, NxForceMode);
    void addForceAtLocalPos(const NxVec3 & force, const NxrVec3 & pos, NxForceMode);
    void addLocalForceAtPos(const NxVec3 & force, const NxVec3 & pos, NxForceMode);
    void addLocalForceAtLocalPos(const NxrVec3& force, const NxVec3& pos, NxForceMode);
    void addForce(const NxVec3 &, NxForceMode);
    void addLocalForce(const NxVec3 &, NxForceMode);
    void addTorque(const NxVec3 &, NxForceMode);
    void addLocalTorque(const NxVec3 &, NxForceMode);
NxForceMode默认是NX_FORCE(简单的受力), 还有NX_IMPULSE(冲力)和NX_VELOCITY_CHANGE(忽略惯性, 直接改变速度). 
重力:
对于不需要重力控制的dynamic actor可以设置NxActor::raiseBodyFlag(NX_BF_DISABLE_GRAVITY).
**设置速度:**
不知道质量却需要与之相关的速度可以直接设置动量(momentum).    void setLinearVelocity(const NxVec3 &);
    void setAngularVelocity(const NxVec3 &);
    void setLinearMomentum(const NxVec3 &);
    void setAngularMomentum(const NxVec3 &);
注意: 不要让设置的速度超出float的上限, 不然会产生错误!
**睡眠(Sleep):**
睡眠后的物体不参与物理模拟, 直到有一个外部力作用于它. 
睡眠判定:
l简单睡眠: 速度低于NX_SLEEP_INTERVAL
l平均速度: 比较适合震动体, 设置NX_BF_FILTER_SLEEP_VEL标记开启
l基于动能: 这是默认的, 设置NX_BF_ENERGY_SLEEP_TEST标记开启
控制睡眠状态:
    void NxActor::wakeUp();
    void NxActor::putToSleep();
更改一些参数也会影响它, 如全局重力.
通过派生NxUserNotify可以实现睡眠状态改变时的事件处理回调. **活跃变换通知:**有大量交互对象时遍历取得变换信息显然不可取. 那么可以只取得活跃的(上一次模拟步进的结果). 开启:sceneDesc.flags |= NX_SF_ENABLE_ACTIVETRANSFORMS;获取:    NxActiveTransform *NxScene::getActiveTransforms(NxU32 &nbActiveTransforms);
    struct NxActiveTransform
    {
        NxActor* actor;
        void* userData;
        NxMat34 actor2World;
    };**Static actor:**没有dynamic actor的任何属性. 创建时把body设置为NULL就好. 创建好后不建议改变, 因这会打乱SDK为静态物体所做的优化. 需要运动的static actor, 应使用kinematic actor**Kinematic actor:**特殊的dynamic actor, 不受用户控制(力, 碰撞等). 创建: 先创建一个dynamic actor, 再设置成kinematic: NxActor::raiseBodyFlag(NX_BF_KINEMATIC).附加说明:lDynamic actor在受到一个kinematic和static(或两个kinematic)actor挤压时会被压进它们之中. lKinematic actor会与dynamic actor碰撞, 而不会与static actor发生关系. lKinematic actor每个次simulte移动的距离可能不一样, 因为速度是相对步进恒定的. 
