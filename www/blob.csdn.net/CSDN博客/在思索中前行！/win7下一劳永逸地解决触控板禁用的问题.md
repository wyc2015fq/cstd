# win7下一劳永逸地解决触控板禁用的问题 - 在思索中前行！ - CSDN博客





2014年07月03日 13:25:07[_Tham](https://me.csdn.net/txl16211)阅读数：2251









##           win7下一劳永逸地解决触控板禁用的问题——有鼠标时触摸板不再可用，

##                                          没鼠标时才启用










- 

下载Synaptics触摸板驱动并安装（去官网或驱动之家都可以）


→重启

- 

开始→运行→框内输入 regedit 点确定（进入注册表）→HKEY_CURRENT_USERSoftwareSynapticsSynTPEnh

- 



右键→新建DWORD（32位-值） disableintpdfeature 修改值为 3


右键→新建DWORD（32位-值） showdisableicon 修改值为 1

![](http://hiphotos.baidu.com/exp/pic/item/a0ca99d6277f9e2ffac4d37b1e30e924b999f396.jpg)


→重启

- 

开始→控制面板→硬件和声音→鼠标→装置设定值→


连接外部 USB 指向装置是禁用内部指向装置（选上勾）【此句意思为：插入USB鼠标自动禁用触摸板】


（未执行2,3步骤 红框内的内容不会显现）

![](http://hiphotos.baidu.com/exp/pic/item/dbf554ed2e738bd444a87c42a08b87d6267ff9fe.jpg)

- 

然后重启就OK了







