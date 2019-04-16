# 转载和积累系列 - 解决zend Studio 代码无法提示 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年08月05日 17:30:35[initphp](https://me.csdn.net/initphp)阅读数：818
个人分类：[转载和积累系列](https://blog.csdn.net/initphp/article/category/2609089)









1. 找到zend的默认workspace，一般在新建php project的时候会有这个路径。例如我的：C:\Users\xxx\Zend\workspaces


2. 删除一个文件：C:\Users\xxx\Zend\workspaces\DefaultWorkspace7\.metadata\.plugins\org.eclipse.core.runtime\.settings\org.eclipse.dltk.ui.prefs

3. 重启，解决问题




