# eclipse修改默认工作空间 - 专注于数据挖掘算法研究和应用 - CSDN博客





2013年08月27日 16:15:58[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2516
个人分类：[Java](https://blog.csdn.net/fjssharpsword/article/category/6480440)









如果勾选了Use this as the default and do not ask again 下次要启动时不会再次显示修改工作空间的选择,

要改变工作空间的方法，有3中方法可以更改workspace的路径设置.

1. 启动Eclipse/MyEclipse后, 打开"Window -> Preferences -> General -> Workspace", 点Workspace页上的"Startup and Shutdown", 然后勾选"Startup and  Shutdown"页中的"Prompt for workspace on startup";

2. 用记事本打开"\eclipse\configuration\.settings\org.eclipse.ui.ide.prefs", 将"SHOW_WORKSPACE_SELECTION_DIALOG"的值改为"true";

P.S.: "RECENT_WORKSPACES"的值表示设置过的workspace绝对路径. 第一个路径是当前设定的路径, 向后依次之前曾设置过的. 各路径之间用"\n"分隔.

3. 删掉"\eclipse\configuration\.settings\org.eclipse.ui.ide.prefs".

执行上述操作后, 再次启动, 又会弹出"Workspace Launcher"对话框, 可以重新设置了.



