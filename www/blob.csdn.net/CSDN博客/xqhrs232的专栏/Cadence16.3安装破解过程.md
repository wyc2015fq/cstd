# Cadence16.3安装破解过程 - xqhrs232的专栏 - CSDN博客
2013年11月08日 15:05:38[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：4736
原文地址::[http://blog.csdn.net/zjt289198457/article/details/6896754](http://blog.csdn.net/zjt289198457/article/details/6896754)
相关文章
1、Cadence16.3破解文件----[http://download.csdn.net/download/linatzw/4768696](http://download.csdn.net/download/linatzw/4768696)
2、cadence 16.3 之破解问题 RunMe!.bat打不开----[http://blog.sina.com.cn/s/blog_7aee30e80100sia3.html](http://blog.sina.com.cn/s/blog_7aee30e80100sia3.html)
3、Cadence软件下载网址----[http://www.orcad.net.cn/](http://www.orcad.net.cn/)
1.关闭杀毒软件。
2.运动Cadence_SPB_16.3\setup.exe然后点击“License Manager”，安装过程中询问license file时，选择取消。结束安装。
3.复制 Cadence_SPB_16.3\Aspirin\license_manager\cdslmd.exe到"C:\Cadence\LicenseManager\"（安装目录）,替代原文件。
4.建立环境变量 CDS_LIC_FILE=this_host @ 5280，，其中this_host为主机名
    环境变量添加修改方法：
       我的电脑\ 属性 \ 高级 \ 环境变量 \ 新建 ; 编辑之。
    计算机主机名获取方式：
   （方法1） 按下Windows徽标键+PauseBreak键后，点击"计算机名"选项卡，完整的计算机名称处的内容即为你的计算机主机名，复制其即可
              或者
    （方法2)  按下Windows徽标键+R键后打开运行对话框，输入"cmd"进入命令提示符窗口后直接输入"ipconfig /all"后，你会看到你的hostname
5.复制license163.lic到安装目录 （例如C:\Cadence\LicenseManager\ ）
6.打开license163.lic，编辑第1行，将this_host修改为主机名（见4中方法）
7.运行LicenseServerConfiguration.exe
8.选取license163.lic
9.端口填写5280
10.编辑window主计算机名，检查hostname是否正确。其它可不填。
11.重启
12.安装Cadence SPB/orCAD 选取默认值（记得选上对勾），时间会较长。
13.拷贝Aspirinpatch\ RunMe!.bat，Pattern,Skff三个文件到安装目录(例如：C:\Cadence\)
14.运行RunMe!.bat（运行\CMD中,见4中方法）,等待一会儿，结束后会显示多个文件中有部分改变。
15.完成。可以看到开始\所有程序\Cadence\Release 16.3\下有很多执行程序。
//============================================================================
备注::
1>按这个方法还是没有成功破解，打开EDA的时候老是报找不到License.真是悲催，装Cadence16.5第一遍没成功，再装第二遍老是报已经安装，删除注册表也不行，装Cadence16.3又破解不了，装这个软件真的装的火起啊！
2>今天乱整了一下，居然就成功了----设置一下环境变量，把系统变量的CDS_LIC_FILE变量删除，在Adiminstator的用户变量里面增加CDS_LIC_FILE变量，并定义其值为5280·pc_name,注意这个顺序，5280写前面，要不然就会出现
打开错误----Error(LMF-01001):License call failed.
3>再重新运行一下LicenseServerConfiguration.exe，指定你修改好的license163.lic文件就可以破解成功了！！！这个破解问题折腾了我好几天，今天才搞定。
4>网上还提到一个SHooTERS版本及其SHooTERS的破解方法，不过我没下到这个版本，这个破解方法也没试！
