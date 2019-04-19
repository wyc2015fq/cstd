# Bullet中创建中心不在原点的碰撞体 - 逍遥剑客 - CSDN博客
2014年05月17日 20:55:25[xoyojank](https://me.csdn.net/xoyojank)阅读数：3382
个人分类：[动画物理](https://blog.csdn.net/xoyojank/article/category/569711)
从Havok换到Bullet后, 最大的不适应就是各种btCollisionShape默认是以中心为齐的
举个栗子:
Box只有extent, 没有center. 那么创建质心不在原点的Box碰撞体就会有种蛋蛋的忧伤
在生成Ragdoll时, 更奇葩的btCapsuleShape竟然中心也是在原点, 这让我如何把碰撞体对齐到Bone的LocalSpace啊? 这不仅是忧伤了, 还疼!
后来被逼成了使用btMultiSphereShape代替, 用两个蛋形定义Capsule的两端
今天回来重构时, [手贱google了一下](http://www.bulletphysics.org/Bullet/phpBB3/viewtopic.php?f=9&t=2209), 原来还有个btCompoundShape!  所以, 上面的问题都变成了:
```cpp
btCollisionShape* boxShape = new btBoxShape(btVector3(2.f,2.f, 2.0f));
   btCompoundShape* compound = new btCompoundShape();
   btTransform localTrans;
   localTrans.setIdentity();
   //localTrans effectively shifts the center of mass with respect to the chassis
   localTrans.setOrigin(btVector3(0,0,1));
   compound->addChildShape(localTrans,boxShape);
```
缺文档的苦逼开源库, 跟Havok里教材一样详细的文档没法比啊
怪不得PhysX成了市场占有率第一的物理引擎, 看来不是没有道理的
