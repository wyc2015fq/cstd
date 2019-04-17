# Selenium3源码之common package篇 - zhusongziye的博客 - CSDN博客





2018年03月19日 20:34:18[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：229








### 简介

本文主要分析Selenium3源码common package下各模块和package的说明

common package作用是定义和实现了webdriver通用的基础能力，主要包含以下：
- 
键盘映射

- 
DesiredCapabilities

- 
webdriver基础service和Proxy

- 
html5支持

- 
移动端基础支持，例如各种动作、设备交互、定位方式等

- 
web端基础支持，例如各种动作、定位方式

- 
动作链构建（action chains）

- 
等等





### 组织

下面我们通过一张截图来看下common package下模块的组织方式：




![](https://img-blog.csdn.net/20180319203313817?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




说明：

红色框：表示common package整体的源码
蓝色框：表示对移动特定支持的各种actions的定义和实现
绿色框：表示对html5支持的定义和实现
黄色框：表示对web和移动端通用支持的定义和实现

下面我们对个模块基本能力进行说明

蓝色框中模块说明

> 
action_builder.py
实现了移动端动作的构建能力

input_device.py
描述用于可操作的输入设备

interaction.py
定义了交互f封装

key_actions.py
定义和实现移动端键盘动作行为

key_input.py
定义和实现移动端键盘动作的创建等能力

mouse_button.py
定义了鼠标按键的标识

pointer_actions.py
定义和实现屏幕点击等动作

pointer_input.py
定义和实现屏幕点击等基础能力


绿色框中模块说明

> 
application_cache.py
定义了html5 cache能力的支持


黄色框中模块说明

> 
action_chains.py
提供了系列底层的自动化交互的方法，主要包括：鼠标移动、鼠标按键动作、按键和上下文菜单交互、鼠标拖曳等等

alert.py
定义和实现了alert弹出的处理方法

by.py
定义了常用的定位方式,例如id、name、xpath等

desired_capabilities.py
定义和实现了不同浏览器的DesiredCapabilities，例如ie、firefox、edge、chrome等等

keys.py
定义了各键盘各功能键的code，例如F1、F2、shift等等这类的按键的code

proxy.py
定义和实现了代理，主要有http代理、ftp代理、ssl代理、sock代理等等

service.py
定义和实现连接、关闭等基础服务

touch_actions.py
定义和实现移动设备的一些基本操作，例如点击、双击、按住等等

utils.py
实现了一些辅助性质的能力





### 总结

common中主要定义和实现了通用的功能，是需要大家了解的，因为这些通用能力是可以大大提升你在自动化测试过程中的调试能力的。



