# Windows 8 安装之后怎样更改产品码 - weixin_33985507的博客 - CSDN博客
2012年08月31日 14:58:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
在Windows 8安装之后，需要激活。但在激活之前，需要更改产品码。操作介面找不到更改的地方，只有使用下面方法来实现了。
1. 用管理员权限运行命令提示。
2. 输入"slmgr.vbs -ipk <insert your product key here>” ，如 slmgr.vbs -ipk xxxxx-xxxxx-xxxxx-xxxxx-xxxxx
3. 更改产品Key激活Windows之后，重启电脑，电脑在进行系统时，会自动激活windows。 或者是运行"slmgr.vbs -ato"。
