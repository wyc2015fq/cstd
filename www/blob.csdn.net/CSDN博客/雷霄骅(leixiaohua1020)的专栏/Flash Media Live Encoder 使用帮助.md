# Flash Media Live Encoder 使用帮助 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年11月24日 19:53:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：51
个人分类：[开源多媒体系统																[RTMP](https://blog.csdn.net/leixiaohua1020/article/category/8432890)](https://blog.csdn.net/leixiaohua1020/article/category/1362938)









翻译了一下Flash Media Live Encoder的帮助内容。

Flash Media Live Encoder包含两个工具：
- 
Flash Media Live Encoder GUI（图像化界面）

- 
Flash Media Live Encoder command-line（命令行）




**关于Flash MediaLive Encoder GUI**

预览窗口位于上方，包含了输入视频，输出视频，以及音量的预览。预览窗口的上方可以调节窗口的大小。在编码的过程中，如果您同时编码了多个流，您可以选择查看其中的一个流。

控制面板位于下方，用于设置编码选项，查看日志，以及开启/停止编码进程。

![](https://img-blog.csdn.net/20131124194614343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



**关于Flash MediaLive Encoder command-line**

在命令提示符窗口下，切换到Flash Media Live Encoder安装目录，可以使用如下格式命令：

FMLEcmd [/p [path\profile.xml] [/l [path\log_file] [/apuserID:password] [/ab userID:password] [/t dd:hh:mm] [/r] /d [/s fmle_uid] [/ufmle_uid] [/dvrstart[fmle_uid]| /dvrstop fmle_uid | /dvrstate fmle_uid]



参数含义如下
|**参数**|**描述**|
|----|----|
|/h|列出所有参数及其用法。|
|/p [*path*\*profile*.xml]|（可选）指定使用XML编码配置文件的路径和文件名。如果你不使用此参数，Flash Media Live Encoder使用的默认编码配置文件：startup.xml。|
|/g|启动 Flash Media Live Encoder GUI（图形化界面）。此选项不能和其他参数一起使用，除了/p.|
|/l|（可选）指定的编码的日志文件的路径和文件名。在日志文件中的设置将覆盖当前加载的编码配置文件中的设置。|
|/ap *userID*:*password*|（如果主服务器要求身份验证要求）指定认证的主服务器的用户ID和密码。由于密码是明文显示，请注意您密码的安全。|
|/ab *userID*:*password*|（如果备份服务器要求身份验证要求）指定认证为备份服务器的用户ID和密码。由于密码是明文显示，请注意您密码的安全。|
|/t *dd*:*hh*:*mm*|（可选）指定Flash Media Live Encoder于dd:hh:mm时间间隔后重新启动。|
|/r|（可选）注册编码会话。当系统因崩溃而关闭重启后，可以重新加载该会话。|
|/d|（推荐）验证编码配置文件，如果值是无效的，使用默认值取代之。|
|/s *fmle_uid*|停止标识符为*fmle_uid*的会话。如果*fmle_uid*是无效的或者是空的，则列出当前正在运行的所有会话。|
|/u *fmle_uid*|取消注册标识符为*fmle*_*uid*的会话。取消注册的会话将不会随着操作系统的启动而自动开始。如果fmle_uid是无效的或者是空的，则列出当前正在运行的所有会话。|
|/dvrstart [*fmle_uid*]/dvrstop *fmle_uid*/dvrstate *fmle_uid*|控制在发布流的时候录像功能。|





完整英文文档位于Flash Media Live Encoder安装目录中。

Flash Media Live Encoder下载：[http://download.csdn.net/detail/leixiaohua1020/6374021](http://download.csdn.net/detail/leixiaohua1020/6374021)









