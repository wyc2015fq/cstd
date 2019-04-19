# Tiled源码分析(二): 多文档支持 - 逍遥剑客 - CSDN博客
2014年01月17日 00:02:55[xoyojank](https://me.csdn.net/xoyojank)阅读数：2726
![](https://img-blog.csdn.net/20140116230533703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 文档模型
文档对象是一个MapDocument类, 它的主要功能有:
- 管理编辑操作的signal的派发, 保证一些UI的状态可以正确地更新
- 管理地图的数据模型, 如layer model(Qt MVC结构中的model, 可以绑定到控件)
- 增加删除地图对象的操作接口
- 管理这个地图的undo堆栈(后续专门分析一下undo/redo)
- 管理地图编辑时的选中状态
然后就是DocumentManager, 是一个单件类, 用于管理打开的所有MapDocument, 看看它都有些什么:
- 持有一个QTabWidget, 这个控件就是MainWindow的center widget, 所一直是最大化居中显示的
- 持有一个QUndoGroup, 因为每个MapDocument都有一个QUndoStack, 所以算是一个undo管理器(Qt这点很人性化, 常用功能都封装好了)
- 管理当前文档, 以及切换状态. 每当当前文档变化时, 都会发出一个currentDocumentChanged的signal
# 文档操作
不管什么编辑器, 针对文档的操作就这么几种, 看看所有编辑器工具栏上那万年不变的前几个图标就知道了.
## 新建
![](https://img-blog.csdn.net/20140116233225640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
先弹出NewMapDialog, 确定各种参数后就会生成一个MapDocument对象
让人意外的是MapDocument是NewMapDialog创建的, 而不是DocumentManager
估计这么做的原因是参数太多, 私有访问比较方便吧?
然后调用DocumentManager::addDocument把文档加入管理器, 同时创建显示需要的一些东西, 并绑定事件
## 打开
在读取之前, 会先查询一下插件管理器, 看看有没有可以读文件的插件
如果选择的文件已经打开, 则直接切换到那个文档, 不再打开新的
读完文件后就有了Map对象, 创建出MapDocument, 剩下的流程就跟新建一样addDocument就行了
## 关闭
关闭之前需要确认保存, 而需不需要保存则是看QUndoStack是不是空的(跟我之前YY的一样)
DocumentManager干的事就是删除Tab, 释放显示对象和MapDocument对象
## 保存
如果没有名字, 就换成"另存为"的操作流程
保存的时候同样也要扫描插件看扩展的格式writer
保存完的话就要把文档对应的QUndoStack标记空了, 这样就不会出现没有修改重复保存的情况
最后就把文件加入"recent file"
# 最近打开文件记录
![](https://img-blog.csdn.net/20140116235114250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这个打开记录是保存在QSettings里的, 在编辑器下次启动时也会还原打开的文件
QSettings是个之前没用过的东西, 可以保存各种选项, 而且还不用操作保存/读取, 挺实用的
# 总结
这么看下来多文档模型还是挺清晰的, 最重要的是管理好状态切换
