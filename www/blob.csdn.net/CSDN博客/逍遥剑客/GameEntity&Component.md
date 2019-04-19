# GameEntity&Component - 逍遥剑客 - CSDN博客
2012年04月12日 11:23:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：3094
- Update
- - Entity的不同功能的更新频率可以不一致, 比如渲染60FPS, 逻辑15FPS
- 更新频率可以做LOD处理, 如500米开外的更新频率可以设置为1FPS
- 更新可以多线程化, 但是目前很少有人实现, 主要是复杂度与性价比的问题
- Component
- - 优点
- - 可复用, 为数据驱动提供基础
- 节省资源, Entity只包含它所需要的功能, 省要不必要的CPU计算和内存占用
- 可以运行时动态增加/删除Component, Entity的功能可以灵活地改变
- 缺点
- - 粒度难于把握, 太大的话复用性降低, 太小的话管理它们的代价上升
- 依赖关系的处理, 需要避免循环依赖的现象
- Component之间的互相调用容易耦合, 可以使用消息机制
- 不同Component之间的更新次序需要提供保障
- Manager
- - 各个管理器之间可以使用智能指针保存Entity的引用
- 使用UniqueID的优点是安全, 缺点是有效率损失, 调试不方便
[The Game Entity – Part I, A Retrospect](http://altdevblogaday.com/2011/07/10/the-game-entity-%E2%80%93-part-i-a-retrospect/)
[](http://altdevblogaday.com/2011/07/10/the-game-entity-%E2%80%93-part-i-a-retrospect/)[The Game Entity – Part II, The Life Cycle and Processing Architecture](http://altdevblogaday.com/2011/07/25/the-game-entity-part-ii-the-life-cycle-and-processing-architecture/)
[](http://altdevblogaday.com/2011/07/25/the-game-entity-part-ii-the-life-cycle-and-processing-architecture/)[The Game Entity – Part III, Components](http://altdevblogaday.com/2011/08/10/the-game-entity-%E2%80%93-part-iii-components/)
[The Game Entity – Part IV, Game Systems](http://altdevblogaday.com/2011/08/24/the-game-entity-%E2%80%93-part-iv-game-systems/)
[The Game Entity – Part V, Future Ponderings](http://www.altdevblogaday.com/2011/09/23/the-game-entity-part-v-future-ponderings/)
