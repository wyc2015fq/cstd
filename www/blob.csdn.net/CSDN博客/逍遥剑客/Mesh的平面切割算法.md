# Mesh的平面切割算法 - 逍遥剑客 - CSDN博客
2017年01月05日 00:23:19[xoyojank](https://me.csdn.net/xoyojank)阅读数：7268
看了一下UKismetProceduralMeshLibrary::SliceProceduralMesh的代码实现, 发现也没想像中的复杂, 只要把网格/三角形/顶点/边的关系理清楚, 逐步分解问题就可以把复杂问题给简化成一个个的小问题, 然后各个击破. 
把注释和代码的步骤整理了一下, 变成了人话:
- 把切割面从World转换到Local空间
- 对于每个Section(SubMesh), 计算包围盒与切割面的关系 
- 如果在背面, 移动到另一半(新产生Mesh)里去
- 如果在正面, 保留不动
- 如果相交, 创建两个空的Section1和Section2 
- 对于每个顶点, 计算到切割面的距离 
- 如果顶点在正面, 添加Index到Set1里
- 如果顶点在背面, 添加Index到Set2里
- 对于每个三角形(三个index) 
- 如果三个顶点都在Set1里, 把三角形加到Section1
- 如果三个顶点都在Set2里, 把三角形加到Section2
- 如果三角形与切割面相交, 需要创建一两个新的三角形 
- 对于三角形的每条边 
- 如果起点在正面, 加到SliceSet1, 否则加到SliceSet2
- 如果终点跟起点不在一边 
- 求边与平面的交点, 生成一个新的顶点
- 把新的顶点分别加到两个Section里, 并更新包围盒
- 分别对两边的顶点(不超过4个)生成三角形, 加到对应的Section里
- 如果有两条边相交, 那么新生成的两个顶点会生成一条新边, 记录下来
- 如果Section里没有东西, 那就可以把这个Section舍弃
- 根据记录的新生成的边, 创建截面几何体 
- 把3D空间的边投影到切割面上, 变成2D空间的边
- 根据2D边的集合生成封闭的多边形
- 对多边形进行三角形化, 并生成UV
- 把生成的截面三角形分别生成两个新的Section, 并关联切割面的材质
- 针对碰撞体也做一遍类似的切割操作, 生成新的凸包碰撞体
- 对切下来的另一半, 生成新的MeshComponent
引擎的功能使用很简单, 见[https://www.youtube.com/watch?v=mG55ZWv3ezw](https://www.youtube.com/watch?v=mG55ZWv3ezw).  
但是了解了原理后, 可以自己实现StaticMesh和SkeletalMesh的切割, 比如做全场景任意切割效果, 断肢效果等 
向Metal Gear Rising致敬(虽然人家用的Havok-_-)
