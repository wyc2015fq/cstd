# 国产InitPHP框架系列 - InitPHP3.8 发布 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年11月25日 16:55:37[initphp](https://me.csdn.net/initphp)阅读数：5457








InitPHP框架已经不知不觉写了三年了。发现今年自己也是蛮拼的，发了四次版本，框架也是越来越完善了。

做PHP框架的门槛其实不高，但是要去想透里面的每一个细节还是需要花很多心思的。

最近两年虽然一直在搞Java的项目，但是php一定是不能丢的。物联网，无线等各种应用的起来，PHP依然是做后端服务最佳的选择之一。

做这个框架也从来没有刻意去追求有多少人使用，也不追求能给我带来什么。只是能希望对于初学PHP或者正在使用PHP的开发人员有所帮助，哪怕是用到了我框架中的一个函数，也正是Init存在的价值。


以下是3.8版本的更新日志，感谢支持和帮助过InitPHP成长的朋友。

框架地址：http://initphp.com




```php
InitPHP V3.8版本更新：
1. 优化日志，自定义日志错误级别常量 ERROR  WARN  DEBUG  INFO。并且日志根据天来划分。
2. 新增框架Service的RPC的调用功能，使业务分割更加容易。
3. 解决框架coreInit中load全局加载方法类名可能会冲突的隐藏BUG
4. 新增InitPHP扩展库的静态调用方式：InitPHP::getLibrarys() 
5. 新增InitPHP工具库的静态调用方式：InitPHP::getUtil()
6. 新增return403 return405方法 
7. 优化ajax_return方法，支持jsonp类型  
8. InitPHP::cli_init() 命令行错误提示优化
9. 框架异常类exception 关闭调试不打印LOG的BUG修复
10. 新增InitPHP::log，静态方式直接可以使用工具库中的日志类
11. 注册全局变量方法
12. 异常和错误捕获并记录日志的功能优化（采用统一的日志记录方式）
感谢 @巴途Way @cloverphp @SK @西湖螃蟹 @Anran 等的建议和支持
```







