# adb error: device offline 的处理方法 - 定义域 - CSDN博客
2016年04月07日 13:15:48[定义域](https://me.csdn.net/u010278882)阅读数：1537标签：[adb](https://so.csdn.net/so/search/s.do?q=adb&t=blog)
个人分类：[工具](https://blog.csdn.net/u010278882/article/category/5802879)
adb error: device offline 的处理方法
按如下顺序进行，第一种方法不行，则仅需尝试下一种
- 重启 adb server 
adb kill-server 
adb start-server
- 如果通过 USB 调试, 可以尝试拔插设备
- 如果通过 USB 调试, 可以通过重新开启开发者设置中的开发者选项
