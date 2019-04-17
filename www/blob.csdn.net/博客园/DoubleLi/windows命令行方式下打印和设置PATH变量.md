# windows命令行方式下打印和设置PATH变量 - DoubleLi - 博客园






点击开始菜单，运行=》cmd
打印当前变量：echo %PATH%
结果：
C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;d:\PROGRA~1\ssh;D:\Program Files\tortoisesvn\bin;D:\Program Files\baofengyingyin\Codec;D:\Program Files\baofengyingyin

设置变量：
set PATH=C:\Program Files\EasyPHP5.3.0\php;%PATH%

再次打印：echo %PATH%
结果显示增加了新的变量：
C:\Program Files\EasyPHP5.3.0\php;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;d:\PROGRA~1\ssh;D:\Program Files\tortoisesvn\bin;D:\Program Files\baofengyingyin\Codec;D:\Program Files\baofengyingyin

以上为临时设置，如果要永久有效，用这样的步骤：
右键桌面的我的电脑=》属性=》高级=》环境变量=》系统变量=》PATH=》修改加入新的路径，重启计算机。









