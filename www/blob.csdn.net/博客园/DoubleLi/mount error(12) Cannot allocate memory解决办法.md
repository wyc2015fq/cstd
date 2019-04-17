# mount error(12): Cannot allocate memory解决办法 - DoubleLi - 博客园






http://hi.baidu.com/zhangbin101004/item/e459f4d1f818dfbd33db903b




今天囧了啊，在ubuntu挂载的文件夹里面解压数据库，结果linux嫌太大挂掉了直接mount error(12): Cannot allocate memory折腾了好久，终于发现解决办法：



方法一：

------------------------------------------------------------------------------------------------------------------------------------------

http://blog.csdn.net/strategycn/article/details/7917082

当通过mount.cifs命令对windows下的文件进行映射时，若文件太大，便会产生这种错误，解决方法是：

   修改注册表 HKEY_LOCAL_MACHINE\System\CurrentControlSet\Services\LanmanServer\Parameters\IRPStackSize项的值大于等于15，若IRPStackSize项不存在，就新建一个DWORD值，点击弹出窗口的的进制为十进制，值写个18就ok了，还要重启一下。

-------------------------------------------------------------------------------------------------------------------------------------------






--------------------------------------------------------detail-----------------------------------------------------------------------------------

[http://linux.derkeiler.com/Newsgroups/comp.os.linux.networking/2006-10/msg00629.html](http://linux.derkeiler.com/Newsgroups/comp.os.linux.networking/2006-10/msg00629.html)

Check the Event Log on the Windows machine（
- 单击“开始”，然后单击“控制面板”。单击“性能和维护”，再单击“管理工具”，然后双击“计算机管理”。或者，打开包含事件查看器管理单元的 MMC。
- 在控制台树中，单击“事件查看器”。 

应用程序日志、安全日志和系统日志显示在“事件查看器”窗口中。

） that fails to allow it's share 
to be mounted under System. Look for a red X, and the word Error. The 
Source is Srv. The error will be:

The server's configuration parameter "irpstacksize" is too small for the 
server to use a local device. Please increase the value of this 
parameter.

If you have this error on the Windows machine that fails to have it's 
share mounted, then do the following...

This key in the system registry must be modifed or created:（这个运行regedit）

HKEY_LOCAL_MACHINE\System\CurrentControlSet\Services\LanmanServer
\Parameters\IRPStackSize

If the key is there, increase it a bit to say 15 or 18. If the key does 
not exist, as in my case, create a new DWORD Valute. Name it 
IRPStackSize. Double click on it to edit the value, put a check on the 
Decimal radio button for the Base so that you can see the parameter value 
as a decimal value and not a hexidecimal value. Give the new key a value 
of 15 and reboot. If that is not enough, raise it a litte to let's say 18 
and then reboot again. The problem is solved. Go mount your samba shares.



方法二

------------------------------------------------------------------------------------------------------------------------------------------

http://cplusplus2012.blog.163.com/blog/static/206676268201322054621392/



转载自： [http://blog.chinaunix.net/uid-9185047-id-2973258.html](http://blog.chinaunix.net/uid-9185047-id-2973258.html)

If you mount a Windows 7 share using Samba/CIFS you may run into “mount error(12): Cannot allocate memory” if you are using very large files on the Windows machine. Looks like in certain situations Windows needs to be told to run as a file server and to expect large files. You can read more details at [Large Files are locking up Windows 7 32 bit and 64 bit](http://social.technet.microsoft.com/Forums/en-US/w7itpronetworking/thread/d8f2c741-3ad0-4839-9cb3-ff1f11e8ec1a), but the solution is to make two registry edits and then restart a service:


Set “HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Memory Management\LargeSystemCache” to “1″.
Set “HKLM\SYSTEM\CurrentControlSet\Services\LanmanServer\Parameters\Size” to “3″.
Restart the “server” service.

Once you have done that you should be able to mount the share using a command like “sudo mount -a” or just reboot the Linux machine.




这个问题太纠结了，有时挂载成功了，解压或者编译什么的，有时遇到Cannot allocate memory，就看不到你mount的目录，然后重启ubuntu还是没有，重新手动挂载就报这个错。 有时又没有，说不准这个问题的出现原因。 看来这个才明白。

------------------------------------------------------------------------------------------------------------------------------------------

-------------------------------------------------details-----------------------------------------------------------------------------------------

还有其他办法没有试过：祝大家好运！

[http://jlcoady.net/windows/how-to-resolve-mount-error12-cannot-allocate-memory-windows-share](http://jlcoady.net/windows/how-to-resolve-mount-error12-cannot-allocate-memory-windows-share)

If you mount a Windows 7 share using Samba/CIFS you may run into “mount error(12): Cannot allocate memory” if you are using very large files on the Windows machine. Looks like in certain situations Windows needs to be told to run as a file server and to expect large files. You can read more details at [Large Files are locking up Windows 7 32 bit and 64 bit](http://social.technet.microsoft.com/Forums/en-US/w7itpronetworking/thread/d8f2c741-3ad0-4839-9cb3-ff1f11e8ec1a), but the solution is to make two registry edits and then restart a service:
- Set “HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Memory Management\LargeSystemCache” to “1″.
- Set “HKLM\SYSTEM\CurrentControlSet\Services\LanmanServer\Parameters\Size” to “3″.
- Restart the “server” service.

Once you have done that you should be able to mount the share using a command like “sudo mount -a” or just reboot the Linux machine.









