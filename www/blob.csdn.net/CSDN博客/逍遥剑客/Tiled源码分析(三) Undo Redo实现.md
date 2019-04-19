# Tiled源码分析(三): Undo/Redo实现 - 逍遥剑客 - CSDN博客
2014年01月21日 00:15:14[xoyojank](https://me.csdn.net/xoyojank)阅读数：4840
就像[之前](http://blog.csdn.net/xoyojank/article/details/9391029)说过的, 没有Undo/Redo的编辑器都是耍流氓
# 优点
不过, Qt为我们提供了Undo/Redo的完整框架, 连工具栏按钮和History控件都齐活了
Command模式的核心当然是Command, 就是说所有操作都是QUndoCommand, push进一个QUndoStack就redo, pop一个就是undo
Tiled中比较特别是多文档编辑器, 也就意味着有多个QUndoStack, 所以比我之前写的编辑器多用了个QUndoGroup, 用于切换当前使用的stack
另外, QUndoView就是现成的操作记录控件, 真是省心...
![](https://img-blog.csdn.net/20140120234747093?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20140120235121390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20140121000031171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
无意中发现了beginMacro/endMacro的调用, 原来一个command序列可以一次性undo/redo, 所谓的:多次修改, 一次回退"
仔细看了下文档, 有一点跟我之前想的不太一样, 判断文件需不需要保存不是看QUndoStack是不是空的, 而是有一个clean state
# 缺点
由于push需要访问QUndoStack, 而QUndoStack又在MapDocument中, 所以到处都充斥着mMapDocument这样的引用, 所以setMapDocument这样的SLOT到处都是
这个设计我觉得有点冗余, 是可以改进的地方, 还不如直接访问DocumentManager来得简单, 统计由DocumentManager发出documentChanged类似的signal或许更简洁
Command模式比较烦人的就是要实现很多Command, Tiled中也不能免俗, 大约有40多个
![](https://img-blog.csdn.net/20140121001127312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
QUndoComand中对于对象的生命周期管理也是不太好的地方, 我的做法是使用引用计数和智能指针管理
