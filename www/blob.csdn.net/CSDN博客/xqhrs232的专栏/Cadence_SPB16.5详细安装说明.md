# Cadence_SPB16.5详细安装说明 - xqhrs232的专栏 - CSDN博客
2013年11月07日 10:46:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1938
原文地址::[http://hi.baidu.com/tekyanny/item/536f2d8a9383ad16c216276f](http://hi.baidu.com/tekyanny/item/536f2d8a9383ad16c216276f)
相关文章
1、**真正的cadence_16.5_破解方法----[http://www.docin.com/p-456722231.html](http://www.docin.com/p-456722231.html)**
2、[Cadence
 Allegro 16.5 详细破解步骤](http://blog.csdn.net/xqhrs232/article/details/6938844)----[http://blog.csdn.net/xqhrs232/article/details/6938844](http://blog.csdn.net/xqhrs232/article/details/6938844)

安装注意事项：
1、安装源文件不能放到有中文的目录下面；安装路径同样不能有中文的目录。下面路径都是不可以的，
例如：C:\Documents and Settings\Administrator\桌面\Cadence等；
目前SPB16.5支持有空格的目录和文件名称，但UNIX平台不支持。
但推荐安装在根目录下，如：C:\Cadence\SPB_16.5;
2、请关闭杀毒软件和360安全卫士等软件。
Allegro支持Windows 2008  R2服务器版; Windows XP   SP3专业版;Windows Vista的（32位和64位 ）；Windows 7（32和64位 ）（Home Premium，Professional，Enterprise和Ultimate）。
在win 7下安装必须是管理者权限。 
推荐使用4G内存，支持1280X1024分辨率的显卡。
Note: Allegro不支持  Windows NT (versions 4.0 及  earlier)、Windows 2000、Windows XP 
Home Edition、Windows Vista Home Basic。
Note: Allegro不支持  IPF chip ( Intel Itanium 64-bit )
具体的步骤：
1、下载SPB16.5下来后，点setup.exe，先安装第一项licensemanager；
1.1【Step 1】：点击License Manager；
![](http://b.hiphotos.bdimg.com/album/pic/item/cf1b9d16fdfaaf516a0c419f8e5494eef01f7a27.jpg)
1.2【Step 2】：点击next进入安装；
![](http://f.hiphotos.bdimg.com/album/pic/item/8b13632762d0f703a48956fc0afa513d2697c5d3.jpg)
1.3【Step 3】：同意合约说明，点击next；
![](http://d.hiphotos.bdimg.com/album/pic/item/e1fe9925bc315c609780a8cf8fb1cb13495477fd.jpg)
1.4【Step 4】：默认License管理器安装路径，或者修改安装路径，点击next；
![](http://a.hiphotos.bdimg.com/album/pic/item/9213b07eca8065387cd1673395dda144ad3482d3.jpg)
1.5【Step 5】：点击next；
![](http://b.hiphotos.bdimg.com/album/pic/item/b64543a98226cffc17c47f92bb014a90f603ea20.jpg)
1.6【Step 6】：点击Install；
![](http://b.hiphotos.bdimg.com/album/pic/item/8cb1cb13495409237452a05e9058d109b3de49d3.jpg)
1.7【Step 7】：选择license文件，指定license_xxx.txt文件。（我们给客户提供的license文件），点击next；
![](http://h.hiphotos.bdimg.com/album/pic/item/0b46f21fbe096b63b9f79e700e338744ebf8ac20.jpg)
单击cancel，然后点finish；
![](http://c.hiphotos.bdimg.com/album/pic/item/adaf2edda3cc7cd90ffff4f93b01213fb90e91c4.jpg)
2、安装第二项product。
2.1【Step 1】：点击Protect Installation；
![](http://f.hiphotos.bdimg.com/album/pic/item/0dd7912397dda144d8e7ac92b0b7d0a20cf48620.jpg)
注意：
在安装之前先在任务管理器中确认一下是否有这两个进程，有就结束掉，即
cdsNameServer.exe和cdsMsgServer.exe，没有就算了. 
‚在电脑开始菜单中的程序里找到cadence文件夹(windows7下)，点开再点开License Manager，点击LMTOOL：
![](http://g.hiphotos.bdimg.com/album/pic/item/34fae6cd7b899e51e15cfa3440a7d933c9950dc4.jpg)
关掉LMTOOL里面的cadence LIC服务（就是把服务都stop了）：
![](http://d.hiphotos.bdimg.com/album/pic/item/71cf3bc79f3df8dc1ad34fb1cf11728b47102820.jpg)
2.2【Step 2】：点击Next，进入产品安装；
![](http://c.hiphotos.bdimg.com/album/pic/item/c83d70cf3bc79f3d320f2410b8a1cd11728b2920.jpg)
2.3【Step 3】：同意合约要求，点击Next，进入产品安装；
![](http://b.hiphotos.bdimg.com/album/pic/item/8cb1cb13495409237453a05e9058d109b3de49dc.jpg)
2.4【Step 4】：选择全部安装还是部分安装；
![](http://f.hiphotos.bdimg.com/album/pic/item/dc54564e9258d1091ed96a92d358ccbf6c814ddc.jpg)
【Step 2.4-1】：如果选择Custom，需要选安装产品控制文件Control file(提供正版用户license文件同时，还有CONTROLFILE.TXT)，自动选择安装哪些产品。
若没有的话，清掉内容，按Next 键再手动自行选择要装的程序模块，后面选择的时候也可以全选，安装所有产品，但最后license会控制产品模块的使用。
【Step 2.4-2】：指定安装路径，5280@xxx指定license，xxx为license服务器的Host name，如果 license在本机，填写本机的Host name（机器名称），服务器名称不清楚，暂时可以空着，等产品安装完成后，手动配置，在这里不影响产品的安装。
Working Directory工作目录最好修改成安装目录下  \work（如果机器中安装多个cadence软件版本，可以通过版本切换，自动切换工作目录），如下图：
![](http://f.hiphotos.bdimg.com/album/pic/item/4afbfbedab64034f85f70126adc379310a551ddc.jpg)
【Step 2.4-3】：选择安装的产品模块，如果前面有控制安装文件，这一步软件会自动选择，如果没有，可手动选择我们需要安装的产品，例如：
![](http://c.hiphotos.bdimg.com/album/pic/item/f636afc379310a55b1111dd4b54543a9822610dc.jpg)
【Step 2.4-4】：如果选择Complete，全部安装
![](http://c.hiphotos.bdimg.com/album/pic/item/6f061d950a7b020851f8fcc160d9f2d3572cc8d9.jpg)
2.5【Step 5】：点击next进入，准备安装；
![](http://g.hiphotos.bdimg.com/album/pic/item/e824b899a9014c08480aeb85087b02087bf4f4d9.jpg)
2.6【Step 6】：点击Install进入安装产品，根据机器配置不同，安装时间15-90分钟不等，请耐心等待
![](http://h.hiphotos.bdimg.com/album/pic/item/faedab64034f78f008e25ad37b310a55b3191c21.jpg)
【Step 2.6-1】：安装过程中，添加防火墙列表步骤可能比较慢，请耐心等待。
![](http://d.hiphotos.bdimg.com/album/pic/item/83025aafa40f4bfb00395e74014f78f0f7361821.jpg)
2.7【Step7】：产品安装完成选Finish键结束，您可以选View Product Notes看相关产品说明
![](http://g.hiphotos.bdimg.com/album/pic/item/7acb0a46f21fbe0925b6033469600c338744ad22.jpg)
2.8【Step 8】：请等待安装界面自动消失
3、把安装目录下的SPB_16.5/tools/pspice目录下的orsimsetup.dll剪切出来找个地方先放着不理（待全部步骤完成后再拷回原来的地方，如果不用仿真部分删掉也无所谓）；
4、把pubkey、pubkey1.3.exe和lLicenseManagerPubkey.bat放到Cadence/LicenseManager目录下并运行lLicenseManagerPubkey.bat；
5、把破解文件夹crack里的pubkey、pubkey1.3.exe和ToolsPubkey.bat放到
Cadence/SPB_16.5/tools目录下并运行ToolsPubkey.bat。如果ToolsPubkey.bat运行的时候提示pubkey命令窗口出现任何can not open …… 或者是 diff pubkey等字样（出线的话，相应的工具就不能破解），[我安装时出现了can'tnot open pubkey1.3.exe read and write . 在后面也遇到了问题但解决后成功了]）；
6、将破解文件夹下的licens_gen文件夹整体复制到安装目录Cadence下。然后删除破解文件夹licens_gen下的license.lic，然后双击licgen.bat生成新的license.lic；LIC位置随便放，留意hostname必须要为计算机名；
7、在电脑开始菜单中的程序里找到cadence文件夹(windows7下)，点开再点开License Manager
7.1【Step 1】：运行License serversconfiguration Unilily：
![](http://g.hiphotos.bdimg.com/album/pic/item/d000baa1cd11728b891c4f02cafcc3cec3fd2cd9.jpg)
7.2【Step 2】：弹出的对话框中点browes：
![](http://a.hiphotos.bdimg.com/album/pic/item/9a504fc2d56285357508120192ef76c6a7ef6322.jpg)
7.3【Step 3】：指向刚才生成的license.lic打开它(open)；
![](http://g.hiphotos.bdimg.com/album/pic/item/91ef76c6a7efce1b0bc308eaad51f3deb48f6522.jpg)
![](http://f.hiphotos.bdimg.com/album/pic/item/279759ee3d6d55fb8ff6b5066f224f4a20a4ddda.jpg)
7.4【Step 4】：再点下一步(next)；
![](http://c.hiphotos.bdimg.com/album/pic/item/562c11dfa9ec8a13512a94c0f503918fa0ecc0da.jpg)
7.5【Step 5】：将主机名改成你的电脑名称(系统里的主机名)；
![](http://e.hiphotos.bdimg.com/album/pic/item/0df431adcbef7609373b5abf2cdda3cc7cd99eda.jpg)
7.6【Step 6】：点击next；
![](http://g.hiphotos.bdimg.com/album/pic/item/caef76094b36acaf50d055dc7ed98d1001e99cda.jpg)
7.7【Step 7】：License更新完成，点击Finish；
![](http://b.hiphotos.bdimg.com/album/pic/item/500fd9f9d72a60597cfb213a2a34349b033bba23.jpg)
8、在电脑开始菜单中的程序里找到cadence文件夹(windows7下)，点开再点开，运行License client configuration Unility，不用填什么，点下一步(next)，最后点finish。
9、使用License Server Configuration Utility指定新生成的license.lic并启动服务使用LMTOOLS 将自己的license.lic的目录改进到如图所示的地方。
![](http://h.hiphotos.bdimg.com/album/pic/item/95eef01f3a292df57ff36f35be315c6034a873da.jpg)
10、最后要开启第三步关闭的服务。可以看到左下角显示Serve start succesfully或者关闭时的 Stop succesfully !
![](http://b.hiphotos.bdimg.com/album/pic/item/3b87e950352ac65c79eea653f9f2b21193138a23.jpg)
![](http://f.hiphotos.bdimg.com/album/pic/item/0df431adcbef760936e059bf2cdda3cc7cd99e23.jpg)
注意：在安装完成后会出现OrCad Cpapture license was not found这时打开  环境变量（相信这个大家都会吧）；我们发现里面的CDS_LIC_FIL E的值是空的，呵呵问题就在这里！
![](http://h.hiphotos.bdimg.com/album/pic/item/5366d0160924ab187026221737fae6cd7b890bda.jpg)
![](http://g.hiphotos.bdimg.com/album/pic/item/dc54564e9258d1091d086992d358ccbf6c814d23.jpg)
![](http://b.hiphotos.bdimg.com/album/pic/item/42a98226cffc1e17838c4f114890f603738de9db.jpg)
自己手动将你的license.lic的目录加进去。确定！就OK了！
11、将第三步拷出来的orSimSetup.dll 在放在SPB_16.5/tools/pspice 文件夹下，不然会不能进行pSpice 的仿真功能。

//==========================================================================================================
备注::
1》开始老是报各种错误----找不到data6.cab/data7.cab等等，原来是被华为网盘摆了一道，本来200多MB的东西，但从华为网盘下只有170MB，太坑人了
2》后来是装了一半，再重新安装老是提示已经安装，删除注册表的相关项也还是装不上，现在不知道有什么办法来解决这个问题----重装系统？
3》装Cadence16.3又破解不了，真的是悲催啊！
