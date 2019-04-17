# Selenium3源码之common下action_chains.py模块分析 - zhusongziye的博客 - CSDN博客





2018年03月22日 20:59:21[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：140标签：[Selenium3源码之common																[common下action_chains.py模块分析](https://so.csdn.net/so/search/s.do?q=common下action_chains.py模块分析&t=blog)](https://so.csdn.net/so/search/s.do?q=Selenium3源码之common&t=blog)
个人分类：[Selenium自动化测试](https://blog.csdn.net/zhusongziye/article/category/7131148)








### 介绍

本文主要对action_chains.py模块的源码进行分析说明，其代码位置如图：




![](https://img-blog.csdn.net/20180322205851356?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




> 
在action_chains.py模块中定义和实现了类：ActionChains,主要实现鼠标移动，鼠标按键动作，按键和上下文菜单交互。
这对于执行更复杂的操作(如鼠标悬停、拖放)非常有用。


该模块源码是掌握selenium必须阅读和掌握的！！！




### 分类说明

下面我们通过该源码模块的结构大致了解下ActionChains类的成员：




![](https://img-blog.csdn.net/20180322205909137?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




下面我们通过一张表对上图中对外使用的成员进行分类（即带下划线的成员不放入下表）:
|分类|函数名|描述|
|----|----|----|
|鼠标操作|def click(self, on_element=None)|鼠标左键单击|
|鼠标操作|def click_and_hold(self, on_element=None)|鼠标左键按键，不释放|
|鼠标操作|def context_click(self, on_element=None)|鼠标右击|
|鼠标操作|def double_click(self, on_element=None)|鼠标左键双击|
|鼠标操作|def drag_and_drop(self, source, target)|鼠标拖曳动作,将指定元素位置拖曳到目标元素位置，来移动元素|
|鼠标操作|def drag_and_drop_by_offset(self, source, xoffset, yoffset)|鼠标拖曳动作，将指定元素按沿x、y方向拖曳指定距离来移动元素|
|鼠标操作|def move_by_offset(self, xoffset, yoffset)|将鼠标光标沿x、y方向移动指定距离，来实现鼠标的移动|
|鼠标操作|def move_to_element(self, to_element)|将鼠标光标移动到指定的元素上|
|鼠标操作|def move_to_element_with_offset(self, to_element, xoffset, yoffset)|将鼠标光标移动到指定的元素相对元素左上角位置偏移（xoffset、yoffset）的位置|
|鼠标操作|def release(self, on_element=None)|释放鼠标,与click_and_hold函数配对使用|
|键盘操作|def key_down(self, value, element=None)|按下指定的键盘按键|
|键盘操作|def key_up(self, value, element=None)|释放已按下的键盘按键（与key_down配套使用）|
|键盘操作|def send_keys(self, *keys_to_send)|键盘输入|
|键盘操作|def send_keys_to_element(self, element, *keys_to_send)|给指定元素（可输入元素）进行键盘输入|
|其他|def perform(self)|执行所有存储的动作|
|其他|def reset_actions(self)|清空所有存储的动作|
|其他|def pause(self, seconds)|在指定的时间内暂定所有的输入|




