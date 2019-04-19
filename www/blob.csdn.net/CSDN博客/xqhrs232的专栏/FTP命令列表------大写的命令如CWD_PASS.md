# FTP命令列表------大写的命令如CWD_PASS - xqhrs232的专栏 - CSDN博客
2017年01月20日 16:40:24[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：6891
原文地址::[http://m.blog.csdn.net/article/details?id=17450597](http://m.blog.csdn.net/article/details?id=17450597)
相关文章
1、linux下FTP客户端主动模式设置 
 ----[http://reeddeer.blog.163.com/blog/static/118171040201273113753561/](http://reeddeer.blog.163.com/blog/static/118171040201273113753561/)
2、**FTP命令详解（含操作实例）----[http://www.2cto.com/os/201506/405441.html](http://www.2cto.com/os/201506/405441.html)**
3、[FTP 命令模式下 PASV OR PORT](http://blog.csdn.net/loonghun/article/details/14005865)----[http://blog.csdn.net/loonghun/article/details/14005865](http://blog.csdn.net/loonghun/article/details/14005865)
4、[FTP的主动模式(PORT Mode)及被动模式(Passive Mode)](http://blog.csdn.net/changzhi1990/article/details/37562019)----[http://blog.csdn.net/changzhi1990/article/details/37562019](http://blog.csdn.net/changzhi1990/article/details/37562019)
### FTP命令列表
发表于2013/12/20 17:13:25  1741人阅读
分类： android
|命令|描述|
|----|----|
|ABOR|(ABORT)此命令使服务器终止前一个FTP服务命令以及任何相关数据传输。|
|ACCT|(ACCOUNT)此命令的参数部分使用一个Telnet字符串来指明用户的账户。|
|ADAT|(AUTHENTICATION/SECURITY DATA)认证/安全数据|
|ALLO|为接收一个文件分配足够的磁盘空间|
|APPE|增加|
|AUTH|认证/安全机制|
|CCC|清除命令通道|
|CDUP|改变到父目录|
|CONF|机密性保护命令|
|CWD|改变工作目录|
|DELE|删除文件|
|ENC|隐私保护通道|
|EPRT|为服务器指定要连接的扩展地址和端口|
|EPSV|进入扩展被动模式|
|FEAT|获得服务器支持的特性列表|
|HELP|如果指定了命令，返回命令使用文档；否则返回一个通用帮助文档|
|LANG|语言协商|
|LIST|如果指定了文件或目录，返回其信息；否则返回当前工作目录的信息|
|LPRT|为服务器指定要连接的长地址和端口|
|LPSV|进入长被动模式|
|MDTM|返回指定文件的最后修改时间|
|MIC|完整性保护命令|
|MKD|创建目录|
|MLSD|如果目录被命名，列出目录的内容|
|MLST|提供命令行指定的对象的数据|
|MODE|设定传输模式（流、块或压缩）|
|NLST|返回指定目录的文件名列表|
|NOOP|无操作（哑包；通常用来保活）|
|OPTS|为特性选择选项|
|PASS|认证密码|
|PASV|进入被动模式|
|PBSZ|保护缓冲大小|
|PORT|指定服务器要连接的地址和端口|
|PROT|数据通道保护级别|
|PWD|打印工作目录，返回主机的当前目录|
|QUIT|断开连接|
|REIN|重新初始化连接|
|REST|从指定点重新开始传输|
|RETR|传输文件副本|
|RMD|删除目录|
|RNFR|从...重命名|
|RNTO|重命名到...|
|SITE|发送站点特殊命令到远端服务器|
|SIZE|返回文件大小|
|SMNT|挂载文件结构|
|STAT|返回当前状态|
|STOR|接收数据并且在服务器站点保存为文件|
|STOU|唯一地保存文件|
|STRU|设定文件传输结构|
|SYST|返回系统类型|
|TYPE|设定传输模式（ASCII/二进制).|
|USER|认证用户名|
|XCUP|改变之当前工作目录的父目录|
|XMKD|创建目录|
|XPWD|打印当前工作目录|
|XRCP| |
|XRMD|删除目录|
|XRSQ| |
|XSEM|发送，否则邮件|
|XSEN|发送到终端|
