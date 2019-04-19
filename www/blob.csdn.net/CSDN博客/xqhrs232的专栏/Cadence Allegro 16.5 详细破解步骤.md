# Cadence Allegro 16.5 详细破解步骤 - xqhrs232的专栏 - CSDN博客
2011年11月05日 17:05:47[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2880
原文地址::[http://blog.csdn.net/cs9874123/article/details/6456434](http://blog.csdn.net/cs9874123/article/details/6456434)
相关网帖
1、[Cadence Allegro SPB 16.5 完整下载 网盘（含破解）](http://blog.csdn.net/cs9874123/article/details/6456286)----[http://blog.csdn.net/cs9874123/article/details/6456286](http://blog.csdn.net/cs9874123/article/details/6456286)
2、[申精 cadence 16.5完整软件下载地址，已验证](http://blog.csdn.net/xqhrs232/article/details/6933923)----[http://www.eda365.com/thread-55960-1-1.html](http://www.eda365.com/thread-55960-1-1.html)
3、于博士信号完整性研究网----[http://www.sig007.com/](http://www.sig007.com/)-----------侧重于电源完整性与信号完整性分析，OrCAD/Cadance教程
4、[Cadence&OrCAD官方授权代理](http://www.google.com.hk/aclk?sa=l&ai=Cq5SY0Nx5UqbsGOzaigfmpYHIA6GU36YDyduqvja2pIUUCAAQAVD77ejB-f____8BYJ2h34HYBaABh9_n3QPIAQGpAkd-_E36A4Y-qgQlT9DXx3ESfEP3TUeisXEHRQKT5AO_VB1XAFFDB4RnalfFLQSkmIAH4aCYIpAHAw&sig=AOD64_3-iQLUdlfkRTQhcLpycIVElNWsRA&ved=0CCoQ0Qw&adurl=http://www.orcad.net.cn)----[http://www.orcad.net.cn/](http://www.orcad.net.cn/)-------提供有Cadence16.2/16.3/16.5/16.6各个版本的下载
5、电子发烧友论坛--------[http://bbs.elecfans.com/](http://bbs.elecfans.com/)
6、**真正的cadence_16.5_破解方法----[http://www.docin.com/p-456722231.html](http://www.docin.com/p-456722231.html)**

Cadence Allegro 16.5 详细破解步骤
**和谐文件：**
##### [Cadence Allegro 16.5 crack 修正 破解 方法 支持 windows 7](http://download.csdn.net/detail/cs9874123/3581683)
具体的步骤： 
1、下载SPB16.5下来后，点setup.exe,先安装第一项licensemanager，问license时，单击cancel，然后finish. 
2、接下来安装cadence的product,即第二项,直到安装结束这个时间有点长装过以前版本的人都知道. 
3、在任务管理器中确认一下是否有这两个进程,有就结束掉,即cdsNameServer.exe和cdsMsgServer.exe,没有就算了. 
4.把安装目录下的SPB_16.5/tools/pspice目录下的orsimsetup.dll剪切出来找个地方先放着不理（待第8步完成后再拷回原来的地方,如果不用仿真部分删掉也无所谓）。
5、把pubkey、pubkey1.3.exe和lLicenseManagerPubkey.bat放到Cadence/LicenseManager目录下并运行 
lLicenseManagerPubkey.bat 
6、把破解文件夹crack里的pubkey、pubkey1.3.exe和ToolsPubkey.bat放到Cadence/SPB_16.5/tools目录下并运行
ToolsPubkey.bat 
7、删除破解文件夹licens_gen下的license.lic,然后双击licgen.bat生成新的license.lic 
8.在电脑开始菜单中的程序里找到cadence文件夹(windows7下),点开再点开License Manager,运行License  servers 
configuration Unilily,弹出的对话框中点browes...指向刚才生成的license.lic打开 它(open)再点下一步 
(next),将主机名改成你的电脑名称(系统里的主机名)后点下一步按界面提示直 
到完成第7步. 
到此,破解完成. 
不必重启电脑就可运行程序(本人只在window7下装过) 
9、以上顺序不要搞反，直到第8便结束破解,无需重电脑就可以用了. 
以上根据rx-78gp02a写的改编.破解文件到他那去下载. 
以下两点仅供参考(完成上处8点后接着以下两条) 
1.在电脑开始菜单中的程序里找到cadence文件夹(windows7下),点开再点开,运行License client configuration Unility,不用填什么,点下一步(next),最后点finish,完成这第8步.
2.在电脑开始菜单中的程序里找到cadence文件夹(windows7下),点开再点开,运行Lm Tools,点Config Services项,Path to the license file项中,点Browes指向c:/License Manager/license.lic,打开它 (open)再点Save Service.到此,破解完成.不必重启电脑就可运行程序
//=================================================================================
备注:
 1＞要按crack说明.pdf中提到的那样安装licensemanager，而不应该像Cadence_SPB165_安装说明.pdf中提到的那样＂如果是客户端安装，不需要安装License Manager＂
因为不安装licensemanager就没法破解
